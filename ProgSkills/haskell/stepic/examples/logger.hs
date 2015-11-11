data Log a = Log [String] a
		deriving Show

toLogger :: (a -> b) -> String -> (a -> Log b)
toLogger f msg x = Log [msg] (f x)

execLoggers :: a -> (a -> Log b) -> (b -> Log c) -> Log c
execLoggers x f g = Log [msg1, msg2] res
			where Log [msg1] mid = f x
 		              Log [msg2] res = g mid
			      
