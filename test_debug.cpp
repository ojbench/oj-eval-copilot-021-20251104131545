#include <iostream>
#include <fstream>
#include "game.h"

using namespace std;

int main() {
    auto mapfile = ifstream("/workspace/data/021/testcases/naive.in");
    Game *game = new Game(mapfile);
    
    cerr << "n = " << game->n << endl;
    cerr << "Total bricks: " << game->bricksTotal() << endl;
    cerr << "Remaining: " << game->bricksRemaining() << endl;
    
    // Try a simple sequence
    char ops[] = {'B', 'D', 'A'};
    for (int i = 0; i < 3 && game->bricksRemaining() > 0; i++) {
        int r = game->play(ops[i]);
        cerr << "Op " << ops[i] << ": reward=" << r << ", hits=" << game->bricksHit() << ", remaining=" << game->bricksRemaining() << endl;
        cout << ops[i] << endl;
    }
    
    delete game;
    return 0;
}
