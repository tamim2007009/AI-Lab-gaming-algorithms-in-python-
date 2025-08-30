import math

class Node:
    def __init__(self, name, value, children=None):
        self.name = name
        self.value = value
        self.children = children if children is not None else []

def minimax(node, depth, maximizing_player):
    if depth == 0 or not node.children:
        return node.value, []   # leaf: return value, no name in path

    if maximizing_player:
        max_eval = -math.inf
        best_path = []
        for child in node.children:
            eval_score, path = minimax(child, depth - 1, False)
            if eval_score > max_eval:
                max_eval = eval_score
                best_path = [child.name] + path
        return max_eval, best_path
    else:
        min_eval = math.inf
        best_path = []
        for child in node.children:
            eval_score, path = minimax(child, depth - 1, True)
            if eval_score < min_eval:
                min_eval = eval_score
                best_path = [child.name] + path
        return min_eval, best_path

if __name__ == "__main__":
    # Leaves
    d1 = Node("d1", -1)
    d2 = Node("d2", 8)
    e1 = Node("e1", -3)
    e2 = Node("e2", -1)
    f1 = Node("f1", 2)
    f2 = Node("f2", 1)
    g1 = Node("g1", -3)
    g2 = Node("g2", 4)

    # Internal nodes
    D = Node("D", 0, [d1, d2])
    E = Node("E", 0, [e1, e2])
    F = Node("F", 0, [f1, f2])
    G = Node("G", 0, [g1, g2])

    # Level B and C
    B = Node("B", 0, [D, E])
    C = Node("C", 0, [F, G])

    # Root A
    A = Node("A", 0, [B, C])

    # Run minimax
    optimal_value, path = minimax(A, 3, True)

    print(f"Optimal value at root (A): {optimal_value}")
    print("Path:", " -> ".join(["A"] + path))
