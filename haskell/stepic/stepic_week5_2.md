##Монада Reader

эффект монады Reader состоит в том, что в процессе последовательных вычислений
мы можем обращаться в любой момент этих вычислений к некоторому общему для
всех вычислений константному окружению
и оттуда считывать какие-то значения

иногда эту монаду называют Environment

вспомним, как мы реализовали класс типов Functor для (->)

```hs
instance Functor ((->) e) where
  fmap g h = g . h

fmap :: (a -> b) -> f a -> f b
     :: (a -> b) -> (e -> a) -> (e -> b) , то есть у нас функция a -> b
как бы "обогащается" окружением e

> :t fmap (^2) length
fmap (^2) length :: [a] -> Int

> fmap (^2) length [1,2,3]
9
```

давайте перейдем к реализации

```hs
instance Monad ((->) e) where
  -- return :: a -> e -> a
  return x = \_ -> x
  (>>=) :: (e -> a) -> (a -> (e -> b)) -> (e -> b)
  -- (>>=) :: (e -> a) -> (a -> e -> b) -> e -> b то же самое
  m >>= k = \e -> k (m e) e
```

давайте реализуем с помощью этой монады функцию safeHead

```hs
safeHead = do
  b <- null -- проверка на null :t null == null :: [a] -> Bool
  if b
  then return Nothing
  else do
    h <- head
    return $ Just h

:t safeHead
safeHead :: [a] -> Maybe a -- в данном случае [a] -- окружение

safeHead' = do
  e <- id -- функция id, примененная к окружению, возвращает окружение
  if null e
  then return Nothing
  else return $ Just (head e)
```

Не используя интерпретатор, вычислите значение следующего выражения:

```hs
return 2 >>= (+) >>= (*) $ 4
```

Ответ: 24

При работе с монадой Reader, каков смысл оператора (>>)?
В сочетании с монадой Reader этот оператор бесполезен

```hs
> :t (>>)
(>>) :: Monad m => m a -> m b -> m b
```

но нам бы хотелось напрямую обращаться к нашему окружению, поэтому давайте
реализуем другую версию Reader более универсальную и высокоуровневую

мы вводим тип Reader:
```hs
newtype Reader r a = Reader {runReader :: (r -> a)}
```

это двухпараметрический тип:
первый тип r параметризует наше окружение,
второй тип нам нужен просто для того, чтобы хранить результат каждого шага
монадических вычислений

наш новый Reader внутри ничем не отличается от нашей предыдущей версии Reader
для того, чтобы получать доступ к той функции, которая лежит внутри монады
Reader, у нас есть метка runReader, она позволяет нам вынимать функцию из Reader'а

```hs
-- runReader :: Reader r a -> r -> a
```

давайте теперь объявим наш новый тип монадой

```hs
instance Monad (Reader r) where
  return x = Reader $ \e -> x
  m >>= k  = Reader $ \e ->
    let v = runReader m e
    in runReader (k v) e
```

функция return просто оборачивает результат функции из окружения (игнорирующая ее)
в x в конструктор Reader

то же самое происходит при определении оператора монадического связывания
в этой версии Monad Reader написано практически то же самое, что и в прошлой,
только у нас есть сборка и разборка типа Reader с помощью конструктора, когда
делаем сборку, и с помощью функции runReader, которая позволяет нам вынуть
изнутри Reader'а интересующую нас функцию и запустить ее

в прошлой версии Reader мы использовали функцию id, для "вытаскивания" окружения
но это путает читателя монады

давайте сделаем новую функцию именно для этого, которая возвращает значение
окружения


`ask :: Reader r r` -- поскольку у нас применяется функция id, то возвращаемое
`ask = Reader id`      значение функции, упакованное в Reader, должно быть то же, что и аргумент

```hs
runReader ask 42
```

посмотрим на другой пример

```hs
type User = String
type Password = String
type UsersTable = [(User,Password)]

pwds :: UsersTable
pwds = [("Bill","123"),("Ann","qwerty"),("John","2sRq8P")]
```

давайте напишем своего ридера, который возвращает первого пользователя, лежащего
в этой таблице

```hs
firstUser :: Reader UsersTable User
firstUser = do
  e <- ask
  return $ fst (head e)

> runReader firstUser pwds
"Bill"
```

конструктор Reader упаковывает некоторую функцию в монаду Reader

```hs
asks :: (r -> a) -> Reader r a
asks = Reader

firstUserPwd :: Reader UsersTable Password
firstUserPwd = do
  pwd <- asks (snd . head)
  return pwd

> runReader firstUserPwd pwds
"123"
```

можно даже короче

```hs
firstUserPwd :: Reader UsersTable Password
firstUserPwd = asks (snd . head)

usersCount :: Reader UsersTable Int
usersCount = asks length


local :: (r -> r) -> Reader r a -> Reader r a
local f m = Reader $ \e -> runReader m (f e)
```

функция local делает следующее:
она берет и локально модифицирует окружение
мы задаем функцию из окружения в окружение (r -> r)
и передаем туда монаду Reader

