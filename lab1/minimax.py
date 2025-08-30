import math

class Node:
  
    def __init__(self, value, children=None):
    
        self.value = value
   
        self.children = children if children is not None else []

def minimax(node, depth, maximizing_player):

   
    if depth == 0 or not node.children:
        return node.value

    if maximizing_player:
        max_eval = -math.inf
        for child in node.children:
            eval_score = minimax(child, depth - 1, False)
            max_eval = max(max_eval, eval_score)
        return max_eval
    else: 
        min_eval = math.inf
        for child in node.children:
            eval_score = minimax(child, depth - 1, True)
            min_eval = min(min_eval, eval_score)
        return min_eval

if __name__ == "__main__":
  
    d1 = Node(-1)
    d2 = Node(8)
    e1 = Node(-3)
    e2 = Node(-1)
    f1 = Node(2)
    f2 = Node(1)
    g1 = Node(-3)
    g2 = Node(4)

 
    D = Node(0, [d1, d2])
    E = Node(0, [e1, e2])
    F = Node(0, [f1, f2])
    G = Node(0, [g1, g2])


    B = Node(0, [D, E])
    C = Node(0, [F, G])


    A = Node(0, [B, C])

  
    optimal_value = minimax(A, 3, True)

    print(f"Optimal value at root (A): {optimal_value}")