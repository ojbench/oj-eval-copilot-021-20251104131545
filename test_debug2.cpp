#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "game.h"

using namespace std;

char ops[] = {'A', 'B', 'C', 'D', 'E'};

int main() {
    auto mapfile = ifstream("/workspace/data/021/testcases/naive.in");
    Game *game = new Game(mapfile);
    
    cerr << "Testing first move:" << endl;
    for (char op1 : ops) {
        Game::Save* save1 = game->save();
        int r1 = game->play(op1);
        int h1 = game->bricksHit();
        
        cerr << "Op " << op1 << ": r1=" << r1 << ", h1=" << h1;
        
        // Look ahead
        int bestFuture = 0;
        if (game->bricksRemaining() > 0) {
            for (char op2 : ops) {
                Game::Save* save2 = game->save();
                int r2 = game->play(op2);
                if (r2 > bestFuture) bestFuture = r2;
                game->load(save2);
                game->erase(save2);
            }
        }
        cerr << ", bestFuture=" << bestFuture << ", score=" << (r1*2 + bestFuture) << endl;
        
        game->load(save1);
        game->erase(save1);
    }
    
    delete game;
    return 0;
}
