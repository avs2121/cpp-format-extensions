#pragma once

#include <cassert>
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
    bool isColumn = false;            // [:c / C]
    bool isNormalized = false;        // [:n / N]
    bool isSpherical = false;         // [:s / S] - coordinates for 3d vector
    bool isPolar = false;             // [:p / P] - defaults to degrees, if no specification provided
    bool isPolarRadians = false;      // [:pR / pr / PR / Pr]
    bool isSphericalRadians = false;  // [:sR / sr / SR / Sr]
                                      // Degree output will be for polar [:pD / pd / PD / Pd]
                                      // Degree output will be for spherical [:sD / sd / sD / sd]

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
            else if (*last == 's' || *last == 'S')
            {
                isSpherical = true;
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
            else if (ranged_end - it >= 2 && (*(last - 1) == 's' || *(last - 1) == 'S'))
            {
                if (*last == 'r' || *last == 'R')
                {
                    isSpherical = true;
                    isSphericalRadians = true;
                    ranged_end = last - 1;
                }
                if (*last == 'd' || *last == 'D')
                {
                    isSpherical = true;
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
                double mag = std::hypot(static_cast<double>(v.x()), static_cast<double>(v.y()), static_cast<double>(v.z()));
                assert(mag != 0);  // assert mag is not 0, because divison with 0 will happen later then.
                result = std::format("({},{},{})", v.x() / mag, v.y() / mag, v.z() / mag);
            }
            else if constexpr (Vector<Args...>::dimension == 2)
            {
                double mag = std::hypot(static_cast<double>(v.x()), static_cast<double>(v.y()));
                assert(mag != 0);  // assert mag is not 0, because divison with 0 will happen later then.
                result = std::format("({},{})", v.x() / mag, v.y() / mag);
            }
        }
        else if (isPolar || isSpherical)
        {
            if constexpr (Vector<Args...>::dimension == 2)
            {
                double mag = std::hypot(static_cast<double>(v.x()), static_cast<double>(v.y()));
                // atan2 internally converts int to double, and then convert to degrees
                double angle{0};
                bool isDegrees = true;
                if (isPolarRadians)
                {
                    angle = std::atan2(v.y(), v.x());
                    isDegrees = false;
                }
                else
                {
                    angle = std::atan2(v.y(), v.x()) * (180.0 / std::numbers::pi_v<double>);
                }

                result = std::format("({} mag, {} {})", mag, angle, isDegrees ? "deg" : "rad");
            }
            else if constexpr (Vector<Args...>::dimension == 3)
            {
                double radial_dis = std::hypot(static_cast<double>(v.x()), static_cast<double>(v.y()), static_cast<double>(v.z()));
                assert(radial_dis != 0);  // assert radial_dis is not 0, because divison with 0 will happen later then.

                double polar_angle{0};
                double azimuth_angle{0};
                bool isDegrees = true;
                if (isSphericalRadians)
                {
                    isDegrees = false;
                    polar_angle = std::acos(v.z() / radial_dis);
                    azimuth_angle = std::atan2(v.y(), v.x());
                }
                else
                {
                    polar_angle = std::acos(v.z() / radial_dis) * (180.0 / std::numbers::pi_v<double>);
                    azimuth_angle = std::atan2(v.y(), v.x()) * (180.0 / std::numbers::pi_v<double>);
                }
                result = std::format("({0} dist, {1} {3}, {2} {3})", radial_dis, azimuth_angle, polar_angle, isDegrees ? "deg" : "rad");
            }
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