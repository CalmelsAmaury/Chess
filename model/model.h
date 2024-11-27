#pragma once
#include <vector>
#include <string>
#include <memory>


namespace chess
{
    struct Position {
        Position(int _row, int _col) : row(_row), col(_col) {}

        int row;
        int col;
    };

    struct Piece {
        Piece(const std::string& pieceName, const Position& piecePosition) : name(pieceName), position(piecePosition) {}

        Position position;  
        std::string name;
        
    };

    struct Pawn : Piece{
        Pawn(const Position& position) : Piece("Pawn", position) {}
    };

    struct Rook : Piece{
        Rook(const Position& position) : Piece("Rook", position) {}
    };

    struct Bishop : Piece{
        Bishop(const Position& position) : Piece("Bishop", position) {}
    };

    struct King : Piece{
        King(const Position& position) : Piece("King", position) {}
    };

    struct Queen : Piece{
        Queen(const Position& position) : Piece("Queen", position) {}
    };

    struct Knight : Piece{
        Knight(const Position& position) : Piece("Knight", position) {}
    };




    /**
     * construis le model compose de pions, tours, etc
     */
    class Model
    {
        public:
        bool load(const std::string &path)
        {
            // code pour loader le model
            Pawn whitePawn1(Position(2,1));
            Pawn whitePawn2(Position(2,2));
            Pawn whitePawn3(Position(2,3));
            Pawn whitePawn4(Position(2,4));
            Pawn whitePawn5(Position(2,5));
            Pawn whitePawn6(Position(2,6));
            Pawn whitePawn7(Position(2,7));
            Pawn whitePawn8(Position(2,8));

            Pawn blackPawn1(Position(7,1));
            Pawn blackPawn2(Position(7,2));
            Pawn blackPawn3(Position(7,3));
            Pawn blackPawn4(Position(7,4));
            Pawn blackPawn5(Position(7,5));
            Pawn blackPawn6(Position(7,6));
            Pawn blackPawn7(Position(7,7));
            Pawn blackPawn8(Position(7,8));

            Bishop whiteBishop1(Position(1,3));
            Bishop whiteBishop2(Position(1,6));

            Bishop blackBishop1(Position(8,3));
            Bishop blackBishop2(Position(8,6));

            Rook whiteRook1(Position(1,1));
            Rook whiteRook2(Position(1,8));

            Rook blackRook1(Position(8,1));
            Rook blackRook2(Position(8,8));

            Knight whiteKnight1(Position(1,2));
            Knight whiteKnight2(Position(1,7));

            Knight blackKnight1(Position(8,2));
            Knight blackKnight2(Position(8,7));

            Queen whiteQueen(Position(1,4));

            Queen blackQueen(Position(8,4));
            
            King whiteKing(Position(1,5));

            King blackKing(Position(8,5));
        }
        std::vector<std::shared_ptr<Piece>> pieces_;
    };

}
