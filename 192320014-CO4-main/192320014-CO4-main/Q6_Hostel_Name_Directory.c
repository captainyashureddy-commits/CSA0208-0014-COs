#include <stdio.h>
#include <string.h>
#define MAX 100
#define LEN 30
void strip_newline(char s[]){s[strcspn(s,"\n")]='\0';}
void sort_names(char a[][LEN],int n){char temp[LEN];for(int i=1;i<n;i++){strcpy(temp,a[i]);int j=i-1;while(j>=0&&strcmp(a[j],temp)>0){strcpy(a[j+1],a[j]);j--;}strcpy(a[j+1],temp);}}
int binary_search(char a[][LEN],int n,const char key[]){int l=0,r=n-1;while(l<=r){int m=l+(r-l)/2,c=strcmp(a[m],key);if(c==0)return m;if(c<0)l=m+1;else r=m-1;}return -1;}
int main(void){char names[MAX][LEN],key[LEN];int n;printf("Number of residents (1-%d): ",MAX);if(scanf("%d",&n)!=1||n<1||n>MAX)return 1;getchar();for(int i=0;i<n;i++){printf("Name %d: ",i+1);if(!fgets(names[i],LEN,stdin))return 1;strip_newline(names[i]);}sort_names(names,n);puts("\nSorted directory:");for(int i=0;i<n;i++){printf("%s",names[i]);if(i&&strcmp(names[i],names[i-1])==0)printf(" [DUPLICATE]");putchar('\n');}printf("Search name: ");if(!fgets(key,LEN,stdin))return 1;strip_newline(key);int p=binary_search(names,n,key);printf(p>=0?"Found at sorted index %d\n":"Not found\n",p);return 0;}