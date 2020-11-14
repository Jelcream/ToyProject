#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/stat.h>



#define BUF_SIZE 150
#define TAB_SIZE 500
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
int table_max;
char * ptr;
void init_table(FILE*, int);
void search_test(FILE*,char*,int);
int *search_name(FILE *,char*,int);
int * search_phone(FILE*,char*);
void list_Phone(FILE*,int);
void add_phone();
char* make_phone_number_item(char, char*,char*,char*);
void print_ui();
void add_table(FILE*);
void remove_phone();
void print_item(FILE*,int*);
void remove_item(FILE*, int);
void modify_contact();
void modify_ui(FILE* fp);
char* getName(FILE*,int index);
char* getPhone(FILE*, int index);
char* getBook(FILE*, int index);
char* getMemo(FILE *, int index);
long file_length(char *f){
    struct stat st;
    stat(f, &st);
    return st.st_size;
}
void remove_buf(){
    while(getchar()!='\n');
}


int main(int argc, char*argv[]){
    FILE *fp;
    if(argc < 2){
        printf("This program needs Phone Number's file path");
        return 0;
    }
    ptr = argv[argc-1];
    fp = fopen(ptr,"r+");
    if(fp == NULL){
        printf("can't open file\n");
        return 0;
    }
    init_table(fp,0);
    system("clear");
    int * arr;
    int len;
    char select[2];
    int index;
    char buf[BUF_SIZE];
    while(1){
        len = 0;
        memset(buf,0,sizeof(buf));
        print_ui();
        select[0] = getchar();getchar();
        system("clear");
        switch(select[0]){
            case '1'://Search name
                //input name, need exact name
                printf("Enter the name for searching phone number: ");
                scanf("%s",buf);
                arr = search_name(fp,buf,strlen(buf));
                print_item(fp,arr);
                free(arr);
                break;
            case '2'://Search Phone Number
                //input phone number, need exact name
                printf("Enter the phone number for searching phone number: ");
                len = scanf("%s",buf);
                arr = search_phone(fp,buf);
                print_item(fp,arr);
                free(arr);
                break;
            case '3'://Add contact
                add_phone(fp);
                break;
            case '4'://Remove contact
                printf("Enter the contact number to be removed: ");
                scanf("%d",&index);
                remove_item(fp,index);
                break;
            case '5'://Phone Number list
                list_Phone(fp,0);// book 0 is phone number list
                break;
            case '6'://Book mark list
                list_Phone(fp,1);//book 1 is bookmarked phone number list
                break;
            case '7'://Modify contact information
                modify_contact(fp);
                break;
            case '8'://Program exit
                fclose(fp);
                return 0;
            default:
                printf("Please enter the number\n");
        }
        remove_buf();
        system("clear");
    }
    system("clear");
    //list_Phone(fp,0); // 0 -> Booked, 1 -> not book
    fclose(fp);
    //add_phone();
    return 0;
}

