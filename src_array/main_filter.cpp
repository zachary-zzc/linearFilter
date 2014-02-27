#include <iostream>
#include <string>
#include <armadillo>

#include "filter.h"

using namespace std;
using namespace arma;

int main(int argc, char *argv[]){
    cout << "Start to Init Filter, Get Config From: ../cfg.txt" << endl;
    Filter matLinearFilter;
    cout << "Init Filter Success" << endl << endl;

    cout << "Start Read Matrix From File " << matLinearFilter.getInputMatFilePath() << endl;
    mat matrix;
    matLinearFilter.getMatrixFromInputFile(matrix);
    cout << "Gen Original Matrix Success" << endl << endl;

    cout << "Start Build" << endl;
    matLinearFilter.genBuilding(matrix);
    cout << "Build Success" << endl << endl;

    cout << "Start to Trace Back" << endl;
    matLinearFilter.doFilter();
    cout << "Trace Back Success, Check Output in: " << matLinearFilter.getRecordFilePath() << endl;

}
    
    
