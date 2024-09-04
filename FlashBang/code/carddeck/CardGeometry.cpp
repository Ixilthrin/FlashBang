#include "CardGeometry.h"

#include <iostream>

using std::cout;
using std::endl;

CardGeometry::CardGeometry(Card *card, Converter *converter)
{
    _card = card;
    _converter = converter;
}

vector<XYZColorTex> CardGeometry::getVertices()
{
    float x1 = _converter->screenXToNDC(0);
    float y1 = _converter->screenYToNDC(0);
    float x2 = _converter->screenXToNDC(_card->getWidth());
    float y2 = _converter->screenYToNDC(_card->getHeight());

    vector<XYZColorTex> vertexData
    {
        XYZColorTex {
            x1,
            y1,
            0.0f,
            .82f, 0.41f, 0.14f,
            0.0f, 0.0f,
        },

        XYZColorTex {
            x1,
            y2,
            0.0f,
            .82f, 0.41f, 0.14f,
            0.0f, 1.0f
        },

        XYZColorTex {
            x2,
            y2,
            0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 1.0f,
        },

        XYZColorTex {
            x2,
            y1,
            0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f
        }
    };

    return vertexData;
}

vector<unsigned int> CardGeometry::getIndexData()
{
    std::vector<unsigned int> indexData
    {
        0, 1, 2,
        2, 3, 0
    };
    return indexData;
}