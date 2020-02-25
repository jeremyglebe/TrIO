#include <iostream>
#include "../trio.hpp"

int main()
{
    srand(time(NULL));
    trio::IO io;
    char ch;
    io >> ch;
    while (ch != 'q')
    {
        io << "Key: " << trio::Color(trio::CYAN) << ch;
        io << trio::Color(trio::DEFAULT);
        io << ", Code: " << trio::Color(trio::GREEN) << int(ch);
        io << trio::Color(trio::DEFAULT) << "\n";
        io << trio::sleep(100);
        io >> ch;
    }
    io << trio::Color(trio::RED) << "goodbye\n";
}