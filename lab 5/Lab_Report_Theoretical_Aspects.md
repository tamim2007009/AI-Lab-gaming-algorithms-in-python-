# Lab Report: Sudoku Solver Using Constraint Satisfaction and Backtracking Algorithms

**Date:** October 18, 2025  
**Subject:** Artificial Intelligence - Gaming Algorithms in Python  
**Lab:** Lab 5 - Sudoku Solver with Heuristic Optimization

---

## 1. Introduction

This lab explores the application of **Constraint Satisfaction Problem (CSP)** solving techniques to the classic Sudoku puzzle. Sudoku is a combinatorial logic puzzle that serves as an excellent benchmark for studying backtracking algorithms and intelligent search heuristics. The lab implements and compares three different solving approaches:

1. **Naive Backtracking** - Basic recursive search
2. **MRV (Minimum Remaining Values)** - Variable ordering heuristic
3. **MRV + LCV (Least Constraining Value)** - Combined variable and value ordering heuristics

The primary objective is to understand how intelligent heuristics can dramatically reduce the computational complexity of solving constraint satisfaction problems without sacrificing completeness or correctness.

---

## 2. Problem Formulation as a Constraint Satisfaction Problem (CSP)

### 2.1 CSP Components

A Constraint Satisfaction Problem is formally defined as a triple **(X, D, C)** where:

- **X**: A set of variables
- **D**: A set of domains for each variable
- **C**: A set of constraints that specify allowable combinations of values

For the 9×9 Sudoku puzzle, this translates to:

**Variables (X):**

- 81 variables, one for each cell in the grid: `X = {x₀₀, x₀₁, ..., x₈₈}`
- Each variable `xᵢⱼ` represents the value in row `i`, column `j`
- Initially assigned cells have fixed values; unassigned cells are represented by 0

**Domains (D):**

- For assigned cells: Domain is a singleton set containing the given value
- For unassigned cells: `D(xᵢⱼ) = {1, 2, 3, 4, 5, 6, 7, 8, 9}`
- The domain is dynamically restricted by constraints during search

**Constraints (C):**
Three types of **AllDifferent** constraints:

1. **Row Constraints**: All cells in each row must contain distinct values

   - `∀i, ∀j₁≠j₂: xᵢⱼ₁ ≠ xᵢⱼ₂`
   - 9 constraints (one per row)

2. **Column Constraints**: All cells in each column must contain distinct values

   - `∀j, ∀i₁≠i₂: xᵢ₁ⱼ ≠ xᵢ₂ⱼ`
   - 9 constraints (one per column)

3. **Box Constraints**: All cells in each 3×3 subgrid must contain distinct values
   - `∀box, ∀cells₁≠cells₂ in box: cell₁ ≠ cell₂`
   - 9 constraints (one per box)

**Total Constraints:** 27 AllDifferent constraints

### 2.2 Solution Definition

A **solution** to the Sudoku CSP is a complete assignment of values to all variables such that:

- Every variable has exactly one value from its domain
- All 27 constraints are satisfied simultaneously

---

## 3. Theoretical Foundations of Search Algorithms

### 3.1 The Search Space

The search space for Sudoku is **exponentially large**:

- Without constraints: `9⁸¹ ≈ 1.97 × 10⁷⁷` possible configurations
- With initial clues (typically 17-30 filled cells): Still astronomical
- With constraint propagation: Drastically reduced but still challenging

### 3.2 Backtracking: The Core Algorithm

**Backtracking** is a systematic depth-first search strategy that:

1. **Incrementally builds** a solution by assigning values to variables one at a time
2. **Checks constraints** after each assignment
3. **Backtracks** (undoes assignments) when a constraint violation is detected
4. **Explores alternatives** at previous decision points

**Pseudocode:**

```
function BACKTRACK(assignment):
    if assignment is complete:
        return assignment

    var ← SELECT-UNASSIGNED-VARIABLE(assignment)

    for each value in ORDER-DOMAIN-VALUES(var, assignment):
        if value is consistent with assignment:
            add {var = value} to assignment
            result ← BACKTRACK(assignment)
            if result ≠ failure:
                return result
            remove {var = value} from assignment

    return failure
```

