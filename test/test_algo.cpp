
#include <gtest/gtest.h>
#include "model.h"

using namespace chess;

TEST(SampleTest, Example)
{

    Model c;
    auto loaded = c.load("C:\\dev\\Chess\\test\\Chess.cfg");
    EXPECT_TRUE(loaded);
    auto queen = c.GetQueen(Color::white);
    auto piece = c.GetPiece(13);

    EXPECT_TRUE(queen != nullptr);
    EXPECT_TRUE(piece != nullptr);
    /* Presentation pres;
     ClassAlgorithm algo;

     Move w=algo.nextMove(chess, Player::White);
     assert(w==Move("whiteRook:h1"));
     pres.show(chess);
     Move b=algo.nextMove(chess, Player::Black);
     pres.show(chess);
     assert(w==Move("blackRook:h8"));*/
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}