#include "Termio.h"

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

std::string Term::fuse(string left, string right, bool pad)
{
    string result = "";
    // split the strings, create vectors
    vector<string> lv = split(left, '\n');
    vector<string> rv = split(right, '\n');
    // correct the number of lines in the strings so they match
    if (lv.size() > rv.size())
    {
        int diff = lv.size() - rv.size();
        for (int i = 0; i < diff; i++)
        {
            rv.push_back("");
        }
    }
    else if (lv.size() < rv.size())
    {

        int diff = rv.size() - lv.size();
        for (int i = 0; i < diff; i++)
        {
            lv.push_back("");
        }
    }
    if (pad)
    {
        // find the largest line out of any of the vectors
        int max = 0;
        for (int i = 0; i < lv.size(); i++)
        {
            if (lv[i].size() > max)
                max = lv[i].size();
            if (rv[i].size() > max)
                max = rv[i].size();
        }
        // for each line in both vectors, pad the line for max size
        for (int i = 0; i < lv.size(); i++)
        {
            if (lv[i].size() < max)
            {
                int diff = max - lv[i].size();
                lv[i] += std::string(diff, ' ');
            }
            if (rv[i].size() < max)
            {
                int diff = max - rv[i].size();
                rv[i] += std::string(diff, ' ');
            }
        }
    }
    // loop through the vectors and produce the new string
    for (int i = 0; i < lv.size(); i++)
    {
        result += lv[i] + rv[i] + '\n';
    }
    return result;
}

std::string Term::fuse(std::initializer_list<string> strings, bool pad)
{
    // Variable for storing the resulting string
    string result = "";
    // Moving the initializer_list into a vector b/c I think they're easier
    vector<string> vstrings;
    vstrings.insert(vstrings.end(), strings.begin(), strings.end());
    // Loop through all strings and fuse them
    for (int i = 0; i < vstrings.size(); i++)
    {
        result = fuse(result, vstrings[i], pad);
    }
    return result;
}

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
Term::Sleep::Sleep() {}

Term::Sleep &Term::Sleep::get()
{
    static Sleep instance;
    return instance;
}

Term::Sleep &Term::Sleep::operator()(int ms)
{
    this->ms = ms;
    return get();
}

void Term::Sleep::call()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

Term::Clear::Clear() {}

Term::Clear &Term::Clear::get()
{
    static Clear instance;
    return instance;
}

void Term::Clear::call()
{
#if defined(WINDOWS)
    // The Windows version of this method comes almost character-for-character
    // from the Microsoft docs. Credit Here:
    // https://docs.microsoft.com/en-us/windows/console/clearing-the-screen
    static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coordScreen = {0, 0}; // home for the cursor
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
    {
        return;
    }
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    if (!FillConsoleOutputCharacter(hConsole, (TCHAR)' ', dwConSize, coordScreen, &cCharsWritten))
    {
        return;
    }
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
    {
        return;
    }
    if (!FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten))
    {
        return;
    }
    SetConsoleCursorPosition(hConsole, coordScreen);
#else
    // on *nix use ANSI escape
    std::cout << "\033[2J";
#endif
}

/*  ____   __  __  __ _  ____    _  _  ____  ____  _  _   __  ____  ____ 
 * (  _ \ /  \(  )(  ( \(_  _)  ( \/ )(  __)(_  _)/ )( \ /  \(    \/ ___)
 *  ) __/(  O ))( /    /  )(    / \/ \ ) _)   )(  ) __ ((  O )) D (\___ \
 * (__)   \__/(__)\_)__) (__)   \_)(_/(____) (__) \_)(_/ \__/(____/(____/
 */
