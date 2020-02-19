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
        io << "Key: " << Term::Color(Term::CYAN) << ch;
        io << Term::Color(Term::DEFAULT);
        io << ", Code: " << Term::Color(Term::GREEN) << int(ch);
        io << Term::Color(Term::DEFAULT) << "\n";
        io << Term::sleep(100);
        io >> ch;
    }
    io << Term::Color(Term::RED) << "goodbye\n";
}