#include <stdio.h>
#include <time.h>
#include <string.h>

const char *month[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
const int daysinmonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

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
    printf("%d\n", sec);
    return 0;
}

int main() {
    FILE *logFile;
    logFile = fopen("access_log_Jul95.log", "r");
    char curstr[500], date[27], status[4];
    int search, pos, amountspaces, amount500 = 0;
    while (!feof(logFile)) {
        fgets(curstr, 500, logFile);
        search = -1;
        for (int i = 0; curstr[i] != ']'; i++) {
            if (curstr[i] == '[')
                search = i + 1;
            if (search != -1)
                date[i - search] = curstr[i];
        }
        amountspaces = 0;
        for (int i = strlen(curstr) - 1; i >= 0; i--) {
            if (curstr[i] == ' ')
                amountspaces++;
            if (amountspaces == 2) {
                if (curstr[i + 1] == '5')
                    amount500++;
                break;
            }
        }
    }
    printf("%d\n", amount500);
    fclose(logFile);
    return 0;
}