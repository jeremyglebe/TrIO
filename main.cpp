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

    // Testing input
    char test = '1';
    io << "Press keys 0-8 to see the color associated with that code.\n"
       << "Press any other key to continue.\n";
    io >> test;
    while (test >= '0' && test <= '8')
    {
        if (test != '1')
            io << std::string("&") + test + '1' + test + '\n';
        else
            io << std::string("&") + test + '8' + test + '\n';
        io >> test;
    }

    // Test chained input and output
    char input_test;
    io << "Testing chaining...\n"
       << "Press a key to be tripled,\n"
       << "then press a key to be doubled,\n"
       << "then press a key to be printed once.\n";
    io >> input_test << input_test << input_test << input_test >> input_test << input_test << input_test >> input_test << input_test;

    // Test fusing strings in multiple ways
    // io << "Testing fusing two strings...\n";
    // io << "Testing fusing two strings with padding...\n";
    io << "Testing fusing numerous strings...\n";
    std::string blank = "";
    blank += "&58┌────┐\n";
    blank += "&58│■■■■│\n";
    blank += "&58│■■■■│\n";
    blank += "&58└────┘\n";
    io << Term::fuse({blank, blank, blank, blank});
}