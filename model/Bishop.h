#include <Piece.h>

struct Bishop : Piece
    {
        Bishop(int id, const Position &position, Color color) : Piece(id, "Bishop", position, color) {}
        std::vector<NextMove> nextPossibleMoves(BoardPtr board) override
        {
            std::vector<NextMove> nextMove;
            ruleDiagonalMove(board, nextMove);
            return nextMove;
        }

        auto getDirections()
        {
            std::vector<Direction> directions = {{1, -1}, {1, 1}, {-1, 1}, {-1, -1}};
            return directions;
        }

        // Le fou se déplace en suivant les diagonales, on remarque qu'il se déplace toujours sur les cases d'une même couleur.
        void ruleDiagonalMove(BoardPtr board, std::vector<NextMove> &nextMove)
        {
            auto directions = getDirections();
            auto action = Action(Actions::toMove, nullptr);

            for (auto direction : directions)
            {
                Position targetPosition(position_.row_ + direction.row_, position_.col_ + direction.col_);
                while (true)
                {
                    if (!isInRange(board, targetPosition))
                    {
                        break;
                    }
                    else if (!isEmptyCell(board, targetPosition) && !isEnemy(board, targetPosition))
                    {
                        break;
                    }
                    else if (isEmptyCell(board, targetPosition))
                    {
                        nextMove.push_back(NextMove(action, targetPosition));
                        targetPosition = Position(targetPosition.row_ + direction.row_, targetPosition.col_ + direction.col_);
                    }
                    else if (!isEmptyCell(board, targetPosition) && isEnemy(board, targetPosition))
                    {
                        action = Action(Actions::toTake, getPiece(board, targetPosition));
                        nextMove.push_back(NextMove(action, targetPosition));
                        break;
                    }
                }
            }
        }

        // Fonction pour savoir si le fou met en echec le roi
        bool doesItCheck(BoardPtr board, const Position &targetPositionKing) override
        {
            // Vérifier si les deux positions sont sur la même diagonale
            if (abs(position_.col_ - targetPositionKing.col_) != abs(position_.row_ - targetPositionKing.row_))
            {
                return false;
            }
            // Calcul de la direction du mouvement
            int dx = (targetPositionKing.col_ > position_.col_) ? 1 : -1;
            int dy = (targetPositionKing.row_ > position_.row_) ? 1 : -1;
            // Parcourir les cases entre le fou et la destination (excluant la destination)
            int x = position_.col_ + dx;
            int y = position_.row_ + dy;

            while (x != targetPositionKing.col_ && y != targetPositionKing.row_)
            {
                Position intermediatePosition(y, x);
                if (!isEmptyCell(board, intermediatePosition))
                {
                    return false; // Un obstacle bloque la diagonale
                }
                x += dx;
                y += dy;
            }

            return true;
        }
    };