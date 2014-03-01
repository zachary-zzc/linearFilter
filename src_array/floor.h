#ifndef FLOOR_H
#define FLOOR_H

#include <armadillo>
#include <assert.h>
#include <math.h>

#include "tools.h"


using namespace std;
using namespace arma;

class Floor{
    public:
        Floor(const uint& iSplitCount, const uint& iRowCount);
        ~Floor();

        bool checkActivated(const uint& iIndex);
        bool checkLinear(const uint& iIndex, const uint& iSplitCount, const uint& iRowCount, const double& dThresheld);

        int ergodicFloor(Floor& nextFloor, const mat& matrix, const uint& iSplitCount, const uint& iRowCount, int (Floor::*f)(const uint&, const uint&, const uint&));

        int setCounter(Floor& prevFloor, const mat& matrix, const uint& iSplitCount, const uint& iRowCount);

        int setList(Floor& prevFloor, const mat& matrix, const uint& iSplitCount, const uint& iRowCount); 

        // for first floor use
        int initFirstFloor(const mat& matrix, const uint& iSplitCount, const uint& iRowCount);

        uint getCounter(const uint& iLength);
        inline uint getLength(){return m_iLength;}

        uint getCellIndex(uint iIndex){return (*(m_pCellPointerList + iIndex));}
        uint getColIndex(uint iIndex){return (*(m_pColIndexList + iIndex));}
    private:
        uint *m_pCellPointerList;
        uint *m_pColIndexList;
        uint *m_pCounter;
        uint *m_pOffsetCounter;

        uint m_iLength;

        int addCounter(const uint& iPrevCellIndex, const uint& iCurCellIndex, const uint& iColIndex);

        int initList();
        int addIndex(const uint& iPrevCellIndex, const uint& iCurCellIndex, const uint& iColIndex);

        uint getNextCellIndex(const uint& iIndex, const double& dParam1, const double& dParam2, const uint& iSplitCount, const uint& iRowCount, const vec& vecElem);
};

#endif
