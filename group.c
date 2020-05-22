#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>


struct Schedule{
    char Date[11];
    char productName[10];
    char orderNum[10];
    int quantity;
    char dueDate[11];
};

struct Schedule schedule[3];
int startDate[3], endDate[3], numOrders = 6; //why 5???
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
char ** getOrder(int line_num, char* filename);
void writeDays(int begin[3], int end[3], int flag);
void runPLS();
void runSJF();
void runANA();
bool isDatevalid(char date1[3],char *order1,int availDate[3],int plant);
void flusharray(char **arr);
void writeValid(char* order_num,int startD[3], int endD[3], int days, int quantity, char *plantName);
void clearPlants();
int main(int argc,char *argv[]){

//    char**ba=getOrder(4);
//    printf("%s",ba[1]);
    char command[100];
//    FILE *fp = fopen("orders.txt","w");
//    fclose(fp);
//    fp = fopen("invalid.txt","w");
//    fclose(fp);
//    fp = fopen("temp.txt","w");
//    fclose(fp);
    clearPlants();
    printf("~~WELCOME TO PLS~~\n\n");
    printf("Please enter:\n");
    while(1){
        fgets(command, 100, stdin);
        if(strncmp(command, "exitPLS", 7) == 0){
            break;
        }
        if(strncmp(command, "-1", 2) != 0){
            runcmd(command,sizeof(command)/sizeof(int));
            //runSJF();
           //runPLS();
           //runANA();
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
    if (!(startDate1[0] % 4) && (startDate1[0] % 100) || !(startDate1[0] % 400)){months[i]++;}
    for (i = 0; i < startDate1[1] - 1; i++)
        total += months[i];
    total += startDate1[2];

    int months1[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    i=0;
    total1 = (endDate1[0] - 1) * 365L + (endDate1[0] - 1) / 4 - (endDate1[0] - 1) / 100 + (endDate1[0] - 1) / 400;
    if (!(endDate1[0] % 4) && (endDate1[0] % 100) || !(endDate1[0] % 400)){months1[i]++;}
    for (i = 0; i < endDate1[1] - 1; i++)
        total1 += months1[i];
    total1 += endDate1[2];
    return total1-total;
}
void flusharray(char **arr){free(*arr);free(arr);}
char ** getOrder(int line_num, char* filename){
    char **buf = malloc(sizeof(char* )*4);
    int j;
    for (j = 0; j < 4; ++j) {
        buf[j] = malloc(sizeof(char)*10);
    }
//    strcpy(buf[0],"Parin");
//    strcpy(buf[1],"Hiotra");
    int count = 0,i=0;
    char line[100],delimit[]=" \n";
    FILE *fp = fopen(filename ,"r");
    if(fp == NULL){printf("Error!");exit(1);}
    while (fgets(line,sizeof(line),fp)!=NULL){
        if (count==line_num){
            char * token = strtok(line, delimit);
            while(token != NULL){
                strcpy(buf[i++], token);
                token = strtok(NULL, delimit);
            }
            break;
        }else{count++;}
    }
    fclose(fp);
    return buf;
}
bool isDatevalid(char date1[3],char *order1,int availDate[3],int plant){
    int duedate[3],order;
    sscanf( &date1[0], "%d", &duedate[0]);
    sscanf(&date1[5], "%d", &duedate[1]);
    sscanf(&date1[8], "%d", &duedate[2]);
    sscanf( order1, "%d", &order);
    int c1 = totalday(startDate,duedate);
    int c2 = totalday(duedate,endDate);
    int c3 = totalday(availDate,duedate);
    int capacity = ceil((double)order/(double)c3);
    if (c1 >= 0 && c2 >=0 && c3 > 0 && capacity <= plant){
        return true;
    } else {return false;}
}
void clearPlants(){
    FILE *fp1 = fopen("PlantX.txt","w");
    FILE *fp2 = fopen("PlantY.txt","w");
    FILE *fp3 = fopen("PlantZ.txt","w");
    FILE *fp4 = fopen("invalid.txt","w");
    FILE *fp5 = fopen("valid.txt","w");
    fclose(fp1);fclose(fp2);fclose(fp3);fclose(fp4);fclose(fp5);
}
/*int* dateFormater(char date[3]){
    int intDate[3];
    sscanf(& date[0], "%d", &intDate[0]);
    sscanf(& date[5], "%d", &intDate[1]);
    sscanf(& date[8], "%d", &intDate[2]);
    return intDate;
}*/
void counterVariables(int accepRejec[2],int usagePlants[3],int prodPlants[3] ){
    FILE *fp = fopen("valid.txt","r");if(fp == NULL){printf("Error!");exit(1);}
    int prod_index=0;
    char line[100],delimit[]=" \n\t";
    while (fgets(line,sizeof(line),fp)!=NULL){
        accepRejec[0]++;
        if (strstr(line,"Plant_X")){
            char * token = strtok(line, delimit);
            while(prod_index < 5){
                if (prod_index==3){
                    usagePlants[0]+= atoi(token);
                } else if (prod_index==4){
                    prodPlants[0]+= atoi(token);
                }
                token = strtok(NULL, delimit);
                prod_index++;
            }
            prod_index=0;
        }
        else if (strstr(line,"Plant_Y")){
            char * token = strtok(line, delimit);
            while(prod_index < 5){
                if (prod_index==3){
                    usagePlants[1]+= atoi(token);
                } else if (prod_index==4){
                    prodPlants[1]+= atoi(token);
                }
                token = strtok(NULL, delimit);
                prod_index++;
            }
            prod_index=0;
        }
        else if (strstr(line,"Plant_Z")){
            char * token = strtok(line, delimit);
            while(prod_index < 5){
                if (prod_index==3){
                    usagePlants[2]+= atoi(token);
                } else if (prod_index==4){
                    prodPlants[2]+= atoi(token);
                }
                token = strtok(NULL, delimit);
                prod_index++;
            }
            prod_index=0;
        }
    }
    fclose(fp);
    fp = fopen("invalid.txt","r");if(fp == NULL){printf("Error!");exit(1);}
    while (fgets(line,sizeof(line),fp)!=NULL) {
        accepRejec[1]++;
    }
}
void reportGenerator(int accepRejec[2],int usagePlants[3],int prodPlants[3],char* algoUsed,char* outputFile ){
    double utilPlants[3]={300,400,600};
    int i;
    for (i = 0; i < 3 ; ++i) {
        utilPlants[i]= ((float)prodPlants[i]/(float)(totalday(startDate,endDate)*utilPlants[i]))*100;
    }
    FILE *fp = fopen(outputFile,"w+");
    fprintf(fp,"***PLS Schedule Analysis Report***\r\n\r\nAlgorithm used: %s\r\n\r\nThere are %d Orders ACCEPTED. Details are as follows:\r\n\r\n",algoUsed,accepRejec[0]);
    fprintf(fp,"ORDER NUMBER\t START\t\t\tEND\t\t\tDAYS\tQUANTITY\tPLANT\r\n===========================================================================\r\n");

    FILE *fp1 = fopen("valid.txt","r");
    char c=fgetc(fp1);
    while (c !=EOF){fprintf (fp,"%c", c);c = fgetc(fp1); }fclose(fp1);
    fprintf(fp,"\t\t\t\t\t\t\t- End -\r\n\r\n===========================================================================\r\n\r\n\r\n");
    fprintf(fp,"There are %d Orders REJECTED. Details are as follows:\r\n\r\n",accepRejec[1]);
    fprintf(fp,"ORDER NUMBER\tPRODUCT NAME\tDue Date\t\tQUANTITY\r\n===========================================================================\r\n");
    fp1 = fopen("invalid.txt","r");c=fgetc(fp1);
    while (c !=EOF){fprintf (fp,"%c", c);c = fgetc(fp1); }fclose(fp1);
    fprintf(fp,"\t\t\t\t\t\t\t- End -\r\n\r\n===========================================================================\r\n\r\n\r\n");
    fprintf(fp,"***PERFORMANCE\r\n\r\n");
    fprintf(fp,"Plant_X:\r\n\t\tNumber of days in use:\t\t %d days\r\n\t\tNumber of products produced:\t%d (in total)\r\n\t\tUtilization of the plant:\t\t%.2f %%\r\n\r\n",usagePlants[0],prodPlants[0],utilPlants[0]);
    fprintf(fp,"Plant_Y:\r\n\t\tNumber of days in use:\t\t %d days\r\n\t\tNumber of products produced:\t%d (in total)\r\n\t\tUtilization of the plant:\t\t%.2f %%\r\n\r\n",usagePlants[1],prodPlants[1],utilPlants[1]);
    fprintf(fp,"Plant_Z:\r\n\t\tNumber of days in use:\t\t %d days\r\n\t\tNumber of products produced:\t%d (in total)\r\n\t\tUtilization of the plant:\t\t%.2f %%\r\n\r\n",usagePlants[2],prodPlants[2],utilPlants[2]);
    fprintf(fp,"Overall of utilization %.2f %%\r\n\r\n",(utilPlants[0]+utilPlants[1]+utilPlants[2])/3);
    fclose(fp);
    fp = fopen(outputFile,"r");
    c = fgetc(fp);
    while (c !=EOF){printf ("%c", c);c = fgetc(fp); }fclose(fp);
}
int* writeSch(int availDate[3],char* order_num,char* product_name,char endD[3],int quantity,int sizeplant){
    FILE *fp;
    char* plantName;
    if (sizeplant==300){plantName="Plant_X";fp = fopen("PlantX.txt","ab+");
    }else if (sizeplant==400){plantName="Plant_Y";fp = fopen("PlantY.txt","ab+");
    }else{plantName="Plant_Z";fp = fopen("PlantZ.txt","ab+");}
    int daysNeed = ceil((double)quantity/(double)sizeplant);
    int months[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    int *currD = availDate,initD[3];memcpy(initD,availDate,sizeof(initD));
    int i,j=0,k,l;
    for ( i = 0; i < 12; ++i) {
        if (currD[1]==months[i]){ break;}
    }
    k=0;
    l=0;
    int dailyProd = quantity%sizeplant,capacity=sizeplant;
    while (k < daysNeed) {
        if (currD[2]+l <= days[i]){
            if (dailyProd != 0 && k == daysNeed-1){capacity=dailyProd;}
            printf("%d-%02d-%02d %s %s %d %s %d\r\n",currD[0],currD[1],currD[2]+l,product_name,order_num,capacity,endD,sizeplant);
            fprintf(fp,"%d-%02d-%02d %s %s %d %s\r\n",currD[0],currD[1],currD[2]+l,product_name,order_num,capacity,endD);
            k++;
            l++;
        }else {
            currD[1] += 1; // go to next month
            currD[2] = 1;  // start from first day
            l=0;
            i++;
        }
    }
    printf("\n");
    currD[2]+=l-1;
    fclose(fp);
    writeValid(order_num,initD,currD,l,quantity,plantName);
    currD[2]++;
    return currD;
}
void writeInvalid(char* order_num,char* product_name,char endD[3],int quantity){
    FILE *fp = fopen("invalid.txt","ab+");
    fprintf(fp, "%-16s%-16s%-16s%d\n", order_num, product_name, endD, quantity);
    //printf("INVALID %-16s%-16s%-16s%d\n", order_num, product_name, endD, quantity);
    fclose(fp);
}
void writeValid(char* order_num,int startD[3], int endD[3], int days, int quantity, char *plantName){
    FILE *fp = fopen("valid.txt","ab+");
    fprintf(fp, "%-16s %d-%02d-%02d\t\t%d-%02d-%02d\t  %-8d %-8d %s \n", order_num, startD[0],startD[1],startD[2], endD[0],endD[1],endD[2], days, quantity,plantName);
//    printf("VALID %-16s %d-%02d-%02d\t\t%d-%02d-%02d\t  %-8d %-8d %s \n", order_num, startD[0],startD[1],startD[2], endD[0],endD[1],endD[2], days, quantity,plantName);
    fclose(fp);
}
void schChild(int in_pipe[][2],int out_pipe[][2]) {
    close(in_pipe[0][1]);
    close(out_pipe[0][0]);
    char deck[5][30], ch;
    int n, totaldays=totalday(startDate,endDate),ord=0,ordTemp=0,j=0,i,x,y;
    int availDate[3][3], sizePlants[] = {300,400,600};
    for (i = 0; i < 3 ; ++i) {memcpy(availDate[i], startDate, sizeof(startDate));}
    int num = 6 , arr[num][2];

    while ((n = read(in_pipe[0][0], deck, sizeof(deck)) > 0)) {
        if (strcmp(deck[0], "FCFS") == 0 || strcmp(deck[0], "SJF2") == 0) {
            char *filename= (strcmp(deck[0],"FCFS")==0) ? "orders.txt":"temp.txt" ;
            while (ord < numOrders) {
                char **buf = getOrder(ord, filename);
                int ordernum;
                sscanf(buf[2], "%d", &ordernum);
                int ordValid = 1;
                for (i = 0; i < 3; ++i) {
                    if (isDatevalid(buf[1], buf[2], availDate[i], sizePlants[i])) {
                        memcpy(availDate[i], writeSch(availDate[i], buf[3], buf[0], buf[1], ordernum, sizePlants[i]),
                               sizeof(availDate));
                        ordValid = 1;
                        break;
                    }
                    ordValid = 0;
                }
                if (ordValid == 0) { writeInvalid(buf[3], buf[0], buf[1], ordernum); }
                ord++;
                flusharray(buf);
            }
            strcpy(deck[0],"doneR");
            write(out_pipe[0][1],deck,sizeof(deck));
        }
        else if(strcmp(deck[0], "SJF") == 0){
            i = 0;
            while (ord < num){
                char **buf = getOrder(ord, "orders.txt");
                sscanf(buf[2], "%d", &arr[i][0]);
                arr[i][1] = i;
                i++;
                ord++;
                flusharray(buf);
            }
            for(i=0; i<num-1; i++){
                for(j=i+1; j<num; j++){
                    if(arr[i][0] > arr[j][0]){
                        x = arr[i][0];
                        y = arr[i][1];
                        arr[i][0] = arr[j][0];
                        arr[i][1] = arr[j][1];
                        arr[j][0] = x;
                        arr[j][1] = y;
                    }
                }
            }

            FILE *fp1 = fopen("orders.txt","r");
            FILE *fp2 = fopen("temp.txt","w+");
            while((ch = fgetc(fp1)) != EOF){
                fputc(ch, fp2);
            }
            fclose(fp1);
            fclose(fp2);

            fp1 = fopen("temp.txt","w+");
            fclose(fp1);

            fp1 = fopen("temp.txt","a");
            fp2 = fopen("orders.txt","r");

            ord = 0;
            while (ord < num){
                char **buf = getOrder(arr[ord][1], "orders.txt");
                for(i = 0; i < 4; i++){
                    fprintf(fp1,"%s ",buf[i]);
                    fflush(fp1);
                }
                fprintf(fp1,"\n");
                ord++;
                flusharray(buf);
            }
            fclose(fp1);
            fclose(fp2);
            ord=0;
            strcpy(deck[0],"doneS");
            write(out_pipe[0][1],deck,sizeof(deck));
        }
        else if (strcmp(deck[0], "REPO") == 0) {
            int accepRejec[2]={0,0},usagePlants[3]={0,0,0},prodPlants[3]={0,0,0};
            counterVariables(accepRejec,usagePlants,prodPlants);
            reportGenerator(accepRejec,usagePlants,prodPlants,deck[1],deck[2]);
            strcpy(deck[0],"doneA");
            write(out_pipe[0][1],deck,sizeof(deck));
        }
        else if (strcmp(deck[0], "f") == 0) {
            break;
        }
    }
    close(in_pipe[0][0]);
    close(out_pipe[0][1]);
    printf("Child schedule exited while loop\r\n");
}

void runPLS() {
    clearPlants();
    char deck[5][10];
    int  flag = 0, ord = 0, j = 0,i;
    int availDate[3][3], sizePlants[] = {300, 400, 600};

    for (i = 0; i < 3 ; ++i) {memcpy(availDate[i], startDate, sizeof(startDate));}

    while (ord < numOrders) {
        char **buf = getOrder(ord, "orders.txt");
        int ordernum;
        sscanf(buf[2], "%d", &ordernum);
        int ordValid = 1;
        for (i = 0; i < 3; ++i) {
            if (isDatevalid(buf[1], buf[2], availDate[i], sizePlants[i])) {
                memcpy(availDate[i], writeSch(availDate[i], buf[0], buf[3], buf[1], ordernum, sizePlants[i]),sizeof(availDate));
                ordValid = 1;
                break;
            }
            ordValid = 0;
        }
        if (ordValid == 0) { writeInvalid( buf[0],buf[3], buf[1], ordernum); }
        ord++;
        flusharray(buf);
    }
    int accepRejec[2]={0,0},usagePlants[3]={0,0,0},prodPlants[3]={0,0,0};
    counterVariables(accepRejec,usagePlants,prodPlants);

}
void runSJF(){
    int i = 0,j,x,y,ord=0, num = 6,arr[num][2];
    char ch;
    while (ord < num){
        char **buf = getOrder(ord, "orders.txt");
        sscanf(buf[2], "%d", &arr[i][0]);
        arr[i][1] = i;
        i++;
        ord++;
        flusharray(buf);
    }
    for(i=0; i<num-1; i++){
        for(j=i+1; j<num; j++){
            if(arr[i][0] > arr[j][0]){
                x = arr[i][0];
                y = arr[i][1];
                arr[i][0] = arr[j][0];
                arr[i][1] = arr[j][1];
                arr[j][0] = x;
                arr[j][1] = y;
            }
        }
    }

    FILE *fp1 = fopen("orders.txt","r");
    FILE *fp2 = fopen("temp.txt","w+");
    while((ch = fgetc(fp1)) != EOF){
        fputc(ch, fp2);
    }
    fclose(fp1);
    fclose(fp2);

    fp1 = fopen("temp.txt","w+");
    fclose(fp1);

    fp1 = fopen("temp.txt","a");
    fp2 = fopen("orders.txt","r");

    ord = 0;
    while (ord < num){
        char **buf = getOrder(arr[ord][1], "orders.txt");
        for(i = 0; i < 4; i++){
            fprintf(fp1,"%s ",buf[i]);
            fflush(fp1);
        }
        fprintf(fp1,"\n");
        ord++;
        flusharray(buf);
    }
    fclose(fp1);
    fclose(fp2);

    runPLS();
}
void runANA(){
    int accepRejec[2]={0,0},usagePlants[3]={0,0,0},prodPlants[3]={0,0,0};
    counterVariables(accepRejec,usagePlants,prodPlants);
    reportGenerator(accepRejec,usagePlants,prodPlants,"FCFS","report_01_FCFS.txt");
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
void runcmd(char command[],int count) {
    int ppid = getpid();
    int in_pipe[1][2]; int out_pipe[1][2];
    char deck[5][30];char *algoUsed;

    if (strstr(command, "addPEIOD") != NULL) {
        addPEIOD(command);
    } else if (strstr(command, "addPEIOD") != NULL) {
        addORDER(command);
    } else if (strstr(command, "addBATCH") != NULL) {
        addBATCH(command, count);
    } else if (strstr(command, "runPLS") != NULL) {
        if (strstr(command, "FCFS") != NULL) {
            algoUsed="FCFS";
            clearPlants();
            createChild(ppid, in_pipe, out_pipe);
            strcpy(deck[0], "FCFS");
            write(in_pipe[0][1], deck, sizeof(deck));
            read(out_pipe[0][0], deck, sizeof(deck));

        } else if (strstr(command, "SJF") != NULL) {
            algoUsed="SJF";
            clearPlants();
            createChild(ppid, in_pipe, out_pipe);
            strcpy(deck[0], "SJF");
            write(in_pipe[0][1], deck, sizeof(deck));
            read(out_pipe[0][0], deck, sizeof(deck));
            strcpy(deck[0], "SJF2");
            write(in_pipe[0][1], deck, sizeof(deck));
            read(out_pipe[0][0], deck, sizeof(deck));

        }
        if (strstr(command, "printREPORT") != NULL) {
            int i = 0;
            char delimiter[] = " \n";
            char *token = strtok(command, delimiter);
            while (i != 5) {
                i++;
                token = strtok(NULL, delimiter);
            }
            strcpy(deck[0], "REPO");
            strcpy(deck[1], algoUsed);
            strcpy(deck[2], token);
            write(in_pipe[0][1], deck, sizeof(deck));
            read(out_pipe[0][0], deck, sizeof(deck));
            strcpy(deck[0], "f");
            write(in_pipe[0][1], deck, sizeof(deck));
            read(out_pipe[0][0], deck, sizeof(deck));
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
void writeDays(int begin[3], int end[3], int flag) {
    FILE *px = fopen("PlantX.txt","w+");
    FILE *py = fopen("PlantY.txt","w+");
    FILE *pz = fopen("PlantZ.txt","w+");
    if(px == NULL || py == NULL || pz == NULL){printf("Error!");exit(1);}

    char newline[] = "NA NA NA NA \n";
    int months[] = {1,2,3,4,5,6,7,8,9,10,11,12};
    int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (startDate[0]%4==0){days[1]=29;}
    int *currD = begin,i,j,k,l;
    for ( i = 0; i < 12; ++i) {
        if (currD[1]==months[i]){ break;}
    }
    k=0;
    while (memcmp(currD,end,sizeof(end)) != 0) {
        while (currD[2]+k<=days[i]){
            //printf("%d-%d-%d",currD[0],currD[1],currD[2]+k);
            fwrite(currD,sizeof(currD),1,px);
            fwrite(currD,sizeof(currD),1,py);
            fwrite(currD,sizeof(currD),1,pz);
            if (flag==1){
                fwrite(newline,sizeof(newline),1,px);
                fwrite(newline,sizeof(newline),1,py);
                fwrite(newline,sizeof(newline),1,pz);
            }
            k++;
        }
        currD[1]+=1; // go to next month
        currD[2]=1;  // start from first day
        i++;
    }
    fclose(px);
    fclose(py);
    fclose(pz);

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
    numDays = totalday(startDate, endDate);
}
void addORDER(char arr[]){
    FILE *fp = fopen("orders.txt","a");
    if(fp == NULL){printf("Error!\n");exit(1);}
    char delimit[]=" \n";
    strtok(arr, " ");
    char * token = strtok(NULL, delimit);
    while(token != NULL){
        fprintf(fp,"%s ",token);
        fflush(fp);
        token = strtok(NULL, delimit);
    }
    fprintf(fp,"\n");
    fclose(fp);
    numOrders++;
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
    int total = totalday(startDate,endDate);
    printf("Plant_X (300 per day)\n");
    printf("%d-%d-%d to %d-%d-%d\n", startDate[0], startDate[1], startDate[2], endDate[0], endDate[1], endDate[2]);
    printf("    Date       Product Name      Order Number     Quantity(Produced)    Due Date\n");

    for (i = 0; i < total; i++) {
        printf("%-10s \t %-12s \t %-10s \t %-5s \t %-10s \n", curDate, product, order, quantity, dueDate);
    }
}
