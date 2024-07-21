#pragma once

#include "CardDeck.h"
#include "Card.h"

class CardDeckInputListener
{
private:
    int _mouseX;
    int _mouseY;
    bool _selectAndMoveInProgress;
    CardDeck *_deck;
    int _selectionStartX;
    int _selectionStartY;
    int _selectedId;

public:
    CardDeckInputListener();
    inline void setDeck(CardDeck *deck)
    {
        _deck = deck;
    }

    inline int getMovementX()
    {
        return _mouseX - _selectionStartX;
    }

    inline int getMovementY()
    {
        return _mouseY - _selectionStartY;
    }

    void select(int x, int y);
    void moveSelection(int x, int y);
    void endSelect(int x, int y);
    void flip(int x, int y);

    bool isSelectAndMoveInProgress();
    int getMouseX();
    int getMouseY();
    int getSelectedId();
};
