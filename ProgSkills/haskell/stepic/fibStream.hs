fibStream :: [Integer]
fibStream = 0 : 1 : fibstream' 0 1 where
        fibstream' x y = x+y : fibstream' y (x+y)
