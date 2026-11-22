#define _CRT_SECURE_NO_WARNINGS
#include "map.h"
#include "utils.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstdio>

using namespace std;

void move_menu() {
    printf("\nW - вверх | S - вниз | A - влево | D - направо | Q - выйти (если стоите на U)\n");
}

void show_map(const Map* map) {
    for (int i = 0; i < map->height; ++i) {
        for (int j = 0; j < map->width; ++j) {
            cout << map->grid[i][j] << " ";
        }
        cout << "\n";
    }
}

bool checking_next_cell(const Map* map, int pos_y, int pos_x) {
    if (pos_y < 0 || pos_y >= map->height || pos_x < 0 || pos_x >= map->width) return true;

    char cell = map->grid[pos_y][pos_x];
    if (cell == CELL_EMPTY || cell == CELL_FRAGILE || cell == CELL_DOOR || cell == CELL_EXIT) {
        return false; 
    }
    return true;
}

void trip_character(Player* player) {
    bool in_trip = true;
    bool is_fall = false;

   
    Map* map = (Map*)malloc(sizeof(Map));

    map->floor_count = 1;
    map->floor_reward = 50;

    while (in_trip) {
        map->height = rand() % MAP_MAX_SIZE + MAP_MIN_SIZE;
        map->width = rand() % MAP_MAX_SIZE + MAP_MIN_SIZE;

        // Выделение памяти под сетку текущего этажа
        map->grid = (char**)malloc(map->height * sizeof(char*));
        map->fragile_floor = (bool**)malloc(map->height * sizeof(bool*));

        for (int i = 0; i < map->height; ++i) {
            map->grid[i] = (char*)malloc(map->width * sizeof(char));
            map->fragile_floor[i] = (bool*)malloc(map->width * sizeof(bool));
            for (int j = 0; j < map->width; j++) {
                map->fragile_floor[i][j] = false;
            }
        }


        for (int i = 0; i < map->height; ++i) {
            for (int j = 0; j < map->width; ++j) {
                if (i == 0 || i == map->height - 1 || j == 0 || j == map->width - 1) {
                    map->grid[i][j] = CELL_WALL;
                }
                else {
                    int prob = rand() % 101;
                    if (prob > 20) {
                        if (rand() % 101 < 11) map->fragile_floor[i][j] = true;
                        map->grid[i][j] = CELL_EMPTY;
                    }
                    else {
                        map->grid[i][j] = (prob < 11) ? CELL_TREE : CELL_ROCK;
                    }
                }
            }
        }


        player->x = rand() % (map->width - 2) + 1;
        player->y = rand() % (map->height - 2) + 1;
        map->grid[player->y][player->x] = CELL_PLAYER;

        int d_x, d_y;
        do {
            d_x = rand() % (map->width - 2) + 1;
            d_y = rand() % (map->height - 2) + 1;
        } while (d_y == player->y && d_x == player->x);
        map->grid[d_y][d_x] = CELL_DOOR;


        if (!is_fall) {
            int u_x, u_y;
            do {
                u_x = rand() % (map->width - 2) + 1;
                u_y = rand() % (map->height - 2) + 1;
            } while (map->grid[u_y][u_x] == CELL_DOOR || map->grid[u_y][u_x] == CELL_PLAYER);
            map->grid[u_y][u_x] = CELL_EXIT;
        }

        char last_char = CELL_EMPTY;
        show_map(map);
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
                int next_x = player->x;
                int next_y = player->y;

                if (move_step == 'A') { next_x--; }
                else if (move_step == 'W') { next_y--; }
                else if (move_step == 'S') { next_y++; }
                else if (move_step == 'D') { next_x++; }
                else if (move_step == 'Q' && last_char == CELL_EXIT) {
                    end_sub_trip = true;
                    in_trip = false;
                    printf("\nВы успешно покинули локацию! Пройдено этажей: %d\n", map->floor_count);
                }

                if (!checking_next_cell(map, next_y, next_x)) {
                    map->grid[player->y][player->x] = last_char;
                    last_char = map->grid[next_y][next_x];
                    map->grid[next_y][next_x] = CELL_PLAYER;
                    player->x = next_x;
                    player->y = next_y;
                    bool lose_stamina = true;

                    if (player->profession == scout) {
                        if (rand() % 100 < 50) {
                            lose_stamina = false;
                        }
                    }

                    if (lose_stamina) {
                        player->stamina--;
                    }
                }
            }

            // Логика хрупкого пола
            if (last_char == CELL_FRAGILE) {
                if (rand() % 101 < 51) cout << "Пол скрипит...\n";
                else {
                    is_fall = true;
                    end_sub_trip = true;
                    in_trip = true;
                    printf("Вы провалились! Этаж не засчитан.\n");
                }
            }
            if (last_char == CELL_EMPTY && map->fragile_floor[player->y][player->x]) {
                cout << "Осторожно, скрипучий пол!\n";
                last_char = CELL_FRAGILE;
            }

           
            if (last_char == CELL_DOOR) {
                map->floor_count++;
                player->current_hour += 1;
                player->stamina -= 5;

                
                int reward = map->floor_reward * map->floor_count;
                if (player->profession == banker) {
                    reward *= 2;
                    printf("Бонус банкира: награда удвоена!\n");
                }
                player->gold += reward;

                is_fall = false;
                end_sub_trip = true;
                in_trip = true;
                printf("\nВы спустились на этаж %d! Получено золота: %d\n", map->floor_count, reward);
            }

            // Экстренная эвакуация (если кончились силы или наступило 02:00)
            if (player->stamina <= 0 || player->current_hour == 2) {
                player->gold /= 2;
                std::cout << "Экстренная эвакуация, вы теряете половину золота: " << player->gold << "\n";
                end_sub_trip = true;
                in_trip = false;
            }

            // Вывод характеристик
            std::cout << "HP:" << player->health << "/" << player->max_health
                << "| Выносливость:" << player->stamina << "/" << player->max_stamina
                << "| Золото:" << player->gold
                << "| Этаж:" << map->floor_count
                << "| Время:" << player->current_hour << "\n";

            if (!end_sub_trip) show_map(map);
        }

        // ОСВОБОЖДЕНИЕ ПАМЯТИ СЕТКИ
        for (int i = 0; i < map->height; ++i) {
            free(map->grid[i]);
            free(map->fragile_floor[i]);
        }
        free(map->grid);
        free(map->fragile_floor);
    }

    // ОСВОБОЖДЕНИЕ САМОЙ СТРУКТУРЫ КАРТЫ
    printf("Память структуры карты по адресу %p освобождена\n", (void*)map);
    free(map);
}