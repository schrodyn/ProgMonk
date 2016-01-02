import Data.Char
data Token = Number Int | Plus | Minus | LeftBrace | RightBrace     
      deriving (Eq, Show)
-- Тип Token уже объявлен, его писать не нужно


asToken :: String -> Maybe Token
asToken s | isNumber s = Just (Number (read s :: Int))
	  | s == "+"   = Just Plus
          | s == "-"   = Just Minus
          | s == "("   = Just LeftBrace
	  | s == ")"   = Just RightBrace
	  | otherwise  = Nothing
	where isNumber [x] = isDigit x
              isNumber (x:xs) = (isDigit x) && (isNumber xs)

              

tokenize :: String -> Maybe [Token]
tokenize input = tokenize' (words input) (Just [])
		where 	tokenize' [] res = revTokens res
			tokenize' (x:xs) res = tokenize' xs (addToken (asToken x) res)

addToken (Just x) (Just xs) = Just (x:xs)
addToken Nothing  _ = Nothing

revTokens (Just xs) = Just (reverse xs)
revTokens Nothing = Nothing
