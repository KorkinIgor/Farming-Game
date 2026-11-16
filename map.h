#pragma once
#include "player.h"

struct MapData {
    char** grid;
    bool** fragile_floor;
    int rows;
    int cols;
};

void move_menu();
void show_map(const MapData* map);
bool checking_next_cell(const MapData* map, int pos_y, int pos_x);
void trip_character(GameState* game);