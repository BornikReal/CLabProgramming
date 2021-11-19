#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    FILE* musicf;
    musicf = fopen("minecraft.mp3", "r");
    char buf_str[100];
    int size_of_tag;
    buf_str[0] = fgetc(musicf);
    buf_str[1] = fgetc(musicf);
    buf_str[2] = fgetc(musicf);
    if (!((buf_str[0] == 'I') && (buf_str[1] == 'D') && (buf_str[2] == '3'))) {
        printf("Not ID3v2 file!");
        return 1;
    }
    fseek(musicf, 6, SEEK_SET);
    size_of_tag = 128 * 128 * 128 * fgetc(musicf);
    size_of_tag += 128 * 128 * fgetc(musicf);
    size_of_tag += 128 * fgetc(musicf);
    size_of_tag += fgetc(musicf);
    printf("%d\n", size_of_tag);
    return 0;
}