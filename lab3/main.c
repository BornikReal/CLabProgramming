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

long long int datetosec(char date[]) {
    struct tm* ptr;
    time_t t;
    char cur[6];
    t = time(NULL);
    // ptr = localtime(&t);

    copy(cur, date, 2);
    ptr -> tm_mday = atoi(cur);
    // printf("%s\n", cur);

    copy(cur, date + 3, 3);
    ptr -> tm_mon = monthtonum(cur);
    // printf("%s\n", cur);
    // printf("%d\n", monthtonum(cur));

    copy(cur, date + 7, 4);
    ptr -> tm_year = atoi(cur) - 1900;
    // printf("%s\n", cur);
    
    copy(cur, date + 12, 2);
    ptr -> tm_hour = atoi(cur);
    // printf("%s\n", cur);

    copy(cur, date + 15, 2);
    ptr -> tm_min = atoi(cur);
    // printf("%s\n", cur);

    copy(cur, date + 18, 2);
    ptr -> tm_sec = atoi(cur);
    // ptr -> tm_isdst = 121;
    // printf("%s\n", cur);
    
    copy(cur, date + 21, 1);
    // printf("%s\n", cur);
    int typ, tmz = 0;
    if (strcmp(cur, "-") == 0)
        typ = -1;
    else
        typ == 1;

    copy(cur, date + 22, 2);
    // printf("%s\n", cur);

    copy(cur, date + 24, 2);
    // printf("%s\n", cur);

    char str[100];
    strftime(str, 100, "%d/%b/%Y:%H:%M:%S", ptr);
    printf("%s\n", str);

    // printf("%s", asctime(ptr));
    return 0;
}

int main() {
    // struct tm* ptr;
    // time_t t;
    // t = time(NULL);
    // ptr = localtime(&t);
    // printf("%s", asctime(ptr));
    // // ptr = gmtime(&t);
    // // ptr -> tm_year = 2010;
    // // ptr -> tm_yday = 2000;
    // ptr -> tm_year = 100;
    // printf("%s", asctime(ptr));

    // char *s, *s1;
    // scanf("%s%s", s, s1);
    // printf("12345678" + 4);
    datetosec("03/Jul/1995:10:50:02 -0400");
    // struct tm timestr;
    // // time(&timestr);
    // timestr.tm_year = 2012;
    // timestr.tm_mon = 11;
    // timestr.tm_mday = 4;
    // timestr.tm_hour = 1;
    // timestr.tm_min = 30;
    // timestr.tm_sec = 0;
    // timestr.tm_isdst = 0;
    // // time_t t = localtime(&timestr);
    // // printf("%lli\n", t);
    // printf("%s\n", asctime(&timestr));
    // // str = asctime(&t);
    // // strftime(str, sizeof(str), "%A %c", localtime(&t));
    // printf("%s\n", asctime(t));
    // char cur[] = "sasdsa";
    // strcat(cur, "1231");
    // printf("%s\n", cur);

    // FILE *logFile;
    // logFile = fopen("access_log_Jul95.log", "r");
    // char *curstr;
    // while (!feof(logFile)) {
    //     fgets(curstr, 500, logFile);
    // //     printf("%d||||||", strcspn(curstr, "["));
    // //     printf("%s", curstr);
    // }
    // fclose(logFile);
    return 0;
}