main' :: IO ()
main' = putStr "Substring: "     >>
        getLine                  >>= (\name ->
        if name == "" then putStrLn "Canceled"
        else
          getDirectoryContents "." >>= (\content_list ->
          mapM_ removeFileStr (filter (\s -> L.isInfixOf name s)  content_list)))
        
removeFileStr :: String -> IO ()
removeFileStr name = do
                     removeFile name
                     putStrLn $ "Removing file: " ++ name
