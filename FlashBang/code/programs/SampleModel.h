#pragma once

#include <chrono>
#include <glad/glad.h>

#include "RotationShaderNoProjection.h"
#include "SampleGeometry.h"

using std::chrono::system_clock;
using std::chrono::duration;
using std::chrono::time_point;
using std::vector;

#include "IModel.h"

class SampleModel : public IModel
{
public:
    SampleModel();
    virtual ~SampleModel();
    virtual int Setup();
    virtual int Start();

private:
    GLuint _vaoHandle;
    RotationShaderNoProjection _shader;
    SampleGeometry _geometry;
    time_point<system_clock> _startTime;
};

