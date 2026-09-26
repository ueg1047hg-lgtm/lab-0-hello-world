#include <stdio.h>
#include <windows.h>

int current_day = 1;
int current_hour = 8;
int inventory[10] = {0,0,0,0,0,0,0,0,0,0};
int inventory_id[10] = {0,1,2,3,4,5,6,7,8,9};
const char *inventory_name[10] = {
    "Пусто", "Дерево", "Камень", "Семена", "Мотыга",
    "Огниво", "Часы", "Компас", "Карта", "Рюкзак"
};

void menu() {
    printf("     Текстовое меню:     \n");
    printf("  [0] Выход\n");
    printf("  [1] Посмотреть на часы\n");
    printf("  [2] Промотать время\n");
    printf("  [3] Посмотреть инвентарь\n");
    printf("  [4] Положить предмет в слот\n");
    printf("  [5] Выбросить предмет\n");
    printf("  [6] Очистка мусора\n");
}

void pause() {
    getchar();
    getchar();
}

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    int pick = -1;
    while (pick != 0) {
        menu();
        printf("Введите пункт: \n");
        scanf("%d", &pick);
        if (pick > 6 || pick < 0) {
            printf("Неверный пункт, введите заново\n");
            pause();
            continue;
        }
        switch (pick) {
            case 0:
                printf("   Выход   \n");
                break;
            case 1:
                printf("Текущее время: День %d, %d:00\n", current_day, current_hour);
                pause();
                break;
            case 2:
                printf("Сколько часов работать?\n");
                int work;
                scanf("%d", &work);
                if (work > 0) {
                    int time_past = current_hour;
                    current_hour += work;
                    while (current_hour >= 24) {
                        current_day += 1;
                        current_hour -= 24;
                    }
                    printf("Было %d:00. Игрок проработал %d часов. Стало: День %d, %d:00\n", time_past, work, current_day, current_hour);
                    pause();
                } else {
                    printf("Неверный ввод часов");
                    pause();
                    continue;
                }
                break;
            case 3:
                for (int i = 0; i < 10; i++) {
                    printf("Слот %d: [%d] - %s\n", i, inventory[i], inventory_name[inventory_id[i]]);
                }
                pause();
                break;
            case 4:
                printf("Введите индекс слота:\n");
                int index;
                scanf("%d", &index);
                printf("Введите id предмета:\n");
                int id;
                scanf("%d", &id);
                if ((index <= 9 && index >= 0) && (id <= 9 && id >= 0)) {
                    if (inventory_id[index] == id) {
                        inventory[index]++;
                    } else {
                        inventory[index] = 1;
                    }
                    inventory_id[index] = id;
                } else {
                    printf("Неверный индекс или айди\n");
                    pause();
                    continue;
                }
                pause();
                break;
            case 5:
                printf("Введите индекс слота для выброса предмета:\n");
                int indexdrop;
                scanf("%d", &indexdrop);
                if (indexdrop <= 9 && indexdrop >= 0) {
                    inventory[indexdrop] = 0;
                    inventory_id[indexdrop] = 0;
                } else {
                    printf("Неверный индекс слота\n");
                }
                pause();
                break;
            case 6:
                printf("Введите id предмета для очистки:\n");
                int id_trash;
                int score = 0;
                scanf("%d", &id_trash);
                if (id_trash > 0 && id_trash <= 9) {
                    for (int i = 0; i < 10; i++) {
                        if (inventory_id[i] == id_trash) {
                            inventory[i] = 0;
                            printf("Удалён слот %d\n", i);
                            score += 1;
                            inventory_id[i] = 0;
                        }
                    }
                    printf("Всего удалено %d слотов\n", score);
                } else {
                    printf("Неверный id предмета\n");
                    pause();
                    continue;
                }
                break;
        }
    }
    return 0;
}