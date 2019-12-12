# TermIO: Simple, portable IO customization in your terminal.
C++ library designed to streamline the use of unicode, colors, and
unbuffered input in the terminal. Make everything from menu-driven programs to
simple games without the complexity of GUI or graphics libraries.

### History & Purpose
This library was originally designed for the Computer Science II class that I
was a supplemental instructor for. Our objective was to let them make a fun
program that wasn't limited to basic text output without all the overhead of
Unicode and ANSI. (While these encodings work great on *nix systems, they don't
always work on Windows and don't port easily) Existing libraries seemed a bit
complex for our freshmen-level courses, so I created this. With that background
it should come as no surprise that this library, while fairly feature-packed,
does not come with any complex functionality. Most operations are simply
operations on an IO object, which should feel very familiar to freshmen or
sophomore students because of its similarity to std::cin and std::cout.

## The IO object

### Color Escape Sequences
The IO object can receive strings with special escape sequences designed for
use with this library. These sequences **do not** correspond with ANSI escapes,
but they are implemented using ANSI on *nix systems. Use of these sequences
are portable even on Windows devices that don't support ANSI.

Escape sequences in TermIO are of the form "&FB" where F and B represent the 
foreground and background, respectively, and are replaced with numeric codes from the chart below.

| Color    | Code |
| :---:    | :-:  |
| Default  | 0    |
| Black    | 1    |
| Red      | 2    |
| Yellow   | 3    |
| Green    | 4    |
| Blue     | 5    |
| Cyan     | 6    |
| Magenta  | 7    |
| White    | 8    |

To print a literal '&' you only need to enter the character twice like so: "&&"
(Note, when escaping to print a literal '&', there will not be a B value)