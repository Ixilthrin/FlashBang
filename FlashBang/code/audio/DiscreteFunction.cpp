#include "DiscreteFunction.h"
#include "DataReader.h"

DiscreteFunction::DiscreteFunction(int numberOfPoints)
    : _numberOfPoints(numberOfPoints)
{
    //OUTPUT("Created new float array with size = ");
    //OUTPUT(StringUtil::intToString(numberOfPoints));
    //OUTPUT("\n");
    _values = new float[numberOfPoints];
    for (int i = 0; i < numberOfPoints; ++i)
    {
    _values[i] = 0;
    }
}
DiscreteFunction::~DiscreteFunction()
{
    delete [] _values;
}

float DiscreteFunction::getMaximumValue()
{
    float maxValue = 0.0;
    for (int i = 0; i < _numberOfPoints; ++i)
    {
    float value = _values[i];
    if (value < 0)
    {
        value = -value;
    }
    if (value > maxValue)
    {
        maxValue = value;
    }
    }
    return maxValue;
}

// See HUDGraph.h and HUDGraph.cpp for information abot HUDGraph.
// It is essentially a set of points taken from the screen that
// represents a cartesian graph.
DiscreteFunction *DiscreteFunction::readFromHUDGraphFile(string filePath)
{
    DataReader *reader = new DataReader(filePath);
    reader->readString();  // Ignore header info.
    reader->readString();  // Ignore "version" string.
   
    DiscreteFunction *f = 0;
    int version = reader->readInt();
    //OUTPUT("Version = ");
    //OUTPUT(StringUtil::intToString(version));
    //OUTPUT("\n");
    if (version == 1)
    {
    //OUTPUT(reader->readString());  // Ignore "Delta_Y"
    //OUTPUT("\n");
    reader->readInt();     // Ignore deltaY
    reader->readString();  // Ignore "Delta_X"
    int numberOfValues = reader->readInt(); // deltaX
    //OUTPUT("Number of values = ");
    //OUTPUT(StringUtil::intToString(numberOfValues));
    //OUTPUT("\n");
    reader->readString(); // "An the data is..."
    f = new DiscreteFunction(numberOfValues);
    int value1 = 0;
    int value2 = 0;
    for (int i = 0; i < numberOfValues; ++i)
    {
        value1 = reader->readInt();  // Ignore this since it should correspond to i.
        //OUTPUT("Value1 Is ");
        //OUTPUT(StringUtil::intToString(value1));
        //OUTPUT("\n");
        value2 = reader->readInt();
        //OUTPUT("Value2 Is ");
        //OUTPUT(StringUtil::intToString(value2));
        //OUTPUT("\n");
        f->setValue(i, (float) value2);
    }
    }
    else
    {
    //OUTPUT("Unknown HUDGraph Version ");
    //OUTPUT(StringUtil::intToString(version));
    //OUTPUT("\n");
    }
    delete reader;
    return f;
}

void DiscreteFunction::remapDomain(DiscreteFunction *oldFunction, 
                   DiscreteFunction *newFunction)
{
    // Boundary values stay the same
    newFunction->setValue(0, oldFunction->getValue(0));
    int numOldPoints = oldFunction->getNumberOfPoints();
    //OUTPUT("remap: numOldPoints = ");
    //OUTPUT(StringUtil::intToString(numOldPoints));
    //OUTPUT("\n");
    int numNewPoints = newFunction->getNumberOfPoints();
    //OUTPUT("remap: numNewPoints = ");
    //OUTPUT(StringUtil::intToString(numNewPoints));
    //OUTPUT("\n");
    float ratio = (float) numNewPoints / (float) numOldPoints;
    //OUTPUT("remap: ratio = ");
    //OUTPUT(StringUtil::floatToString(ratio));
    //OUTPUT("\n");

    newFunction->setValue(numNewPoints - 1, 
              oldFunction->getValue(numOldPoints - 1));

    for (int i = 1; i < numNewPoints - 1; ++i)
    {
    int oldIndex = (int) ((float) i / ratio);
    newFunction->setValue(i, oldFunction->getValue(oldIndex));
    }
}

void DiscreteFunction::remapRange(DiscreteFunction *oldFunction, 
                   DiscreteFunction *newFunction,
                   float ratio)
{
    for (int i = 0; i < oldFunction->getNumberOfPoints(); ++i)
    {
    newFunction->setValue(i, oldFunction->getValue(i) * ratio);
    }
}
