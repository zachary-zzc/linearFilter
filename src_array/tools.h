/* Author: Zachary ZHAO
 */

#ifndef TOOLS_H
#define TOOLS_H

#include <armadillo>
#include <iostream>
#include <vector>
#include <string>
using namespace std;
using namespace arma;

class Utils{
    public:
        Utils();
        ~Utils();
        static void vecScaling(vec &vector);
        static void matScaling(mat &matrix);
        template<typename T>
        static void record(vector<T> s, ofstream &ofs){
            for (typename vector<T>::iterator iter = s.begin(); iter != s.end(); iter ++){
                ofs << *iter << " ";
            }
            ofs << endl;
        }

        static void matDeduce(mat matrix, mat &deMatrix, int iRowCount);

        static string& rTrim(string &ss);
        static string& lTrim(string &ss);
        static string& trim(string &ss);
        static void parseLine(string strLine, string deLimiter, vector<string> &vecString);

        static int strToInt(string ss);
        static double strToDouble(string ss);

        static void indexToVector(uint iIndex, vec &vecCell, uint iSplitCount, uint iRowCount);
        static void vectorToIndex(uint &iIndex, vec vecCell, uint iSplitCount, uint iRowCount);
};

#endif
