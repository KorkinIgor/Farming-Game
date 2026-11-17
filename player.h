#pragma once

const int MAX_INVENTORY = 10;
const int MAX_NAME_LENGTH = 33;
const int MAX_ITEMS_COUNT = 10;
const int MAX_ITEM_NAME = 64;
const int HOURS_IN_DAY = 24;

struct Player {
    char name[MAX_NAME_LENGTH];
    int current_day;
    int current_hour;

    int health;
    int max_health;
    int stamina;
    int max_stamina;
    int gold;

    int x;
    int y;

    int inventory[MAX_INVENTORY];
    char index_item_array[MAX_ITEMS_COUNT][MAX_ITEM_NAME];
};

// Объявления функций
void init_player(Player* player);
void record_file(Player* player);
void input_player_name(Player* player);
void write_diary(Player* player);
void search_item(Player* player);
void show_time(Player* player);
void work(Player* player);
void show_inventory(Player* player);
void change_item(Player* player);
void clear_cell(Player* player);
void search_unique_items(Player* player);
void create_or_reset_player(Player** player_ptr);