#include <iostream>
#include "Termio.h"

int main()
{
    srand(time(NULL));
    Term::IO io;
    char ch;
    io >> ch;
    while (ch != 'q')
    {
        io << Term::Color(rand() % 9, rand() % 9) << ch << '\n';
        io >> ch;
    }
    io << Term::Color(Term::RED, Term::BLACK) << "goodbye\n";
}