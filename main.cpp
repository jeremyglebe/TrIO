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

}