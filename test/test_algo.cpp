
#include <gtest/gtest.h>
#include "model.h"
#include "ChessAlgorithm.h"
using namespace chess;

TEST(Pion, ThePawnMovesStraight)
{
    Model c;
    auto loaded = c.load("Chess.cfg");
    EXPECT_TRUE(loaded);
    auto board = c.CreateBoard();
 
    auto pos = Position(1, 'a');
    auto piece = Piece::getPiece(board, pos);
    EXPECT_TRUE(piece != nullptr);

    auto nextPos = piece->nextPossibleMoves(board);
    EXPECT_TRUE(nextPos.size() == 2);
    EXPECT_TRUE(nextPos[0].position_.row_ == 2);
    EXPECT_TRUE(nextPos[0].position_.col_ == 0);
  
}

TEST(Pion, DiagonalCapturePawn)
{
    Model c;
    auto loaded = c.load("Chess.cfg");
    EXPECT_TRUE(loaded);
    auto board = c.CreateBoard();
 
    auto pos = Position(1, 'g');
    auto piece = Piece::getPiece(board, pos);
    EXPECT_TRUE(piece != nullptr);

    auto nextPos = piece->nextPossibleMoves(board);
    EXPECT_TRUE(nextPos.size() == 3);
    EXPECT_TRUE(nextPos[2].position_.row_ == 2);
    EXPECT_TRUE(nextPos[2].position_.col_ == 7);
    EXPECT_TRUE(nextPos[2].action_.actions_ == Actions::toTake);
  
}

TEST(Pion, PriseEnPassantPawn)
{
    Model c;
    auto loaded = c.load("Chess.cfg");
    EXPECT_TRUE(loaded);
    auto board = c.CreateBoard();
 
    auto pos = Position(4, 'e');
    auto piece = Piece::getPiece(board, pos);
    EXPECT_TRUE(piece != nullptr);

    auto nextPos = piece->nextPossibleMoves(board);
    EXPECT_TRUE(nextPos.size() == 2);
    EXPECT_TRUE(nextPos[1].position_.row_ == 5);
    EXPECT_TRUE(nextPos[1].position_.col_ == 3);
    EXPECT_TRUE(nextPos[1].action_.actions_ == Actions::toTake);
  
}

TEST(Pion, PromotePawn)
{
    Model c;
    auto loaded = c.load("Chess.cfg");
    EXPECT_TRUE(loaded);
    auto board = c.CreateBoard();
 
    auto pos = Position(7, 'b');
    auto piece = Piece::getPiece(board, pos);
    EXPECT_TRUE(piece != nullptr);

    auto nextPos = piece->nextPossibleMoves(board);
    EXPECT_TRUE(nextPos.size() == 1);
    EXPECT_TRUE(nextPos[0].position_.row_ == 7);
    EXPECT_TRUE(nextPos[0].position_.col_ == 1);
    EXPECT_TRUE(nextPos[0].action_.actions_ == Actions::toPromote);
  
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}