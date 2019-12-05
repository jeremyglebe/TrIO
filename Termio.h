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
    unsigned short fg;
    unsigned short bg;
    /**
     * Creates a Color object
     * @param fg color code for the foreground
     * @param bg color code for the background
     */
    Color(const unsigned short &fg, const unsigned short &bg);
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
    void _set_color(Color c);

#if defined(WINDOWS)
    bool _windows_setup;
    HANDLE _active_terminal;
    void _setupWindows();
#endif
};
} // namespace Term

/* ██╗███╗   ███╗██████╗ ██╗     ███████╗███╗   ███╗███████╗███╗   ██╗████████╗ █████╗ ████████╗██╗ ██████╗ ███╗   ██╗███████╗
 * ██║████╗ ████║██╔══██╗██║     ██╔════╝████╗ ████║██╔════╝████╗  ██║╚══██╔══╝██╔══██╗╚══██╔══╝██║██╔═══██╗████╗  ██║██╔════╝
 * ██║██╔████╔██║██████╔╝██║     █████╗  ██╔████╔██║█████╗  ██╔██╗ ██║   ██║   ███████║   ██║   ██║██║   ██║██╔██╗ ██║███████╗
 * ██║██║╚██╔╝██║██╔═══╝ ██║     ██╔══╝  ██║╚██╔╝██║██╔══╝  ██║╚██╗██║   ██║   ██╔══██║   ██║   ██║██║   ██║██║╚██╗██║╚════██║
 * ██║██║ ╚═╝ ██║██║     ███████╗███████╗██║ ╚═╝ ██║███████╗██║ ╚████║   ██║   ██║  ██║   ██║   ██║╚██████╔╝██║ ╚████║███████║
 * ╚═╝╚═╝     ╚═╝╚═╝     ╚══════╝╚══════╝╚═╝     ╚═╝╚══════╝╚═╝  ╚═══╝   ╚═╝   ╚═╝  ╚═╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 * ANSI Shadow font
 * http://patorjk.com/software/taag/
*/

/*  _  _  ____  __    ____  ____  ____    ____  _  _  __ _   ___  ____  __  __   __ _    __  _  _  ____  __   
 * / )( \(  __)(  )  (  _ \(  __)(  _ \  (  __)/ )( \(  ( \ / __)(_  _)(  )/  \ (  ( \  (  )( \/ )(  _ \(  )  
 * ) __ ( ) _) / (_/\ ) __/ ) _)  )   /   ) _) ) \/ (/    /( (__   )(   )((  O )/    /   )( / \/ \ ) __// (_/\
 * \_)(_/(____)\____/(__)  (____)(__\_)  (__)  \____/\_)__) \___) (__) (__)\__/ \_)__)  (__)\_)(_/(__)  \____/
 * Graceful font
 * http://patorjk.com/software/taag/
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
    // code -1 at the end means find everything but the rgx string (delimeter)
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
        // code 0 at the end means find instances of the rgx string (delimeter)
        std::sregex_token_iterator iter(text.begin(), text.end(), rgx, 0);
        std::sregex_token_iterator end;

        int i = 0;
        while (iter != end)
        {
            // We are inserting the delimiter at the start
            // of the NEXT string in the vector
            string temp = elems[i + 1];
            // assign the string the delimiter
            elems[i + 1] = *iter;
            // add back the original content
            elems[i + 1] += temp;
            // the iterator moves forward
            ++iter;
            // increment elems index
            i++;
        }
    }

    return elems;
}

std::string Term::replace_all(string text, string from, string to)
{
    size_t start_pos = 0;
    // While we can find the substring "from"
    // (start position changes each time we make a replacement)
    while ((start_pos = text.find(from, start_pos)) != string::npos)
    {
        // Make the replacement
        text.replace(start_pos, from.length(), to);
        // Move past the string replacement's length, we don't need to
        // check it
        start_pos += to.length();
    }
    return text;
}

std::wostream &Term::operator<<(wostream &wout, string text)
{
    /**
     * create a string <-> wide string converter
     * example from stack overflow
     * std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
     * std::string narrow = converter.to_bytes(wide_utf16_source_string);
     * std::wstring wide = converter.from_bytes(narrow_utf8_source_string);
     */
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring wide_text = converter.from_bytes(text);
    wout << wide_text;
    return wout;
}

/*   ___  __   _  _  _  _   __   __ _  ____    __  _  _  ____  __   
 *  / __)/  \ ( \/ )( \/ ) / _\ (  ( \(    \  (  )( \/ )(  _ \(  )  
 * ( (__(  O )/ \/ \/ \/ \/    \/    / ) D (   )( / \/ \ ) __// (_/\
 *  \___)\__/ \_)(_/\_)(_/\_/\_/\_)__)(____/  (__)\_)(_/(__)  \____/
 * Graceful font
 * http://patorjk.com/software/taag/
 */
