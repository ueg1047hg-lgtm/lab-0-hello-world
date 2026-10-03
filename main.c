#include <stdio.h>
int current_day = 1; // дни
int current_hour = 8; //часы
int inventory[10] = {0,0,0,0,0,0,0,0,0,0}; //инвентарь, кол-во чего то
int inventory_id[10] = {0,1,2,3,4,5,6,7,8,9}; // id предметов
//id:
    //0 = пустота
    //1 = дерево
    //2 = камень
    //3 = семена
    //4 = мотыга
    //5 = огниво
    //6 = часы
    //7 = компас
    //8 = карта
    //9 = рюкзак
const char *inventory_name[10] = { 
    "Пусто",
    "Дерево",
    "Камень",
    "Семена",
    "Мотыга",
    "Огниво",
    "Часы",
    "Компас",
    "Карта",
    "Рюкзак"
}; //список для id



void menu() { //def функция меню
    printf("     Текстовое меню:     \n");
    printf("  [0] Выход\n");
    printf("  [1] Посмотреть на часы\n");
    printf("  [2] Промотать время\n");
    printf("  [3] Посмотреть инвентарь\n");
    printf("  [4] Положить предмет в слот\n");
    printf("  [5] Выбросить предмет\n");
    printf("  [6] Очистка мусора\n");
}

void pause() { // пауза между выбором и меню
    getchar();
    getchar();
}

int main() { //запуск
    SetConsoleCP(65001); //русский язык
    SetConsoleOutputCP(65001); //русский язык
    int pick = -1; //ввели выбор
    while (pick != 0) {
        menu(); //вызывается меню
        printf("Введите пункт: \n");
        if (scanf("%d", &pick) != 1) {
            scanf("%*s");
            printf("Неверное значение\n");
            pause();
            continue;
        } //защита от дурака
        if (pick >6 || pick <0) {
            printf("Неверный пункт, введите заново\n");
            pause();
            continue;
        } //2 защита от дурака


        switch (pick) { // кейсы для меню
            case 0: //выход
                printf("   Выход   \n");
                break;

            case 1: //время
                printf("Текущее время: День %d, %d:00\n", current_day, current_hour);
                pause();
                break;

            case 2: //работа по часам
                printf("Сколько часов работать?\n");
                int work;
                if (scanf("%d", &work) != 1) {
                    scanf("%*s");
                    printf("Неверное значение\n");
                    pause();
                    break;
                } //защита от дурака
                if (work>0){
                    int time_past = current_hour;
                    current_hour += work;
                    while (current_hour >= 24) {
                        current_day += 1; //смена дня при часов>24
                        current_hour -= 24;
                    }
                    printf("Было %d:00. Игрок проработал %d часов. Стало: День %d, %d:00\n", time_past,work,current_day,current_hour);
                    pause();
                }
                else {
                    printf("Неверный ввод часов");
                    pause();
                    continue;
                } //2 защита от дурака при минусовых значениях
                break;

            case 3: //просмотр инвентаря
                for (int i = 0; i<10; i++) { //перебор
                    int value = inventory[i];
                    int slot = i;
                    const char *name = inventory_name[inventory_id[i]];
                    printf("Слот %d: [%d] - %s, id = %d\n",slot,value,name, inventory_id[i]);
                }
                pause();
                break;

            case 4: //кладём предмет в слот
                printf("Введите индекс слота:\n");
                int index;
                if (scanf("%d", &index) != 1) {
                    scanf("%*s");
                    printf("Неверное значение\n");
                    pause();
                    break;
                } //защита от дурака
                printf("Введите id предмета:\n");
                int id;
                if (scanf("%d", &id) != 1) {
                    scanf("%*s");
                    printf("Неверное значение\n");
                    pause();
                    break;
                } //2 защита от дурака
                if ((index<=9 && index >= 0 ) && (id<=9 && id>=0)) {
                    if (inventory_id[index] == id) {
                        inventory[index] ++; //если айди предмета, который хотим добавить в слот совпадает с тем же айди, который уже есть, то +1
                    }
                    else {
                        inventory[index] = 1; //иначе прошлое кол-во стирается
                    }
                    inventory_id[index] = id;
                }
                else {
                    printf("Неверный индекс или айди\n");
                    pause();
                    continue;
                } //2 защита от дурака
                pause();
                break;

            case 5: //выбросить предмет
                printf("Введите индекс слота для выброса предмета:\n");
                int indexdrop;
                if (scanf("%d", &indexdrop) != 1) {
                    scanf("%*s");
                    printf("Неверное значение\n");
                    pause();
                    break;
                } //защита от дурака
                if (indexdrop<=9 && indexdrop>=0) {
                    inventory[indexdrop] = 0;
                    inventory_id[indexdrop] = 0;
                }
                else {
                    printf("Неверный индекс слота\n");
                }//2 защита от дурака
                pause();
                break;
            case 6:
                printf("Введите id предмета для очистки:\n");
                int id_trash;
                if (scanf("%d", &id_trash) != 1) {
                    scanf("%*s");
                    printf("Неверное значение\n");
                    pause();
                    break;
                }//защита от дурака
                int score = 0;
                if (id_trash>0 && id_trash<=9){
                    for (int i = 0; i<10;i++) {
                        if (inventory_id[i]==id_trash){ //если айди совпадает с айди того, что хотим удалить
                            inventory[i] = 0; //то удаляем
                            printf("Удалён слот %d\n", i);
                            score += 1;
                            inventory_id[i]=0; //айди стал пустотой
                        }
                    }
                printf("Всего удалено %d слотов\n", score);
                }
                else {
                    printf("Неверный id предмета\n");
                    pause();
                    continue;
                }
                break;



        }



    }



return 0;
}