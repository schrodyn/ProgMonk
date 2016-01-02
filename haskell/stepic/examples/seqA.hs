seqA 0 = 1
seqA 1 = 2
seqA 2 = 3
seqA n = let
		seqA' 0 p q r = r
		seqA' n p q r = seqA' (n-1) q r (r+q-2 * p)
	  in seqA' (n-2) 1 2 3

 
