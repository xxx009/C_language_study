#include "subway_planner.h"

#include <limits.h>
#include <stdio.h>

static bool valid_station_id(const struct subway_graph *graph, int station_id)
{
    return graph != NULL && station_id >= 0 && station_id < graph->station_count && graph->stations[station_id].active;
}

static int default_cost(int travel_minutes, int is_transfer)
{
    const int transfer_penalty = is_transfer ? 3 : 0;
    return travel_minutes + transfer_penalty;
}

void subway_print_map_grid(const struct subway_graph *graph, int columns)
{
    if (graph == NULL || columns <= 0) {
        return;
    }

    printf("\n=== 地铁线路图（按网格展示）===\n");
    for (int i = 0; i < graph->line_count; i++) {
        if (!graph->lines[i].active) {
            continue;
        }

        printf("[%s]\n", graph->lines[i].name);
        for (int j = 0; j < graph->lines[i].station_count; j++) {
            int station_id = graph->lines[i].stations[j];
            if (valid_station_id(graph, station_id)) {
                printf("%-16s", graph->stations[station_id].name);
                if ((j + 1) % columns == 0 || j + 1 == graph->lines[i].station_count) {
                    printf("\n");
                }
            }
        }
        printf("\n");
    }
}

struct route_result subway_plan_route(const struct subway_graph *graph, int start_station, int end_station)
{
    struct route_result result = {0};
    if (!valid_station_id(graph, start_station) || !valid_station_id(graph, end_station)) {
        return result;
    }

    int dist[MAX_STATIONS];
    int actual_minutes[MAX_STATIONS];
    int prev[MAX_STATIONS];
    bool visited[MAX_STATIONS] = {false};

    for (int i = 0; i < graph->station_count; i++) {
        dist[i] = INT_MAX;
        actual_minutes[i] = INT_MAX;
        prev[i] = -1;
    }
    dist[start_station] = 0;
    actual_minutes[start_station] = 0;

    route_cost_fn cost_fn = graph->custom_cost == NULL ? default_cost : graph->custom_cost;
    for (int step = 0; step < graph->station_count; step++) {
        int current = -1;
        int min_dist = INT_MAX;

        for (int i = 0; i < graph->station_count; i++) {
            if (valid_station_id(graph, i) && !visited[i] && dist[i] < min_dist) {
                min_dist = dist[i];
                current = i;
            }
        }
        if (current == -1 || current == end_station) {
            break;
        }

        visited[current] = true;
        for (int next = 0; next < graph->station_count; next++) {
            const struct edge edge = graph->edges[current][next];
            if (!edge.active || !valid_station_id(graph, next)) {
                continue;
            }
            int weighted = cost_fn(edge.minutes, edge.is_transfer);
            if (dist[current] != INT_MAX && dist[current] + weighted < dist[next]) {
                dist[next] = dist[current] + weighted;
                actual_minutes[next] = actual_minutes[current] + edge.minutes;
                prev[next] = current;
            }
        }
    }

    if (dist[end_station] == INT_MAX) {
        return result;
    }

    int reverse_path[MAX_PATH_LEN];
    int count = 0;
    for (int at = end_station; at != -1 && count < MAX_PATH_LEN; at = prev[at]) {
        reverse_path[count++] = at;
    }

    result.reachable = true;
    result.total_minutes = actual_minutes[end_station];
    for (int i = count - 1; i >= 0; i--) {
        result.station_path[result.path_len++] = reverse_path[i];
    }

    for (int i = 0; i + 1 < result.path_len; i++) {
        int from = result.station_path[i];
        int to = result.station_path[i + 1];
        if (graph->edges[from][to].is_transfer) {
            result.transfer_count += 1;
        }
    }

    return result;
}

void subway_print_route(const struct subway_graph *graph, const struct route_result *result)
{
    if (graph == NULL || result == NULL || !result->reachable) {
        printf("未找到可达路径。\n");
        return;
    }

    printf("\n最优路径：\n");
    for (int i = 0; i < result->path_len; i++) {
        int station_id = result->station_path[i];
        printf("%s", graph->stations[station_id].name);
        if (i + 1 < result->path_len) {
            printf(" -> ");
        }
    }
    printf("\n预计耗时：%d 分钟\n", result->total_minutes);
    printf("换乘次数：%d 次\n", result->transfer_count);
}
