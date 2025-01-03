#pragma once
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <iostream>
#include <sstream>
#include <array>

enum class Color
{
    white,
    black
};

struct Position
{
    explicit Position(int row, char col) : row_(row), col_(static_cast<int>(col - 'a'))
    {
    }

    explicit Position(int row, int col) : row_(row), col_(col)
    {
    }

    explicit Position() : row_(-1), col_(-1)
    {
    }

    int row_;
    int col_;
};

class Piece;

using PiecePtr = std::shared_ptr<Piece>;

struct BoardPositions
{
    PiecePtr piece;
    std::vector<Position> nextMove;
};

using Board = std::array<std::array<BoardPositions, 8>, 8>;
using BoardPtr = std::shared_ptr<Board>;

enum class Actions
{
    none,
    toMove,
    toMoveTwo,
    toTake,
    toCheck,
    toPromote
};

struct Action
{
    Action(const Actions &action, PiecePtr piece) : actions_(action), piece_(piece) {}
    Actions actions_;
    PiecePtr piece_;
};

struct NextMove
{
    NextMove(Action &action, Position &position) : action_(action), position_(position) {}
    NextMove() : action_(Actions::none, nullptr) {}
    Action action_;
    Position position_;
    int weight_{0};
};

struct Direction
{
    int row_;
    int col_;
};

struct Piece
{
    Piece(int id, const std::string &pieceName, const Position &piecePosition, const Color &color) : id_(id), name_(pieceName), position_(piecePosition), color_(color) {}
    virtual ~Piece() = default;
    std::string toString()
    {
        std::ostringstream oss;
        oss << id_ << "," << name_ << "," << (color_ == Color::white ? 'w' : 'b') << "," << position_.col_ << "," << position_.row_;
        return oss.str();
    }

    static PiecePtr getPiece(BoardPtr board, const Position &pos)
    {
        return (*board)[pos.row_][pos.col_].piece;
    }

    static bool isEmptyCell(BoardPtr board, const Position &pos)
    {
        auto piece = (*board)[pos.row_][pos.col_].piece;
        if (!piece)
            return true;
        return false;
    }

    bool isPawn(BoardPtr board, const Position &pos)
    {
        auto piece = (*board)[pos.row_][pos.col_].piece;
        if (piece->name_ == "Pawn")
            return true;
        return false;
    }

    bool isEnemy(BoardPtr board, const Position &pos)
    {
        auto piece = (*board)[pos.row_][pos.col_].piece;
        if (piece->color_ != color_)
        {
            return true;
        }
        return false;
    }

    bool isInRange(BoardPtr board, const Position &pos)
    {
        if (pos.col_ >= 0 && pos.col_ < 8 && pos.row_ >= 0 && pos.row_ < 8)
        {
            return true;
        }
        return false;
    }

    virtual bool doesItCheck(BoardPtr board, const Position &targetPosition) { return {}; }

    virtual int getWeight(Actions action, PiecePtr targetedPiece)
    {
        int pieceValue = 0;
        if (targetedPiece)
        {
            if (targetedPiece->name_ == "Pawn")
                pieceValue = 1;
            else if (targetedPiece->name_ == "Knight" || targetedPiece->name_ == "Bishop")
                pieceValue = 3;
            else if (targetedPiece->name_ == "Rook")
                pieceValue = 5;
            else if (targetedPiece->name_ == "Queen")
                pieceValue = 9;
        }

        switch (action)
        {
        case Actions::none:
            return -1;
        case Actions::toMove:
            return 0;
        case Actions::toMoveTwo:
            return 0;
        case Actions::toCheck:
            return 1;
        case Actions::toTake:
            return pieceValue;
        case Actions::toPromote:
            return 9;
        }
        return -1;
    }

    virtual NextMove bestMove(std::vector<NextMove> &nextPositions)
    {
        int weight = 0;
        int index = 0;
        int i = 0;
        for (auto &move : nextPositions)
        {
            int v = getWeight(move.action_.actions_, move.action_.piece_);
            if (v > weight)
            {
                index = i;
                weight = v;
            }
            i++;
        }
        nextPositions[index].weight_ = weight;
        return nextPositions[index];
    }

    virtual std::vector<NextMove> nextPossibleMoves(BoardPtr board) { return {}; }
    Position position_;
    std::string name_;
    Color color_;
    int id_;
};