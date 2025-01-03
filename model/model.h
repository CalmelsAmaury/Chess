#pragma once
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <iostream>
#include <sstream>
#include <array>
#include <Piece.h>
#include <Queen.h>
#include <King.h>
#include <Rook.h>
#include <Knight.h>
#include <Bishop.h>
#include <Pawn.h>

namespace chess
{
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

        std::string WHITE_COLOR = "\033[36m";
        std::string BLACK_COLOR = "\033[91m";
        std::string PINK_COLOR = "\033[32m";
        std::string RESET_COLOR = "\033[0m";
        std::string WHITE_BACKGROUND = "\033[100m";
        std::string BLACK_BACKGROUND = "\033[10m";

        void PrintBoard(BoardPtr board)
        {
            std::cout << PINK_COLOR << "     0  1  2  3  4  5  6  7\n"
                      << RESET_COLOR;
            std::cout << PINK_COLOR << "     A  B  C  D  E  F  G  H\n"
                      << RESET_COLOR;
            for (int i = 7; i >= 0; i--)
            {
                std::cout << PINK_COLOR << i << "|" << i + 1 << " " << RESET_COLOR;
                for (int j = 0; j <= 7; j++)
                {
                    std::string background_color = (i + j) % 2 == 0 ? BLACK_BACKGROUND : WHITE_BACKGROUND;
                    std::cout << background_color;

                    auto &cell = (*board)[i][j];
                    if (cell.piece)
                    {
                        auto &piece = cell.piece;
                        char symbol = piece->name_ == "King"     ? (piece->color_ == Color::white ? 'K' : 'k')
                                      : piece->name_ == "Queen"  ? (piece->color_ == Color::white ? 'Q' : 'q')
                                      : piece->name_ == "Rook"   ? (piece->color_ == Color::white ? 'R' : 'r')
                                      : piece->name_ == "Bishop" ? (piece->color_ == Color::white ? 'B' : 'b')
                                      : piece->name_ == "Knight" ? (piece->color_ == Color::white ? 'N' : 'n')
                                      : piece->name_ == "Pawn"   ? (piece->color_ == Color::white ? 'P' : 'p')
                                                                 : '?';
                        if (piece->color_ == Color::white)
                        {
                            std::cout << WHITE_COLOR << " " << symbol << " " << RESET_COLOR;
                        }
                        else if (piece->color_ == Color::black)
                        {
                            std::cout << BLACK_COLOR << " " << symbol << " " << RESET_COLOR;
                        }
                    }
                    else
                    {
                        std::cout << "   ";
                    }
                    std::cout << RESET_COLOR;
                }
                std::cout << PINK_COLOR << " " << i + 1 << "|" << i << " " << RESET_COLOR;
                std::cout << "\n";
            }
            std::cout << PINK_COLOR << "     A  B  C  D  E  F  G  H\n"
                      << RESET_COLOR;
            std::cout << PINK_COLOR << "     0  1  2  3  4  5  6  7\n"
                      << RESET_COLOR;
        }

        std::vector<PiecePtr> pieces_;
    };

}
