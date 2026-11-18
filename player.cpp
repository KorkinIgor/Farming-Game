#define _CRT_SECURE_NO_WARNINGS
#include "player.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <cstdio>
#include <ctime>

using namespace std;

void init_player(Player* player) {
    player->current_day = 1;
    player->current_hour = 8;

    // Инициализация характеристик
    player->health = 100;
    player->max_health = 100;
    player->stamina = 50;
    player->max_stamina = 50;
    player->gold = 0;

    // Стартовый инвентарь
    int default_inv[MAX_INVENTORY] = { 1, 2, 2, 2, 3, 3, 4, 6, 0, 0 };
    for (int i = 0; i < MAX_INVENTORY; i++) {
        player->inventory[i] = default_inv[i];
    }
}

void record_file(Player* player) {
    const char* default_names[] = { "пусто", "дерево", "камень", "семена", "ножницы", "сено", "мотыга", "золото", "железо", "навоз" };

    std::ifstream file("items.txt");
    if (!file.is_open()) {
        printf("Файл items.txt не найден!\nЗагружены стандартные названия.\n");
        for (int i = 0; i < MAX_ITEMS_COUNT; ++i) {
            strncpy(player->index_item_array[i], default_names[i], MAX_ITEM_NAME - 1);
            player->index_item_array[i][MAX_ITEM_NAME - 1] = '\0';
        }
        return;
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        int id;
        std::string name;
        if (iss >> id >> name) {
            if (id >= 0 && id < MAX_ITEMS_COUNT) {
                strncpy(player->index_item_array[id], name.c_str(), MAX_ITEM_NAME - 1);
                player->index_item_array[id][MAX_ITEM_NAME - 1] = '\0';
            }
        }
    }
    file.close();
}

void input_player_name(Player* player) {
    printf("Укажите имя вашего фермера: ");
    fgets(player->name, sizeof(player->name), stdin);
    size_t size_name = strlen(player->name);

    if (size_name > 0 && player->name[size_name - 1] == '\n') {
        player->name[size_name - 1] = '\0'; // Убираем \n
        printf("Привет, %s!\n", player->name);
    }
    else {
        printf("Ошибка ввода имени. Попробуйте снова.\n");
        cell_cleaning();
        input_player_name(player);
    }
}

void write_diary(Player* player) {
    ofstream file("diary.txt", ios::app);
    file << "Игрок: " << player->name << " | День: " << player->current_day << " | Время: " << player->current_hour << ":00\n";
    file << "Здоровье: " << player->health << "/" << player->max_health << " | Золото: " << player->gold << "\n";
    for (int i = 0; i < MAX_INVENTORY; i++) {
        file << "Слот " << i << ": " << player->index_item_array[player->inventory[i]] << "\n";
    }
    printf("Записано в дневник!\n");
    file.close();
}

void search_item(Player* player) {
    cell_cleaning();
    char search_name[MAX_NAME_LENGTH];
    printf("Введите название предмета: ");
    fgets(search_name, sizeof(search_name), stdin);
    size_t len = strlen(search_name);
    if (len > 0 && search_name[len - 1] == '\n') search_name[len - 1] = '\0';

    int repeats = 0;
    for (int i = 0; i < MAX_INVENTORY; i++) {
        if (strcmp(search_name, player->index_item_array[player->inventory[i]]) == 0) {
            repeats++;
            printf("Найдено в слоте: %d\n", i);
        }
    }
    if (repeats == 0) printf("Такого предмета нет.\n");
}


void work(Player* player) {
    printf("Сколько часов вы хотите поработать?: ");
    int working_hours;
    if (scanf_s("%d", &working_hours) == 1 && working_hours > 0) {
        player->current_hour += working_hours;

        while (player->current_hour >= HOURS_IN_DAY) {
            player->current_day += 1;
            player->current_hour -= HOURS_IN_DAY;
        }
    }
    else {
        cell_cleaning();
    }
}

