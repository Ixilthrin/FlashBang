#pragma once

#include <vector>
#include "Card.h"
#include "Converter.h"
#include "XYZColorTex.h"

using std::vector;

class CardGeometry
{
private:
    Card *_card;
    Converter *_converter;
public:
    CardGeometry(Card *card, Converter *converter);
    vector<XYZColorTex> getVertices();
    vector<unsigned int> getIndexData();
};