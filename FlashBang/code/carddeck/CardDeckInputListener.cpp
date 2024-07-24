#include "CardDeckInputListener.h"

#include <iostream>
#include <vector>

using std::make_pair;
using std::vector;

CardDeckInputListener::CardDeckInputListener()
{
    _selectAndMoveInProgress = false;
    _initialSelectedMouseWorldLocation = make_pair<int, int>(0, 0);
    _mouseWorldLocation = make_pair<int, int>(0, 0);
    _initialSelectedMouseWorldLocation = make_pair<int, int>(0, 0);
    _previousMouseWorldLocation = make_pair<int, int>(0, 0);
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
    if (_selectAndMoveInProgress)
        return;

    pair<int, int> worldCoords = computeTransformation(x, y);

    _soundPlayed = false;

    vector<int> ids = _deck->getIds();
    auto it = ids.rbegin();
    while (it != ids.rend())
    {
        Card *card = _deck->get(*it);
        if (card)
        {
            if (card->contains(worldCoords.first, worldCoords.second))
            {
                _selectedId = *it;
                break;
            }
        }
        it++;
    }

    _selectionStartX = worldCoords.first;
    _selectionStartY = worldCoords.second;
    _mouseWorldLocation.first = worldCoords.first;
    _mouseWorldLocation.second = worldCoords.second;
    _previousMouseWorldLocation = _mouseWorldLocation;
    _initialSelectedMouseWorldLocation = _mouseWorldLocation;
    _selectAndMoveInProgress = true;
}

void CardDeckInputListener::moveSelection(int x, int y)
{
    pair<int, int> worldCoords = computeTransformation(x, y);

    Card *card = _deck->get(_selectedId);
    if (_selectAndMoveInProgress)
    {
        _mouseWorldLocation = worldCoords;

        if (card)
        {
            if (!_soundPlayed)
            {
                _deck->playSound(0);
                _soundPlayed = true;
            }

            _deck->bringToTop(_deck->getId(card));

            card->setTranslationX(card->getTranslationX() + _mouseWorldLocation.first - _previousMouseWorldLocation.first);
            card->setTranslationY(card->getTranslationY() + _mouseWorldLocation.second - _previousMouseWorldLocation.second);
        }
        else
        {
            float transX = (float)(_mouseWorldLocation.first - _initialSelectedMouseWorldLocation.first);
            float transY = (float)(_mouseWorldLocation.second - _initialSelectedMouseWorldLocation.second);
            _camera->setTranslationX(_camera->getTranslationX() - transX);
            _camera->setTranslationY(_camera->getTranslationY() - transY);
        }
    }
    else if (card && card->contains(worldCoords.first, worldCoords.second))
    {
        //std::cout << "Mouse Over" << std::endl;
    }
    _previousMouseWorldLocation = _mouseWorldLocation;
}

void CardDeckInputListener::endSelect(int x, int y)
{
    Card *card = _deck->get(_selectedId);
    if (_selectAndMoveInProgress)
    {
        _mouseWorldLocation.first = 0;
        _mouseWorldLocation.second = 0;
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

int CardDeckInputListener::getSelectedId()
{
    return _selectedId;
}