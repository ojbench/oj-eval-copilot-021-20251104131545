#include <iostream>
#include <fstream>
#include "game.h"

using namespace std;

int main() {
    // Test sequence 1: A A
    {
        auto mapfile = ifstream("/workspace/data/021/testcases/naive.in");
        Game *g = new Game(mapfile);
        int total = 0;
        total += g->play('A');
        total += g->play('A');
        cerr << "A A: total=" << total << ", remaining=" << g->bricksRemaining() << endl;
        delete g;
    }
    
    // Test sequence 2: B D A
    {
        auto mapfile = ifstream("/workspace/data/021/testcases/naive.in");
        Game *g = new Game(mapfile);
        int total = 0;
        total += g->play('B');
        total += g->play('D');
        total += g->play('A');
        cerr << "B D A: total=" << total << ", remaining=" << g->bricksRemaining() << endl;
        delete g;
    }
    
    return 0;
}
