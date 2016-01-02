groupElems :: Eq a => [a] -> [[a]]
groupElems [] = []
groupElems xs = groupElems' xs (head xs) [] [] where
            groupElems' []     m p ls   = reverse (p:ls)
            groupElems' (x:xs) m p ls | x== m = groupElems' xs x (x:p) ls
                                      | otherwise = groupElems' xs x [x] (p:ls)
