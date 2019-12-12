# TermIO: Simple, portable IO customization in your terminal.
C++ library designed to streamline the use of unicode, colors, and
unbuffered input in the terminal. Make everything from menu-driven programs to
simple games without the complexity of GUI or graphics libraries.

### Installation
Just download the "Termio.h" file into your project directory, or if you're
using an IDE include it by whatever means you would usually do so, and then
you're good to go! Make sure to have `#include "Termio.h"` in the file you
want to use the library in.

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
The IO object can receive unbuffered, echoless input from the keyboard and
store it into a character variable like so:
```cpp
Term::IO io;
char myChar;
io >> myChar;
```
The IO object can print simple strings or characters.

`io << "Hello World!\n"`

Used in conjuction with the Color object, the IO object can print text to
the terminal with color in the foreground and background.
```cpp
io << Term::Color(Term::RED, Term::WHITE)
    << "This text would be red with white background\n";
```
The IO object can also print strings with color sequences defined within them,
see the subsection further down.

The IO object can move the cursor, clear the screen, and even make the current
thread sleep (wait) some number of miliseconds. The Point object makes the
cursor move to a new location, while singletons clear and slp will cause the
screen to clear or the program to wait.
```cpp
// The following code will print "Hello" one character at a time with a quarter
// of a second between each character. (It will clear the screen first, just to
// demonstrate that functionality)
std::string str = "Hello";
io << Term::clear;
for (int i = 0; i < str.size(); i++){
    io << Term::slp(250) << str[i];
}
// Now the cursor will move back to the first position, where it will start
// writing "World" a character at a time, overwriting the previous "Hello".
io << Term::Point(0,0);
str = "World!\n";
for (int i = 0; i < str.size(); i++){
    io << Term::slp(250) << str[i];
}
```

Note: If you were to get tired of typing `Term::` for everything, you could
of course utilize the `using` command to avoid it. (Only do this if you are
certain it will not cause scoping issues. This will make the keyword `sleep`,
or possibly even `Sleep`, ambiguous in most environments)
`using namespace Term;`

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