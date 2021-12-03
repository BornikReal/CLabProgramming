#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>
// #include <unistd.h>

typedef struct {
    char frame_id[5];
    int frame_flag[2];
    int frame_size;
    char *frame_cont;
    int pos;
} frame;
frame list_of_frames[100];
int num = 0;

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

void change_frame(char* frame_id, char* frame_new_cont) {
    int fpos = find_frame(frame_id);
    if (fpos != -1) {
        int dif = strlen(frame_new_cont) - list_of_frames[fpos].frame_size + 1;
        list_of_frames[fpos].frame_size = strlen(frame_new_cont) + 1;
        strcpy(list_of_frames[fpos].frame_cont, frame_new_cont);
        for (int i = (fpos + 1); i < num; i++)
            list_of_frames[i].pos += dif;
    }
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
        filepath = malloc(sizeof(char) * (strlen(argv[1]) - 10));
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
    // printf("%s\n", getenv("TEMP"));
    FILE* musicf;
    musicf = fopen(filepath, "rb");
    if (!musicf) {
        printf("File not found");
        return 1;
    }
    char buf_str[100];
    int size_of_tag = 0, addit = 128 * 128 * 128;
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
        addit = 128 * 128 * 128;
        list_of_frames[num].frame_size = 0;
        for (int i = 0; i < 4; i++) {
            list_of_frames[num].frame_size += addit * fgetc(musicf);
            addit /= 128;
        }
        list_of_frames[num].frame_flag[0] = fgetc(musicf);
        list_of_frames[num].frame_flag[1] = fgetc(musicf);
        if (list_of_frames[num].frame_id[0] != 'T') {
            fseek(musicf, list_of_frames[num].frame_size, SEEK_CUR);
            continue;
        }
        list_of_frames[num].frame_cont = malloc(list_of_frames[num].frame_size);
        for (int i = 0; i < list_of_frames[num].frame_size; i++)
            list_of_frames[num].frame_cont[i] = fgetc(musicf);
        list_of_frames[num].pos = ftell(musicf);
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
        if (find_frame(input1) == -1) {
            printf("Frame not found");
            return 1;
        }
        change_frame(input1, input2);
        FILE* musico;
        musico = fopen(filepath, "wb");
        // show_frame(&list_of_frames[find_frame(input1)]);
    }
    return 0;
}