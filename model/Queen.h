#include <Piece.h>

struct Queen : Piece
{
    Queen(int id, const Position &position, Color color) : Piece(id, "Queen", position, color) {}
    std::vector<NextMove> nextPossibleMoves(BoardPtr board) override
    {
        std::vector<NextMove> nextMove;
        ruleMove(board, nextMove);
        return nextMove;
    }

    auto getDirections()
    {
        std::vector<Direction> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, -1}, {1, 1}, {-1, 1}, {-1, -1}};
        return directions;
    }

    // La dame cumule les déplacements de la tour et du fou, cela en fait la pièce la plus puissante du jeu.
    void ruleMove(BoardPtr board, std::vector<NextMove> &nextMove)
    {
        auto directions = getDirections();
        auto action = Action(Actions::toMove, nullptr);

        for (auto direction : directions)
        {
            Position targetPosition(position_.row_ + direction.row_, position_.col_ + direction.col_);
            while (true)
            {
                if (isInRange(board, targetPosition) && isEmptyCell(board, targetPosition))
                {
                    nextMove.push_back(NextMove(action, targetPosition));
                    targetPosition = Position(targetPosition.row_ + direction.row_, targetPosition.col_ + direction.col_);
                }

                else if (isInRange(board, targetPosition) && !isEmptyCell(board, targetPosition) && isEnemy(board, targetPosition))
                {
                    action = Action(Actions::toTake, getPiece(board, targetPosition));
                    nextMove.push_back(NextMove(action, targetPosition));
                    break;
                }
                else
                    break;
            }
        }
    }

    // Fonction pour savoir si la reine met en echec le roi
    bool doesItCheck(BoardPtr board, const Position &targetPositionKing) override
    {
        if (position_.col_ != targetPositionKing.col_ && position_.row_ != targetPositionKing.row_ && abs(position_.col_ - targetPositionKing.col_) != abs(position_.row_ - targetPositionKing.row_))
        {
            return false;
        }

        int dx = (targetPositionKing.col_ > position_.col_) ? 1 : (targetPositionKing.col_ < position_.col_) ? -1
                                                                                                             : 0;
        int dy = (targetPositionKing.row_ > position_.row_) ? 1 : (targetPositionKing.row_ < position_.row_) ? -1
                                                                                                             : 0;
        int x = position_.col_ + dx;
        int y = position_.row_ + dy;

        while (x != targetPositionKing.col_ || y != targetPositionKing.row_)
        {
            Position intermediatePosition(y, x);
            if (!isEmptyCell(board, intermediatePosition))
            {
                return false;
            }
            x += dx;
            y += dy;
        }
        return true;
    }
};