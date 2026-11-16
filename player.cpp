#define _CRT_SECURE_NO_WARNINGS
#include "player.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <cstdio>

using namespace std;

const char* item_names_defualt[] = { "пусто", "дерево", "камень", "семена", "ножницы", "сено", "мотыга", "золото", "железо", "навоз" };

void init_game_state(GameState* game) {
	game->current_day = 1;
	game->current_hour = 8;
	game->is_game = true;
	int default_inv[] = { 1, 2, 2, 2, 3, 3, 4, 6, 0, 0 };
	for (int i = 0; i < MAX_INVENTORY; i++) {
		game->inventory[i] = default_inv[i];
	}
}

void record_file(GameState* game) {
	std::ifstream file("items.txt");
	if (!file.is_open()) {
		printf("Файл items.txt не найден!\nЗагружены стандартные названия предметов.\n");
		for (int i = 0; i < MAX_ITEMS_COUNT; ++i) {
			strncpy(game->index_item_array[i], item_names_defualt[i], MAX_ITEM_NAME - 1);
			game->index_item_array[i][MAX_ITEM_NAME - 1] = '\0';
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
				strncpy(game->index_item_array[id], name.c_str(), MAX_ITEM_NAME - 1);
				game->index_item_array[id][MAX_ITEM_NAME - 1] = '\0';
			}
		}
	}
	file.close();
}

void inputFarmerName(GameState* game) {
	printf("Укажите имя вашего фермера \n");
	fgets(game->name_user, sizeof(game->name_user), stdin);
	size_t size_name = strlen(game->name_user);
	if (size_name > 0 && game->name_user[size_name - 1] == '\n') {
		game->name_user[size_name - 1] = '\0'; // Убираем \n
		printf("привет! %s\n", game->name_user);
	}
	else {
		printf("Вы указали неправильное имя фермера \n");
		cell_cleaning();
		inputFarmerName(game);
	}
}

void write_diary(GameState* game) {
	ofstream file("diary.txt", ios::app);
	file << game->name_user << " День " << game->current_day << " Часы " << game->current_hour << "\n";
	for (int i = 0; i < MAX_INVENTORY; i++) {
		file << "Cлот " << i << " " << game->index_item_array[game->inventory[i]] << "\n";
	}
	printf("Записал!\n");
	file.close();
}

void searh_item(GameState* game) {
	cell_cleaning();
	char search_name[MAX_NAME_LENGTH];
	printf("Введите название предмета \n");
	fgets(search_name, sizeof(search_name), stdin);
	size_t len = strlen(search_name);
	if (len > 0 && search_name[len - 1] == '\n') search_name[len - 1] = '\0';

	int repeats_item = 0;
	for (int i = 0; i < MAX_INVENTORY; i++) {
		if (strcmp(search_name, game->index_item_array[game->inventory[i]]) == 0) {
			repeats_item++;
			printf("слот: %d \n", i);
		}
	}
	if (repeats_item == 0) printf("Такого предмета нет \n");
}

void show_time(GameState* game) {
	printf("Текущее время День: %d Часы: %d\n", game->current_day, game->current_hour);
}

void work(GameState* game) {
	printf("Сколько часов вы хотите поработать?: \n");
	int working_hours;
	if (scanf_s("%d", &working_hours) == 1) {
		game->current_hour += working_hours;
		while (game->current_hour >= HOURS_IN_DAY) {
			game->current_day += 1;
			game->current_hour -= HOURS_IN_DAY;
		}
	}
	else {
		cell_cleaning();
	}
}

void show_inventory(GameState* game) {
	for (int i = 0; i < MAX_INVENTORY; i++) {
		printf("слот %d %s\n", i, game->index_item_array[game->inventory[i]]);
	}
}

void change_item(GameState* game) {
	printf("Укажите индекс слота \n");
	int slot_index, item_index_num;
	if (scanf_s("%d", &slot_index) == 1) {
		printf("Укажите индекс предмета \n");
		if (scanf_s("%d", &item_index_num) == 1) {
			if ((slot_index < MAX_INVENTORY && slot_index >= 0) && (item_index_num < MAX_ITEMS_COUNT && item_index_num >= 0)) {
				game->inventory[slot_index] = item_index_num;
			}
			else {
				printf("Укажите значения от 0 до %d \n", MAX_INVENTORY - 1);
			}
		}
		else cell_cleaning();
	}
	else cell_cleaning();
}

void clear_cell(GameState* game) {
	printf("Укажите номер слота, который хотите обнулить(0-%d): \n", MAX_INVENTORY - 1);
	int slot_index_zero;
	if (scanf_s("%d", &slot_index_zero) == 1) {
		if (0 <= slot_index_zero && slot_index_zero < MAX_INVENTORY) {
			game->inventory[slot_index_zero] = 0;
		}
		else {
			printf("Укажи корректное значение \n");
		}
	}
	else cell_cleaning();
}

void search_unique_items(GameState* game) {
	int uniqueness_check[MAX_INVENTORY] = { 0 };
	for (int i = 0; i < MAX_INVENTORY; i++) {
		if (game->inventory[i] != 0) {
			int counter = 0;
			for (int j = 0; j < MAX_INVENTORY; j++) {
				if (uniqueness_check[j] != game->inventory[i]) counter++;
				else break;
			}
			if (counter == MAX_INVENTORY) uniqueness_check[i] = game->inventory[i];
		}
	}
	for (int i = 0; i < MAX_INVENTORY; i++) {
		if (uniqueness_check[i] != 0) {
			int count_item = 0;
			for (int j = 0; j < MAX_INVENTORY; j++) {
				if (game->inventory[j] == uniqueness_check[i]) count_item++;
			}
			printf("Количество %s %d\n", game->index_item_array[uniqueness_check[i]], count_item);
		}
	}
}