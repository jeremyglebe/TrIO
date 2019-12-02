#pragma once
// Define whether we are using Windows
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define WINDOWS true
#endif
// Include for all platforms
#include <chrono>
#include <iostream>
#include <regex>
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
 *  _____                 _                           _        _   _                 
 * |_   _|               | |                         | |      | | (_)                
 *   | |  _ __ ___  _ __ | | ___ _ __ ___   ___ _ __ | |_ __ _| |_ _  ___  _ __  ___ 
 *   | | | '_ ` _ \| '_ \| |/ _ \ '_ ` _ \ / _ \ '_ \| __/ _` | __| |/ _ \| '_ \/ __|
 *  _| |_| | | | | | |_) | |  __/ | | | | |  __/ | | | || (_| | |_| | (_) | | | \__ \
 * |_____|_| |_| |_| .__/|_|\___|_| |_| |_|\___|_| |_|\__\__,_|\__|_|\___/|_| |_|___/
 *                 | |                                                               
 *                 |_|                                                               
 */

/*
IMPLEMENTATIONS FOR HELPER FUNCTIONS
*/

// Just calls the regular expression split but delimeter does not have to be
// a regular expression
std::vector<std::string> Term::split(string text, char delim, bool include)
{
    return rsplit(text, string(1, delim), include);
}

// Part of the rsplit() implementation came from Stack Overflow user Marcin,
// see the following post:
// https://stackoverflow.com/questions/16749069/c-split-string-by-regex
std::vector<std::string> Term::rsplit(string text, string delim, bool include)
{
    std::vector<std::string> elems;
    std::regex rgx(delim);
    // Find parts of the string which do not match the regexp and append them
    // to the vector
    std::sregex_token_iterator iter(text.begin(), text.end(), rgx, -1);
    std::sregex_token_iterator end;
    while (iter != end)
    {
        elems.push_back(*(iter));
        ++iter;
    }

    // If we want to include the delimeter, then we need to go re-add it to
    // the beginning of each line.
    if (include)
    {
        std::sregex_token_iterator iter(text.begin(), text.end(), rgx, 0);
        std::sregex_token_iterator end;

        int i = 0;
        while (iter != end)
        {
            string temp = elems[i+1];
            elems[i+1] = *iter;
            elems[i+1] += temp;
            ++iter;
            i++;
        }
    }

    return elems;
}

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

/*
IMPLEMENTATIONS FOR IO
*/

Term::IO::IO()
{
    return;
}

Term::IO &Term::IO::operator<<(string text)
{
    // Reset the color on every new line, easiest way to do this is just to
    // replace every '\n' with '&00\n' (using the Termio color escapes)
    size_t start_pos = 0;
    string old = "\n";
    string repl = "&00\n";
    while ((start_pos = text.find(old, start_pos)) != string::npos)
    {
        text.replace(start_pos, old.length(), repl);
        start_pos += repl.length();
    }

    // Replace every instance of "&&" with, where X represents an
    // invisible non-printable, "&X". We will later ignore X when printing
    start_pos = 0;
    old = "&&";
    repl = '&' + string(1, char(0));
    while ((start_pos = text.find(old, start_pos)) != string::npos)
    {
        text.replace(start_pos, old.length(), repl);
        start_pos += repl.length();
    }

    // Split the string by "&XY" with X and Y as numeric color codes
    vector<string>
        strings = rsplit(text, "&[0-8][0-8]", true);

    // Debugging, print out the split lines to help understand how the string
    // is split.
    for (int i = 0; i < strings.size(); i++)
    {
        cout << strings[i] << std::endl;
    }

    // Return this IO object (for any chained outputs)
    return *this;
}