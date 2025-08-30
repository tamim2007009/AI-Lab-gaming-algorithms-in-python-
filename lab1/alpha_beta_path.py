import math

class Node:
    def __init__(self, name, value, children=None):
        self.name = name
        self.value = value
        self.children = children if children is not None else []

def minimax_alpha_beta(node, depth, maximizing_player, alpha, beta):
    # Leaf node
    if depth == 0 or not node.children:
        return node.value, [node.name]

    if maximizing_player:
        max_eval = -math.inf
        best_path = []
        for child in node.children:
            eval_score, path = minimax_alpha_beta(child, depth - 1, False, alpha, beta)
            if eval_score > max_eval:
                max_eval = eval_score
                best_path = [node.name] + path
            alpha = max(alpha, eval_score)
            if beta <= alpha:  # prune
                break
        return max_eval, best_path
    else:
        min_eval = math.inf
        best_path = []
        for child in node.children:
            eval_score, path = minimax_alpha_beta(child, depth - 1, True, alpha, beta)
            if eval_score < min_eval:
                min_eval = eval_score
                best_path = [node.name] + path
            beta = min(beta, eval_score)
            if beta <= alpha:  # prune
                break
        return min_eval, best_path


if __name__ == "__main__":
    # Leaves
    n1, n2, n3, n4 = Node("n1", 3), Node("n2", 4), Node("n3", 2), Node("n4", 1)
    n5, n6, n7, n8 = Node("n5", 7), Node("n6", 8), Node("n7", 9), Node("n8", 10)
    n9, n10, n11, n12 = Node("n9", 2), Node("n10", 11), Node("n11", 1), Node("n12", 12)
    n13, n14, n15, n16 = Node("n13", 14), Node("n14", 9), Node("n15", 13), Node("n16", 16)

    # Build tree
    min1 = Node("min1", 0, [n1, n2])
    min2 = Node("min2", 0, [n3, n4])
    min3 = Node("min3", 0, [n5, n6])
    min4 = Node("min4", 0, [n7, n8])
    min5 = Node("min5", 0, [n9, n10])
    min6 = Node("min6", 0, [n11, n12])
    min7 = Node("min7", 0, [n13, n14])
    min8 = Node("min8", 0, [n15, n16])

    max1 = Node("max1", 0, [min1, min2])
    max2 = Node("max2", 0, [min3, min4])
    max3 = Node("max3", 0, [min5, min6])
    max4 = Node("max4", 0, [min7, min8])

    min9 = Node("min9", 0, [max1, max2])
    min10 = Node("min10", 0, [max3, max4])

    root = Node("root", 0, [min9, min10])

    optimal_value, path = minimax_alpha_beta(root, 4, True, -math.inf, math.inf)

    print(f"Optimal value: {optimal_value}")
    print("Path:", " -> ".join(path))
