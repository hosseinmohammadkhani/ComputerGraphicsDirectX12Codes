#include <iostream>
using namespace std;

void printMatrix(int mat[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            cout << mat[i][j] << " ";
        }
        cout << endl;
    }
}

void sumMat(int mat1[4][4] , int mat2[4][4]){
    int sumMat[4][4] = {{0,0,0,0}, {0,0,0,0}, {0,0,0,0} , {0,0,0,0}};
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            sumMat[i][j] = mat1[i][j] + mat2[i][j];
        }
    }
    printMatrix(sumMat);
}

void subMat(int mat1[4][4] , int mat2[4][4]){
    int subMat[4][4] = {{0,0,0,0}, {0,0,0,0}, {0,0,0,0} , {0,0,0,0}};
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            subMat[i][j] = mat1[i][j] - mat2[i][j];
        }
    }
    printMatrix(subMat);
}

void mulMat(int mat1[4][4] , int mat2[4][4]){
    int mulMat[4][4] = {{0,0,0,0}, {0,0,0,0}, {0,0,0,0} , {0,0,0,0}};
    for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                mulMat[i][j] = 0;
                for(int k = 0; k < 4; k++){
                    mulMat[i][j] += (mat1[i][k] * mat2[k][j]);
                }
            }
        }
    printMatrix(mulMat);
}

void taranahade(int mat1[4][4]){  
    int tMat[4][4] = {{0,0,0,0}, {0,0,0,0}, {0,0,0,0} , {0,0,0,0}};
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            tMat[i][j] = mat1[j][i];
        } 
    }
    printMatrix(tMat); 
     
} 

int determinan(int mat1[4][4]) {
    int det = 0;
    for (int i = 0; i < 4; i++) {
        det += (mat1[0][i] * (mat1[1][(i + 1) % 3] * mat1[2][(i + 2) % 3] - mat1[1][(i + 2) % 3] * mat1[2][(i + 1) % 3]));
    }
    return det;
}

int main() {
    int matrix1[4][4] = {{1, 2, 3 , 4}, {5, 6 , 7 , 8}, {9 , 10 , 11 , 12} , {13 , 14 , 15 ,16}};
    int matrix2[4][4] = {{1, 2, 3 , 4}, {5, 6 , 7 , 8}, {9 , 10 , 11 , 12} , {13 , 14 , 15 ,16}};

    //چاپ کردن دو ماتریس
    cout << "ماتریس 1 : " << endl;
    printMatrix(matrix1);

    cout << "ماتریس 2 : " << endl;
    printMatrix(matrix2);

    // جمع دو ماتریس
    cout << "جمع ماتریس ها : " << endl;
    sumMat(matrix1 , matrix2);

    // تفریق دو ماتریس
    cout << "جمع ماتریس ها : " << endl;
    subMat(matrix1 , matrix2);    

    // ضرب دو ماتریس
    cout << "ضرب ماتریس ها : " << endl;
    mulMat(matrix1 , matrix2);

    // ترانهاده ی ماتریس
    cout << "ترانهاده ی ماتریس :  " << endl;
    taranahade(matrix1);

    // دترمینان مارتیس
    cout << "دترمینان ی ماتریس :  " << endl;
    cout << determinan(matrix1) << endl;

    return 0;
}

