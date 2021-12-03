#include <stdio.h>
#include <time.h>
#include <string.h>

const char *month[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
const int daysinmonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
int delay[2000000];

void copy(char first[], char second[], int count) {
    strncpy(first, "\0\0\0\0\0\0", 6);
    strncpy(first, second, count);
}

int monthtonum(char cur[]) {
    for (int i = 0; i < 11; i++)
        if (strcmp(cur, month[i]) == 0)
            return i;
}

int datetosec(char date[]) {
    char cur[6];
    int sec = 0;

    copy(cur, date, 2);
    sec += (atoi(cur, cur + 2, 10) - 1) * 86400;

    copy(cur, date + 3, 3);
    for (int i = 0; i < monthtonum(cur); i++)
        sec += daysinmonth[i] * 86400;

    copy(cur, date + 7, 4);
    for (int i = 1970; i < atoi(cur, cur + 4, 10); i++) {
        if ((i % 4 == 0) && (i % 100 != 0) || (i % 400 == 0))
            sec += 31622400;
        else
            sec += 31536000;
    }
    
    copy(cur, date + 12, 2);
    sec += atoi(cur, cur + 2, 10) * 3600;

    copy(cur, date + 15, 2);
    sec += atoi(cur, cur + 2, 10) * 60;

    copy(cur, date + 18, 2);
    sec += atoi(cur, cur + 2, 10);

    
    copy(cur, date + 21, 1);
    
    int typ;
    if (strcmp(cur, "-") == 0)
        typ = -1;
    else
        typ = 1;

    copy(cur, date + 22, 2);
    sec += typ * (atoi(cur, cur + 2, 10) * 3600);

    copy(cur, date + 24, 2);
    sec += typ * (atoi(cur, cur + 2, 10) * 60);
    return sec;
}

int main(int argc, char *argv[]) {
    FILE *logFile;
    logFile = fopen("access_log_Jul95", "r");
    char curstr[250], date[27], status[4], errorstr[40000];
    int search, pos, amountspaces, amount500 = 0, glob_pos = -1, prev = 0, cur, gap = 10, first = 0, time = 0, max = 0, count = 0;
    if (argc == 1) {
        printf("Enter the gap: ");
        scanf("%d", &gap);
    }
    else if (argc == 2) {
        if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
            printf("Usage: main.exe [gap]\n");
            return 0;
        }
        else if(strspn(argv[1], "0123456789") != strlen(argv[1])) {
            printf("Gap can be only usigned integer!\n");
            return 1;
        }
        else
            gap = atoi(argv[1]);
    }
    else {
        printf("To many arguments. Right usage: main.exe [gap]\n");
        return 1;
    }
    while (!feof(logFile)) {
        fgets(curstr, 250, logFile);
        search = -1;
        for (int i = 0; ((curstr[i] != ']') && (i < strlen(curstr))); i++) {
            if (curstr[i] == '[')
                search = i + 1;
            if (search != -1)
                date[i - search] = curstr[i];
        }
        cur = datetosec(date);
        if (glob_pos != -1) {
            delay[glob_pos] = cur - prev;
        }
        amountspaces = 0;
        for (int i = strlen(curstr) - 1; i >= 0; i--) {
            if (curstr[i] == ' ')
                amountspaces++;
            if (amountspaces == 2) {
                if (curstr[i + 1] == '5') {
                    amount500++;
                    strcat(errorstr, curstr);
                }
                break;
            }
        }
        prev = cur;
        if (glob_pos != -1) {
            time += delay[glob_pos];
            count++;
            if (time > gap) {
                if (count > max)
                    max = count;
                for (int j = first; j <= glob_pos; j++) {
                    time -= delay[j];
                    first++;
                    count--;
                    if (time <= gap)
                        break;
                }
            }
        }
        glob_pos++;
    }
    printf("%s", errorstr);
    printf("Number of requests with 5xx status %d\n", amount500);
    printf("Maximal number of requests in gap of %d sec. is %d\n", gap, max);
    fclose(logFile);
    return 0;
}