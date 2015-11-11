data Error = ParsingError | IncompleteDataError | IncorrectDataError String
		deriving (Show)

data Person = Person { firstName :: String, lastName :: String, age :: Int }
				deriving (Show)

flatMap _ [] = []
flatMap f (x:xs) = f x ++ flatMap f xs

split ch xs = split' ch xs [] []
	where 	split' ch [] buf res = reverse (reverse(buf):res)
		split' ch (x:xs) buf res = if ch == x then 
						split' ch xs [] ([ch]:(reverse buf):res)
					else split' ch xs (x:buf) res

isNumber [] = True
isNumber (x:xs) = (x >= '0') && (x <= '9') && (isNumber xs)

removeSpaces s = removeSpaces' s []
	where removeSpaces' [] p = reverse p
	      removeSpaces' (x:xs) p = if x == ' ' then removeSpaces' xs p
					else removeSpaces' xs (x:p)

parsePerson :: String -> Either Error Person
parsePerson str = parsePerson' (flatMap (\s -> split '=' s) (split '\n' (removeSpaces str))) (Person{}) 0

parsePerson' ("firstName":"=": name : "\n" : rest) pers count = parsePerson' rest pers{firstName = name} (count+1)
parsePerson' ("lastName":"=": last: "\n": rest) pers count = parsePerson' rest pers{lastName = last} (count+1)
parsePerson' ("age":"=": age) pers count | count < 2 = Left IncompleteDataError
					 | isNumber (age!!0) = Right (pers{age = (read (age!!0)::Int)})
					 | otherwise = Left (IncorrectDataError (age!!0))
parsePerson' (field:"=":value:"\n":rest) pers count = parsePerson' rest pers count
parsePerson' _ _ _ = Left ParsingError

