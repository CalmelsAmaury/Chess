#pragma once
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <iostream>
#include <sstream>
#include <array>

namespace chess
{

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

        int row_;
        int col_;
    };

    class Piece;

    using PiecePtr = std::shared_ptr<Piece>;

    struct BoardPositions
    {
        PiecePtr piece;
        std::vector<Position> possiblePositions;
    };

    using Board = std::array<std::array<BoardPositions, 8>, 8>;
    using BoardPtr = std::shared_ptr<Board>;

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

        virtual std::vector<Position> nextPossibleMoves(BoardPtr board) { return {}; }
        Position position_;
        std::string name_;
        Color color_;
        int id_;
    };

    struct Pawn : Piece
    {
        bool isFirstMove = true;
        Pawn(int id, const Position &position, Color color) : Piece(id, "Pawn", position, color) {}

        // avance de 1 dans la direction de la couleur
        int getDirection()
        {
            int direction(color_ == Color::white ? +1 : -1);
            return direction;
        }

        // Le pion se déplace droit devant lui (vers la 8e rangée pour les Blancs et vers la 1re rangée pour les Noirs),
        // d'une seule case à chaque coup et sans jamais pouvoir reculer7.
        void ruleMoveOnCases(BoardPtr board, std::vector<Position> &possiblePositions)
        {
            auto direction = getDirection();
            auto position = Position(position_.row_ + direction, position_.col_);
            if (isEmptyCell(board, position))
            {
                possiblePositions.push_back(position);
            }
        }

        // Lors de son premier déplacement (alors qu'il est sur sa case initiale), un pion peut avancer de deux cases en un seul coup
        void ruleMoveTwoCases(BoardPtr board, std::vector<Position> &possiblePositions)
        {
            if(!isFirstMove) return;
            auto direction = getDirection();
            if (color_ == Color::white && position_.row_ == 2 || color_ == Color::black && position_.row_ == 7)
            {
                auto position = Position(position_.row_ + direction * 2, position_.col_);
                if (isEmptyCell(board, position))
                {
                    possiblePositions.push_back(position);
                }
            }
        }

        // Il ne peut capturer une pièce adverse que si elle se trouve à une case en diagonale de lui dans son sens de déplacement
        void ruleMoveDiagonal(BoardPtr board, std::vector<Position> &possiblePositions)
        {
            // TODO
        }

        // calcule la liste des possibles positions de la pièce
        std::vector<Position> nextPossibleMoves(BoardPtr board) override
        {
            std::vector<Position> positions;
            ruleMoveOnCases(board, positions);
            ruleMoveTwoCases(board, positions);
            return positions;
        }
    };

    struct Rook : Piece
    {
        Rook(int id, const Position &position, Color color) : Piece(id, "Rook", position, color) {}
    };

    struct Bishop : Piece
    {
        Bishop(int id, const Position &position, Color color) : Piece(id, "Bishop", position, color) {}
    };

    struct King : Piece
    {
        King(int id, const Position &position, Color color) : Piece(id, "King", position, color) {}
    };

    struct Queen : Piece
    {
        Queen(int id, const Position &position, Color color) : Piece(id, "Queen", position, color) {}
    };

    struct Knight : Piece
    {
        Knight(int id, const Position &position, Color color) : Piece(id, "Knight", position, color) {}
    };

    /**
     * construit le model compose de pions, tours, etc
     */
    class Model
    {

    public:
        bool load(const std::string &path)
        {
            std::ifstream file(path);

            // Check if the file was opened successfully
            if (!file.is_open())
            {
                std::cerr << "Error: Could not open the file " << path << std::endl;
                return false; // Exit with an error code
            }

            // Read the file line by line
            std::string line;
            while (std::getline(file, line))
            {
                // Print each line to the console
                std::cout << line << std::endl;
                auto v = splitString(line, ',');
                auto row = std::stoi(v[4]) - 1;
                auto col = v[3][0];
                auto color = v[2][0];
                auto id = std::stoi(v[0]);

                if (v[1] == "Queen")
                {

                    auto queen = std::make_shared<Queen>(id, Position(row, col), color == 'w' ? Color::white : Color::black);
                    pieces_.push_back(queen);
                }
                else if (v[1] == "King")
                {

                    auto king = std::make_shared<King>(id, Position(row, col), color == 'w' ? Color::white : Color::black);
                    pieces_.push_back(king);
                }
                else if (v[1] == "Rook")
                {

                    auto rook = std::make_shared<Rook>(id, Position(row, col), color == 'w' ? Color::white : Color::black);
                    pieces_.push_back(rook);
                }
                else if (v[1] == "Knight")
                {

                    auto knight = std::make_shared<Knight>(id, Position(row, col), color == 'w' ? Color::white : Color::black);
                    pieces_.push_back(knight);
                }
                else if (v[1] == "Bishop")
                {

                    auto bishop = std::make_shared<Bishop>(id, Position(row, col), color == 'w' ? Color::white : Color::black);
                    pieces_.push_back(bishop);
                }
                else if (v[1] == "Pawn")
                {

                    auto pawn = std::make_shared<Pawn>(id, Position(row, col), color == 'w' ? Color::white : Color::black);
                    pieces_.push_back(pawn);
                }
            }

            // Close the file
            file.close();

            return true;
        }

        PiecePtr GetQueen(Color color)
        {
            for (PiecePtr piece : pieces_)
            {
                if (piece->name_ == "Queen" && piece->color_ == color)
                {
                    return piece;
                }
            }
            return nullptr;
        }

        PiecePtr GetPiece(int id)
        {
            for (PiecePtr pieceId : pieces_)
            {
                if (pieceId->id_ == id)
                {
                    return pieceId;
                }
            }
            return nullptr;
        }

        std::vector<std::string> splitString(const std::string &str, char delimiter)
        {
            std::vector<std::string> tokens;
            std::stringstream ss(str);
            std::string token;

            while (std::getline(ss, token, delimiter))
            {
                tokens.push_back(token);
            }

            return tokens;
        }

        bool save()
        {
            int i = 1;
            std::string fileName = "coupNumero" + std::to_string(i) + ".txt";

            std::ofstream file(fileName);

            if (!file.is_open())
            {
                std::cerr << "Error: Could not open the file " << fileName << std::endl;
                return false;
            }

            for (auto &piece : pieces_)
            {
                file << piece->toString() << "\n";
            }

            file.close();

            return true;
        }

        BoardPtr CreateBoard()
        {
            BoardPtr board = std::shared_ptr<Board>(new Board());

            for (auto piece : pieces_)
            {
                int col = piece->position_.col_;
                int row = piece->position_.row_;
                std::cout << piece->toString() << std::endl;
                if (col >= 0 && col < 8 && row >= 0 && row < 8)
                {
                    BoardPositions &bp = ((*board)[row][col]);
                    bp.piece = piece;
                }
                else
                {
                    throw std::out_of_range("Piece position out of board bounds");
                }
            }

            return board;
        }

        std::vector<PiecePtr> pieces_;
    };

}
