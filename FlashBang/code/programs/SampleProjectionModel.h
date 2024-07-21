#pragma once

#include <chrono>
#include <glad/glad.h>

#include "ProjectionOnlyShader.h"
#include "SampleGeometry.h"

using std::chrono::system_clock;
using std::chrono::duration;
using std::chrono::time_point;
using std::vector;

#include "IModel.h"

class SampleProjectionModel : public IModel
{
public:
    SampleProjectionModel();
    virtual ~SampleProjectionModel();
    virtual int Setup();
    virtual int Start();

private:
    GLuint _vaoHandle;
    ProjectionOnlyShader _shader;
    SampleGeometry _geometry;
    time_point<system_clock> _startTime;
};

