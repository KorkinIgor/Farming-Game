#pragma once
#include "player.h"

// Константы карты
const int MAP_MIN_SIZE = 5;
const int MAP_MAX_SIZE = 30;

enum CellType {
    CELL_EMPTY = '.',
    CELL_WALL = '#',
    CELL_TREE = 'T',
    CELL_ROCK = 'R',
    CELL_PLAYER = '@',
    CELL_DOOR = 'D',
    CELL_EXIT = 'U',
    CELL_FRAGILE = ','
};


struct Map {
    char** grid;     
    bool** fragile_floor;  
    int width;              // cols
    int height;             // rows
    int floor_count;      
    int floor_reward = 50;
};

void move_menu();
void show_map(const Map* map);
bool checking_next_cell(const Map* map, int pos_y, int pos_x);
void trip_character(Player* player);