void show_character_card(Player* player) {
    time_t now = time(0);
    tm* localTime = localtime(&now);
    std::cout << player->name << "\n";
    printf("--- Инвентарь ---\n");
    for (int i = 0; i < MAX_INVENTORY; i++) {
        printf("Слот %d: %s\n", i, player->index_item_array[player->inventory[i]]);
    }
    printf("%02d.%02d.%d\n", localTime->tm_mday, localTime->tm_mon + 1, localTime->tm_year + 1900);
    printf("%02d:%02d:%02d\n", localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
}
void time_management(Player* player) {
    bool in_time_menu = true;

    while (in_time_menu) {
        printf("\n--- Управление временем ---\n");
        printf("1 - Посмотреть на часы\n");
        printf("2 - Пропустить время (Ждать)\n");
        printf("3 - Отдых (Лечь спать)\n");
        printf("0 - Вернуться в главное меню\n");
        printf("> ");

        int choice;
        if (scanf_s("%d", &choice) != 1) {
            cell_cleaning();
            continue;
        }

        switch (choice) {
        case 1: // ПОСМОТРЕТЬ НА ЧАСЫ
            printf("\nТекущее время -> День: %d, Время: %02d:00\n", player->current_day, player->current_hour);
            break;

        case 2: { // ПРОПУСТИТЬ ВРЕМЯ (ЖДАТЬ)
            printf("Сколько часов вы хотите пропустить? (от 1): ");
            int hours_to_skip;

            if (scanf_s("%d", &hours_to_skip) != 1 || hours_to_skip < 1) {
                printf("Ошибка: нужно ввести целое число больше 0.\n");
                cell_cleaning();
                break;
            }

            long long total_hours = (long long)player->current_day * HOURS_IN_DAY + player->current_hour;

            // Вычисляем, когда наступит ближайшие 02:00 ночи
            long long next_2am;
            if (player->current_hour < 2) {
                next_2am = (long long)player->current_day * HOURS_IN_DAY + 2;
            }
            else {
                // Если сейчас >= 02:00, то 02:00 наступит на следующий день
                next_2am = (long long)(player->current_day + 1) * HOURS_IN_DAY + 2;
            }

            long long target_total_hours = total_hours + hours_to_skip;

            // Проверка: не пересекаем ли мы границу 02:00
            if (target_total_hours > next_2am) {
                printf("Нельзя! Ночью фермер должен спать. До 02:00 ночи можно подождать максимум %lld ч.\n", next_2am - total_hours);
            }
            else {
                // Применяем новое время
                player->current_day = target_total_hours / HOURS_IN_DAY;
                player->current_hour = target_total_hours % HOURS_IN_DAY;
                printf("Вы подождали %d ч. Текущее время -> День: %d, Время: %02d:00\n",
                    hours_to_skip, player->current_day, player->current_hour);

                // Если дошли ровно до 02:00, намекаем, что пора спать
                if (player->current_hour == 2) {
                    printf("Уже 2 часа ночи! Пора ложиться спать.\n");
                }
            }
            break;
        }

        case 3: // ОТДЫХ (ЛЕЧЬ СПАТЬ)
            player->current_day++;       // Переход на следующий день
            player->current_hour = 8;    // Время 08:00 утра

            // Восстановление характеристик на 100%
            player->health = player->max_health;
            player->stamina = player->max_stamina;

            printf("\nВы отлично выспались! Наступил День: %d, Время: 08:00\n", player->current_day);
            printf("Здоровье и выносливость полностью восстановлены.\n");
            break;

        case 0: // ВЫХОД В ГЛАВНОЕ МЕНЮ
            in_time_menu = false;
            break;

        default:
            printf("Такого пункта нет.\n");
            break;
        }
    }
}

void change_item(Player* player) {
    printf("Укажите индекс слота (0-%d): ", MAX_INVENTORY - 1);
    int slot_index, item_index;
    if (scanf_s("%d", &slot_index) == 1) {
        printf("Укажите индекс нового предмета (0-%d): ", MAX_ITEMS_COUNT - 1);
        if (scanf_s("%d", &item_index) == 1) {
            if ((slot_index >= 0 && slot_index < MAX_INVENTORY) && (item_index >= 0 && item_index < MAX_ITEMS_COUNT)) {
                player->inventory[slot_index] = item_index;
                printf("Предмет заменен.\n");
            }
            else {
                printf("Некорректный индекс.\n");
            }
        }
        else cell_cleaning();
    }
    else cell_cleaning();
}

void clear_cell(Player* player) {
    printf("Укажите номер слота для очистки (0-%d): ", MAX_INVENTORY - 1);
    int slot;
    if (scanf_s("%d", &slot) == 1) {
        if (slot >= 0 && slot < MAX_INVENTORY) {
            player->inventory[slot] = 0;
            printf("Слот очищен.\n");
        }
        else {
            printf("Некорректный слот.\n");
        }
    }
    else cell_cleaning();
}

void search_unique_items(Player* player) {
    int unique_check[MAX_INVENTORY] = { 0 };
    for (int i = 0; i < MAX_INVENTORY; i++) {
        if (player->inventory[i] != 0) {
            int counter = 0;
            for (int j = 0; j < MAX_INVENTORY; j++) {
                if (unique_check[j] != player->inventory[i]) counter++;
                else break;
            }
            if (counter == MAX_INVENTORY) unique_check[i] = player->inventory[i];
        }
    }
    printf("--- Уникальные предметы ---\n");
    for (int i = 0; i < MAX_INVENTORY; i++) {
        if (unique_check[i] != 0) {
            int count_item = 0;
            for (int j = 0; j < MAX_INVENTORY; j++) {
                if (player->inventory[j] == unique_check[i]) count_item++;
            }
            printf("%s: %d шт.\n", player->index_item_array[unique_check[i]], count_item);
        }
    }
}
void create_or_reset_player(Player** player_ptr) {
    if (*player_ptr != NULL) {
        char confirm;
        printf("Пересоздать персонажа %s? (y/n): ", (*player_ptr)->name); // Добавил %s
        scanf_s(" %c", &confirm, 1); // Добавил 1 для scanf_s

        if (confirm == 'y' || confirm == 'Y') {
            printf("Память структуры игрока по адресу %p освобождена\n", (void*)(*player_ptr));
            free(*player_ptr);
            *player_ptr = NULL;
        }
        else {
            printf("Создание отменено.\n");
            cell_cleaning();
            return; // Выходим ТОЛЬКО если отменили
        }
    }

    // А вот здесь БЕЗ else создаем нового персонажа (или после удаления)
    *player_ptr = (Player*)malloc(sizeof(Player));

    if (*player_ptr == NULL) {
        printf("Ошибка выделения памяти!\n");
        return;
    }

    init_player(*player_ptr);
    record_file(*player_ptr);
    input_player_name(*player_ptr);

    printf("Персонаж успешно создан!\n");
}