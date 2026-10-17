#include <stdio.h> //printf, scanf....
#include <string.h> //strcpy, strlen....
int current_day = 1; // текущий день
int current_hour = 8; // текущее время
int inventory[10] = {0,0,0,0,0,0,0,0,0,0}; //количество чего-то в инвентаре
int inventory_id[10] = {0,1,2,3,4,5,6,7,8,9}; // id предметов в инвентаре
char farmer_name[32]; // имя фермера до 31 символа
char item_names[10][32]; // массив для сохранения названий предметов до 31 символа


void load__item_names() {                                                                              //функция для предметов
    FILE *file = fopen("items.txt", "r"); //Открывать файл в формате r - read
    if (file == NULL) {
        printf("Пустой/не найден items.txt, Стандартные значение приняты");
        strcpy(item_names[0], "Пусто"); //стандартные значения
        strcpy(item_names[1], "Дерево");
        strcpy(item_names[2], "Камень");
        strcpy(item_names[3], "Семена");
        strcpy(item_names[4], "Мотыга");
        strcpy(item_names[5], "Огниво");
        strcpy(item_names[6], "Часы");
        strcpy(item_names[7], "Компас");
        strcpy(item_names[8], "Карта");
        strcpy(item_names[9], "Рюкзак");
        return;
    }
    char line[256]; //строка одна из 256 символов
    int id = 0; //счётчик
    while (fgets(line, sizeof(line), file) != NULL && id < 10) { //пока строка есть(не ничего) и айди<10
        line[strcspn(line, "\n")] = '\0'; //убираем пробел
        int file_id; //файл id
        char name[32]; //для имени предмета  до 31символа
        if (sscanf(line, "%d %31[^\n]", &file_id, name) == 2) { //читаем два значения(число - пробел - название предмета), если их 2, то
            if (file_id >= 0 && file_id < 10) { //если айди в поле, то
                strcpy(item_names[file_id], name); //копируем имя в нужный айди
            }
        } else {
            strcpy(item_names[id], line); //иначе кладём линию
        }
        id++; //прибавка к счётчику
    }
    fclose(file);
}


void menu() {                                                                                          //функция меню
    printf("     Текстовое меню:     |Имя: %s \n", farmer_name);
    printf("  [0] Выход\n");
    printf("  [1] Посмотреть на часы\n");
    printf("  [2] Промотать время\n");
    printf("  [3] Посмотреть инвентарь\n");
    printf("  [4] Положить предмет в слот\n");
    printf("  [5] Выбросить предмет\n");
    printf("  [6] Очистка мусора\n");
    printf("  [7] Поиск предета в рюкзаке по названию\n");
    printf("  [8] Записать состояние в дневник\n");
    printf("  [9] Доска объявлений\n");
}


void pause() {                                                                                        // пауза между выбором и меню
    getchar();
    getchar();
}


void show_time() {                                                                                    //1 пункт меню {
    printf("Текущее время: День %d, %d:00\n", current_day, current_hour); //вывод времени
}
                                             //}


void work_time() {                                                                                    //2 пункт меню {
    printf("Сколько часов работать?\n");
    int work;

    if (scanf("%d", &work) != 1) {
        scanf("%*s");
        printf("Неверное значение\n");
        pause();
        return;
    }//защита от дурака

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
        return;
    } //2 защита от дурака при минусовых значениях
}


void show_inventory() {                                                                                  //3 пункт меню
    for (int i = 0; i<10; i++) { //перебор
        int value = inventory[i];
        int slot = i;
        const char *name = item_names[inventory_id[i]];
        printf("Слот %d: [%d] - %s, id = %d\n",slot,value,name, inventory_id[i]);
    }
}


void put_item() {                                                                                         //4 пункт меню
    printf("Введите индекс слота:\n");
    int index;

    if (scanf("%d", &index) != 1) {
        scanf("%*s");
        printf("Неверное значение\n");
        pause();
        return;
    } //защита от дурака

    printf("Введите id предмета:\n");
    int id;

    if (scanf("%d", &id) != 1) {
        scanf("%*s");
        printf("Неверное значение\n");
        pause();
        return;
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
        return;
    } //2 защита от дурака
}


void drop_item() {                                                                                        //5 пункт меню
    printf("Введите индекс слота для выброса предмета:\n");
    int indexdrop;

    if (scanf("%d", &indexdrop) != 1) {
        scanf("%*s");
        printf("Неверное значение\n");
        pause();
        return;
    } //защита от дурака

    if (indexdrop<=9 && indexdrop>=0) {
        inventory[indexdrop] = 0;
        inventory_id[indexdrop] = 0;
    }

    else {
        printf("Неверный индекс слота\n");
    }//2 защита от дурака
}


void clear_trash() {                                                                                      //6 пункт меню
    printf("Введите id предмета для очистки:\n");
    int id_trash;

    if (scanf("%d", &id_trash) != 1) {
        scanf("%*s");
        printf("Неверное значение\n");
        pause();
        return;
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
        return;
    } //защита от дурака
}



void search_n_name() {                                                                                    //7 пункт меню
    char search_name[32]; //что будем искать/название
    printf("Введите название предмета: ");
    fgets(search_name, sizeof(search_name), stdin); //читаем строку в предмет
    search_name[strcspn(search_name, "\n")] = '\0'; //убираем лишний пробел
    int search_id = 999999999; //счётчик
    for (int i = 0; i<10; i++) { //от 0 до 9
        if (strcmp(item_names[i], search_name) == 0) { //если 2 строки одинаковые из нашего списка айди и нашего предмета который ищем
            search_id = i; //вводим айди в счётчик
            break;
        }
    }
    if (search_id == 999999999) { //если айди не нашли
        printf("Предмета с идентичным названием отсутствует в каталоге\n");
        return;
    }

    int found = 0;
    for (int i = 0; i<10; i++){ // от 0 до 9
        if(inventory_id[i] == search_id && inventory[i] > 0) { //если айди предмета == счётчику И кол-во больше 0
            printf("Слот %d: %s (id=%d, количество = %d)\n", i,item_names[search_id],search_id,inventory[i]); //слот, имя, какой по счёту, кол-во
            found = 1;
        }
    }
    if (!found) { //если ничего не нашли
        printf("Предмет %s отсутствует в инвентаре\n", search_name);
    }
}