Term::Sleep::Sleep(const unsigned int &ms)
{
    this->ms = ms;
}

void Term::Sleep::call()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

/*   ___  __   __     __  ____    __  _  _  ____  __   
 *  / __)/  \ (  )   /  \(  _ \  (  )( \/ )(  _ \(  )  
 * ( (__(  O )/ (_/\(  O ))   /   )( / \/ \ ) __// (_/\
 *  \___)\__/ \____/ \__/(__\_)  (__)\_)(_/(__)  \____/
 * Graceful font
 * http://patorjk.com/software/taag/
 */
Term::Color::Color(const unsigned short &fg, const unsigned short &bg)
{
    this->fg = fg;
    this->bg = bg;
}

/*   __  __     __  _  _  ____  __   
 *  (  )/  \   (  )( \/ )(  _ \(  )  
 *   )((  O )   )( / \/ \ ) __// (_/\
 *  (__)\__/   (__)\_)(_/(__)  \____/
 * Graceful font
 * http://patorjk.com/software/taag/
 */

Term::IO::IO()
{
#if defined(WINDOWS)
    _windows_setup = false;
    wide = true;
    wout = &wcout;
#else
    wide = false;
    out = &cout;
#endif
}

Term::IO &Term::IO::operator<<(string text)
{
#if defined(WINDOWS)
    // Setup Windows if we haven't yet.
    if (!_windows_setup)
        _setupWindows();
#endif

    // Reset the color on every new line, easiest way to do this is just to
    // replace every '\n' with '&00\n' (using the Termio color escapes)
    text = replace_all(text, "\n", "&00\n");

    // Replace every instance of "&&" with, where X represents an
    // invisible non-printable, "&X". We will later ignore X when printing
    text = replace_all(text, "&&", '&' + string(1, char(0)));

    // Split the string by "&XY" with X and Y as numeric color codes
    vector<string>
        strings = rsplit(text, "&[0-8][0-8]", true);

    // Print each line of the vector
    for (int i = 0; i < strings.size(); i++)
    {
        // Now that we've finished searching for escapes, we want the "&X"'s
        // to become just plain '&'.
        strings[i] = replace_all(strings[i], "&" + string(1, char(0)), string(1, '&'));

        // Set the color based on the first 3 characters of each substring
        // (Make sure the substring contains a color code, it may be "")
        if (strings[i].size() > 3)
        {
            _set_color(Color(strings[i][1] - '0', strings[i][2] - '0'));
        }

        // Print the line, using either a wide or narrow stream
        if (wide)
        {
            if (strings[i].size() > 3)
                *wout << strings[i].substr(3);
            else
                *wout << strings[i];
        }
        else
        {
            if (strings[i].size() > 3)
                *out << strings[i].substr(3);
            else
                *out << strings[i];
        }

        _set_color(Color(0, 0));
    }

    // Return this IO object (for any chained outputs)
    return *this;
}

void Term::IO::_set_color(Color c)
{
#if defined(WINDOWS)
    static const unsigned short _fg[] = {7, 0, 4, 6, 2, 1, 3, 5, 7};
    static const unsigned short _bg[] = {0, 0, 64, 96, 32, 16, 48, 80, 112};
#else
    static const unsigned short _fg[] = {39, 30, 31, 33, 32, 34, 36, 35, 37};
    static const unsigned short _bg[] = {49, 40, 41, 43, 42, 44, 46, 45, 47};
#endif

#if defined(WINDOWS)
    if (!_windows_setup)
        _setupWindows();
    SetConsoleTextAttribute(_active_terminal, _fg[c.fg] + _bg[c.bg]);
#else
    if (wide)
    {
        *wout << "\033[" + to_string(_fg[c.fg]) + to_string(_bg[c.bg]) + 'm';
    }
    else
    {
        *out << "\033[" + to_string(_fg[c.fg]) + ';' + to_string(_bg[c.bg]) + 'm';
    }
#endif
}

#if defined(WINDOWS)
void Term::IO::_setupWindows()
{
    // If we're using windows and it has not yet been fixed
    if (!_windows_setup)
    {
        // set the console mode for unicode
        _setmode(_fileno(stdout), _O_U16TEXT);
        // We must have a reference to the active terminal for Windows' color API
        _active_terminal = GetStdHandle(STD_OUTPUT_HANDLE);
        // Mark the Windows fix as complete
        _windows_setup = true;
    }
}
#endif