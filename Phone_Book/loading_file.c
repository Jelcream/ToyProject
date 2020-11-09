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
    int entry = 1,n;
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
                    phone_table[index].name_start = offset -1;
                    printf("test============ %d\n", phone_table[index].name_start);
                    order++;
                    break;
                case 7:
                    phone_table[index].name_len = offset - phone_table[index].name_start -3;
                    phone_table[index].phone_start = offset -1;
                    order++;
                    break;
                case 10:
                    phone_table[index].phone_len = offset - phone_table[index].phone_start -2;
                    phone_table[index].memo_start = offset -1;
                    order++;
                    break;  
            }
            if(buf[i]=='\n'){
                phone_table[index++].memo_len = offset - phone_table[index].memo_start -1;
                phone_table[index].offset = offset + ++enter;
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
int main(){
    FILE *fp;
    fp = fopen("data\\Phone_number.txt","r");
    init_table(fp);
    char buf[BUF_SIZE];
    for(int i = 0 ; i < TAB_SIZE ; i++){
        memset(buf,0,BUF_SIZE);
        search_test(fp, buf, i);
    }
    return 0;
}