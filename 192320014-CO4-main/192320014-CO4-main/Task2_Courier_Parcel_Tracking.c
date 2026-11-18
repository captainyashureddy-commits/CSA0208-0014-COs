#include <stdio.h>
#include <string.h>
typedef struct{char city[30];int pin;} Address;
typedef struct{int day,month,year;} Date;
typedef struct{char name[40];Address address;} Person;
typedef struct{int id;float weight;Date booked;Person sender,receiver;int express;float charge;} Parcel;
void print_parcel(const Parcel *p){printf("%d %.1f kg %s-%d %02d/%02d/%04d\n",p->id,p->weight,p->receiver.address.city,p->receiver.address.pin,p->booked.day,p->booked.month,p->booked.year);}
void sort_by_weight(Parcel p[],int n){for(int i=0;i<n-1;i++){int m=i;for(int j=i+1;j<n;j++)if(p[j].weight<p[m].weight)m=j;if(m!=i){Parcel t=p[i];p[i]=p[m];p[m]=t;}}}
int main(void){Parcel p[3]={{501,4.5,{12,10,2026},{"Ravi",{"Madurai",625001}},{"A",{"Madurai",625001}},0,50},{502,1.2,{12,10,2026},{"K",{"Chennai",600077}},{"B",{"Chennai",600077}},1,80},{503,2.8,{13,10,2026},{"M",{"Coimbatore",641001}},{"C",{"Coimbatore",641001}},0,60}};sort_by_weight(p,3);for(int i=0;i<3;i++)print_parcel(&p[i]);return 0;}