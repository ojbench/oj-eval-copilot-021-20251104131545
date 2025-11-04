#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include "game.h"

using namespace std;

char ops[] = {'A', 'B', 'C', 'D', 'E'};

// Beam search to explore multiple paths
struct BeamNode {
    vector<char> path;
    int totalReward;
    int hits;
    Game::Save* save;
    
    bool operator<(const BeamNode& other) const {
        if (totalReward != other.totalReward) return totalReward < other.totalReward;
        return hits < other.hits;
    }
};

char findBestMoveBeam(Game* game) {
    int beamWidth = 3;  // Keep top 3 paths
    int depth = 4;  // Search 4 moves ahead
    
    priority_queue<BeamNode> beam;
    beam.push({{}, 0, game->bricksHit(), game->save()});
    
    for (int d = 0; d < depth && !beam.empty(); d++) {
        priority_queue<BeamNode> nextBeam;
        vector<BeamNode> currentLevel;
        
        // Get all nodes from current beam
        while (!beam.empty()) {
            currentLevel.push_back(beam.top());
            beam.pop();
        }
        
        // Expand each node
        for (auto& node : currentLevel) {
            game->load(node.save);
            
            if (game->bricksRemaining() == 0) {
                game->erase(node.save);
                continue;
            }
            
            for (char op : ops) {
                Game::Save* newSave = game->save();
                int reward = game->play(op);
                
                BeamNode newNode;
                newNode.path = node.path;
                newNode.path.push_back(op);
                newNode.totalReward = node.totalReward + reward;
                newNode.hits = game->bricksHit();
                newNode.save = game->save();
                
                nextBeam.push(newNode);
                
                game->load(newSave);
                game->erase(newSave);
            }
            
            game->erase(node.save);
        }
        
        // Keep only top beamWidth nodes
        beam = priority_queue<BeamNode>();
        for (int i = 0; i < beamWidth && !nextBeam.empty(); i++) {
            beam.push(nextBeam.top());
            nextBeam.pop();
        }
        
        // Clean up remaining nodes
        while (!nextBeam.empty()) {
            game->erase(nextBeam.top().save);
            nextBeam.pop();
        }
    }
    
    // Get best path
    if (beam.empty()) return 'C';
    
    BeamNode best = beam.top();
    
    // Clean up
    while (!beam.empty()) {
        game->erase(beam.top().save);
        beam.pop();
    }
    
    return best.path.empty() ? 'C' : best.path[0];
}

// Simple greedy for when beam is too slow
char findBestMoveGreedy(Game* game) {
    int bestScore = -1;
    char bestOp = 'C';
    
    for (char op : ops) {
        Game::Save* save = game->save();
        int r = game->play(op);
        
        if (r > bestScore) {
            bestScore = r;
            bestOp = op;
        }
        
        game->load(save);
        game->erase(save);
    }
    
    return bestOp;
}

int main() {
    Game *game = new Game(cin);
    
    vector<char> operations;
    int prev_hits = 0;
    int stuck_count = 0;
    
    while (game->bricksRemaining() > 0 && operations.size() < (size_t)game->m) {
        char op;
        
        int remaining = game->bricksRemaining();
        
        if (stuck_count > 40) {
            // Exploration
            op = ops[(operations.size() / 10) % 5];
            stuck_count = 0;
        } else if (remaining < 50 || remaining < game->bricksTotal() / 8) {
            // Use beam search for endgame or when few bricks left
            op = findBestMoveBeam(game);
        } else {
            // Use greedy for speed
            op = findBestMoveGreedy(game);
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
