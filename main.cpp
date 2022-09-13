#include <iostream>
#include <cstring>

#include  <sstream>
#include <algorithm>
#include "strat.h"

const unsigned LastTurn = 4;

enum class TOpponents { Naomi, Olga };

enum class TCommands { Move , PlayX, Play0, End };

typedef std::pair<TCommands, TPos> TUserRequest;

class TPlay {
private:
        TStrat *Field = nullptr;
        unsigned Played = 0;
        unsigned Won = 0;
        unsigned Lost = 0;
        unsigned Tie = 0;

        TUserRequest AskWhatNext();
        void AskXO();
        TUserRequest ParseInput(const char* str);
        bool IsValidInput(TUserRequest& req);
        void GoodBye();
        void PlayParty();
        void ChooseOpponent();

public:
        TPlay() {}
        ~TPlay() {
            if (Field)
                delete Field;
        }
        void DoPlay();
};

TUserRequest TPlay::ParseInput(const char* str) {
    std::istringstream Is(str);
    char row, col;
    Is >> row >> col;
    if (Is.fail()) 
        return TUserRequest(TCommands::End,InvalidPos);
    return TUserRequest(TCommands::Move,TField::Pos(row, col));
}


bool TPlay::IsValidInput(TUserRequest& req) {
     if( req.first != TCommands::Move)
         return false;
    if (!TField::IsValidPos(req.second))
       return false;
    return true;
}

//const size_t InpSize = 20;

TUserRequest TPlay::AskWhatNext() {
    TUserRequest  WhatNext;
    std::string UserInput;
    bool ValidInput = false;
    while(true) {
            std::cout << "Your Move ?\n";
            std::getline(std::cin, UserInput);
            WhatNext = ParseInput(UserInput.c_str());
            ValidInput = IsValidInput(WhatNext);
            if (ValidInput) {
                  if (Field->CheckEmpty(WhatNext.second))
                     return WhatNext;
                std::cout << "Cell is not empty. Please choose another\n";
             }
             else 
                std::cout << "Mmm? Enter empty cell postion\n";
        }
}

bool AskYN(std::string question, char Yes, char No) {
    std::string UserInput;
    while(true) {
    std::cout << question << "\n";
        std::getline(std::cin, UserInput);
        for (size_t i = 0; i < UserInput.size(); i++) {
            if (toupper(UserInput[i]) == Yes)
               return true;
            if (toupper(UserInput[i]) == No)
               return false;
        }
        std::cout << "Mmm? Please enter " << Yes << " or " << No << "\n"; 
    }
}

void TPlay::AskXO() {
    Field->IPlayX = AskYN("You prefer to play X or 0?", Cell0, CellX);
}

void TPlay::GoodBye() {
    std::cout << "Played:" << Played << "\n";
    std::cout << "Won:" << Won << "\n";
    std::cout << "Lost:" << Lost << "\n";
    std::cout << "Tie:" << Tie << "\n";
    std::cout << "See You later\n";
}

void TPlay::ChooseOpponent() {
    std::cout << "Choose opponent: Naomi (enter N), Romeo (R) or Olga (O)\n";
    std::string UserInput;
    while(true) {
        std::getline(std::cin, UserInput);
        for (size_t i = 0; i < UserInput.size(); i++) 
            switch (toupper(UserInput[i])) {
                case 'N':
                    Field = new TNaomiStrat;
                    return;
                case 'R':
                    Field = new TRomeoStrat;
                    return;
                case 'O':
                    Field = new TOlgaStrat;
                    return;
                }
        std::cout << "Mmm? Please enter N for Naomi, R for Romeo or O for Olga\n"; 
    }
}

void TPlay::DoPlay() {
    std::cout << "Welcome to TicTacToe!\n";
    ChooseOpponent();
    while (true) {
        PlayParty();
        if (! AskYN("Once more? ", 'Y', 'N')) 
            break;
        Field->Clear();
    }
    GoodBye();
    return;
}

void TPlay::PlayParty() {
    Played++;
    TUserRequest WhatNext;
    AskXO();
    if (Field->IPlayX)
       Field->MyNextTurn();

    Field->PrintMe();
    while (true) {
        WhatNext = AskWhatNext();
        Field->SetIfEmpty(WhatNext.second, false);
        if (Field->WinCheck(false)) {
           Field->PrintMe();
           Won++;
           std::cout << "Grates! You WON!\n";
           return;
        }
        Field->MyNextTurn();
        bool IWon = Field->WinCheck(true);
        if (!IWon && !Field->IPlayX && Field->Turn == LastTurn) {
           std::cout << "Autofilling last cell\n";
           Field->RandomTurn(false);
        }
        Field->PrintMe();
        if (IWon) {
           Lost++;
           std::cout << "Sorry, You lost. Better luck next time\n";
           return;
        }
        if (Field->CheckAllSet()) {
           Tie++;
           std::cout << "TIE!\n";
           return;
           }
    }   
}
        
int main() {
    TPlay Play;
    Play.DoPlay();
    return 0;
}
