#pragma once

#include <concepts>

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
template <typename T>
concept vector_scalar = std::is_floating_point_v<T> || std::same_as<T, int> || std::same_as<T, long>;

template <vector_scalar T>
class Vector
{
   public:
    Vector(T x, T y) : x_(x), y_(y)
    {
    }

    T x() const
    {
        return x_;
    }

    T y() const
    {
        return y_;
    }

   private:
    T x_;
    T y_;
};
