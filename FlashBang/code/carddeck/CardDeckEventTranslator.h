#pragma once

#include "CardDeckInputListener.h"
#include "OverheadCamera.h"

using std::shared_ptr;

class CardDeckEventTranslator
{
private:
    double _mouseX;
    double _mouseY;
    CardDeckInputListener *_listener;
public:

    CardDeckEventTranslator();
    enum EventType
    {
        LeftMouseDown,
        LeftMouseUp,
        RightMouseDown,
        RightMouseUp
    };

    void registerListener(CardDeckInputListener *listener);
    void mouseMoved(double x, double y);
    void translateMouseEvent(EventType type);
    void translateMouseWheelMove(double yoffset);
};