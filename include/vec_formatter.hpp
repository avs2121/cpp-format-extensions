#pragma once

#include <cmath>
#include <format>
#include <numbers>

#include "vector.hpp"

template <vector_scalar... Args>
struct std::formatter<Vector<Args...>>
{
   private:
    size_t spec_len{0};
    char spec_buf[32]{};
    bool isColumn = false;        // [:c / C]
    bool isNormalized = false;    // [:n / N]
    bool isPolar = false;         // [:p / P] - defaults to degrees, if no specification provided
    bool isPolarRadians = false;  // [:pR / pr / PR / Pr]
                                  // Degree output will be [:pD / pd / PD / Pd]

   public:
    constexpr auto parse(std::format_parse_context& ctx)
    {
        auto it = ctx.begin();
        auto end = ctx.end();

        if (it == end)
        {
            return it;
        }

        auto type_it = it;
        while (type_it != end && *type_it != '}')
        {
            ++type_it;
        }

        auto ranged_end = type_it;

        if (ranged_end != it)
        {
            auto last = ranged_end - 1;
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
            else if (ranged_end - it >= 2 && (*(last - 1) == 'p' || *(last - 1) == 'P'))
            {
                if (*last == 'r' || *last == 'R')
                {
                    isPolar = true;
                    isPolarRadians = true;
                    ranged_end = last - 1;
                }
                if (*last == 'd' || *last == 'D')
                {
                    isPolar = true;
                    ranged_end = last - 1;
                }
            }
        }

        spec_len = 0;
        for (auto i = it; i != ranged_end; ++i)
        {
            spec_buf[spec_len++] = *i;
        }

        return type_it;
    }

    auto format(const Vector<Args...>& v, std::format_context& ctx) const  // read the interal state at runtime
    {
        std::string result;

        if (isColumn)
        {
            std::apply(
                [&result](const auto& first, const auto&... restArgs)
                {
                    result += "(";
                    result += std::format("[{}]", first);
                    ((result += std::format("\n[{}]", restArgs)), ...);  // pack expansion
                    result += ")";
                },
                v.as_tuple());
        }
        else if (isNormalized)
        {
            if constexpr (Vector<Args...>::dimension == 3)
            {
                double mag = std::sqrt(v.x() * v.x() + v.y() * v.y() + v.z() * v.z());
                result = std::format("({},{},{})", v.x() / mag, v.y() / mag, v.z() / mag);
            }
            else if constexpr (Vector<Args...>::dimension == 2)
            {
                double mag = std::sqrt(v.x() * v.x() + v.y() * v.y());
                result = std::format("({},{})", v.x() / mag, v.y() / mag);
            }
        }
        else if (isPolar)
        {
            auto mag = std::sqrt(v.x() * v.x() + v.y() * v.y());
            // atan2 internally converts int to double, and then convert to degrees
            double angle{0};
            bool degrees = true;
            if (isPolarRadians)
            {
                angle = std::atan2(v.y(), v.x());
                degrees = false;
            }
            else
            {
                angle = std::atan2(v.y(), v.x()) * (180.0 / std::numbers::pi_v<double>);
            }

            result = std::format("({},{} {})", mag, angle, degrees ? "deg" : "rad");
        }
        else
        {
            if constexpr (Vector<Args...>::dimension == 3)
            {
                result = std::format("({},{},{})", v.x(), v.y(), v.z());
            }
            else if constexpr (Vector<Args...>::dimension == 2)
            {
                result = std::format("({},{})", v.x(), v.y());
            }
        }

        std::string fmt = std::string{"{:" + std::string(spec_buf, spec_len) + "}"};

        return std::vformat_to(ctx.out(), fmt, std::make_format_args(result));
    }
};