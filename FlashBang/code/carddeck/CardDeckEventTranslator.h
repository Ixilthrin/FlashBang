#pragma once

#include "CardDeckInputHandler.h"
#include "OverheadCamera.h"

using std::shared_ptr;

class CardDeckEventTranslator
{
private:
    double _mouseX;
    double _mouseY;
    CardDeckInputHandler * _handler;
public:

    CardDeckEventTranslator();
    enum EventType
    {
        LeftMouseDown,
        LeftMouseUp,
        RightMouseDown,
        RightMouseUp
    };

    void registerEventHandler(CardDeckInputHandler *handler);
    void mouseMoved(double x, double y);
    void translateMouseEvent(EventType type);
    void translateMouseWheelMove(double yoffset);
};