#pragma once

/*
normal vector
{:}    ->  (1.0, 2.0)

column vector
{:c}   ->  [1.0]
       ->  [2.0]

normalized vector
{:n} (v=(3,4)) ->([0.6], [0.8])

polar coordinates
{:p}   ->  (r=2.24, θ=63.4°)

*/

class Vector
{
   public:
    Vector(int x, int y) : x_(x), y_(y)
    {
    }

    const int x() const
    {
        return x_;
    }

    const int y() const
    {
        return y_;
    }

   private:
    int x_;
    int y_;
};
