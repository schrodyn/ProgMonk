module Demo where

data Odd = Odd Integer deriving (Eq,Show)

instance Enum Odd where
    succ (Odd a) = Odd (a+2)
    pred (Odd a) = Odd (a-2)
    fromEnum (Odd a) = fromIntegral(a)--if a <= (2^31 - 1) then fromIntegral(a) else (2^31 - 1)
    toEnum a = Odd (toInteger a)
    enumFrom (Odd a) = map (\a -> Odd a) $ filter odd [a..]
    enumFromThen (Odd a) (Odd b) = map (\a -> Odd a) $ filter odd [a,b..] 
    enumFromTo (Odd a) (Odd b) = map (\a -> Odd a) $ filter odd [a..b]
    enumFromThenTo (Odd a) (Odd b) (Odd c)= map (\a -> Odd a) $ filter odd [a,b..c]
