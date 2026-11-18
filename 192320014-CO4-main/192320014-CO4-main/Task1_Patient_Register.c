#include <stdio.h>
#include <string.h>
typedef struct{int day,month,year;} Date;
typedef struct{int systolic,diastolic;} BloodPressure;
typedef struct{char name[50],phone[16];int age;BloodPressure bp;Date visit;} Patient;
int addPatient(Patient a[],int *count,int capacity,const Patient *p){if(*count>=capacity)return 0;a[(*count)++]=*p;return 1;}
int main(void){Patient list[100];int n=0;Patient p={"Anu","9876543210",35,{120,80},{13,11,2026}};if(addPatient(list,&n,100,&p)){Patient *x=&list[0];printf("%s, age %d, phone %s, BP %d/%d, visit %02d/%02d/%04d\n",x->name,x->age,x->phone,x->bp.systolic,x->bp.diastolic,x->visit.day,x->visit.month,x->visit.year);}return 0;}