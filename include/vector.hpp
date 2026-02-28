#pragma once
#include <concepts>
#include <tuple>

/*
2D :

normal vector
{:}    ->  (1.0, 2.0)

column vector
{:c}   ->  [1.0]
       ->  [2.0]

normalized vector
{:n} (v=(3,4)) ->([0.6], [0.8])

polar coordinates
{:p}   ->  (r=2.24, θ=63.4°)

3D :

default output (x, y, z)
normalized
Spherical coordinates {:p} → (r, θ, φ)

*/

// Concept constraining T to either a floating point type, int, or long.
template <typename T>
concept vector_scalar = std::is_floating_point_v<T> || std::same_as<T, int> || std::same_as<T, long>;

// variadic template: accepts any number of Args, each must satisfy vector_scalar.
// requires clause: fold expression using && over the pack, ensuring every Arg is the
// same type as the first Arg (tuple_element_t<0>), preventing mixed types like <int, double>.
template <vector_scalar... Args>
    requires(std::is_same_v<std::tuple_element_t<0, std::tuple<Args...>>, Args> && ...)

class Vector
{
   public:
    static constexpr std::size_t dimension = sizeof...(Args);

    Vector(Args... args) : data(args...)
    {
    }

    auto x() const
        requires(std::tuple_size_v<std::tuple<Args...>> >= 1)  // could also use (sizeof...(Args) >= 1)

    {
        return std::get<0>(data);
    }

    auto y() const
        requires(std::tuple_size_v<std::tuple<Args...>> >= 2)

    {
        return std::get<1>(data);
    }

    auto z() const
        requires(std::tuple_size_v<std::tuple<Args...>> >= 3)
    {
        return std::get<2>(data);
    }

    const auto& as_tuple() const
    {
        return data;
    }

   private:
    std::tuple<Args...> data;
};
