#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>

typedef struct {
    char frame_id[5];
    char frame_flag[2];
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
    printf("Frame ID: %s\nFrame content: ", cur_frame -> frame_id);
    print_str(cur_frame -> frame_cont, cur_frame -> frame_size);
}

void show_all_frames() {
    for (int i = 0; i < num; i++)
        show_frame(&list_of_frames[i]);
}

int find_frame(char* frame_id) {
    for (int i = 0; i < num; i++)
        if (!strcmp(list_of_frames[i].frame_id, frame_id))
            return i;
    return -1;
}

void change_frame(char* frame_id, char* frame_new_cont) {
    int fpos = find_frame(frame_id);
    int dif = strlen(frame_new_cont) - list_of_frames[fpos].frame_size;
    list_of_frames[fpos].frame_size = strlen(frame_new_cont);
    strcpy(list_of_frames[fpos].frame_cont, frame_new_cont);
    for (int i = (fpos + 1); i < num; i++)
        list_of_frames[num].pos += dif;
}

int main(int argc, char* argv[]) {
    // printf("%s\n", getenv("TEMP"));
    int mode;
    char* input1, input2;
    if (argc == 1) {
        printf("Right usage: --show - отображение всей метаинформации в виде таблицы\n");
        printf("--set=prop_name --value=prop_value - выставляет значение 
                определенного поля метаинформации с именем prop_name в 
                значение prop_value\n");
        printf("--get=prop_name - вывести определенное поле 
                метаинформации с именем prop_name\n");
        return 1;
    }
    if (argc >= 4) {
        printf("Too many arguents!");
        return 1;
    }
    else if (argc == 2) {
        if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
            printf("Right usage: --show - отображение всей метаинформации в виде таблицы\n");
            printf("--set=prop_name --value=prop_value - выставляет значение 
                    определенного поля метаинформации с именем prop_name в 
                    значение prop_value\n");
            printf("--get=prop_name - вывести определенное поле 
                    метаинформации с именем prop_name\n");
            return 0;
        }
        else if (!strcmp(argv[1], "--show"))
            mode = 0;
        // else if (!strcmp(argv[1], "--show"))
        //     mode = 0;
    }
    // for (int i = 0; i < argc; i++) {

    // }
    // char rez;
    // while ((rez = getopt_long(argc,argv,"showW;b:C:d:")) != -1) {
    //     switch (rez){
	// 	case 'a': printf("found argument \"a\".\n");break;
	// 	case 'b': printf("found argument \"b = %s\".\n",optarg);break;
	// 	case 'C': printf("found argument \"C = %s\".\n",optarg);break;
	// 	case 'd': printf("found argument \"D = %s\".\n",optarg);break;
	// 	case '?': printf("Error found !\n"); break;
    //     }
    // }
    setlocale(LC_ALL, "Russian");
    FILE* musicf;
    // test = fopen("%TEMP%");
    musicf = fopen("minecraft.mp3", "rb");
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
        if (list_of_frames[num].frame_id[0] != 'T') {
            fseek(musicf, list_of_frames[num].frame_size, SEEK_CUR);
            continue;
        }
        list_of_frames[num].frame_flag[0] = fgetc(musicf);
        list_of_frames[num].frame_flag[1] = fgetc(musicf);
        list_of_frames[num].frame_cont = malloc(list_of_frames[num].frame_size);
        for (int i = 0; i < list_of_frames[num].frame_size; i++)
            list_of_frames[num].frame_cont[i] = fgetc(musicf);
        list_of_frames[num].pos = ftell(musicf);
        // printf("%d %d %s\n", list_of_frames[num].frame_size, list_of_frames[num].pos, list_of_frames[num].frame_id);
        // print_str(list_of_frames[num].frame_cont, list_of_frames[num].frame_size);
        num++;
    }
    // change_frame("TPE1", "aboba");
    // show_all_frames();
    return 0;
}