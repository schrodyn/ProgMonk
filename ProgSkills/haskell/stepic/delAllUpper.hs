delAllUpper :: String -> String
delAllUpper = unwords . delUpper . words where
            notUpper = any (\x -> x < 'A' || x > 'Z')
	    delUpper = filter notUpper
