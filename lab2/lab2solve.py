import math

class State:
    
    def __init__(self, board=None, player='X'):
        if board is None:
            self.board = [[' ' for _ in range(3)] for _ in range(3)]
        else:
            self.board = [row[:] for row in board]
        self.player = player

    def display(self):
        print("\n".join([" | ".join(row) for row in self.board]))
        print("-" * 9)

def actions(state):
    """
    Returns list of available (row, col) tuples.
    """
    ac = []
    for row in range(3):
        for col in range(3):
            if state.board[row][col] == ' ':
                ac.append((row, col))
    return ac

def result(state, action):
    """
    Returns a new state after applying the action.
    """
    row, col = action
    if state.board[row][col] != ' ':
        raise ValueError("Invalid move")

    new_board = [r[:] for r in state.board]
    new_board[row][col] = state.player
    next_player = 'O' if state.player == 'X' else 'X'
    return State(new_board, next_player)

def terminal(state):
    """
    Returns True if the game is over (win or draw).
    """
    return utility(state) != 0 or all(cell != ' ' for row in state.board for cell in row)

def utility(state):
    """
    Returns 1 if X wins, -1 if O wins, 0 otherwise.
    """
    b = state.board
    lines = []

    # Rows, columns, diagonals
    lines.extend(b)  # rows
    lines.extend([[b[r][c] for r in range(3)] for c in range(3)])  # columns
    lines.append([b[i][i] for i in range(3)])  # main diagonal
    lines.append([b[i][2 - i] for i in range(3)])  # anti-diagonal

    for line in lines:
        if line == ['X', 'X', 'X']:
            return 1
        if line == ['O', 'O', 'O']:
            return -1
    return 0

def max_value(state):
    if terminal(state):
        return utility(state), None

    v = -math.inf
    best_action = None
    for action in actions(state):
        value, _ = min_value(result(state, action))
        if value > v:
            v = value
            best_action = action
    return v, best_action

def min_value(state):
    if terminal(state):
        return utility(state), None

    v = math.inf
    best_action = None
    for action in actions(state):
        value, _ = max_value(result(state, action))
        if value < v:
            v = value
            best_action = action
    return v, best_action

def minimax(state):
    if state.player == 'X':   # Maximizer
        return max_value(state)
    else:                     # Minimizer
        return min_value(state)

def play():
    state = State()
    print("Welcome to Tic-Tac-Toe! You are O, AI is X.")
    state.display()

    while not terminal(state):
        if state.player == 'O':
            try:
                row, col = map(int, input("Enter row and col (0-2 space separated): ").split())
            except:
                print("Invalid input. Please enter row and column numbers (0-2).")
                continue

            if (row, col) not in actions(state):
                print("Invalid move! Try again.")
                continue

            state = result(state, (row, col))
        else:
            print("AI is thinking...")
            _, action = minimax(state)
            print(f"AI plays: {action}")
            state = result(state, action)

        state.display()

    score = utility(state)
    if score == 1:
        print("X wins!")
    elif score == -1:
        print("O wins!")
    else:
        print("It's a draw!")

if __name__ == "__main__":
    play()
