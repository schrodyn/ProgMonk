--change :: (Ord a, Num a) => a -> [[a]]

change n coins = map (\(_,y) -> y!!0) $ filter (\(x,_)->x) (change' n coins)
change' 0 coins = [(True, [[]])]
change' n coins | n < minimum coins = [(False,[[]])]
	       | otherwise  = [if fs then (True, map (\xs -> x:xs) ls) 
				else  (False,[[]]) |x<-coins, (fs,ls) <- change' (n-x) coins]
--change n coins = map (\xs -> (x:xs)) change (n-x) coins 
