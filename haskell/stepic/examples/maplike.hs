import Prelude hiding (lookup)
import qualified Data.List as L

class MapLike m where
    empty :: m k v
    lookup :: Ord k => k -> m k v -> Maybe v
    insert :: Ord k => k -> v -> m k v -> m k v
    delete :: Ord k => k -> m k v -> m k v
    fromList :: Ord k => [(k,v)] -> m k v
    fromList [] = empty
    fromList ((k,v):xs) = insert k v (fromList xs)

newtype ListMap k v = ListMap { getListMap :: [(k,v)] }
    deriving (Eq,Show)

instance MapLike ListMap where
    empty = ListMap{getListMap=[]}
    lookup k table = search k (getListMap table)
                where search k [] = Nothing
                      search k ((x,y):xs) | k == x = Just y
                                          | otherwise = search k xs
    insert k v table = let
				insertValue k v [] = [(k,v)]
				insertValue k v ((x,y):xs) | k == x = ((k,v):xs)
							   | otherwise = (x,y):(insertValue k v xs)
			in table{getListMap=(insertValue k v (getListMap table))}
    delete k table = let
   			deleteKey k [] = []
   			deleteKey k ((x,y):xs) | k == x = xs
                        		       | otherwise = ((x,y):(deleteKey k xs))
			in table{getListMap=(deleteKey k (getListMap table))}

