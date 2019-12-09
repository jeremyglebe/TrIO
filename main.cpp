<<<<<<< HEAD
#include <iostream>
#include <string>
#include "Termio.h"

int main()
{
    Term::IO io;

    // Print a rainbow alphabet
    io << "Testing colors with a Rainbow-Alphabet\n";
    std::string s = "";
    for (char let = 'a', code = '1'; let <= 'z'; let++)
    {
        code = code < '8' ? code + 1 : '2';
        s += std::string(1, '&') + code + '0' + let;
    }
    s += '\n';
    io << s;

    // Print random color alphabet
    io << "Testing colors with a Rando-Alphabet\n";
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

    // Test fusing strings
    io << "\nTesting fusing numerous strings...\n";
    std::string blank = "";
    blank += "&58┌────┐\n";
    blank += "&58│■■■■│\n";
    blank += "&58│■■■■│\n";
    blank += "&58└────┘\n";
    io << Term::fuse({blank, blank, blank, blank});
    for (int i = 0; i < 3; i++)
        io << Term::sleep(1333) << '.';

    // Test clear, moving cursor & sleeping by random X's around the screen
    io << Term::clear << "Testing clear, move & sleep by printing X's";
    for (int i = 0; i < 3; i++)
        io << Term::sleep(1333) << '.';
    for (int i = 0; i < 40; i++)
    {
        short fg = rand() % 8 + 1;
        short row = rand() % 40;
        short col = rand() % 80;
        io << Term::sleep(100) << Term::Point(row, col) << Term::Color(fg, Term::BLACK) << 'X';
    }
    io << Term::sleep(3000) << Term::clear << Term::Color(0, 0);
=======
#include "TermGame.h"

int main(){
    using namespace TermGame;
    using namespace TermPrint;

    print("Hello World!\n");
    print("Testing... Testing...\n");
    print("Goodbye World!\n");

    // plant randomly positioned green X's in the terminal
    srand(time(NULL));
    for(int i = 0; i < 9; i++){
        moveCursor(rand() % 3 + 3, rand() % 80);
        TermGame::sleep(10);
        print("&40X");
        TermGame::sleep(10);
    }

    moveCursor(0, 0);
    print("&30X");
    moveCursor(1, 1);
    print("&30X");

>>>>>>> master
}