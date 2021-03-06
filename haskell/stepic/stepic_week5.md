```hs
class Functior f where  
  fmap :: (a -> b) -> f a -> f b  -- функция над типами

kind f : * -> *
```

f -- контейнерный тип (список, Maybe,...)

```hs
instance Functor [] where
  fmap = map
```

говорят, что fmap "поднимает вычисления в контейнер"

он принимает некоторую произвольную функцию, а возвращает некоторую функцию
над контейнерами

```hs
fmap :: (a -> b) -> (f a -> f b)
```

то есть мы имеем возможность "поднять вычисления"

```hs
instance Functor Maybe where
  fmap _ Nothing = Nothing
  fmap f (Just a) = Just (f a)


> fmap (*2) Nothing
Nothing
> fmap (*2) (Just 21)
Just 42
```

Задача:

Определите представителя класса Functor для следующего типа данных,
представляющего точку в трёхмерном пространстве:

```hs
data Point3D a = Point3D a a a deriving Show

GHCi> fmap (+ 1) (Point3D 5 6 7)
Point3D 6 7 8
```

```hs
instance Functor Point3D where
    fmap f (Point3D a b c) = Point3D (f a) (f b) (f c)
```

```hs
data Tree a = Leaf a | Branch (Tree a) a (Tree a)
  deriving Show

testTree = Branch (Leaf 2) 3 (Leaf 4)

instance Functor Tree where
  fmap g (Leaf x) 	= Leaf (g x)
  fmap g (Branch l x r) = Branch (fmap g l) (g x) (fmap g r)
```

у функции fmap определен инфиксный синоним `<$>` в модуле Data.Functor
import Data.Functor

```hs
fmap (^2) testTree <==> (^2) <$> testTree

:i <$>
(<$>) :: Functor f => (a -> b) -> f a -> f b
  	-- Defined in `Data.Functor'
infixl 4 <$>

(+5) <$> (^2) <$> testTree
Branch (Leaf 9) 14 (Leaf 21)
```

есть еще оператор `<$`
он как бы "затирает" все элементы в контейнеры на заданное значение
но структура сохраняется

```hs
42 <$ testTree
Branch (Leaf 42) 42 (Leaf 42)
```

главное требование к правильному представителю класса типов функтор:
структура представителя класса функтор не должна меняться

```hs
instance Functor ((,) s) where
  fmap g (x,y) = (x, g y)

:k (,)
(,) :: * -> * -> *
:k (,) Int
(,) Int :: * -> *
```

```
-- (a -> b) -> (,) s a -> (,) s b
или (a -> b) -> (s,a) -> (s,b)
```

первый элемент пары может быть произвольным и наша функция fmap ничего не может
с этим поделать
поэтому мы можем работать только со вторым параметром

```hs
fmap succ (1,'A')
(1,'B')

:k Either
Either :: * -> * -> *

instance Functor (Either e) where
  fmap _ (Left x) = Left x
  fmap g (Right y) = Right (g y)

-- (a -> b) -> Either e a -> Either e b
```

мы опять ничего не можем делать с типом e, так как он можеть быть произвольным
и функция g работает только с элементами типа a (a -> b)

```hs
fmap (+3) $ Right 1
Right 4
fmap (+3) $ Left 1
Left 1

fmap (+3) $ Left "AAA"
Left "AAA"

:k (->)
(->) :: * -> * -> *

:k (->) Int
(->) Int :: * -> *

instance Functor ((->) e) where
  fmap = ?

-- (a -> b) -> f a -> f b
-- (a -> b) -> (->) e a -> (->) e b
-- (a -> b) -> (e -> a) -> e -> b
```

если приглядеться, то можно заметить, что мы принимаем функцию из a -> b,
функцию из e -> a и возвращаем функцию из e -> b

оператор с такими свойствами -- оператор композиции функций

```hs
instance Functor ((->) e) where
  fmap = (.)

:t fmap length tail

