#pragma once

#include <cstdint>

class Color
{
   public:
    Color(uint8_t r, uint8_t g, uint8_t b) : r_(r), g_(g), b_(b)
    {
    }

    uint8_t r() const
    {
        return r_;
    }
    uint8_t g() const
    {
        return g_;
    }
    uint8_t b() const
    {
        return b_;
    }

   private:
    uint8_t r_;
    uint8_t g_;
    uint8_t b_;
};
