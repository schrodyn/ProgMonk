data Coord a = Coord a !a

getX :: Coord a -> a
getX (Coord x _) = x

getY :: Coord a -> a
getY (Coord _ y) = y


