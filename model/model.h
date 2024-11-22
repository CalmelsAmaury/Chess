#pragma once
#include <vector>
#include <string>
#include <memory>


namespace chess
{
    struct Position{
        int row;
        int col;

    };

    struct Piece{
        Position position;
        std::string name;
    };

    struct Tour : Piece{
        
    };

    struct Bishop : Piece{
        
    };




    /**
     * construis le model compose de pions, tours, etc
     */
    class Model
    {
        public:
        bool load(const std::string &path)
        {
            // code pour loader le model
        }
        std::vector<std::shared_ptr<Piece>> pieces_;
    };

}
