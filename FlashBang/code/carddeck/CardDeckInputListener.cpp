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
    float transX = 0;
    float transY = 0;
    if (_camera)
    {
        zoomFactor = _camera->getZoomFactor();
        transX = _camera->getTranslationX();
        transY = -_camera->getTranslationY();
    }
    int worldX = x - 800;
    int worldY = y - 450;
    worldX = (float)worldX * zoomFactor;
    worldY = (float)worldY * zoomFactor;
    worldX += 800;
    worldY += 450;
    cout << "\n\nworldX:  " << worldX << endl;
    cout << "worldY:  " << worldY << endl;
    cout << "transX:  " << transX << endl;
    cout << "transY:  " << transY << endl;

    slideSoundPlayed = false;

    bool itemSelected = false;
    vector<int> ids = _deck->getIds();
    auto it = ids.rbegin();
    while (it != ids.rend())
    {
        Card *card = _deck->get(*it);
        if (card)
        {
            if (card->contains(worldX, worldY))
            {
                itemSelected = true;
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
    
    if (!itemSelected)
    {
        if (!_selectAndMoveInProgress)
        {
            _selectionStartX = worldX;
            _selectionStartY = worldY;
            _mouseX = worldX;
            _mouseY = worldY;
        }
        _selectAndMoveInProgress = true;
    }

    //_selectAndMoveInProgress = true;
}

void CardDeckInputListener::moveSelection(int x, int y)
{
    // TODO : This is used in several places so refactor
    // TODO : 800 and 450 are hard-coded - should get screen height & width from window
    float zoomFactor = 1;
    float transX = 0;
    float transY = 0;
    if (_camera)
    {
        zoomFactor = _camera->getZoomFactor();
        transX = _camera->getTranslationX();
        transY = -_camera->getTranslationY();
    }
    int worldX = x - 800;
    int worldY = y - 450;
    worldX = (float)worldX * zoomFactor;
    worldY = (float)worldY * zoomFactor;
    worldX += 800;
    worldY += 450;
    //cout << "\n\nworldX:  " << worldX << endl;
    //cout << "worldY:  " << worldY << endl;
    //cout << "transX:  " << transX << endl;
    //cout << "transY:  " << transY << endl;

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
        }
        else
        {
            //float transX = (_camera->getTranslationX() + _mouseX - _selectionStartX) / 1600;
            //float transY = (_camera->getTranslationY() + _mouseY - _selectionStartY) / 900;
            float transX = (float)(_mouseX - _selectionStartX);
            float transY = (float)(_mouseY - _selectionStartY);
            float initialTransX = _camera->getTranslationX();
            float initialTransY = _camera->getTranslationY();
            _camera->setTranslationX(_camera->getTranslationX() + transX);
            ///_camera->setTranslationY(_camera->getTranslationY() + transY);
            ///float finalTransX = _camera->getTranslationX();
            ///float finalTransY = _camera->getTranslationY();
            //cout << "\n\ntransX: " << initialTransX << endl;
            //cout << "transY: " << initialTransX << endl;
            //cout << "\n\ntransX: " << finalTransX << endl;
            //cout << "transY: " << finalTransX << endl;

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
    // TODO : This is used in several places so refactor
    // TODO : 800 and 450 are hard-coded - should get screen height & width from window
    float zoomFactor = 1;
    float transX = 0;
    float transY = 0;
    if (_camera)
    {
        zoomFactor = _camera->getZoomFactor();
        transX = _camera->getTranslationX() * 1600;
        transY = -_camera->getTranslationY() * 900;
    }
    int worldX = x - 800;
    int worldY = y - 450;
    worldX = (float)worldX * zoomFactor;
    worldY = (float)worldY * zoomFactor;
    worldX += 800;
    worldY += 450;
    cout << "\n\nworldX:  " << worldX << endl;
    cout << "worldY:  " << worldY << endl;
    cout << "transX:  " << transX << endl;
    cout << "transY:  " << transY << endl;

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