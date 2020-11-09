#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <memory.h>
#include <string.h>

#define BUF_SIZE 500
#define LENGTH_MAX 1000
typedef struct files{
    int len;
    long offset;
}FLE;

void init_f2(FLE* f2, FILE * fp2){
    char buf[BUF_SIZE];
    memset(f2, 0, sizeof(f2));
    int index =0;
    int n;
    long offset = 0;
    int enter = 1;
    printf("start init f2\n");
    while(feof(fp2)==0){
        n = fread(buf,sizeof(char),BUF_SIZE,fp2);
        for(int i = 0; i < n ; i++){
            f2[index].len++;
            offset++;
            if(buf[i] == '\n'){
                f2[++index].offset = offset+enter++;
                printf("index %d, offset %d, len %d, buflen %d\n", index, f2[index-1].offset, f2[index-1].len, strlen(buf));
            }
        }
    }
}
void search_f2(int index, char* buf, FLE * f2, FILE* fp2){
    fseek(fp2, f2[index].offset, SEEK_SET);
    fread(buf,sizeof(char),f2[index].len,fp2);
    buf[f2[index].len-1] = '\0';
    //printf("file point %d %d %d %d\n",point, buf[0],buf[1], buf[2]);
}
void init_f3(FLE* f3, FILE * fp3){
    char buf[BUF_SIZE];
    memset(f3, 0, sizeof(f3));
    int index =0;
    int n, enter = 1;
    long offset = 0;
    printf("start init f2\n");
    while(feof(fp3)==0){
        n = fread(buf,sizeof(char),BUF_SIZE,fp3);
        for(int i = 0; i < n ; i++){
            f3[index].len++;
            offset++;
            if(buf[i] == '\n'){
                f3[++index].offset = offset+enter++;
                printf("index %d, offset %d, len %d, buflen %d\n", index, f3[index-1].offset, f3[index-1].len, strlen(buf));
            }
        }
    }
}
void search_f3(int index, char*buf, FLE * f3, FILE * fp3){
    fseek(fp3,f3[index].offset, SEEK_SET);
    fread(buf,sizeof(char),f3[index].len,fp3);
    buf[f3[index].len-1] = '\0';
}
void search_nper(int len, char*buf){
    for(int i = 0 ; i < len+1; i++){
        if(buf[i] == '\n')printf("========================================\n");
    }
}

int main(){
    FLE f2[LENGTH_MAX], f3[LENGTH_MAX];
    FILE *fp1, *fp2, *fp3;
    int index = 0;
    int n, offset = 0;
    char bool;
    char buf1[BUF_SIZE];
    char buf2[BUF_SIZE];
    char buf3[14];
    char result[BUF_SIZE];

    fp1 = fopen("Phone_number.txt", "w");
    fp2 = fopen("First_name.txt", "r");
    fp3 = fopen("names.txt", "r");
    memset(f2, 0, sizeof(f2));
    memset(f3, 0, sizeof(f3));
    srand((unsigned int)time(NULL));

    init_f2(f2,fp2);
    system("pause");
    init_f3(f3,fp3);
    system("pause");

    for(int x =0; x < 400; x++){
        memset(result,0,BUF_SIZE);
        memset(buf1,0,BUF_SIZE);
        memset(buf2,0, BUF_SIZE);
        memset(buf3,0,sizeof(buf3));
        index=0;
        bool = rand()%2;
        //bookmark
        if(bool){
            strcat(result,"Y$%$");
            index+=4;
        }else{
            strcat(result,"N$%$");
            index+=4;
        }
        printf("add bool result %s\n",result);
        search_nper(strlen(result),result);
        //name
        search_f2(rand()%51,buf1,f2,fp2);
        strcat(result,buf1);
        printf("add name %s \n", result);
        search_f3(rand()%71,buf2,f3,fp3);
        strcat(result,buf2);
        strcat(result,"$%$");
        printf("add name %s\n", result);
        //phone-number
        strcpy(buf3,"010-");
        for(int j = 4; j < 8; j++){
            buf3[j] = '0'+rand()%10;
        }
        buf3[8] = '-';
        for(int j = 9; j < 9+4; j++){
            buf3[j] = '0'+rand()%10;
        }
        strcat(result, buf3);
        strcat(result,"$%$");
        printf("add phone_number %s \n", result);
        strcat(result,"\n");
        printf("result: %s\n\n",result);
        fwrite(result,sizeof(char),strlen(result),fp1);
    }
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);

}