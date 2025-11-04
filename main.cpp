#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>
#include "game.h"

using namespace std;

char ops[] = {'A', 'B', 'C', 'D', 'E'};

// Greedy BFS to find best immediate move
char findBestMove(Game* game, int depth = 1) {
    int bestScore = -1;
    char bestOp = 'C';
    int bestHits = 0;
    
    for (char op : ops) {
        Game::Save* save = game->save();
        int reward = game->play(op);
        int hits = game->bricksHit();
        
        // Prefer moves that hit bricks, then maximize reward
        if (hits > bestHits || (hits == bestHits && reward > bestScore)) {
            bestScore = reward;
            bestOp = op;
            bestHits = hits;
        }
        
        game->load(save);
        game->erase(save);
    }
    
    return bestOp;
}

int main() {
    Game *game = new Game(cin);
    
    vector<char> operations;
    int total_bricks = game->bricksTotal();
    int prev_hits = 0;
    int no_progress_count = 0;
    
    // Try to hit all bricks
    while (game->bricksRemaining() > 0 && operations.size() < (size_t)game->m) {
        char op = findBestMove(game, 1);
        game->play(op);
        operations.push_back(op);
        
        int current_hits = game->bricksHit();
        if (current_hits == prev_hits) {
            no_progress_count++;
            // If no progress for many moves, try random exploration
            if (no_progress_count > 20) {
                op = ops[operations.size() % 5];
                game->play(op);
                operations.push_back(op);
                no_progress_count = 0;
            }
        } else {
            no_progress_count = 0;
            prev_hits = current_hits;
        }
        
        // Early exit if we've tried too many operations
        if (operations.size() > (size_t)(game->m * 0.8)) {
            break;
        }
    }
    
    // Output all operations
    for (char op : operations) {
        cout << op << endl;
    }
    
    delete game;
    return 0;
}
