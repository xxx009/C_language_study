#include "subway_planner.h"

#include <stdio.h>

bool subway_export_json(const struct subway_graph *graph, const char *path)
{
    if (graph == NULL || path == NULL) {
        return false;
    }

    FILE *fp = fopen(path, "w");
    if (fp == NULL) {
        return false;
    }

    fprintf(fp, "{\n  \"stations\": [\n");
    for (int i = 0; i < graph->station_count; i++) {
        fprintf(fp, "    {\"id\":%d,\"name\":\"%s\",\"active\":%d}%s\n", graph->stations[i].id,
                graph->stations[i].name, graph->stations[i].active ? 1 : 0,
                i + 1 == graph->station_count ? "" : ",");
    }
    fprintf(fp, "  ],\n  \"lines\": [\n");
    for (int i = 0; i < graph->line_count; i++) {
        fprintf(fp, "    {\"id\":%d,\"name\":\"%s\",\"active\":%d}%s\n", graph->lines[i].id, graph->lines[i].name,
                graph->lines[i].active ? 1 : 0, i + 1 == graph->line_count ? "" : ",");
    }
    fprintf(fp, "  ]\n}\n");
    fclose(fp);
    return true;
}

bool subway_import_json(struct subway_graph *graph, const char *path)
{
    if (graph == NULL || path == NULL) {
        return false;
    }

    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        return false;
    }

    subway_init(graph);
    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL) {
        int id = 0;
        int active = 0;
        char name[MAX_NAME_LEN] = {0};

        if (sscanf(line, " {\"id\":%d,\"name\":\"%63[^\"]\",\"active\":%d}", &id, name, &active) == 3) {
            int sid = subway_add_station(graph, name);
            if (sid >= 0 && !active) {
                subway_remove_station(graph, sid);
            }
        }
    }

    fclose(fp);
    return true;
}

void subway_ui_maintenance(struct subway_graph *graph)
{
    if (graph == NULL) {
        return;
    }

    int running = 1;
    while (running) {
        int cmd = -1;
        printf("\n[UI] 1新增站点 2新增线路 3导出JSON 4退出\n");
        if (scanf("%d", &cmd) != 1) {
            return;
        }

        if (cmd == 1) {
            char name[MAX_NAME_LEN];
            printf("站点名: ");
            if (scanf("%63s", name) == 1) {
                subway_add_station(graph, name);
            }
        } else if (cmd == 2) {
            char name[MAX_NAME_LEN];
            printf("线路名: ");
            if (scanf("%63s", name) == 1) {
                subway_add_line(graph, name);
            }
        } else if (cmd == 3) {
            subway_export_json(graph, "subway_data.json");
            printf("已导出 subway_data.json\n");
        } else if (cmd == 4) {
            running = 0;
        }
    }
}
