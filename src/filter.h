#ifndef FILTER_H
#define FILTER_H

#include <armadillo>
#include <string>
#include "tools.h"
#include "building.h"

#define DEFAULT_CFG_PATH "../cfg.txt"

class Filter{
    public:
        Filter();
        Filter(string strCfgFilePath);
        ~Filter();

        void setCfgFilePath(string strCfgFilePath);

        int getMatrixFromInputFile(mat &matrix);
        int genBuilding(mat matrix);
        int doFilter();

        string getCfgFilePath();
        string getInputMatFilePath();
        string getRecordFilePath();

    private:
        string m_strCfgFilePath;
        string m_strMatFilePath;
        string m_strRecordFilePath;
        Building m_bStorage;

        int init();
        void setVar(string strVarName, string strVarValue);

};

#endif

