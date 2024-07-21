#include "SampleProjectionModel.h"

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

SampleProjectionModel::SampleProjectionModel()
{
}


SampleProjectionModel::~SampleProjectionModel()
{
}

int SampleProjectionModel::Setup()
{
    _shader = ProjectionOnlyShader();
    int setupResult = _shader.Setup();
    if (setupResult != 0)
        return setupResult;

    _geometry = SampleGeometry();
    _geometry.GenerateData();

    GLuint vboHandles[2];
    glGenBuffers(2, vboHandles);

    GLuint positionBufferHandle = vboHandles[0];
    GLuint colorBufferHandle = vboHandles[1];

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, _geometry.GetPositions().size() * sizeof(float), &_geometry.GetPositions()[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, _geometry.GetColors().size() * sizeof(float), &_geometry.GetColors()[0], GL_STATIC_DRAW);

    glGenVertexArrays(1, &_vaoHandle);
    glBindVertexArray(_vaoHandle);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexBuffer(0, positionBufferHandle, 0, sizeof(GLfloat) * 3);
    glBindVertexBuffer(1, colorBufferHandle, 0, sizeof(GLfloat) * 3);

    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(0, 0);

    glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(1, 1);


    _startTime = system_clock::now();

    return 0;
}

int SampleProjectionModel::Start()
{
    float angle = 0.0f;
    float rotationRate = 2.0f * 3.14159f / 10.0f;
    auto currentTime = system_clock::now();
    duration<double> deltaTime;
    deltaTime = currentTime - _startTime;
    angle = rotationRate * (float)deltaTime.count();

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(90.0f), 4.0f / 3.0f, 0.1f, 100.f);
    //glm::mat4 projectionMatrix = glm::mat4(1.0f);
    glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.2f, 0.5f, -1.01f));
    //View = glm::rotate(View, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    projectionMatrix = projectionMatrix * View;
    GLuint location = glGetUniformLocation(_shader.GetProgramHandle(), "ProjectionMatrix");
    if (location >= 0)
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    glBindVertexArray(_vaoHandle);

    glDrawElements(GL_TRIANGLES, _geometry.GetIndices().size(), GL_UNSIGNED_INT, &_geometry.GetIndices()[0]);

    return 0;
}
