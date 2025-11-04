#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "game.h"

using namespace std;

char ops[] = {'A', 'B', 'C', 'D', 'E'};

char findBest(Game* game) {
    struct MoveScore {
        char op;
        int totalScore;
        int hits;
        
        bool operator<(const MoveScore& other) const {
            if (totalScore != other.totalScore) return totalScore > other.totalScore;
            return hits > other.hits;
        }
    };
    
    vector<MoveScore> scores;
    
    for (char op1 : ops) {
        Game::Save* save1 = game->save();
        int r1 = game->play(op1);
        int h1 = game->bricksHit();
        
        int bestFuture = 0;
        if (game->bricksRemaining() > 0) {
            for (char op2 : ops) {
                Game::Save* save2 = game->save();
                int r2 = game->play(op2);
                bestFuture = max(bestFuture, r2);
                game->load(save2);
                game->erase(save2);
            }
        }
        
        scores.push_back({op1, r1 + bestFuture, h1});
        cerr << "Op " << op1 << ": total=" << (r1 + bestFuture) << " (r1=" << r1 << ", future=" << bestFuture << ", hits=" << h1 << ")" << endl;
        
        game->load(save1);
        game->erase(save1);
    }
    
    sort(scores.begin(), scores.end());
    cerr << "Best: " << scores[0].op << endl;
    return scores[0].op;
}

int main() {
    auto mapfile = ifstream("/workspace/data/021/testcases/naive.in");
    Game *game = new Game(mapfile);
    
    cerr << "=== Move 1 ===" << endl;
    char op1 = findBest(game);
    game->play(op1);
    cout << op1 << endl;
    
    cerr << "\n=== Move 2 (remaining=" << game->bricksRemaining() << ") ===" << endl;
    char op2 = findBest(game);
    game->play(op2);
    cout << op2 << endl;
    
    cerr << "\nFinal remaining: " << game->bricksRemaining() << endl;
    
    delete game;
    return 0;
}
