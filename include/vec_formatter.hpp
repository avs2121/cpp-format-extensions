#pragma once

#include <cmath>
#include <format>
#include <numbers>

#include "vector.hpp"

template <typename T>
struct std::formatter<Vector<T>>
{
   private:
    size_t spec_len{0};
    char spec_buf[32]{};
    bool isColumn = false;      // [:c / C]
    bool isNormalized = false;  // [:n / N]
    bool isPolar = false;       // [:p / P]

   public:
    constexpr auto parse(std::format_parse_context& ctx)  // parses spec format and stores state
    {
        auto it = ctx.begin();  // points to first character of spec e.g -> '*'
        auto end = ctx.end();   // points 1 past the ctx buffer.

        if (it == end)  // stop if empty (when spec is {}).
        {
            return it;
        }

        auto type_it = it;
        while (type_it != end && *type_it != '}')  // advance until '}' to find end.
        {
            ++type_it;
        }

        auto ranged_end = type_it;  // pointing to last '}'

        if (ranged_end != it)
        {
            auto last = ranged_end - 1;  // pointing to last character before '}'
            if (*last == 'c' || *last == 'C')
            {
                isColumn = true;
                ranged_end = last;
            }
            else if (*last == 'n' || *last == 'N')
            {
                isNormalized = true;
                ranged_end = last;
            }
            else if (*last == 'p' || *last == 'P')
            {
                isPolar = true;
                ranged_end = last;
            }
        }

        // the range [it, ranged_end] only contains spec parts now.
        // Store the spec as a string, in a fixed-size char array.
        spec_len = 0;
        for (auto i = it; i != ranged_end; ++i)
        {
            spec_buf[spec_len++] = *i;
        }

        return type_it;  // returning iterator pointing to '}'
    }

    auto format(const Vector<T>& c, std::format_context& ctx) const  // read the interal state at runtime
    {
        std::string result;

        if (isColumn)
        {
            result = std::format("\n([{}]\n[{}])", c.x(), c.y());
        }
        else if (isNormalized)
        {
            double mag = sqrt(c.x() * c.x() + c.y() * c.y());
            result = std::format("({},{})", c.x() / mag, c.y() / mag);
        }
        else if (isPolar)
        {
            auto mag = sqrt(c.x() * c.x() + c.y() * c.y());
            // atan2 internally converts int to double, and then convert to degrees
            auto angle = atan2(c.y(), c.x()) * (180.0 / std::numbers::pi_v<float>);

            result = std::format("({},{} deg)", mag, angle);
        }
        else
        {
            result = std::format("({},{})", c.x(), c.y());
        }
        // reconstruct the format string
        std::string fmt = std::string{"{:" + std::string(spec_buf, spec_len) + "}"};

        // takes the build string (fmt) wraps the result as the argument to format, and into the
        // output iterator (ctx)
        return std::vformat_to(ctx.out(), fmt, std::make_format_args(result));
    }
};