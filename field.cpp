#include "field.h"

const char ColHeaderStart = '1';
const char RowHeaderStart = 'A';

TPos TField::Pos(char row, char col) {
    if (isdigit((int)row) && isalpha((int)col))
        std::swap(row, col); 
    row = toupper(row);
    if (row < RowHeaderStart || row >= (char)(RowHeaderStart + FDim))
        return InvalidPos;
    if (col < ColHeaderStart || col >= (char)(ColHeaderStart + FDim))
        return InvalidPos;
    return TPos((unsigned)(row - RowHeaderStart), unsigned(col - ColHeaderStart));
}


void TField::Set(TPos p, bool me) {
     Field[Pos(p)] = (IPlayX ^ me) ? Cell0 : CellX;
}

void TField::UnSet(TPos p) {
     Field[Pos(p)] = CellNothing;
}


bool TField::CheckEmpty(TPos p) const {
      return  (Field[Pos(p)] == CellNothing) ;
}

bool TField::SetIfEmpty(TPos p, bool me) {
     if (!CheckEmpty(p))
        return false;
     Set(p, me);
     return true;
}

void TField::PrintRow(unsigned row) const {
     std::cout << (char)('A' + row) << " ";
     for (unsigned i = 0; i < FDim; i++)
         std::cout << Field[i + FDim*row];
     std::cout << "\n";

}


void TField::PrintMe ()  const {
     std::cout << "  123\n"; 
     for (unsigned i = 0; i < FDim; i++)
         PrintRow(i);
     std::cout << "\n";
}


bool TField::WinCheckRow(unsigned row, const char symb) const {
     for (unsigned i = 0; i < FDim; i++)
         if (Field[row*FDim + i] != symb)
      return false;
    return true;
}

bool TField::WinCheckCol(unsigned col, const char symb)  const {
     for (unsigned i = 0; i < FDim; i++)
         if (Field[i*FDim + col] != symb)
      return false;
    return true;
}

bool TField::WinCheckDiag1(const char symb) const {
     for (unsigned i =0; i < FDim; i++) 
         if (Field[i + FDim*i] != symb)
            return false;
    return true;

}

bool TField::WinCheckDiag2(const char symb) const {
     for (unsigned i =0; i < FDim; i++) 
         if (Field[FDim - i - 1 + FDim*i] != symb)
            return false;
    return true;

}

std::vector<TPos> TField::PercpCheckRow(unsigned row, const char symb) const {
    std::vector<TPos> Out;
    for (unsigned i = 0; i < FDim; i++) {
        char CellValue = Field[row*FDim + i];
        if (CellValue == CellNothing) {
            Out.push_back(TPos(row, i));
            continue;
        }
        if (CellValue != symb) {
            Out.clear();
            return Out;
        }
    }
    if (Out.size() == FDim)
        Out.clear();
    return Out;
}

std::vector<TPos> TField::PercpCheckCol(unsigned col, const char symb) const {
    std::vector<TPos> Out;
    for (unsigned i = 0; i < FDim; i++) {
        char CellValue = Field[i*FDim + col];
        if (CellValue == CellNothing) {
            Out.push_back(TPos(i, col));
            continue;
        }
        if (CellValue != symb) {
            Out.clear();
            return Out;
        }
    }
    if (Out.size() == FDim)
        Out.clear();
    return Out;
}

std::vector<TPos> TField::PercpCheckDiag1(const char symb) const {
    std::vector<TPos> Out;
    for (unsigned i = 0; i < FDim; i++) {
        char CellValue = Field[i*FDim + i];
        if (CellValue == CellNothing) {
            Out.push_back(TPos(i, i));
            continue;
        }
        if (CellValue != symb) {
            Out.clear();
            return Out;
        }
    }
    if (Out.size() == FDim)
        Out.clear();
    return Out;
}


std::vector<TPos> TField::PercpCheckDiag2(const char symb) const {
    std::vector<TPos> Out;
    for (unsigned i = 0; i < FDim; i++) {
        char CellValue = Field[FDim - i - 1 + FDim*i];
        if (CellValue == CellNothing) {
            Out.push_back(TPos(FDim - i - 1, i));
            continue;
        }
        if (CellValue != symb) {
            Out.clear();
            return Out;
        }
    }
    if (Out.size() == FDim)
        Out.clear();
    return Out;
}

TPos TField::WinIn2Turns(bool me) const {
    char SymToCheck = (IPlayX ^ me) ? Cell0 : CellX;
    std::set<TPos> WinPos;
    std::vector<TPos> VP;
    for (unsigned i = 0; i < FDim; i++) {
        VP = PercpCheckRow(i, SymToCheck);
        for (unsigned j = 0; j < VP.size(); j++)
            if (!WinPos.insert(VP[j]).second) 
                return TPos(VP[j]);
        VP = PercpCheckCol(i, SymToCheck);
        for (unsigned j = 0; j < VP.size(); j++)
            if (!WinPos.insert(VP[j]).second) 
                return TPos(VP[j]);
    }
    VP = PercpCheckDiag1(SymToCheck);
    for (unsigned j = 0; j < VP.size(); j++)
        if (!WinPos.insert(VP[j]).second) 
            return TPos(VP[j]);
    VP = PercpCheckDiag2(SymToCheck);
    for (unsigned j = 0; j < VP.size(); j++)
        if (!WinPos.insert(VP[j]).second) 
            return TPos(VP[j]);
    return InvalidPos;
}

bool TField::WinCheck(bool me) const {
    char SymToCheck = (IPlayX ^ me) ? Cell0 : CellX;
    for (unsigned i = 0; i < FDim; i++)
        if (WinCheckRow(i, SymToCheck) || 
                WinCheckCol(i, SymToCheck))
            return true;
        if (WinCheckDiag1(SymToCheck) || 
                WinCheckDiag2(SymToCheck))
        return true;
    return false;
}

