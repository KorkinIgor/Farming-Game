#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <iostream>
#include <map>
#include <windows.h>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdio>    
#include <sstream>    

int is_game = 1;
int current_day = 1;
int current_hour = 8;
int inventory[10] = { 1, 2, 2, 2, 3, 3, 4, 6, 0, 0 };
const char* item_names_defualt[] = { "пусто", "дерево", "камень", "семена", "ножницы", "сено", "мотыга", "золото", "железо", "навоз" };
char index_item_array[10][64];
char name_user[33];

void record_file() {
	std::ifstream file("items.txt");
	if (!file.is_open()) {
		printf("Файл items.txt не найден!\n");
		printf("Загружены стандартные названия предметов.\n");
		for (int i = 0; i < 10; ++i) {
			strncpy(index_item_array[i], item_names_defualt[i], 63);
			index_item_array[i][63] = '\0';
		}
	}
	std::string line;
	int loaded = 0;
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		int id;
		std::string name;

		// Парсим строку "0 Пусто" → id = 0, name = "Пусто"
		if (iss >> id >> name) {
			if (id >= 0 && id < 10) {
				// Копируем name (std::string) в char-массив
				strncpy(index_item_array[id], name.c_str(), 63);
				index_item_array[id][63] = '\0';
				++loaded;
			}
		}
	}
	file.close();
}
void menu_entry() {
	printf("\nВыберите команду(0-6):\n");
	printf("0 - выйти из игры\n");
	printf("1 - посмотреть время\n");
	printf("2 - поработать n часов\n");
	printf("3 - посмотреть инвентарь\n");
	printf("4 - заменить предмет\n");
	printf("5 - выбросить предмет\n");
	printf("6 - кол-во уникальных предметов\n");
}

void cell_cleaning() {
	printf("Введите корректное значение \n");
	while (getchar() != '\n');
}

void finish_game() {
	is_game = 0;
}

void show_time() {
	printf("Текущее время День: %d Часы: %d\n", current_day, current_hour);
}

void work() {
	printf("Сколько часов вы хотите поработать?: \n");
	int working_hours;
	int input_result = scanf_s("%d", &working_hours);
	if (input_result == 1) {
		current_hour += working_hours;
		while (current_hour >= 24) {
			current_day += 1;
			current_hour -= 24;
		}
	}
	else
	{
		cell_cleaning();
	}
}

void show_inventory() {
	for (int i = 0; i < 10; i++) {
		int i_item = inventory[i];
		printf("слот %d %s\n", i, index_item_array[i_item]);
	}
}

void change_item() {
	printf("Укажите индекс слота \n");
	int slot_index;
	int item_index_num;
	int input_result1 = scanf_s("%d", &slot_index);
	if (input_result1 == 1) {
		printf("Укажите индекс предмета \n");
		int input_result2 = scanf_s("%d", &item_index_num);
		if (input_result2 == 1) {
			if ((slot_index < 10 and slot_index >= 0) and (item_index_num < 10 and item_index_num >= 0)) {
				inventory[slot_index] = item_index_num;
			}
			else {
				printf("Укажите значения от 0 до 9 \n");
			}
		}
		else {
			cell_cleaning();
		}
	}
	else {
		cell_cleaning();
	}
}

void clear_cell() {
	printf("Укажите номер слота, который хотите обнулить(0-9): \n");
	int slot_index_zero;
	int input_result = scanf_s("%d", &slot_index_zero);
	if (input_result == 1) {
		if (0 <= slot_index_zero and slot_index_zero <= 9) {
			inventory[slot_index_zero] = 0;
		}
		else {
			printf("Укажи корректное значение \n");
		}
	}
	else {
		cell_cleaning();
	}
}
void search_unique_items() {
	int uniqueness_check[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; //этот массив нужен для того чтобы не вывести кол-во одного и того же предмета несколько раз 
	for (int i = 0; i < 10; i++) {
		if (inventory[i] != 0) {
			int counter = 0;
			for (int check_variable = 0; check_variable < 10; check_variable++) {
				if (uniqueness_check[check_variable] != inventory[i]) {
					counter++;
				}
				else {
					break;
				}
			}
			if (counter == 10) {
				uniqueness_check[i] = inventory[i];
			}
		}
	}
	for (int index_un_check = 0; index_un_check < 10; index_un_check++) {
		if (uniqueness_check[index_un_check] != 0) {
			int count_item = 0;
			for (int index_inventory = 0; index_inventory < 10; index_inventory++) {
				if (inventory[index_inventory] == uniqueness_check[index_un_check]) {
					count_item++;
				}
			}
			printf("Количество %s %d\n", index_item_array[uniqueness_check[index_un_check]], count_item);
		}
	}
}


void inputFarmerName() {
	setlocale(LC_ALL, "Russian");
	printf("Укажите имя вашего фермера \n");
	fgets(name_user, sizeof(name_user), stdin);
	size_t size_name = strlen(name_user);
	if (size_name > 0 and name_user[size_name - 1] == '\n'){
		printf("Привет! %s", name_user);
	}
	else {
		printf("Вы указали неправильное имя фермера \n");
		cell_cleaning();
		inputFarmerName();
	}

}

int main() {
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	record_file();
	inputFarmerName();
	for (int i = 0; i < 10; ++i) {
		printf("%s \n", index_item_array[i]);
	}
	while (is_game) {
		menu_entry();
		std::map<int, void(*)()> actions = {
			{0, finish_game}, {1, show_time}, {2, work}, {3, show_inventory}, {4, change_item}, {5, clear_cell}, {6, search_unique_items}
		};
		int menu_operation;
		int input_result = scanf_s("%d", &menu_operation);


		if (auto it = actions.find(menu_operation); it != actions.end())
			it->second();
		else {
			if (input_result == 0 or menu_operation > 6){
				cell_cleaning();
			}
		}
	}
}
