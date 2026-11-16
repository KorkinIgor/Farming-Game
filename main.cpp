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
	printf("\nВыберите команду(0-10):\n");
	printf("0 - выйти из игры\n1 - посмотреть время\n2 - поработать n часов\n");
	printf("3 - посмотреть инвентарь\n4 - заменить предмет\n5 - выбросить предмет\n");
	printf("6 - кол-во уникальных предметов\n7 - найти предмет\n8 - записать в дневник\n");
	printf("9 - парсинг\n10 - отправиться в приключение\n");
}

void finish_game(GameState* game) {
	game->is_game = false;
}

int main() {
	srand(std::time(nullptr));
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	//Просим у компьютера память записываем структуру(название адреса game)
	GameState* game = (GameState*)malloc(sizeof(GameState));
	init_game_state(game);

	record_file(game);
	inputFarmerName(game);

	while (game->is_game) {
		menu_entry();

		std::map<int, void(*)(GameState*)> actions = {
			{ 0, finish_game },{ 1, show_time },{ 2, work },{ 3, show_inventory },
			{ 4, change_item },{ 5, clear_cell },{ 6, search_unique_items },
			{ 7, searh_item },{ 8, write_diary },{ 9, [](GameState*) { parser(); } },
			{ 10, trip_character }
		};

		int menu_operation;
		if (scanf_s("%d", &menu_operation) != 1) {
			cell_cleaning();
			continue;
		}

		if (auto it = actions.find(menu_operation); it != actions.end()) {
			it->second(game); // Передаем структуру
		}
		else {
			if (menu_operation != 0) cell_cleaning(); // Очистка если ввели число вне диапазона
		}
	}

	// ОСВОБОЖДЕНИЕ ПАМЯТИ ПЕРСОНАЖА
	printf("Память структуры по адресу %p освобождена\n", (void*)game);
	free(game);

	return 0;
}