-- (a -> b) -> (e -> a) -> (e -> b)
-- length принимает [a] и возвращает Int: ([a] -> Int) -> (e -> [a]) -> (e -> Int)
-- tail принимает [a] и возвращает [a]:
([a] -> Int) -> ([a] -> [a]) -> ([a] -> Int)

fmap length tail "ABC"
2
```

**Законы для функторов**:

  1. `fmap id == id (id на контейнер)`
  2. `fmap (f . g) == fmap f . fmap g`

```hs
instance Functor [] where
  fmap _ [] = []
  fmap g (x:xs) = g x : fmap g xs

(fmap (+1) . fmap (^2)) [1,2,3]
[2,5,10]
fmap ((+1) . (^2)) [1,2,3]
[2,5,10]
```

если законы функторов выполняются, то функция fmap гарантированно не меняет
структуру контейнера

если ваш класс является представителем класса функтор, то это означает, что ваш
fmap должен удовлетворять всем законам, иначе другие функции, основанные на этих
законах, не будут корректно работать на нашем типе

haskell сам не умеет проверять выполнение этих законов

самый общий вид функции в хаскеле

```hs
f :: a -> b
```

##ФУНКЦИИ С ЭФФЕКТАМИ

```hs
f :: a -> Maybe b	-- иногда могут завершиться неудачей
f :: a -> [b] 		-- могут возвращать много результатов
f :: a -> (Either s) b	-- иногда могут завершиться типизированным исключением
			тип s -- типизированное исключение
f :: a -> (s,b)		-- могут делать записи в лог
f :: a -> ((->) e) b	-- могут читать из внешнего окружения
f :: a -> (State s) b	-- работают с изменяемым состоянием
f :: a -> IO b		-- осуществляют ввод/вывод (файлы, консоль)
```

Обобщая, получаем *стрелку Клейсли*:

```hs
f :: a -> m b
```

это просто обертка над обычной функцией, которая превращает ее в функцию с
эффектами

```hs
class Monad m where (m -- однопараметрический конструктор типа)
  return :: a -> m a -- берет значение и каким-то тривиальным образом упаковывает
			в контейнер
  (>>=) :: m a -> (a -> m b) -> m b -- произносят bind, также называется
					оператором монадического связывания

  ...
infixl 1 >>=
```


```hs
toKleisli :: Monad m => (a -> b) -> (a -> m b)
toKleisli f = \x -> return (f x)

:t toKleisli cos
toKleisli cos :: (Floating a, Monad m) => a -> m a

toKleisli cos 0 :: [Double]
[1.0]

toKleisli cos 0 :: Maybe Double
Just 1.0

toKleisli cos 0 :: IO Double
1.0

infixr 0 $
($) :: (a -> b) -> a -> b
f $ x = f x

infixl 1 &
(&) :: a -> (a -> b) -> b
x & f = f x

(+1) $ (*3) $ (+2) $ 5
22

5 & (+2) & (*3) & (+1)
22
```

оператор `(>>=)` очень похож на оператор `(&)`

```hs
:t fmap
fmap :: Functor f => (a -> b) -> f a -> f b
:t flip fmap
flip fmap :: Functor f => f a -> (a -> b) -> f b

flip fmap тоже очень похожа на (>>=)
```

```hs
class Monad m where
  return :: a -> m a

  (>>=) :: m a -> (a -> m b) -> m b

  (>>) :: m a -> m b -> m b
  x >> y = x >>= \_ -> y

  fail :: String -> m a
  fail s = error s

(=<<) :: Monad m => (a -> m b) -> m a -> m b
(=<<) = flip (>>=)

(<=<) :: Monad m => (b -> m c) -> (a -> m b) -> (a -> m c) -- оператор рыбки
f <=< g = \x -> g x >>= f  -- оператор композиции стрелок Клейсли

(<=<) f g x = g x >>= f
```

##монада Identity

```hs
newtype Identity a = Identity { runIdentity :: a }
  deriving (Eq,Show)

