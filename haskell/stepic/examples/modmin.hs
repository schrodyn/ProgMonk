module Modmin where

infixl 6 |-|
x |-| y = if x > y then x - y else y-x
