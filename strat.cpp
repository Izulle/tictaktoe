#include "strat.h"

#include <stdexcept>

bool TStrat::WinOrPreventingEnemyFromWinning() {
    TPos TheyMightWin = InvalidPos;
    for (size_t i = 0; i < FDim; i++) 
        for (size_t j = 0; j < FDim; j++) {
            TPos P = TPos(i, j);
            if (!CheckEmpty(P))
                continue;
            Set(P);
            if (WinCheck())
                return true;
            Set(P, false);
            if (WinCheck(false))
                TheyMightWin = P;
            UnSet(P);
        }
    if (TheyMightWin == InvalidPos)
        return false;
    Set(TheyMightWin, true);
    return true;
}

bool TStrat::RandomTurn(bool me) {
    if (CheckAllSet())
        return false;
    while(true) 
        if (SetIfEmpty(RandomPos(), me))
           return true;
}

// ========= Olga =============

void TOlgaStrat::FirstTurn(){
    if (IPlayX) 
        Set(UpperLeft);
    else
        SetIfEmpty(Middle) ||
            SetIfEmpty(UpperLeft);
}

void TOlgaStrat::SecondTurnX() {
    if (! CheckEmpty(Middle)) {
        Set(LowerRight); // LowerLeft ???
        return;
    }
    if (CheckEmpty(UpperRight) && CheckEmpty(LowerLeft) && CheckEmpty(LowerRight))
        Set(Middle);
    else
        SetIfEmpty(UpperRight) || 
            SetIfEmpty(LowerLeft);
}  

void TOlgaStrat::SecondTurn0() {
    if (WinOrPreventingEnemyFromWinning())
        return;
    if (! CheckEmpty(UpperLeft)) {
        SetIfEmpty(LowerRight) || 
            SetIfEmpty(Upper);  // actually can put random { upper, lower, left, right }
        return;
    }
    if (! CheckEmpty(UpperRight)){
        SetIfEmpty(LowerLeft) || 
            SetIfEmpty(Upper);
        return;
    }
    if (! CheckEmpty(LowerLeft)){
        SetIfEmpty(UpperRight) || 
            SetIfEmpty(Upper);
        return;
    }
    if (! CheckEmpty(LowerRight)){
        SetIfEmpty(UpperLeft) || 
            SetIfEmpty(Upper);
        return;
    }
    if (! CheckEmpty(Upper) && ! CheckEmpty(Lower)) {
        Set(Left);
        return;
    }
    if (! CheckEmpty(Left) && ! CheckEmpty(Right)) {
        Set(Upper);
        return;
    }
    if (! CheckEmpty(Upper)) {
        Set(UpperLeft);
        return;
    }
    Set(LowerLeft);
}


void TOlgaStrat::OtherTurn() {
   if (WinOrPreventingEnemyFromWinning())
       return;
    TPos P = WinIn2Turns(true);
    if (P == InvalidPos) 
       P =  WinIn2Turns(false);
    if (P != InvalidPos) {
       Set(P);
       return;
    }
    if (! RandomTurn())
        throw std::logic_error("cannot generate any move");
}
void TOlgaStrat::MyNextTurn() {
    switch (Turn) {
        case 0:
            FirstTurn();
            break;
        case 1:
            if (IPlayX)
                SecondTurnX();
            else
                SecondTurn0();
            break;
        default:
            OtherTurn();
    }
    Turn++;
}

/// ==================== Naomi

void TNaomiStrat::FirstTurn() {
   unsigned Rand = rand() % 5;
	std::cout << "FT rand = " << Rand << "\n";
    switch (Rand) {
        case 0:
            if (SetIfEmpty(Middle))
                return;
        case 1:
            if (SetIfEmpty(UpperLeft))
                return;
        case 2:
            if (SetIfEmpty(UpperRight))
                return;
        case 3:
            if (SetIfEmpty(LowerLeft))
                return;
        case 4:
            if (SetIfEmpty(LowerRight))
                return;
    }
    Set(Middle);
}


void TNaomiStrat::MyNextTurn() {
    if (Turn == 0) {
        FirstTurn();
        Turn++;
        return;
    }
    if (rand() % Randomness == 0) {
        RandomTurn();
        Turn++;
        return;
    }
    
    OtherTurn();
    Turn++;
}

