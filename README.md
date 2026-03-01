# cpp-format-extensions

A collection of custom `std::formatter` specializations built on top of C++20 `std::format`.
Each formatter supports the standard fill, align, and width specifiers alongside custom type specifiers.

## Build
```bash
make run
```

## Formatters

### Color
Formats an RGB color from three `uint8_t` components.

Types of specifiers: 
- {} => standard rgb format
- {:h} => hex format
- {:H} => capital hex format 

### Duration
Formats a `uint64_t` millisecond value into human-readable time units.
Default output suppresses leading zero units: `1y 2d 3h` not `0y... 1y 2d 3h`.

Types of specifiers: 
- {} => Includes all types of values conferring the aforementioned rules
- {:ms} => ms format
- {:s} => second format 
- {:m} => minute format
- {:h} => hour format
- {:d} => day format
- {:y} => year format

### Vector
Formats a `Vector<T, ...>` where `T` is `int`, `long`, or a floating point type.
Supports both 2D and 3D vectors.

Types of specifiers for 2D vector: 
- {} => standard vector 
- {:c} => column vector
- {:n} => normalized vector
- {:p} => polar coordinates for vector in degrees
  - {:pD} / {:pd} => polar coordinates explicitly in degrees
- {:pR} / {:pr} => polar coordinates for vector in radians

 Types of specifiers for 3D vectors:
- {}       => standard 3D vector (x, y, z)
- {:c}     => column vector ([x]\n[y]\n[z])
- {:n}     => normalized vector ((x/mag, y/mag, z/mag))
- {:s}     => spherical coordinates in degrees
  - {:sD} / {:sd} => spherical coordinates explicitly in degrees
- {:sR} / {:sr} => spherical coordinates in radians
