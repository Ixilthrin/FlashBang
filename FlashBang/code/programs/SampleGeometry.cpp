#include "SampleGeometry.h"



SampleGeometry::SampleGeometry()
{
}


SampleGeometry::~SampleGeometry()
{
}

int SampleGeometry::GenerateData()
{
    float x = -0.6f;
    float y = -0.6f;
    float z = 0.5f;

    _positions =
    {
        x, y, z
    };

    _colors =
    {
        .82f, 0.41f, 0.14f
    };

    _indices = {};

    int index = 0;
    float previousY = y;
    y -= .05f;

    _positions.push_back(x);
    _positions.push_back(y);
    _positions.push_back(z);
    _colors.push_back(.82f);
    _colors.push_back(.41f);
    _colors.push_back(.14f);

    for (int c = 0; c < 10; ++c)
    {
        y = previousY + .01 * c;
        x += .05f;
        if (c < 5)
            z -= .05f;
        else
            z += .05f;
        _positions.push_back(x);
        _positions.push_back(y);
        _positions.push_back(z);
        _colors.push_back(1.0f);
        _colors.push_back(0.0f);
        _colors.push_back(0.0f);
        _indices.push_back(index);
        _indices.push_back(index + 1);
        _indices.push_back(index + 2);
        index += 1;
        previousY = y;
        y -= .05f;
        _positions.push_back(x);
        _positions.push_back(y);
        _positions.push_back(z);
        _colors.push_back(.82f);
        _colors.push_back(.41f);
        _colors.push_back(.14f);
        _indices.push_back(index);
        _indices.push_back(index + 2);
        _indices.push_back(index + 1);
        index += 1;
    }

    return 0;
}

vector<float> SampleGeometry::GetPositions()
{
    return _positions;
}

vector<float> SampleGeometry::GetColors()
{
    return _colors;
}

vector<int> SampleGeometry::GetIndices()
{
    return _indices;
}