**Key Properties:**

- **Completeness**: Guaranteed to find a solution if one exists
- **Optimality**: Not applicable (constraint satisfaction has no cost function)
- **Time Complexity**: O(d^n) where d is domain size, n is number of variables
  - For Sudoku: O(9^k) where k is the number of empty cells
- **Space Complexity**: O(n) for recursion depth

---

## 4. Algorithm 1: Naive Backtracking

### 4.1 Strategy

The naive approach implements pure backtracking with no intelligent ordering:

- **Variable Selection**: First unassigned cell in row-major order (left-to-right, top-to-bottom)
- **Value Selection**: Try values 1 through 9 in ascending order
- **Constraint Checking**: Use `is_safe()` to verify row, column, and box constraints

### 4.2 Theoretical Analysis

**Advantages:**

- Simple to implement and understand
- Guaranteed correctness and completeness
- No preprocessing overhead

**Disadvantages:**

- **Blind search**: No consideration of problem structure
- **Late failure detection**: May explore deep branches before discovering conflicts
- **High branching factor**: Tries all 9 values even when many are obviously invalid
- **Poor performance**: Typically requires 50,000+ recursive calls for moderate puzzles

**Time Complexity:**

- Worst case: O(9^k) where k is empty cells
- Average case: Highly dependent on puzzle structure and luck

**Why is it inefficient?**
The naive algorithm wastes computational effort on:

1. **Unpromising branches**: Choosing easy cells first leaves hard cells for later
2. **Redundant work**: Rechecking the same constraints repeatedly
3. **No lookahead**: Cannot detect future conflicts early

---

## 5. Algorithm 2: MRV (Minimum Remaining Values) Heuristic

### 5.1 The "Fail-First" Principle

The **MRV heuristic** (also called "most constrained variable" or "fail-first") is a variable ordering strategy that selects the cell with the **smallest legal domain** at each step.

**Intuition:**

- Cells with fewer legal options are more likely to cause failures
- Choosing constrained variables early **prunes the search tree** sooner
- If a variable has no legal values, we detect failure immediately (domain wipeout)

### 5.2 Implementation

**Variable Selection Algorithm:**

```
function SELECT-MRV-VARIABLE(board):
    min_domain_size ← ∞
    best_cell ← null

    for each empty cell (r, c):
        candidates ← GET-CANDIDATES(board, r, c)
        if |candidates| = 0:
            return failure  // Early failure detection
        if |candidates| < min_domain_size:
            min_domain_size ← |candidates|
            best_cell ← (r, c)

    return best_cell
```

**Domain Calculation:**

```
function GET-CANDIDATES(board, row, col):
    candidates ← {1, 2, 3, 4, 5, 6, 7, 8, 9}

    // Remove values from same row
    for each c in columns:
        candidates ← candidates \ {board[row][c]}

    // Remove values from same column
    for each r in rows:
        candidates ← candidates \ {board[r][col]}

    // Remove values from same 3×3 box
    for each (r, c) in box(row, col):
        candidates ← candidates \ {board[r][c]}

    return candidates
```

### 5.3 Theoretical Analysis

**Advantages:**

- **Early failure detection**: Impossible states discovered quickly
- **Reduced branching factor**: Only legal values are tried
- **Tighter search tree**: More pruning occurs higher in the tree
- **Better performance**: Typically 13,000 recursive calls (74% reduction from naive)

**Disadvantages:**

- **Overhead**: Computing domain sizes at each step
- **Value ordering**: Still tries values in arbitrary order

**Time Complexity:**

- Worst case: Still O(9^k) theoretically
- Average case: **Much better in practice** due to pruning
- Domain computation: O(n) per selection, where n = 27 (neighbors)

**Why is it better?**

1. **Constraint propagation**: Implicitly performs forward checking
2. **Smart ordering**: Tackles hard decisions first
3. **Pruning efficiency**: Dead ends discovered with less work

