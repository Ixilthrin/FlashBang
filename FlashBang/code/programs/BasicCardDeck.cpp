#include "BasicCardDeck.h"

#include <iostream>
#include <glad/glad.h>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <filesystem>
#include <thread>
namespace fs = std::filesystem;

using std::endl;
using std::cout;
using std::cin;
using std::cerr;
using std::string;
using std::ifstream;
using std::stringstream;
using std::map;
using std::pair;
using std::vector;

#include "CardGeometry.h"
#include "ShaderProgramFactory.h"
#include "OverheadCamera.h"

//#include <ft2build.h>
//#include FT_FREETYPE_H  

const string BASE_PATH = "../FlashBangProject/";

using std::make_shared;

BasicCardDeck::BasicCardDeck()
{
    _appContext = make_shared<AppContext>();
    _appContext->setWindowWidth(1600);
    _appContext->setWindowHeight(900);
    _textureNames = 0;
    _camera = new OverheadCamera();
    _handler = new CardDeckInputHandler(_appContext, _camera);
    _deck = new CardDeck();
    _translator = new CardDeckEventTranslator();
}

BasicCardDeck::~BasicCardDeck()
{
}

int BasicCardDeck::Start()
{
    string deck = getDeck();
    float scale = 1;// getScale();
    float xPosition = 50; // getXPosition();
    float yPosition = 50; // getYPosition();

    int result = initializeWindow();
    if (result != 0)
        return result;

    result = initializeOpenGL();
    if (result != 0)
        return result;

    loadDeck(deck, scale, xPosition, yPosition);

    result = setupShaders();
    if (result != 0)
        return result;

    setupBuffers();
    setupIndexData();
    setupBindings();
    setupUniformLocations();
    setupTextures();
    setupEventListeners();
    setupGlobalGraphicsState();

    _currentFlipRotation = 0;
    _flippedHalf = false;

    while (!glfwWindowShouldClose(_window))
    {
        renderFrame();
        glfwSwapBuffers(_window);
        glfwPollEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    cleanUp();

    return 0;
}

string BasicCardDeck::getDeck()
{
    cout << "Which deck?" << endl;

    int index = 0;

    vector<string> decks{};

    for (const auto &entry : fs::directory_iterator(BASE_PATH + "decks"))
    {
        if (!entry.is_directory())
            continue;

        string path{ entry.path().u8string() };
        int position = path.find_last_of("\\");
        if (position > 0)
        {
            path = path.substr(position + 1);
            decks.push_back(path);
            cout << "(" << (index + 1) << ") " << path << endl;
        }
        ++index;
    }

    int x;
    cin >> x;

    if (x < 0 || x > decks.size() + 1) {
        return "";
    }
    return decks[x - 1];
}

float BasicCardDeck::getScale()
{
    cout << "Enter scale: ";
    float scale;
    cin >> scale;
    if (scale <= 1)
        scale = 1;

    return scale;
}

int BasicCardDeck::getXPosition()
{
    cout << "Enter x: ";
    float x;
    cin >> x;
    if (x <= 1)
        x = 1;

    return x;
}

int BasicCardDeck::getYPosition()
{
    cout << "Enter y: ";
    float y;
    cin >> y;
    if (y <= 1)
        y = 1;

    return y;
}

int BasicCardDeck::initializeWindow()
{
    if (!glfwInit())
        return -1;

    _window = glfwCreateWindow(_appContext->getWindowWidth(), _appContext->getWindowHeight(), "Simulated Card Deck Prototype", NULL, NULL);
    if (!_window)
    {
        glfwTerminate();
        return -2;
    }

    glfwMakeContextCurrent(_window);

    return 0;
}

int BasicCardDeck::initializeOpenGL()
{
    if (!gladLoadGL())
        return -3;
    return 0;
}

int BasicCardDeck::loadDeck(string name, float scale, int xPosition, int yPosition)
{
    string inputString;
    string targetFile = name;
    string strScale = "";
    string startX = "";
    string startY = "";

    if (targetFile.length() < 1) 
    {
        ifstream myfile(BASE_PATH + "flashbang.props");
        if (myfile.is_open())
        {
            int index = 0;
            while (getline(myfile, inputString))
            {
                switch (index)
                {
                case 0:
                    targetFile = inputString;
                    break;
                case 1:
                    strScale= inputString;
                    break;
                case 2:
                    startX = inputString;
                    break;
                case 3:
                    startY = inputString;
                    break;
                }
                index++;
            }
            myfile.close();
        }
    }
    if (strScale.length() > 0)
    {
        float scaleValue = std::stof(strScale);
        if (scaleValue > 0)
            _deck->setScale(scaleValue);
    }
    else
    {
        _deck->setScale(scale);
    }

    if (startX.length() > 0 && startY.length() > 0)
    {
        float x = std::stof(startX);
        float y = std::stof(startY);
        if (x > 0 && y > 0)
            _deck->setInitialPosition(x, y);
    }
    else
    {
        _deck->setInitialPosition(xPosition, yPosition);
    }

    string baseDir = BASE_PATH + "decks/" + targetFile + "/";

    _converter = new Converter(_appContext->getWindowWidth(), _appContext->getWindowHeight());
    _deck->setConverter(_converter);
    _deck->setUpFromBaseDir(baseDir);

    glfwSetWindowTitle(_window, targetFile.c_str());
    return 0;
}

int BasicCardDeck::setupShaders()
{
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    if (vertShader == 0)
        return -4;

    _programHandle = ShaderProgramFactory::BuildShaderProgram(
        Card::getVertexShaderPath(),
        Card::getFragmentShaderPath()
    );

    glUseProgram(_programHandle);

    //glDetachShader(_programHandle, vertShader);
    //glDetachShader(_programHandle, fragShader);

    //glDeleteShader(vertShader);
    //glDeleteShader(fragShader);
    return 0;
}

int BasicCardDeck::setupBuffers()
{
    vector<float> vertexData{};

    for (auto const& id : _deck->getIds())
    {
        CardGeometry* geo = _deck->getGeometry(id);

        // card vertices
        // v1 : x, y, z, r, g, b, u, v
        // v2 : x, y, z, r, g, b, u, v
        // v3 : x, y, z, r, g, b, u, v
        // v4 : x, y, z, r, g, b, u, v

        vector<XYZColorTex> vertices = geo->getVertices();

        for (auto const& vertex : vertices)
        {
            vertexData.push_back(vertex.x);
            vertexData.push_back(vertex.y);
            vertexData.push_back(vertex.z);
            vertexData.push_back(vertex.r);
            vertexData.push_back(vertex.g);
            vertexData.push_back(vertex.b);
            vertexData.push_back(vertex.u);
            vertexData.push_back(vertex.v);
        }
    }
    glGenBuffers(1, &_vertexHandle);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexHandle);
    //glNamedBufferStorage(_vertexHandle, vertexData.size() * sizeof(float), vertexData.data(), GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferStorage(_vertexHandle, vertexData.size() * sizeof(float), vertexData.data(), GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);

    //cout << "error: " << glGetError() << endl;
    return 0;
}

