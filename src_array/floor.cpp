#include <iostream>
#include <fstream>
#include <algorithm>

#include "floor.h"

// -------------------------------------------
// Constructor
// -------------------------------------------
Floor::Floor(const uint& iSplitCount, const uint& iRowCount):
    m_pCellPointerList(NULL),
    m_pColIndexList(NULL){

    m_iLength = pow(iSplitCount, iRowCount);
    m_pCounter = new uint[(int)m_iLength];
    m_pOffsetCounter = new uint[(int)(pow(iSplitCount, iRowCount))];
    
    assert(m_pCounter != NULL && m_pOffsetCounter != NULL);

    for (uint i = 0; i != m_iLength; i++){
        *(m_pCounter + i) = 0;
        *(m_pOffsetCounter + i) = 0;
    }
}

Floor::~Floor(){
    delete[] m_pCellPointerList;
    m_pCellPointerList = NULL;
    delete[] m_pColIndexList;
    m_pColIndexList = NULL;
    delete[] m_pCounter;
    m_pCounter = NULL;
    if (m_pOffsetCounter){ 
        delete[] m_pOffsetCounter; m_pOffsetCounter = NULL;
    }
}

bool Floor::checkActivated(const uint& iIndex){
    return (*(m_pCounter + iIndex));
}

// -------------------------------------------
// Function: Check if the cell is linear according to the dThresheld
// Input   : Split count of the cell's floor
//           Row count of the cell's floor
//           check dThresheld
// Output  : Linear or not
// -------------------------------------------
bool Floor::checkLinear(const uint& iIndex, const uint& iSplitCount, const uint& iRowCount, const double& dThresheld){
    vec cellVec = zeros<vec>(iRowCount);
    Utils::indexToVector(iIndex, cellVec, iSplitCount, iRowCount);
    double max = -1;
    double min = 1;
    for (vec::iterator iter = cellVec.begin(); iter != cellVec.end(); iter ++){
        max = max > *iter ? max : *iter;
        min = min < *iter ? min : *iter;
    }
    return ((max - min) <= dThresheld);
}

// ergodic this floor and act f
int Floor::ergodicFloor(Floor& nextFloor, const mat& matrix, const uint& iSplitCount, const uint& iRowCount, int (Floor::*f)(const uint&, const uint&, const uint&)){
   uint nextCellIndex = 0;
   for (uint i = 0; i != m_iLength; i ++){
       if (checkActivated(i)){
           // cell activated
           for (double dParam1 = -1; dParam1 <= 1; dParam1 += (2.0 / (double)(iSplitCount - 1))){
               for (double dParam2 = -1; dParam2 <= 1; dParam2 += (2.0 / (double)(iSplitCount - 1))){
                   for (uint k = 0; k != matrix.n_cols; k++){
                       nextCellIndex = getNextCellIndex(i, dParam1, dParam2, iSplitCount, iRowCount, matrix.col(k));
                       (nextFloor.*f)(i, nextCellIndex, k);
                   }
               }
           }
       }
       uint iScale = m_iLength / 10;
       if (i % iScale == 0 && i != 0){
           cout << "Finish scan " << i / iScale * 10 << "\% cells..." << endl;
           cout << endl;
       }
   }
   return 0;
}

int Floor::setCounter(Floor prevFloor, const mat& matrix, const uint& iSplitCount, const uint& iRowCount){
    assert(m_pCounter != NULL && m_pOffsetCounter != NULL);
    prevFloor.ergodicFloor(*this, matrix, iSplitCount, iRowCount, &Floor::addCounter);
    return 0;
}

int Floor::initList(){
    uint iListLength = getCounter(m_iLength);
    m_pCellPointerList = new uint[(int)iListLength];
    m_pColIndexList = new uint[(int)iListLength];

    return 0;
}

int Floor::setList(Floor prevFloor, const mat& matrix, const uint& iSplitCount, const uint& iRowCount){
    initList();
    prevFloor.ergodicFloor(*this, matrix, iSplitCount, iRowCount, &Floor::addIndex);

    delete[] m_pOffsetCounter;
    m_pOffsetCounter = NULL;

    return 0;
}

int Floor::addCounter(const uint& iPrevCellIndex, const uint& iCurCellIndex, const uint& iColIndex){
    // col index is not used in this function, just for ergodic function pointer use
    (*(m_pCounter + iCurCellIndex)) ++;
    (*(m_pOffsetCounter + iCurCellIndex)) ++;
    return 0;
}

int Floor::addIndex(const uint& iPrevCellIndex, const uint& iCurCellIndex, const uint& iColIndex){
    uint offset = getCounter(iCurCellIndex) + ((*(m_pCounter + iCurCellIndex)) - (*(m_pOffsetCounter + iCurCellIndex)));
    if ((*(m_pOffsetCounter + iCurCellIndex)) != 0)
        (*(m_pOffsetCounter + iCurCellIndex)) --;
    
    assert(offset <= getCounter(m_iLength));

    *(m_pCellPointerList + offset) = iPrevCellIndex;
    *(m_pColIndexList + offset) = iColIndex;
    
    return 0;
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
uint Floor::getNextCellIndex(const uint& iIndex, const double& dParam1, const double& dParam2, const uint& iSplitCount, const uint& iRowCount, const vec& vecElem){
    uint iResultIndex;
    vec cellVec = zeros<vec>(iRowCount);
    Utils::indexToVector(iIndex, cellVec, iSplitCount, iRowCount);
    
    vec vecResult = (cellVec * dParam1 + vecElem * dParam2) / 2;
    Utils::vectorToIndex(iResultIndex, vecResult, iSplitCount, iRowCount);

    return iResultIndex;
}

uint Floor::getCounter(const uint& iLength){
    if (!(iLength <= m_iLength)){
        cout << "iLength : " << iLength << endl;
        cout << "getLength : " << m_iLength << endl;
    }
    assert(iLength <= getLength());

    uint iListLength = 0; 
    for (uint i = 0; i != iLength; i ++){
        iListLength += *(m_pCounter + i);
    }
    return iListLength;
}

// Only for first floor use
int Floor::initFirstFloor(const mat& matrix, const uint& iSplitCount, const uint& iRowCount){
    for (uint i = 0; i != matrix.n_cols; i++){
        uint iCellIndex;
        Utils::vectorToIndex(iCellIndex, matrix.col(i), iSplitCount, iRowCount);
        addCounter(0, iCellIndex, 0);
    }

    initList();
    for (uint i = 0; i != matrix.n_cols; i++){
        uint iCellIndex;
        Utils::vectorToIndex(iCellIndex, matrix.col(i), iSplitCount, iRowCount);
        // don't have prev cell, set to 0 (uint type don't have -1), thus first floor cannot be detected according to collist or celllist
        addIndex(0, iCellIndex, i);
    }
    cout << endl;
    return 0;
}
    
