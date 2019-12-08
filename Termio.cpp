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

Term::IO &Term::IO::operator<<(const Color &color)
{
    set_color(color);
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