#include "CardDeckInputListener.h"

#include <iostream>
#include <vector>

using std::make_pair;
using std::vector;

CardDeckInputListener::CardDeckInputListener()
{
    _selectAndMoveInProgress = false;
    _mouseX = 0;
    _mouseY = 0;
    _selectionStartX = 0;
    _selectionStartY = 0;
    _selectedId = -1;
    _deck = nullptr;
    _soundPlayed = false;
}
CardDeckInputListener::CardDeckInputListener(shared_ptr<AppContext> context, OverheadCamera *camera)
    : CardDeckInputListener()
{
    _context = context;
    _camera = camera;
}

/*
*  The inputs are screen space.  The models as seen by the
*  user are in view space.  To get to view space each model
*  is first translated to world space then the shader used
*  the w coordinate for zoom.
*/
pair<int, int> CardDeckInputListener::computeTransformation(int x, int y)
{
    int windowWidth = _context->getWindowWidth();
    int windowHeight = _context->getWindowHeight();
    float zoomFactor = 1;
    if (_camera)
    {
        zoomFactor = _camera->getZoomFactor();
    }

    int worldX = x - (windowWidth / 2);
    int worldY = y - (windowHeight / 2);
    worldX = (float)worldX * zoomFactor;
    worldY = (float)worldY * zoomFactor;
    worldX += (windowWidth / 2) + _camera->getTranslationX();
    worldY += (windowHeight / 2) + _camera->getTranslationY();

    return  make_pair(worldX, worldY);
}

void CardDeckInputListener::select(int x, int y)
{
    pair<int, int> worldCoords = computeTransformation(x, y);

    _soundPlayed = false;

    bool itemSelected = false;
    vector<int> ids = _deck->getIds();
    auto it = ids.rbegin();
    while (it != ids.rend())
    {
        Card *card = _deck->get(*it);
        if (card)
        {
            if (card->contains(worldCoords.first, worldCoords.second))
            {
                itemSelected = true;
                if (!_selectAndMoveInProgress)
                {
                    _selectionStartX = worldCoords.first;
                    _selectionStartY = worldCoords.second;
                    _mouseX = worldCoords.first;
                    _mouseY = worldCoords.second;
                }
                _selectedId = *it;
                _selectAndMoveInProgress = true;
                break;
            }
        }
        it++;
    }
    
    if (!itemSelected)
    {
        if (!_selectAndMoveInProgress)
        {
            _selectionStartX = worldCoords.first;
            _selectionStartY = worldCoords.second;
            _mouseX = worldCoords.first;
            _mouseY = worldCoords.second;
        }
        _selectAndMoveInProgress = true;
    }
}

void CardDeckInputListener::moveSelection(int x, int y)
{
    pair<int, int> worldCoords = computeTransformation(x, y);

    Card *card = _deck->get(_selectedId);
    if (_selectAndMoveInProgress)
    {
        _mouseX = worldCoords.first;
        _mouseY = worldCoords.second;

        if (card)
        {
            if (!_soundPlayed)
            {
                _deck->playSound(0);
                _soundPlayed = true;
            }

            _deck->bringToTop(_deck->getId(card));
        }
    }
    else if (card && card->contains(worldCoords.first, worldCoords.second))
    {
        //std::cout << "Mouse Over" << std::endl;
    }
}

void CardDeckInputListener::endSelect(int x, int y)
{
    Card *card = _deck->get(_selectedId);
    if (_selectAndMoveInProgress)
    {
        if (card)
        {
            card->setTranslationX(card->getTranslationX() + _mouseX - _selectionStartX);
            card->setTranslationY(card->getTranslationY() + _mouseY - _selectionStartY);
        }
        else
        {
            float transX = (float)(_mouseX - _selectionStartX);
            float transY = (float)(_mouseY - _selectionStartY);
            float initialTransX = _camera->getTranslationX();
            float initialTransY = _camera->getTranslationY();
            _camera->setTranslationX(_camera->getTranslationX() - transX);
            _camera->setTranslationY(_camera->getTranslationY() - transY);
        }
        _mouseX = 0;
        _mouseY = 0;
        _selectionStartX = 0;
        _selectionStartY = 0;
        _selectAndMoveInProgress = false;
    }
    _selectedId = -1;
    _deck->stopSound(0);
}

void CardDeckInputListener::flip(int x, int y)
{
    pair<int, int> worldCoords = computeTransformation(x, y);

    auto ids = _deck->getIds();
    auto it = ids.rbegin();
    while (it != ids.rend())
    {
        Card *card = _deck->get(*it);
        if (card && card->contains(worldCoords.first, worldCoords.second))
        {
            if (card->hasFlipSide())
            {
                card->flip();
                _deck->playSound(1);
                _deck->bringToTop(*it);
            }
            break;
        }
        it++;
    }
}

void CardDeckInputListener::mouseWheelMoved(float yoffset)
{
    if (_camera)
    {
        float zoomFactorIncrement = .1f;
        float initialZoom = _camera->getZoomFactor();
        float zoom = initialZoom - yoffset * zoomFactorIncrement;
        _camera->setZoomFactor(zoom);
    }
}

bool CardDeckInputListener::isSelectAndMoveInProgress()
{
    return _selectAndMoveInProgress;
}

int CardDeckInputListener::getMouseX()
{
    return _mouseX;
}

int CardDeckInputListener::getMouseY()
{
    return _mouseY;
}

int CardDeckInputListener::getSelectedId()
{
    return _selectedId;
}