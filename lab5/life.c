#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

const int hash_size1 = 50007;
const int hash_size2 = 50041;

/**
 * @brief Функция для преобразования двойного адреса в одиночночный
 * 
 * @param i Номер строки
 * @param j Номер столбца
 * @param wight Количество столбцов в массиве
 * @return int Возвращает одиночный адрес
 */
int add(int i, int j, int wight)
{
    return i * wight + j;
}

/**
 * @brief Целочисленное деление с округлением вверх
 * 
 * @param x Делимое
 * @param y Делитель
 * @return int Результат деления
 */
int delup(int x, int y)
{
    if (x % y == 0)
        return (x / y);
    else
        return ((x / y) + 1);
}

/**
 * @brief Выравнивание ширины до 4 байт
 * 
 * @param x Количество байт
 * @return int Число байт кратное 4
 */
int count_real_wight(int x)
{
    x = delup(x, 8);
    if (x % 4 != 0)
        return (4 - (x % 4)) + x;
    else
        return x;
}

/**
 * @brief Функция вызова сообщения об ошибке аргументов и экстренного выхода из программы.
 * 
 */
void error_exit()
{
    printf("Frong argumnets. Use \"life.exe -h\" for help.");
    exit(1);
}

/**
 * @brief Функция для подсчёта полиноминального хэша
 * 
 * @param array Двумерный массив
 * @param height Количество строк
 * @param wight Количество столбцов
 * @param multipl Множитель
 * @return int Число кратное значению hash_size
 */
int hash(char *array, int height, int wight, int multipl, int del)
{
    int hash_code = 0, p = 1;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < wight; j++)
        {
            hash_code = (hash_code + array[add(i, j, wight)] * p) % del;
            p = (p * multipl) % del;
        }
    }
    return hash_code;
}

/**
 * @brief Функция для проверки существования элемента в хэш сете
 * 
 * @param generation Хэш сет
 * @param sizes Массив размеров
 * @param array Элемент
 * @param height Количество строк
 * @param wight Количество столбцов
 * @return int 0 - если не существет и 1 - если существует
 */
int exists(char *generation[hash_size1], char sizes[hash_size1], char *array, int height, int wight)
{
    int search_index = hash(array, height, wight, 3, hash_size1);
    if (!sizes[search_index])
        return 0;
    return generation[search_index][hash(array, height, wight, search_index, hash_size2)];
}

/**
 * @brief Функция добавления элемента в хэш сэт
 * 
 * @param generation Хэш сет
 * @param sizes Массив размеров
 * @param array Элемент
 * @param height Количество строк
 * @param wight Количество столбцов
 */
void input(char *generation[hash_size1], char sizes[hash_size1], char *array, int height, int wight)
{
    int pos = hash(array, height, wight, 3, hash_size1);
    if (!sizes[pos])
        generation[pos] = calloc(hash_size2, sizeof(char));
    generation[pos][hash(array, height, wight, pos, hash_size2)] = 1;
    sizes[pos]++;
}

/**
 * @brief Функция создания изображения
 * 
 * @param header Заголовок bmp файла
 * @param dir Директория для сохранения итого файла
 * @param array Массив с пикселями
 * @param num Номер изображения
 * @param size Размер заголовка
 * @param height Высота изображения
 * @param wight Ширина изображения
 */
void make_image(char *header, char *dir, char *array, int num, int size, int height, int wight)
{
    char num_of_gen[10];
    sprintf(num_of_gen, "%d", num);
    char *filename = (char *)malloc((strlen(num_of_gen) + strlen(dir) + 6) * sizeof(char));
    strcpy(filename, dir);
    strcat(filename, "/");
    strcat(filename, num_of_gen);
    strcat(filename, ".bmp");
    FILE *OPFile;
    OPFile = fopen(filename, "wb");
    fwrite(header, sizeof(char), size, OPFile);
    int input, mult;
    for (int i = 0; i < height; i++)
    {
        mult = 128;
        input = 0;
        for (int j = 0; j < wight; j++)
        {
            if (mult != 0)
            {
                input += array[add(i, j, wight)] * mult;
                mult /= 2;
            }
            else
            {
                fputc(input, OPFile);
                input = array[add(i, j, wight)] * 128;
                mult = 64;
            }
        }
        fputc(input, OPFile);
        for (int j = delup(wight, 8); j < count_real_wight(wight); j++)
            fputc(0, OPFile);
    }
    fclose(OPFile);
    free(filename);
}

