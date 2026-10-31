#include <stdio.h> //printf, scanf....
#include <string.h> //strcpy, strlen....
#include <stdlib.h> // malloc, free, rand, srand - блоки памяти, рандомы
#include <windows.h> //на всяк для ру языка
#include <time.h>

int current_day = 1; // текущий день
int current_hour = 8; // текущее время
int inventory[10] = {0,2,16,6,1,1,1,1,1,1}; //количество чего-то в инвентаре
int inventory_id[10] = {0,1,2,3,4,5,6,7,8,9}; // id предметов в инвентаре
char farmer_name[32]; // имя фермера до 31 символа
char item_names[10][32]; // массив для сохранения названий предметов до 31 символа
int depth = 0;    //глубоко ли?
int journey_cards = 0;    //карты


typedef struct {    //коробка данных про карту
    char **net;   // сетка карты(указатель на указатель)
    int x_height_map;       // Высота сетки x
    int y_width_map;       // ширина сетки y
    int hero_xpos;   // позиция чела высота x
    int hero_ypos;   // позиция чела ширина y
    int in_cave;    // пещера или нет
} GameMap;


void my_pause() {                                                                                        // пауза между выбором и меню
    getchar();
    getchar();
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}


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
    while (fgets(line, sizeof(line), file) != NULL && id < 10) { //пока строка есть(не ничего) И айди<10
        line[strcspn(line, "\n")] = '\0'; //убираем \n
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
    printf("  [10] Отправиться в приключение\n");
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
        my_pause();
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
        my_pause();
    }

    else {
        printf("Неверный ввод часов");
        my_pause();
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
        my_pause();
        return;
    } //защита от дурака

    printf("Введите id предмета:\n");
    int id;

    if (scanf("%d", &id) != 1) {
        scanf("%*s");
        printf("Неверное значение\n");
        my_pause();
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
        my_pause();
        return;
    } //2 защита от дурака
}


void drop_item() {                                                                                        //5 пункт меню
    printf("Введите индекс слота для выброса предмета:\n");
    int indexdrop;

    if (scanf("%d", &indexdrop) != 1) {
        scanf("%*s");
        printf("Неверное значение\n");
        my_pause();
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
        my_pause();
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
        my_pause();
        return;
    } //защита от дурака
}



