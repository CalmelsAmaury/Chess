#pragma once
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <iostream>
#include <sstream>
#include <array>

namespace chess
{

    enum class Color
    {
        white,
        black
    };

    struct Position
    {
        explicit Position(int row, char col) : row_(row), col_(static_cast<int>(col - 'a'))
        {
        }

        explicit Position(int row, int col) : row_(row), col_(col)
        {
        }

        explicit Position() : row_(-1), col_(-1)
        {
        }

        int row_;
        int col_;
    };

    class Piece;

    using PiecePtr = std::shared_ptr<Piece>;

    struct BoardPositions
    {
        PiecePtr piece;
        std::vector<Position> nextMove;
    };

    using Board = std::array<std::array<BoardPositions, 8>, 8>;
    using BoardPtr = std::shared_ptr<Board>;

    enum class Actions
    {
        none,
        toMove,
        toMoveTwo,
        toTake,
        toCheck,
        toPromote
    };

    struct Action
    {
        Action(const Actions &action, PiecePtr piece) : actions_(action), piece_(piece) {}
        Actions actions_;
        PiecePtr piece_;
    };

    struct NextMove
    {
        NextMove(Action &action, Position &position) : action_(action), position_(position) {}
        NextMove() : action_(Actions::none, nullptr) {}
        Action action_;
        Position position_;
        int weight_{0};
    };

    struct Direction
    {
        int row_;
        int col_;
    };

    struct Piece
    {
        Piece(int id, const std::string &pieceName, const Position &piecePosition, const Color &color) : id_(id), name_(pieceName), position_(piecePosition), color_(color) {}
        virtual ~Piece() = default;
        std::string toString()
        {
            std::ostringstream oss;
            oss << id_ << "," << name_ << "," << (color_ == Color::white ? 'w' : 'b') << "," << position_.col_ << "," << position_.row_;
            return oss.str();
        }

        static PiecePtr getPiece(BoardPtr board, const Position &pos)
        {
            return (*board)[pos.row_][pos.col_].piece;
        }

        static bool isEmptyCell(BoardPtr board, const Position &pos)
        {
            auto piece = (*board)[pos.row_][pos.col_].piece;
            if (!piece)
                return true;
            return false;
        }

        bool isPawn(BoardPtr board, const Position &pos)
        {
            auto piece = (*board)[pos.row_][pos.col_].piece;
            if (piece->name_ == "Pawn")
                return true;
            return false;
        }

        bool isEnemy(BoardPtr board, const Position &pos)
        {
            auto piece = (*board)[pos.row_][pos.col_].piece;
            if (piece->color_ != color_)
            {
                return true;
            }
            return false;
        }

        bool isInRange(BoardPtr board, const Position &pos)
        {
            if (pos.col_ >= 0 && pos.col_ < 8 && pos.row_ >= 0 && pos.row_ < 8)
            {
                return true;
            }
            return false;
        }

        virtual bool doesItCheck(BoardPtr board, Position &targetPosition) { return {}; }

        virtual int getWeight(Actions action, PiecePtr targetedPiece)
        {
            int pieceValue = 0;
            if (targetedPiece)
            {
                if (targetedPiece->name_ == "Pawn")
                    pieceValue = 1;
                else if (targetedPiece->name_ == "Knight" || targetedPiece->name_ == "Bishop")
                    pieceValue = 3;
                else if (targetedPiece->name_ == "Rook")
                    pieceValue = 5;
                else if (targetedPiece->name_ == "Queen")
                    pieceValue = 9;
            }

            switch (action)
            {
            case Actions::none:
                return -1;
            case Actions::toMove:
                return 0;
            case Actions::toMoveTwo:
                return 0;
            case Actions::toCheck:
                return 1;
            case Actions::toTake:
                return pieceValue;
            case Actions::toPromote:
                return 9;
            }
            return -1;
        }

        virtual NextMove bestMove(std::vector<NextMove> &nextPositions)
        {
            int weight = 0;
            int index = 0;
            int i = 0;
            for (auto &move : nextPositions)
            {
                int v = getWeight(move.action_.actions_, move.action_.piece_);
                if (v > weight)
                {
                    index = i;
                    weight = v;
                }
                i++;
            }
            nextPositions[index].weight_ = weight;
            return nextPositions[index];
        }

        virtual std::vector<NextMove> nextPossibleMoves(BoardPtr board) { return {}; }
        Position position_;
        std::string name_;
        Color color_;
        int id_;
    };

