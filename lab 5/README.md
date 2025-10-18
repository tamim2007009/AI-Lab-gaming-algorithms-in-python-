# Sudoku Solver: Backtracking, MRV, and LCV

This lab implements three Sudoku solvers for the same 9×9 puzzle in `suduku.py` and compares their performance:

- `solve_naive`: Plain recursive backtracking with a simple variable/value order
- `solve_mrv`: Backtracking with the MRV (Minimum Remaining Values) variable-ordering heuristic
- `solve_mrv_lcv`: Backtracking with MRV plus the LCV (Least Constraining Value) value-ordering heuristic

The code also measures wall-clock time and counts recursive calls for each solver.

## Problem formulation (CSP view)

- Variables: Each grid cell `(r, c)` with value `0..9`; `0` means unassigned
- Domains: For unassigned cells, `1..9` (restricted by constraints)
- Constraints:
  - Row constraint: All numbers in a row must be different
  - Column constraint: All numbers in a column must be different
  - Box constraint: All numbers in each 3×3 box must be different

A solution is a complete assignment with all constraints satisfied.

## Algorithms

### 1) Naive backtracking (`solve_naive`)

- Selects the first unassigned cell (top-left to bottom-right), tries values `1..9` in order
- Uses `is_safe` to check if placing a value violates any row/column/box constraint
- On dead-end, backtracks to try a different value upstream

Properties:

- Complete and correct: finds a solution if one exists, otherwise proves unsatisfiable by exhausting search
- Exponential worst-case time; performance depends heavily on ordering

### 2) MRV heuristic (`solve_mrv`)

- Variable ordering (which cell to fill next): choose the empty cell with the Minimum Remaining Values (the smallest domain)
- Intuition: commit early on the most constrained choice to detect dead-ends sooner and shrink the search tree
- Value ordering: tries the candidate values in ascending order (arbitrary but stable)

Effect:

- Reduces branching factor by picking the tightest cell first
- Preserves completeness and correctness

### 3) MRV + LCV (`solve_mrv_lcv`)

- Variable ordering: MRV (as above)
- Value ordering: LCV (Least Constraining Value) — prefer values that eliminate the fewest options for neighboring cells
- Implementation detail: `count_constraints` estimates how many neighbors would lose `num` as a candidate if placed now. Values with smaller counts are tried first.

Effect:

- Further reduces the expected branching factor compared to MRV alone
- Still complete and correct

## Key helper functions

- `is_safe(board, row, col, num)`: Checks row/column/box to decide if `num` can be placed at `(row, col)`
- `find_empty(board)`: Finds the next unassigned cell (used by the naive approach)
- `get_candidates(board, row, col)`: Computes the legal domain of an empty cell (used by MRV/LCV)
- `count_constraints(board, row, col, num)`: Approximates how constraining a choice is by counting neighbor candidate eliminations

## Performance notes

- All backtracking variants are exponential in the worst case, but heuristics greatly reduce the practical search effort
- Typical behavior on the provided puzzle (numbers will vary by machine):
  - Naive: ~50k recursive calls
  - MRV: ~13k recursive calls
  - MRV+LCV: ~11–12k recursive calls
- Heuristics influence the shape of the search tree, not correctness; all three produce the same solved grid

## How to run

On Windows PowerShell:

```powershell
python ".\suduku.py"
```

The script prints, for each solver:

- Elapsed time in seconds
- Number of recursive calls
- The final board (or a message if no solution is found)

## Modifying the puzzle

- Edit the top-level `puzzle` constant in `suduku.py`
- Use `0` for empty cells
- Ensure the puzzle is valid (no immediate constraint violations). The solvers will backtrack indefinitely for impossible inputs but terminate when they detect contradictions while searching.

## Correctness and completeness

- Backtracking guarantees completeness: if a solution exists, it will be found
- `is_safe` enforces the Sudoku constraints locally at each placement
- MRV/LCV are heuristics that only affect the search order; they never prune valid solutions

## Extensions (ideas for further study)

- Forward checking: After each assignment, update neighbors’ domains and fail fast if any domain becomes empty
- Arc consistency (AC-3): Enforce consistency along arcs to prune values before/while searching
- Additional heuristics: Degree heuristic (break MRV ties by most constraining variable), dynamic value ordering
- Alternative algorithms: Exact cover with Dancing Links (Algorithm X), constraint programming solvers, SAT encodings

## Troubleshooting

- If the solver prints the initial board unchanged for MRV/MRV+LCV, verify that the helpers `get_candidates` and `count_constraints` are present and used by the solvers (they are in the provided version)
- For very hard or invalid puzzles, search may be slow; try adding forward checking or AC-3 for more pruning

---

File of interest: `suduku.py`

- Entrypoint at the bottom runs all three solvers and prints results
- Feel free to instrument or log intermediate choices for learning/analysis
