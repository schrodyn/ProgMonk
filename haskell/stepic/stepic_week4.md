```hs
data Bool = True | False -- конструктор типа
```

тип данных и его конструкторы задаются всегда с заглавной буквы

```hs
data B = T | F

not' :: B -> B
not' T = F
not' F = T
```

если попытаемся вызвать в интерпретаторе not' F, то выйдет ошибка

```hs
data B = T | F deriving Show

data B = T | F deriving (Show,Eq,Read,Enum)
```

если удалим `not' F = T` в определении функции, то функция станет частичной
чтобы компилятор предупреждал нас об этом:

```
:set -fwarn-incomplete-patterns
```

```hs
data Color = Red | Green | Blue

instance Show Color where
    show Red = "Red"
    show Green = "Green"
    show Blue = "Blue"
```

```
GHCi> show Red
"Red"
```

```hs
intToChar :: Int -> Char
intToChar 0 = '0'
intToChar 1 = '1'
intToChar 2 = '2'
intToChar 3 = '3'
intToChar 4 = '4'
intToChar 5 = '5'
intToChar 6 = '6'
intToChar 7 = '7'
intToChar 8 = '8'
intToChar 9 = '9'
```

это частичная функция

```hs
isz :: Char -> Bool
isz 'z' = True
isz _ = False

stringToBool :: String -> String
stringToBool "true" = True
stringToBool "false" = False

foo 1 2 = 3
foo 0 _ = 5

foo 0 undefined == 5

foo undefined 0 ===>> Exception
foo 2 2 ===>> Exception: Non-exhaustive patterns

foo 1 (5-3) ~> foo 1 2 -- форсирование вычислений
foo 1 (5-3) == 3


data LogLevel = Error | Warning | Info

cmp :: LogLevel -> LogLevel -> Ordering
cmp Error Warning = GT
cmp Warning Info = GT
cmp Error Info = GT
cmp Error Error = EQ
cmp Warning Warning = EQ
cmp Info Info = EQ
cmp _ _ = LT
```

Пусть объявлен следующий тип данных:
```hs
data Result = Fail | Success
```

И допустим определен некоторый тип данных SomeData и некоторая функция
```hs
doSomeWork :: SomeData -> (Result,Int)
```

возвращающая результат своей работы и либо код ошибки в случае неудачи, либо 0 в
случае успеха.

Определите функцию processData, которая вызывает doSomeWork и возвращает строку
"Success" в случае ее успешного завершения, либо строку "Fail: N" в случае неудачи
, где N -- код ошибки.

```hs
processData :: SomeData -> String
processData someData = checker ( doSomeWork someData )
            where checker (Success, n) = "Success"
                  checker (Fail, n) = code n

code n = "Fail: "++ ( show n )
```

конструкторы данных могут принимать аргументы
```hs
data Point = Pt Double Double deriving Show

origin :: Point
origin = Pt 0.0 0.0

distanceToOrigin :: Point -> Double
distanceToOrigin (Pt x y) = sqrt (x ^ 2 + y ^ 2)
```

Pt -- конструктор данных
Point -- имя типа данных


```hs
data Roots = Roots Double Double | None
	deriving Show

roots :: Double -> Double -> Double -> Roots
roots a b c
  | discr >= 0 = Roots x1 x2
  | otherwise = None
  where
    x1 = helper(-d)
    x2 = helper d
    helper x = (-b + x) / (2 * a)
    d = sqrt discr
    discr = b ^ 2 - 4 * a * c
```

```hs
data Shape = Circle Double | Rectangle Double Double

area :: Shape -> Double
area (Circle r) = pi * r ^ 2
area (Rectangle a b) = a * b
```

```hs
import Data.Ratio
2 % 3 == "2/3"

2 % 3 + 1 % 6 == 5 % 6
```

синтаксис записей

```hs
data Person' = Person' String String Int

firstName' :: Person' -> String
firstName' (Person x _ _) = x

lastName' :: Person' -> String
lastName' (Person' _ y _) = y

age' :: Person' -> Int
age' :: (Person' _ _ z) = z
```

