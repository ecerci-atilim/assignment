#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

#define FNAME_LEN 10
#define DUMMY_SIZE 1<<8

int getID(char strin[]);
int getBase(char strin[]);
int getLength(char strin[]);
void getStr(char strin[], int buflen, char **bufout);

int main (void)
{
     //system("cls");
     DIR *dirptr;
     char *fname = (char *)malloc(FNAME_LEN);
     char dummy[DUMMY_SIZE];
     FILE *fid;
     int fcnt = 0;
     int fileIDtemp;

     // Database
     fid = fopen("nums.db", "r");
     fgets(dummy, DUMMY_SIZE, fid);
     fclose(fid);
     fileIDtemp = getLength(dummy);
     printf("%d: ", fileIDtemp);
     char *t = malloc(2);
     getStr(dummy, fileIDtemp, &t);
     printf("%s\n", t);


     struct dirent *entry;     
     dirptr = opendir ("./num/");
     if (dirptr != NULL){
          while ((entry = readdir (dirptr)) != NULL){
               fname = entry->d_name;
               if(fname[0] != 46){
                    //printf("Processing %s, file #%d\n", fname, fcnt);
                    // Processing
                    fcnt++;
               }
          }
               
     (void) closedir(dirptr);
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
     char base[2];
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