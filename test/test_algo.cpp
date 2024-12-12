
#include <gtest/gtest.h>
#include "model.h"
#include "ChessAlgorithm.h"
using namespace chess;

TEST(SampleTest, Example)
{

    Model c;
    auto loaded = c.load("Chess.cfg");
    EXPECT_TRUE(loaded);
    auto queen = c.GetQueen(Color::white);
    auto piece = c.GetPiece(13);

    EXPECT_TRUE(queen != nullptr);
    EXPECT_TRUE(piece != nullptr);

    auto save = c.save();
    EXPECT_TRUE(save);

    auto board = c.CreateBoard();

    // TEST du pion sans case en diagonal à manger :
    auto pos = Position(1, 'a');
    piece = Piece::getPiece(board, pos);
    EXPECT_TRUE(piece != nullptr);

    auto nextPos = piece->nextPossibleMoves(board);
    EXPECT_TRUE(nextPos.size() == 2);
    EXPECT_TRUE(nextPos[0].row_ == 2);
    EXPECT_TRUE(nextPos[0].col_ == 0);
    
    // TEST pion capture en diagonal :
    auto pos1 = Position(1, 'g');
    piece = Piece::getPiece(board, pos1);
    EXPECT_TRUE(piece != nullptr);

    auto nextPos1 = piece->nextPossibleMoves(board);
    EXPECT_TRUE(nextPos1.size() == 3);
    EXPECT_TRUE(nextPos1[2].row_ == 2);
    EXPECT_TRUE(nextPos1[2].col_ == 7);

    // TEST pion prise en passasnt :
    auto pos2 = Position(4, 'e');
    piece = Piece::getPiece(board, pos2);
    EXPECT_TRUE(piece != nullptr);

    auto nextPos2 = piece->nextPossibleMoves(board);
    EXPECT_TRUE(nextPos2.size() == 2);
    EXPECT_TRUE(nextPos2[1].row_ == 5);
    EXPECT_TRUE(nextPos2[1].col_ == 3);


    // ChessAlgorithm algo;
    // algo.nextMove(c);

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