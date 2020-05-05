#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

struct Schedule{
    char orderNum[10];
    char dueDate[11];
    int quantity;
    char productName[100];
};

int startDate[3], endDate[3], numOrders = 0;
struct Schedule schedule[100];
const int SIZE = 80;

long totalday(int year,int month,int day);
void runcmd(char command[],int count);
void addPEIOD(char arr[]);
void addDate(char input[3][SIZE], int x, int start, int end, bool stDate);
void addORDER(char arr[]);
void addBATCH(char arr[], int count);

int main(int argc,char *argv[]){
    char command[100];
    printf("~~WELCOME TO PLS~~\n\n");
    printf("Please enter:\n");
    while(1){
        fgets(command, 100, stdin);
        if(strncmp(command, "exit", 4) == 0){
            break;
        }
        if(strncmp(command, "-1", 2) != 0){
            runcmd(command,sizeof(command)/sizeof(int));
            strcpy(command, "-1");
            printf("Please enter:\n");
        }
    }
    return 0;
}

long totalday(int year, int month, int day)
{
    int months[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    int i = 0;
    long total = 0L;
    total = (year - 1) * 365L + (year - 1) / 4 - (year - 1) / 100 + (year - 1) / 400;
    if (!(year % 4) && year % 100 || !(year % 400))
        months[i]++;
    for (i = 0; i < month - 1; i++)
        total += months[i];
    total += day;
    return total;
}

void runcmd(char command[],int count){
    char *ptr = strstr(command,"addPEIOD");
    if(ptr != NULL){
        addPEIOD(command);
    }
    else{
        ptr=strstr(command,"addORDER");
        if(ptr != NULL){
            addORDER(command); //count
        }
        else{
            ptr=strstr(command,"addBATCH");
            if(ptr != NULL){
                addBATCH(command, count);
            }
            else{
                ptr=strstr(command,"runPLS");
                if(ptr != NULL){
                    //runPLS(ptr,count);
                    printf("runPLS");
                }
                else{
                    //exitPLS();
                    printf("exit");
                }
            }
        }
    }
}

void addDate(char input[3][SIZE], int x, int start, int end, bool stDate){
    int i, j = 1;
    char temp[5];
    if(stDate == false)
        j=2;
    for (i = start; i < end; i++) {
        temp[i-start] = input[j][i];
    }
    temp[i-start] = '\0';
    if(stDate == true)
        startDate[x] = atoi(temp);
    else
        endDate[x] = atoi(temp);
}

void addPEIOD(char arr[]){
    int i=0;
    char input[3][SIZE];
    char * token = strtok(arr, " ");
    while(token != NULL){
        strcpy(input[i++], token);
        token = strtok(NULL, " ");
    }
    // Add Start Date
    addDate(input, 0, 0, 4, true);
    addDate(input, 1, 5, 7, true);
    addDate(input, 2, 8, 10, true);
    //Add End Date
    addDate(input, 0, 0, 4, false);
    addDate(input, 1, 5, 7, false);
    addDate(input, 2, 8, 10, false);
}

void addORDER(char arr[]){
    strtok(arr, " ");
    char * token = strtok(NULL, " ");

    strcpy(schedule[numOrders].orderNum, token);

    token = strtok(NULL, " ");
    strcpy(schedule[numOrders].dueDate, token);

    token = strtok(NULL, " ");
    schedule[numOrders].quantity = atoi(token);

    token = strtok(NULL, " ");
    strcpy(schedule[numOrders].productName, token);

    //printf("%s %s %d %s\n", schedule[numOrders].orderNum, schedule[numOrders].dueDate, schedule[numOrders].quantity, schedule[numOrders].productName);
    numOrders++;
}

void addBATCH(char arr[], int count){
    int i;
    char a[100], buf[150], line[100] = "";
    FILE *fp;
    for (i = 9; i < count; i++) {
        a[i-9] = arr[i];
    }
    a[i] = '\0';

    strtok(arr, " ");
    char * token = strtok(NULL, " ");

    strcat(line, a);
    char* filename = line;
    fp = fopen(filename ,"r");
    while(fgets(buf, 150, (FILE*)fp) != NULL){
        addORDER(buf);
    }
    fclose(fp);
}