#pragma once

#include <format>

#include "color.hpp"

/*
Primary template looks like template<Class T, class CharT> -> struct formatter.
Here T is the type we are formatting, and charT is the character type of the string.
  - Because the character type can be both char and wchar -> important distinction.

So basically says std::formatter<Color, char> -> The formatter formats Color when
the format string is made out of char

The Template <> signals full specilization of std::formatter for the combination of <T =
Color, CharT = char>
*/

template <>
struct std::formatter<Color>
{
    bool isHexUpper = false;
    bool isHexLower = false;

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
            if (*last == 'h')
            {
                isHexLower = true;
                ranged_end = last;
            }
            else if (*last == 'H')
            {
                isHexUpper = true;
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

    auto format(const Color& c, std::format_context& ctx) const  // read the interal state at runtime
    {
        std::string result;
        if (isHexUpper || isHexLower)
        {
            uint32_t val = (static_cast<uint32_t>(c.r()) << 16) | (static_cast<uint32_t>(c.g()) << 8) | static_cast<uint32_t>(c.b());

            if (isHexUpper)
                result = std::format("#{:06X}", val);  // 06 for width

            else
                result = std::format("#{:06x}", val);  // 06 for width
        }
        else
            result = std::format("({}, {}, {})", c.r(), c.g(), c.b());

        // reconstruct the format string
        std::string fmt = std::string{"{:" + std::string(spec_buf, spec_len) + "}"};

        // takes the build string (fmt) wraps the result as the argument to format, and into the
        // output iterator (ctx)
        return std::vformat_to(ctx.out(), fmt, std::make_format_args(result));
    }

   private:
    size_t spec_len{0};
    char spec_buf[32]{};
};