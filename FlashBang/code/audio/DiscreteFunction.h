#ifndef __DISCRETE_FUNCTION_H__
#define __DISCRETE_FUNCTION_H__

#include <string>
using namespace std;
#include "DataWriter.h"

class DiscreteFunction
{
    float *_values;
    int _numberOfPoints;
 public:
    DiscreteFunction(int numberOfPoints);
    ~DiscreteFunction();
    float getValue(int index) { return _values[index]; }
    void setValue(int index, float value) { _values[index] = value; }
    int getNumberOfPoints() { return _numberOfPoints; }
    float getMaximumValue();
    void save(string filePath)
    {
    DataWriter *writer = new DataWriter(filePath);
    for (int i = 0; i < _numberOfPoints; ++i)
    {
        writer->writeFloat(_values[i]);
    }
    delete writer;
    }

    static DiscreteFunction *readFromHUDGraphFile(string filePath);
    static void remapDomain(DiscreteFunction *oldFunction, 
                DiscreteFunction *newFunction);
    static void remapRange(DiscreteFunction *oldFunction, 
               DiscreteFunction *newFunction,
               float ratio);
};

#endif 
