#include "Piece.h"

struct Pawn : Piece
    {
        bool isFirstMove = true;
        Pawn(int id, const Position &position, Color color) : Piece(id, "Pawn", position, color) {}

        // calcule la liste des possibles positions de la pièce
        std::vector<NextMove> nextPossibleMoves(BoardPtr board) override
        {
            std::vector<NextMove> nextMove;
            ruleMoveOnCases(board, nextMove);
            ruleMoveTwoCases(board, nextMove);
            ruleMoveDiagonal(board, nextMove);
            rulePriseEnPassant(board, nextMove);
            rulePromote(board, nextMove);
            return nextMove;
        }

        // avance de 1 dans la direction de la couleur
        int getDirection()
        {
            int direction(color_ == Color::white ? +1 : -1);
            return direction;
        }

        // Le pion se déplace droit devant lui (vers la 8e rangée pour les Blancs et vers la 1re rangée pour les Noirs),
        // d'une seule case à chaque coup et sans jamais pouvoir reculer7.
        void ruleMoveOnCases(BoardPtr board, std::vector<NextMove> &nextMove)
        {
            auto direction = getDirection();
            auto position = Position(position_.row_ + direction, position_.col_);
            auto action = Action(Actions::toMove, nullptr);
            if (isInRange(board, position) && isEmptyCell(board, position))
            {
                nextMove.push_back(NextMove(action, position));
            }
        }

        // Lors de son premier déplacement (alors qu'il est sur sa case initiale), un pion peut avancer de deux cases en un seul coup
        void ruleMoveTwoCases(BoardPtr board, std::vector<NextMove> &nextMove)
        {
            if (!isFirstMove)
                return;
            auto direction = getDirection();
            auto action = Action(Actions::toMoveTwo, nullptr);
            if (color_ == Color::white && position_.row_ == 1 || color_ == Color::black && position_.row_ == 6)
            {
                auto position = Position(position_.row_ + direction * 2, position_.col_);
                if (isInRange(board, position) && isEmptyCell(board, position))
                {
                    nextMove.push_back(NextMove(action, position));
                }
            }
        }

        // Il ne peut capturer une pièce adverse que si elle se trouve à une case en diagonale de lui dans son sens de déplacement
        void ruleMoveDiagonal(BoardPtr board, std::vector<NextMove> &nextMove)
        {
            auto direction = getDirection();
            auto positionLeft = Position(position_.row_ + direction, position_.col_ - 1);
            auto positionRight = Position(position_.row_ + direction, position_.col_ + 1);
            auto action = Action(Actions::toTake, nullptr);

            if (isInRange(board, positionLeft) && !isEmptyCell(board, positionLeft) && isEnemy(board, positionLeft))
            {
                action.piece_ = getPiece(board, positionLeft);
                nextMove.push_back(NextMove(action, positionLeft));
            }
            if (isInRange(board, positionRight) && !isEmptyCell(board, positionRight) && isEnemy(board, positionRight))
            {
                action.piece_ = getPiece(board, positionRight);
                nextMove.push_back(NextMove(action, positionRight));
            }
        }

        // Ce dernier a la possibilité de prendre comme si le coup de début n'avait été que d'une case. Cette prise en passant ne peut se faire qu'en réponse immédiate à l'avance double
        void rulePriseEnPassant(BoardPtr board, std::vector<NextMove> &nextMove)
        {
            auto direction = getDirection();
            auto positionLeftDiagonal = Position(position_.row_ + direction, position_.col_ - 1);
            auto positionRightDiagonal = Position(position_.row_ + direction, position_.col_ + 1);
            auto positionLeft = Position(position_.row_, position_.col_ - 1);
            auto positionRight = Position(position_.row_, position_.col_ + 1);
            auto action = Action(Actions::toTake, nullptr);

            if (color_ == Color::white && position_.row_ == 4 || color_ == Color::black && position_.row_ == 3)
            {
                if (isInRange(board, positionLeftDiagonal) && isInRange(board, positionLeft) && isEmptyCell(board, positionLeftDiagonal) && !isEmptyCell(board, positionLeft) && isEnemy(board, positionLeft) && isPawn(board, positionLeft))
                {
                    nextMove.push_back(NextMove(action, positionLeftDiagonal));
                }
                if (isInRange(board, positionRightDiagonal) && isInRange(board, positionRight) && isEmptyCell(board, positionRightDiagonal) && !isEmptyCell(board, positionRight) && isEnemy(board, positionRight) && isPawn(board, positionRight))
                {
                    nextMove.push_back(NextMove(action, positionRightDiagonal));
                }
            }
        }

        // Quand le pion arrive sur la dernière rangée, il doit se transformer en une pièce de son camp de valeur supérieure, au choix du joueur : dame, tour, fou ou cavalier.
        void rulePromote(BoardPtr board, std::vector<NextMove> &nextMove)
        {
            auto promotedPiece = std::make_shared<Queen>(id_, position_, color_);
            auto action = Action(Actions::toPromote, promotedPiece);
            if (color_ == Color::white && position_.row_ == 7 || color_ == Color::black && position_.row_ == 0)
            {
                nextMove.push_back(NextMove(action, position_));
            }
        }

        // Fonction pour savoir si le pion met en echec le roi
        bool doesItCheck(BoardPtr board, const Position &targetPositionKing) override
        {
            auto direction = getDirection();
            Position targetPositionLeftDiagonal(position_.row_ + direction, position_.col_ - 1);
            Position targetPositionRightDiagonal(position_.row_ + direction, position_.col_ + 1);

            if ((targetPositionLeftDiagonal.col_ == targetPositionKing.col_ && targetPositionLeftDiagonal.row_ == targetPositionKing.row_) || (targetPositionRightDiagonal.col_ == targetPositionKing.col_ && targetPositionRightDiagonal.row_ == targetPositionKing.row_))
            {
                return true;
            }
            return false;
        }
    };