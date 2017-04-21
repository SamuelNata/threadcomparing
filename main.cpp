#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <ratio>
#include <chrono>
#include <thread>
#include <vector>

#define AUTORUN true
#define TESTCASEFOLDER "Matrizes/"
#define RESULTMATRIXFOLDER "resultMatrix/"
#define SOLUTIONTIMEFOLDER "solutionTimes/"
#define READINGFILETIMEFOLDER "readingFileTime/"
#define PRINTINGMATRIXTIMEFOLDER "printingMatrixTime/"
#define ALLTIMES "allTimes.dat"

char filesA[10][15] = { "A4x4.txt", "A8x8.txt", "A16x16.txt", "A32x32.txt", "A64x64.txt", "A128x128.txt", "A256x256.txt", "A512x512.txt", "A1024x1024.txt", "A2048x2048.txt"};
char filesB[10][15] = { "B4x4.txt", "B8x8.txt", "B16x16.txt", "B32x32.txt", "B64x64.txt", "B128x128.txt", "B256x256.txt", "B512x512.txt", "B1024x1024.txt", "B2048x2048.txt"};

double sizePerNumThreadsTime[12][10];

void initializeTimeTable(){
	for( int expNumThre(0) ; expNumThre<12 ; expNumThre++ ){
		for( int N(0) ; N<10 ; N++ ){
			sizePerNumThreadsTime[expNumThre][N] = -1;
		}
	}
	return;
}

int pow(int b, int e){
	int r=1;
	for(int i(0) ; i<e ; i++ ){
		r*=b;
	}
	return r;
}

void clearAllTimesFiles(){
	std::string filename;
	std::ofstream file;
	for( int c(0) ; c<3 ; c++ ){
		switch(c){
			case(0):
				filename = SOLUTIONTIMEFOLDER;
				break;
			case(1):
				filename = READINGFILETIMEFOLDER;
				break;
			case(2):
				filename = PRINTINGMATRIXTIMEFOLDER;
				break;
		}
		file.open((filename+ALLTIMES).c_str());
		file << "\n";
		file.close();
	}
	return;
}

void vectorMultiply(int **& m1, int l1, int c1, int **& m2, int l2, int c2, int **& mr, int targetLine, int numThreads);