int BasicCardDeck::setupIndexData()
{
    _deck->getIndexData(_indexData, _indexOffsets);
    return 0;
}

int BasicCardDeck::setupBindings()
{
    // I don't know why the following 2 lines did not work, but instead of
    // binding an index buffer I just use the array of index values.

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexCoordsBufferHandle);
    //glBufferData(indexCoordsBufferHandle, indexData.size() * sizeof(unsigned int), indexData.data(), GL_STATIC_DRAW);

    GLuint vaoHandle;
    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    int stride = 8 * sizeof(float);
    glBindVertexBuffer(0, _vertexHandle, 0, stride);
    glBindVertexBuffer(1, _vertexHandle, 3 * sizeof(float), stride);
    glBindVertexBuffer(2, _vertexHandle, 6 * sizeof(float), stride);

    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(0, 0);

    glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(1, 1);

    glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(2, 2);

    return 0;
}

int BasicCardDeck::setupUniformLocations()
{
    _translationLocation = glGetUniformLocation(_programHandle, "Translation");
    _rotationYLocation = glGetUniformLocation(_programHandle, "RotationY");
    _objectWidthLocation = glGetUniformLocation(_programHandle, "ObjectWidth");
    _zoomFactorLocation = glGetUniformLocation(_programHandle, "ZoomFactor");
    _cameraTranslationLocation = glGetUniformLocation(_programHandle, "CameraTranslation");

    return 0;
}

