#pragma once

#include "field.h"

const TPos UpperLeft = TPos(0,0);
const TPos UpperRight = TPos(0,FDim -1);
const TPos LowerLeft = TPos(FDim -1 , 0);
const TPos LowerRight = TPos(FDim -1 ,FDim -1);


const TPos Middle = TPos(1,1);

const TPos Upper = TPos(0,1);
const TPos Left = TPos(1,0);
const TPos Lower = TPos(2,1);
const TPos Right = TPos(1,2);


class TStrat : public TField {
public:
    TStrat() : TField(), Turn(0) {}  
    virtual ~TStrat() {};
    bool WinOrPreventingEnemyFromWinning();
    virtual void MyNextTurn() = 0;
    bool RandomTurn(bool me = true);
    void Clear() {
         Turn = 0; ClearField();
    }
public:
    size_t Turn = 0;
};


class TOlgaStrat : public TStrat {
    // see http://xkcd.com/832/
public:
    virtual ~TOlgaStrat() {}
    virtual void MyNextTurn();
protected:
    void FirstTurn();
    void SecondTurnX();
    void SecondTurn0();
    void OtherTurn();
};

class TNaomiStrat : public TOlgaStrat {
public:
    virtual ~TNaomiStrat() {}
    virtual void MyNextTurn() override;
private:
    unsigned Randomness = 7;
    void FirstTurn();
};

class TRomeoStrat: public TStrat {
public:
    virtual ~TRomeoStrat() {}
    virtual void MyNextTurn() override {
	RandomTurn();
    }
};

