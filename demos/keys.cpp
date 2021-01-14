#include <iostream>
#include "../trio.hpp"

int main()
{
    trio::IO io;
    char ch;
    io >> ch;
    // Press 'q' to exit
    while (ch != 'q')
    {
        io << "Key: " << trio::Color(trio::CYAN) << ch;
        io << trio::Color(trio::DEFAULT);
        io << ", Code: " << trio::Color(trio::GREEN) << int(ch);
        io << trio::Color(trio::DEFAULT) << "\n";
        trio::sleep(100);
        io >> ch;
    }
    io << trio::Color(trio::RED) << "goodbye\n";
}