void init_table(FILE *fp, int init ){
    // ****error control****
    long offset = 0;
    int n;
    int index = 0, order = 0;
    // in windows enter is linefeed ,carriage return
    char buf[BUF_SIZE];

    if(!init){
        fseek(fp,0,SEEK_SET);
        memset(phone_table, 0, sizeof(phone_table));
    }
    else{
        index = table_max;
        offset = phone_table[index].offset;
        fseek(fp,offset,SEEK_SET);
    }

    while(feof(fp)==0){
        n = fread(buf,sizeof(char),BUF_SIZE,fp);
        printf("===========%d===============\n",n);
        if(!n)return;
        //if(def) system("pause");
        for(int i = 0; i < n ; i++){
            phone_table[index].len++;
            offset++;

            if(buf[i] == '$'|| buf[i]=='%')order++;

            switch(order){
                case 3:
                    //printf("test============ %d\n", offset);
                    phone_table[index].name_start = offset;
                    //printf("test============ %d\n", phone_table[index].name_start);
                    order++;
                    break;
                case 7:
                    phone_table[index].name_len = offset - phone_table[index].name_start -3;
                    phone_table[index].phone_start = offset;
                    order++;
                    break;
                case 11:
                    phone_table[index].phone_len = offset - phone_table[index].phone_start -3;
                    phone_table[index].memo_start = offset;
                    order++;
                    break;  
            }
            if(buf[i]=='\n'){
                phone_table[index++].memo_len = offset - phone_table[index].memo_start;
                phone_table[index].offset = offset;
                table_max = index;
                order=0;
                //printf("%ld %ld %d %ld %d %ld %d %d\n", phone_table[index-1].offset, phone_table[index-1].name_start, phone_table[index-1].name_len, phone_table[index-1].phone_start,phone_table[index-1].phone_len,phone_table[index-1].memo_start,phone_table[index-1].memo_len, phone_table[index-1].len--);
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
    char buf[BUF_SIZE];
    fflush(stdin);
    index[0] = -1;
    for(int i =0 ; i <table_max; i++){
        fseek(fp,phone_table[i].name_start,SEEK_SET);
        fread(buf,sizeof(char),phone_table[i].name_len,fp);
        buf[name_len] = '\0';
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
    index[0] = -1;
    fflush(stdin);
    for(int i =0 ; i <TAB_SIZE; i++){
        fseek(fp,phone_table[i].phone_start,SEEK_SET);
        fread(buf,sizeof(char),phone_table[i].phone_len,fp);
        buf[phone_table[i].phone_len] = '\0';
        //printf("%s\n",buf);
        if(!strcmp(phone, buf)){
            index[index_len++] = i;
        }
    }
    index[index_len] = -1;
    return index;
}

void list_Phone(FILE * fp, int book){
    //book mark per is strange
    int phone, phone_len, name, name_len, memo, memo_len;
    char name_buf[50], phone_buf[50], memo_buf[50];
    int bookmarked[TAB_SIZE];
    char b1[2];
    int index = 0;
    int i;
    for(i = 0; i < table_max; i++){
        fseek(fp,phone_table[i].offset,SEEK_SET);
        fread(&b1,sizeof(char),2,fp);
        b1[1] = '\0';
        if(book == 1 && b1[0] == 'N')continue;
        bookmarked[index++] = i;
    }
    for(int j = 0 ; j < index; j++){
        i = bookmarked[j];
        fseek(fp,phone_table[i].offset,SEEK_SET);
        fread(&b1,sizeof(char),2,fp);
        b1[1] = '\0';
        if(book == 1 && b1[0] == 'N')continue;
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
        printf("=========================================\nno.%d\nname: %s\nphone number: %s\nbook mark: %s\nmemo: %s\n=========================================\n",i,name_buf,phone_buf,b1,memo_buf);
        if((j + 1) % 3 == 0){
            printf("current page (%d / %d)\n", j/3+1, index/3.0 > index/3 ? index/3 + 1 : index/3);
            getchar();
            system("clear");
        }else if(j >= index-2 && j == index - 1){
            printf("current page (%d / %d)\n", j/3+1, index/3.0 > index/3 ? index/3 + 1 : index/3);
            getchar();
            system("clear");
        }
    }
}

void print_ui(){
    printf("\t\t================================================\n");
    printf("\t\t|                                              |\n");
    printf("\t\t|         Phone Book Management System         |\n");
    printf("\t\t|                                              |\n");
    printf("\t\t================================================\n");
    printf("\n\n");
    printf("\t\t1. Search name\n");
    printf("\t\t2. Search Phone Number\n"); 
    printf("\t\t3. Add contact\n");
    printf("\t\t4. Remove contact\n");
    printf("\t\t5. Contact list\n");
    printf("\t\t6. Book mark list\n");
    printf("\t\t7. Modify contact information\n");
    printf("\t\t8. Program exit\n");
    printf("\n");
    printf("\t\tPlease enter the number.> ");
}

char* make_phone_number_item(char book, char*name, char*phone, char* memo){
    char* result = (char *) malloc(sizeof(char)*BUF_SIZE);
    result[0] = book;result[1] ='\0';
    strcat(result, "$%$");
    strcat(result, name);
    strcat(result, "$%$");
    strcat(result, phone);
    strcat(result, "$%$");
    if(strlen(memo)!=0)
        strcat(result, memo);
    strcat(result, "\n");
    printf("%s",result);
    return result;
}

void add_phone(FILE * fp){
    // need input: name, phone, memo, book
    // ****error control****
    char name_buf[50], phone_buf[50], memo_buf[50], book;
    char result[BUF_SIZE];
    memset(memo_buf,0,sizeof(memo_buf));
    printf("\t\t          Add Contact\n");
    printf("name: ");scanf("%s", name_buf);
    printf("phone: ");scanf("%s", phone_buf);
    printf("Do you want take the notes?(Y/N)"); scanf(" %c", &book);
    if(book == 'y' || book == 'Y'){
        printf("memo: ");scanf("%s",memo_buf);
    }
    printf("Do you want a book mark on this phone Number?(Y/N): ");scanf(" %c", &book);
    strcpy(result, make_phone_number_item(book, name_buf, phone_buf, memo_buf));
    fseek(fp,0,SEEK_END);
    //printf("%d\n",strlen(result));
    fwrite(result,sizeof(char),strlen(result),fp);

    // new line on the table
    //printf("before phone_table: %d %d\n",phone_table[table_max-1].offset,phone_table[table_max-1].len);
    //fclose(fp);
    // fp = fopen("data\\Phone_number.txt", "r");
    // fseek(fp,phone_table[table_max].offset,SEEK_SET);
    // init_table(fp,1);
    // printf("after phone_table: %d %d", phone_table[table_max].offset,phone_table[table_max].len);
    fclose(fp);
};

void print_item(FILE* fp, int *arr ){
    int phone, phone_len, name, name_len, memo, memo_len;
    char name_buf[50], phone_buf[50], memo_buf[50];
    char b1[2];
    int len = 0;
    int i;
    remove_buf();
    for(len=0; arr[len]!=-1; len++);

    for(int j = 0 ; j<len; j++){
        i = arr[j];
        fseek(fp,phone_table[i].offset,SEEK_SET);
        fread(&b1,sizeof(char),2,fp);
        b1[1] = '\0';

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
        printf("=========================================\nno.%d\nname: %s\nphone number: %s\nbook mark: %s\nmemo: %s\n=========================================\n",i,name_buf,phone_buf,b1,memo_buf);
        if((j + 1) % 3 == 0){
            printf("current page (%d / %d)\n", j/3+1, len/3.0 > len/3 ? len/3 + 1 : len/3);
            getchar();
            system("clear");
        }else if(j >= len-3 && j == len - 1){
            printf("current page (%d / %d)\n", j/3+1, len/3.0 > len/3 ? len/3 + 1 : len/3);
            getchar();
            system("clear");
        }
    }
}

void remove_item(FILE* fp, int index){
    char*buf;
    long len;
    fseek(fp, phone_table[index+1].offset,SEEK_SET);
    len = file_length(ptr) - ftell(fp);
    buf = (char *) malloc(len);
    memset(buf,0,len);
    len = fread(buf,1,len,fp);
    fseek(fp,phone_table[index].offset,SEEK_SET);
    fwrite(buf,sizeof(char),len,fp);
    fflush(fp);
    free(buf);
    ftruncate(fileno(fp),ftell(fp));
    table_max = index;
    init_table(fp,1);
    printf("Clear\n");
    getchar();
}

void modify_contact(FILE * fp){
    int index, len, offset =0,size;
    char check;
    char bookmark;
    char* buf, *buf2;
    char name_buf[50], phone_buf[15], memo_buf[100];
    printf("\t\t          Modify contact\n\n");
    printf("Please enter the number to be modified: ");scanf("%d",&index);
    if(index < 0 || index > table_max){
        printf("There is no number. \n");
        return;
    }
    buf = getBook(fp, index);
    bookmark = buf[0];
    printf("\nBefore : %c\n",bookmark);
    free(buf);
    printf("Would you like to change your bookmark?(Y/N): ");
    scanf(" %c", &check);
    if(check == 'Y'|| check == 'y' ){
        bookmark =0;
        while(bookmark != 'Y' && bookmark != 'y' && bookmark != 'N' && bookmark != 'n'){
            printf("\nEnter the BookMark: ");
            scanf(" %c",&bookmark);
        }
        bookmark = toupper(bookmark);
    }

    buf = getName(fp,index);
    strncpy(name_buf,buf,phone_table[index].name_len);
    name_buf[phone_table[index].name_len]='\0';
    printf("\nBefore: %s\n",name_buf);
    printf("Would you like to change the name?(Y/N): ");
    scanf(" %c", &check);
    if(check == 'Y'|| check == 'y' ){
        printf("Enter the name: ");
        scanf("%s",name_buf);
    }
    free(buf);

    buf = getPhone(fp,index);
    strncpy(phone_buf,buf,phone_table[index].phone_len);
    printf("\nBefore: %s\n",phone_buf);
    printf("Would you like to change the phone number?(Y/N): ");
    scanf(" %c", &check);
    if(check == 'Y'|| check == 'y' ){
        printf("Enter the phone number: ");
        scanf("%s",phone_buf);
    }
    free(buf);

    buf = getMemo(fp,index);
    strncpy(memo_buf,buf,phone_table[index].memo_len);
    printf("\nBefore: %s\n",memo_buf);
    printf("Would you like to change the memo?(Y/N): ");
    scanf(" %c", &check);
    if(check == 'Y'|| check == 'y' ){
        printf("Enter the memo: ");
        scanf("%s", memo_buf);
    }
    free(buf);

    printf("no.%d\nname: %s\nPhone number: %s\nmemo: %s\n",index,name_buf,phone_buf,memo_buf);
    size = 1+3+strlen(name_buf)+3+strlen(phone_buf)+3+strlen(memo_buf)+2;
    buf = (char*) malloc(size);
    memset(buf,0,size);
    buf[0] = bookmark;
    strcat(buf, "$%$");
    strcat(buf, name_buf);
    strcat(buf, "$%$");
    strcat(buf, phone_buf);
    strcat(buf, "$%$");
    strcat(buf, memo_buf);
    //strcat(buf,"\n");
    printf("%s\n",buf);
    //init_table(fp,1);

    fseek(fp,phone_table[index+1].offset,SEEK_SET);
    len = file_length(ptr)+ftell(fp);
    buf2 = (char *)malloc(len);
    memset(buf2,0,len);

    len = fread(buf2, sizeof(char), len, fp);
    fseek(fp,phone_table[index].offset,SEEK_SET);
    fwrite(buf,sizeof(char),strlen(buf),fp);
    //fflush(fp);
    fwrite(buf2,sizeof(char),len,fp);
    //fflush(fp);
    ftruncate(fileno(fp),ftell(fp));

    init_table(fp,0);
    free(buf2);
    free(buf);
}


char* getName(FILE* fp, int index){
    char* name_buf;
    fseek(fp,phone_table[index].name_start,SEEK_SET);
    name_buf = (char *) malloc(phone_table[index].name_len);
    fread(name_buf,sizeof(char),phone_table[index].name_len,fp);
    name_buf[phone_table[index].name_len] = '\0'; 
    return name_buf;
}
char* getPhone(FILE*fp,int index){
    char* buf;
    fseek(fp,phone_table[index].phone_start,SEEK_SET);
    buf = (char *) malloc(phone_table[index].phone_len+1);
    fread(buf,sizeof(char),phone_table[index].phone_len,fp);
    buf[phone_table[index].phone_len]='\0';
    return buf;
}
char* getBook(FILE*fp,int index){
    char* buf;
    fseek(fp,phone_table[index].offset,SEEK_SET);
    buf = (char *) malloc(2);
    fread(buf,sizeof(char),2,fp);
    return buf;
}
char* getMemo(FILE*fp,int index){
