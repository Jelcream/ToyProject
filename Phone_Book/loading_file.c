#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#define BUF_SIZE 150
#define TAB_SIZE 400
#define BOOK_START 1

typedef struct node{
    long offset;
    long name_start; // offset + name_start
    long phone_start;
    long memo_start;
    int name_len;
    int phone_len;
    int memo_len;
    int len;
}Table;
Table phone_table[TAB_SIZE];


void init_table(FILE *fp){
    long offset = 0;
    int entry = 1, n;
    int index = 0, order = 0;
    int enter = 0; // in windows enter is linefeed ,carriage return
    char buf[BUF_SIZE];
    memset(phone_table, 0, sizeof(phone_table));

    while(feof(fp)==0){
        n = fread(buf,sizeof(char),BUF_SIZE,fp);
        printf("===========%d===============\n",n);

        for(int i = 0; i < n ; i++){
            phone_table[index].len++;
            offset++;

            if(buf[i] == '$'|| buf[i]=='%')order++;

            switch(order){
                case 3:
                    printf("test============ %d\n", offset);
                    phone_table[index].name_start = offset;
                    printf("test============ %d\n", phone_table[index].name_start);
                    order++;
                    break;
                case 7:
                    phone_table[index].name_len = offset - phone_table[index].name_start -3;
                    phone_table[index].phone_start = offset;
                    order++;
                    break;
                case 10:
                    phone_table[index].phone_len = offset - phone_table[index].phone_start -2;
                    phone_table[index].memo_start = offset+1;
                    order++;
                    break;  
            }
            if(buf[i]=='\n'){
                phone_table[index++].memo_len = offset - phone_table[index].memo_start;
                phone_table[index].offset = ++offset
                ;
                order=0;
                printf("%ld %ld %d %ld %d %ld %d %d\n", phone_table[index-1].offset, phone_table[index-1].name_start, phone_table[index-1].name_len, phone_table[index-1].phone_start,phone_table[index-1].phone_len,phone_table[index-1].memo_start,phone_table[index-1].memo_len,phone_table[index-1].len--);
                //system("pause");
            }

        }
    }
}

void search_test(FILE *fp, char *buf, int index){
    fseek(fp,phone_table[index].offset,SEEK_SET);
    fread(buf,sizeof(char),phone_table[index].len,fp);
    buf[phone_table[index].len] ='\0';
    printf("%d %s\n", index, buf);
}
int * search_name(FILE*fp, char* name, int name_len){
    int *index = (int*)malloc(sizeof(int)*100);
    int index_len = 0;
    char buf[100];
    for(int i =0 ; i <TAB_SIZE; i++){
        fseek(fp,phone_table[i].name_start,SEEK_SET);
        fread(buf,sizeof(char),phone_table[i].name_len,fp);
        buf[name_len] = '\0';
        //printf("%s\n",buf);
        if(strcmp(name, buf) == 0){
            index[index_len++] = i;
        }
    }
    index[index_len] = -1;
    return index;
}

int * search_phone(FILE*fp, char* phone){
    int *index = (int*)malloc(sizeof(int)*100);
    int index_len = 0;
    char buf[100];
    for(int i =0 ; i <TAB_SIZE; i++){
        fseek(fp,phone_table[i].phone_start,SEEK_SET);
        fread(buf,sizeof(char),phone_table[i].phone_len,fp);
        buf[phone_table[i].phone_len] = '\0';
        printf("%s\n",buf);
        if(strcmp(phone, buf) == 0){
            index[index_len++] = i;
        }
    }
    index[index_len] = -1;
    return index;
}

void list_Phone(FILE * fp, int book){
    int phone, phone_len, name, name_len, memo, memo_len;
    char name_buf[50], phone_buf[50], memo_buf[50];
    char b1[2];
    for(int i = 0 ; i < TAB_SIZE; i++){
        
        fseek(fp,phone_table[i].offset,SEEK_SET);
        fread(&b1,sizeof(char),2,fp);
        b1[1] = '\0';
        if(book == 0 && b1[0] == 'N')continue;

        memset(name_buf, 0, 50);
        name = phone_table[i].name_start;
        name_len = phone_table[i].name_len;
        memset(phone_buf, 0, 50);
        phone  = phone_table[i].phone_start;
        phone_len = phone_table[i].phone_len;
        memset(memo_buf, 0, 50);
        memo = phone_table[i].memo_start;
        memo_len = phone_table[i].memo_len;
        fseek(fp,name,SEEK_SET);
        fread(name_buf,sizeof(char),name_len,fp);
        name_buf[name_len]='\0';
        fseek(fp,phone,SEEK_SET);
        fread(phone_buf, sizeof(char),phone_len,fp);
        phone_buf[phone_len]='\0';
            
        fseek(fp,memo,SEEK_SET);
        fread(memo_buf, sizeof(char),memo_len,fp);
        memo_buf[memo_len]='\0';
        printf("=========================================\nname: %s\nphone number: %s\nbook mark: %s\nmemo: %s\n=========================================",name_buf,phone_buf,b1,memo_buf);
        if((i + 1) % 10 == 0)system("pause");
    }
}
void print_ui(){
    printf("\t\t================================================\n");
    printf("\t\t|                                              |\n");
    printf("\t\t|         Phone Book Management System         |\n");
    printf("\t\t|                                              |\n");
    printf("\t\t================================================\n");
    printf("\n\n");
    printf("\t\t1. 이름 검색\n");
    printf("\t\t2. 전화번호 검색\n"); 
    printf("\t\t3. 연락처 추가\n");
    printf("\t\t4. 연락처 삭제\n");
    printf("\t\t5. 연락처 리스트\n");
    printf("\t\t6. 즐겨찾기 리스트\n");
    printf("\t\t7. 연락처 정보 수정\n");
    printf("\n");
    printf("\t\t원하는 숫자를 입력해주세요.>");
}
int main(){
    FILE *fp;
    fp = fopen("data\\Phone_number.txt","r");
    init_table(fp);
    char buf[BUF_SIZE];
    for(int i = 0 ; i < TAB_SIZE ; i++){
        memset(buf,0,BUF_SIZE);
        search_test(fp, buf, i);
    }
    //system("cls");
    //print_ui();
    system("cls");
    int * arr = search_name(fp,"지",2);
    for(int i = 0; arr[i] != -1; i++){
        if(arr[i] != -1){
            fseek(fp,phone_table[arr[i]].name_start,SEEK_SET);
            memset(buf,0,BUF_SIZE);
            fread(buf,sizeof(char),phone_table[arr[i]].name_len,fp);
            buf[phone_table[arr[i]].name_len] = '\0';
            printf("%s\n", buf);
        }
    }
    arr = search_phone(fp,"010-0770-3160");
    for(int i = 0; arr[i] != -1; i++){
        if(arr[i] != -1){
            fseek(fp,phone_table[arr[i]].phone_start,SEEK_SET);
            memset(buf,0,BUF_SIZE);
            fread(buf,sizeof(char),phone_table[arr[i]].phone_len,fp);
            buf[phone_table[arr[i]].phone_len] = '\0';
            printf("\n%s\n", buf);
        }
    }
    system("cls");
    list_Phone(fp,0);
    fclose(fp);

    return 0;
}