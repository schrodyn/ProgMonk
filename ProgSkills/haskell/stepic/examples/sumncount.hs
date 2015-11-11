sum'n'count 0 = (0,1)
sum'n'count x 	| x >= 0 = sum'n'count' x (0,0)
		| x < 0  = sum'n'count' (-x) (0,0)
		where
			sum'n'count' 0 (p,q) = (p,q)
			sum'n'count' x (p,q) = sum'n'count' (div x 10) (p+ mod x 10, q+1)