этот подход возможен, но нам самим нужно задавать функции проекции

```hs
data Person = Person { firstName :: String, lastName :: String,
age :: Int } deriving (Show, Eq)
```

фукнция называется меткой поля и абсолютна эквивалентна предыдущим
записям

```
let john = Person "John" "Smith" 33
> age john
33
> lastName john
"Smith"
```

обычно в объектно-ориентированных языках пишется object.method
в haskell недавно ввели новый оператор & (его еще называют "евро",
типа по аналогии к оператору доллару $)

```hs
(&) :: a -> (a -> b) -> b
x & f = f x

john & firstName
```

в ghci 7.6.3 еще не введен

если мы определили метки полей, то можем использовать такой синтаксис:

```hs
let xavier = Person {age = 40, firstName = "Phideaux", lastName = "Xavier"}

let unknownBill = Person{firstName = "Bill"}
```

модификация структуры
```hs
updateAge :: Int -> Person -> Person
updateAge newAge person = person {age = newAge} -- способ изменить только
							конкретное поле
```

эта функция не изменяет структуру, а только возвращает новую

```hs
name :: Person -> String
name person = firstName person ++ " " ++ lastName person

name' :: Person -> String
name' (Person fn ln _) = fn ++ " " ++ ln

name'' :: Person -> String
name'' (Person {lastName = ln, firstName = fn}) = fn ++ " " ++ ln
```

представим, что мы хоти хранить такие типы

```hs
data CoordD = CoordD Double Double
data CoordI = CoordI Int Int
```

большинство операций над этими типами будут одинаковыми, но нам придется их
определять для каждого в отдельности, неудобно

в хаскеле мы можем вводить параметризованные типы
data Coord a = Coord a a

теперь у нас универсальный конструктор
конструктор Coord -- полиморфный

теперь мы видим различия между конструкторами данных (Coord a a) и конструкторами
типа (Coord a)

мы можем писать так

```hs
twice :: a -> [a]
twice x = [x,x]
```

или так
```hs
twice :: a -> [] a
twice x = [x,x]
```

это удобный способ, который пригодится при изучении функторов и монад

```hs
thrice :: a -> (,,) a a a
thrice x = (,,) x x x
```

запись a -> b эквивалентна записи (->) a b

пример функции
```hs
id' :: (->) a a
id' x = x

k :: a -> (b -> a)
k x y = x

k :: (->) a (b -> a) =====> k :: (->) a ((->) b a)
```

есть еще два параметризованных типа Maybe и Either

```hs
data Maybe a = Nothing | Just a
data Either a b = Left a | Right b
```

Разница между конструктором данных (Just a) и конструктором типа (Maybe a)
заключается в том, что при вызове конструктора данных мы передаем туда
некоторое значение произвольного типа, а при вызове конструктора типа
мы передаем туда тип

для чего нужен тип Either ?
Рассмотрим пример

```hs
roots :: Double -> Double -> Double -> Either [Char] (Double, Double)
roots a b c
  | discr >=0 = Right (x1, x2)
  | otherwise = Left "Negative discriminant"
  where
    x1 = helper (-d)
    x2 = helper d
    helper x = (-b + x) / (2 * a)
    d = sqrt discr
```

Реализуйте функцию, которая ищет в строке первое вхождение символа, который
является цифрой, и возвращает Nothing, если в строке нет цифр.

```hs
import Data.Char(isDigit)

findDigit :: [Char] -> Maybe Char
findDigit [] = Nothing
findDigit (x:xs) | isDigit x = Just x
                 | otherwise = findDigit xs

```
```
> :type 'c'
'c' :: Char
> :kind Char
Char :: *
> :k Maybe
Maybe :: * -> *

> :k Maybe Int
Maybe Int :: *
```

kind контролирует конструкторы типов
то есть мы не сможем написать
```
> :k Maybe Int Char

<interactive>:1:1:
    `Maybe' is applied to too many type arguments


