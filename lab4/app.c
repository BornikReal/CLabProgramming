#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    char frame_id[4];
    int frame_size;
    char *frame_cont;
    int pos;
} frame;

void print_str(char *str_to_print, int size) {
    for (int i = 0; i < size; i++)
        printf("%c", str_to_print[i]);
    printf("\n");
}

void show_frame(frame* cur_frame) {
    printf("Frame ID: %s\nFrame content: ", cur_frame -> frame_id);
    print_str(cur_frame -> frame_cont, cur_frame -> frame_size);
}

int main(int argc, char* argv[]) {
    FILE* musicf;
    musicf = fopen("minecraft.mp3", "r");
    char buf_str[100];
    int size_of_tag = 0, addit = 128 * 128 * 128, num = 0;
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
    printf("%d\n", size_of_tag);
    frame list_of_frames[100];
    while (ftell(musicf) < (size_of_tag + 10)) {
        for (int i = 0; i < 4; i++)
            list_of_frames[num].frame_id[i] = fgetc(musicf);
        if (((list_of_frames[num].frame_id[3] < 'A') || (list_of_frames[num].frame_id[3] > 'Z')) && ((list_of_frames[num].frame_id[3] < '0') || (list_of_frames[num].frame_id[3] > '9')))
            continue;
        addit = 128 * 128 * 128;
        list_of_frames[num].frame_size = 0;
        for (int i = 0; i < 4; i++) {
            list_of_frames[num].frame_size += addit * fgetc(musicf);
            addit /= 128;
        }
        fseek(musicf, 2, SEEK_CUR);
        list_of_frames[num].frame_cont = malloc(list_of_frames[num].frame_size);
        for (int i = 0; i < list_of_frames[num].frame_size; i++)
            list_of_frames[num].frame_cont[i] = fgetc(musicf);
        list_of_frames[num].pos = ftell(musicf);
        num++;
    }
    for (int i = 0; i < num; i++)
        show_frame(&list_of_frames[i]);
    return 0;
}