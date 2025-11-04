#include <iostream>
#include <vector>
#include <algorithm>
#include "game.h"

using namespace std;

char ops[] = {'A', 'B', 'C', 'D', 'E'};

// 2-level lookahead to balance speed and quality
char findBestMove(Game* game) {
    struct MoveScore {
        char op;
        int totalScore;
        int hits;
        
        bool operator<(const MoveScore& other) const {
            if (hits != other.hits) return hits > other.hits;
            return totalScore > other.totalScore;
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
        
        scores.push_back({op1, r1 * 2 + bestFuture, h1});
        
        game->load(save1);
        game->erase(save1);
    }
    
    sort(scores.begin(), scores.end());
    return scores[0].op;
}

int main() {
    Game *game = new Game(cin);
    
    vector<char> operations;
    int prev_hits = 0;
    int stuck_count = 0;
    
    while (game->bricksRemaining() > 0 && operations.size() < (size_t)game->m) {
        char op;
        
        if (stuck_count > 25) {
            // Try systematic exploration when stuck
            op = ops[(operations.size() / 5) % 5];
            stuck_count = 0;
        } else {
            op = findBestMove(game);
        }
        
        game->play(op);
        operations.push_back(op);
        
        int current_hits = game->bricksHit();
        if (current_hits == prev_hits) {
            stuck_count++;
        } else {
            stuck_count = 0;
            prev_hits = current_hits;
        }
    }
    
    for (char op : operations) {
        cout << op << endl;
    }
    
    delete game;
    return 0;
}