{-

class Monad m where
  return :: a -> m a
  (>>=) :: m a -> (a -> m b) -> m b

-}
```

для этого нам нужно реализовать две функции: return и bind

```hs
instance Monad Identity where
  return x = Identity x
  Identity x >>= k = k x
```

Identity -- просто коробочка для типа

```hs
return x = Identity x -- тривиальная стрелка Клейсли
```

пример не тривиальной стрелки Клейсли:
```hs
wrap'n'succ :: Integer
wrap'n'succ x = Identity (succ x)

> runIdentity (wrap'n'succ 3)
4
> wrap'n'succ 3
Identity {runIdentity = 4}
```

наша цель -- спрятать монадические вычисления

```
> runIdentity $ wrap'n'succ 3 >>= wrap'n'succ
5
можем еще прицепить("сбиндить") монадическое вычисление
> runIdentity $ wrap'n'succ 3 >>= wrap'n'succ >>= wrap'n'succ

>>= очень поход на оператор амперсант (&)
let (&) = flip ($)

> 3 & succ & succ & succ
6

> runIdentity $ return 3 >>= wrap'n'succ >>= wrap'n'succ >>= wrap'n'succ
```

Если некоторый тип является представителем класса Monad, то его можно сделать
представителем класса Functor, используя функцию return и оператор >>=.

При чём, это можно сделать даже не зная, как данный тип устроен.
Пусть вам дан тип

```hs
data SomeType a = ...
```

и он является представителем класса Monad. Сделайте его представителем класса
Functor.

Решение:
```hs
instance Functor SomeType where
    fmap f x = x >>= (\x -> return (f x))
```


первый закон монад
```hs
return a >>= k === k a -- k -- стрелка Клейсли
```

второй закон монад
```hs
m >>= return === m  --аналог правого нейтрального элемента
```

третий закон
```hs
(m >>= k) >>= k' === m >>= (\x -> k x >>= k') -- аналог ассоциативности
```

скобки необязательны


```hs
goWrap0 =
  wrap'n'succ 3 >>=
  wrap'n'succ >>=
  wrap'n'succ >>=
  return

goWrap1 =
  wrap'n'succ 3 >>= (\x ->
  wrap'n'succ x >>= (\y ->
  wrap'n'succ y >>= (\z ->
  return z)))

> runIdentity goWrap0
6
> runIdentity goWrap1
6
```

область видимости x простирается во всем последующем теле функции
(то есть до закрывающей скобки)

```hs
goWrap2 =
  wrap'n'succ 3 >>= (\x ->    -- x := succ 3;
  wrap'n'succ x >>= (\y ->    -- y := succ x;
  wrap'n'succ y >>= (\z ->    -- z := succ y;
  return (x,y,z))))           -- return (x,y,z)
```

опа, да мы же изобрели императивное программирование :)
таким образом, мы видим, что при помощи монад мы можем писать
программы в императивном стиле

более того, наш оператор монадического связывания >>= он в принципе может
рассматриваться как перегруженная версия оператора ;

```hs
goWrap3 =
  wrap'n'succ 3 >>= \x ->
  wrap'n'succ x >>= \y ->
  wrap'n'succ y >>
  return (x+y)
```


`>>` -- облегченная версия оператора связывания
он просто ничего не передает в следующее вычисление


то есть мы просто запускаем инструкцию wrap'n'succ y (выполняем некоторую
императивную инструкцию) и говорим: "А дальше верни сумму x и y"
то есть мы не хотим связывать результат wrap'n'succ y ни с какой переменной

```
> runIdentity goWrap3
9
```

это все круто, но мы ленивые, чтобы писать постоянно стрелочки :)
поэтому есть синтаксический сахар для монадического связывания -- do-нотация

```hs
do { e1 ; e2 } == e1 >> e2
do { p <- e1; e2 } == e1 >>= \p -> e2 -- p связывается со значением e1
do { let v = e1; e2 } == let v = e1 in do e2 -- в рамках монадических вычислений
```

мы можем иметь желание выполнить не ионадическое вычисление, и связать его
значение с переменной (v) и после этого использовать эту переменную
где-то внутри монадического вычисления (e2)

по сути это та же инструкция присваивания
только `p <- e1` -- это присваивание внутри монадического вычисления
а `let v = e1` -- простое присваивание

```hs
goWrap4 =          -- здесь просто i связывается c значением 3
  let i = 3 in
  wrap'n'succ i >>= \x ->
  wrap'n'succ x >>= \y ->
  wrap'n'succ y >>
  return (i,x+y)

