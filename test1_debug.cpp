#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "game.h"

using namespace std;

char ops[] = {'A', 'B', 'C', 'D', 'E'};

char findBestMove(Game* game) {
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
        
        game->load(save1);
        game->erase(save1);
    }
    
    sort(scores.begin(), scores.end());
    return scores[0].op;
}

int main() {
    auto mapfile = ifstream("/workspace/data/021/testcases/1.in");
    Game *game = new Game(mapfile);
    
    cerr << "Total bricks: " << game->bricksTotal() << endl;
    
    vector<char> operations;
    int totalReward = 0;
    
    while (game->bricksRemaining() > 0 && operations.size() < (size_t)game->m) {
        char op = findBestMove(game);
        int r = game->play(op);
        totalReward += r;
        operations.push_back(op);
        cerr << "Op #" << operations.size() << ": " << op << ", reward=" << r << ", total=" << totalReward << ", remaining=" << game->bricksRemaining() << endl;
    }
    
    cerr << "\nFinal: " << operations.size() << " operations, total reward=" << totalReward << endl;
    
    for (char op : operations) {
        cout << op << endl;
    }
    
    delete game;
    return 0;
}
