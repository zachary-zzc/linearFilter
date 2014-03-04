#include <algorithm>
#include <functional>

#include "tools.h"

// ------------------------------------------------------
// Constructor
// ------------------------------------------------------
Utils::Utils(){
}

Utils::~Utils(){
}

// ------------------------------------------------------
// Function: Standarlize a vector to a range from -1 to 1
// Input   : Vector needs to be standalized
// Output  : None
// ------------------------------------------------------
/*void Utils::vecScaling(vec &vector){
    double dMean = mean(vector);
    double dRange = max(vector) - min(vector);
    vector = 2 * (vector - dMean) / dRange;
}*/

void Utils::matScaling(mat &matrix){ // matrix scaling according to cols
    double dMin, dRange;
    dMin = min(min(matrix));
    dRange = max(max(matrix)) - dMin;
    matrix = ((matrix - dMin) / dRange - 0.5) * 2;
}

/*template<typename T>
void Utils::record(vector<T> s, ofstream &ofs){
    for (typename vector<T>::iterator iter = s.begin(); iter != s.end(); iter ++){
        ofs << *iter << " ";
    }
    ofs << endl;
}*/

// ------------------------------------------------------
// Function: Deduce a n-row matrix to a m-row matrix (m < n)
// Input   : Original Matrix
//           Deduced Matrix
//           Deduced Matrix's row number
// Output  : None
// ------------------------------------------------------
void Utils::matDeduce(mat matrix, mat &deMatrix, int iRowCount){
    int iRowInterval = matrix.n_rows % iRowCount==0? (matrix.n_rows / iRowCount): (matrix.n_rows / iRowCount + 1);
    deMatrix = zeros<mat>(iRowCount, matrix.n_cols);
    for (int i = 0; i < iRowCount; i ++){
        rowvec tmpRow = zeros<rowvec>(matrix.n_cols);
        for (int j = i; j < iRowInterval + i && j < matrix.n_rows; j ++){
            tmpRow += matrix.row(j);
        }
        deMatrix.row(i) = tmpRow;
    }
}
   

void Utils::parseLine(string strLine, string deLimiter, vector<string> &vecString){
    size_t pos = 0;
    string tmp;
    vecString.clear();
    // cout << "pos = " << strLine.find("\n") << endl;
    while ((pos = strLine.find(deLimiter)) != string::npos){
        tmp = strLine.substr(0, pos);
        vecString.push_back(trim(tmp));
        strLine.erase(0, pos + deLimiter.length());
    }
    vecString.push_back(trim(strLine));
}

string& Utils::lTrim(string &ss){
    string::iterator iter = find_if(ss.begin(), ss.end(), not1(ptr_fun<int, int>(isspace)));
    ss.erase(ss.begin(), iter);
    return ss;
}

string& Utils::rTrim(string &ss){
    string::reverse_iterator iter = find_if(ss.rbegin(), ss.rend(), not1(ptr_fun<int, int>(isspace)));
    ss.erase(iter.base(), ss.end());
    return ss;
}

string& Utils::trim(string &ss){
    lTrim(rTrim(ss));
    return ss;
}

int Utils::strToInt(string ss){
    return atoi(ss.c_str());
}

double Utils::strToDouble(string ss){
    return atof(ss.c_str());
}



void Utils::indexToVector(uint iIndex, vec &vecCell, uint iSplitCount, uint iRowCount){
    vecCell.fill(-1);
    for (uint i = 0; i != iRowCount && iIndex > 0; i ++){
        double tmp = (double)(iIndex % (iSplitCount - 1));
        tmp = (tmp / (iSplitCount - 1) - 0.5) * 2;
        iIndex = iIndex / (iSplitCount - 1);
        try{
            vecCell(i) = tmp;
        }catch (int e){
            cerr << "Out of Vector Index Error" << e << endl;
        }
    }
}

void Utils::vectorToIndex(uint &iIndex, vec vecCell, uint iSplitCount, uint iRowCount){
    vec convVec = zeros<vec>(iRowCount);
    for (int i = iRowCount - 1; i >= 0; i --){
        convVec(i) = pow(iSplitCount, i);
    }
    vecCell = (vecCell / 2 + 0.5) * (iSplitCount - 1);
    for (vec::iterator iter = vecCell.begin(); iter != vecCell.end(); iter ++){
        *iter = (double)(static_cast<uint>((*iter)+0.5));
    }
    iIndex = static_cast<uint>(dot(convVec, vecCell) + 0.5);
}
