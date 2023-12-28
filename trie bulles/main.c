#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
void affich(int *t,int n,int j){
    for(int i=0;i<n;i++){
        if(i==j) printf("[%d] ",t[i]);
        else printf("%d ",t[i]);
    }
    printf("\n");
}
int main()
{
    bool p;
    int *t,n,j,tt,i;
    scanf("%d",&n);
    t=(int *)malloc(sizeof(int)*n);
    for(i=0;i<n;i++){
        printf("t[%d]=",i+1);
        scanf("%d",&t[i]);
    }
    for(int i=n-2;i>-1;i--){
        for(j=0;j<=i;j++){
             affich(t,n,j);
            if(t[j]>t[j+1]){
                tt=t[j];
                t[j]=t[j+1];
                t[j+1]=tt;
            }

        }
        affich(t,n,j);

    }

    return 0;
}
