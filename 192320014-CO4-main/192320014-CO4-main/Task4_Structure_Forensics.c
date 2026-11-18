#include <stdio.h>
typedef struct{int number;float balance;} Account;
void addinterest(Account *a,float rate){printf("Inside function address: %p\n",(void*)a);a->balance+=a->balance*rate/100.0f;}
int main(void){Account list[3]={{101,5000},{102,12000},{103,800}};Account *p=list;for(int i=0;i<3;i++){printf("Main address: %p\n",(void*)&list[i]);addinterest(&list[i],4.0f);}for(int i=0;i<3;i++)printf("%d %.2f\n",p[i].number,p[i].balance);return 0;}