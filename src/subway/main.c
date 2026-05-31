#include <stdio.h>

#include "subway_planner.h"

static int sample_strategy(int travel_minutes, int is_transfer)
{
    return travel_minutes + (is_transfer ? 8 : 0);
}

int main(void)
{
    struct subway_graph graph;
    subway_init(&graph);

    if (!subway_import_json(&graph, "subway_data.json")) {
        printf("导入 subway_data.json 失败。\n");
        return 1;
    }

    subway_print_map_grid(&graph, 4);
    subway_set_cost_profile(&graph, 200000.0, 900.0, 35.0, 120.0);

    printf("请输入起点终点编号（例如 0 8）和乘客数（例如 20）：\n");
    for (int i = 0; i < graph.station_count; i++) {
        if (graph.stations[i].active) {
            printf("%d: %s\n", graph.stations[i].id, graph.stations[i].name);
        }
    }

    int start = -1;
    int end = -1;
    int passenger_count = 1;
    if (scanf("%d %d %d", &start, &end, &passenger_count) != 3) {
        printf("输入无效。\n");
        return 1;
    }

    subway_set_cost_strategy(&graph, sample_strategy);
    struct route_result result = subway_plan_route(&graph, start, end);
    subway_print_route(&graph, &result);

    printf("本次行程收益估算: %.2f 元, 运营成本估算: %.2f 元\n",
           subway_estimate_revenue(&result, passenger_count),
           subway_estimate_trip_operating_cost(&result, passenger_count));
    return 0;
}
