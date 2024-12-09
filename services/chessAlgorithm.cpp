#include <model.h>
#include <vector>

class ChessAlgorithm{
    public:
        void nextMove(chess::Model model, chess::Board board)
        {
            chess::NextPossiblePositions possiblePositions;
            for(auto piece : model.pieces_) 
            {   
                if(isValidMove){
                    possiblePositions.push_back(position)
                } 
            }
        }

        bool isValidMove(chess::Model::PiecePtr piece, chess::Position position, chess::Board board)
        {
            if(nextPos.piece && nextPos.piece->color_==piece->color_)
            {
                return false;
            }
            return true;
        }






};