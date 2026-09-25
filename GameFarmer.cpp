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
	int inventory[10] = { 1, 2, 2, 2, 3, 3, 4, 6, 0, 0 }; //4 - ножницы, 5 - сено, 6 - мотыга, 7 - золото, 8 - железо, 9 - навоз
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
					printf("слот %d %d\n",i, inventory[i]);
				}
				break;
			case 4:
				printf("Укажите индекс слота, затем укажите индекс предмета \n");
				int slot_index;
				int item_index;
				if ((check_for_int(scanf_s("%d", &slot_index)) == false) and check_for_int(scanf_s("%d", &item_index)) == false) {
					if ((slot_index < 10 and slot_index >= 0) and (item_index < 10 and item_index >= 0)) {
						inventory[slot_index] = item_index;
					}
					else {
						printf("Укажите значения от 0 до 9 \n");
					}
				}

		}
	}
}
