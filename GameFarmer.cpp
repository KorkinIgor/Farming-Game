#include <stdio.h>
#include <locale.h>
#include <iostream>

bool check_for_int(int count) {
	if (count == 0) {
		printf("Это не целое число, укажите целое число \n");
		while (getchar() != '\n');
		return true;
	}
	return false;
}


int main() {
    setlocale(LC_ALL, "Russian");
	int current_day = 1;
	int current_hour = 8;
	int inventory[10] = { 1, 2, 2, 2, 3, 3, 4, 6, 0, 0 };
	const char *index_item_array[] = {"пусто", "дерево", "камень", "семена", "ножницы", "сено", "мотыга", "золото", "железо", "навоз"};
	
	while (1) {
		printf("Выберите команду(0-6):\n");
		int menu_operation;
		if (check_for_int(scanf_s("%d", &menu_operation)) == true) {
			continue;
		}
		if ((0 <= menu_operation and menu_operation <= 6) == false){
			printf("Укажи корректное значение \n");
			continue;
		}
		switch (menu_operation) {
			case 0:
				return 0;
			case 1:
				printf("Текущее время День: %d Часы: %d\n", current_day, current_hour);
				break;
			case 2:
				printf("Сколько часов вы хотите поработать?: \n");
				int working_hours;
				if (check_for_int(scanf_s("%d", &working_hours)) == false) {
					current_hour += working_hours;
					while (current_hour >= 24) {
						current_day += 1;
						current_hour -= 24;
					}
				}
				break;
			case 3:
				for (int i = 0; i < 10; i++) {
					int i_item = inventory[i];
					printf("слот %d %s\n", i, index_item_array[i_item]);
				}
				break;
			case 4:
				printf("Укажите индекс слота, затем укажите индекс предмета \n");
				int slot_index;
				int item_index_num;
				if ((check_for_int(scanf_s("%d", &slot_index)) == false) and check_for_int(scanf_s("%d", &item_index_num)) == false) {
					if ((slot_index < 10 and slot_index >= 0) and (item_index < 10 and item_index_num >= 0)) {
						inventory[slot_index] = item_index_num;
					}
					else {
						printf("Укажите значения от 0 до 9 \n");
					}
				}
				break;
			case 5:
				printf("Укажите номер слота, который хотите обнулить(0-9): \n");
				int slot_index_zero;
				if (check_for_int(scanf_s("%d", &slot_index_zero)) == false){
					if (0 <= slot_index_zero and slot_index_zero <= 9){
						inventory[slot_index_zero] = 0;
					}
					else {
						printf("Укажи корректное значение \n");
					}
				}
				break;
			case 6:
				int uniqueness_check[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //этот массив нужен для того чтобы не вывести кол-во одного и того же предмета несколько раз 
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
					if (uniqueness_check[index_un_check] != 0){
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
	}
}
