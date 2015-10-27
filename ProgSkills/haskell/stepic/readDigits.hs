readDigits xs = readDigits' xs [] where
             readDigits' [] ys = ((reverse ys),"")
             readDigits' (x:xs) ys | x >= '0' && x <= '9' = readDigits' xs (x:ys)
                                   | otherwise = ((reverse ys), (x:xs))