bool multiplySequentialy( int ** m1, int l1, int c1, int **m2, int l2, int c2, int **&mresult, int & lr, int & cr){
    if( c1!=l2 ){
        std::cout << "So e possivel multiplicar matrizes se o numero de colunas da primeira for igual a o numero e linhas da segunda. ¬¬\"\n";
        return false;
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

bool multiplyConcurrently(int ** m1, int l1, int c1, int **m2, int l2, int c2, int **& mresult, int & lr, int &cr, int numThreads){
	if( c1!=l2){
		std::cout << "não é possivel multiplicar estas matrizes.\n";
		return false;
	}
	if( numThreads>c2 || numThreads<1 ){
		std::cout << "O número de threads deve estar entre 1 e o numero de colunas da segunda matriz.";
		return false;
	}
	
	std::thread tVet[numThreads];
	
	for( int t(0) ; t<numThreads ; t++ ){
		tVet[t] = std::thread(vectorMultiply, std::ref(m1), l1, c1, std::ref(m2), l2, c2, std::ref(mresult), t, numThreads);
	}

	for( int t(0) ; t<numThreads ; t++ ){
		tVet[t].join();	
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
    std::string folder = TESTCASEFOLDER;
    fileName = folder+fileName;
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

void alocateResultMatrix(int l1, int c1, int l2, int c2, int **& mr, int & lr, int & cr){
	if(c1!=l2){
		std::cout << "Para multiplicar matrizes é necessário que o numero de colunas da primeira seja igual o numero de linhas da segunda.\n";
	}
	
	if(mr!=0){
		for( int i(0) ; i<cr ; i++ ){
			delete [] mr[i];
		}
		delete mr;
	}

	lr=l1;
	cr=c2;

	mr = new int*[c2];
	for(int i(0) ; i<c2 ; i++){
		mr[i] = new int[l1];
		for( int j(0) ; j<l1 ; j++ ){
			mr[i][j] = 0;
		}
	}
	return;
}

void vectorMultiply(int **& m1, int l1, int c1, int **& m2, int l2, int c2, int **& mr, int targetLine, int numThreads){
	int atualL = targetLine;
	while(atualL<l1){
		for(int atualC(0) ; atualC<c2 ; atualC++){
			for( int k(0) ; k<l1 ; k++ ){
				mr[atualL][atualC] += m1[atualL][k]*m2[k][atualC];
			}
		}
		atualL += numThreads;
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

void printResultMatrixToFile( std::string file1, std::string file2, int ** mr, int lr, int cr){
	std::string folder = RESULTMATRIXFOLDER;
	std::string name = folder+file1.substr(0, file1.find("."))+"_"+file2.substr(0, file2.find("."))+"_Matrix.dat";
    std::ofstream resultMatrixFile;
    resultMatrixFile.open( name.c_str() );
    resultMatrixFile << lr << " " << cr << "\n";
    for( int i(0) ; i<lr ; i++ ){
        for( int j(0) ; j<cr ; j++ ){
            resultMatrixFile << mr[i][j] << " ";
        }
        resultMatrixFile << "\n";
    }
    resultMatrixFile.close();
    return;
}

void printToBanchMarkSolutionTimeFile( std::string file1, std::string file2, double solutionTimes[]){
	std::string folder = SOLUTIONTIMEFOLDER;
	std::string name = folder+file1.substr(0, file1.find("."))+"_"+file2.substr(0, file2.find("."))+"_SolutionTimes.dat";
	std::ofstream outFile;
	//std::ofstream allTimesFile;
	outFile.open( name.c_str() );
	//allTimesFile.open( (std::string(SOLUTIONTIMEFOLDER)+ALLTIMES).c_str(), std::ios_base::app );
	for( int i(0) ; i<12 ; i++ ){
		if(solutionTimes[i]<0){	break; }
		outFile << (pow(2,i)) << "\t" << solutionTimes[i] << "\n";
		//allTimesFile << (pow(2,i)) << "\t" << solutionTimes[i] << "\n";

	}
	outFile.close();
	return;
}

void printTableSolutionTimeFile(){
	std::string folder = SOLUTIONTIMEFOLDER;
	std::ofstream allTimesFile;
	allTimesFile.open( (std::string(SOLUTIONTIMEFOLDER)+ALLTIMES).c_str(), std::ios_base::app );

	for( int expNumThre(0) ; expNumThre<12 ; expNumThre++ ){
		for( int N(0) ; N<10 ; N++ ){
			allTimesFile << sizePerNumThreadsTime[expNumThre][N] << "\t";
		}
		allTimesFile << std::endl;
	}
	allTimesFile.close();
	return;
}

void printToBenchMarkReadFile(std::string file1, std::string file2, double ReadTime ){
	std::string folder = READINGFILETIMEFOLDER;
	std::string name = folder+file1.substr(0, file1.find("."))+"_"+file2.substr(0, file2.find("."))+"_ReadTime.dat";
	std::ofstream outFile;
	std::ofstream allTimesFile;
	outFile.open( name.c_str() );
	allTimesFile.open( (std::string(READINGFILETIMEFOLDER)+ALLTIMES).c_str(), std::ios_base::app );
	outFile << ReadTime << "\n";
	allTimesFile << ReadTime << "\n";
	outFile.close();
	allTimesFile.close();
	return;
}

void printToBenchMarkPrintTimeFile(std::string file1, std::string file2, double printTime ){
	std::string folder = PRINTINGMATRIXTIMEFOLDER;
	std::string name = folder+file1.substr(0, file1.find("."))+"_"+file2.substr(0, file2.find("."))+"_PrintTime.dat";
	std::ofstream outFile;
	std::ofstream allTimesFile;
	outFile.open( name.c_str() );
	allTimesFile.open( (std::string(PRINTINGMATRIXTIMEFOLDER)+ALLTIMES).c_str(), std::ios_base::app );
	outFile << printTime << "\n";
	allTimesFile << printTime << "\n";
	allTimesFile.close();
	outFile.close();
	return;
}

int main()
{
    int ** mat1 = 0;
    int l1, c1;

    int ** mat2 = 0;
    int l2, c2;

    int ** matR = 0;
    int lr, cr;
    
    std::string filename1;
    std::string filename2;
    int numThreads;

    std::chrono::duration<double> readFilesDuration;
    std::chrono::duration<double> solutionDuration[12]; //the number of threads used is 2^index, where index 0 is the sequencial case (without using threads).
    std::chrono::duration<double> concurrentSolutionDuration;
    std::chrono::duration<double> printingDuration;

    double solutionTime[12];

    std::chrono::high_resolution_clock::time_point readBeginTime;
    std::chrono::high_resolution_clock::time_point sequentialBeginTime;
    std::chrono::high_resolution_clock::time_point concurrentBeginTime;
    std::chrono::high_resolution_clock::time_point printingBeginTime;


    if(!AUTORUN){
	    std::cout << "Insira o nome do arquivo da primeira matriz: ";
	    getline(std::cin, filename1);
	    std::cout << "Insira o nome do arquivo da segunda matriz: ";
	    getline(std::cin, filename2);
	    std::cin.clear();
	    std::cout << "Insira o numero de threads: ";
	    std::cin >> numThreads;
	    while( !filename1.empty() && !filename2.empty() ){
	    	readBeginTime = std::chrono::high_resolution_clock::now();
	    	readMatrix(filename1.c_str(), l1, c1, mat1);
	    	readMatrix(filename2.c_str(), l2, c2, mat2);
	    	readFilesDuration = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now()-readBeginTime);
	    	std::cout << "read files duration: " << readFilesDuration.count() << " s\n";

	    	alocateResultMatrix(l1, c1, l2, c2, matR, lr, cr);

	    	sequentialBeginTime = std::chrono::high_resolution_clock::now();
	    	multiplySequentialy(mat1, l1, c1, mat2, l2, c2, matR, lr, cr);
	    	std::cout << "sequential solution duration: " << (std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now()-sequentialBeginTime).count()) << " s\n";

			alocateResultMatrix(l1, c1, l2, c2, matR, lr, cr);

	    	concurrentBeginTime = std::chrono::high_resolution_clock::now();
	    	multiplySequentialy(mat1, l1, c1, mat2, l2, c2, matR, lr, cr);
	    	std::cout << "concurrent solution duration: " << (std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now()-concurrentBeginTime).count()) << " s\n";

	    	printingBeginTime = std::chrono::high_resolution_clock::now();
	    	printMatrix(matR,lr, cr);
	    	printingDuration = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now()-printingBeginTime);    	
	    	std::cout << "printing duration: " << printingDuration.count() << " s\n\n\n";
			
	    	std::cin.ignore(1);
			std::cout << "Insira o nome do arquivo da primeira matriz: ";
		    getline(std::cin, filename1);
		    std::cin.clear();
		    std::cout << "Insira o nome do arquivo da segunda matriz: ";
		    getline(std::cin, filename2);
		    std::cin.clear();
		    std::cout << "Insira o numero de threads: ";
		    std::cin >> numThreads;
		    std::cin.clear();
	    }
	    return 0;
	}
	else{
		clearAllTimesFiles(); // LIMPA OS ARQUIVOS QUE GUARDAM O TEMPO PARA TODAS AS EXECUÇÕES EM CADA MODALIDADE (LEITURA, SOLUÇÃO E ESCRITA)
		initializeTimeTable(); // INICIALIZA A TABELA DE TEMPOS QUE SERÁ IMPRESSA EM ARQUIVO NO FINAL
		for( int f(0) ; f<10 ; f++ ){
			//LENDO ARQUIVOS
			std::cout << "---------------------------------------\n";
			std::cout << "Multiplicando " << filesA[f] << " por " << filesB[f] << " (" << f << " de 10)\n";
			std::cout << "readFiles\n";
	    	readBeginTime = std::chrono::high_resolution_clock::now();
	    	readMatrix(filesA[f], l1, c1, mat1);
	    	readMatrix(filesB[f], l2, c2, mat2);
	    	readFilesDuration = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now()-readBeginTime);
	    	//SALVANDO BENCHMARK DE LEITURA
	    	printToBenchMarkReadFile(filesA[f], filesB[f], readFilesDuration.count());
	    	alocateResultMatrix(l1, c1, l2, c2, matR, lr, cr);

	    	for(int p(0) ; p<12 ; p++) { solutionTime[p] = -1;	} //clear duration time

	    	std::cout << "sequential Solution\n";
	    	//SOLUÇÃO SEQUENCIAL
	    	sequentialBeginTime = std::chrono::high_resolution_clock::now();
	    	multiplySequentialy(mat1, l1, c1, mat2, l2, c2, matR, lr, cr);
	    	solutionDuration[0] = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now()-sequentialBeginTime);
	    	solutionTime[0] = solutionDuration[0].count();
	    	sizePerNumThreadsTime[0][f] = solutionDuration[0].count();

	    	std::cout << "concurrentSolution: 2^i threads, com i = ";
			for( int i=1 ; i<=f+2 ; i++ ){
				alocateResultMatrix(l1, c1, l2, c2, matR, lr, cr);
				std::cout << i << ", ";
				//SOLUÇÃO CONCORRENTE
	    		concurrentBeginTime = std::chrono::high_resolution_clock::now();
	    		multiplyConcurrently(mat1, l1, c1, mat2, l2, c2, matR, lr, cr, pow(2,i));
	    		solutionDuration[i] = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now()-concurrentBeginTime);
	    		solutionTime[i] = solutionDuration[i].count();
	    		sizePerNumThreadsTime[i][f] = solutionDuration[i].count();
	    	}
	    	std::cout << std::endl;
	    	//SALVANDO BENCHMARK DE SOLUÇÃO
	    	printToBanchMarkSolutionTimeFile(filesA[f], filesB[f], solutionTime);

	    	std::cout << "printing time\n";
	    	//IMPRIMINDO
	    	printingBeginTime = std::chrono::high_resolution_clock::now();
	    	printResultMatrixToFile(filesA[f], filesB[f], matR,lr, cr);
	    	printingDuration = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now()-printingBeginTime);    	
	    	//SALVANDO BENCHMARK DE ESCRITA
	    	printToBenchMarkPrintTimeFile(filesA[f], filesB[f], printingDuration.count());
	    }
	    printTableSolutionTimeFile();
		return 0;
	}
	return 0;
}