> :k Either Int Char
Either Int Char :: *
> :k []
[] :: * -> *
> :k [Int]
[Int] :: *

> :k (,)
(,) :: * -> * -> *
> :k (,,)
(,,) :: * -> * -> * -> *

> :k (->)
(->) :: * -> * -> *

> :k (->) Char Char
(->) Char Char :: *
```

когда kind у выражения равен `*`, то мы можем использовать этот конструктор типа
для построения типов

2 :+ 5 -- комплексное число (2+5i)
инфиксный конструктор данных по соглашению должен начинаться с ":"

```hs
data Complex a = !a :+ !a
data Ratio a = !a :% !a
```

! -- означает форсированные вычисления

```hs
type String = [Char] -- синоним типа

type IntegerList = [Integer]

sumSquares :: IntegerList -> Integer
sumSquares = foldr1 (+) . map (^2)

let xs = [1,2] :: IntegerList

type AssocList k v = [(k,v)] -- параметризованные синонимы типов

newtype IntList = IList [Int] deriving Show
example = IList [1,2]
```

отличается от type тем, что все представители для упаковываемого типа пропадают

```hs
data IntList' = IList' [Int] deriving Show
```

чем отличается newtype от data ?

newtype имеет гарантированно один конструктор

типы, определенные с помощью newtype, более ленивы, чем типы с data

newtype Identity a = Identity {runIdentity :: a}

моноид в хаскеле

```hs
class Monoid a where
  mempty :: a -- нейтральный элемент
  mappend :: a -> a -> a -- операция

  mconcat :: [a] -> a    -- свертка
  mconcat = foldr mappend mempty
```

законы:

```hs
mempty `mappend` x == x
x `mappend` mempty == x
(x `mappend` y) `mappend` z == x `mappend` (y `mappend` z)

instance Monoid [a] where
  mempty = []
  mappend = (++)
```

если мы хотим использовать числа как моноид относительно сложения

```hs
newtype Sum a => Sum {getSum :: a}
  deriving (Eq, Ord, Read, Show, Bounded)

instance Num a => Monoid (Sum a) where
 mempty = Sum 0
 Sum x `mappend` Sum y = Sum (x + y)
```

для умножения аналогично

для Xor:

```hs
newtype Xor = Xor { getXor :: Bool }
    deriving (Eq,Show)

instance Monoid Xor where
    mempty = Xor False
    Xor True `mappend` Xor False = Xor True
    Xor False `mappend` Xor True = Xor True
    _ `mappend` _ = Xor False

```

моноид пары:

```hs
instance (Monoid a, Monoid b) => Monoid(a,b) where
  mempty = (mempty, mempty)
  (x1, x2) `mappend` (y1, y2) =
       		(x1 `mappend` y1, x2 `mappend` y2)
```

Maybe тоже можно превратить в моноид:

```hs
instance Monoid a => Monoid (Maybe a) where
  mempty = Nothing
  Nothing `mappend` m = m
  m `mappend` Nothing = m
  Just m1 `mappend` Just m2 = Just (m1 `mappend` m2)
```

мы можем упаковывать функции в контейнеры любого рода
например, мы можем положить функцию в список
естественно, одного типа

```
[(*2),(+5),(^2)] -- сечения операторов

:t [(*2),(+5),(^2)]
[(*2),(+5),(^2)] :: Num a => [a -> a]
```

что мы можем делать с этим списком?
например, зазиповать

```hs
zipWith ($) [(*2),(+5),(^2)] [1,2,3]
[2,7,9]
```

функции, которые принимают значения некоторого типа и возвращают значения этого
же типа, называются эндоморфизмами

```hs
newtype Endo a = Endo {appEndo :: a -> a}
```

Endo является представителем класса моноид относительно операции композиции
нейтральным элементом служит функция id

```hs
instance Monoid (Endo a) where
  mempty = Endo id
  Endo f `mappend` Endo g = Endo (f . g)

:t map Endo [(*2),(+5),(^2)]
map Endo [(*2),(+5),(^2)] :: Num a => [Endo a]
```