> runIdentity goWrap4
(3,9)

goWrap5 = do  -- то же самое с использованием do-нотации
  let i = 3
  x <- wrap'n'succ i
  y <- wrap'n'succ x
  wrap'n'succ y
  return (i,x+y)

(тут вместо ; используются одинаковые отступы)
:t goWrap5
goWrap5 :: Identity (Integer, Integer)
```

бывает ситуация, когда сопоставление с образцом происходит неудачно
пусть мы имеем

```hs
do {p <- e1; e2 } == e1 >>= \(Just x) -> e2
```

если нам пришло значение Just, то все ок, однако если пришло значение Nothing
то тогда возникает ситуация неполного сопоставления с образцом

в монаде этот момент организован следующим образом
если p это образец `(\p -> e2)` и сопоставление с ним произошло неудачно
то в этом случае вызывается функция fail, одна из четырех функций, определенных
в монаде

по дефолту она вызывает error, но мы можем ее перегрузить

##монада Maybe

```hs
data Maybe a = Nothing | Just a
  deriving (Eq, Ord)

{-

class Monad m where
  return :: a -> m a
  (>>=) :: m a -> (a -> m b) -> m b
  (>>) :: m a -> m b -> m b
  fail :: String -> m a

-}
```

Nothing -- "произошла ошибка"

первые две функции (return и bind) -- необходимые

```hs
instance Monad Maybe where
  return x = Just x  -- можем даже просто написать return = Just

  (Just x) >>= k = k x
  Nothing >>= _  = Nothing

  (Just _) >> m = m
  Nothing >> _  = Nothing
```

значение оператора `>>` состоит в следующем: выполни эффект, который имеет
монада, которая стоит слева, однако значение проигнорируй

 тип Nothing подходит для любой монады Maybe, он универсален для всех возможных
 типов данных, которые в ней лежат

```hs
  fail _ = Nothing
```

фукнция fail вызывается при неудачном сопоставлении с образцом при связывании
в монадических вычислениях

база данных
```hs
type Name= String
type DataBase = [(Name, Name)]

fathers, mothers :: DataBase
fathers = [	("Bill","John"),
		("Ann","John"),
		("John","Piter")]

mothers = [	("Bill","Jane"),
		("Ann","Jane"),
		("John","Alice"),
		("Jane","Dorothy"),
		("Alice","Mary")]


getM, getF :: Name -> Maybe Name
getM person = lookup person mothers
getF person = lookup person fathers
```
```
> :t lookup
```

`lookup :: Eq a => a -> [(a, b)] -> Maybe b` -- можем сделать стрелку Клейсли
то есть мы передаем ей person, а она возвращает его отца или мать, обернутую
в монаде Maybe

getM и getF -- представляют собой стрелки Клейсли в нашей монаде

теперь пусть мы хотим получить прабабушку Билла по отцовской линии:
мы вызываем

```hs
> getF "Bill" >>= getM >>= getM
Just "Marry"
```

в do-нотации можно записать так
```hs
> do {f <- getF "Bill"; gm <- getM f; getM gm}
Just "Marry"
```

Ищем бабушек
```hs
granmas :: Name -> Maybe (Name, Name)
granams person do
  m   <- getM person
  gmm <- getM m
  f   <- getF person
  gmf <- getM f
  return (gmm, gmf)

