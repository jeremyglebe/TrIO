#pragma once
// Define whether we are using Windows
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define WINDOWS true
#endif

// Include for all platforms
#include <chrono>
#include <codecvt>
#include <iostream>
#include <regex>
#include <string>
#include <thread>
#include <vector>

// Include only for Windows
#if defined(WINDOWS)
#include <fcntl.h>
#include <io.h>
#include <windows.h>
#else
// Include only for *nix
#include <unistd.h>
#endif

namespace Term
{

using std::cout;
using std::ostream;
using std::string;
using std::to_string;
using std::vector;
using std::wcout;
using std::wostream;

/**
 * TermIO Color codes, used to choose foreground and background
 * in the Color object
 */
const unsigned short DEFAULT = 0;
const unsigned short BLACK = 1;
const unsigned short RED = 2;
const unsigned short YELLOW = 3;
const unsigned short GREEN = 4;
const unsigned short BLUE = 5;
const unsigned short CYAN = 6;
const unsigned short MAGENTA = 7;
const unsigned short WHITE = 8;

/**
 * Fuses two multi-line string together for printing side-by-side
 * @param left the string that will be on the left half of the fused string
 * @param right the string that will be on the right half of the fused string
 * @param pad bool, whether to pad each line of the string to be the same width
 */
string fuse(string left, string right, bool pad = false);

/**
 * Fuses multiple multi-line strings together for printing side-by-side.
 * @param strings an initializer list of strings to fuse, left to right
 * Initializer lists look like: {myStrVar, "Hello", "test", other_string_var}
 * @param pad bool, whether to pad each line of the string to be the same width
 */
string fuse(std::initializer_list<string> strings, bool pad = false);

/**
 * Split a string and store each new substring in a vector.
 * @param text the original string
 * @param delim the delimiting character to split by
 * @param include determines if the delimeter should be included in the
 * returned substrings
 * @return vector containing each substring
 */
vector<string> split(string text, char delim, bool include = false);

/**
 * Split a string, using a regular expression as a delimeter, and store
 * each new substring in a vector.
 * @param text the original string
 * @param delim the regular expression to match and split by
 * @param include determines if the delimeter should be included in the
 * returned substrings
 * @return vector containing each substring
 */
vector<string> rsplit(string text, string delim, bool include = false);

/**
 * Replaces all instances of a substring in a text with a new string.
 * @param text the main text to replace in
 * @param from the substring to replace
 * @param to the new string to replace it with
 * @return the updated main string
 */
string replace_all(string text, string from, string to);

/**
 * It is easier to consistently pass in strings instead of keeping track of
 * wide vs narrow strings. So, we will overload << to make wostreams able to
 * work with strings (by converting them inside the operation to wstring)
 * @param wout the wide ostream to be output to
 * @param text the text to be converted to a wstring
 * @return the same wostream being used (for chaining output statements)
 */
std::wostream &operator<<(wostream &wout, string text);

/**
 * A Point object is used to move the cursor on the terminal.
 * When sent into an IO object, the cursor will change to the Point's
 * row and column position
 */
class Point
{
public:
    unsigned short row;
    unsigned short col;
    /**
     * Creates a point
     * @param row the row where the point is positioned
     * @param col the column where the point is positioned
     */
    Point(const unsigned short &row, const unsigned short &col);
};
typedef Point pnt;
typedef Point p;

/**
 * A Color object is used to color the strings being sent to the terminal.
 * When sent to an IO object, the terminal will begin using the specified
 * foreground and background colors from the Color object.
 */
class Color
{
public:
    unsigned short fg;
    unsigned short bg;
    /**
     * Creates a Color object
     * @param fg color code for the foreground
     * @param bg color code for the background
     */
    Color(const unsigned short &fg, const unsigned short &bg = 0);
};
typedef Color col;

/**
 * Commands are objects which when sent to an IO object should run some
 * function.
 * Their function is stored as the call() method. Commands should be unique
 * inherited classes of this super-class and should override call().
 */
class Command
{
public:
    virtual void call() = 0;
};
typedef Command com;

/**
 * CLEAR IS A SINGLETON: This means there is only ONE instance of the class.
 * The instance is defined by Clear::get() and there are two references to it
 * declared immediately below the class.
 * The Clear object is a command which, when passed to an IO object, will
 * clear the terminal's screen using its call() method.
 */
class Clear : public Command
{
public:
    static Clear &get();
    void call() override;

private:
    Clear();
};
static Clear &clear = Clear::get();
static Clear &clr = Clear::get();

/**
 * SLEEP IS A SINGLETON: This means there is only ONE instance of the class.
 * The instance is defined by Sleep::get() and there are two references to it
 * declared immediately below the class.
 * The Sleep object is a command which, when passed to an IO object, will
 * make the program sleep for a time specified in the objects constructor.
 */
class Sleep : public Command
{
public:
    static Sleep &get();
    /**
     * Stops the thread (or program, if single-threaded) for a number of
     * milliseconds determined by the data member of this object.
     */
    void call() override;
    /**
     * Sets how many ms the thread should sleep
     * when using the sleep object.
     * @param ms how much time, in miliseconds (1/1000 of a second), the
     * program should sleep when this object is passed to an IO object.
     */
    Sleep &operator()(int ms);

private:
    unsigned int ms;
    Sleep();
};
static Sleep &sleep = Sleep::get();
static Sleep &slp = Sleep::get();

/**
 * Main input/output control for the library. Can use various other objects to
 * print, colorize, get input, and various screen functions.
 */
class IO
{
public:
    // constructors
    IO();
    IO(ostream &out);
    IO(wostream &wout);

    // output operations
    /**
     * Prints a string to the terminal and interprets any color codes found
     * @param text the string to print to the terminal
     * @return a reference to this IO object, to account for chained outputs
     */
    IO &operator<<(string text);
    IO &operator<<(const char &letter);
    IO &operator<<(const int &number);
    IO &operator<<(const double &number);

    // special output operations
    /**
     * Moves the terminal's cursor to a row/column specified by
     * a Point object.
     * @param point the Point containing the row/column to move to
     * @return this object, for chaining outputs.
     */
    IO &operator<<(const Point &point);
    /**
     * Changes the color of the terminal to the foreground and
     * background specified by the Color object.
     * @param color a Color object containing a foreground and background code
     * (codes are specified in Termio.h near the top of the file)
     * @return this object, for chaining outputs.
     */
    IO &operator<<(const Color &color);
    /**
     * Executes a command object.
     * @param command the command to use .call() on.
     * @return this object, for chaining outputs.
     */
    IO &operator<<(Command &command);

    // input operations
    IO &operator>>(string &str_var);
    IO &operator>>(char *&str_var);
    /**
     * Gets a single character from stdin.
     * Input is unbuffered, echoless, blocking. For non-blocking, use a
     * separate thread.
     * @param ch_var the variable to read a character into
     * @return this object (for chaining inputs)
     */
    IO &operator>>(char &ch_var);

private:
    ostream *out;
    wostream *wout;
    bool wide;
    void set_color(Color c);

#if defined(WINDOWS)
    bool windows_setup;
    HANDLE stdin_terminal;
    HANDLE stdout_terminal;
    void setupWindows();
#endif
};
} // namespace Term
