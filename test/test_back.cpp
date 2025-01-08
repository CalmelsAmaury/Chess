
#include <gtest/gtest.h>
#include "model.h"
#include "ChessAlgorithm.h"
#include "PrintingBoard.h"
using namespace chess;


class Shape
{
    public:
    virtual double calculSurface() = 0;
    
};

class Rectangle : public Shape
{
    public:
    Rectangle(double height, double width) {height_ = height, width_ = width;}

    double calculSurface() override
    {
        return height_*width_;
    }

    private:
    double height_;
    double width_;
};

class Circle : public Shape
{
    public:
    Circle(double rayon) { rayon_ = rayon;}

    double calculSurface() override
    {
        return (rayon_*rayon_)*3.14;
    }

    private:
    double rayon_ = 0;
};

class Carre : public Rectangle
{
    public:
    Carre(double height, double width)
    : Rectangle(height, width) {}
};



//template <typename t> 
//struct MySharePtr
//{
//    MySharePtr(t* ptr) :ptr_{ptr} {}
//    ~ MySharePtr(){
//        delete ptr_;
//        ptr_ = nullptr;
//    }
//
//    private:
//    t* ptr_;
//};



using ShapePtr = std::shared_ptr<Shape>;

double calculSurfaces(std::vector<ShapePtr> shapes)
{
    double surfaceTotal = 0;
    for(ShapePtr shape: shapes)
    {
        surfaceTotal += shape->calculSurface();
    }
    return surfaceTotal;
}

TEST(All, Surface)
{
    ShapePtr c = std::make_shared<Shape>(new Circle(10));
    ShapePtr d = std::make_shared<Shape>(new Rectangle(10,2));
    ShapePtr e = std::make_shared<Shape>(new Circle(5));
    ShapePtr f = std::make_shared<Shape>(new Carre(5,12.4));
    std::vector<ShapePtr> shapes{c, d, e, f};
    double surfaces = calculSurfaces(shapes);

    EXPECT_TRUE(surfaces == 474.50);
}


TEST(Knight, KnightBestMove)
{
    Shape* c = new Circle(10);
    double surface = c->calculSurface();

    EXPECT_TRUE(surface == 314);
}

TEST(Rect, rect)
{
    Shape* c = new Rectangle(10, 15);
    double surface = c->calculSurface();

    EXPECT_TRUE(surface == 150);
}

TEST(Carre, Carre)
{
    Shape* c = new Carre(10, 15);
    double surface = c->calculSurface();

    EXPECT_TRUE(surface == 150);
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}