
#include <cassert>
#include "model.h"

using namespace chess;

int main() {
    


    Model c;
    auto loaded = c.load("C:\\dev\\Chess\\test\\Chess.cfg");
    assert(loaded==true);
    auto queen=c.GetQueen(Color::white);
    assert(queen!=nullptr);
   /* Presentation pres;
    ClassAlgorithm algo;
    
    Move w=algo.nextMove(chess, Player::White);
    assert(w==Move("whiteRook:h1"));
    pres.show(chess);
    Move b=algo.nextMove(chess, Player::Black);
    pres.show(chess);
    assert(w==Move("blackRook:h8"));*/
    return 0;
}