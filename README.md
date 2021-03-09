# TrIO: Simple, portable IO customization in your terminal.
C++ library designed to streamline the use of unicode, colors, and
unbuffered input in the terminal. Make everything from menu-driven programs to
simple games without the complexity of GUI or graphics libraries.

(Looking for **TermIO**? You've found it! The library was renamed to avoid
confusion with *termios*)

### Installation
Just download the "trio.hpp" file into your project directory, or if you're
using an IDE include it by whatever means you would usually do so, and then
you're good to go! Make sure to have `#include "trio.hpp"` in the file you
want to use the library in.

### Objective
There are 3 main objective of TrIO:
  - Allow for easy use of colors in the terminal
  - Allow for easy unbuffered and echoless input in the terminal
  - Allow for control of when/where/how output is expressed in the terminal
  
There are 3 systems TrIO should (ideally) operate on without error:
  - Windows (8+)
  - Mac
  - Linux

These are the obvious platforms to work on as they account for the
*overwhelming* majority of systems. I cannot guarantee compatibility with
every distribution of Linux. I primarily test with an Ubuntu environment.
I lack a Mac system to test on. I rely on assistance from friends and the
internet as a whole.

### History & Purpose
**TrIO is, first and foremost, intended to be an educational tool.**
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
trio::IO io;
char myChar;
io >> myChar;
```
The IO object can print simple strings or characters.

`io << "Hello World!\n"`

Used in conjuction with the Color object, the IO object can print text to
the terminal with color in the foreground and background.
```cpp
io << trio::Color(trio::RED, trio::WHITE)
    << "This text would be red with white background\n";
```
The IO object can also print strings with color sequences defined within them,
see the subsection further down.

The IO object can move the cursor, clear the screen, and even make the current
thread sleep (wait) some number of miliseconds. The Point object makes the
cursor move to a new location, while commands clear and sleep will cause the
screen to clear or the program to wait.
```cpp
// The following code will print "Hello" one character at a time with a quarter
// of a second between each character. (It will clear the screen first, just to
// demonstrate that functionality)
std::string str = "Hello";
trio::clear_screen();
for (int i = 0; i < str.size(); i++){
    io.sleep(250) << str[i];
}
// Now the cursor will move back to the first position, where it will start
// writing "World" a character at a time, overwriting the previous "Hello".
io << trio::Point(0,0);
str = "World!\n";
for (int i = 0; i < str.size(); i++){
    io.sleep(250) << str[i];
}
// The function trio::sleep_ms(int ms) also exists and is
// equivalent to io.sleep()
// The same code above could be written like so:
io << trio::Point(0,0);
str = "World!\n";
for (int i = 0; i < str.size(); i++){
    trio::sleep_ms(250);
    io << str[i];
}
// There also exists io.clear() which is equivalent to clear_screen() and
// returns the IO object that calls it. This allows clear statements to be
// chained into output. Example:
io << "Hello";
io.sleep(1000).clear() << "World!\n";
```

Note: If you were to get tired of typing `trio::` for everything, you could
of course utilize the `using` command to avoid it. (Only do this if you are
certain it will not cause scoping issues. This may make certain identifiers
of functions or objects become ambiguous.)

`using namespace trio;`

### Color Escape Sequences
The IO object can receive strings with special escape sequences designed for
use with this library. These sequences **do not** correspond with ANSI escapes,
but they are implemented using ANSI on *nix systems. Use of these sequences
are portable even on Windows devices that don't support ANSI.

Escape sequences in TrIO are of the form "&FB" where F and B represent the 
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

### A Note on Unicode
TrIO does enable support for Unicode characters ("wide" characters) within
strings on both Windows and *nix systems. (though *nix supports them by
default) However, even when Windows is configured to accept wide characters in
the terminal, there is the limitation of font. Default fonts for the Windows
terminal are missing a large number of Unicode characters! This is unfortunate
because Unicode is wonderful for creating menus and UIs and just generally fun
output. I recommend downloading and installing a Unicode friendly font, then
setting it to be your default.
To set the default font, or font for the current CMD prompt, right click the
bar at the top of your CMD window. Click on defaults or properties.
I recommend  the font **Deja Vu Sans Mono** or one of many great fonts in
the [Deja Vu Fonts Collection](https://dejavu-fonts.github.io/). (just make sure you choose one that is Unicode friendly!)

## String Fusion
When designing a terminal-based UI, one might find themselves wanting to print
two multi-line strings next to each other. This seems a simple task initially,
but it quickly becomes complicated.
Let's use the example of a card game in which you wish to print two cards
side-by-side. The strings are defined below:
```cpp
std::string card1 = "";
card1 += "&28┌──┐\n";
card1 += "&28│D9│\n";
card1 += "&28└──┘\n";

std::string card2 = "";
card2 += "&18┌──┐\n";
card2 += "&18│S2│\n";
card2 += "&18└──┘\n";
```
Suppose we wish to print these two strings next to each other. We might be
tempted to do something like this:
```cpp
Term:IO io;
io << card1 << card2;
```
But the results would be of the form
```
┌──┐
│D9│
└──┘
┌──┐
│S2│
└──┘
```
(Of course, these would be colored when printed in the terminal. Markdown has
no easy provision to display these results.)
Clearly, this isn't what we wanted. TrIO does have a solution. We can fuse
together two strings with the `trio::fuse` function. It accepts a list of
strings (encased between `{}` brackets) and returns a new string where the
components have been zipped together. Example:
```cpp
std::string both_cards = trio::fuse({card1, card2});
io << card3;
```
Result:
```
┌──┐┌──┐
│D9││S2│
└──┘└──┘
```