int BasicCardDeck::setupTextures()
{
    int numberOfTexturesNeeded = _deck->numberOfCardSides();
    _textureNames = new GLuint[numberOfTexturesNeeded];
    glCreateTextures(GL_TEXTURE_2D, numberOfTexturesNeeded, _textureNames);

    int textureNameIndex = 0;

    vector<int> toBeRemoved;
    for (auto const &id : _deck->getIds())
    {
        auto imageReader = _deck->getImageData(id)->getImageReader();
        GLubyte* image = imageReader->getImageData();

        int imageWidth = imageReader->getWidth();
        int imageHeight = imageReader->getHeight();
        if (image == 0 || imageWidth == 0 || imageHeight == 0)
        {
            toBeRemoved.push_back(id);
            continue;
        }

        _textures.insert(pair<int, GLuint>(id, _textureNames[textureNameIndex]));


        glTextureStorage2D(_textureNames[textureNameIndex], 1, GL_RGBA8, imageWidth, imageHeight);
        glBindTexture(GL_TEXTURE_2D, _textureNames[textureNameIndex]);

        if (image == NULL)
            throw(string("Failed to load image"));
        glTextureSubImage2D(_textureNames[textureNameIndex], 0, 0, 0, imageWidth, imageHeight, GL_RGBA, GL_UNSIGNED_BYTE, image);

        ++textureNameIndex;
    }

    for (auto const &id : toBeRemoved)
    {
        _deck->removeCard(id);
    }

    for (auto const &id : _deck->getIds())
    {
        if (!_deck->get(id)->hasFlipSide())
        {
            continue;
        }

        _flippedTextures.insert(pair<int, GLuint>(id, _textureNames[textureNameIndex]));

        auto reader = _deck->getImageData(id)->getBackImageReader();

        GLubyte* image = reader->getImageData();
        int imageWidth = reader->getWidth();
        int imageHeight = reader->getHeight();

        glTextureStorage2D(_textureNames[textureNameIndex], 1, GL_RGBA8, imageWidth, imageHeight);
        glBindTexture(GL_TEXTURE_2D, _textureNames[textureNameIndex]);

        if (image == nullptr)
            throw(string("Failed to load image"));
        glTextureSubImage2D(_textureNames[textureNameIndex], 0, 0, 0, imageWidth, imageHeight, GL_RGBA, GL_UNSIGNED_BYTE, image);

        ++textureNameIndex;
    }
    return 0;
}

int BasicCardDeck::cleanUp()
{
    if (_textureNames)
    {
        glDeleteTextures(_deck->size(), _textureNames);
        delete[] _textureNames;
    }

    return 0;
}

int BasicCardDeck::setupEventListeners()
{
    _handler->setDeck(_deck);
    _handler->setCamera(_camera);
    CardDeckDispatchingMouseHandlers::translator = _translator;
    _translator->registerEventHandler(_handler);
    glfwSetCursorPosCallback(_window, CardDeckDispatchingMouseHandlers::cursor_position_callback);
    glfwSetCursorEnterCallback(_window, CardDeckDispatchingMouseHandlers::cursor_enter_callback);
    glfwSetMouseButtonCallback(_window, CardDeckDispatchingMouseHandlers::mouse_button_callback);
    glfwSetScrollCallback(_window, CardDeckDispatchingMouseHandlers::mouse_wheel_callback);
    return 0;
}

int BasicCardDeck::setupGlobalGraphicsState()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glCullFace(GL_BACK);
    glClearColor(.6, .6, .6, 1);
    glPointSize(5);
    glLineWidth(3);
    glDisable(GL_DEPTH_TEST);
    glfwSwapInterval(1);
    return 0;
}

