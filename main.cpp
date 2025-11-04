#include <iostream>
#include <vector>
#include <algorithm>
#include "game.h"

using namespace std;

char ops[] = {'A', 'B', 'C', 'D', 'E'};

// Adaptive lookahead based on remaining bricks
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
    int remaining = game->bricksRemaining();
    
    // Adaptive depth: use deeper search when fewer bricks remain
    bool useDeepSearch = (remaining < 100) || (remaining < game->bricksTotal() / 4);
    
    for (char op1 : ops) {
        Game::Save* save1 = game->save();
        int r1 = game->play(op1);
        int h1 = game->bricksHit();
        
        int bestPath = r1;
        
        if (game->bricksRemaining() > 0) {
            for (char op2 : ops) {
                Game::Save* save2 = game->save();
                int r2 = game->play(op2);
                
                int path2 = r1 + r2;
                
                if (useDeepSearch && game->bricksRemaining() > 0) {
                    // Level 3
                    int best3 = 0;
                    for (char op3 : ops) {
                        Game::Save* save3 = game->save();
                        int r3 = game->play(op3);
                        best3 = max(best3, r3);
                        game->load(save3);
                        game->erase(save3);
                    }
                    path2 += best3;
                }
                
                bestPath = max(bestPath, path2);
                
                game->load(save2);
                game->erase(save2);
            }
        }
        
        scores.push_back({op1, bestPath, h1});
        
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
        
        if (stuck_count > 30) {
            // When stuck, try exploration
            op = ops[(operations.size() / 10) % 5];
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
