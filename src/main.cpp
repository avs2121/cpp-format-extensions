#include <cmath>
#include <iostream>

#include "color_formatter.hpp"
#include "duration_formatter.hpp"
#include "vec_formatter.hpp"

/*
Input to handle :
std::format("col {}\n", Color{100, 255, 255}); -> col(100, 255, 255)
std::format("col {:h}\n", Color{100, 255, 255}); -> col #64c8ff

{:*>10} -> fills the width to the left with '*' if empty -> *****hello
{:*<10} -> fills the width to the right with '*' if empty ->  hello*****
{:*^10} -> fills the width to right and left with '*' if empty -> **hello***

format specification -> {'[arg-id]':'[format-spec]'}
  -> {0:*>10} -> first argument, fills width with '*' to the left of the argument

*/

int main()
{
    Color c1(0, 0, 1);
    std::cout << std::format("standard color rgb {}\n", c1);         // standard output
    std::cout << std::format("lower case hex color {:h}\n", c1);     // lower case hex
    std::cout << std::format("upper case hex color {:H}\n", c1);     // upper case hex
    std::cout << std::format("specifier last color {:>20H}\n", c1);  // type specifier in last position
    std::cout << std::format("fill width color {:*^20}\n", c1);      // fill width 20 with *

    Duration d(4 * pow(10, 10));                         // 40.000.000.000 millarder ms
    std::cout << std::format("Duration {}\n", d);        // standard output y,d,h,m,s,ms
    std::cout << std::format("Duration {:ms}\n", d);     // ms
    std::cout << std::format("Duration {:s}\n", d);      // sec
    std::cout << std::format("Duration {:m}\n", d);      // minute
    std::cout << std::format("Duration {:h}\n", d);      // hour
    std::cout << std::format("Duration {:d}\n", d);      // day
    std::cout << std::format("Duration {:y}\n", d);      // year
    std::cout << std::format("Duration {:*^20y}\n", d);  // year with fill

    Vector<int> v(3, 4);
    std::cout << std::format("Standard vector: {}\n", v);
    std::cout << std::format("Column vector: {:c}\n", v);
    std::cout << std::format("Column vector: {:C}\n", v);
    std::cout << std::format("Normalized vector: {:n}\n", v);
    std::cout << std::format("Normalized vector: {:N}\n", v);
    std::cout << std::format("Polar coordinates: {:p}\n", v);
    std::cout << std::format("Polar coordinates: {:P}\n", v);
    std::cout << std::format("Polar coordinates: {:Pr}\n", v);
    std::cout << std::format("Polar coordinates: {:PR}\n", v);
    std::cout << std::format("Polar coordinates: {:Pd}\n", v);
    std::cout << std::format("Polar coordinates: {:PD}\n", v);
}
