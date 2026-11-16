#pragma once
#include <cstdbool>

// Константы вместо магических чисел
const int MAX_INVENTORY = 10;
const int MAX_ITEM_NAME = 64;
const int MAX_NAME_LENGTH = 33;
const int HOURS_IN_DAY = 24;
const int MAX_ITEMS_COUNT = 10;

// Структура, хранящая всё состояние игры (никаких глобальных переменных)
struct GameState {
    int current_day;
    int current_hour;
    int inventory[MAX_INVENTORY];
    char index_item_array[MAX_ITEMS_COUNT][MAX_ITEM_NAME];
    char name_user[MAX_NAME_LENGTH];
    bool is_game;
};

// Объявления функций (теперь все принимают GameState*)
void init_game_state(GameState* game);
void record_file(GameState* game);
void inputFarmerName(GameState* game);
void write_diary(GameState* game);
void searh_item(GameState* game);
void show_time(GameState* game);
void work(GameState* game);
void show_inventory(GameState* game);
void change_item(GameState* game);
void clear_cell(GameState* game);
void search_unique_items(GameState* game);