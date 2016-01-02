newtype Maybe' a = Maybe' { getMaybe :: Maybe a }
    deriving (Eq,Show)


instance Monoid a => Monoid (Maybe' a) where
    mempty = Maybe' (Just mempty)
    mappend (Maybe' x) (Maybe' y) = Maybe' (mappend x y)


