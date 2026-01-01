#include "TEAREADER.H"
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <SFML/Graphics.hpp>

TeaReader::TeaReader()
{
    srand(time(0));
}

TeaReader::~TeaReader()
{
    for (TeaSymbol* sym : database)
    {
        delete sym;
    }
    database.clear();
}

bool TeaReader::loadData(string filename)
{
    ifstream file(filename);
    if (!file.is_open()) return false;

    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        string nameRaw, meaningRaw, imgPath;

        if (getline(ss, nameRaw, '|') &&
            getline(ss, meaningRaw, '|') &&
            getline(ss, imgPath))
        { 

            sf::String nameUtf8 = sf::String::fromUtf8(nameRaw.begin(), nameRaw.end());
            sf::String meaningUtf8 = sf::String::fromUtf8(meaningRaw.begin(), meaningRaw.end());

            TeaSymbol* newSym = new TeaSymbol(nameUtf8, meaningUtf8, imgPath);
            database.push_back(newSym);
        }
    }
    file.close();
    return !database.empty();
}

TeaSymbol* TeaReader::getRandomSymbol()
{
    if (database.empty()) return nullptr;
    int index = rand() % database.size();
    return database[index];
}