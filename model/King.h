#include <Piece.h>

struct King : Piece
    {
        King(int id, const Position &position, Color color) : Piece(id, "King", position, color) {}
        std::vector<NextMove> nextPossibleMoves(BoardPtr board) override
        {
            std::vector<NextMove> nextMove;
            ruleMoveOnCases(board, nextMove);
            return nextMove;
        }

        auto getDirections()
        {
            std::vector<Direction> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, -1}, {1, 1}, {-1, 1}, {-1, -1}};
            return directions;
        }

        void ruleMoveOnCases(BoardPtr board, std::vector<NextMove> &nextMove)
        {
            auto directions = getDirections();
            auto action = Action(Actions::toMove, nullptr);

            for (auto direction : directions)
            {
                Position targetPosition(position_.row_ + direction.row_, position_.col_ + direction.col_);
                if (!isInRange(board, targetPosition))
                {
                    continue;
                }
                else if (!isEmptyCell(board, targetPosition) && !isEnemy(board, targetPosition))
                {
                    continue;
                }
                else if (ruleIsCheck(board, targetPosition))
                {
                    continue;
                }
                else if (isEmptyCell(board, targetPosition))
                {
                    nextMove.push_back(NextMove(action, targetPosition));
                    continue;
                }
                else if (isEnemy(board, targetPosition))
                {

                    action = Action(Actions::toTake, getPiece(board, targetPosition));
                    nextMove.push_back(NextMove(action, targetPosition));
                    continue;
                }
                else
                    continue;
            }
        }

        bool ruleIsCheck(BoardPtr board, Position &targetPosition)
        {
            for (int row = 0; row < 8; row++)
            {
                for (int col = 0; col < 8; col++)
                {
                    auto piece = (*board)[row][col].piece;
                    if (!piece)
                    {
                        continue;
                    }
                    else if (!isEnemy(board, Position(row, col)))
                    {
                        continue;
                    }
                    else if (piece->doesItCheck(board, targetPosition))
                    {
                        std::cout << "This square is attacked at : Row " << targetPosition.row_ << " Col " << targetPosition.col_ << ". By a " << piece->name_ << std::endl;
                        return true;
                    }
                }
            }
            return false;
        }

        // Fonction pour savoir si le roi empêche un déplacement du roi adverse
        bool doesItCheck(BoardPtr board, const Position &targetPositionKing) override
        {
            auto directions = getDirections();

            for (auto direction : directions)
            {
                Position targetPosition(position_.row_ + direction.row_, position_.col_ + direction.col_);

                if (targetPosition.row_ == targetPositionKing.row_ && targetPosition.col_ == targetPositionKing.col_)
                {
                    return true;
                }
            }
            return false;
        }
    };