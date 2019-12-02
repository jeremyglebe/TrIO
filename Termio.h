#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

namespace Term
{

using std::cout, std::wcout, std::ostream, std::wostream;
using std::string, std::vector;

/**
 * Fuses two multi-line string together for printing side-by-side
 * @param left the string that will be on the left half of the fused string
 * @param right the string that will be on the right half of the fused string
 * @return string containing both of the original strings side-by-side
 */
string fuse(string left, string right);

/**
 * Fuses two multi-line string together for printing side-by-side
 * @param left the string that will be on the left half of the fused string
 * @param right the string that will be on the right half of the fused string
 * @param pad bool, whether to pad each line of the string to be the same width
 */
string fuse(string left, string right, bool pad);

/**
 * Split a string and store each new substring in a vector.
 * @param text the original string
 * @param delim the delimiting character to split by
 * @return vector containing each substring
 */
vector<string> splitstring(string text, char delim);

/**
 * A Point object is used to move the cursor on the terminal.
 * When sent into an IO object, the cursor will change to the Point's
 * row and column position
 */
class Point
{
public:
    /**
     * Creates a point
     * @param row the row where the point is positioned
     * @param col the column where the point is positioned
     */
    Point(const unsigned short &row, const unsigned short &col);

private:
    unsigned short row;
    unsigned short col;
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
    /**
     * Creates a Color object
     * @param fg color code for the foreground
     * @param bg color code for the background
     */
    Color(const unsigned short &fg, const unsigned short &bg);

private:
    unsigned short fg;
    unsigned short bg;
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
 * The Clear singleton is a command which, when passed to an IO object, will
 * clear the terminal's screen using its call() method.
 */
class Clear : Command
{
public:
    void call() override;
};
typedef Clear clr;

/**
 * The Sleep object is a command which, when passed to an IO object, will
 * make the program sleep for a time specified in the objects constructor.
 */
class Sleep : Command
{
public:
    /**
     * Creates a sleep command object.
     * @param ms how much time, in miliseconds (1/1000 of a second), the
     * program should sleep when this object is passed to an IO object.
     */
    Sleep(const unsigned int &ms);
    /**
     * Stops the thread (or program, if single-threaded) for a number of
     * milliseconds determined by the data member of this object.
     */
    void call() override;

private:
    unsigned int ms;
};
typedef Sleep sleep;
typedef Sleep slp;

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
    IO &operator<<(const string &text);
    IO &operator<<(const char &letter);
    IO &operator<<(const int &number);
    IO &operator<<(const double &number);

    // special output operations
    IO &operator<<(const Point &point);
    IO &operator<<(const Color &color);
    IO &operator<<(const Command &command);

    // input operations
    IO &operator>>(string &str_var);
    IO &operator>>(char *&str_var);
    IO &operator>>(char &ch_var);

private:
    ostream *out;
    wostream *wout;
    bool wide;
};
} // namespace Term

/*
IMPLEMENTATIONS FOR COMMANDS
*/
Term::Sleep::Sleep(const unsigned int &ms)
{
    this->ms = ms;
}

void Term::Sleep::call()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}