#include "subway_planner.h"

#include <stdio.h>

static bool valid_station_id(const struct subway_graph *graph, int station_id)
{
    return graph != NULL && station_id >= 0 && station_id < graph->station_count && graph->stations[station_id].active;
}

int subway_add_line(struct subway_graph *graph, const char *name)
{
    if (graph == NULL || name == NULL || graph->line_count >= MAX_LINES) {
        return -1;
    }

    const int id = graph->line_count;
    graph->lines[id].id = id;
    graph->lines[id].active = true;
    graph->line_count += 1;
    snprintf(graph->lines[id].name, MAX_NAME_LEN, "%s", name);
    return id;
}

bool subway_set_line_stations(struct subway_graph *graph, int line_id, const int *station_ids, int count)
{
    if (graph == NULL || station_ids == NULL || line_id < 0 || line_id >= graph->line_count || count <= 1 || count > MAX_STATIONS) {
        return false;
    }

    struct line *line = &graph->lines[line_id];
    if (!line->active) {
        return false;
    }

    line->station_count = 0;
    for (int i = 0; i < count; i++) {
        if (!valid_station_id(graph, station_ids[i])) {
            return false;
        }
        line->stations[line->station_count++] = station_ids[i];
    }
    return true;
}

bool subway_remove_line(struct subway_graph *graph, int line_id)
{
    if (graph == NULL || line_id < 0 || line_id >= graph->line_count || !graph->lines[line_id].active) {
        return false;
    }
    graph->lines[line_id].active = false;
    return true;
}

bool subway_set_line_fleet(struct subway_graph *graph, int line_id, int train_count)
{
    if (graph == NULL || line_id < 0 || line_id >= graph->line_count || !graph->lines[line_id].active || train_count <= 0) {
        return false;
    }
    graph->fleet_profiles[line_id].train_count = train_count;
    return true;
}

bool subway_add_operation_window(struct subway_graph *graph, int line_id, int start_minute_of_day, int end_minute_of_day,
                                 int headway_minutes)
{
    if (graph == NULL || line_id < 0 || line_id >= graph->line_count || !graph->lines[line_id].active ||
        start_minute_of_day < 0 || end_minute_of_day > 24 * 60 || start_minute_of_day >= end_minute_of_day ||
        headway_minutes <= 0) {
        return false;
    }

    struct line_fleet_profile *fleet = &graph->fleet_profiles[line_id];
    if (fleet->window_count >= MAX_TIME_WINDOWS) {
        return false;
    }

    fleet->windows[fleet->window_count++] = (struct line_operation_window){start_minute_of_day, end_minute_of_day, headway_minutes};
    return true;
}

bool subway_set_run_time(struct subway_graph *graph, int station_a, int station_b, int line_id, int minutes)
{
    if (!valid_station_id(graph, station_a) || !valid_station_id(graph, station_b) || station_a == station_b ||
        line_id < 0 || line_id >= graph->line_count || !graph->lines[line_id].active || minutes <= 0) {
        return false;
    }

    graph->edges[station_a][station_b] = (struct edge){station_b, minutes, line_id, false, true};
    graph->edges[station_b][station_a] = (struct edge){station_a, minutes, line_id, false, true};
    return true;
}
