#include "TermGame.h"

int main(){
    using namespace TermGame;
    using namespace TermPrint;

    print("Hello World!\n");
    print("Testing... Testing...\n");
    print("Goodbye World!\n");

    // plant randomly positioned red X's in the terminal
    srand(time(NULL));
    for(int i = 0; i < 9; i++){
        moveCursor(rand() % 10, rand() % 6 + 3);
        print("&40X");
    }

}