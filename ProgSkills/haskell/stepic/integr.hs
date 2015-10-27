integration :: (Double -> Double) -> Double -> Double -> Double
integration f a b | a <= b = integration' f a b ( (b-a) /1000) 0
		  | otherwise = - integration' f b a ((a-b) / 1000 ) 0
		where 
		integration' f a b delta p | abs (b - a) < delta = p * delta
				           | otherwise = integration' f (a+delta) b delta (p + delta_fun f a delta)

delta_fun f a delta = ( f a + f (a + delta) ) / 2
