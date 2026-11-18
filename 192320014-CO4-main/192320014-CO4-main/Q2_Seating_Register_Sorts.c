#include <stdio.h>
void bubble(int a[],int n,long *c,long *s){*c=*s=0;for(int i=0;i<n-1;i++){int changed=0;for(int j=0;j<n-i-1;j++){(*c)++;if(a[j]>a[j+1]){int t=a[j];a[j]=a[j+1];a[j+1]=t;(*s)++;changed=1;}}if(!changed)break;}}
void selection(int a[],int n,long *c,long *s){*c=*s=0;for(int i=0;i<n-1;i++){int m=i;for(int j=i+1;j<n;j++){(*c)++;if(a[j]<a[m])m=j;}if(m!=i){int t=a[i];a[i]=a[m];a[m]=t;(*s)++;}}}
void insertion(int a[],int n,long *c,long *s){*c=*s=0;for(int i=1;i<n;i++){int key=a[i],j=i-1;while(j>=0){(*c)++;if(a[j]>key){a[j+1]=a[j];(*s)++;j--;}else break;}a[j+1]=key;}}
void show(const char *name,void(*sort)(int[],int,long*,long*),int src[],int n){int a[20];for(int i=0;i<n;i++)a[i]=src[i];long c,s;sort(a,n,&c,&s);printf("%s: ",name);for(int i=0;i<n;i++)printf("%d ",a[i]);printf("| comparisons=%ld moves/swaps=%ld\n",c,s);}
int main(void){int a[]={45,12,67,23,12,89,34,5},n=8;show("Bubble",bubble,a,n);show("Selection",selection,a,n);show("Insertion",insertion,a,n);return 0;}