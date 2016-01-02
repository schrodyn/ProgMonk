fibonacci_pos n = fibonacci_pos' n 0 1
fibonacci_pos' 0 p q = p
fibonacci_pos' 1 p q = q
fibonacci_pos' n p q = fibonacci_pos' (n-1) q (p+q)


fibonacci_neg n = fibonacci_neg' (n+2) 1 (-1)
fibonacci_neg' 0 p q = q
fibonacci_neg' 1 p q = p
fibonacci_neg' n p q = fibonacci_neg' (n+1) q (-q+p)

fibonacci n | n >= 0    = fibonacci_pos n
            | otherwise = fibonacci_neg n

