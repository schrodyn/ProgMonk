integration :: (Double -> Double) -> Double -> Double -> Double
integration f a b = delta * (total_sum + (f(a) + f(b))/2)
		where
		  total_sum = foldl (+) 0 $ map (\x -> f(a+delta*x)) [1..(n-1)] 
		  n = 10000
		  delta = (b-a)/n
