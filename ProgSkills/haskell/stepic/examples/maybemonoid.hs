class Monoid a where
  mempty :: a -- нейтральный элемент
  mappend :: a -> a -> a -- операция

  mconcat :: [a] -> a    -- свертка
  mconcat = foldr mappend mempty

instance Monoid [a] where
  mempty = []
  mappend = (++)


instance Monoid a => Monoid (Maybe a) where
  mempty = Nothing
  Nothing `mappend` m = m
  m `mappend` Nothing = m
  Just m1 `mappend` Just m2 = Just (m1 `mappend` m2)



newtype Maybe' a = Maybe' { getMaybe :: Maybe a }
    deriving (Eq,Show)

instance Monoid a => Monoid (Maybe' a) where
    mempty = Maybe'{getMaybe= (Just mempty)}
    mappend mempty (Maybe' Nothing) = Maybe' Nothing
    mappend (Maybe' Nothing) mempty = Maybe' Nothing
    mappend (Maybe' x) (Maybe' y) = Maybe' (mappend x y)
