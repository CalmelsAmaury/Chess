
#include <cassert>
#include "model.h"

using namespace chess;

int main() {
    


    Model c;
    auto loaded = c.load("Chess.cfg");
    assert(loaded==true);
   /* Presentation pres;
    ClassAlgorithm algo;
    
    Move w=algo.nextMove(chess, Player::White);
    assert(w==Move("tw1:h1"));
    pres.show(chess);
    Move b=algo.nextMove(chess, Player::Black);
    pres.show(chess);
    assert(w==Move("tb1:h1"));*/
    return 0;
}