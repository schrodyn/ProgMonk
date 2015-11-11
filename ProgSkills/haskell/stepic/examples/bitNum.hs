data Bit = Zero | One
data Sign = Minus | Plus
data Z = Z Sign [Bit]
	deriving Show

instance  (Show Bit) where
	show One = "One"
	show Zero = "Zero"

instance (Show Sign) where
	show Plus = "Plus"
	show Minus = "Minus"

toInt Zero = 0
toInt One = 1

toBit 0 = Zero
toBit 1 = One

add :: Z -> Z -> Z
add (Z Plus xs) (Z Plus ys) = Z Plus (add' xs ys)
add (Z Plus xs) (Z Minus ys) = Z Plus (add' xs ( inverse ys ))
add (Z Minus xs) (Z Plus ys) = Z Plus (add' (inverse xs) ys)
add (Z Minus xs) (Z Minus ys) = Z Minus (add' (inverse xs) (inverse ys))

invBit Zero = One
invBit One = Zero

inverse = map invBit 

add' xs [] = xs
add' [] ys = ys
add' xs ys = map toBit $ add'' (map toInt xs) (map toInt ys) 0
            where 
                add'' [] [] bit | bit == 1 = [bit]
                                | otherwise = []
            
                add'' (x:xs) [] bit = if x+bit == 2 then (0 : (add'' xs [] 1))
                                                    else (1 : xs)
                add'' [] (y:ys) bit = if y+bit == 2 then (0 : (add'' [] ys 1))
                                                    else (1 : ys)                                                
                add'' (x:xs) (y:ys) bit | (x+y+bit) == 3 = 1 : ( add'' xs ys 1 )
                                        | (x+y+bit) == 2 = 0 : ( add'' xs ys 1 )
                                        | (x+y+bit) == 1 = 1 : ( add'' xs ys 0 )

mul :: Z -> Z -> Z
mul = undefined
