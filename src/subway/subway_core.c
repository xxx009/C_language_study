#include "subway_planner.h"

#include <stdio.h>
#include <string.h>

static bool valid_station_id(const struct subway_graph *graph, int station_id)
{
    return graph != NULL && station_id >= 0 && station_id < graph->station_count && graph->stations[station_id].active;
}

void subway_init(struct subway_graph *graph)
{
    if (graph == NULL) {
        return;
    }

    memset(graph, 0, sizeof(*graph));
    graph->custom_cost = NULL;
}

int subway_add_station(struct subway_graph *graph, const char *name)
{
    if (graph == NULL || name == NULL || graph->station_count >= MAX_STATIONS) {
        return -1;
    }

    const int id = graph->station_count;
    graph->stations[id].id = id;
    snprintf(graph->stations[id].name, MAX_NAME_LEN, "%s", name);
    graph->stations[id].active = true;
    graph->station_count += 1;
    return id;
}

bool subway_remove_station(struct subway_graph *graph, int station_id)
{
    if (!valid_station_id(graph, station_id)) {
        return false;
    }

    graph->stations[station_id].active = false;
    for (int i = 0; i < graph->station_count; i++) {
        graph->edges[station_id][i].active = false;
        graph->edges[i][station_id].active = false;
    }

    for (int i = 0; i < graph->transfer_count; i++) {
        if (!graph->transfers[i].active) {
            continue;
        }
        if (graph->transfers[i].station_a == station_id || graph->transfers[i].station_b == station_id) {
            graph->transfers[i].active = false;
        }
    }

    return true;
}

bool subway_set_station_demand(struct subway_graph *graph, int station_id, enum demand_distribution distribution,
                               double p1, double p2, double p3)
{
    if (!valid_station_id(graph, station_id)) {
        return false;
    }

    graph->station_demands[station_id] = (struct demand_profile){distribution, p1, p2, p3};
    return true;
}

void subway_set_cost_strategy(struct subway_graph *graph, route_cost_fn custom_cost)
{
    if (graph == NULL) {
        return;
    }

    graph->custom_cost = custom_cost;
}
