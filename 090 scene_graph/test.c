#include <stdio.h>
void mat333Multiply(double m[3][3], double n[3][3], double mTimesN[3][3]) {
    // for (int i = 0; i < 3; i++) {
    //     for（int j = 0; j < 3; j++){
    //         double sum = 0;
    //         for (int k = 0; k <3; k++) {
    //             sum = sum + m[i][k] * n[k][j];
    //         }
    //         mTimesN[i][j] = sum;
    //     }
    // }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            double sum = 0;
            for (int k = 0; k < 3; k++) {
                sum += m[j][k] * n[k][j];
            }
            mTimesN[i][j] = sum;
        }
    }
}

int main(void) {
    printf("aaa\n");
}