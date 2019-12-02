#include <string>
#include"Termio.h"

int main(){
    std::string s = "";
    s += "&01abc&&&&def\n";
    s += "H3l&80110\n";
    Term::IO io;
    io << s;
}