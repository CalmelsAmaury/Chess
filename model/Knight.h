#include <Piece.h>

struct Knight : Piece
    {
        Knight(int id, const Position &position, Color color) : Piece(id, "Knight", position, color) {}
        std::vector<NextMove> nextPossibleMoves(BoardPtr board) override
        {
            std::vector<NextMove> nextMove;
            ruleMoveOnCases(board, nextMove);
            return nextMove;
        }

        auto getDirections()
        {
            std::vector<Direction> directions = {{1, -2}, {2, -1}, {2, 1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {-1, -2}};
            return directions;
        }

        // Le cavalier est la seule pièce « sauteuse » du jeu. Depuis sa case de départ, il « saute » directement sur sa case d’arrivée,
        // grâce à son déplacement singulier :
        // il se déplace d'une case dans une direction horizontale ou verticale (comme une tour)
        // puis d'une case dans une direction en diagonale (comme un fou) ; on dit alors qu'il se déplace en « Y ».
        void ruleMoveOnCases(BoardPtr board, std::vector<NextMove> &nextMove)
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
                        break;
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

        // Fonction pour savoir si le cavalier met en echec le roi
        bool doesItCheck(BoardPtr board, const Position &targetPositionKing) override
        {
            int dx = abs(position_.col_ - targetPositionKing.col_);
            int dy = abs(position_.row_ - targetPositionKing.row_);

            if ((dx + dy) != 3)
            {
                return false;
            }

            auto directions = getDirections();

            for (auto direction : directions)
            {
                Position targetPosition(position_.row_ + direction.row_, position_.col_ + direction.col_);

                if (targetPosition.col_ == targetPositionKing.col_ && targetPosition.row_ == targetPositionKing.row_)
                {
                    return true;
                }
            }
            return false;
        }
    };