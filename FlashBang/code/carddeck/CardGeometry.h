#pragma once

#include <vector>

#include "Card.h"
#include "Converter.h"

using std::vector;

class CardGeometry
{
private:
    Card *_card;
    Converter *_converter;
public:
    CardGeometry(Card *card, Converter *converter);
    vector<float> getPositions();
    vector<float> getColors();
    vector<float> getTexCoords();
    vector<unsigned int> getIndexData();
};