#include "Model.h"
#include <vector>
namespace chess
{
    class ChessAlgorithm
    {
    public:
        BoardPtr nextMove(Model &model)
        {
            auto board = model.CreateBoard();
            for (int row = 0; row < 8; ++row)
                for (int col = 0; col < 8; ++col)
                {
                    BoardPositions &bpos = (*board)[row][col];
                    if (!bpos.piece)
                        continue;
                    std::vector<NextMove> possibleMoves = bpos.piece->nextPossibleMoves(board);
                }
            return board;
        }

        int getWeight(NextMove &move)
        {
            return move.weight_;
        }

        auto bestMove(Model &model, Color color)
        {
            auto board = model.CreateBoard();
            std::vector<NextMove> bestMoves;
            for (auto piece : model.pieces_)
            {
                if (piece->color_ != color)
                    continue;
                auto nextPos = piece->nextPossibleMoves(board);
                if (nextPos.empty())
                    continue;
                NextMove best = piece->bestMove(nextPos);
                bestMoves.push_back(best);
            }

            int weight = 0;
            int index = 0;
            int i = 0;
            for (auto &move : bestMoves)
            {
                int v = getWeight(move);
                if (v > weight)
                {
                    index = i;
                    weight = v;
                }
                i++;
            }
            return bestMoves[index];
        }
    };
}
