#ifndef TEAREADER_H
#define TEAREADER_H

#include <vector>
#include <string>
#include "TEASYMBOL.H"

class TeaReader
{
private:
    vector<TeaSymbol*> database;

public:
    TeaReader();
    ~TeaReader();

    bool loadData(string filename);
    TeaSymbol* getRandomSymbol();
};

#endif
