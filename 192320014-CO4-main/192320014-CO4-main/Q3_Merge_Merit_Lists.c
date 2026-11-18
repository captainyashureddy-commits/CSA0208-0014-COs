#include <stdio.h>
int merge_desc(const int a[],int na,const int b[],int nb,int out[]){int i=0,j=0,k=0;while(i<na&&j<nb){if(a[i]>=b[j])out[k++]=a[i++];else out[k++]=b[j++];}while(i<na)out[k++]=a[i++];while(j<nb)out[k++]=b[j++];return k;}
int main(void){int a[]={98,91,85,80},b[]={95,91,72},out[7];int n=merge_desc(a,4,b,3,out);for(int i=0;i<n;i++)printf("%d%s",out[i],i==n-1?"\n":" ");return 0;}