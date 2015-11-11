main' :: IO ()
main' = putStrLn "What is your name?" >>
        putStr "Name: "               >>
        getLine                       >>= (\name -> if name == "" then main'
                                                    else putStrLn $ "Hi, " ++ name ++ "!")
