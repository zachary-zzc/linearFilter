#include <iostream>
#include <fstream>
#include <assert.h>

// for quick read file in Linux/Unix system(mmap())
#include <sys/mman.h> 
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include "filter.h"

#define DEFAULT_INPUT_MATRIX_FILE_PATH      "../input.txt"
#define DEFAULT_CFGFILE_PATH                "../cfg.txt"
#define DEFAULT_RECORD_FILE_PATH            "../record.csv"

#define MAX_MATRIX_ROW                      5000
#define MAX_MATRIX_COL                      10000
// -----------------------------------------------------------
// Constructor
// -----------------------------------------------------------
Filter::Filter():
    m_strMatFilePath(DEFAULT_INPUT_MATRIX_FILE_PATH),
    m_strCfgFilePath(DEFAULT_CFGFILE_PATH),
    m_strRecordFilePath(DEFAULT_RECORD_FILE_PATH){

    init();
}

Filter::Filter(string strCfgFilePath):
    m_strCfgFilePath(strCfgFilePath){

    init();
}

Filter::~Filter(){
}

void Filter::setVar(string strVarName, string strVarValue){
    if (strVarName == "height")
        m_bStorage.setHeight(Utils::strToInt(strVarValue));
    else if (strVarName == "split count")
        m_bStorage.setSplitCount(Utils::strToInt(strVarValue));
    else if (strVarName == "split interval")
        m_bStorage.setSplitCount(static_cast<int>(2 / Utils::strToDouble(strVarValue)+0.5) + 1);
    else if (strVarName == "row count")
        m_bStorage.setRowCount(Utils::strToInt(strVarValue));
    else if (strVarName == "record path"){
        m_strRecordFilePath = strVarValue;
        m_bStorage.setRecordFilePath(strVarValue);
    }
    else if (strVarName == "thresheld")
        m_bStorage.setThresheld(Utils::strToDouble(strVarValue));
    else if (strVarName == "matrix path")
        m_strMatFilePath == strVarValue;
    else if (strVarName == "cfg path")
        m_strCfgFilePath == strVarValue;
    else
        cerr << "No such param " << "\"" << strVarName << "\"" << endl;
}

void Filter::setCfgFilePath(string strCfgFilePath){
    m_strCfgFilePath = strCfgFilePath;
}

string Filter::getCfgFilePath(){
    return m_strCfgFilePath;
}

string Filter::getInputMatFilePath(){
    return m_strMatFilePath;
}

string Filter::getRecordFilePath(){
    return m_strRecordFilePath;
}

int Filter::init(){
    m_bStorage = Building();

    ifstream ifs(m_strCfgFilePath.c_str());
    if (ifs.fail()){
        cerr << "Open config file fail" << endl;
        exit(1);
    }
    char line[1024] = {'\0'};
    while(ifs.getline(line, 1024)){
        string strLine(line);
        vector<string> vecStr;
        Utils::parseLine(strLine, ":", vecStr);
        // should contain 2 elements in each line of cfg file
        assert(vecStr.size() == 2);
        setVar(vecStr[0], vecStr[1]);
    }
    ifs.close();

    cout << "DISCRETE SPLIT INTERVAL : " << 2 / (double)(m_bStorage.getSplitCount()-1) << endl;
    cout << "DISCRETE COUNT : " << m_bStorage.getSplitCount() << endl;
    cout << "MATRIX ROW COUNT : " << m_bStorage.getRowCount() << endl;
    cout << "RECURSIVE HEIGHT : " << m_bStorage.getHeight() << endl;
    cout << "LINEAR THRESHELD : " << m_bStorage.getThresheld() << endl;

    return 0;
}

int Filter::genBuilding(mat matrix){
    m_bStorage.initBuilding(matrix);
    m_bStorage.build();
    return 0;
}

int Filter::doFilter(){
    // m_bStorage.traceBack();
    return 0;
}

int Filter::getMatrixFromInputFile(mat &matrix){
    cout << "Max Input Matrix Scale: " << MAX_MATRIX_ROW << " * " << MAX_MATRIX_COL << endl;

    int fd;
    char *mbuf;
    struct stat statbuf;
    if ((fd = open(m_strMatFilePath.c_str(), O_RDONLY)) < 0){
        cerr << "Can't Open " << m_strMatFilePath << endl;
        exit(1);
    }
    if (fstat(fd, &statbuf) < 0){
        cerr << "Fstat Error" << endl;
        exit(1);
    }
    if ((mbuf = (char*)mmap(NULL, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0)) == (caddr_t)-1){
        cerr << "mmap error" << endl;
        exit(1);
    }
    string str(mbuf);
    vector<string> vecLines;
    Utils::parseLine(str, "\n", vecLines);
    
    int iRowNumber, iColNumber;
    vector<string> vecElems;
    Utils::parseLine(vecLines[0], "\t", vecElems);
    iRowNumber = vecLines.size();
    iColNumber = vecElems.size();
    matrix = zeros<mat>(iRowNumber, iColNumber);
    
    int iRowCount = 0;
    for (vector<string>::iterator iter = vecLines.begin() + 1; iter != vecLines.end(); iter ++, iRowCount ++){
        Utils::parseLine(*iter, "\t", vecElems);
        int iColCount = 0;
        for (vector<string>::iterator iterElem = vecElems.begin(); iterElem != vecElems.end(); iterElem ++, iColCount ++){
            matrix(iRowCount, iColCount) = Utils::strToDouble(*iterElem);
        }
    }
    cout << "Get Initial Matrix of " << matrix.n_rows << " * " << matrix.n_cols << endl;
}



