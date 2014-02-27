#ifndef CELL_H
#define CELL_H

#include <armadillo>
#include <vector>
#include <assert.h>
#include <math.h>

#include "tools.h"


using namespace std;
using namespace arma;

class Cell{
    public:
        Cell();
        ~Cell();

        static void indexToVector(int iIndex, vec &vecCells, int iSplitCount, int iRowCount);
        static void vectorToIndex(int &iIndex, vec vecCells, int iSplitCount, int iRowCount);

        bool checkActivated();
        bool checkLinear(int iMyIndex, int iSplitCount, int iRowCount, double dThresheld);

        int lastCellListSize();
        int lastColListSize();

        int getLastCellIndex(int iIndex);
        void getLastColIndex(int iIndex, vector<int> &vecColIndex);

        void activate();
        void addMatrixColIndexVec(vector<int> iColIndex);
        void addPointerIndex(int iPointerIndex);
        void addIndex(int iCellIndex, int iColIndex);

        int getNextCellIndex(int iMyIndex, double dParam1, double dParam2, int iSplitCount, int iRowCount, vec vecElem);
    
    private:
        bool m_bActivated;
        vector<int> m_vecPointerList;
        vector< vector<int> > m_vecMatrixColIndexList;
};

#endif
