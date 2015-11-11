type Endo a = a -> a

g :: Int -> Int
g x = 2*x

f :: Endo (Endo Int)
f x = x 
