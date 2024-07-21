#pragma once

#include "CardDeckInputListener.h"


class CardDeckEventTranslator
{
private:
    int _mouseX;
    int _mouseY;
    CardDeckInputListener *_listener;

public:
    enum EventType
    {
        LeftMouseDown,
        LeftMouseUp,
        RightMouseDown,
        RightMouseUp
    };

    void registerListener(CardDeckInputListener *listener);
    void mouseMoved(int x, int y);
    void translateMouseEvent(EventType type);

};