> granmas "Ann"
Just ("Dorothy","Alice")
> granmas "John" -- несмотря на то, что у Джона есть одна бабушка
Nothing
```

как только произошло возникновение значения Nothing, так все дальнейшие действия
игнорируются

##монада список

монада списка рассматривается как вычисление со следующим эффектом:
у вычисления может быть много значений
(еще иногда говорят в этом случае про недетерминированное вычисление)

давайте посмотрим, как монада реализована в стандартной библиотеке

```hs
{-
class Monad m where
  return :: a -> m a
  (>>=) :: m a -> (a -> m b) -> m b
  (>>) :: m a -> m b -> m b
  fail :: String -> m a
-}

> return 4 :: [Int]
[4]

> [1,2] >>= (\x -> [x,x,x])
[1,1,1,2,2,2]

> [1,2] >>= (\x -> [])
[]

map (\x -> [x,x,x]) [1,2]
[[1,1,1],[2,2,2]]

> concat $ map (\x -> [x,x,x]) [1,2]
[1,1,1,2,2,2]

instance Monad [] where
  return x = [x]
  xs >>= k = concat (map k xs)
  fail _ = []


list = [(x,y) | x <- [1,2,3], y <- [4,5,6]] -- list comprehension

list' = do
  x <- [1,2,3]
  y <- [4,5,6]
  return (x,y)
```

вычисления на каждом шаге "растраиваются" `(*3)`
`x <- [1,2,3]` ==> вычисления для x=1, вычисления для x=2, вычисления для x=3
и так далее
как бы у нас растет дерево вычислений
а в конце все результаты вычислений конкатенируются

более того, list comprehension при трансляции в хаскеле транслируется ровно в
конструкцию list'

а точнее даже сразу в bind-нотацию

```hs
list'' =
  [1,2,3] >>= (\x ->
  [4,5,6] >>= (\y ->
  return (x,y)))
```

то есть что list, что list' оба транслируются в list''

```hs
lst = [(x,y) | x <- [1,2,3], y <- [1,2], x /= y]
```

транслируется в

```hs
lst' = do
  x <- [1,2,3]
  y <- [1,2]
  True <- return (x /= y)
  return (x,y)
```

если сопоставление с образцом неудачное, то в монаде вызывается функция fail,
а для списков эта функция перегружена и возвращает пустой список []

а дальше выполняется вот такой код

```hs
lst'' =
  [1,2,3]	 >>= (\x ->
  [1,2]		 >>= (\y ->
  return (x /= y)>>= (\b ->
  case b of True -> return (x,y)
	    _    -> fail "...")))