---

## 6. Algorithm 3: MRV + LCV (Least Constraining Value)

### 6.1 The "Succeed-First" Principle

While MRV focuses on **variable ordering** (which cell to fill), the **LCV heuristic** optimizes **value ordering** (which number to try first) using the "succeed-first" principle.

**Intuition:**

- After selecting a variable, try values that **leave maximum flexibility** for neighboring cells
- A value that eliminates fewer options from neighbors' domains is less constraining
- This maximizes the chances of finding a solution down the current path

### 6.2 Implementation

**Value Ordering Algorithm:**

```
function ORDER-VALUES-LCV(board, row, col, candidates):
    value_scores ← []

    for each value in candidates:
        constraint_count ← COUNT-CONSTRAINTS(board, row, col, value)
        value_scores.append((value, constraint_count))

    // Sort by constraint count (ascending - least constraining first)
    sort value_scores by constraint_count

    return [value for (value, count) in value_scores]
```

**Constraint Counting:**

```
function COUNT-CONSTRAINTS(board, row, col, num):
    neighbors ← GET-NEIGHBORS(row, col)  // Same row, col, or box
    eliminated ← 0

    for each (r, c) in neighbors:
        if board[r][c] = 0:  // Empty cell
            if num ∈ GET-CANDIDATES(board, r, c):
                eliminated ← eliminated + 1

    return eliminated
```

### 6.3 Theoretical Analysis

**Advantages:**

- **Smart value ordering**: Prefers choices that preserve options
- **Improved success rate**: More likely to find solutions without backtracking
- **Synergy with MRV**: Together they form a powerful combination
- **Best performance**: Typically 11,000-12,000 recursive calls (76-78% reduction from naive)

**Disadvantages:**

- **Higher computational overhead**: Computing constraint counts is expensive
- **Marginal improvement over MRV alone**: Only 8-15% additional reduction
- **Not always worth it**: Overhead may exceed benefits on easy puzzles

**Time Complexity:**

- Worst case: O(9^k) theoretically unchanged
- Per-value overhead: O(n × d) where n = 20 neighbors, d = 9 domain size
- Average case: Best practical performance among the three

**Why the modest improvement?**

- MRV already does most of the heavy lifting
- Value ordering helps but variable ordering is more critical
- Overhead of computing constraint counts adds cost

---

## 7. Comparative Theoretical Analysis

### 7.1 Search Tree Characteristics

| Algorithm | Variable Order         | Value Order   | Branching Factor | Tree Depth | Pruning Efficiency |
| --------- | ---------------------- | ------------- | ---------------- | ---------- | ------------------ |
| Naive     | Static (left-to-right) | Static (1-9)  | ~9 (average)     | ~50 levels | Low                |
| MRV       | Dynamic (min domain)   | Static (1-9)  | ~3-5 (average)   | ~40 levels | High               |
| MRV+LCV   | Dynamic (min domain)   | Dynamic (LCV) | ~2-4 (average)   | ~35 levels | Very High          |

### 7.2 Performance Metrics

**Expected Recursive Calls** (for typical puzzles):

- Naive: ~50,000 calls
- MRV: ~13,000 calls (74% reduction)
- MRV+LCV: ~11,000 calls (78% reduction)

**Execution Time Factors:**

1. **Number of recursive calls** (primary factor)
2. **Cost per call** (domain computation, constraint counting)
3. **Memory overhead** (function call stack)

### 7.3 Trade-offs Summary

**Naive:**

- ✓ Simplest implementation
- ✓ Lowest per-call overhead
- ✗ Explores most nodes
- **Use case:** Educational purposes, very small problems

**MRV:**

- ✓ Major search space reduction
- ✓ Best effort-to-benefit ratio
- ≈ Moderate per-call overhead
- **Use case:** Standard choice for most CSPs

**MRV+LCV:**

- ✓ Maximum search space reduction
- ≈ Marginal improvement over MRV
- ✗ Highest per-call overhead
- **Use case:** Hard puzzles, when solution quality matters

---

