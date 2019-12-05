#include <iostream>
#include <string>
#include "Termio.h"

int main()
{
    Term::IO io;

    // Print a rainbow alphabet
    std::string s = "";
    for (char let = 'a', code = '1'; let <= 'z'; let++)
    {
        code = code < '8' ? code + 1 : '2';
        s += std::string(1, '&') + code + '0' + let;
    }
    s += '\n';
    io << s;

    // Print random color alphabet
    srand(time(NULL));
    s = "";
    for (char let = 'a'; let <= 'z'; let++)
    {
        char fg = (rand() % 9) + '0';
        char bg = (rand() % 9) + '0';
        s += std::string(1, '&') + fg + bg + let;
    }
    s += '\n';
    io << s;
}