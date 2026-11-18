#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <map>
#include <windows.h>
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include "player.h"
#include "map.h"
#include "utils.h"


using namespace std;

void menu_entry() {
	printf("\nВыберите команду:\n");
	printf("%d - выйти из игры\n", 0);
	printf("%d - создать/пересоздать персонажа\n", 1);
	printf("%d - показать карточку игрока\n", 2);
	printf("%d - времечко\n", 3);
	printf("%d - заменить предмет\n", 4);
	printf("%d - выбросить предмет\n", 5);
	printf("%d - кол-во уникальных предметов\n", 6);
	printf("%d - найти предмет\n", 7);
	printf("%d - записать в дневник\n", 8);
	printf("%d - парсинг\n", 9);
	printf("%d - отправиться в приключение\n", 10);
	printf("> ");
}


int main() {
	srand(std::time(nullptr));
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	Player* player = NULL;

	bool is_game = true;
	while (is_game) {
		menu_entry();

		int choice;
		if (scanf_s("%d", &choice) != 1) {
			cell_cleaning();
			continue;
		}

		if (choice == 0) {
			is_game = false;
			continue;
		}

		if (choice == 1) {
			create_or_reset_player(&player);
			input_player_name(player);
			continue;
		}

		// ЗАЩИТА ОТ ВЫЛЕТА (если игрока нет)
		if (player == NULL) {
			printf("Сначала создайте персонажа!\n");
			continue;
		}

		std::map<int, void(*)(Player*)> actions = {
			{ 2, show_character_card },
			{ 3, time_management },
			{ 4, change_item },
			{ 5, clear_cell },
			{ 6, search_unique_items },
			{ 7, search_item },
			{ 8, write_diary },
			{ 9, [](Player*) { parser(); } },
			{ 10, trip_character }
		};

		if (auto it = actions.find(choice); it != actions.end()) {
			it->second(player);
		}
		else {
			printf("Такого пункта меню нет.\n");
			cell_cleaning();
		}
	}

	// ВЫХОД И ОЧИСТКА ПАМЯТИ
	if (player != NULL) {
		printf("Память структуры игрока по адресу %p освобождена\n", (void*)player);
		free(player);
	}
	else {
		printf("Выход из игры. Персонаж не был создан.\n");
	}

	return 0;
}