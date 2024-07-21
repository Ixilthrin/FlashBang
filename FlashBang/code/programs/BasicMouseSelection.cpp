#include "BasicMouseSelection.h"

#include <iostream>
#include <glad/glad.h>
#include <stdio.h>
#include <sstream>
#include <fstream>

using std::endl;
using std::cout;
using std::cin;
using std::cerr;
using std::string;
using std::ifstream;
using std::stringstream;

#include "CardDeckDispatchingMouseHandlers.h" // Interesting: putting this after next line causes link errors
#include <GLFW/glfw3.h>

#include "ShaderSource.h"
#include "Card.h"
#include "Converter.h"

#include "ImageReader.h"

BasicMouseSelection::BasicMouseSelection()
{
}

BasicMouseSelection::~BasicMouseSelection()
{
}

int BasicMouseSelection::Start()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    int width = 640;
    int height = 480;
    window = glfwCreateWindow(width, height, "Select and Move sample application", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -2;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL())
        return -3;

    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    if (vertShader == 0)
        return -4;

    ShaderSource shaderSource;
    string shaderCode = shaderSource.ReadShaderFromFile("../FlashBang/shaders/translation_and_texture.vert.glsl");
    const GLchar *codeArray[] = { shaderCode.c_str() };

    glShaderSource(vertShader, 1, codeArray, NULL);

    glCompileShader(vertShader);

    GLint result;
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE || shaderCode.length() < 1)
        return -5;

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    if (fragShader == 0)
        return -6;

    string fragCode = shaderSource.ReadShaderFromFile("../FlashBang/shaders/texture.frag.glsl");
    const GLchar *fragArray[] = { fragCode.c_str() };

    glShaderSource(fragShader, 1, fragArray, NULL);

    glCompileShader(fragShader);

    GLint fragResult;
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &fragResult);
    if (fragResult == GL_FALSE || fragCode.length() < 1)
        return -7;

    GLuint programHandle = glCreateProgram();
    if (programHandle == 0)
        return -8;

    glAttachShader(programHandle, vertShader);
    glAttachShader(programHandle, fragShader);

    glLinkProgram(programHandle);

    GLint linkStatus;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_FALSE)
        return -9;

    glUseProgram(programHandle);

    glDetachShader(programHandle, vertShader);
    glDetachShader(programHandle, fragShader);

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    GLuint vaoHandle;


    Converter converter{ width, height };
    CardDeck deck{ &converter };
    Card card{ 200, 100, 20, 50 };
    deck.add(0, &card);

    float x1 = converter.screenXToNDC(0);
    float y1 = converter.screenYToNDC(0);
    float x2 = converter.screenXToNDC(card.getWidth());
    float y2 = converter.screenYToNDC(card.getHeight());

    float positionData[] =
    {
        x1,
        y1,
        0.0f,

        x1,
        y2,
        0.0f,

        x2,
        y2,
        0.0f,

        x2,
        y1,
        0.0f
    };

    float colorData[] =
    {
        .82f, 0.41f, 0.14f, 
        .82f, 0.41f, 0.14f,
        1.0f, 0.0f, 0.0f, 
        1.0f, 0.0f, 0.0f
    };

    int indexData[] =
    {
        0, 1, 2, 
        2, 3, 0
    };

    float texCoords[] =
    {
        0.0f, 0.0f, 
        0.0f, 1.0f, 
        1.0f, 1.0f, 
        1.0f, 0.0f
    };

    int positionLength = sizeof(positionData) / sizeof(positionData[0]);
    int colorDataLength = sizeof(colorData) / sizeof(colorData[0]);
    int indexLength = sizeof(indexData) / sizeof(indexData[0]);
    int texCoordLength = sizeof(texCoords) / sizeof(texCoords[0]);

    GLuint vboHandles[3];
    glGenBuffers(3, vboHandles);

    GLuint positionBufferHandle = vboHandles[0];
    GLuint colorBufferHandle = vboHandles[1];
    GLuint texCoordsBufferHandle = vboHandles[2];

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, positionLength * sizeof(float), positionData, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, colorDataLength * sizeof(float), colorData, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, texCoordsBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, texCoordLength * sizeof(float), texCoords, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexBuffer(0, positionBufferHandle, 0, sizeof(GLfloat) * 3);
    glBindVertexBuffer(1, colorBufferHandle, 0, sizeof(GLfloat) * 3);
    glBindVertexBuffer(2, texCoordsBufferHandle, 0, sizeof(GLfloat) * 2);

    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(0, 0);

    glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(1, 1);

    glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(2, 2);

    CardDeckInputListener listener;
    listener.setDeck(&deck);

    GLint loc = glGetUniformLocation(programHandle, "Translation");

    glClearColor(0, 0, 0, 1);
    glPointSize(5);
    glLineWidth(3);

    ImageReader imageProvider("../FlashBangProject/decks/default/test.png");
    
    GLubyte* image = imageProvider.getImageData();

    if (image == nullptr)
        throw(std::string("Failed to load image"));

    std::cout << "test.png loaded   " 
        << imageProvider.getWidth()
        << "x" 
        << imageProvider.getHeight()
        << "    components: " 
        << imageProvider.getComponentCount()
        << "    size: " 
        << (imageProvider.getWidth()*imageProvider.getHeight()*imageProvider.getComponentCount()) << std::endl;


    GLuint textureNames[1];
    glCreateTextures(GL_TEXTURE_2D, 1, textureNames);

    glTextureStorage2D(textureNames[0], 1, GL_RGBA8, imageProvider.getWidth(), imageProvider.getHeight());
    glBindTexture(GL_TEXTURE_2D, textureNames[0]);
    glTextureSubImage2D(textureNames[0], 0, 0, 0, imageProvider.getWidth(), imageProvider.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, image);

    glBindTextureUnit(0, textureNames[0]);

    CardDeckEventTranslator translator;

    CardDeckDispatchingMouseHandlers::translator = &translator;
    translator.registerListener(&listener);
    glfwSetCursorPosCallback(window, CardDeckDispatchingMouseHandlers::cursor_position_callback);
    glfwSetCursorEnterCallback(window, CardDeckDispatchingMouseHandlers::cursor_enter_callback);
    glfwSetMouseButtonCallback(window, CardDeckDispatchingMouseHandlers::mouse_button_callback);
    glfwSetScrollCallback(window, CardDeckDispatchingMouseHandlers::scroll_callback);


    while (!glfwWindowShouldClose(window))
    {
        if (loc != -1)
        {
            float xTrans, yTrans;
            if (listener.isSelectAndMoveInProgress())
            {
                xTrans = converter.screenTranslationXToNDC(card.getTranslationX() + listener.getMovementX());
                yTrans = converter.screenTranslationYToNDC(card.getTranslationY() + listener.getMovementY());
            }
            else
            {
                xTrans = converter.screenTranslationXToNDC(card.getTranslationX());
                yTrans = converter.screenTranslationYToNDC(card.getTranslationY());
            }
            glUniform2f(loc, xTrans, yTrans);
        } 

        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vaoHandle);

        glDrawElements(GL_TRIANGLES, indexLength, GL_UNSIGNED_INT, indexData);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glDeleteTextures(1, textureNames);

    return 0;
}
