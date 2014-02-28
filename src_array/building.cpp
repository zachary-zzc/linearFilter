#include <iostream>
#include <fstream>
#include <assert.h>
#include <map>

#include "building.h"

#define DEFAULT_RECORD_FILE_PATH        "../record.csv"
#define DEFAULT_RECURSIVE_HEIGHT        2
#define DEFAULT_THRESHELD               0
#define DEFAULT_ROW_COUNT               2
#define DEFAULT_DISCRETE_SPLIT_COUNT    100
// -----------------------------------------------
// Constructor
// -----------------------------------------------
Building::Building():
    m_iHeight(DEFAULT_RECURSIVE_HEIGHT),
    m_strRecordFilePath(DEFAULT_RECORD_FILE_PATH),
    m_iRowCount(DEFAULT_ROW_COUNT),
    m_iSplitCount(DEFAULT_DISCRETE_SPLIT_COUNT),
    m_dThresheld(DEFAULT_THRESHELD){
}


Building::~Building(){
}

// ----------------------------------------------
// Function: Recursive call this function in build process
// Input   : Current Floor
//           Reference of next Floor needs to be generated
//           Input matrix
// Output  : State
// ----------------------------------------------
int Building::generateNextFloor(Floor thisFloor, Floor &nextFloor){
    cout << "Set up Counter..." << endl;
    nextFloor.setCounter(thisFloor, m_matrix, m_iSplitCount, m_iRowCount);
    cout << "Set up Counter Success" << endl << endl;

    cout << "Set up Floor..." << endl;
    nextFloor.setList(thisFloor, m_matrix, m_iSplitCount, m_iRowCount);
    cout << "Set up Floor Success" << endl << endl;

    return 0;
}

/*int Building::dfs(Cell* pCurCell, int iFloorIndex, vector<int> &vecColList, ofstream &ofs){
    if (pCurCell->lastColListSize() == 0 || iFloorIndex == 0){
        // this is the last floor, size of lastColListSize() == 0, col index stored in pointer list
        for (int i = 0; i < pCurCell->lastCellListSize(); i ++){
            vecColList.push_back(pCurCell->getLastCellIndex(i));
            Utils::record<int>(vecColList, ofs);
            vecColList.pop_back();
        }
    }
    else{
        for (int i = 0; i < pCurCell->lastCellListSize(); i ++){
            vector<int> vecColIndex;
            pCurCell->getLastColIndex(i, vecColIndex);
            for (vector<int>::iterator iter = vecColIndex.begin(); iter != vecColIndex.end(); iter ++){
                vecColList.push_back(*iter);
                Cell* pNextCell = getCell(iFloorIndex-1, pCurCell->getLastCellIndex(i));
                dfs(pNextCell, iFloorIndex-1, vecColList, ofs);
                vecColList.pop_back();
            }
        }
    }
    return 0;
}*/
        
    
void Building::setRecordFilePath(string strRecordPath){
    m_strRecordFilePath = strRecordPath;
}

void Building::setHeight(int iHeight){
    m_iHeight = iHeight;
}

void Building::setThresheld(double dThresheld){
    m_dThresheld = dThresheld;
}

void Building::setSplitCount(uint iSplitCount){
    m_iSplitCount = iSplitCount;
}

void Building::setRowCount(uint iRowCount){
    m_iRowCount = iRowCount;
}

uint Building::getSplitCount(){
    return m_iSplitCount;
}

uint Building::getRowCount(){
    return m_iRowCount;
}

int Building::getHeight(){
    return m_iHeight;
}

double Building::getThresheld(){
    return m_dThresheld;
}

int Building::initBuilding(mat matrix){
    m_matrix = zeros<mat>(m_iRowCount, matrix.n_cols);
    cout << "Mat Deduce to " << m_iRowCount << " Row Numbers..." << endl;
    Utils::matDeduce(matrix, m_matrix, m_iRowCount);

    cout << "Matrix Scaling..." << endl;
    Utils::matScaling(m_matrix);

    cout << "Current Matrix is " << m_matrix.n_rows << " * " << m_matrix.n_cols << endl;
    cout << "Max: " << max(max(m_matrix)) << endl;
    cout << "Min: " << min(min(m_matrix)) << endl;

    return 0;
}

int Building::build(){
    // set up first floor
    cout << "Set up floor 1..." << endl;
    m_vecFloors.clear();
    for (int i = 0; i != m_iHeight; i++){
        m_vecFloors.push_back(Floor(m_iSplitCount, m_iRowCount));
    }
    // m_vecFloors[0].initFirstFloor(m_matrix, m_iSplitCount, m_iRowCount);

    // set up higher floors
    for (int i = 1; i != m_iHeight; i++){
        cout << "Floor " << i << " Activated Cells are : " << endl;
        uint iActivatedCellCount = 0;
        for (uint l = 0; l != m_vecFloors[i-1].getLength(); l ++){
            if (m_vecFloors[i-1].checkActivated(l)){
                cout << l << " ";
                iActivatedCellCount ++;
            }
        }
        cout << endl << "Floor " << i << " Activated Cell Number : " << iActivatedCellCount << endl;
        cout << endl;

        cout << "Set up floor " << i+1 << "..." << endl; 
        generateNextFloor(m_vecFloors[i-1], m_vecFloors[i]);
    }
    cout << "build func finish" << endl;
    return 0;
}

int Building::traceBack(){
    /*ofstream ofs(m_strRecordFilePath.c_str());
    if (ofs.fail()){
        cerr << "Open output file error " << endl;
        exit(EXIT_FAILURE);
    }

    // don't need to check the first floor
    for (int iCurFloor = m_iHeight-1; iCurFloor > 0; iCurFloor --){
        cout << "Checking Floor " << iCurFloor+1 << "..." << endl;
        int iCurCellIndex = 0;
        for (vector<Cell>::iterator iter = m_vecFloors[iCurFloor].vecCells.begin(); iter != m_vecFloors[iCurFloor].vecCells.end(); iter ++, iCurCellIndex ++ ){
            if (iter->checkActivated() && iter->checkLinear(iCurCellIndex, m_iSplitCount, m_iRowCount, m_dThresheld)){
                cout << iCurCellIndex << " is LINEAR and ACTIVATED, start trace back from this cell..." << endl;
                vec printVec(m_iRowCount);
                Cell::indexToVector(iCurCellIndex, printVec, m_iSplitCount, m_iRowCount);
                printVec.print("Linear Vector is : ");
                cout << "Linear under thresheld : " << m_dThresheld << endl; 
                // this cell is activated and linear, trace back start from this cell
                vector<int> vecColList;
                dfs(&(*iter), iCurFloor, vecColList, ofs);
            }
        }
    }
    ofs.close();*/
    return 0;
}
