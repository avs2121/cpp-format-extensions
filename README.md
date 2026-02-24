# cpp-format-extensions
Custom collection of custom std::formatter specilization built on top of std::format. 

Currently implementing:

# Color formatter. 
  - Default RGB output: (r, g, b)

Custom specifiers:
  - {:h} → lowercase hex (#rrggbb)
  - {:H} → uppercase hex (#RRGGBB)

Full support for alignment, width, and fill ({:>20H}, {:*^20}, etc.)

# Duration formatter
- Custom human-readable duration formatting

Custom specifiers: 
- From milliseconds -> [...] -> years.
  - Using corresponding suffix: [ms, s, m, h, d, y]
