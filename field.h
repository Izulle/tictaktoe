#pragma once

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include <iostream>
#include <cstring>
#include <cctype> //toupper

#include <vector>
#include <set>

const unsigned FDim = 3;
const unsigned FSize = FDim * FDim;
const char CellNothing = ' ';
const char CellX = 'X';
const char Cell0 = '0';

typedef std::pair<unsigned, unsigned> TPos;


const TPos InvalidPos = TPos(FDim, FDim);


class TField {
public:
    virtual ~TField() { }

private:
    void PrintRow(unsigned row) const;

    bool WinCheckRow(unsigned row, const char symb) const;
    bool WinCheckCol(unsigned col, const char symb) const;
    bool WinCheckDiag1(const char symb) const;
    bool WinCheckDiag2(const char symb) const;

    std::vector<TPos> PercpCheckRow(unsigned row, const char symb) const;
    std::vector<TPos> PercpCheckCol(unsigned col, const char symb) const;
    std::vector<TPos> PercpCheckDiag1(const char symb) const;
    std::vector<TPos> PercpCheckDiag2(const char symb) const;


private:
    char Field[FSize];

public:
    static unsigned Pos(TPos p) {
        return p.first*FDim + p.second;
     }
    static TPos Pos(unsigned p) {
        if (p >= FSize)
            return InvalidPos;
        return TPos(p/FDim, p%FDim);
    }

    static bool IsValidPos(TPos p) {
        return Pos(p) < FSize;
    }
    static bool IsValidPos(char row, char col) {
        return IsValidPos(Pos(row, col));
    }
    static TPos Pos(char row, char col);
    
    static TPos RandomPos() {
	 return Pos(rand() % FSize);
    }

protected:
    void Set(TPos p, bool me = true);
    void UnSet(TPos p);
    TPos WinIn2Turns(bool me) const;

    void ClearField() {
        memset(Field, CellNothing, FSize);
    }

public:
    bool IPlayX = true;

    TField() {
        srand(time(NULL));
        ClearField();
    }   
    void PrintMe() const;
    bool SetIfEmpty(TPos p, bool me = true);
    bool CheckEmpty(TPos p) const;
    bool WinCheck(bool me = true) const;
    bool CheckAllSet() const { 
        for (unsigned i = 0; i < FSize; i++)
            if (Field[i] == CellNothing)
                return false;
        return true;
    }
};



