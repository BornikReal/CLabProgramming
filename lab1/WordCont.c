#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>

void errMes()
{
    printf("Неверо введены аргументы. Использование: WordCont.exe [OPTION] filename\n");
    printf("Опции:\n");
    printf("-l, --lines вывод только количества строк\n");
    printf("-c, --bytes вывод размера файла в байтах\n");
    printf("-w, --words вывод количества слов\n");
}

int main(int argc, char *argv[])
{
    FILE *myfile;
    long long int filename_position = -1;
    bool lines = false, bytes = false, words = false;
    if (argc == 1)
    {
        errMes();
        return 1;
    }
    for (int i = 1; i < argc; i++)
    {
        if (((!strcmp(argv[i], "-l")) || (!strcmp(argv[i], "--lines"))) && (!lines))
            lines = true;
        else if ((!strcmp(argv[i], "-c")) || (!strcmp(argv[i], "--bytes")) && (!bytes))
            bytes = true;
        else if ((!strcmp(argv[i], "-w")) || (!strcmp(argv[i], "--words")) && (!words))
            words = true;
        else
        {
            filename_position = i;
            if (i != (argc - 1))
            {
                errMes();
                return 1;
            }
            // myfile = fopen(argv[filename_position], "r");
            // myfile = fopen("dasdas", "r");
            if (!myfile)
            {
                printf("Файл %s не найден", argv[filename_position]);
                return 1;
            }
            break;
        }
    }
    if (filename_position == -1)
    {
        errMes();
        return 1;
    }
    if (lines)
    {
        int num_of_lines = 1;
        bool not_empty = true;
        char prev = '\n', cur;
        fseek(myfile, 0, SEEK_SET);
        while (!feof(myfile))
        {
            not_empty = false;
            cur = fgetc(myfile);
            if ((cur == '\n') && (prev != '\n'))
            {
                if (prev == cur)
                    printf("true");
                num_of_lines++;
            }
            if (cur != EOF)
                prev = cur;
        }
        if (prev == '\n')
            num_of_lines--;
        printf("Количество строк в файле: %d\n", num_of_lines);
    }
    if (bytes)
    {
        long unsigned int *size_of_file = NULL;
        myfile = fopen(argv[filename_position], "r");
        fseek(myfile, 0, SEEK_END);
        int x = ftell(myfile);
        printf("Размер файла в байтах: %d\n", x);
    }
    if (words)
    {
        int num_of_words = 0;
        char sym;
        fseek(myfile, 0, SEEK_SET);
        bool flag = isspace(fgetc(myfile));
        while (!feof(myfile))
        {
            sym = fgetc(myfile);
            if (isspace(sym) && !flag)
            {
                flag = true;
                num_of_words++;
            }
            else if (!isspace(sym) && flag)
                flag = false;
        }
        printf("Количество слов в файле: %d\n", num_of_words);
    }
    fclose(myfile);
    return 0;
}