Term::Point::Point(const unsigned short &row, const unsigned short &col)
{
    this->row = row;
    this->col = col;
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
    windows_setup = false;
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
    if (!windows_setup)
        setupWindows();
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
        if (strings[i].size() > 3 && i > 0)
        {
            set_color(Color(strings[i][1] - '0', strings[i][2] - '0'));
        }

        // Print the line, using either a wide or narrow stream
        if (wide)
        {
            // Strings too small to contain a color code OR
            // the first string in the vector (which can't contain a color code
            // or it would have split into the second) should be printed in
            // their entirety, other strings we need to skip the first 3 chars
            // which are the color code.
            if (strings[i].size() > 3 && i > 0)
                *wout << strings[i].substr(3);
            else
                *wout << strings[i];
        }
        else
        {
            // Strings too small to contain a color code OR
            // the first string in the vector (which can't contain a color code
            // or it would have split into the second) should be printed in
            // their entirety, other strings we need to skip the first 3 chars
            // which are the color code.
            if (strings[i].size() > 3 && i > 0)
                *out << strings[i].substr(3);
            else
                *out << strings[i];
        }

        set_color(Color(0, 0));
    }

    // Return this IO object (for any chained outputs)
    return *this;
}

Term::IO &Term::IO::operator<<(const char &key)
{
    if (wide)
    {
        *wout << key;
    }
    else
    {
        *out << key;
    }
    return *this;
}

Term::IO &Term::IO::operator<<(const Point &point)
{
#if defined(WINDOWS)
    if (!windows_setup)
    {
        setupWindows();
    }
    // if using Windows, use windows.h
    // We must have a reference to the active terminal for Windows
    // Coordinates are (x, y). Columns are x, rows are y, so r/c becomes c/r
    COORD cor = {(short)point.col, (short)point.row};
    SetConsoleCursorPosition(stdout_terminal, cor);
#else
    // for some reason, row and column in ANSI start at 1, we want it to start at 0
    int r = point.row + 1;
    int c = point.col + 1;
    // on *nix use ANSI escape
    std::string pos = "\033[" + std::to_string(r) + ';' + std::to_string(c) + 'f';
    std::cout << pos;
#endif
    return *this;
}

Term::IO &Term::IO::operator<<(const Color &color)
{
    set_color(color);
    return *this;
}

Term::IO &Term::IO::operator<<(Command &command)
{
    command.call();
    return *this;
}

Term::IO &Term::IO::operator>>(char &ch_var)
{
#if defined(WINDOWS)
    // Keeps track of the console mode we started with
    DWORD mode;
    // Get the current mode so we can restore it later
    GetConsoleMode(stdin_terminal, &mode);
    // Set the console mode to unbuffered and echoless
    SetConsoleMode(stdin_terminal, 0);

    ch_var = std::cin.get();

    // Restore the original console mode
    SetConsoleMode(stdin_terminal, mode);
#else
    // turn off echo and get the input without a buffer
    system("stty -brkint -ignpar -istrip -icrnl -ixon -opost -isig -icanon -echo");
    // get the next stdin character
    //key = getchar();
    std::cin >> ch_var;
    // set the terminal back to buffered input and echo
    system("stty cooked echo");
#endif
    return *this;
}

void Term::IO::set_color(Color c)
{
#if defined(WINDOWS)
    static const unsigned short _fg[] = {7, 0, 4, 6, 2, 1, 3, 5, 7};
    static const unsigned short _bg[] = {0, 0, 64, 96, 32, 16, 48, 80, 112};
#else
    static const unsigned short _fg[] = {39, 30, 31, 33, 32, 34, 36, 35, 37};
    static const unsigned short _bg[] = {49, 40, 41, 43, 42, 44, 46, 45, 47};
#endif

#if defined(WINDOWS)
    if (!windows_setup)
        setupWindows();
    SetConsoleTextAttribute(stdout_terminal, _fg[c.fg] + _bg[c.bg]);
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
void Term::IO::setupWindows()
{
    // If we're using windows and it has not yet been fixed
    if (!windows_setup)
    {
        // set the console mode for unicode
        _setmode(_fileno(stdout), _O_U16TEXT);
        // We must have a reference to the input terminal for Windows' input API
        stdin_terminal = GetStdHandle(STD_INPUT_HANDLE);
        // We must have a reference to the output terminal for Windows' color API
        stdout_terminal = GetStdHandle(STD_OUTPUT_HANDLE);
        // Mark the Windows fix as complete
        windows_setup = true;
    }
}
#endif