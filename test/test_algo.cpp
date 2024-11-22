
#include <cassert>

int add(int a, int b);

int main() {
    assert(add(2, 3) == 5);
    assert(add(-1, 1) == 0);


    Model chess;
    Presentation pres;
    ClassAlgorithm algo;
    chess.load("Chess.cfg");
    Move w=algo.nextMove(chess, Player::White);
    assert(w==Move("tw1:h1"));
    pres.show(chess);
    Move b=algo.nextMove(chess, Player::Black);
    pres.show(chess);
    assert(w==Move("tb1:h1"));
    return 0;
}