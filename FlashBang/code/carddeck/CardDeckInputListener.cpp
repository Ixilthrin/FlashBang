#include "CardDeckInputListener.h"

#include <iostream>
#include <vector>
using std::vector;

CardDeckInputListener::CardDeckInputListener()
{
    _selectAndMoveInProgress = false;
    _mouseX = 0;
    _mouseY = 0;
    _selectionStartX = 0;
    _selectionStartY = 0;
    _selectedId = -1;
}

bool slideSoundPlayed = true;

void CardDeckInputListener::select(int x, int y)
{
    slideSoundPlayed = false;

    vector<int> ids = _deck->getIds();
    auto it = ids.rbegin();
    while (it != ids.rend())
    {
        Card *card = _deck->get(*it);
        if (card && card->contains(x, y))
        {
            if (!_selectAndMoveInProgress)
            {
                _selectionStartX = x;
                _selectionStartY = y;
                _mouseX = x;
                _mouseY = y;
            }
            _selectedId = *it;
            _selectAndMoveInProgress = true;
            break;
        }
        it++;
    }
}

void CardDeckInputListener::moveSelection(int x, int y)
{
    Card *card = _deck->get(_selectedId);
    if (_selectAndMoveInProgress)
    {
        _mouseX = x;
        _mouseY = y;

        if (!slideSoundPlayed)
        {
            _deck->playSound(0);
            slideSoundPlayed = true;
        }

        _deck->bringToTop(_deck->getId(card));
    }
    else if (card && card->contains(x, y))
    {
        //std::cout << "Mouse Over" << std::endl;
    }
}

void CardDeckInputListener::endSelect(int x, int y)
{
    Card *card = _deck->get(_selectedId);
    if (_selectAndMoveInProgress && card)
    {
        card->setTranslationX(card->getTranslationX() + _mouseX - _selectionStartX);
        card->setTranslationY(card->getTranslationY() + _mouseY - _selectionStartY);
        _mouseX = 0;
        _mouseY = 0;
        _selectionStartX = 0;
        _selectionStartY = 0;
    }
    _selectAndMoveInProgress = false;
    _selectedId = -1;
    _deck->stopSound(0);
}

void CardDeckInputListener::flip(int x, int y)
{
    auto ids = _deck->getIds();
    auto it = ids.rbegin();
    while (it != ids.rend())
    {
        Card *card = _deck->get(*it);
        if (card && card->contains(x, y))
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