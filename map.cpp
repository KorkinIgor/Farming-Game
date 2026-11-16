#define _CRT_SECURE_NO_WARNINGS
#include "map.h"
#include "utils.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstdio>

using namespace std;

// Константы для карты
const int MAP_MIN_SIZE = 5;
const int MAP_MAX_SIZE = 30;
const int CHANCE_EMPTY = 20;
const int CHANCE_FRAGILE = 11;
const int CHANCE_TREE = 11;

void move_menu() {
    printf("W - вверх \nS - вниз \nA - влево \nD - направо \nQ - домой \n");
}

void show_map(const MapData* map) {
    for (int i = 0; i < map->rows; ++i) {
        for (int j = 0; j < map->cols; ++j) {
            cout << map->grid[i][j] << " ";
        }
        cout << "\n";
    }
}

bool checking_next_cell(const MapData* map, int pos_y, int pos_x) {
    // Проверка на выход за границы
    if (pos_y < 0 || pos_y >= map->rows || pos_x < 0 || pos_x >= map->cols) return true;

    if (map->grid[pos_y][pos_x] == '.' || map->grid[pos_y][pos_x] == ',' || map->grid[pos_y][pos_x] == 'D' || map->grid[pos_y][pos_x] == 'U') {
        return false;
    }
    return true; // Стена или дерево
}

void trip_character(GameState* game) {
    bool is_fall = false; // Локальная переменная вместо глобальной
    bool in_trip = true;

    while (in_trip) {
        MapData map;
        map.rows = rand() % MAP_MAX_SIZE + MAP_MIN_SIZE;
        map.cols = rand() % MAP_MAX_SIZE + MAP_MIN_SIZE;

        // Выделение памяти (malloc)
        map.grid = (char**)malloc(map.rows * sizeof(char*));
        map.fragile_floor = (bool**)malloc(map.rows * sizeof(bool*));

        for (int i = 0; i < map.rows; ++i) {
            map.grid[i] = (char*)malloc(map.cols * sizeof(char));
            map.fragile_floor[i] = (bool*)malloc(map.cols * sizeof(bool));
            for (int j = 0; j < map.cols; j++) map.fragile_floor[i][j] = false;
        }

        for (int i = 0; i < map.rows; ++i) {
            for (int j = 0; j < map.cols; ++j) {
                if (i == 0 || i == map.rows - 1 || j == 0 || j == map.cols - 1) {
                    map.grid[i][j] = '#';
                }
                else {
                    int prob = rand() % 101;
                    if (prob > CHANCE_EMPTY) {
                        if (rand() % 101 < CHANCE_FRAGILE) map.fragile_floor[i][j] = true;
                        map.grid[i][j] = '.';
                    }
                    else {
                        map.grid[i][j] = (prob < CHANCE_TREE) ? 'T' : 'R';
                    }
                }
            }
        }

        int pos_x = rand() % (map.cols - 2) + 1;
        int pos_y = rand() % (map.rows - 2) + 1;
        int d_pos_x, d_pos_y, u_pos_x, u_pos_y;

        map.grid[pos_y][pos_x] = '@';

        do {
            d_pos_x = rand() % (map.cols - 2) + 1;
            d_pos_y = rand() % (map.rows - 2) + 1;
        } while (d_pos_y == pos_y && d_pos_x == pos_x);
        map.grid[d_pos_y][d_pos_x] = 'D';

        if (!is_fall) {
            do {
                u_pos_x = rand() % (map.cols - 2) + 1;
                u_pos_y = rand() % (map.rows - 2) + 1;
            } while (map.grid[u_pos_y][u_pos_x] == 'D' || map.grid[u_pos_y][u_pos_x] == '@');
            map.grid[u_pos_y][u_pos_x] = 'U';
        }

        char last_char = '.';
        show_map(&map);
        move_menu();

        bool end_sub_trip = false;
        while (!end_sub_trip) {
            char move_step;
            cout << "Ходите: ";
            cin >> move_step;
            if (cin.peek() != '\n' && cin.peek() != EOF) {
                cell_cleaning();
            }
            else {
                if (move_step == 'A' && !checking_next_cell(&map, pos_y, pos_x - 1)) {
                    map.grid[pos_y][pos_x] = last_char; last_char = map.grid[pos_y][pos_x - 1]; map.grid[pos_y][pos_x - 1] = '@'; pos_x--;
                }
                if (move_step == 'W' && !checking_next_cell(&map, pos_y - 1, pos_x)) {
                    map.grid[pos_y][pos_x] = last_char; last_char = map.grid[pos_y - 1][pos_x]; map.grid[pos_y - 1][pos_x] = '@'; pos_y--;
                }
                if (move_step == 'S' && !checking_next_cell(&map, pos_y + 1, pos_x)) {
                    map.grid[pos_y][pos_x] = last_char; last_char = map.grid[pos_y + 1][pos_x]; map.grid[pos_y + 1][pos_x] = '@'; pos_y++;
                }
                if (move_step == 'D' && !checking_next_cell(&map, pos_y, pos_x + 1)) {
                    map.grid[pos_y][pos_x] = last_char; last_char = map.grid[pos_y][pos_x + 1]; map.grid[pos_y][pos_x + 1] = '@'; pos_x++;
                }
                if (move_step == 'Q' && last_char == 'U') {
                    end_sub_trip = true; in_trip = false;
                }
            }

            if (last_char == ',') {
                if (rand() % 101 < 51) cout << "Пол скрипит! \n";
                else { is_fall = true; end_sub_trip = true; in_trip = true; }
            }
            if (last_char == '.' && map.fragile_floor[pos_y][pos_x]) {
                cout << "Осторожно, скрипучий пол! \n"; last_char = ',';
            }
            if (last_char == 'D') {
                is_fall = false; end_sub_trip = true; in_trip = true; // Пересоздаем карту
            }
            if (!end_sub_trip) show_map(&map);
        }

        // ОСВОБОЖДЕНИЕ ПАМЯТИ КАРТЫ
        printf("Память структуры карты по адресу %p освобождена\n", (void*)map.grid);
        for (int i = 0; i < map.rows; ++i) {
            free(map.grid[i]);
            free(map.fragile_floor[i]);
        }
        free(map.grid);
        free(map.fragile_floor);

        if (is_fall) in_trip = true; // Упали - генерируем заново
        else in_trip = false; // Вышли через U
    }
}