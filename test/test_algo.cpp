
#include <gtest/gtest.h>
#include "model.h"
#include "ChessAlgorithm.h"
#include "PrintingBoard.h"
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
    EXPECT_TRUE(nextPos.size() == 3);
    EXPECT_TRUE(nextPos[0].position_.row_ == 2);
    EXPECT_TRUE(nextPos[0].position_.col_ == 0);
}

TEST(Pion, BestMoveDiagonalCaptureDown2)
{
    Model c;
    auto loaded = c.load("Chess.cfg");
    EXPECT_TRUE(loaded);
    auto board = c.CreateBoard();

    auto pos = Position(1, 'a');
    auto piece = Piece::getPiece(board, pos);
    EXPECT_TRUE(piece != nullptr);

    auto nextPos = piece->nextPossibleMoves(board);
    NextMove best = piece->bestMove(nextPos);
    EXPECT_TRUE(best.action_.piece_ != nullptr);
    EXPECT_TRUE(best.action_.piece_->name_ == "Pawn");
    EXPECT_TRUE(best.position_.row_ == 2);
    EXPECT_TRUE(best.position_.col_ == 1);
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

    auto pos = Position(4, 'c');
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

TEST(Rook, RookBestMove)
{
    Model c;
    auto loaded = c.load("Chess.cfg");
    EXPECT_TRUE(loaded);
    auto board = c.CreateBoard();
    PrintingBoard p;
    p.PrintBoard(board);

    auto pos = Position(3, 'h');
    auto piece = Piece::getPiece(board, pos);
    EXPECT_TRUE(piece != nullptr);

    auto nextPos = piece->nextPossibleMoves(board);
    NextMove best = piece->bestMove(nextPos);
    EXPECT_TRUE(best.action_.piece_ != nullptr);
    EXPECT_TRUE(best.action_.piece_->name_ == "Rook");
    EXPECT_TRUE(best.position_.row_ == 7);
    EXPECT_TRUE(best.position_.col_ == 7);
}

TEST(Bishop, BishopBestMove)
{
    Model c;
    auto loaded = c.load("Chess.cfg");
    EXPECT_TRUE(loaded);
    auto board = c.CreateBoard();
    PrintingBoard p;
    p.PrintBoard(board);

    auto pos = Position(4, 'f');
    auto piece = Piece::getPiece(board, pos);
    EXPECT_TRUE(piece != nullptr);

    auto nextPos = piece->nextPossibleMoves(board);
    NextMove best = piece->bestMove(nextPos);
    EXPECT_TRUE(best.action_.piece_ != nullptr);
    EXPECT_TRUE(best.action_.piece_->name_ == "Bishop");
    EXPECT_TRUE(best.position_.row_ == 7);
    EXPECT_TRUE(best.position_.col_ == 2);
}

TEST(Queen, QueenBestMove)
{
    Model c;
    auto loaded = c.load("Chess.cfg");
    EXPECT_TRUE(loaded);
    auto board = c.CreateBoard();
    PrintingBoard p;
    p.PrintBoard(board);

    auto pos = Position(5, 'f');
    auto piece = Piece::getPiece(board, pos);
    EXPECT_TRUE(piece != nullptr);

    auto nextPos = piece->nextPossibleMoves(board);
    NextMove best = piece->bestMove(nextPos);
    EXPECT_TRUE(best.action_.piece_ != nullptr);
    EXPECT_TRUE(best.action_.piece_->name_ == "Rook");
    EXPECT_TRUE(best.position_.row_ == 3 || best.position_.row_ == 0);
    EXPECT_TRUE(best.position_.col_ == 7 || best.position_.col_ == 0);
}

TEST(Knight, KnightBestMove)
{
    Model c;
    auto loaded = c.load("Chess.cfg");
    EXPECT_TRUE(loaded);
    auto board = c.CreateBoard();
    PrintingBoard p;
    p.PrintBoard(board);

    auto pos = Position(5, 'c');
    auto piece = Piece::getPiece(board, pos);
    EXPECT_TRUE(piece != nullptr);

    auto nextPos = piece->nextPossibleMoves(board);
    NextMove best = piece->bestMove(nextPos);
    EXPECT_TRUE(best.action_.piece_ != nullptr);
    EXPECT_TRUE(best.action_.piece_->name_ == "Pawn");
    EXPECT_TRUE(best.position_.row_ == 7);
    EXPECT_TRUE(best.position_.col_ == 1);
}

TEST(King, KingBestMove)
{
    Model c;
    auto loaded = c.load("Chess.cfg");
    EXPECT_TRUE(loaded);
    auto board = c.CreateBoard();
    PrintingBoard p;
    p.PrintBoard(board);

    auto pos = Position(0, 'e');
    auto piece = Piece::getPiece(board, pos);
    EXPECT_TRUE(piece != nullptr);

    auto nextPos = piece->nextPossibleMoves(board);
    NextMove best = piece->bestMove(nextPos);
    EXPECT_TRUE(best.action_.piece_ == nullptr);
    EXPECT_TRUE(best.position_.row_ == 1);
    EXPECT_TRUE(best.position_.col_ == 4);
}

TEST(Chess, BestMovesAllPieces)
{
    Model c;
    auto loaded = c.load("Chess2.cfg");
    EXPECT_TRUE(loaded);
    auto board = c.CreateBoard();
    PrintingBoard p;
    p.PrintBoard(board);
    ChessAlgorithm algo;
    NextMove best = algo.bestMove(c, Color::white);
    EXPECT_TRUE(best.action_.piece_ != nullptr);
    EXPECT_TRUE(best.action_.piece_->name_ == "Pawn");
    EXPECT_TRUE(best.position_.row_ == 2);
    EXPECT_TRUE(best.position_.col_ == 7);
    best = algo.bestMove(c, Color::black);
    EXPECT_TRUE(best.action_.piece_ != nullptr);
    EXPECT_TRUE(best.action_.piece_->name_ == "Pawn");
    EXPECT_TRUE(best.position_.row_ == 1 || best.position_.row_ == 1);
    EXPECT_TRUE(best.position_.col_ == 2 || best.position_.col_ == 0);
}

TEST(King, QueenAndPawnCheck)
{
    Model c;
    auto loaded = c.load("Chess3.cfg");
    EXPECT_TRUE(loaded);
    auto board = c.CreateBoard();
    PrintingBoard p;
    p.PrintBoard(board);

    auto pos = Position(5, 'd');
    auto piece = Piece::getPiece(board, pos);
    EXPECT_TRUE(piece != nullptr);

    auto nextPos = piece->nextPossibleMoves(board);
    EXPECT_TRUE(nextPos.size() == 1);
}

TEST(King, RookKnightKingBishopCheck)
{
    Model c;
    auto loaded = c.load("Chess4.cfg");
    EXPECT_TRUE(loaded);
    auto board = c.CreateBoard();
    PrintingBoard p;
    p.PrintBoard(board);

    auto pos = Position(4, 'd');
    auto piece = Piece::getPiece(board, pos);
    EXPECT_TRUE(piece != nullptr);

    auto nextPos = piece->nextPossibleMoves(board);
    EXPECT_TRUE(nextPos.size() == 1);
}

TEST(Board, PrintBoard)
{
    Model c;
    auto loaded = c.load("ChessInitialPosition.cfg");
    EXPECT_TRUE(loaded);
    auto board = c.CreateBoard();
    PrintingBoard p;
    p.PrintBoard(board);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}