void search_n_name() {                                                                                    //7 пункт меню
    char search_name[32]; //что будем искать/название
    printf("Введите название предмета: ");
    fgets(search_name, sizeof(search_name), stdin); //читаем строку в предмет
    search_name[strcspn(search_name, "\n")] = '\0'; //убираем \n
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
    my_pause();
    printf("%s\n", farmer_name); //выводим имя фермера
    printf("День %d, %d:00\n", current_day, current_hour); //день, время
    printf("Инвентарь:\n"); //инвентарь:
    for (int i = 0; i<10; i++) { //от 0 до 9
        int id = inventory_id[i]; //айди
        int count = inventory[i]; //количество
        const char *name = item_names[id]; //имя предмета
        printf(" Слот %d: %s (id=%d), x%d\n", i, name, id, count); //слот, имя, его айди и кол-во
    }
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
    char line[256]; //читаем строку до 256символов
    printf("Активные квесты (день %d): \n", current_day); //сегодняшний день напечатали
    fprintf(out, "Активные квесты (день %d): \n", current_day); //сегодняшние квесты записали в output.txt
    while (fgets(line, sizeof(line), in)) { //если есть что-то в input.txt пишем в линию
        line[strcspn(line, "\n")] = '\0'; //убираем \n
        if (line[0] == '\0') {
            continue; //если строка пустая, то пропуск
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
    printf("\n");
    printf("Результат записан в output.txt\n");
}


char **dedicated_map(int x_height_map, int y_width_map) {          //создаём функцию с x и y размерами карты, функция вернёт указатель на указатель(главный адрес)
    char **map = (char **)malloc(x_height_map * sizeof(char *));        //**map - внутри адрес, который ведёт на адреса
                                                                        //char ** по адресу будет список
                                                                        // Memory ALLOCation занимает(бронирует) определённое количество байтов и возвращает
                                                                        //умножение x строк на sizeof(сколько байт занимает указатель(8 байт))
    if (map == NULL) { 
        printf("ОШИБКА : НЕХВАТКА ПАМЯТИ!!!\n"); //если памяти нет, то возвращаем NULL
        return NULL;
    }

    for (int i = 0; i < x_height_map; i++) {            //цикл по строкам
        map[i] = (char *)malloc(y_width_map * sizeof(char));        //в map[i] - адрес строки записывается
                                                                        //char * по адресу будет строка
                                                                        // Memory ALLOCation занимает(бронирует) определённое количество байтов и возвращает
                                                                        //умножение y столбцов на sizeof(скок весит символ(1 байт))
        if (map[i] == NULL) {
            printf("ОШИБКА : НЕХВАТКА ПАМЯТИ ПОД СТРОКУ!!!\n");     //если памяти нет, то возвращаем NULL в конце
            for (int j = 0; j < i; j++) {     //памяти не хватило на i строке, освобождаем строки которые успели создать
                free(map[j]);    //свобода памяти!!! (в строках которые успели забронировать)
            }
            free(map);   //свобода карте(память освободилась)
            return NULL;    //выход
        }
    }
    return map; //если всё норм то возврат карты
}


void free_map(char **map, int x_height_map) {  //функция для очистки карты //принимаем главный адрес карты и x строк
    if (map == NULL) {
        return;     //если и так пустая то скип
    }
    for (int i = 0; i < x_height_map; i++) {       //цикл по всем строкам
        free(map[i]);         //память освободилась в i строке
    }
    free(map);        //освобождаем массив указателей
}


void random_place(GameMap *g, char sym) {      //функция принимает указатель на структуру-коробку в памяти, а также символ который мы кладём в карту(костёр/пещера)
    while (1) {         //работает всегда до break
        int x = rand() % ((*g).x_height_map - 2) + 1; //rand выдаёт (от 0 до высоты карты -2) + 1(x никогда не будет равен 0), а также никогда не коснётся стены
        int y = rand() % ((*g).y_width_map - 2) + 1; //rand выдаёт (от 0 до высоты карты -2) + 1(y никогда не будет равен 0), а также никогда не коснётся стены
        if ((*g).net[x][y] == '.') {         //в структуре-коробке открываем (*g).net(чтобы открылось данные, а не ) на x,y, если в сетке на x,y трава, то можно
            (*g).net[x][y] = sym;            //поставить символ
            break;      //и закончить функцию
        }
    }
}



void generate_forest(GameMap *g) {          //генерируем лес
    (*g).x_height_map = rand() % 26 + 5;       //высота от 0 до 25 и весь диапозон сдвигаем на 5, то есть высота от 5 до 30
    (*g).y_width_map = rand() % 26 + 5;        //ширина от 0 до 25 и весь диапозон сдвигаем на 5, то есть высота от 5 до 30
    (*g).net = dedicated_map((*g).x_height_map, (*g).y_width_map);     //вызываем функцию для выделения памяти и кидаем туда x,y координату
                                                                       //и записываем главный адрес в net, в структуру-коробку, заполняем
    for (int i = 0; i < (*g).x_height_map; i++) {     //цикл по строкам
        for (int j = 0; j < (*g).y_width_map; j++) {    //в котором цикл по столбцам
            if (i == 0 || i == (*g).x_height_map - 1 || j == 0 || j == (*g).y_width_map - 1) {    //если i=0 или i=высота-1 или j=0 или j=ширина-1
                                                                                                  //то есть край карты, либо вверх, либо низ, либо лево, либо право
                (*g).net[i][j] = '#';      //забор поставили в этом месте
            } else {        //если в лесу не на краю карты
                int r = rand() % 100;       //rand от 0 до 99
                if (r < 11) (*g).net[i][j] = 'T';         //в случае меньше 11, то дерево
                else if (r < 21) (*g).net[i][j] = 'R';          //если от 11 до 20, то камень
                else (*g).net[i][j] = '.';         //иначе трава спавнится
            }
        }
    }

    random_place(g, '^'); //место для костра/лагеря появляется в случайной координате
    random_place(g, 'O');  //место для пещерки появляется в случайной координате

    do { //делай
        (*g).hero_xpos = rand() % ((*g).x_height_map - 2) + 1;        //рандом x позиция игрока изначальная чтобы не заспавнился в стенах
        (*g).hero_ypos = rand() % ((*g).y_width_map - 2) + 1;         //рандом y позиция игрока изначальная чтобы не заспавнился в стенах
    } while ((*g).net[(*g).hero_xpos][(*g).hero_ypos] != '.');       //проверяем, не в дереве или камне ли персонаж, не в костре или не в входе в пещеру

    (*g).in_cave = 0;       //мы в лесу(обращаемся к коробке)
    journey_cards++;        //+1 карта
}


void generate_cave(GameMap *g) {                     //генерируем пещеру
    (*g).x_height_map = rand() % 4 + 5;           //высота от 0 до 3, но смещаем диапазон на 5, в итоге от 5 до 8
    (*g).y_width_map = rand() % 4 + 5;            //ширина от 0 до 3, но смещаем диапазон на 5, в итоге от 5 до 8
    (*g).net = dedicated_map((*g).x_height_map, (*g).y_width_map);          //обращаемся к функции с определёнными x,y, вводим в net в коробку в виде значения

    for (int i = 0; i < (*g).x_height_map; i++) {              //цикл по x
        for (int j = 0; j < (*g).y_width_map; j++) {                  //цикл по y
            if (i == 0 || i == (*g).x_height_map - 1 || j == 0 || j == (*g).y_width_map - 1) {            //если края то
                (*g).net[i][j] = '#';                                                                     //стены
            } else {                        //внутри пещеры
                int r = rand() % 100;                      //от 0 до 99
                if (r < 10) (*g).net[i][j] = 'T';                     //если от 0 до 9, то спавн деревьев
                else if (r < 20) (*g).net[i][j] = 'R';                    //если от 10 до 19 спавнится камни
                else (*g).net[i][j] = '.';         //в любых других случаях трава
            }
        }
    }

    random_place(g, 'O');  //выход из пещеры спавнится через функцию

    do { //делаем
        (*g).hero_xpos = rand() % ((*g).x_height_map - 2) + 1;        //рандом x позиция игрока
        (*g).hero_ypos = rand() % ((*g).y_width_map - 2) + 1;         //рандом y позиция игрока
    } while ((*g).net[(*g).hero_xpos][(*g).hero_ypos] != '.');       //проверяем, не в дереве или камне

    (*g).in_cave = 1;       //мы в пещере
    depth++;        //+1 глубина пещеры
}


void draw_map(GameMap *g) {       //вывод карты
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\n");
    
    for (int i = 0; i < (*g).x_height_map; i++) {       //цикл по x
        for (int j = 0; j < (*g).y_width_map; j++) {      //цикл внутри цикла по y
            if (i == (*g).hero_xpos && j == (*g).hero_ypos) {         //если это клетка героя по x,y, 
                                                                      //то рисуем героя поверх карты (координаты героя лежат отдельно)
                                                                      //в саму карту не вписываем @, карта остаётся той же
                printf("@ ");                  //рисуем героя 
            } else { 
                printf("%c ", (*g).net[i][j]); //иначе рисуем остальное дерево, камни, костёр/лагер  из таблицы net
            }
        }
        printf("\n");
    }

    if ((*g).in_cave == 0) {
        printf("Локация: Лес\n");
    }
    else {
        printf("Локация: Пещера\n");
    }
    printf("Карт пройдено: %d\n", journey_cards); //карт пройдено
    printf("Глубина: %d\n", depth);         //глубина
}


int move_hero(GameMap *g, int dx, int dy) {    //создаём функцию которая принимает указатель коробки, направление по x/y
    int new_x = (*g).hero_xpos + dx;       //топаем по x высоте
    int new_y = (*g).hero_ypos + dy;       //топаем по y ширине

    if (new_x < 0 || new_x >= (*g).x_height_map || new_y < 0 || new_y >= (*g).y_width_map) {    //если новый x < 0(уходит за высоту вверх) или x>=высоты карты(уходит вниз)
                                                                                                //или новый y<0(уходит за ширину влево) или y>=ширины карты(уходит за ширину вправо)
        return 0; //незя за карту, ничего не делаем
    }

    char target = (*g).net[new_x][new_y];      //создаём символ в котором написано что будет на новом шаге из коробки net

    if (target == '#' || target == 'T' || target == 'R') {       //если забор!? дерево!? камень!?
        return 0; //нельзя идти, ничего не делаем
    }

    (*g).hero_xpos = new_x;     //смена координат x героя
    (*g).hero_ypos = new_y;     //смена координат y героя

    if (target == 'O') {         //если вход есть
        return 2;          //возвращаем 2 если есть
    }

    return 1; //обычный шаг по карте
}


void adventure() { //функция, отвечающая за приключение
    GameMap world;              //создаём пустую коробку мира
    generate_forest(&world);        //создаём лес внутри коробки

    int running = 1;            //создаём число - флаг работы цикла
    char walk;                   //создаём символ движения

    while (running == 1) { //пока в цикле мира
        draw_map(&world);               //отображаем карту с помощью функции
        printf("Действие (W/A/S/D, Q - выход,R = побег): "); //пишем управление как ходить
        
        if (scanf(" %c", &walk) != 1) {                 //читаем букву(убираем все пробелы в scanf из буфера памяти) 
            while (getchar() != '\n');                  //если не то ввели
            continue; //скип
        }

        int dx = 0, dy = 0;              //координаты, пока равны 0
        if (walk == 'w' || walk == 'W') dx = -1;                 //вверх(-1, так как чтобы пойти вверх нужно считать строку минус один)
        else if (walk == 's' || walk == 'S') dx = 1;                  //вниз
        else if (walk == 'a' || walk == 'A') dy = -1;                     //влево
        else if (walk == 'd' || walk == 'D') dy = 1;                      //вправо
        else if (walk == 'q' || walk == 'Q') {                            //домой, Уолтер
            if (world.in_cave == 0 && world.net[world.hero_xpos][world.hero_ypos] == '^') {              //если в лесу и на костре и на x,y игроках координатах стоит костёр
                printf("Выход из приключения.\n");
                 printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
                running = 0;            //выходим из while
            } else if (world.in_cave == 1) {                //иначе если в пещере
                printf("В пещере нельзя выйти из приключения, ищите выход!\n");
                my_pause();
            } else {                    //в лесу, но не костёр
                printf("Домой можно только с костром!\n");
                my_pause();
            }
            continue;
        }
        else if (walk == 'r' || walk == 'R') {                            //побег если багованая карта, но с потерей ресурсов
            printf("Убежал от проблем... но потерял что-то одно по дороге...\n");
            int random_slot_lost = rand() % 10; //случайный слот от 0 до 9
            if (inventory[random_slot_lost] >0) {
                inventory[random_slot_lost] -= 1; //потерял рандомный предмет в кол-во 1 штука
                if (inventory[random_slot_lost] == 0) {
                    inventory_id[random_slot_lost] = 0;
                }
            }
            my_pause();
            running = 0; //выходим из while
            continue;
        }
        else {
            printf("Не знаю такой команды.\n");
            continue; //продолжаем и ждём других движений
        }

        int result = move_hero(&world, dx, dy); //создаём результат, функцию куда идёт персонаж
        if (result == 0) {
            printf("Нет возможности идти: препятствие.\n");
        } else if (result == 2) {                        //наступили на О(пещеру)
            if (world.in_cave == 0) {              //если были в лесу
                free_map(world.net, world.x_height_map);          //память освобождаем, удаляем карту
                generate_cave(&world);                         //а уже затем генерируем пещеру в структуре-коробке
                printf("Вы спустились в пещеру... Глубина: %d\n", depth);
                my_pause();
            } else {                                               //если были в пещере и наступили на o(пещеру)
                free_map(world.net, world.x_height_map);           //память освобождаем, удаляем карту
                generate_forest(&world);                               //генерируем новую карту леса
                depth = 0;                                        //глубина 0 теперь
                printf("Вы выбрались в новый лес.\n");
                my_pause();
            }
        }
    }

    free_map(world.net, world.x_height_map);                                            //освобождаем память от последней карты
    printf("Память карты очищена. Возврат в меню.\n");
}



int main() {                 
    srand(time(NULL));             //рандом зависит от времени, чтобы не было повторений последовательности
    SetConsoleCP(65001); //русский язык
    SetConsoleOutputCP(65001); //русский язык
    load__item_names(); //загрузка название предметов
    printf("Введите имя фермера: ");
    fgets(farmer_name, sizeof(farmer_name), stdin); //читаем имя/скок символов/ввод
    farmer_name[strcspn(farmer_name, "\n")] = '\0'; //убираем \n
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
            my_pause();
            continue;
        } //защита от дурака
        if (pick >10 || pick <0) {
            printf("Неверный пункт, введите заново\n");
            my_pause();
            continue;
        } //2 защита от дурака


        switch (pick) { // кейсы для меню
            case 0: //выход
                printf("   Выход   \n");
                break;

            case 1: //время
                show_time();
                my_pause();
                break;

            case 2: //работа по часам
                work_time();
                break;

            case 3: //просмотр инвентаря
                show_inventory();
                my_pause();
                break;

            case 4: //кладём предмет в слот
                put_item();
                my_pause();
                break;

            case 5: //выбросить предмет
                drop_item();
                my_pause();
                break;
            case 6: //очистка мусора
                clear_trash();
                my_pause();
                break;
            case 7: //поиск предмета по имени
                getchar();
                search_n_name();
                getchar();
                break;
            case 8: //запись в дневник
                write_diary();
                getchar();
                break;
            case 9: //доска объявлений
                bulletin_board();
                my_pause();
                break;
            case 10: //отправиться в приключение 
                adventure();
                break;
        }
    }
return 0;
}