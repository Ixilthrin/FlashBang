#include "CardDeck.h"
#include "CardGeometry.h"

#include "ImageReader.h"
#include <filesystem>
namespace fs = std::filesystem;

#include <iostream>
#include <stdlib.h> 
#include <time.h>  

CardDeck::CardDeck() : CardDeck(nullptr) // c++11 - constructor call
{
}

CardDeck::CardDeck(Converter *converter)
{
    _converter = converter;
    _audioPlayer = new SoundStore();
    _audioPlayer->setup();
    _scale = 1.0f;
    _initialX = 50;
    _initialY = 50;
}

void CardDeck::setConverter(Converter *converter)
{
    _converter = converter;
}

void CardDeck::setScale(float scale)
{
    if (scale > 0)
        _scale = scale;
}

void CardDeck::setInitialPosition(int x, int y)
{
    _initialX = x;
    _initialY = y;
}

void CardDeck::setUpFromBaseDir(string baseDir)
{
    addCardsFromDirectory(baseDir);
    shuffle();


    for (auto const& id : getIds())
    {
        auto geometry = new CardGeometry(get(id), _converter);
        addGeometry(id, geometry);
    }
}

void CardDeck::add(int id, Card *card)
{
    _cards.insert(pair<int, Card*>(id, card));
    _ids.push_back(id);
}

void CardDeck::removeCard(int id)
{
    _cards.erase(id);
    _ids.erase(std::remove(_ids.begin(), _ids.end(), id));
}

void CardDeck::addGeometry(int id, CardGeometry *geometry)
{
    _geometry.insert(pair<int, CardGeometry*>(id, geometry));
}

void CardDeck::addImageData(int id, CardImageData *data)
{
    _imageData.insert(pair<int, CardImageData*>(id, data));
}

int CardDeck::size()
{
    return _cards.size();
}

int CardDeck::numberOfCardSides()
{
    int numberOfSides = 0;
    for (auto const& cardPair : _cards)
    {
        ++numberOfSides;
        auto card = cardPair.second;
        if (card->hasFlipSide())
            ++numberOfSides;
    }
    return numberOfSides;
}

vector<int> CardDeck::getIds()
{
    return _ids;
}

Card *CardDeck::get(int id)
{
    return _cards[id];
}

int CardDeck::getId(Card *card)
{
    for (auto const& id : _ids)
    {
        if (get(id)->getImagePath() == card->getImagePath())
            return id;
    }

    return 0;
}

CardGeometry* CardDeck::getGeometry(int id)
{
    return _geometry[id];
}

CardImageData* CardDeck::getImageData(int id)
{
    return _imageData[id];
}

void CardDeck::bringToTop(int id)
{
    auto position = std::find(_ids.begin(), _ids.end(), id);
    if (position != _ids.end())
    {
        _ids.erase(position);
    }
    // Possibly counter-intuitive, but the last item
    // will draw on top of other items.
    _ids.push_back(id);
}

void CardDeck::addImageCard(int id, int x, int y,
    string imageFile, string backImageFile)
{
    CardImageData *imageData = new CardImageData(imageFile);
    imageData->setBackPath(backImageFile);

    int width = imageData->getImageReader()->getWidth();
    int height = imageData->getImageReader()->getHeight();

    width = (float)width * _scale;
    height = (float)height * _scale;

    int maxWidth = 500 * _scale;
    if (width > maxWidth)
    {
        height = (float)height / ((float)width / (float)maxWidth);
        width = maxWidth;
    }

    Card *card = new Card{ width, height, x, y, imageFile };

    if (backImageFile.length() > 0)
    {
        card->setFlippedImagePath(backImageFile);
    }

    add(id, card);

    addImageData(id, imageData);
}

void CardDeck::addCards(vector<string> filenames)
{
    int id = 0;
    int deltaX = 0, deltaY = 0;

    for (auto i = filenames.begin(); i < filenames.end(); ++i)
    {
        string path = "../FlashBangProject/resources/" + *i;
        addImageCard(id, _initialX + deltaX, _initialY + deltaY, path, "");
        ++id;
        deltaX += 1;
        deltaY += 1;
    }
}

void CardDeck::addCardsFromDirectory(string basepath)
{
    int x = _initialX;
    int y = _initialY;
    int id = 0;

    vector<string> imageFrontNames;
    vector<string> imageBackNames;
    string extension = "";
    for (const auto &entry : fs::directory_iterator(basepath))
    {
        string path{ entry.path().u8string() };
        int pos = path.find(".png", 0);
        if (pos > 0)
        {
            extension = ".png";
        }

        // The stb image loader doesn't always work for jpg files
        // so disabling jpg support for now
        //else
        //{
        //    pos = path.find(".jpg", 0);
        //    if (pos > 0)
        //        extension = ".jpg";
        //}
            
        if (pos > 0)
        {
            auto substring = path.substr(0, path.length() - extension.length());
            int posBack = substring.find("-back", 0);
            if (posBack > 0)
                imageBackNames.push_back(substring);
            else
                imageFrontNames.push_back(substring);
        }
    }

    map<string, string> frontBackMap;

    for (const auto &front : imageFrontNames)
    {
        for (const auto &back : imageBackNames)
        {
            if (front == back.substr(0, front.length()))
                frontBackMap.insert(pair<string, string>(front, back));
        }
    }

    for (const auto &name : imageFrontNames)
    {
        bool hasBack = frontBackMap.count(name) > 0;
        string backPath;
        if (hasBack)
        {
            backPath = frontBackMap[name] + extension;
        }
        addImageCard(id, x, y, name + extension, backPath);
        ++id;
        x += 1;
        y += 1;
    }
}

void CardDeck::shuffle()
{    
    if (_ids.size() < 2)
        return;

    srand(time(NULL));

    // randomly pick 2 and swap
    for (int i = 0; i < 100; ++i)
    {
        int max = _ids.size();
        int index1 = rand() % max;
        int index2 = rand() % max;
        if (index1 >= max || index2 >= max)
            continue;
        int tempId = _ids[index1];
        _ids[index1] = _ids[index2];
        _ids[index2] = tempId;
    }

    int x = _initialX;
    int y = _initialY;

    for (auto const& id : _ids)
    {
        auto card = _cards[id];
        card->setTranslationX(x);
        card->setTranslationY(y);
        x += 1;
        y += 1;
    }
}

void CardDeck::getIndexData(vector<unsigned int> &indexData,
                            map<int, int> &indexOffsets)
{
    unsigned int baseIndex = 0;

    int currentIndexOffset = 0;
    for (auto const &id : getIds())
    {
        auto geom = getGeometry(id);
        auto currentIndices = geom->getIndexData();
        indexOffsets.insert(pair<int, int>(id, currentIndexOffset));
        currentIndexOffset += currentIndices.size();
        for (auto const &value : currentIndices)
        {
            auto index = value + baseIndex;
            indexData.push_back(index);
        }
        baseIndex += geom->getPositions().size() / 3;
    }
}

void CardDeck::playSound(int index)
{
    _audioPlayer->play(index);
}

void CardDeck::stopSound(int index)
{
    _audioPlayer->stop(index);
}

