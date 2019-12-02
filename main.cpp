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
        // x and y are flipped from the documented function...?
        moveCursor(rand() % 3 + 4, rand() % 10);
        print("&40X");
    }

    // Counting from 1,1. If you enter 0, it becomes 1...?
    moveCursor(0, 0);
    print("&30X");
    moveCursor(1, 1);
    print("&30X");

}