#include <stdio.h>

#include "subway_planner.h"

static int sample_strategy(int travel_minutes, int is_transfer)
{
    const int heavy_transfer_penalty = is_transfer ? 8 : 0;
    return travel_minutes + heavy_transfer_penalty;
}

int main(void)
{
    struct subway_graph graph;
    subway_init(&graph);
    subway_import_json(&graph, "subway_data.json");

    int s_tianhe = subway_add_station(&graph, "天河公园");
    int s_tiyu = subway_add_station(&graph, "体育西路");
    int s_zhujiang = subway_add_station(&graph, "珠江新城");
    int s_dongshankou = subway_add_station(&graph, "东山口");
    int s_yangji = subway_add_station(&graph, "杨箕");
    int s_gzta = subway_add_station(&graph, "广州塔");

    subway_set_station_demand(&graph, s_tianhe, DEMAND_POISSON, 120.0, 0.0, 0.0);
    subway_set_station_demand(&graph, s_tiyu, DEMAND_UNIFORM, 80.0, 140.0, 0.0);
    subway_set_station_demand(&graph, s_zhujiang, DEMAND_T_DISTRIBUTION, 100.0, 8.0, 0.0);

    int line_3 = subway_add_line(&graph, "3号线");
    int line_5 = subway_add_line(&graph, "5号线");
    int line3_stations[] = {s_tianhe, s_tiyu, s_zhujiang, s_gzta};
    int line5_stations[] = {s_dongshankou, s_yangji, s_zhujiang};

    subway_set_line_stations(&graph, line_3, line3_stations, 4);
    subway_set_line_stations(&graph, line_5, line5_stations, 3);
    subway_set_run_time(&graph, s_tianhe, s_tiyu, line_3, 4);
    subway_set_run_time(&graph, s_tiyu, s_zhujiang, line_3, 5);
    subway_set_run_time(&graph, s_zhujiang, s_gzta, line_3, 6);
    subway_set_run_time(&graph, s_dongshankou, s_yangji, line_5, 3);
    subway_set_run_time(&graph, s_yangji, s_zhujiang, line_5, 4);
    subway_add_transfer(&graph, s_tiyu, s_yangji, 5);

    subway_set_line_fleet(&graph, line_3, 20);
    subway_set_line_fleet(&graph, line_5, 16);
    subway_add_operation_window(&graph, line_3, 360, 540, 10);
    subway_add_operation_window(&graph, line_3, 540, 1260, 3);
    subway_add_operation_window(&graph, line_5, 360, 1260, 6);

    subway_print_map_grid(&graph, 3);
    subway_set_cost_profile(&graph, 200000.0, 900.0, 35.0, 120.0);

    printf("请输入起点终点编号（例如 0 5）和乘客数（例如 20）：\n");
    for (int i = 0; i < graph.station_count; i++) {
        printf("%d: %s\n", graph.stations[i].id, graph.stations[i].name);
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

    struct budget_report budget = subway_estimate_budget(&graph, 1.8);
    printf("今日估算运营成本: %.2f, 预算状态: %s\n", budget.estimated_operating_cost,
           budget.within_budget ? "预算内" : "超预算");

    subway_export_json(&graph, "subway_data.json");
    return 0;
}
