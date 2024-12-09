#include <model.h>
#include <vector>

class ChessAlgorithm{
    public:
        BoardPtr nextMove(Model& model )
        {
           auto board = model.CreateBoard();
            for(int row =0; row< 8; ++row) 
                   for(int col =0; col< 8; ++col) 
            {   
                BoardPositions& bpos = (*board)[row][col];
                if(!bpos.piece) continue;
               bpos.possiblePositions = bpos.piece->nextPossibleMoves(board);
                
            }
            return board;
        }

       


 struct BoardPositions
    {
        Model::PiecePtr piece;
        std::vector<Position> possiblePositions;

    };



};
