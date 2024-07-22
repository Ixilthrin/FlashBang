#include "CardDeckInputListener.h"

#include <iostream>
#include <vector>
using std::vector;

CardDeckInputListener::CardDeckInputListener(OverheadCamera *camera)
{
    _selectAndMoveInProgress = false;
    _mouseX = 0;
    _mouseY = 0;
    _selectionStartX = 0;
    _selectionStartY = 0;
    _selectedId = -1;
    _camera = camera;
}

bool slideSoundPlayed = true;

void CardDeckInputListener::select(int x, int y)
{
    // TODO : This is used in several places so refactor
    // TODO : 800 and 450 are hard-coded - should get screen height & width from window
    float zoomFactor = 1;
    if (_camera)
        zoomFactor = _camera->getZoomFactor();
    int worldX = x - 800;
    int worldY = y - 450;
    worldX = (float)worldX * zoomFactor;
    worldY = (float)worldY * zoomFactor;
    worldX += 800;
    worldY += 450;

    slideSoundPlayed = false;

    vector<int> ids = _deck->getIds();
    auto it = ids.rbegin();
    while (it != ids.rend())
    {
        Card *card = _deck->get(*it);
        if (card)
        {
            if (card->contains(worldX, worldY))
            {
                if (!_selectAndMoveInProgress)
                {
                    _selectionStartX = worldX;
                    _selectionStartY = worldY;
                    _mouseX = worldX;
                    _mouseY = worldY;
                }
                _selectedId = *it;
                _selectAndMoveInProgress = true;
                break;
            }
        }
        it++;
    }
    //_selectAndMoveInProgress = true;
}

void CardDeckInputListener::moveSelection(int x, int y)
{
    // TODO : This is used in several places so refactor
    // TODO : 800 and 450 are hard-coded - should get screen height & width from window
    float zoomFactor = 1;
    if (_camera)
        zoomFactor = _camera->getZoomFactor();
    int worldX = x - 800;
    int worldY = y - 450;
    worldX = (float)worldX * zoomFactor;
    worldY = (float)worldY * zoomFactor;
    worldX += 800;
    worldY += 450;

    Card *card = _deck->get(_selectedId);
    if (_selectAndMoveInProgress)
    {
        _mouseX = worldX;
        _mouseY = worldY;

        if (card)
        {
            if (!slideSoundPlayed)
            {
                _deck->playSound(0);
                slideSoundPlayed = true;
            }

            _deck->bringToTop(_deck->getId(card));
        }
    }
    else if (card && card->contains(worldX, worldY))
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
            _mouseX = 0;
            _mouseY = 0;
            _selectionStartX = 0;
            _selectionStartY = 0;
        } 
    }
    _selectAndMoveInProgress = false;
    _selectedId = -1;
    _deck->stopSound(0);
}

void CardDeckInputListener::flip(int x, int y)
{
    // TODO : This is used in several places so refactor
    // TODO : 800 and 450 are hard-coded - should get screen height & width from window
    float zoomFactor = 1;
    if (_camera)
        zoomFactor = _camera->getZoomFactor();
    int worldX = x - 800;
    int worldY = y - 450;
    worldX = (float)worldX * zoomFactor;
    worldY = (float)worldY * zoomFactor;
    worldX += 800;
    worldY += 450;

    auto ids = _deck->getIds();
    auto it = ids.rbegin();
    while (it != ids.rend())
    {
        Card *card = _deck->get(*it);
        if (card && card->contains(worldX, worldY))
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