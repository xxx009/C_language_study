#include "subway_planner.h"

#include <stddef.h>

static bool valid_station_id(const struct subway_graph *graph, int station_id)
{
    return graph != NULL && station_id >= 0 && station_id < graph->station_count && graph->stations[station_id].active;
}

int subway_add_transfer(struct subway_graph *graph, int station_a, int station_b, int transfer_minutes)
{
    if (graph == NULL || graph->transfer_count >= MAX_TRANSFERS || !valid_station_id(graph, station_a) ||
        !valid_station_id(graph, station_b) || station_a == station_b || transfer_minutes <= 0) {
        return -1;
    }

    int id = graph->transfer_count;
    graph->transfers[id] = (struct transfer){id, station_a, station_b, transfer_minutes, true};
    graph->transfer_count += 1;
    graph->edges[station_a][station_b] = (struct edge){station_b, transfer_minutes, -1, true, true};
    graph->edges[station_b][station_a] = (struct edge){station_a, transfer_minutes, -1, true, true};
    return id;
}

bool subway_remove_transfer(struct subway_graph *graph, int transfer_id)
{
    if (graph == NULL || transfer_id < 0 || transfer_id >= graph->transfer_count || !graph->transfers[transfer_id].active) {
        return false;
    }

    struct transfer *transfer = &graph->transfers[transfer_id];
    transfer->active = false;
    graph->edges[transfer->station_a][transfer->station_b].active = false;
    graph->edges[transfer->station_b][transfer->station_a].active = false;
    return true;
}

bool subway_set_cost_profile(struct subway_graph *graph, double budget_limit, double fixed_cost_per_train_per_day,
                             double variable_cost_per_train_km, double transfer_service_cost)
{
    if (graph == NULL || budget_limit <= 0 || fixed_cost_per_train_per_day < 0 || variable_cost_per_train_km < 0 ||
        transfer_service_cost < 0) {
        return false;
    }

    graph->costs = (struct cost_profile){budget_limit, fixed_cost_per_train_per_day, variable_cost_per_train_km,
                                         transfer_service_cost};
    return true;
}

struct budget_report subway_estimate_budget(const struct subway_graph *graph, double avg_segment_km)
{
    struct budget_report report = {0.0, false};
    if (graph == NULL || avg_segment_km <= 0) {
        return report;
    }

    double total = 0.0;
    for (int i = 0; i < graph->line_count; i++) {
        if (!graph->lines[i].active) {
            continue;
        }
        int train_count = graph->fleet_profiles[i].train_count;
        int segment_count = graph->lines[i].station_count > 1 ? graph->lines[i].station_count - 1 : 0;
        total += graph->costs.fixed_cost_per_train_per_day * train_count;
        total += graph->costs.variable_cost_per_train_km * avg_segment_km * segment_count * train_count;
    }

    total += graph->costs.transfer_service_cost * graph->transfer_count;
    report.estimated_operating_cost = total;
    report.within_budget = total <= graph->costs.budget_limit;
    return report;
}
