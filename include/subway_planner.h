#ifndef C_LANGUAGE_STUDY_SUBWAY_PLANNER_H
#define C_LANGUAGE_STUDY_SUBWAY_PLANNER_H

#include <stdbool.h>

#define MAX_STATIONS 128
#define MAX_LINES 32
#define MAX_NAME_LEN 64
#define MAX_PATH_LEN 128
#define MAX_TRANSFERS 256
#define MAX_TIME_WINDOWS 8

typedef int (*route_cost_fn)(int travel_minutes, int is_transfer);

enum demand_distribution {
    DEMAND_UNIFORM = 0,
    DEMAND_POISSON = 1,
    DEMAND_T_DISTRIBUTION = 2
};

struct station {
    int id;
    char name[MAX_NAME_LEN];
    bool active;
};

struct line {
    int id;
    char name[MAX_NAME_LEN];
    int stations[MAX_STATIONS];
    int station_count;
    bool active;
};

struct transfer {
    int id;
    int station_a;
    int station_b;
    int minutes;
    bool active;
};

struct line_operation_window {
    int start_minute_of_day;
    int end_minute_of_day;
    int headway_minutes;
};

struct line_fleet_profile {
    int train_count;
    struct line_operation_window windows[MAX_TIME_WINDOWS];
    int window_count;
};

struct demand_profile {
    enum demand_distribution distribution;
    double p1;
    double p2;
    double p3;
};

struct cost_profile {
    double budget_limit;
    double fixed_cost_per_train_per_day;
    double variable_cost_per_train_km;
    double transfer_service_cost;
};

struct edge {
    int to;
    int minutes;
    int line_id;
    bool is_transfer;
    bool active;
};

struct route_result {
    int station_path[MAX_PATH_LEN];
    int path_len;
    int total_minutes;
    int transfer_count;
    bool reachable;
};

struct budget_report {
    double estimated_operating_cost;
    bool within_budget;
};

struct subway_graph {
    struct station stations[MAX_STATIONS];
    int station_count;

    struct line lines[MAX_LINES];
    int line_count;

    struct transfer transfers[MAX_TRANSFERS];
    int transfer_count;

    struct line_fleet_profile fleet_profiles[MAX_LINES];
    struct demand_profile station_demands[MAX_STATIONS];
    struct cost_profile costs;

    struct edge edges[MAX_STATIONS][MAX_STATIONS];
    route_cost_fn custom_cost;
};

void subway_init(struct subway_graph *graph);

int subway_add_station(struct subway_graph *graph, const char *name);
bool subway_remove_station(struct subway_graph *graph, int station_id);
bool subway_set_station_demand(struct subway_graph *graph, int station_id, enum demand_distribution distribution,
                               double p1, double p2, double p3);

int subway_add_line(struct subway_graph *graph, const char *name);
bool subway_set_line_stations(struct subway_graph *graph, int line_id, const int *station_ids, int count);
bool subway_remove_line(struct subway_graph *graph, int line_id);
bool subway_set_line_fleet(struct subway_graph *graph, int line_id, int train_count);
bool subway_add_operation_window(struct subway_graph *graph, int line_id, int start_minute_of_day, int end_minute_of_day,
                                 int headway_minutes);

bool subway_set_run_time(struct subway_graph *graph, int station_a, int station_b, int line_id, int minutes);
int subway_add_transfer(struct subway_graph *graph, int station_a, int station_b, int transfer_minutes);
bool subway_remove_transfer(struct subway_graph *graph, int transfer_id);

bool subway_set_cost_profile(struct subway_graph *graph, double budget_limit, double fixed_cost_per_train_per_day,
                             double variable_cost_per_train_km, double transfer_service_cost);
struct budget_report subway_estimate_budget(const struct subway_graph *graph, double avg_segment_km);

void subway_set_cost_strategy(struct subway_graph *graph, route_cost_fn custom_cost);

void subway_print_map_grid(const struct subway_graph *graph, int columns);
struct route_result subway_plan_route(const struct subway_graph *graph, int start_station, int end_station);
void subway_print_route(const struct subway_graph *graph, const struct route_result *result);

bool subway_import_json(struct subway_graph *graph, const char *path);
bool subway_export_json(const struct subway_graph *graph, const char *path);
void subway_ui_maintenance(struct subway_graph *graph);

int subway_count_adjacent_hops(const struct route_result *result);
double subway_estimate_revenue(const struct route_result *result, int passenger_count);
double subway_estimate_trip_operating_cost(const struct route_result *result, int passenger_count);

#endif
