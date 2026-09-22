#include <stdio.h>
#include <locale.h>
#include <iostream>

int main() {
    setlocale(LC_ALL, "Russian");
	int current_day = 1;
	int current_hour = 8;
	int inventory[10] = { 1, 2, 2, 2, 3, 3, 4, 6, 0, 0 }; //4 - ножницы, 5 - сено, 6 - мотыга, 7 - золото, 8 - железо, 9 - навоз
	while (1) {
		int menu_operation;
		printf("Выберите команду(0-6):\n");
		scanf_s("%d", &menu_operation);
		switch (menu_operation) {
			case 0:
				return 0;
			case 1:
				printf("Текущее время День: %d Часы: %d\n", current_day, current_hour);
				break;
			case 2:
				printf("Сколько часов вы хотите поработать?: \n");
				int working_hours;
				scanf_s("%d", &working_hours);
				current_hour += working_hours;
				while (current_hour >= 24){
					current_day += 1;
					current_hour -= 24;
				}
				break;
		}
	}
}
