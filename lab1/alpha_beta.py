import math

class Node:
    def __init__(self, value, children=None):
        self.value = value
        self.children = children if children is not None else []

def minimax_alpha_beta(node, depth, maximizing_player, alpha, beta):
    if depth == 0 or not node.children:
        return node.value

    if maximizing_player:
        max_eval = -math.inf
        for child in node.children:
            eval_score = minimax_alpha_beta(child, depth - 1, False, alpha, beta)
            max_eval = max(max_eval, eval_score)
            alpha = max(alpha, eval_score)
            if beta <= alpha:
                break
        return max_eval
    else:
        min_eval = math.inf
        for child in node.children:
            eval_score = minimax_alpha_beta(child, depth - 1, True, alpha, beta)
            min_eval = min(min_eval, eval_score)
            beta = min(beta, eval_score)
            if beta <= alpha:
                break
        return min_eval

if __name__ == "__main__":
    n1, n2, n3, n4 = Node(3), Node(4), Node(2), Node(1)
    n5, n6, n7, n8 = Node(7), Node(8), Node(9), Node(10)
    n9, n10, n11, n12 = Node(2), Node(11), Node(1), Node(12)
    n13, n14, n15, n16 = Node(14), Node(9), Node(13), Node(16)

    min1 = Node(0, [n1, n2])
    min2 = Node(0, [n3, n4])
    min3 = Node(0, [n5, n6])
    min4 = Node(0, [n7, n8])
    min5 = Node(0, [n9, n10])
    min6 = Node(0, [n11, n12])
    min7 = Node(0, [n13, n14])
    min8 = Node(0, [n15, n16])

    max1 = Node(0, [min1, min2])
    max2 = Node(0, [min3, min4])
    max3 = Node(0, [min5, min6])
    max4 = Node(0, [min7, min8])

    min9 = Node(0, [max1, max2])
    min10 = Node(0, [max3, max4])

    root = Node(0, [min9, min10])

    optimal_value = minimax_alpha_beta(root, 4, True, -math.inf, math.inf)

    print(f"Optimal value: {optimal_value}")