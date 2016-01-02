import Data.Monoid
newtype Writer w a = Writer{runWriter :: (a, w)}

writer :: (a, w) -> Writer w a
writer = Writer


instance (Monoid w) => Monad (Writer w) where
  return x = Writer (x, mempty) -- mempty для моноида
  m >>= k =
    let (x,u) = runWriter m
        (y,v) = runWriter $ k x
    in Writer (y, u `mappend` v)

tell :: Monoid w => w -> Writer w ()
tell w = writer ((),w)

calc :: (Int -> Int -> Int) -> Int -> Int -> Writer String Int
calc op arg1 arg2 = do
  let res = arg1 `op` arg2
  if abs res < 128 then
    return res
  else do
    tell "overflow"
    return res


