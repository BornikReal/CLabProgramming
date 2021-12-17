#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

void error_exit()
{
    printf("Неправильно введены аргументы. Используйте \"expander.exe -h\" для получения справки.");
    exit(1);
}

void check_file(FILE* file, char* filename) {
    if (!file) {
        printf("Файл \"%s\" не найден.", filename);
        exit(1);
    }
}

int main(int argc, char* argv[])
{
    char *archive_name;
    char *dir = NULL;
    int mode = -1, file_list;
    if ((argc < 3) && (argc != 2))
        error_exit();
    else if ((argc == 2) && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")))
    {
        printf("List of arguments:\n");
        printf("--file FILE - имя файлового архива с которым будет работать архиватор,\n");
        printf("--create - команда для создания файлового архив,\n");
        printf("--extract - команда для извлечения из файлового архива файлов,\n");
        printf("--list - команда для предоставления списка файлов, хранящихся в архиве,\n");
        printf("FILE1 FILE2 ... FILEN - свободные аргументы для передачи списка файлов для запаковки.\n");
        return 0;
    }
    else {
        int cur_arg = 1;
        while (cur_arg < argc)
        {
            if (!strcmp(argv[cur_arg], "--file"))
            {
                if (cur_arg + 1 == argc)
                    error_exit();
                else
                    archive_name = argv[cur_arg + 1];
                cur_arg += 2;
            }
            else if (!strcmp(argv[cur_arg], "--output"))
            {
                if (cur_arg + 1 == argc)
                    error_exit();
                else {
                    if (mode != 1)
                        error_exit();
                    if (dir != NULL)
                        error_exit();
                    dir = argv[cur_arg + 1];
                }
                cur_arg += 2;
            }
            else if (!strcmp(argv[cur_arg], "--create"))
            {
                if (cur_arg + 1 == argc)
                    error_exit();
                else {
                    if (mode != -1)
                        error_exit();
                    mode = 0;
                    file_list = cur_arg + 1;
                    break;
                }
                cur_arg += 2;
            }
            else if (!strcmp(argv[cur_arg], "--extract"))
            {
                if (mode != -1)
                    error_exit();
                mode = 1;
                cur_arg++;
            }
            else if (!strcmp(argv[cur_arg], "--list"))
            {
                if (mode != -1)
                    error_exit();
                mode = 2;
                cur_arg++;
            }
            else
                error_exit();
        }
    }
    if (mode == -1)
        mode = 2;
    if (mode == 0) {
        FILE* cur_file;
        FILE* archive = fopen(archive_name, "wb");
        fprintf(archive, "ARC%d|", (argc - file_list));
        for (int i = file_list; i < argc; i++)
        {
            cur_file = fopen(argv[i], "rb");
            if (!cur_file) {
                printf("Файл \"%s\" не найден.", argv[i]);
                fclose(archive);
                remove(archive_name);
                return 1;
            }
            fseek(cur_file, 0, SEEK_END);
            char right_filename[300];
            int name_start = 0;
            for (int j = (strlen(argv[i]) - 1); j >= 0; j--) {
                if (argv[i][j] == '/') {
                    name_start = j + 1;
                    break;
                }
            }
            if (name_start != 0) {
                for (int j = name_start; j < strlen(argv[i]); j++)
                    right_filename[j - name_start] = argv[i][j];
                right_filename[strlen(argv[i]) - name_start] = '\0';
            }
            fprintf(archive, "%s|%d|", right_filename, ftell(cur_file));
            fclose(cur_file);
        }
        fputc('|', archive);
        for (int i = file_list; i < argc; i++)
        {
            cur_file = fopen(argv[i], "rb");
            char buf[1000];
            size_t num;
            while(num = fread(buf, sizeof(char), sizeof(999), cur_file))
                fwrite(buf, sizeof(char), num, archive);
            fclose(cur_file);
        }
        fclose(archive);
    }
    else if (mode == 1) {
        FILE* cur_file;
        FILE* archive = fopen(archive_name, "rb");
        check_file(archive, archive_name);
        char files[1000];
        fread(files, sizeof(char), sizeof(3), archive);
        files[3] = '\0';
        if (strcmp(files, "ARC")) {
            printf("Файл \"%s\" не является архивом или данный архив повреждён.", archive_name);
            return 1;
        }
        if (dir != NULL)
            mkdir(dir);
        fseek(archive, 3, SEEK_SET);
        int pos = 0, is_file_name = 1, is_file_count = 1, num, counter = 0;
        int *sizes;
        char **names;
        char temp = fgetc(archive), prev = '\0';
        while (temp != feof(archive)) {
            if (temp == '|') {
                if (prev == '|')
                    break;
                files[pos] = '\0';
                if (is_file_count) {
                    num = strtol(files, NULL, 10);
                    sizes = (int *)malloc(sizeof(int) * num);
                    names = (char **)malloc(sizeof(char *) * num);
                    is_file_count = 0;
                }
                else {
                    if (is_file_name) {
                        names[counter] = (char *)malloc(sizeof(char) * (pos + 1));
                        strcpy(names[counter], files);
                    }
                    else {
                        sizes[counter] = strtol(files, NULL, 10);
                        counter++;
                    }
                    is_file_name = (is_file_name + 1) % 2;
                }
                pos = 0;
            }
            else {
                files[pos] = temp;
                pos++;
            }
            prev = temp;
            temp = fgetc(archive);
        }
        for (int i = 0; i < num; i++) {
            if (dir != NULL) {
                char* filename = (char *)malloc(sizeof(char) * (strlen(dir) + strlen(names[i]) + 2));
                strcpy(filename, dir);
                strcat(filename, "/");
                strcat(filename, names[i]);
                cur_file = fopen(filename, "wb");
                free(filename);
            }
            else
                cur_file = fopen(names[i], "wb");
            free(names[i]);
            for (int j = 0; j < sizes[i]; j++)
                fputc(fgetc(archive), cur_file);
            fclose(cur_file);
        }
        free(names);
        free(sizes);
        fclose(archive);
    }
    else {
        FILE* archive = fopen(archive_name, "rb");
        check_file(archive, archive_name);
        char files[1000];
        fread(files, sizeof(char), sizeof(3), archive);
        files[3] = '\0';
        if (strcmp(files, "ARC")) {
            printf("Файл \"%s\" не является архивом или данный архив повреждён.", archive_name);
            return 1;
        }
        printf("Список файлов, запакованных в этом архиве:\n");
        fseek(archive, 3, SEEK_SET);
        int pos = 0, is_file_name = 0;
        char temp = fgetc(archive), prev = '\0';
        while (temp != feof(archive)) {
            if (temp == '|') {
                if (prev == '|')
                    break;
                files[pos] = '\0';
                if (is_file_name)
                    printf("%s\n", files);
                is_file_name = (is_file_name + 1) % 2;
                pos = 0;
            }
            else {
                files[pos] = temp;
                pos++;
            }
            prev = temp;
            temp = fgetc(archive);
        }
        fclose(archive);
    }
    return 0;
}