/**
 * @brief Подсчёт количество живых клеток вокруг данной
 * 
 * @param array Массив текущего поколения
 * @param i Номер строки
 * @param j Номер столбца
 * @param height Количество строк
 * @param wight Количество столбцов
 * @return int 
 */
int count_alive(char *array, int i, int j, int height, int wight)
{
    int alive = 0;
    if ((i >= 1) && !array[add(i - 1, j, wight)])
        alive++;
    if ((j >= 1) && !array[add(i, j - 1, wight)])
        alive++;
    if ((i >= 1) && (j >= 1) && !array[add(i - 1, j - 1, wight)])
        alive++;
    if ((i < (height - 1)) && !array[add(i + 1, j, wight)])
        alive++;
    if ((j < (wight - 1)) && !array[add(i, j + 1, wight)])
        alive++;
    if ((i < (height - 1)) && (j < (wight - 1)) && !array[add(i + 1, j + 1, wight)])
        alive++;
    if ((i >= 1) && (j < (wight - 1)) && !array[add(i - 1, j + 1, wight)])
        alive++;
    if ((i < (height - 1)) && (j >= 1) && !array[add(i + 1, j - 1, wight)])
        alive++;
    return alive;
}

/**
 * @brief Функция чтения многобайтового числа из файла
 * 
 * @param file_read Указатель на файл
 * @param num Количество байт для чтения
 * @return int Итоговое число в десятичной системе счисления
 */
int num_reader(FILE *file_read, int num)
{
    int mult = 1, res = 0;
    for (int i = 0; i < num; i++)
    {
        res += fgetc(file_read) * mult;
        mult *= 256;
    }
    return res;
}

/**
 * @brief Проверить правильность входного файла
 * 
 * @param filename Имя входного файла
 * @param dir Название папки
 * @param bmpf Указатель на файл
 */
void check_input(char *filename, char *dir, FILE *bmpf)
{
    if (!bmpf)
    {
        printf("File \"%s\" not found.", filename);
        exit(1);
    }
    DIR *directory = opendir(dir);
    if (!directory)
        mkdir(dir);
    closedir(directory);
    if ((fgetc(bmpf) != 'B') || (fgetc(bmpf) != 'M'))
    {
        printf("Not a BMP file or BMP file is corrupted.");
        exit(1);
    }
    fseek(bmpf, 28, SEEK_SET);
    if (num_reader(bmpf, 2) != 1)
    {
        printf("Not a monochrome image.");
        exit(1);
    }
}

/**
 * @brief Конвертация набора bmp файлов в gif
 * 
 * @param dir Папка с bmp
 * @param z Количество изображений
 */
void gif_conventer(char *dir, int z, int freq)
{
    char counters[10], freqs[10];
    sprintf(counters, "%d", z - 1);
    sprintf(freqs, "%d", freq);
    char *command = (char *)malloc((strlen(counters) + strlen(dir) + strlen("python gif_conventor.py --size= --dir=") + 1) * sizeof(char));
    strcpy(command, "python gif_conventor.py --size=");
    strcat(command, counters);
    strcat(command, " --dir=");
    strcat(command, dir);
    strcat(command, " --freq=");
    strcat(command, freqs);
    system(command);
    free(command);
}

