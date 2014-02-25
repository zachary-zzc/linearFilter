#ifndef BUILDING_H
#define BUILDING_H

#include <armadillo>
#include <vector>
#include <string>
#include "cell.h"


using namespace std;
using namespace arma;

class Building{
    private:
        string m_strRecordFilePath;
        int m_iHeight;
        int m_iSplitCount;
        int m_iRowCount;
        double m_dThresheld;

        mat m_matrix;

        typedef struct Floor{
            vector<Cell> vecCells;
            Floor(int iSplitCount, int iRowCount){vecCells = vector<Cell>(pow(iSplitCount, iRowCount));}
        } Floor;

        vector<Floor> m_vecFloors;

        Cell* getCell(int iFloorIndex, int iCellIndex);
 
        int dfs(Cell* pCurCell, int iFloorIndex, vector<int> &vecColList, ofstream &ofs);

        int generateNextFloor(Floor thisFloor, Floor &nextFloor);

    public:
        Building();
        ~Building();

        void setRecordFilePath(string strRecordFilePath);
        void setSplitCount(int iSplitCount);
        void setRowCount(int iRowCount);
        void setHeight(int iHeight);
        void setThresheld(double dThresheld);

        int getSplitCount();
        int getRowCount();
        int getHeight();
        double getThresheld();

        int initBuilding(mat matrix);

        int  build();
        int  traceBack();
};

#endif
