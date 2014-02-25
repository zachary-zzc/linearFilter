#include <iostream>
#include <algorithm>

#include "cell.h"

// -------------------------------------------
// Constructor
// -------------------------------------------
Cell::Cell():
    m_bActivated(false){
    m_vecMatrixColIndexList.clear();
    m_vecPointerList.clear();
}

Cell::~Cell(){
}


// -------------------------------------------
// Function: Convert from cell index to cell vector
// Input   : Cell index
//           Reference of cell vector
//           Split count of the cell's floor
//           Row count of the cell's floor
// Output  : None
// -------------------------------------------
void Cell::indexToVector(int iIndex, vec &vecCells, int iSplitCount, int iRowCount){
    vecCells.fill(-1);
    for (int i = 0; i < iRowCount && iIndex > 0; i++){
        double tmp = (double)(iIndex%(iSplitCount-1));
        tmp = (tmp/(iSplitCount - 1) - 0.5) * 2;
        iIndex = iIndex / (iSplitCount-1);
        try{
            vecCells(i) = tmp;
        }catch (int e){
            cerr << "Out of Vector Index Error" << e << endl;
        }
    }
}

// -------------------------------------------
// Function: Convert from cell vector to cell index
// Input   : Reference of cell index
//           Cell vector
//           Split count of the cell's floor
//           Row count of the cell's floor
// Output  : None
// -------------------------------------------
void Cell::vectorToIndex(int &iIndex, vec cellVec, int iSplitCount, int iRowCount){
    vec convVec = zeros<vec>(iRowCount);
    for (int i = iRowCount-1; i >= 0; i --){
        convVec(i) = pow(iSplitCount, i);
    }
    cellVec = (cellVec / 2 + 0.5) * (iSplitCount - 1);
    for (vec::iterator iter = cellVec.begin(); iter != cellVec.end(); iter ++){
        *iter = (double)(static_cast<int>((*iter) + 0.5));
    }
    iIndex = static_cast<int>(dot(convVec, cellVec) + 0.5);
}

bool Cell::checkActivated(){
    return m_bActivated;
}

// -------------------------------------------
// Function: Check if the cell is linear according to the dThresheld
// Input   : Split count of the cell's floor
//           Row count of the cell's floor
//           check dThresheld
// Output  : Linear or not
// -------------------------------------------
bool Cell::checkLinear(int iMyIndex, int iSplitCount, int iRowCount, double dThresheld){
    // raw number should be larger than 0
    assert(iRowCount > 0);

    vec cellVec = zeros<vec>(iRowCount);
    indexToVector(iMyIndex, cellVec, iSplitCount, iRowCount);
    double max = -1;
    double min = 1;
    for (vec::iterator iter = cellVec.begin(); iter != cellVec.end(); iter ++){
        max = max > *iter ? max : *iter;
        min = min < *iter ? min : *iter;
    }
    return ((max - min) <= dThresheld);
}

int Cell::lastCellListSize(){
    return m_vecPointerList.size();
}

int Cell::lastColListSize(){
    return m_vecMatrixColIndexList.size();
}

int Cell::getLastCellIndex(int iIndex){
    return m_vecPointerList[iIndex];
}

void Cell::getLastColIndex(int iIndex, vector<int> &vecColIndex){
    vecColIndex =  m_vecMatrixColIndexList[iIndex];
}

void Cell::activate(){
    if (!m_bActivated)
        m_bActivated = true;
}

void Cell::addMatrixColIndexVec(vector<int> vecColIndex){
    m_vecMatrixColIndexList.push_back(vecColIndex);
}

void Cell::addPointerIndex(int iPointerIndex){
    m_vecPointerList.push_back(iPointerIndex);
}

void Cell::addIndex(int iCellIndex, int iColIndex){
    vector<int>::iterator iterCell, iterCol;
    iterCell = find(m_vecPointerList.begin(), m_vecPointerList.end(), iCellIndex);
    if (iterCell != m_vecPointerList.end()){
        // cell index already in pointer list, no need to add again
        int iColIndex = iterCell - m_vecPointerList.begin();
        iterCol = find(m_vecMatrixColIndexList[iColIndex].begin(), m_vecMatrixColIndexList[iColIndex].end(), iColIndex);
        if (iterCol != m_vecMatrixColIndexList[iColIndex].end()){
            // col index already in col list, no need to add again
        }
        else{
            m_vecMatrixColIndexList[iColIndex].push_back(iColIndex);
        }
    }
    else{
        // new cell index
        m_vecPointerList.push_back(iCellIndex);
        vector<int> newColVec;
        newColVec.push_back(iColIndex);
        m_vecMatrixColIndexList.push_back(newColVec);
    }
}


// -------------------------------------------
// Function: Get the next cell's index after linear operating
// Input   : dParam1 multiple this cell's vector
//           dParam2 multiple the incoming vecElem vector
//           Split count of the cell's floor
//           Row count of the cell's floor
//           Incoming vecElem vector
// Output  : Linear or not
// -------------------------------------------
int Cell::getNextCellIndex(int iMyIndex, double dParam1, double dParam2, int iSplitCount, int iRowCount, vec vecElem){
    int iResultIndex;
    vec cellVec = zeros<vec>(iRowCount);
    indexToVector(iMyIndex, cellVec, iSplitCount, iRowCount);
    
    vec vecResult = (cellVec * dParam1 + vecElem * dParam2) / 2;
    vectorToIndex(iResultIndex, vecResult, iSplitCount, iRowCount);

    return iResultIndex;
} 
