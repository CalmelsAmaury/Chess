#pragma once
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <iostream>
#include <sstream>

namespace chess
{
    enum class Color
    {
        white,
        black
    };

    struct Position
    {
        Position(int row, char col) : row_(row), col_(col) {}

        int row_;
        char col_;
    };

    struct Piece
    {
        Piece(const std::string &pieceName, const Position &piecePosition, const Color &color) : name_(pieceName), position_(piecePosition), color_(color) {}

        Position position_;
        std::string name_;
        Color color_;
    };

    struct Pawn : Piece
    {
        Pawn(const Position &position, Color color) : Piece("Pawn", position, color) {}
    };

    struct Rook : Piece
    {
        Rook(const Position &position, Color color) : Piece("Rook", position, color) {}
    };

    struct Bishop : Piece
    {
        Bishop(const Position &position, Color color) : Piece("Bishop", position, color) {}
    };

    struct King : Piece
    {
        King(const Position &position, Color color) : Piece("King", position, color) {}
    };

    struct Queen : Piece
    {
        Queen(const Position &position, Color color) : Piece("Queen", position, color) {}
    };

    struct Knight : Piece
    {
        Knight(const Position &position, Color color) : Piece("Knight", position, color) {}
    };

    /**
     * construit le model compose de pions, tours, etc
     */
    class Model
    {

    public:
        using PiecePtr = std::shared_ptr<Piece>;

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
                auto v=splitString(line, ',');
                if(v[0]=="Queen"){
                    auto row=std::stoi(v[3]);
                    auto col=v[2][0];
                    auto color=v[1][0];
                    
                    auto queen = std::make_shared<Queen>(Position(row, col), color=='w'? Color::white : Color::black);
                    pieces_.push_back(queen);
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

        std::vector<PiecePtr> pieces_;
    };

}
