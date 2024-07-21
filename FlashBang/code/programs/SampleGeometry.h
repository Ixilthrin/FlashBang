#pragma once

#include <vector>

using std::vector;

class SampleGeometry
{
public:
    SampleGeometry();
    virtual ~SampleGeometry();
    int GenerateData();
    vector<float> GetPositions();
    vector<float> GetColors();
    vector<int> GetIndices();

private:
    vector<float> _positions;
    vector<float> _colors;
    vector<int> _indices;
};