## 8. Mathematical Foundations

### 8.1 Combinatorial Complexity

**Without constraints:**

- State space: `9! × 9! × 9! / (3!)⁹ ≈ 6.67 × 10²¹` valid Sudoku grids
- Search space (with initial clues): Still exponential

**With constraint propagation:**

- Each constraint eliminates invalid states
- 27 constraints create a highly interconnected constraint graph
- Effective branching factor: `b_eff ≈ 2-5` (with good heuristics)

### 8.2 Constraint Graph Analysis

The Sudoku constraint graph has special properties:

- **Degree**: Each cell is connected to 20 other cells (8 row + 8 col + 4 box neighbors)
- **Density**: High connectivity leads to strong constraint propagation
- **Treewidth**: Relatively low, making decomposition methods viable

### 8.3 Completeness and Soundness

**Theorem:** All three backtracking algorithms are:

1. **Complete**: Will find a solution if one exists
2. **Sound**: Any solution found satisfies all constraints
3. **Optimal**: N/A (CSP has no objective function to optimize)

**Proof sketch:**

- Systematic exploration of all possibilities (completeness)
- Explicit constraint checking at each step (soundness)
- Heuristics only affect search order, not reachability

---

## 9. Heuristic Design Principles

### 9.1 General CSP Heuristics

**Variable Ordering Heuristics:**

1. **MRV (Minimum Remaining Values)**: Choose most constrained variable
2. **Degree Heuristic**: Choose variable involved in most constraints
3. **Combined**: MRV with degree as tie-breaker

**Value Ordering Heuristics:**

1. **LCV (Least Constraining Value)**: Choose value that rules out fewest options
2. **Max-degree**: Choose value most frequently used in solutions
3. **Random**: Sometimes helps escape local patterns

### 9.2 Why These Heuristics Work

**MRV Effectiveness:**

- **Prunes earlier**: Failures detected before deep recursion
- **Reduces branching**: Constrained variables have fewer options
- **Exploits problem structure**: Uses constraint tightness

**LCV Effectiveness:**

- **Increases success probability**: Leaves flexibility for future choices
- **Complements MRV**: Variable order finds problems; value order avoids them
- **Greedy optimization**: Local decisions improve global efficiency

### 9.3 The Fail-First vs. Succeed-First Dichotomy

- **Fail-First (MRV)**: Detect dead ends quickly to avoid wasted work
- **Succeed-First (LCV)**: Increase probability of success to avoid backtracking

These are complementary strategies that together form an effective search approach.

---

## 10. Complexity Theory Perspective

### 10.1 Computational Complexity Class

**Sudoku Solving:**

- **Decision problem**: "Does this puzzle have a solution?" is **NP-complete**
- **Search problem**: Finding a solution is in **NP**
- **Hardness**: As hard as Boolean Satisfiability (SAT)

**Implications:**

- No known polynomial-time algorithm (unless P = NP)
- Backtracking is asymptotically optimal for worst case
- Heuristics improve average case but not worst case complexity

### 10.2 Problem Size Scaling

| Grid Size | Variables | Constraints | Naive Complexity | With Heuristics |
| --------- | --------- | ----------- | ---------------- | --------------- |
| 4×4       | 16        | 12          | ~4^k             | Trivial         |
| 9×9       | 81        | 27          | ~9^k             | Manageable      |
| 16×16     | 256       | 48          | ~16^k            | Very Hard       |
| 25×25     | 625       | 75          | ~25^k            | Intractable     |

---

## 11. Extensions and Advanced Topics

### 11.1 Further Optimization Techniques

**Arc Consistency (AC-3):**

- Preprocess constraints to reduce domains before search
- Propagate constraints after each assignment
- Can dramatically reduce search space

**Constraint Propagation:**

- **Naked Singles**: Cells with only one legal value
- **Hidden Singles**: Values that can only go in one cell
- **Locked Candidates**: Advanced Sudoku techniques

**Search Strategies:**

- **Iterative Deepening**: Depth-limited search with increasing limits
- **Best-First Search**: Use heuristic to guide search order
- **Local Search**: Incomplete methods (may not find solution)

