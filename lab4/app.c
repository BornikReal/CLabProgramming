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

int main(int argc, char* argv[]) {
    FILE* musicf;
    musicf = fopen("minecraft.mp3", "r");
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
    printf("%d\n", size_of_tag);
    frame new_fr;
    for (int i = 0; i < 4; i++)
        new_fr.frame_id[i] = fgetc(musicf);
    addit = 128 * 128 * 128;
    new_fr.frame_size = 0;
    for (int i = 0; i < 4; i++) {
        new_fr.frame_size += addit * fgetc(musicf);
        addit /= 128;
    }
    fseek(musicf, 2, SEEK_CUR);
    new_fr.frame_cont = malloc(new_fr.frame_size);
    for (int i = 0; i < new_fr.frame_size; i++)
        new_fr.frame_cont[i] = fgetc(musicf);
    printf("%d %s\n", new_fr.frame_size, new_fr.frame_id);
    print_str(new_fr.frame_cont, new_fr.frame_size);
    return 0;
}