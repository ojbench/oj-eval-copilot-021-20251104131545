# Solution Summary - Problem 021: Snooker Brick Out

## Final Score: 301,686 / 1,000,000 (30.2%)

### Best Submission
- **Submission ID**: 707685
- **Commit**: 0ddf0df (Adaptive lookahead depth based on remaining bricks)
- **Strategy**: Greedy search with 2-3 level lookahead, adaptive depth

### Submission History

| # | ID | Score | Strategy |
|---|-----|-------|----------|
| 1 | 707677 | 297,476 | Initial greedy with 2-level lookahead |
| 2 | 707681 | 297,468 | 2-level lookahead (refined) |
| 3 | 707683 | 299,642 | Fixed scoring priorities |
| 4 | 707685 | **301,686** | **Adaptive depth (BEST)** |
| 5 | 707690 | 185,644 | Beam search (failed) |

### Key Insights

1. **Problem Complexity**: The game requires hitting all 4n² bricks while maximizing:
   - Base score: +1 per brick
   - Combo bonus: +50 for hitting bricks in color sequence 1→2→3
   - Operation efficiency: Minimize operations while hitting all bricks

2. **Scoring Breakdown**:
   - Game score component: 90% (based on reward/max_reward)
   - Control count component: 10% (based on operation efficiency)

3. **Achieved Results**:
   - Successfully hits all bricks in all test cases
   - Control score: >93% (excellent efficiency)
   - Game score: ~20% (missing most combo bonuses)
   - Overall: ~30% total score

4. **Challenges**:
   - Combo bonuses require hitting colored bricks in specific order
   - Greedy lookahead (2-3 levels) cannot see far enough to plan combos
   - Colored bricks are sparse (~25% of total)
   - Ball physics and collision mechanics make path planning complex

5. **What Worked**:
   - Adaptive lookahead depth (deeper search when fewer bricks remain)
   - Prioritizing total reward over immediate hits
   - Exploration mode when stuck

6. **What Didn't Work**:
   - Beam search (too slow or implementation issues)
   - Simple greedy without lookahead
   - Over-weighting immediate rewards

### Technical Approach

The final solution uses:
- Adaptive 2-3 level lookahead based on remaining bricks
- Greedy selection of best path considering future rewards
- Exploration mode to escape local optima
- O(5²×5) = O(125) simulations per move (with adaptive depth)

### Limitations

- Cannot effectively plan multi-move combos (requires 10+ move lookahead)
- Save/load operations are O(n²), limiting deeper search
- Theoretical maximum includes combos that may require perfect planning
- 30% score is respectable but far from optimal

### Potential Improvements (if allowed more attempts)

1. Monte Carlo Tree Search (MCTS) for better long-term planning
2. Reinforcement learning to learn combo patterns
3. Dynamic programming to cache and reuse game states
4. Heuristic-guided search specifically targeting colored brick sequences
5. Pattern-based strategies (e.g., systematic sweeping)

