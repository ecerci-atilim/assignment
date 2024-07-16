#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define FNAME_LEN 6
#define DUMMY_SIZE 1<<8
#define BIG_SIZE 1<<10
#define SMALL_SIZE 1<<4
#define PATH_SIZE 1<<8

int getID(char strin[]);
int getBase(char strin[]);
int getLength(char strin[]);
void getStr(char strin[], int buflen, char **bufout);
void processStr(char l1[], char l2[], char l3[], int len, char **processed);

int main (void)
{
     system("cls");
     DIR *dirptr;
     FILE *fiddb, *fidtemp;
     char *fname = (char *)malloc(FNAME_LEN);
     char dummy[DUMMY_SIZE], l1[BIG_SIZE], l2[BIG_SIZE], l3[BIG_SIZE], t1[SMALL_SIZE], t2[SMALL_SIZE];
     char path[PATH_SIZE];
     int fcnt = 0;
     int fileIDtemp, basetemp, lentemp;
     char *strtemp, *checkStr;
     bool mainSuccess = true, tempSuccess;

     // Database
     fiddb = fopen("nums.db", "r");
     char *t = (char *)malloc(DUMMY_SIZE);

     struct dirent *entry;     
     dirptr = opendir ("./num/");
     if (dirptr != NULL){
          while ((entry = readdir (dirptr)) != NULL){
               tempSuccess = true;
               fname = entry->d_name;
               if(fname[0] != 46){
                    printf("Processing %s, file #%d\n", fname, fcnt);
                    // Processing
                    (void)fgets(dummy, sizeof(dummy), fiddb);
                    fileIDtemp = getID(dummy);
                    basetemp = getBase(dummy);
                    lentemp = getLength(dummy);
                    getStr(dummy, lentemp, &t);
                    printf("dB Info:\n\tdB ID:    %d\n\tdB base:  %d\n\tdB len:   %d\n\tdB value: ", fileIDtemp, basetemp, lentemp);
                    for(int i = 0; i < lentemp; i++)
                         printf("%c", *(t+i));
                    printf("\nProcess Info:\n");
                    if ((size_t)snprintf(path, sizeof(path), "./num/%s", entry->d_name) >= sizeof(path)) {
                         printf("Filename too long!");
                         continue;
                    } 
                    printf("\tFile path: %s\n", path);

                    fidtemp = fopen(path, "r");
                    if(fidtemp==NULL) printf("fid null");
                    (void)fgets(t1, sizeof(t1), fidtemp);
                    (void)fgets(t2, sizeof(t2), fidtemp);
                    (void)fgets(l1, sizeof(l1), fidtemp);
                    (void)fgets(l2, sizeof(l2), fidtemp);
                    (void)fgets(l3, sizeof(l3), fidtemp);
                    processStr(l1, l2, l3, lentemp, &strtemp);
                    printf("\tValue   : ");
                    for(int i = 0; i < lentemp; i++){
                         printf("%c",*(strtemp + i));
                         if(*(strtemp + i) != *(t+i))
                              tempSuccess = false;
                    }
                    if(tempSuccess == false){
                         printf("\n\tStatus  : Failed");
                         mainSuccess = false;
                    }
                    else printf("\n\tStatus  : Success");


                    (void)fclose(fidtemp);
                    printf("\n\n----------\n\n");
                    fcnt++;
                    usleep((unsigned int)(1<<20));
               }
          }
               
     (void)closedir(dirptr);
     (void)fclose(fiddb);

     if(mainSuccess)
          printf("Files successfully processed. \nFile contents match exactly with database.");
     else
          printf("Execution ended with errors.");
     return 0;

     }
     else{
          return -1;
     }
}

int getID(char strin[]){
     int i;
     int c = 0;
     char fileID[3];
     for(i = 0; i < strlen(strin); i++){
          if(strin[i] == 9)
               break;
     }
     while(c < i){
          fileID[c] = strin[c];
          c++;
     }
     return atoi(fileID);
}

int getBase(char strin[]){
     int htab1 = 0, i, c, sc = 0;
     char base[3];
     for(i = 0; i < strlen(strin); i++){
          if(strin[i] == 9){
               htab1 = i;
               break;
          }
     }
     for(i = htab1+1; i < strlen(strin); i++){
          if(strin[i] == 9)
               break;
     }
     for(c = htab1; c < i; c++){
          base[sc] = strin[c];
          sc++;
     }
     return atoi(base);
}

int getLength(char strin[]){
     int tc = 0, i, htab2, htab3, sc = 0;
     char len[4];
     for(i = 0; i < strlen(strin); i++){
          if(strin[i] == 9){
               tc++;
          if(tc == 2)
               htab2 = i;
          else if(tc==3)
               htab3 = i;
          }
     }
     for(i = htab2; i < htab3; i++){
          len[sc] = strin[i];
          sc++;
     }
     return atoi(len);
}

void getStr(char strin[], int buflen, char **bufout){
     int ltloc = 0, i, nlloc = 0, sc = 0;
     char *buftemp = malloc(buflen*sizeof(char));
     for(i = 0; i < strlen(strin); i++){
          if(strin[i] == 9)
               ltloc = i+1;
          else if(strin[i] == 10)
               nlloc = i;
     }
     for(i = ltloc; i < nlloc; i++){
          buftemp[sc] = strin[i];
          sc++;
     }
     *bufout = buftemp;
}