void BasicCardDeck::renderFrame()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (_zoomFactorLocation != -1)
    {
        float zoom = _camera->getZoomFactor();
        glUniform1f(_zoomFactorLocation, zoom);
    }

    if (_cameraTranslationLocation != -1)
    {
        float transX = _camera->getTranslationX();
        float transY = _camera->getTranslationY();
        glUniform2f(_cameraTranslationLocation, transX, transY);
    }

    if (_translationLocation != -1)
    {
        float xTrans{ 0 }, yTrans{ 0 };

        int objectCount = 0;
        vector<int> ids = _deck->getIds();
        for (auto const& id : ids)
        {
            auto card = _deck->get(id);
            auto width = _converter->screenTranslationXToNDC(card->getWidth());
            if (card->hasFlipSide() && card->requestFlip())
            {
                glUniform1f(_rotationYLocation, _currentFlipRotation);
                glUniform1f(_objectWidthLocation, width);
                _currentFlipRotation += .25;
                if (_currentFlipRotation > 1.57 && !_flippedHalf)
                {
                    _currentFlipRotation += 3.14f;
                    card->flipHalfComplete();
                    _flippedHalf = true;
                }
                if (_currentFlipRotation > 6.28)
                {
                    card->flipComplete();
                    _currentFlipRotation = 0;
                    _flippedHalf = false;
                }
            }
            else
            {
                glUniform1f(_rotationYLocation, 0);
            }

            if (card->hasFlipSide() && card->isFlipped())
            {
                glBindTextureUnit(0, _flippedTextures[id]);
            }
            else
            {
                glBindTextureUnit(0, _textures[id]);
            }

            /*int selected = _listener->getSelectedId();
            if (_listener->isSelectAndMoveInProgress() && selected == id)
            {
                xTrans = _converter->screenTranslationXToNDC(_deck->get(id)->getTranslationX() + _listener->getMovementX());
                yTrans = _converter->screenTranslationYToNDC(_deck->get(id)->getTranslationY() + _listener->getMovementY());
                glUniform2f(_translationLocation, xTrans, yTrans);
            }
            else if (_listener->isSelectAndMoveInProgress() && selected == -1)
            {
                xTrans = _converter->screenTranslationXToNDC(_camera->getTranslationX() - _listener->getMovementX());
                yTrans = _converter->screenTranslationYToNDC(_camera->getTranslationY() - _listener->getMovementY());


                if (_cameraTranslationLocation != -1)
                {
                    glUniform2f(_cameraTranslationLocation, xTrans, yTrans);
                }

                xTrans = _converter->screenTranslationXToNDC(_deck->get(id)->getTranslationX());
                yTrans = _converter->screenTranslationYToNDC(_deck->get(id)->getTranslationY());
                glUniform2f(_translationLocation, xTrans, yTrans);
            }
            else
            {
                xTrans = _converter->screenTranslationXToNDC(_deck->get(id)->getTranslationX());
                yTrans = _converter->screenTranslationYToNDC(_deck->get(id)->getTranslationY());
                glUniform2f(_translationLocation, xTrans, yTrans);

                xTrans = _converter->screenTranslationXToNDC(_camera->getTranslationX());
                yTrans = _converter->screenTranslationYToNDC(_camera->getTranslationY());
                if (_cameraTranslationLocation != -1)
                {
                    glUniform2f(_cameraTranslationLocation, xTrans, yTrans);
                }
            }*/

            xTrans = _converter->screenTranslationXToNDC(_deck->get(id)->getTranslationX());
            yTrans = _converter->screenTranslationYToNDC(_deck->get(id)->getTranslationY());
            glUniform2f(_translationLocation, xTrans, yTrans);

            xTrans = _converter->screenTranslationXToNDC(_camera->getTranslationX());
            yTrans = _converter->screenTranslationYToNDC(_camera->getTranslationY());
            if (_cameraTranslationLocation != -1)
            {
                glUniform2f(_cameraTranslationLocation, xTrans, yTrans);
            }

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, &(_indexData.data())[_indexOffsets[id]]);
        }
    }
}