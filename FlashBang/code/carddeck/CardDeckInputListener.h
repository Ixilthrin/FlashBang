#pragma once

#include "AppContext.h"
#include "CardDeck.h"
#include "Card.h"
#include "OverheadCamera.h"

using std::pair;
using std::shared_ptr;

class CardDeckInputListener
{
private:
    int _mouseX;
    int _mouseY;
    bool _selectAndMoveInProgress;
    CardDeck *_deck;
    OverheadCamera* _camera;
    int _selectionStartX;
    int _selectionStartY;
    int _selectedId;
    shared_ptr<AppContext> _context;
    bool _soundPlayed;

    pair<int, int> computeTransformation(int x, int y);

public:
    CardDeckInputListener(shared_ptr<AppContext> context, OverheadCamera *camera);
    CardDeckInputListener();

    inline void setDeck(CardDeck *deck)
    {
        _deck = deck;
    }
    inline void setCamera(OverheadCamera* camera)
    {
        _camera = camera;
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
    void mouseWheelMoved(float yoffset);

    bool isSelectAndMoveInProgress();
    int getMouseX();
    int getMouseY();
    int getSelectedId();

};
