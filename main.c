#include <stdio.h>

int current_day = 1;
int current_hour = 8;

void menu() {
    printf("     Текстовое меню:     \n");
    printf("  [0] Выход\n");
    printf("  [1] Посмотреть на часы\n");
    printf("  [2] Промотать время\n");
}

int main() {
    int pick = -1;
    while (pick != 0) {
        menu();
        printf("Введите пункт: \n");
        scanf("%d", &pick);
        switch (pick) {
            case 0:
                printf("   Выход   \n");
                break;
            case 1:
                printf("Текущее время: День %d, %d:00\n", current_day, current_hour);
                break;
            case 2:
                printf("Сколько часов работать?\n");
                int work;
                scanf("%d", &work);
                int time_past = current_hour;
                current_hour += work;
                while (current_hour >= 24) {
                    current_day += 1;
                    current_hour -= 24;
                }
                printf("Было %d:00. Игрок проработал %d часов. Стало: День %d, %d:00\n", time_past, work, current_day, current_hour);
                break;
        }
    }
    return 0;
}