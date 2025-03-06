#include <iostream>
#include <omp.h>

using namespace std;

const int n = 5; //Размер квадратной матрицы

//Функция для последовательного умножения квадратных матриц N*N
void singleMultiply(float** firstMatrix, float** secondMatrix, float** result, int q) {

    for (int b = 0; b < q; b++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                result[i][j] = 0;
                for (int k = 0; k < n; k++) {
                    result[i][j] += firstMatrix[i][k] * secondMatrix[k][j];
                }
            }
        }

    }
}

//Функция для параллельного умножения квадратных матриц N*N, алгоритм 1
void parallelMultiplyAlg1(float** firstMatrix, float** secondMatrix, float** result, int q) {

    for (int b = 0; b < q; b++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                float sum = 0;
#pragma omp parallel for reduction(+:sum)
                for (int k = 0; k < n; k++) {
                    sum += firstMatrix[i][k] * secondMatrix[k][j];
                }
                result[i][j] = sum;
            }
        }
    }
}

//Функция для параллельного умножения квадратных матриц N*N, алгоритм 2
void parallelMultiplyAlg2(float** firstMatrix, float** secondMatrix, float** result, int q) {

    for (int b = 0; b < q; b++) {
        for (int i = 0; i < n; i++) {
#pragma omp parallel for
            for (int j = 0; j < n; j++) {
                result[i][j] = 0;
                for (int k = 0; k < n; k++) {
                    result[i][j] += firstMatrix[i][k] * secondMatrix[k][j];
                }
            }
        }
    }
}

//Функция для параллельного умножения квадратных матриц N*N, алгоритм 3
void parallelMultiplyAlg3(float** firstMatrix, float** secondMatrix, float** result, int q) {

    for (int b = 0; b < q; b++) {
#pragma omp parallel for if (n>13) num_threads(4)
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                result[i][j] = 0;
                for (int k = 0; k < n; k++) {
                    result[i][j] += firstMatrix[i][k] * secondMatrix[k][j];
                }
            }
        }
    }
}

//Вычисление евклидовой нормы
float norm(float** matrix) {
    
    float C = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
        {
            C += matrix[i][j] * matrix[i][j];
        }
    }

    return C;
}


int main() {
    setlocale(0, "ru");
    srand(time(NULL));

    //Глобальные переменные
    float start = -0.5;
    float finish = 0.5;

    float** firstMatrix = new float* [n];
    for (int i = 0; i < n; ++i) {
        firstMatrix[i] = new float[n];
    }
    float** secondMatrix = new float* [n];
    for (int i = 0; i < n; ++i) {
        secondMatrix[i] = new float[n];
    }
    float** result = new float* [n];
    for (int i = 0; i < n; ++i) {
        result[i] = new float[n];
    }
    //float secondMatrix[n][n]{};
    //float result[n][n]{};

    int dim = 1000;
    int q;
    //

    //Заполнение матриц значениями от -0.5 до 0.5
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
        {
            firstMatrix[i][j] = (rand() % int(dim * (finish - start) + 1) + start * dim) / dim;
            secondMatrix[i][j] = (rand() % int(dim * (finish - start) + 1) + start * dim) / dim;
        }
    }
    //

    //Последовательное умножение матриц
    cin >> q;
    double start_time = omp_get_wtime();
    singleMultiply(firstMatrix, secondMatrix, result, q);
    double end_time = omp_get_wtime();
    cout << end_time - start_time << endl;
    printf("%g %g\n\n", result[0][0], result[n - 1][n - 1]);
    //

    ////Распараллеленное умножение матриц Алгоритм 1
    //cin >> q;
    //start_time = omp_get_wtime();
    //parallelMultiplyAlg1(firstMatrix, secondMatrix, result, q);
    //end_time = omp_get_wtime();
    //cout << end_time - start_time << endl;
    //printf("%g %g\n\n", result[0][0], result[n - 1][n - 1]);
    ////

    ////Распараллеленное умножение матриц Алгоритм 2
    //cin >> q;
    //start_time = omp_get_wtime();
    //parallelMultiplyAlg2(firstMatrix, secondMatrix, result, q);
    //end_time = omp_get_wtime();
    //cout << end_time - start_time << endl;
    //printf("%g %g\n\n", result[0][0], result[n - 1][n - 1]);
    ////

    //Распараллеленное умножение матриц Алгоритм 3
    //cin >> q;
    start_time = omp_get_wtime();
    parallelMultiplyAlg3(firstMatrix, secondMatrix, result, q);
    end_time = omp_get_wtime();
    cout << end_time - start_time << endl;
    printf("%g %g\n\n", result[0][0], result[n - 1][n - 1]);
    //

    //Евклидова норма
    printf("%g", norm(result));
    //

    for (int i = 0; i < n; ++i) {
        delete[] firstMatrix[i];
        delete[] secondMatrix[i];
        delete[] result[i];
    }
    delete[] firstMatrix;
    delete[] secondMatrix;
    delete[] result;
    return 0;
}