```

```hs
lst''' = do                   --- то же самое
  x <- [1,2,3]
  y <- [1,2]
  if x /= y then "Z" else [] -- если возвращается [], то ветка "засыхает"
  return (x,y)
```

##монада IO

ввод-вывод в хаскеле осуществляется в специальном контейнере IO, который является
монадой

```hs
module Main where

main = do
  putStrLn "What is your name?"
  name <- getLine
  putStrLn $ "Nice to meet you, " ++ name ++ "!"
```

мы, во-первых, видим, что вычисления являются монадическими, потому что у нас
используется do-нотация

main -- точка входа в нашу программу (можешь посмотреть putIO.hs)

```hs
> :t main
main :: IO ()
```

() -- это unit type, это тип, который содержит только единственный элемент ()
он может рассматриваться как пустой кортеж

почему тип main -- IO () ?
это следует из последней инструкции нашей программы

```hs
> :t putStrLn
putStrLn :: String -> IO ()
```

-- это стрелка Клейсли в монаде IO

```hs
> :t getLine
getLine :: IO String
```

в `name <- getLine` происходит вынимание из монады ( как мы помним, символ <-
вынимает из монады некоторое значение)

в хаскеле любой ввод-вывод может осуществляться только в рамках монады IO

почему в хаскеле весь ввод-вывод реализован через монаду IO ?
давайте посмотрим почему метод работы с простыми функциями не очень хорош

предположим, что у нас есть функция getCharFromConsole :: Char, которая
считывает символ с консоли, имеет следующий тип

```hs
getCharFromConsole :: Char
```
в принципе это подходит, мы хотим считать Char и получаем Char, но мы знаем
что в хаскеле такого рода функции, которые не принимают никаких аргументов,
она обязательно должна быть константной

таким образом такая сигнатура функции -- плохая

```hs
getCharFromConsole :: RealWorld -> (RealWorld, Char)
```

тип IO внутри ghc реализован следующим образом (неточное описание, потому что
оно скрыто в деталях реализации компилятора)

мы знаем только его kind

```hs
> :k IO
IO :: * -> *

newtype IO a = IO (RealWorld -> (RealWorld, a))
```

давайте реализуем монаду IO

только, к сожалению, тип RealWorld нам не доступен

```hs
type IO a = RealWorld -> (RealWorld, a)

return :: a -> IO a ====>
return :: a -> RealWorld -> (RealWorld, a)

(>>=) :: IO a -> (a -> IO b) -> IO b ====>
(>>=) :: (RealWorld -> (RealWorld, a))
	-> (a -> RealWorld -> (RealWorld, b))
	-> RealWorld -> (RealWorld, b)

instance Monad IO where
  return a = \w -> (w,a) -- "мир" не меняется

  (>>=) m k = \w -> case m w of (w', a) -> k a w'
```

давайте реализуем getLine через getChar

```hs
> :t getChar
getChar :: IO Char

getLine' :: IO String
getLine' = do
  c <- getChar
  if c = '\n' then
    return []
  else do
    cs <- getLine'
    return (c:cs)
```

теперь реализуем putStr через putChar

```hs
> :t putChar
putChar :: Char -> IO ()

putStr' :: String -> IO ()
putStr' [] = return ()
putStr' (x:xs) = putChar x >> putStr' xs -- слабый оператор монадического связывания
```

многие функции для работы с монадами определены в модуле Control.Monad

```hs
sequence_ :: Monad m => [m a] -> m ()
sequence_ = foldr (>>) (return ())
```

цель функции sequnce -- выполнить все монадические действия в списке, связав их
при помощи слабого оператора связывания, проигнорировав значения

```hs
> sequence_ [Just 1, Just 2]
Just ()
```

возвращается unit type, упакованный в монаду

```hs
> sequence_ [Just 1, Nothing]
Nothing
```

если хотя бы один Nothing, то результат -- Nothing

```hs
> sequence_ [[1,2], [3,4,5,6]]
[(),(),(),(),(),(),(),()]
```

-- эффект монады списка заключается в том, что наши вычисления становятся
неоднозначными, в итоге получается 8 цепочек вычислений, каждый из которых
возвращает unit type

```hs
> sequence_ [putChar 'a', putChar 'b']
ab
```

а можно так

```hs
> sequence_ $ map putChar ['a', 'b']
ab
```
таким образом, мы можем реализовать функцию putStr следующим образом

```hs
putStr'' :: String -> IO ()
putStr'' = sequence_ . map putChar
```

еще есть такая функция `mapM_`
```hs
mapM_ :: Monad m => (a -> m b) -> [a] -> m ()
mapM_ f = sequence_ . map f

> mapM_ (\x -> [x,x]) "ABC"
[(),(),(),(),(),(),(),()]
```

но почему получилось 8 цепочек?

```hs
map (\x -> [x,x]) "ABC"
["AA","BB","CC"] -- список, состоящий из трех списков, а дальше вызывается функция sequence_
```

на каждом шаге вычисления раздваиваются
в итоге получается 8 вычислений

```hs
putStr''' :: String -> IO ()
putStr''' = mapM_ putChar

> sequence [Just 1, Just 2]
Just [1,2]
> sequence [Just 1, Nothing]
Nothing

> sequence [getLine, getLine]
ab
cde
["ab","cde"]

> mapM_ putChar "Hello"
Hello

> mapM putChar "Hello"
Hello[(),(),(),(),()]
```

`mapM_` игнорирует значение,
иногда нас интересуют только эффекты, тогда мы должны пользоваться версиями с _
иногда нас интересуют значения, тогда мы должны пользоваться версиями без
подчеркивания
