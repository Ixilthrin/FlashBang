#pragma once

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include "Card.h"
#include "CardGeometry.h"
#include "CardImageData.h"
#include "Converter.h"
#include "SoundStore.h"

using std::endl;
using std::cout;
using std::cin;
using std::cerr;
using std::string;
using std::ifstream;
using std::stringstream;
using std::map;
using std::pair;
using std::vector;

class CardDeck
{
private:
    vector<int> _ids;
    map<int, Card*> _cards;
    map<int, CardGeometry*> _geometry;
    map<int, CardImageData*> _imageData;
    Converter *_converter;
    SoundStore *_audioPlayer;
    float _scale;
    int _initialX;
    int _initialY;
public:
    CardDeck();
    CardDeck(Converter *converter);
    void setConverter(Converter *converter);
    void setScale(float scale);
    void setInitialPosition(int x, int y);
    void setUpFromBaseDir(string baseDir);
    void add(int id, Card *card);
    void removeCard(int id);
    void addGeometry(int id, CardGeometry *geometry);
    void addImageData(int id, CardImageData *data);
    int size();
    int numberOfCardSides();
    vector<int> getIds();
    Card *get(int id);
    int getId(Card *card);
    CardGeometry *getGeometry(int id);
    CardImageData *getImageData(int id);
    void bringToTop(int id);
    void addImageCard(int id, int x, int y, 
        string imagePath, string backImagePath);
    void addCards(vector<string> filenames);
    void addCardsFromDirectory(string basepath);
    void shuffle();
    void getIndexData(vector<unsigned int> &indexData,
                      map<int, int> &indexOffsets);
    void playSound(int index);
    void stopSound(int index);
};