    struct Rook : Piece
    {
        Rook(int id, const Position &position, Color color) : Piece(id, "Rook", position, color) {}
        std::vector<NextMove> nextPossibleMoves(BoardPtr board) override
        {
            std::vector<NextMove> nextMove;
            ruleMove(board, nextMove);
            return nextMove;
        }

        auto getDirections()
        {
            std::vector<Direction> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
            return directions;
        }

        // La tour se déplace en suivant les colonnes ou les rangées.
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
    };

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

        // Fonction pour trouver les cases entre le fou et une autre position
        bool doesItCheck(BoardPtr board, Position &targetPositionKing) 
        {
            // Vérifier si les deux positions sont sur la même diagonale
            if (abs(position_.col_ - targetPositionKing.col_) != abs(position_.row_ - targetPositionKing.row_ )) 
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
                while(true)
                {
                    if(isInRange(board, targetPosition))
                    {
                        if (isEmptyCell(board, targetPosition))
                        {   
                            if(!ruleIsCheck(board, targetPosition))
                            {
                                nextMove.push_back(NextMove(action, targetPosition));
                                break;
                            }
                        }

                        else if (!isEmptyCell(board, targetPosition) && isEnemy(board, targetPosition))
                        {
                            if(!ruleIsCheck(board, targetPosition))
                            {
                                action = Action(Actions::toTake, getPiece(board, targetPosition));
                                nextMove.push_back(NextMove(action, targetPosition));
                                break;
                            }
                            
                        }
                        else
                            break;
                    }
                    else
                        break;
                }
            }
        }

        bool ruleIsCheck(BoardPtr board, Position &targetPosition)
        {
            for (int row = 0; row < 8; row++)
            {
                for (int col = 0; col < 8; col++)
                {
                    auto piece = (*board)[row][col].piece;
                    if(piece && isEnemy(board, Position(row, col)))
                    {
                        if(piece->doesItCheck(board, targetPosition))
                        {
                            return true;
                        }
                    }
                    continue;
                }
            }
            return false;
        }
    };

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
    };

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
    };

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

        // ce dernier a la possibilité de prendre comme si le coup de début n'avait été que d'une case. Cette prise en passant ne peut se faire qu'en réponse immédiate à l'avance double
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
                if (isInRange(board, positionRightDiagonal) && isInRange(board, positionRight) && isEmptyCell(board, positionRightDiagonal) && !isEmptyCell(board, positionRight) && isEnemy(board, positionRight) && isPawn(board, positionLeft))
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
    };

    /**
     * construit le model compose de pions, tours, etc
     */
    class Model
    {

    public:
        bool load(const std::string &path)
        {
            std::ifstream file(path);

            // Check if the file was opened successfully
            if (!file.is_open())
            {
                std::cerr << "Error: Could not open the file " << path << std::endl;
                return false; // Exit with an error code
            }

            // Read the file line by line
            std::string line;
            while (std::getline(file, line))
            {
                // Print each line to the console
                std::cout << line << std::endl;
                auto v = splitString(line, ',');
                auto row = std::stoi(v[4]) - 1;
                auto col = v[3][0];
                auto color = v[2][0];
                auto id = std::stoi(v[0]);

                if (v[1] == "Queen")
                {

                    auto queen = std::make_shared<Queen>(id, Position(row, col), color == 'w' ? Color::white : Color::black);
                    pieces_.push_back(queen);
                }
                else if (v[1] == "King")
                {

                    auto king = std::make_shared<King>(id, Position(row, col), color == 'w' ? Color::white : Color::black);
                    pieces_.push_back(king);
                }
                else if (v[1] == "Rook")
                {

                    auto rook = std::make_shared<Rook>(id, Position(row, col), color == 'w' ? Color::white : Color::black);
                    pieces_.push_back(rook);
                }
                else if (v[1] == "Knight")
                {

                    auto knight = std::make_shared<Knight>(id, Position(row, col), color == 'w' ? Color::white : Color::black);
                    pieces_.push_back(knight);
                }
                else if (v[1] == "Bishop")
                {

                    auto bishop = std::make_shared<Bishop>(id, Position(row, col), color == 'w' ? Color::white : Color::black);
                    pieces_.push_back(bishop);
                }
                else if (v[1] == "Pawn")
                {

                    auto pawn = std::make_shared<Pawn>(id, Position(row, col), color == 'w' ? Color::white : Color::black);
                    pieces_.push_back(pawn);
                }
            }

            // Close the file
            file.close();

            return true;
        }

        PiecePtr GetQueen(Color color)
        {
            for (PiecePtr piece : pieces_)
            {
                if (piece->name_ == "Queen" && piece->color_ == color)
                {
                    return piece;
                }
            }
            return nullptr;
        }

        PiecePtr GetPiece(int id)
        {
            for (PiecePtr pieceId : pieces_)
            {
                if (pieceId->id_ == id)
                {
                    return pieceId;
                }
            }
            return nullptr;
        }

        std::vector<std::string> splitString(const std::string &str, char delimiter)
        {
            std::vector<std::string> tokens;
            std::stringstream ss(str);
            std::string token;

            while (std::getline(ss, token, delimiter))
            {
                tokens.push_back(token);
            }

            return tokens;
        }

        bool save()
        {
            int i = 1;
            std::string fileName = "coupNumero" + std::to_string(i) + ".txt";

            std::ofstream file(fileName);

            if (!file.is_open())
            {
                std::cerr << "Error: Could not open the file " << fileName << std::endl;
                return false;
            }

            for (auto &piece : pieces_)
            {
                file << piece->toString() << "\n";
            }

            file.close();

            return true;
        }

        BoardPtr CreateBoard()
        {
            BoardPtr board = std::shared_ptr<Board>(new Board());

            for (auto piece : pieces_)
            {
                int col = piece->position_.col_;
                int row = piece->position_.row_;
                std::cout << piece->toString() << std::endl;
                if (col >= 0 && col < 8 && row >= 0 && row < 8)
                {
                    BoardPositions &bp = ((*board)[row][col]);
                    bp.piece = piece;
                }
                else
                {
                    throw std::out_of_range("Piece position out of board bounds");
                }
            }

            return board;
        }

        std::string WHITE_COLOR = "\033[36m";
        std::string BLACK_COLOR = "\033[91m";
        std::string PINK_COLOR = "\033[32m";
        std::string RESET_COLOR = "\033[0m";
        std::string WHITE_BACKGROUND = "\033[100m";
        std::string BLACK_BACKGROUND = "\033[10m";

        void PrintBoard(BoardPtr board)
        {
            std::cout << PINK_COLOR << "     0  1  2  3  4  5  6  7\n" << RESET_COLOR;
            std::cout << PINK_COLOR << "     A  B  C  D  E  F  G  H\n" << RESET_COLOR;
            for (int i = 7; i >= 0; i--)
            {
                std::cout << PINK_COLOR << i << "|" << i + 1 << " " << RESET_COLOR;
                for (int j = 0; j <= 7; j++)
                {
                    std::string background_color = (i + j) % 2 == 0 ? BLACK_BACKGROUND : WHITE_BACKGROUND;
                    std::cout << background_color;

                    auto &cell = (*board)[i][j];
                    if (cell.piece)
                    {
                        auto &piece = cell.piece;
                        char symbol = piece->name_ == "King"     ? (piece->color_ == Color::white ? 'K' : 'k')
                                      : piece->name_ == "Queen"  ? (piece->color_ == Color::white ? 'Q' : 'q')
                                      : piece->name_ == "Rook"   ? (piece->color_ == Color::white ? 'R' : 'r')
                                      : piece->name_ == "Bishop" ? (piece->color_ == Color::white ? 'B' : 'b')
                                      : piece->name_ == "Knight" ? (piece->color_ == Color::white ? 'N' : 'n')
                                      : piece->name_ == "Pawn"   ? (piece->color_ == Color::white ? 'P' : 'p')
                                                                 : '?';
                        if (piece->color_ == Color::white)
                        {
                            std::cout << WHITE_COLOR << " " << symbol << " " << RESET_COLOR;
                        }
                        else if (piece->color_ == Color::black)
                        {
                            std::cout << BLACK_COLOR << " " << symbol << " " << RESET_COLOR;
                        }
                    }
                    else
                    {
                        std::cout << "   ";
                    }
                    std::cout << RESET_COLOR;
                }
                std::cout << PINK_COLOR << " " << i + 1 << "|" << i << " " << RESET_COLOR;
                std::cout << "\n";
            }
            std::cout << PINK_COLOR << "     A  B  C  D  E  F  G  H\n" << RESET_COLOR;
            std::cout << PINK_COLOR << "     0  1  2  3  4  5  6  7\n" << RESET_COLOR;
        }

        std::vector<PiecePtr> pieces_;
    };

}
