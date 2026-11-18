#include <stdio.h>
#include <string.h>
typedef struct{int day,month,year;} Date;
enum ItemType{PACKAGED,COOKED,COMBO};
typedef struct{int id;char name[30];enum ItemType type;float price;int stock;union{Date expiry;char prepared[6];int componentIds[3];}detail;float revenue;} Item;
int sell(Item *p,int qty){if(qty<=0||qty>p->stock)return 0;p->stock-=qty;p->revenue+=p->price*qty;return 1;}
void warn_expiry(const Item *p,Date today){if(p->type==PACKAGED&&p->detail.expiry.year==today.year&&p->detail.expiry.month==today.month&&p->detail.expiry.day>=today.day&&p->detail.expiry.day-today.day<=2)printf("Expiry warning: %s\n",p->name);}
void sort_revenue(Item a[],int n){for(int i=0;i<n-1;i++)for(int j=i+1;j<n;j++)if(a[j].revenue>a[i].revenue){Item t=a[i];a[i]=a[j];a[j]=t;}}
int main(void){Item a[3]={{1,"Milk",PACKAGED,30,10,{.expiry={14,11,2026}},0},{2,"Sandwich",COOKED,50,8,{.prepared="10:30"},0},{3,"Combo Meal",COMBO,100,5,{.componentIds={1,2,0}},0}};Date today={13,11,2026};sell(&a[0],2);sell(&a[1],1);if(!sell(&a[2],9))puts("Sale rejected: insufficient stock.");for(int i=0;i<3;i++)warn_expiry(&a[i],today);sort_revenue(a,3);puts("Daily revenue summary:");for(int i=0;i<3;i++)printf("%s: Rs. %.2f (stock %d)\n",a[i].name,a[i].revenue,a[i].stock);return 0;}