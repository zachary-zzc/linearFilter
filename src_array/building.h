#ifndef BUILDING_H
#define BUILDING_H

#include <armadillo>
#include <vector>
#include <string>
#include "tools.h"
#include "floor.h"


using namespace std;
using namespace arma;

class Building{
    public:
        Building();
        ~Building();

        void setRecordFilePath(string strRecordFilePath);
        void setSplitCount(uint iSplitCount);
        void setRowCount(uint iRowCount);
        void setHeight(int iHeight);
        void setThresheld(double dThresheld);

        uint getSplitCount();
        uint getRowCount();
        int getHeight();
        double getThresheld();

        int initBuilding(mat matrix);

        int  build();
        int  traceBack();

    private:
        string m_strRecordFilePath;
        int m_iHeight;
        uint m_iSplitCount;
        uint m_iRowCount;
        double m_dThresheld;

        mat m_matrix;

        vector< Floor* > m_pFloors;

        int dfs(uint iCellIndex, uint iFloorIndex, vector<uint> &vecColList, ofstream &ofs);

        int generateNextFloor(Floor& thisFloor, Floor &nextFloor);
};

#endif
