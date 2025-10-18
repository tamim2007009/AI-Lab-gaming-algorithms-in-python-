import time
import copy

# Sample Sudoku puzzle (0 = empty)
puzzle = [
    [8,0,0,0,0,0,0,0,0],
    [0,0,3,6,0,0,0,0,0],
    [0,7,0,0,9,0,2,0,0],
    [0,5,0,0,0,7,0,0,0],
    [0,0,0,0,4,5,7,0,0],
    [0,0,0,1,0,0,0,3,0],
    [0,0,1,0,0,0,0,6,8],
    [0,0,8,5,0,0,0,1,0],
    [0,9,0,0,0,0,4,0,0]
]

# --- Student Task 1: Implement this ---
def is_safe(board, row, col, num):
    """Check if 'num' can be placed at board[row][col]"""
    # Check row
    for c in range(9):
        if board[row][c] == num:
            return False
    # Check column
    for r in range(9):
        if board[r][col] == num:
            return False
    # Check 3x3 subgrid
    start_row = (row // 3) * 3
    start_col = (col // 3) * 3
    for r in range(start_row, start_row + 3):
        for c in range(start_col, start_col + 3):
            if board[r][c] == num:
                return False
    return True  # placeholder

# Helper: compute candidate values for an empty cell
def get_candidates(board, row, col):
    if board[row][col] != 0:
        return set()
    candidates = set(range(1, 10))
    # row
    for c in range(9):
        candidates.discard(board[row][c])
    # col
    for r in range(9):
        candidates.discard(board[r][col])
    # box
    start_row = (row // 3) * 3
    start_col = (col // 3) * 3
    for r in range(start_row, start_row + 3):
        for c in range(start_col, start_col + 3):
            candidates.discard(board[r][c])
    candidates.discard(0)
    return candidates

# --- Student Task 2: Naive backtracking solver ---
naive_calls = 0
def solve_naive(board):
    global naive_calls
    naive_calls += 1
    # TODO: implement naive backtracking
    empty = find_empty(board)
    if not empty:
        return True  # Solved
    row, col = empty
    for num in range(1, 10):
        if is_safe(board, row, col, num):
            board[row][col] = num
            if solve_naive(board):
                return True
            board[row][col] = 0  # Backtrack
    return False

# --- Student Task 3: MRV heuristic solver ---
mrv_calls = 0
def solve_mrv(board):
    global mrv_calls
    mrv_calls += 1
    # Choose the empty cell with Minimum Remaining Values (fewest legal candidates)
    best_cell = None
    best_domain = None
    min_size = 10
    for r in range(9):
        for c in range(9):
            if board[r][c] == 0:
                domain = get_candidates(board, r, c)
                size = len(domain)
                if size == 0:
                    return False
                if size < min_size:
                    min_size = size
                    best_cell = (r, c)
                    best_domain = domain
                if min_size == 1:
                    break
        if min_size == 1:
            break

    if best_cell is None:
        return True  # solved

    r, c = best_cell
    for val in sorted(best_domain):
        if is_safe(board, r, c, val):
            board[r][c] = val
            if solve_mrv(board):
                return True
            board[r][c] = 0
    return False

# --- Student Task 4: MRV + LCV solver ---
mrv_lcv_calls = 0
def count_constraints(board, row, col, num):
    """Count how many choices neighbors lose if 'num' is placed here"""
    # Count neighbors (same row, col, or box) that would lose 'num' from their domain
    neighbors = set()
    for c2 in range(9):
        if c2 != col:
            neighbors.add((row, c2))
    for r2 in range(9):
        if r2 != row:
            neighbors.add((r2, col))
    start_row = (row // 3) * 3
    start_col = (col // 3) * 3
    for r2 in range(start_row, start_row + 3):
        for c2 in range(start_col, start_col + 3):
            if (r2, c2) != (row, col):
                neighbors.add((r2, c2))

    eliminated = 0
    for (r2, c2) in neighbors:
        if board[r2][c2] == 0:
            if num in get_candidates(board, r2, c2):
                eliminated += 1
    return eliminated

# Minimal helper used by MRV/LCV loops above
def count_remaining_values(board, row, col, num):
    """Return how many other values (excluding num) are still legal at (row,col).
    Used to break ties when selecting a value; smaller is more constraining."""
    count = 0
    for v in range(1, 10):
        if v != num and is_safe(board, row, col, v):
            count += 1
    return count

def solve_mrv_lcv(board):
    global mrv_lcv_calls
    mrv_lcv_calls += 1
    # MRV: choose the empty cell with fewest legal candidates
    best_cell = None
    best_domain = None
    min_size = 10
    for r in range(9):
        for c in range(9):
            if board[r][c] == 0:
                domain = get_candidates(board, r, c)
                size = len(domain)
                if size == 0:
                    return False
                if size < min_size:
                    min_size = size
                    best_cell = (r, c)
                    best_domain = domain
                if min_size == 1:
                    break
        if min_size == 1:
            break

    if best_cell is None:
        return True  # Solved

    r, c = best_cell
    # LCV: order by least constraining on neighbors
    ordered_vals = sorted(best_domain, key=lambda v: count_constraints(board, r, c, v))
    for val in ordered_vals:
        if is_safe(board, r, c, val):
            board[r][c] = val
            if solve_mrv_lcv(board):
                return True
            board[r][c] = 0
    return False

# --- Utility: find empty cell ---
def find_empty(board):
    for i in range(9):
        for j in range(9):
            if board[i][j] == 0:
                return i, j
    return None

# --- Runner ---
def run_solver(solver, name):
    board_copy = copy.deepcopy(puzzle)
    start = time.time()
    solver(board_copy)
    end = time.time()
    print(f"{name}: Time = {end-start:.6f}s, Recursive calls = {globals()[name+'_calls']}")
    for row in board_copy:
        print(row)
    print("\n"+"-"*40+"\n")

# --- Execute ---
run_solver(solve_naive, "naive")
run_solver(solve_mrv, "mrv")
run_solver(solve_mrv_lcv, "mrv_lcv")
