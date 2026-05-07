#include "subway_planner.h"

#include <stddef.h>

int subway_count_adjacent_hops(const struct route_result *result)
{
    if (result == NULL || !result->reachable || result->path_len < 2) {
        return 0;
    }
    return result->path_len - 1;
}

double subway_estimate_revenue(const struct route_result *result, int passenger_count)
{
    int hops = subway_count_adjacent_hops(result);
    if (hops <= 0 || passenger_count <= 0) {
        return 0.0;
    }
    return hops * 0.1 * passenger_count;
}

double subway_estimate_trip_operating_cost(const struct route_result *result, int passenger_count)
{
    int hops = subway_count_adjacent_hops(result);
    if (hops <= 0 || passenger_count <= 0) {
        return 0.0;
    }
    return hops * 0.4 * passenger_count;
}
