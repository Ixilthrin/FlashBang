#include "CardDeckEventTranslator.h"

CardDeckEventTranslator::CardDeckEventTranslator()
{
}

void CardDeckEventTranslator::registerEventHandler(CardDeckInputHandler *handler)
{
    _handler = handler;
}

void CardDeckEventTranslator::mouseMoved(double x, double y)
{
    _handler->moveSelection(x, y);
    _mouseX = x;
    _mouseY = y;
}

void CardDeckEventTranslator::translateMouseEvent(EventType type)
{
    if (_handler && type == LeftMouseDown)
    {
        _handler->select(_mouseX, _mouseY);
    }
    else if (_handler && type == LeftMouseUp)
    {
        _handler->endSelect(_mouseX, _mouseY);
    }
    else if (_handler && type == RightMouseUp)
    {
        _handler->flip(_mouseX, _mouseY);
    }
}

void CardDeckEventTranslator::translateMouseWheelMove(double yoffset)
{
    _handler->mouseWheelMoved(yoffset);
}