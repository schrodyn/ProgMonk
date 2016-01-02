import Prelude hiding (lookup)

class MapLike m where
    empty :: m k v
    lookup :: Ord k => k -> m k v -> Maybe v
    insert :: Ord k => k -> v -> m k v -> m k v
    delete :: Ord k => k -> m k v -> m k v
    fromList :: Ord k => [(k,v)] -> m k v

newtype ArrowMap k v = ArrowMap { getArrowMap :: k -> Maybe v }
instance MapLike ArrowMap where
    empty = ArrowMap{getArrowMap=(\k -> Nothing)}
    lookup k table = (getArrowMap table) k
    insert k v table = table {getArrowMap=arrMap}
                          where  arrMap x | k == x = Just v
                                          | otherwise = (getArrowMap table) x
    delete k table = table {getArrowMap=arrMap}
                          where arrMap x | k == x = Nothing
                                         | otherwise = (getArrowMap table) x
    fromList xs = fromList' xs empty
                    where fromList' [] arrowMap = arrowMap
                          fromList' ((k,v):xs) arrowMap = fromList' xs (insert k v arrowMap)
