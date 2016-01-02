{-# LANGUAGE ScopedTypeVariables #-}
class (Eq a, Enum a, Bounded a) => SafeEnum a where
  ssucc :: a -> a
--  ssucc (maxBound :: a) = (minBound :: a)
  ssucc x = if x == (maxBound :: a) then (minBound :: a) else succ x

  spred :: a -> a
 -- spred (minBound :: a) = (maxBound :: a)
  spred x = if x == (minBound :: a) then (maxBound :: a) else pred x




