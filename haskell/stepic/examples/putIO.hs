module Main where

main = do
  putStrLn "What is your name?"
  name <- getLine
  putStrLn $ "Nice to meet you, " ++ name ++ "!"

main' = putStrLn "What is your name?" >>
	getLine			      >>= (\name ->
	putStrLn $ "Nice to meet you, " ++ name ++ "!") 
