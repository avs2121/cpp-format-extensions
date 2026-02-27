#pragma once

#include <cstring>
#include <format>

#include "duration.hpp"

/*
wrap a uint64_t ms

{:ms}  ->  15023ms
{:s}   ->  15.023s
{:m}   ->  0m 15s
{:h}   ->  0h 0m 15s
{:f}   ->  00:00:15.023 - removed

convertion from ms to years from maximum duration using uint64_t :
18,446,744,073,709,551,615 / 1000 / 60 / 60 / 24 / 365.25 = 584.542.046 (584.5 million years)
ms -> s -> m -> h -> d -> y
*/

template <>
struct std::formatter<Duration>
{
   private:
    static void append_unit(std::string& out, bool& started, uint64_t value, const char* suffix)
    {
        if (value != 0 || started)
        {
            if (strcmp(suffix, "ms") == 0)
            {
                if (value > 0)
                {
                    out += std::format("{}{}", value, suffix);
                }
            }
            else
            {
                out += std::format("{}{} ", value, suffix);
                started = true;
            }
        }
    }

    std::string format_human_readable_string(const Duration& d) const
    {
        std::string result;
        if (isMs)
        {
            result = std::format("{}ms", d.ms());
        }
        else if (isSec)
        {
            result = std::format("{}s", d.s());
        }
        else if (isMin)
        {
            result = std::format("{}m", d.m());
        }
        else if (isHour)
        {
            result = std::format("{}h", d.h());
        }
        else if (isDay)
        {
            result = std::format("{}d", d.d());
        }
        else if (isYear)
        {
            result = std::format("{}y", d.y());
        }
        else
        {
            auto val_ms = d.ms();
            auto val_y = val_ms / MS_PER_YEAR;
            val_ms %= MS_PER_YEAR;

            auto val_d = val_ms / MS_PER_DAY;
            val_ms %= MS_PER_DAY;

            auto val_h = val_ms / MS_PER_HOUR;
            val_ms %= MS_PER_HOUR;

            auto val_m = val_ms / MS_PER_MINUTE;
            val_ms %= MS_PER_MINUTE;

            auto val_s = val_ms / MS_PER_SECOND;
            val_ms %= MS_PER_SECOND;

            // suppress leading zero units.
            bool started = false;
            append_unit(result, started, val_y, "y");
            append_unit(result, started, val_d, "d");
            append_unit(result, started, val_h, "h");
            append_unit(result, started, val_m, "m");
            append_unit(result, started, val_s, "s");
            append_unit(result, started, val_ms, "ms");
        }
        return result;
    }

    char spec_buf[32]{};
    size_t spec_len{0};

    bool isMs = false;    // [:ms]
    bool isSec = false;   // [:s]
    bool isMin = false;   // [:m]
    bool isHour = false;  // [:h]
    bool isDay = false;   // [:d]
    bool isYear = false;  // [:y]

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
            type_it++;
        }

        auto ranged_end = type_it;
        if (ranged_end != it)
        {
            auto last = ranged_end - 1;
            if (ranged_end - it >= 2 && *last == 's' && *(last - 1) == 'm')  // make sure not to get UB by getting diff.
            {
                isMs = true;
                ranged_end = last - 1;
            }
            else if (*last == 's')
            {
                isSec = true;
                ranged_end = last;
            }
            else if (*last == 'm')
            {
                isMin = true;
                ranged_end = last;
            }
            else if (*last == 'h')
            {
                isHour = true;
                ranged_end = last;
            }
            else if (*last == 'd')
            {
                isDay = true;
                ranged_end = last;
            }
            else if (*last == 'y')
            {
                isYear = true;
                ranged_end = last;
            }
        }

        spec_len = 0;
        for (auto i = it; i != ranged_end; ++i)
        {
            spec_buf[spec_len++] = *i;
        }
        return type_it;
    }

    auto format(const Duration& d, std::format_context& ctx) const
    {
        std::string result = format_human_readable_string(d);
        std::string fmt = std::string("{:" + std::string(spec_buf, spec_len) + "}");
        return std::vformat_to(ctx.out(), fmt, std::make_format_args(result));
    }
};
