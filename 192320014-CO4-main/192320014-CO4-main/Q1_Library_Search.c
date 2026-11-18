#include <stdio.h>
int comparisons;
int linear_search(const int a[], int n, int key) {
    comparisons = 0;
    for (int i=0; i<n; i++) { comparisons++; if (a[i]==key) return i; }
    return -1;
}
int binary_search(const int a[], int n, int key) {
    int low=0, high=n-1; comparisons=0;
    while (low<=high) {
        int mid=low+(high-low)/2; comparisons++;
        if (a[mid]==key) return mid;
        if (a[mid]<key) low=mid+1; else high=mid-1;
    }
    return -1;
}
int main(void) {
    int unsorted[]={1222,1021,1460,1103,1533,1305,1048,1402,1190,1377};
    int sorted[]={1021,1048,1103,1190,1222,1305,1377,1402,1460,1533};
    int keys[]={1021,1305,1533,1400}, n=10;
    for(int i=0;i<4;i++) {
        int p=linear_search(unsorted,n,keys[i]);
        printf("Linear: %d -> index %d, comparisons=%d\n",keys[i],p,comparisons);
        p=binary_search(sorted,n,keys[i]);
        printf("Binary: %d -> index %d, comparisons=%d\n",keys[i],p,comparisons);
    }
    return 0;
}