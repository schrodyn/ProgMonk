data Bit = Zero | One
data Sign = Minus | Plus
data Z = Z Sign [Bit]
        deriving Show

instance (Show Bit) where
	show Zero = "Zero"
	show One = "One"

instance (Show Sign) where
	show Plus = "Plus"
	show Minus = "Minus" 
        
bitToInt Zero = 0
bitToInt One = 1

toInt :: Z -> Int
toInt (Z Plus xs) = toInt' (map bitToInt xs) 1
        where toInt' [] p   = 0
              toInt' (x:xs) p = x * p + (toInt' xs p*2)
              
toInt (Z Minus xs) = - toInt (Z Plus xs)

add :: Z -> Z -> Z
add z1 z2 = toZ $ (toInt z1) + (toInt z2)

mul :: Z -> Z -> Z
mul z1 z2 = toZ $ (toInt z1) * (toInt z2)

toZ :: Int -> Z
toZ int | int >= 0 = Z Plus (toBitList int)
        | otherwise = Z Minus (toBitList (-int))
        where
            toBitList 0 = []
            toBitList int = if int `mod` 2 == 0 then Zero : (toBitList (int `div` 2))
                                else One : (toBitList (int `div` 2))