что здесь происходит? мы берем монаду Reader m и запускаем ее в окружении (f e)


Вспомним пример с базой пользователей и паролей:

```hs
type User = String
type Password = String
type UsersTable = [(User, Password)]
```

Реализуйте функцию, принимающую в качестве окружения UsersTable и возвращающую
список пользователей, использующих пароль "123456" (в том же порядке, в котором
они перечислены в базе).

```hs
GHCi> runReader usersWithBadPasswords [("user", "123456"), ("x", "hi"),
("root", "123456")]
["user","root"]

usersWithBadPasswords :: Reader UsersTable [User]
usersWithBadPasswords = Reader $ \e -> (map fst $ filter (\(l,p) -> p == "123456") e)
```

##монада Writer

на кажжом шаге вычислений можем записать какие-то значения в некоторо хранилище,
которое зовется лог

в хаскеле лог -- это любой моноид (не только строки)

```hs
import Data.Monoid

newtype Writer w a = Writer{runWriter :: (a, w)}
```

w -- это тип лога
a -- свободный параметр, который будет меняться от одного вычисления к другому

```hs
--runWriter :: Writer w a -> (a, w) -- возвращает результат и лог

writer :: (a, w) -> Writer w a
writer = Writer

execWriter :: Writer w a -> w
execWriter m = snd (runWriter m)  -- если нам нужен только лог после вычисления

instance (Monoid w) => Monad (Writer w) where
  return x = Writer (x, mempty) -- mempty для моноида
  m >>= k =
    let (x,u) = runWriter m
    	(y,v) = runWriter $ k x
    in Writer (y, u `mappend` v)
```

если w -- это String, то mappend это ++

```hs
> runWriter (return 3 :: Writer String Int)
(3,"")
> runWriter (return 3 :: Writer (Sum Int) Int)
(3, Sum {getSum = 0})
> runWriter (return 3 :: Writer (Product Int) Int)
(3, Product {getProduct = 1})

> execWriter (return 3 :: Writer String Int)
""

tell :: Monoid w => w -> Writer w ()
tell w = writer ((),w)
```

функция tell позволяет организовать запись в лог

```hs
calc :: (Int -> Int -> Int) -> Int -> Int -> Writer String Int
calc op arg1 arg2 = do
  let res = arg1 `op` arg2
  if abs res < 128 then
    return res
  else do
    tell "overflow"
    return res

> runWriter $ calc (+) 33 44
(77,"")

> runWriter $ calc (+) 99 44
(143,"overflow")
```

##Задача:

Давайте разработаем программное обеспечение для кассовых аппаратов одного
исландского магазина. Заказчик собирается описывать товары, купленные покупателем,
 с помощью типа Shopping следующим образом:

```hs
type Shopping = Writer (Sum Integer) ()

shopping1 :: Shopping
shopping1 = do
  purchase "Jeans"   19200
  purchase "Water"     180
  purchase "Lettuce"   328
```

Последовательность приобретенных товаров записывается с помощью do-нотации.
Для этого используется функция purchase, которую вам предстоит реализовать.
Эта функция принимает наименование товара, а также его стоимость в исландских
кронах (исландскую крону не принято делить на меньшие единицы, потому используется
 целочисленный тип Integer).
Кроме того, вы должны реализовать функцию total:

```hs
GHCi> total shopping1
19708
```

Решение:

```hs
purchase :: String -> Integer -> Shopping
purchase item cost =   tell Sum{getSum=cost} >>
                       return ()

total :: Shopping -> Integer
total m = getSum $ snd (runWriter m)
```

##монада State

это монада, побочным эффектом которой служит возможность работы с изменяемым
состоянием

по факту это объединение идей монады Reader и монады Writer

```hs
newtype State s a = State {runState :: s -> (a,s) } -- "s ->" - reader;
						    -- "(a,s)"- writer
-- runState :: State s a -> s -> (a,s)
-- принимается текущее состояние и инициализируещее значение
-- результатом служит пара -- это возвращаемое значение a и новое состояние

instance Monad (State s) where
  return a = State $ \st -> (a,st)

  m >>= k = State $ \st ->
    let (a,st') = runState m st
 	m' = k a
    in runState m' st'
```

runState возвращает пару, но иногда нам нужно только состояние или только
результат

```hs
execState :: State s a -> s -> s
execState m s = snd (runState m s)

evalState :: State s a -> s -> a
evalState m s = fst (runState m s)
```

нам нужно каким-то образом получать текущее состояние
и также мы должны уметь перезаписать это самое состояние

```hs
get :: State s s
get = State $ \st -> (st, st)

put :: s -> State s ()
put st = State $ \_ -> ((), st)


> runState get 5
(5,5)

> runState (put 7) 5
((),7)


tick :: State Int Int
tick = do
  n <- get
  put (n+1)
  return n

> runState tick 5
(5,6)
```

можно сделать еще функцию modify, которая модифицирует состояние

```hs
modify :: (s -> s) -> State s ()
modify f = State $ \s -> ((), f s)

> runState (modify (^2)) 5
((),25)
```