void write_diary() {                                                                                        //8 пункт меню
    FILE *file = fopen("diary.txt", "a"); //открыли diary.txt в формате дозаписи(Append)
    if (file == NULL) { //если пустой файл/нет его
        printf("Не смог открыть дневник\n");
        return;
    }
    fprintf(file, "%s\n", farmer_name); //вводим имя фермера в txt
    fprintf(file, "День %d, %d:00\n", current_day, current_hour); //день, время
    fprintf(file, "Инвентарь:\n"); //инвентарь:

    for (int i = 0; i<10; i++) { //от 0 до 9
        int id = inventory_id[i]; //айди
        int count = inventory[i]; //количество
        const char *name = item_names[id]; //имя предмета
        fprintf(file, " Слот %d: %s (id=%d), x%d\n", i, name, id, count); //слот, имя, его айди и кол-во
    }
    fprintf(file, "\n"); //переход на новую строку
    fclose(file); //закрыли файл
    printf("Запись сделана в дневник");
}



void bulletin_board() {                                                                                     //9 пункт меню
    FILE *in = fopen("input.txt", "r"); //открыли input.txt в формате чтения(read)
    if (in == NULL) { //если пусто/не существует
        printf("Ошибка: не удалось открыть input.txt\n");
        return;
    }
    FILE *out = fopen("output.txt", "w"); //создали в формате записи(Write)
    if (out == NULL) { //если пусто/не существует
        printf("Ошибка: не удалось открыть output.txt\n");
        fclose(in);
        return;
    }
    char line[256]; //читаем линию до 256символов
    printf("Активные квесты (день %d): \n", current_day); //сегодняшний день напечатали
    fprintf(out, "Активные квесты (день %d): \n", current_day); //сегодняшние квесты записали в output.txt
    while (fgets(line, sizeof(line), in)) { //если есть что-то в input.txt
        line[strcspn(line, "\n")] = '\0'; //убираем пробелы
        if (line[0] == '\0') {
            continue; //если строка пустая, то дальше
        }
        char *day_str = strtok(line, "|");
        char *author  = strtok(NULL, "|");
        char *item    = strtok(NULL, "|");
        char *reward  = strtok(NULL, "|");

        if (day_str == NULL || author == NULL || item == NULL || reward == NULL) { //если чего то нет - пропуск
            continue;
        }
        int day = atoi(day_str); //перевод строки в число
        if (day > current_day) continue; //не тот день = пропуск
        int item_id = 99999999;
        for (int i = 0; i < 10; i++) { //от 0 до 9
            if (strcmp(item_names[i], item) == 0) { //если нашло совпадение
                item_id = i;
                break;
            }
        }
        int have = 0;
        if (item_id != 99999999) { //если нашлось в инвентаре
            for (int i = 0; i < 10; i++) { //от 0 до 9
                if (inventory_id[i] == item_id && inventory[i] > 0) { //если айди == айди И кол-во больше 0
                    have = 1; // Нашли
                    break;
                }
            }
        }
        const char *status;
        if (have == 1) {
            status = "ГОТОВО К СДАЧЕ";
        }
        else {
            status = "В ПРОЦЕССЕ";
        }
        printf("День %d | %s | %s | Награда: %s | %s\n", day, author, item, reward, status); //в консоль день|автор|предмет|награда|статус
        fprintf(out, "День %d | %s | %s | Награда: %s | %s\n", day, author, item, reward, status); //в output.txt день|автор|предмет|награда|статус
    }
    fclose(in);
    fclose(out);
    printf("Результат записан в output.txt\n");
}

int main() {                                                                                                      //запуск
    SetConsoleCP(65001); //русский язык
    SetConsoleOutputCP(65001); //русский язык
    load__item_names(); //загрузка название предметов
    printf("Введите имя фермера: ");
    fgets(farmer_name, sizeof(farmer_name), stdin); //читаем имя/скок символов/ввод
    farmer_name[strcspn(farmer_name, "\n")] = '\0'; //убираем пробел
    if (strlen(farmer_name) == 0) { //если длина 0
        strcpy(farmer_name, "Неизвестный"); // то меняем имя на Неизвестный
    }

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
        if (pick >9 || pick <0) {
            printf("Неверный пункт, введите заново\n");
            pause();
            continue;
        } //2 защита от дурака


        switch (pick) { // кейсы для меню
            case 0: //выход
                printf("   Выход   \n");
                break;

            case 1: //время
                show_time();
                pause();
                break;

            case 2: //работа по часам
                work_time();
                break;

            case 3: //просмотр инвентаря
                show_inventory();
                pause();
                break;

            case 4: //кладём предмет в слот
                put_item();
                pause();
                break;

            case 5: //выбросить предмет
                drop_item();
                pause();
                break;
            case 6: //очистка мусора
                clear_trash();
                pause();
                break;
            case 7: //поиск предмета по имени
                getchar();
                search_n_name();
                pause();
                break;
            case 8: //запись в дневник
                write_diary();
                pause();
                break;
            case 9: //доска объявлений
                bulletin_board();
                pause();
                break;
        }
    }
return 0;
}