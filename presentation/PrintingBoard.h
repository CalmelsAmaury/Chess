#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <iostream>
#include <sstream>
#include <array>
#include "Model.h"

namespace chess
{
    class PrintingBoard
    {
        public: 
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
    };
}
