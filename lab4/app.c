#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>

typedef struct {
    char frame_id[5];
    int frame_size;
    char *frame_cont;
    int pos;
} frame;
frame list_of_frames[100];
int num = 0;
int size_of_tag = 0;

void print_str(char *str_to_print, int size) {
    for (int i = 0; i < size; i++)
        printf("%c", str_to_print[i]);
    printf("\n");
}

void show_frame(frame* cur_frame) {
    printf("Frame ID: %s\nFrame size: %d\n", cur_frame -> frame_id, cur_frame->frame_size);
    printf("Frame content: ");
    print_str(cur_frame -> frame_cont, cur_frame -> frame_size);
}

void show_all_frames() {
    printf("---------------------------\n");
    for (int i = 0; i < num; i++) {
        show_frame(&list_of_frames[i]);
        printf("---------------------------\n");
    }
}

int find_frame(char* frame_id) {
    for (int i = 0; i < num; i++)
        if (!strcmp(list_of_frames[i].frame_id, frame_id))
            return i;
    return -1;
}

int change_frame(char* frame_id, char* frame_new_cont) {
    int fpos = find_frame(frame_id);
    if (fpos != -1) {
        int dif = strlen(frame_new_cont) - list_of_frames[fpos].frame_size;
        list_of_frames[fpos].frame_size = strlen(frame_new_cont);
        strcpy(list_of_frames[fpos].frame_cont, frame_new_cont);
        for (int i = (fpos + 1); i < num; i++)
            list_of_frames[i].pos += dif;
        return dif;
    }
    return -1;
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");
    int mode;
    char* filepath;
    char* input1;
    char* input2;
    if (argc == 1) {
        printf("Right usage: --show - show all frames\n");
        printf("--set=prop_name --value=prop_value - set new value of some frame\n");
        printf("--get=prop_name - show exactly frame\n");
        return 1;
    }
    if (argc > 4) {
        printf("Too many arguents!");
        return 1;
    }
    if (strstr(argv[1], "--filepath=") && strcmp(argv[1], "--filepath=")) {
        filepath = malloc(sizeof(char) * (strlen(argv[1]) - 9));
        strcpy(filepath, argv[1] + 11);
        mode = 0;
    }
    else {
        printf("Argument \"--filepath\" not found");
        return 1;
    }
    if (argc == 3) {
        if (!strcmp(argv[2], "-h") || !strcmp(argv[2], "--help")) {
            printf("Right usage: --show - show all frames\n");
            printf("--set=prop_name --value=prop_value - set new value of some frame\n");
            printf("--get=prop_name - show exactly frame\n");
            return 0;
        }
        else if (!strcmp(argv[2], "--show"))
            mode = 0;
        else if (strstr(argv[2], "--get=") && strcmp(argv[2], "--get=")) {
            input1 = malloc(sizeof(char) * (strlen(argv[2]) - 5));
            strcpy(input1, argv[2] + 6);
            mode = 1;
        }
        else {
            printf("Wrong argumnet. Use \"app.exe -h\" to get info about argumnets.");
            return 1;
        }
    }
    else if (argc == 4) {
        if (strstr(argv[2], "--set=") && strcmp(argv[2], "--set=")) {
            input1 = malloc(sizeof(char) * (strlen(argv[2]) - 5));
            strcpy(input1, argv[2] + 6);
            mode = 2;
        }
        else {
            printf("Wrong argumnet. Use \"app.exe -h\" to get info about argumnets.");
            return 1;
        }
        if (strstr(argv[3], "--value=") && strcmp(argv[3], "--value=")) {
            input2 = malloc(sizeof(char) * (strlen(argv[3]) - 7));
            strcpy(input2, argv[3] + 8);
            mode = 2;
        }
        else {
            printf("Wrong argumnet. Use \"app.exe -h\" to get info about argumnets.");
            return 1;
        }
    }
    FILE* musicf;
    musicf = fopen(filepath, "rb");
    if (!musicf) {
        printf("File not found");
        return 1;
    }
    char buf_str[100];
    int addit = 128 * 128 * 128;
    for (int i = 0; i < 3; i++)
        buf_str[i] = fgetc(musicf);
    if (!((buf_str[0] == 'I') && (buf_str[1] == 'D') && (buf_str[2] == '3'))) {
        printf("Not ID3v2 file!");
        return 1;
    }
    fseek(musicf, 3, SEEK_CUR);
    for (int i = 0; i < 4; i++) {
        size_of_tag += addit * fgetc(musicf);
        addit /= 128;
    }
    while (ftell(musicf) < (size_of_tag + 10)) {
        for (int i = 0; i < 4; i++)
            list_of_frames[num].frame_id[i] = fgetc(musicf);
        list_of_frames[num].frame_id[4] = '\0';
        if (((list_of_frames[num].frame_id[3] < 'A') || (list_of_frames[num].frame_id[3] > 'Z')) && ((list_of_frames[num].frame_id[3] < '0') || (list_of_frames[num].frame_id[3] > '9')) && ((list_of_frames[num].frame_id[3] < 'А') || (list_of_frames[num].frame_id[3] > 'Я')))
            continue;
        list_of_frames[num].pos = ftell(musicf) - 4;
        addit = 128 * 128 * 128;
        list_of_frames[num].frame_size = 0;
        for (int i = 0; i < 4; i++) {
            list_of_frames[num].frame_size += addit * fgetc(musicf);
            addit /= 128;
        }
        fseek(musicf, 2, SEEK_CUR);
        if (list_of_frames[num].frame_id[0] != 'T') {
            fseek(musicf, list_of_frames[num].frame_size, SEEK_CUR);
            continue;
        }
        list_of_frames[num].frame_cont = malloc(list_of_frames[num].frame_size);
        for (int i = 0; i < list_of_frames[num].frame_size; i++)
            list_of_frames[num].frame_cont[i] = fgetc(musicf);
        num++;
    }
    if (mode == 0)
        show_all_frames();
    else if (mode == 1) {
        bool check = true;
        for (int i = 0; i < num; i++) {
            if (!strcmp(list_of_frames[i].frame_id, input1)) {
                check = false;
                show_frame(&list_of_frames[i]);
                break;
            }
        }
        if (check) {
            printf("Frame not found");
            return 1;
        }
    }
    else if (mode == 2) {
        int pos1 = find_frame(input1);
        if (pos1 == -1) {
            printf("Frame not found");
            return 1;
        }
        int dif = change_frame(input1, input2);
        FILE* musico;
        char *temp_path = malloc(strlen(getenv("TEMP")) + strlen("\\temp.mp3") + 1);
        strcpy(temp_path, getenv("TEMP"));
        strcat(temp_path, "\\temp.mp3");
        musico = fopen(temp_path, "wb");
        fseek(musicf, 3, SEEK_SET);
        fprintf(musico, "ID3");
        for (int i = 0; i < 3; i++)
            fputc(fgetc(musicf), musico);
        char temp[4];
        int temp1 = size_of_tag + dif + 1;
        for (int i = 0; i < 4; i++) {
            temp[i] = temp1 % 128;
            temp1 /= 128;
        }
        for (int i = 3; i >= 0; i--)
            fputc(temp[i], musico);
        fseek(musicf, 4, SEEK_CUR);
        for (int i = 10; i < list_of_frames[pos1].pos + 4; i++)
            fputc(fgetc(musicf), musico);
        temp1 = list_of_frames[pos1].frame_size + 1;
        for (int i = 0; i < 4; i++) {
            temp[i] = temp1 % 128;
            temp1 /= 128;
        }
        for (int i = 3; i >= 0; i--)
            fputc(temp[i], musico);
        fputc(fgetc(musicf), musico);
        fputc(fgetc(musicf), musico);
        fputc(0, musico);
        for (int i = 0; i < strlen(list_of_frames[pos1].frame_cont); i++)
            fputc(list_of_frames[pos1].frame_cont[i], musico);
        fseek(musicf, list_of_frames[pos1].frame_size - dif + 4, SEEK_CUR);
        for (int i = ftell(musicf); i < size_of_tag; i++)
            fputc(fgetc(musicf), musico);
        char c;
        while (fscanf(musicf, "%c", &c) != EOF) {
            fprintf(musico, "%c", c);
        }
        fclose(musico);
        fclose(musicf);
        remove(filepath);
        rename(temp_path, filepath);
    }
    return 0;
}