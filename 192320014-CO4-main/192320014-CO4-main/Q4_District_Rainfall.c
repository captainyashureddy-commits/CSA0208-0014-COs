#include <stdio.h>
#define D 3
#define M 4
void district_totals(float r[D][M]){for(int i=0;i<D;i++){float s=0;for(int j=0;j<M;j++)s+=r[i][j];printf("District %d annual total: %.1f\n",i+1,s);}}
void monthly_averages(float r[D][M]){for(int j=0;j<M;j++){float s=0;for(int i=0;i<D;i++)s+=r[i][j];printf("Month %d average: %.1f\n",j+1,s/D);}}
void wettest(float r[D][M]){float mx=r[0][0];int di=0,mj=0;for(int i=0;i<D;i++)for(int j=0;j<M;j++)if(r[i][j]>mx){mx=r[i][j];di=i;mj=j;}printf("Wettest: district %d month %d = %.1f\n",di+1,mj+1,mx);}
int main(void){float rain[D][M]={{80,60,40,20},{30,90,70,50},{50,40,100,60}};district_totals(rain);monthly_averages(rain);wettest(rain);return 0;}