int main(int argc, char *argv[])
{
    FILE *bmpf;
    char *dir = NULL;
    char *first_gen = NULL;
    int max_iter = -2, dump_freq = -2;
    if ((argc < 5) && (argc != 2))
        error_exit();
    if ((argc == 2) && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")))
    {
        printf("List of arguments:\n");
        printf("--input input_file.bmp: monochrome picture in bmp format, storing the initial situation (first generation) of the game,\n");
        printf("--output dir_name: the name of the directory for storing generations of the game in the form of a monochrome picture,\n");
        printf("--max_iter N (optional): the maximum number of generations that a program can emulate,\n");
        printf("--dump_freq N (optional): the frequency with which the program should save generations in the form of an image.\n");
        return 0;
    }
    else
    {
        int cur_arg = 1;
        while (cur_arg < argc)
        {
            if (!strcmp(argv[cur_arg], "--input"))
            {
                if (cur_arg + 1 == argc)
                    error_exit();
                else {
                    if (first_gen != NULL)
                        error_exit();
                    first_gen = argv[cur_arg + 1];
                }
                cur_arg += 2;
            }
            else if (!strcmp(argv[cur_arg], "--output"))
            {
                if (cur_arg + 1 == argc)
                    error_exit();
                else {
                    if (dir != NULL)
                        error_exit();
                    dir = argv[cur_arg + 1];
                }
                cur_arg += 2;
            }
            else if (!strcmp(argv[cur_arg], "--max_iter"))
            {
                if (cur_arg + 1 == argc)
                    error_exit();
                else {
                    if (max_iter != -2)
                        error_exit();
                    max_iter = strtol(argv[cur_arg + 1], NULL, 10);
                    if (max_iter < 1)
                        error_exit();
                }
                cur_arg += 2;
            }
            else if (!strcmp(argv[cur_arg], "--dump_freq"))
            {
                if (cur_arg + 1 == argc)
                    error_exit();
                else {
                    if (dump_freq != -2)
                        error_exit();
                    dump_freq = strtol(argv[cur_arg + 1], NULL, 10);
                    if (dump_freq < 1)
                        error_exit();
                }
                cur_arg += 2;
            }
            else
                error_exit();
        }
    }
    if (max_iter == -2)
        max_iter = -1;
    if (dump_freq == -2)
        dump_freq = 1;
    if ((first_gen == NULL) || (dir == NULL))
        error_exit();
    int depth = 0, wight = 0, height = 0, pos = 0;
    bmpf = fopen(first_gen, "rb");
    check_input(first_gen, dir, bmpf);
    fseek(bmpf, 10, SEEK_SET);
    pos = num_reader(bmpf, 4);
    fseek(bmpf, 18, SEEK_SET);
    wight = num_reader(bmpf, 4);
    height = num_reader(bmpf, 4);
    char *header = (char *)malloc((pos + 1) * sizeof(char));
    fseek(bmpf, 0, SEEK_SET);
    fread(header, sizeof(char), pos, bmpf);
    fseek(bmpf, pos, SEEK_SET);
    char *universe = (char *)malloc(wight * height * sizeof(char));
    char *buf = (char *)malloc(wight * height * sizeof(char));
    char *generation[hash_size1];
    char sizes[hash_size1];
    for (int i = 0; i < hash_size1; i++)
        sizes[i] = 0;
    int counter, trans, temp1, size = delup(wight, 8);
    int *temp = (int *)malloc(size * sizeof(int));
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < count_real_wight(wight); j++)
        {
            if (j < size)
                temp[j] = fgetc(bmpf);
            else
                fgetc(bmpf);
        }
        counter = 0;
        for (int j = 0; j < size; j++)
        {
            trans = 128;
            temp1 = temp[j];
            for (int z = 1; z <= 8; z++)
            {
                if (counter == wight)
                    break;
                if (temp1 >= trans)
                {
                    universe[add(i, counter, wight)] = 1;
                    temp1 -= trans;
                }
                else
                    universe[add(i, counter, wight)] = 0;
                trans /= 2;
                counter++;
            }
        }
    }
    free(temp);
    int alive, z;
    for (z = 1; z != (max_iter + 1); z++)
    {
        int is_null = 1;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < wight; j++)
            {
                alive = count_alive(universe, i, j, wight, height);
                if (alive == 3)
                    buf[add(i, j, wight)] = 0;
                else if (alive != 2)
                    buf[add(i, j, wight)] = 1;
                else
                    buf[add(i, j, wight)] = universe[add(i, j, wight)];
                if (!buf[add(i, j, wight)])
                    is_null = 0;
            }
        }
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < wight; j++)
            {
                universe[add(i, j, wight)] = buf[add(i, j, wight)];
            }
        }
        if (exists(generation, sizes, buf, height, wight) || is_null)
            break;
        else
            input(generation, sizes, buf, height, wight);
        if (z % dump_freq == 0)
            make_image(header, dir, buf, z, pos, height, wight);
    }
    gif_conventer(dir, z, dump_freq);
    free(buf);
    free(universe);
    fclose(bmpf);
    return 0;
}