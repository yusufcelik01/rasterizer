#include "Color.h"
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

Color::Color() {}

Color::Color(double r, double g, double b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}

Color::Color(const Color &other)
{
    this->r = other.r;
    this->g = other.g;
    this->b = other.b;
}

ostream& operator<<(ostream& os, const Color& c)
{
    os << fixed << setprecision(0) << "rgb(" << c.r << ", " << c.g << ", " << c.b << ")";
    return os;
}

Color Color::operator-(Color c)
{
    Color temp;
    temp.r = this->r - c.r;
    temp.g = this->g - c.g;
    temp.b = this->b - c.b;

    return temp;
}

Color Color::operator/(int n)
{
    Color temp;
    temp.r = this->r /n;
    temp.g = this->g /n;
    temp.b = this->b /n;

    return temp;
}


Color Color::operator+=(Color c)
{
    this->r = this->r + c.r;
    this->g = this->g + c.g;
    this->b = this->b + c.b;

    return *this;
}
Color Color::operator+(Color c)
{
    Color temp;
    temp.r = this->r + c.r;
    temp.g = this->g + c.g;
    temp.b = this->b + c.b;

    return temp;
}

Color Color::cround()
{
    Color temp;
    temp.r = round(this->r);
    temp.g = round(this->g);
    temp.b = round(this->b);
    return temp;
}

Color Color::operator*(double x){
    Color temp;
    temp.r = this->r*x;
    temp.g = this->g*x;
    temp.b = this->b*x;

    return temp;
}