void processStr(char l1[], char l2[], char l3[], int len, char **processed){
     char *proctemp = malloc(len*sizeof(char));
     int i1, i2, i3;

     for(int i = 0; i < len; i++){
          i1 = i*4;
          i2 = i1 + 1;
          i3 = i2 + 1;
          
          if(
               l1[i1]==32 && l1[i2]!=32 && l1[i3]==32 &&
               l2[i1]!=32 && l2[i2]==32 && l2[i3]!=32 &&
               l3[i1]!=32 && l3[i2]!=32 && l3[i3]!=32)      *(proctemp + i) = '0';
          else if(
               l1[i1]==32 && l1[i2]==32 && l1[i3]==32 &&
               l2[i1]==32 && l2[i2]==32 && l2[i3]!=32 &&
               l3[i1]==32 && l3[i2]==32 && l3[i3]!=32)      *(proctemp + i) = '1';
          else if(
               l1[i1]==32 && l1[i2]!=32 && l1[i3]==32 &&
               l2[i1]==32 && l2[i2]!=32 && l2[i3]!=32 &&
               l3[i1]!=32 && l3[i2]!=32 && l3[i3]==32)      *(proctemp + i) = '2';
          else if(
               l1[i1]==32 && l1[i2]!=32 && l1[i3]==32 &&
               l2[i1]==32 && l2[i2]!=32 && l2[i3]!=32 &&
               l3[i1]==32 && l3[i2]!=32 && l3[i3]!=32)      *(proctemp + i) = '3';
          else if(
               l1[i1]==32 && l1[i2]==32 && l1[i3]==32 &&
               l2[i1]!=32 && l2[i2]!=32 && l2[i3]!=32 &&
               l3[i1]==32 && l3[i2]==32 && l3[i3]!=32)      *(proctemp + i) = '4';
          else if(
               l1[i1]==32 && l1[i2]!=32 && l1[i3]==32 &&
               l2[i1]!=32 && l2[i2]!=32 && l2[i3]==32 &&
               l3[i1]==32 && l3[i2]!=32 && l3[i3]!=32)      *(proctemp + i) = '5';
          else if(
               l1[i1]==32 && l1[i2]!=32 && l1[i3]==32 &&
               l2[i1]!=32 && l2[i2]!=32 && l2[i3]==32 &&
               l3[i1]!=32 && l3[i2]!=32 && l3[i3]!=32)      *(proctemp + i) = '6';
          else if(
               l1[i1]==32 && l1[i2]!=32 && l1[i3]==32 &&
               l2[i1]==32 && l2[i2]==32 && l2[i3]!=32 &&
               l3[i1]==32 && l3[i2]==32 && l3[i3]!=32)      *(proctemp + i) = '7';
          else if(
               l1[i1]==32 && l1[i2]!=32 && l1[i3]==32 &&
               l2[i1]!=32 && l2[i2]!=32 && l2[i3]!=32 &&
               l3[i1]!=32 && l3[i2]!=32 && l3[i3]!=32)      *(proctemp + i) = '8';
          else if(
               l1[i1]==32 && l1[i2]!=32 && l1[i3]==32 &&
               l2[i1]!=32 && l2[i2]!=32 && l2[i3]!=32 &&
               l3[i1]==32 && l3[i2]!=32 && l3[i3]!=32)      *(proctemp + i) = '9';
          else if(
               l1[i1]==32 && l1[i2]!=32 && l1[i3]==32 &&
               l2[i1]!=32 && l2[i2]!=32 && l2[i3]!=32 &&
               l3[i1]!=32 && l3[i2]==32 && l3[i3]!=32)      *(proctemp + i) = 'A';
          else if(
               l1[i1]==32 && l1[i2]==32 && l1[i3]==32 &&
               l2[i1]!=32 && l2[i2]!=32 && l2[i3]==32 &&
               l3[i1]!=32 && l3[i2]!=32 && l3[i3]!=32)      *(proctemp + i) = 'B';
          else if(
               l1[i1]==32 && l1[i2]!=32 && l1[i3]==32 &&
               l2[i1]!=32 && l2[i2]==32 && l2[i3]==32 &&
               l3[i1]!=32 && l3[i2]!=32 && l3[i3]==32)      *(proctemp + i) = 'C';
          else if(
               l1[i1]==32 && l1[i2]==32 && l1[i3]==32 &&
               l2[i1]==32 && l2[i2]!=32 && l2[i3]!=32 &&
               l3[i1]!=32 && l3[i2]!=32 && l3[i3]!=32)      *(proctemp + i) = 'D';
          else if(
               l1[i1]==32 && l1[i2]!=32 && l1[i3]==32 &&
               l2[i1]!=32 && l2[i2]!=32 && l2[i3]==32 &&
               l3[i1]!=32 && l3[i2]!=32 && l3[i3]==32)      *(proctemp + i) = 'E';
          else if(
               l1[i1]==32 && l1[i2]!=32 && l1[i3]==32 &&
               l2[i1]!=32 && l2[i2]!=32 && l2[i3]==32 &&
               l3[i1]!=32 && l3[i2]==32 && l3[i3]==32)      *(proctemp + i) = 'F';
               else *(proctemp + i) = '?';

     }

     *processed = proctemp;
     

}