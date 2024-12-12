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
        std::vector<Position> nextMove;
    };

    using Board = std::array<std::array<BoardPositions, 8>, 8>;
    using BoardPtr = std::shared_ptr<Board>;

    enum class Actions
    {
        toMove, 
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
        Action action_;
        Position position_;
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
            if(piece->color_ != color_)
            {
                return true;
            }
            return false;
        }

        bool isInRange(BoardPtr board, const Position &pos)
        {
            if(pos.col_ >= 0 && pos.col_ < 8 && pos.row_ >= 0 && pos.row_ < 8)
            {
                return true;
            }
            return false;
        }

        virtual std::vector<NextMove> nextPossibleMoves(BoardPtr board) { return {}; }
        Position position_;
        std::string name_;
        Color color_;
        int id_;
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
        void ruleMoveOnCases(BoardPtr board, std::vector<NextMove> &nextMove)
        {
            auto direction = getDirection();
            auto position = Position(position_.row_ + direction, position_.col_);
            auto action = Action(Actions::toMove, nullptr);
            if (isInRange(board, position) &&isEmptyCell(board, position))
            {
                nextMove.push_back(NextMove(action, position));
            }
        }

        // Lors de son premier déplacement (alors qu'il est sur sa case initiale), un pion peut avancer de deux cases en un seul coup
        void ruleMoveTwoCases(BoardPtr board, std::vector<NextMove> &nextMove)
        {
            if(!isFirstMove) return;
            auto direction = getDirection();
            auto action = Action(Actions::toMove, nullptr);
            if (color_ == Color::white && position_.row_ == 1 || color_ == Color::black && position_.row_ == 6)
            {
                auto position = Position(position_.row_ + direction * 2, position_.col_);
                if (isInRange(board, position) && isEmptyCell(board, position))
                {
                    nextMove.push_back(NextMove(action, position));
                }
            }
        }

        // Il ne peut capturer une pièce adverse que si elle se trouve à une case en diagonale de lui dans son sens de déplacement
        void ruleMoveDiagonal(BoardPtr board, std::vector<NextMove> &nextMove)
        {
            auto direction = getDirection();
            auto positionLeft = Position(position_.row_ + direction, position_.col_ - 1);
            auto positionRight = Position(position_.row_ + direction, position_.col_ + 1);
            auto action = Action(Actions::toTake, nullptr);
            
            if(isInRange(board, positionLeft) && !isEmptyCell(board, positionLeft) && isEnemy(board, positionLeft))
            {
                nextMove.push_back(NextMove(action, positionLeft));
            }
            if(isInRange(board, positionRight) && !isEmptyCell(board, positionRight) && isEnemy(board, positionRight))
            {
                nextMove.push_back(NextMove(action, positionRight));
            }
        }

        // ce dernier a la possibilité de prendre comme si le coup de début n'avait été que d'une case. Cette prise en passant ne peut se faire qu'en réponse immédiate à l'avance double
        void rulePriseEnPassant(BoardPtr board, std::vector<NextMove> &nextMove)
        {
            auto direction = getDirection();
            auto positionLeftDiagonal = Position(position_.row_ + direction, position_.col_ - 1);
            auto positionRightDiagonal = Position(position_.row_ + direction, position_.col_ + 1);
            auto positionLeft = Position(position_.row_, position_.col_ - 1);
            auto positionRight = Position(position_.row_, position_.col_ + 1);
            auto action = Action(Actions::toMove, nullptr);

            if (color_ == Color::white && position_.row_ == 4 || color_ == Color::black && position_.row_ == 3)
            {
                if(isInRange(board, positionLeftDiagonal) && isInRange(board, positionLeft) && isEmptyCell(board, positionLeftDiagonal) && !isEmptyCell(board, positionLeft) && isEnemy(board, positionLeft) && isPawn(board, positionLeft))
                {
                    nextMove.push_back(NextMove(action, positionLeftDiagonal));
                }
                if(isInRange(board, positionRightDiagonal) && isInRange(board, positionRight) && isEmptyCell(board, positionRightDiagonal) && !isEmptyCell(board, positionRight) && isEnemy(board, positionRight) && isPawn(board, positionLeft))
                {
                    nextMove.push_back(NextMove(action, positionRightDiagonal));
                }

            }
        }

        // Quand le pion arrive sur la dernière rangée, il doit se transformer en une pièce de son camp de valeur supérieure, au choix du joueur : dame, tour, fou ou cavalier.
        void rulePromote(BoardPtr board, std::vector<NextMove> &nextMove)
        {
            auto action = Action(Actions::toPromote, nullptr);
            if(color_ == Color::white && position_.row_ == 7 || color_ == Color::black && position_.row_ == 0)
            {
                auto promotedPiece = std::make_shared<Queen>(id_, position_, color_);
                nextMove.push_back(NextMove(action, position_));
                (*board)[position_.row_][position_.col_].piece = promotedPiece;
            }
        }

        // calcule la liste des possibles positions de la pièce
        std::vector<NextMove> nextPossibleMoves(BoardPtr board) override
        {
            std::vector<NextMove> nextMove;
            ruleMoveOnCases(board, nextMove);
            ruleMoveTwoCases(board, nextMove);
            ruleMoveDiagonal(board, nextMove);
            rulePriseEnPassant(board, nextMove);
            rulePromote(board, nextMove);
            return nextMove;
        }
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
