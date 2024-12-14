#include <model.h>
#include <vector>
namespace chess
{
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
                std::vector<NextMove> possibleMoves = bpos.piece->nextPossibleMoves(board);
                
            }
            return board;
        }

        auto bestMove(NextMove nextMove)
        {   
            std::vector<NextMove> bestMove;
            if(nextMove.action_.actions_ == Actions::toTake)
            {
                bestMove.push_back(nextMove);
            }
            else if(nextMove.action_.actions_ != Actions::toTake)
            {
                bestMove.push_back(nextMove);
            }
            return bestMove;
        }
};
}
