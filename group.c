#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <zconf.h>

struct Schedule{
    char Date[11];
    char productName[10];
    char orderNum[10];
    int quantity;
    char dueDate[11];
};

struct Schedule schedule[3];
int startDate[3], endDate[3], numOrders = 0;
const int SIZE = 80;
long numDays;

long totalday(int startDate1[3],int endDate1[3]);
void schChild(int in_pipe[][2],int out_pipe[][2]);
void runcmd(char command[],int count);
void addPEIOD(char arr[]);
void addDate(char input[3][SIZE], int x, int start, int end, bool stDate);
void addORDER(char arr[]);
void addBATCH(char arr[], int count);
void printSchedule();
void createChild();
char **getOrder();

int main(int argc,char *argv[]){
    //int n=totalday(2020,06,01,2020,07,30);
    char command[100];
    printf("~~WELCOME TO PLS~~\n\n");
    printf("Please enter:\n");
    while(1){
        fgets(command, 100, stdin);
        if(strncmp(command, "exitPLS", 4) == 0){
            break;
        }
        if(strncmp(command, "-1", 2) != 0){
            runcmd(command,sizeof(command)/sizeof(int));
            getOrder();
            strcpy(command, "-1");
            printf("Please enter:\n");
        }
    }
    return 0;
}
long totalday(int startDate1[3], int endDate1[3])
{
    int months[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    int i = 0;
    long total = 0L,total1 = 0L;
    total = (startDate1[0] - 1) * 365L + (startDate1[0] - 1) / 4 - (startDate1[0] - 1) / 100 + (startDate1[0] - 1) / 400;
    if (!(startDate1[0] % 4) && startDate1[0] % 100 || !(startDate1[0] % 400))
        months[i]++;
    for (i = 0; i < startDate1[1] - 1; i++)
        total += months[i];
    total += startDate1[2];

    i=0;
    total1 = (endDate1[0] - 1) * 365L + (endDate1[0] - 1) / 4 - (endDate1[0] - 1) / 100 + (endDate1[0] - 1) / 400;
    if (!(endDate1[0] % 4) && endDate1[0] % 100 || !(endDate1[0] % 400))
        months[i]++;
    for (i = 0; i < endDate1[1] - 1; i++)
        total1 += months[i];
    total1 += endDate1[2];
    return total1-total;
}

char **getOrder(){
    char **buf = malloc(4*35*sizeof(char));
    char* filename = "orders.txt";
    FILE *fp = fopen(filename ,"r");
    int i=0;
    while(fscanf((FILE*)fp,"%s ",buf[i])){
        if (i == 4){ break;}
        i++;
    }
    fclose(fp);
    return buf;
}
void schChild(int in_pipe[][2],int out_pipe[][2]) {
    close(in_pipe[0][1]);
    close(out_pipe[0][0]);
    char deck[5][10];
    int n, totaldays=totalday(startDate,endDate);
    while ((n = read(in_pipe[0][0], deck, sizeof(deck)) > 0)) {
        if (strcmp(deck[0], "FCFS") == 0) {
            char **buf = getOrder();
            if (buf[2]/totaldays(startDate,buf[1]))
        }
    }
    close(in_pipe[0][0]);
    close(out_pipe[0][1]);
    printf("Child sch exited while loop\r\n");
}
void createChild(int ppid, int in_pipe[][2],int out_pipe[][2]){
    if ((pipe(in_pipe[0]) < 0) || (pipe(out_pipe[0]) < 0)) {
        printf("Pipe creation error\r\n");
        exit(1);
    }
    if(getpid()==ppid) {
        int pid = fork();
        if (pid < 0) { /* error occurred */
            printf("Fork Failed\r\n");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { // execute child program
            schChild(in_pipe, out_pipe);
            exit(1);
            wait(NULL);
        }
        close(in_pipe[0][0]);
        close(out_pipe[0][1]);
    }
}
void runcmd(char command[],int count){
    int ppid = getpid();
    int in_pipe[1][2];
    int out_pipe[1][2];
    char deck[5][10];
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
                    strtok(command, " ");
                    char * token = strtok(NULL, " ");
                    token = strtok(NULL, " ");
                    strcpy(deck[0],token);
                    createChild(ppid,in_pipe,out_pipe);
                    write(in_pipe[0][1],deck,sizeof(deck));
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
// We assume that the period is in the same year
void writeDays() {
    int months[] = {1,2,3,4,5,6,7,8,9,10,11,12};
    int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (startDate[0]%4==0){days[1]=29;}
    int *currD = startDate,i,j,k,l;
    for ( i = 0; i < 12; ++i) {
        if (currD[1]==months[i]){ break;}
    }
    k=0;
    while (memcmp(currD,endDate)) {
        while (currD[2]+k<=days[i]){
            printf("%d-%d-%d",currD[0],currD[1],currD[2]+k);
            k++;
        }
        currD[1]+=1; // go to next month
        currD[2]=1;  // start from first day
        i++;
    }

};

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
    numDays = totalday(startDate, endDate);
    writeDays();
}

void addORDER(char arr[]){
    FILE *fp = fopen("orders.txt","ab+");
    if(fp == NULL)
    {
        printf("Error!");
        exit(1);
    }
    strtok(arr, " ");
    char * token = strtok(NULL, " ");
    while(token != NULL){
        fprintf(fp," %s",token);
        fflush(fp);
        token = strtok(NULL, " ");
    }
    fclose(fp);
    //printf("%s %s %d %s\n", schedule[numOrders].orderNum, schedule[numOrders].dueDate, schedule[numOrders].quantity, schedule[numOrders].productName);
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

void printSchedule(){
    char curDate[20], product[20], order[20], quantity[20], dueDate[20];
    int i;
    int total = totalday(startDate1[0], startDate1[1], startDate1[2],endDate1[0], endDate1[1], endDate1[2]);
    printf("Plant_X (300 per day)\n");
    printf("%d-%d-%d to %d-%d-%d\n", startDate1[0], startDate1[1], startDate1[2], endDate1[0], endDate1[1], endDate1[2]);
    printf("    Date       Product Name      Order Number     Quantity(Produced)    Due Date\n");

    for (i = 0; i < total; i++) {
        printf("%-10s \t %-12s \t %-10s \t %-5s \t %-10s \n", curDate, product, order, quantity, dueDate);
    }
}
