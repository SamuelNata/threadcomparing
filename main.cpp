#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <ratio>
#include <chrono>


/**
#include <sstream>
#include <unistd.h>
#include <vector>
#include <math.h>


A1024x1024.txt
A128x128.txt
A16x16.txt
A2048x2048.txt
A256x256.txt
A32x32.txt
A4x4.txt
A512x512.txt
A64x64.txt
A8x8.txt
B1024x1024.txt
B128x128.txt
B16x16.txt
B2048x2048.txt
B256x256.txt
B32x32.txt
B4x4.txt
B512x512.txt
B64x64.txt
B8x8.txt
**/


bool multiplySequentialy( int ** m1, int l1, int c1, int **m2, int l2, int c2, int **&mresult, int & lr, int & cr){
    if( c1!=l2 ){
        std::cout << "So e possivel multiplicar matrizes se o numero de colunas da primeira for igual a o numero e linhas da segunda. ¬¬\"\n";
        return false;
    }

    if(mresult!=0){
        for(int j(0) ; j<cr ; j++){
            delete [] mresult[j];
        }
        delete [] mresult;
    }

    lr = l1;
    cr = c2;
    mresult = new int* [cr];
    for(int i(0) ; i<cr ; i++){
        mresult[i] = new int [lr];
        for(int j(0) ; j<lr ; j++){
            mresult[i][j]=0;
        }
    }

    for(int i(0) ; i<lr ; i++){
        for(int j(0) ; j<cr ; j++){
            for(int k(0) ; k<c1 ; k++){
                mresult[i][j] += m1[i][k]*m2[k][j];
            }
        }
    }

    return true;
}

void readMatrix( std::string fileName , int & numL, int & numC, int **& matrix ){
    if(matrix!=0){
        for(int j(0) ; j<numC ; j++){
            delete [] matrix[j];
        }
        delete [] matrix;
    }

    std::string line;
    std::ifstream file( fileName.c_str() );
    int begin, end;

    if( file.is_open() && getline(file,line) ){
        begin = 0;
        end = line.find(" ");
        numL = atoi( line.substr(begin, end).c_str() );

        begin = end+1;
        end = line.find(" ", begin);
        numC = atoi( line.substr(begin, end).c_str() );

        matrix = new int* [numC];
        for(int i(0) ; i<numC ; i++){
            matrix[i] = new int [numL];
        }

        getline(file,line);
        for( int i(0) ; i<numL ; i++ ){
            begin = 0;
            end = line.find(" ", begin);
            for( int j(0) ; j<numC ; j++ ){
                matrix[i][j] = atoi( line.substr(begin, end).c_str() );
                begin = end+1;
                end = line.find(" ", begin);
            }
            getline(file,line);
        }
        file.close();
    }
    else{
        std::cout << "(" << fileName << ") Esse arquivo existe mesmo? Escreva esse negocio direito homi.\n";
    }
    return;
}

void printMatrix(int **& matrix, int numL, int numC ){
    std::cout << "Matriz " << numL << " por " << numC << ":\n";
    for( int i(0) ; i<numL ; i++ ){
        for( int j(0) ; j<numC ; j++ ){
            std::cout << matrix[i][j] << "\t";
        }
        std::cout << std::endl;
    }
    return;
}

int main()
{
    int ** mat1 = 0;
    int l1, c1;
    readMatrix("Matrizes/A1024x1024.txt", l1, c1, mat1);

    int ** mat2 = 0;
    int l2, c2;
    readMatrix("Matrizes/B1024x1024.txt", l2, c2, mat2);

    int ** matR = 0;
    int lr, cr;

    //printMatrix(mat1, l1, c1);
    //printMatrix(mat2, l2, c2);

    std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();
    //clock_t begin = clock();
    //std::cout << "Begin: " << begin << std::endl;
    //clock_t end;
    multiplySequentialy(mat1, l1, c1, mat2, l2, c2, matR, lr, cr);
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    //eend = clock();
    //std::cout << "End: " << end << std::endl;
    //double solutionTime = (float)(end-begin)/CLOCKS_PER_SEC;
    std::chrono::duration<double> solutionTime = std::chrono::duration_cast<std::chrono::duration<double>>(end-begin);


    std::cout << "Time solution: " << solutionTime.count() << " s.\n";
    //printMatrix(matR,lr, cr);

    return 0;
}
