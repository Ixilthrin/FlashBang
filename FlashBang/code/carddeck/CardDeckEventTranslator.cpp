#include "CardDeckEventTranslator.h"

void CardDeckEventTranslator::registerListener(CardDeckInputListener *listener)
{
    _listener = listener;
}

void CardDeckEventTranslator::mouseMoved(int x, int y)
{
    _listener->moveSelection(x, y);
    _mouseX = x;
    _mouseY = y;
}

void CardDeckEventTranslator::translateMouseEvent(EventType type)
{
    if (_listener && type == LeftMouseDown)
    {
        _listener->select(_mouseX, _mouseY);
    }
    else if (_listener && type == LeftMouseUp)
    {
        _listener->endSelect(_mouseX, _mouseY);
    }
    else if (_listener && type == RightMouseUp)
    {
        _listener->flip(_mouseX, _mouseY);
    }
}