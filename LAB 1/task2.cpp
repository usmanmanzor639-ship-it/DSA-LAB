// MUHAMMAD USMAN
// DSALA-2025S-CS-48
// LAB 01
// 2D Array Spiral Fill
#include<iostream>
using namespace std;
int main(){
    int arr[4][5];
    int value=1;
    
    for(int i=0;i<5;i++){
        arr[0][i]=value++;
    }
    for(int j=1;j<3;j++){
        arr[j][4]=value++;
    }
    for(int i=4;i>=0;i--){
        arr[3][i]=value++;
    }
    for(int j=2;j>=1;j--){
        arr[j][0]=value++;
    }
    for(int i=1;i<4;i++){
        arr[1][i]=value++;

    }
    for(int j=3;j>0;j--){
        arr[2][j]=value++;
    }

    for(int i=0;i<4;i++){
        for(int j=0;j<5;j++){
            cout<<arr[i][j]<<" ";
        }
        cout<<endl;
    }
    return 0;
}