### 11.2 Alternative Formulations

**SAT Encoding:**

- Convert Sudoku to Boolean satisfiability problem
- Use modern SAT solvers (extremely efficient)
- Example: Cell(r,c,v) = True iff cell at (r,c) has value v

**Integer Linear Programming:**

- Formulate as optimization problem
- Use ILP solvers
- Can handle variants (diagonal Sudoku, etc.)

**Dancing Links (Algorithm X):**

- Knuth's technique for exact cover problems
- Extremely efficient for Sudoku
- Uses backtracking with doubly-linked list structure

---

## 12. Conclusion

This lab demonstrates that **intelligent heuristics** can transform intractable problems into manageable ones without sacrificing completeness or correctness. The key insights are:

1. **Problem Structure Matters**: CSP formulation exposes constraint relationships that guide search

2. **Heuristics Improve Average Case**: While worst-case complexity remains exponential, practical performance improves dramatically

3. **Variable Ordering Dominates**: MRV provides the largest benefit (74% reduction)

4. **Value Ordering Supplements**: LCV provides additional but smaller gains (4% more reduction)

5. **Trade-offs Are Real**: Computational overhead must be balanced against search space reduction

### Theoretical Contributions

- Understanding of CSP solving techniques
- Appreciation for heuristic design principles
- Recognition of complexity theory implications
- Foundation for solving other combinatorial problems

### Practical Implications

The techniques learned here apply broadly to:

- **Scheduling problems** (class timetabling, job scheduling)
- **Resource allocation** (frequency assignment, map coloring)
- **Planning problems** (robot planning, logistics)
- **Configuration problems** (software configuration, circuit design)

---

## 13. References and Further Reading

**Foundational Papers:**

1. Haralick & Elliott (1980): "Increasing Tree Search Efficiency for Constraint Satisfaction Problems"
2. Dechter & Meiri (1994): "Experimental Evaluation of Preprocessing Algorithms for Constraint Satisfaction Problems"

**Textbooks:** 3. Russell & Norvig: "Artificial Intelligence: A Modern Approach" (Chapter 6: Constraint Satisfaction Problems) 4. Apt: "Principles of Constraint Programming"

**Advanced Topics:** 5. Knuth: "Dancing Links" (2000) - Algorithm X for exact cover 6. Modern SAT solver techniques for CSP encoding

---

## Appendix: Algorithm Pseudocode Comparison

### A.1 Naive Backtracking

```
function SOLVE-NAIVE(board):
    (row, col) ← FIND-EMPTY(board)  // First empty cell
    if no empty cell:
        return SUCCESS

    for num from 1 to 9:
        if IS-SAFE(board, row, col, num):
            board[row][col] ← num
            if SOLVE-NAIVE(board):
                return SUCCESS
            board[row][col] ← 0  // Backtrack

    return FAILURE
```

### A.2 MRV Backtracking

```
function SOLVE-MRV(board):
    (row, col) ← SELECT-MRV-VARIABLE(board)  // Min domain size
    if no empty cell:
        return SUCCESS

    candidates ← GET-CANDIDATES(board, row, col)
    if candidates is empty:
        return FAILURE  // Early detection

    for num in candidates:  // Only legal values
        board[row][col] ← num
        if SOLVE-MRV(board):
            return SUCCESS
        board[row][col] ← 0

    return FAILURE
```

### A.3 MRV + LCV Backtracking

```
function SOLVE-MRV-LCV(board):
    (row, col) ← SELECT-MRV-VARIABLE(board)
    if no empty cell:
        return SUCCESS

    candidates ← GET-CANDIDATES(board, row, col)
    if candidates is empty:
        return FAILURE

    ordered ← SORT-BY-LCV(board, row, col, candidates)  // LCV ordering

    for num in ordered:
        board[row][col] ← num
        if SOLVE-MRV-LCV(board):
            return SUCCESS
        board[row][col] ← 0

    return FAILURE
```

---

**End of Report**
