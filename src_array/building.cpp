#include <iostream>
#include <fstream>
#include <assert.h>
#include <map>

#include "building.h"

#define DEFAULT_RECORD_FILE_PATH        "../record_array.csv"
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
    for (vector<Floor*>::iterator iter = m_pFloors.begin(); iter != m_pFloors.end(); iter ++){
        delete *iter;
    }
    m_pFloors.clear();
}

// ----------------------------------------------
// Function: Recursive call this function in build process
// Input   : Current Floor
//           Reference of next Floor needs to be generated
//           Input matrix
// Output  : State
// ----------------------------------------------
int Building::generateNextFloor(Floor& thisFloor, Floor& nextFloor){
    cout << "Set up Counter..." << endl;
    nextFloor.setCounter(thisFloor, m_matrix, m_iSplitCount, m_iRowCount);
    cout << "Set up Counter Success" << endl << endl;

    cout << "Set up Floor..." << endl;
    nextFloor.setList(thisFloor, m_matrix, m_iSplitCount, m_iRowCount);
    cout << "Set up Floor Success" << endl << endl;

    return 0;
}

int Building::dfs(uint iCellIndex, uint iFloorIndex, vector<uint> &vecColList, ofstream &ofs){
    uint iStartIndex = m_pFloors[iFloorIndex]->getCounter(iCellIndex);
    uint iEndIndex = m_pFloors[iFloorIndex]->getCounter(iCellIndex + 1);
    if (iFloorIndex == 0){
        // this is the last floor, size of lastColListSize() == 0, col index stored in pointer list
        for (uint i = iStartIndex; i != iEndIndex; i ++){
            vecColList.push_back(m_pFloors[iFloorIndex]->getColIndex(i));
            Utils::record<uint>(vecColList, ofs);
            vecColList.pop_back();
        }
    }
    else{
        for (uint i = iStartIndex; i != iEndIndex; i ++){
            vecColList.push_back(m_pFloors[iFloorIndex]->getColIndex(i));
            uint iNextCellIndex = m_pFloors[iFloorIndex]->getCellIndex(i);
            dfs(iNextCellIndex, iFloorIndex-1, vecColList, ofs);
            vecColList.pop_back();
        }
    }
    return 0;
}
        
    
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
    m_pFloors.clear();

    Floor* pFirstFloor = new Floor(m_iSplitCount, m_iRowCount);
    pFirstFloor->initFirstFloor(m_matrix, m_iSplitCount, m_iRowCount);
    m_pFloors.push_back(pFirstFloor);
    pFirstFloor = NULL;

    // set up higher floors
    for (int i = 1; i != m_iHeight; i++){
        cout << "Floor " << i << " Total Cell Number : " << m_pFloors[i-1]->getLength() << endl;
        uint iActivatedCellCount = 0;
            for (uint l = 0; l != m_pFloors[i-1]->getLength(); l ++){
            if (m_pFloors[i-1]->checkActivated(l)){
                iActivatedCellCount ++;
            }
        }
        cout << "Floor " << i << " Activated Cell Number : " << iActivatedCellCount << endl;
        cout << endl;

        cout << "Set up floor " << i+1 << "..." << endl; 
        Floor* pFloor = new Floor(m_iSplitCount, m_iRowCount);
        m_pFloors.push_back(pFloor);
        generateNextFloor(*m_pFloors[i-1], *m_pFloors[i]);
        pFloor = NULL;
    } 
    return 0;
}

int Building::traceBack(){
    ofstream ofs(m_strRecordFilePath.c_str());
    if (ofs.fail()){
        cerr << "Open output file error " << endl;
        exit(EXIT_FAILURE);
    }

    // don't need to check the first floor
    for (uint iCurFloor = m_iHeight-1; iCurFloor != 0; iCurFloor --){
        cout << "Checking Floor " << iCurFloor+1 << "..." << endl;
        uint iCurCellIndex = 0;
        for (uint i = 0; i != m_pFloors[iCurFloor]->getLength(); i ++ ){
            if (m_pFloors[iCurFloor]->checkActivated(i) && m_pFloors[iCurFloor]->checkLinear(i, m_iSplitCount, m_iRowCount, m_dThresheld)){
                cout << i << " is LINEAR and ACTIVATED, start trace back from this cell..." << endl;
                vec printVec(m_iRowCount);
                Utils::indexToVector(i, printVec, m_iSplitCount, m_iRowCount);
                printVec.print("Linear Vector is : ");
                cout << "Linear under thresheld : " << m_dThresheld << endl; 

                // this cell is activated and linear, trace back start from this cell
                vector<uint> vecColList;
                uint iStartFloor = iCurFloor;
                dfs(i, iStartFloor, vecColList, ofs);
            }
        }
    }
    ofs.close();
    return 0;
}
