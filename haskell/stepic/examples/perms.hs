perms :: Eq a => [a] -> [[a]]
perms [] = [[]]
perms xs = [y:ys | (y,zs) <- select xs, ys <- permutations zs] where
	select [] = []
	select (x:xs) = (x,xs) : [(y,x:ys) | (y,ys) <- select xs]

