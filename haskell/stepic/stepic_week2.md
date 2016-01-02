`let id x = x` -- параметрически полиморфна, потому что на тип x не имеется никаких
ограничений

```hs
:t id
id :: a -> a

:t (id id)
(id id) :: a -> a

let k x y = x
k 42 True == 42
k 42 55 == 42

:t k
k :: t1 -> t -> t1
```

```hs
mono :: Char -> Char -- можем передавать только аргументы типа Char
mono x = x
```

мы можем частично ограничивать полиморфизм
```hs
semiMono :: Char -> a -> Char
semiMono x y = x
```

функция высшего порядка -- которая принимает в качестве аргумента другую функцию
```hs
:t ($)
($) :: (a -> b) -> a -> b

apply2 f x = f (f x)
:t apply2
apply2 :: (t -> t) -> t -> t -- так как тип a == типу b
```

лямбда-выражение (или анонимная функция)
```hs
\x -> 2 * x + 7
```

```hs
lenVec x y = sqrt $ x^2 + y^2
lenVec x = \y -> sqrt $ x^2 + y^2
lenVec = \x -> \y -> sqrt $ x^2 + y^2
lenVec = \x y -> sqrt $ x^2 + y^2
```

```hs
(.) -- оператор композиции функций
:i (.)
(.) :: (b -> c) -> (a -> b) -> a -> c 	-- Defined in `GHC.Base'
infixr 9 .

:t (++)
(++) :: [a] -> [a] -> [a]
```

Мы можем использовать (,) как конструктор кортежей, а элементы передавать как
аргументы функции

```hs
:t (,)
(,) :: a -> b -> (a, b)

let x = (,,) True 3
x 2 == (True,3,2)
```

curry -- прокаррировать функцию

например, функция
```hs
avg :: (Double, Double) -> Double
avg p = (fst p + snd p) / 2
```

не является каррированной, а
```hs
:t curry avg
curry avg :: Fractional c => c -> c -> c
```

уже является
как устроена функция curry?
```hs
curry f x y = f (x,y)

let cur f x y = f (x,y)
:t cur
cur :: ((t1, t2) -> t) -> t1 -> t2 -> t
```

а у curry:
```hs
curry :: ((a, b) -> c) -> a -> b -> c
```

есть и анкаррированная функция uncurry
```hs
:t uncurry
uncurry :: (a -> b -> c) -> (a, b) -> c
```

специальный полиморфизм
```hs
:t 7
7 :: Num a => a
```

Num a => -- на тип a наложено ограничение, а именно он должен реализовывать
интерфейс Num

```hs
:t (>)
(>) :: Ord a => a -> a -> Bool
```

тип a должен быть представителем класса типа Ord

```hs
:t (>7)
(>7) :: (Num a, Ord a) => a -> Bool
```

на a наложено два ограничения

```hs
:t (>(1,2))
(>(1,2)) :: (Num t, Num t1, Ord t, Ord t1) => (t, t1) -> Bool
```

на обе переменные должно быть наложено два ограничения
классы типов
```hs
class Eq where
  (==) : a -> a -> Bool
  (/=) : a -> a -> Bool

:t (==)
(==) :: Eq a => a -> a -> Bool

:t (==42)
(==42) :: (Eq a, Num a) => a -> Bool

:t (=='c')
(=='x') :: Char -> Bool
```

функция elem -- проверяет на наличие данного элемента в списке

```hs
:t elem
elem :: Eq a => a -> [a] -> Bool
```

представитель класса типов
для того, чтобы определить представителя класса типов, используется словоInstance

```hs
class Eq a where
  (==),(/=) :: a -> a -> Bool

instance Eq Bool where
  True == True	 = True
  False == False = True
  _     == _	 = False

  x /= y = not (x == y)
```

мы можем написать в классе типов реализацию по умолчанию
то есть
```hs
class Eq a where
  (==),(/=) :: a -> a -> Bool
  x /= y = not (x == y)
```

например,
Реализуйте класс типов Printable, предоставляющий один метод toString - функцию
одной переменной, которая преобразует значение типа, являющегося представителем
Printable, в строковое представление.

Сделайте типы данных Bool и () представителями этого класса типов, обеспечив
следующее поведение:

```
GHCi> toString True
"true"
GHCi> toString False
"false"
GHCi> toString ()
"unit type"
```

Решение:
```hs
class Printable a where
    toString :: a -> String

instance Printable Bool where
        toString True = "true"
        toString False = "false"

instance Printable () where
        toString () = "unit type"
```

предположим, мы хотим реализовать функцию сравнения пары:

```hs
class Eq a where
  (==), (/=) :: a -> a -> Bool
  x /= y = not (x == y)
  x == y = not (x /= y)

instance Eq Bool where
  True == True	 = True
  False == False = True
  _    == _	 = False

instance (Eq a, Eq b) => Eq (a,b) where
   p1 == p2 = fst p1 == fst p2 && snd p1 == snd p2
```

здесь появляется контекст (Eq a, Eq b) => который накладывает ограничения на a и b
:i (==) -- узнать информацию про оператор
в частности, его приоритет

Class Extension (Расширение классов типов)
схоже с наследованием классов (наследованием интерфейсов):

```hs
class Eq a where
  (==), (/=) :: a -> a -> Bool
  x /= y = not (x == y)
  x == y = not (x /= y)

class (Eq a) => Ord a where
  (<), (<=), (>=), (>) :: a -> a -> Bool
  max, min :: a -> a -> a
  compare :: a -> a -> Ordering
```

тип Ordering устроен довольно просто:
```
:i Ordering
```

```hs
data Ordering = LT | EQ | GT 	-- Defined in `GHC.Types'
instance Bounded Ordering -- Defined in `GHC.Enum'
instance Enum Ordering -- Defined in `GHC.Enum'
instance Eq Ordering -- Defined in `GHC.Classes'
instance Ord Ordering -- Defined in `GHC.Classes'
instance Read Ordering -- Defined in `GHC.Read'
instance Show Ordering -- Defined in `GHC.Show'
```

также возможно множественное наследование
```hs
class (Eq a, Printable a) => MyClass a where
  ...
```

здесь именно наследование интерфейсов, поэтому проблем с множественным
наследованием не возникает

класс Show

```hs
:t show
show :: Show a => a -> String
```

в контексте указан класс типов Show a, который говорит, что для a должен быть
реализован соответсвующий интерфейс

функция read
```hs
:t read
read :: Read a => String -> a
```

она принимает строку и возвращает объект соответствующего типа
однако если написать
read "5"
то интерпретатор вернет сообщение об ошибке
поскольку 5 может быть Int, а может быть и Double

поэтому надо писать так
```hs
read "5" :: Int
```
оператор типизации, он имеет низкий приоритет

чтобы распарсить "5 rings" нужно использовать более клевую функцию
```hs
reads "5 rings" :: [(Int, String)]
[(5," rings")]
```

класс типов Enum

```hs
class Enum a where
  succ, pred :: a -> a
  toEnum :: Int -> a
  fromEnum :: a -> Int

succ 4
5
pred 'z'
'y'
succ 'z'
'{'
```
```hs
fromEnum 'z' -- возвращает ASCII-код
toEnum 122 :: Int
122
toEnum 122 :: Char
'z'

succ False
True
succ True
*** Exception: Prelude.Enum.Bool.succ: bad argument
```
```hs
minBound :: Bool -- возвращает минимальное возможное значение для типа Bool
maxBound :: Bool -- максимальное

minBound :: Int
-9223372036854775808
maxBound :: Int
9223372036854775807

maxBound :: Integer -- ошибка, поскольку этот тип трактуется как бесконечный
{-# LANGUAGE ScopedTypeVariables #-}
```

```hs
Num --> Integral (наследуются) -- целочисленное деление
Num --> Fractional (наследуется) -- вещественное деление
```

quotRem быстрее, чем divMod
```
:i Integral
```

```hs
class (Real a, Enum a) => Integral a where
  quot :: a -> a -> a
  rem :: a -> a -> a
  div :: a -> a -> a
  mod :: a -> a -> a
  quotRem :: a -> a -> (a, a)
  divMod :: a -> a -> (a, a)
  toInteger :: a -> Integer
  	-- Defined in `GHC.Real'
instance Integral Integer -- Defined in `GHC.Real'
instance Integral Int -- Defined in `GHC.Real'
```

```
:i Fractional
```

```hs
class Num a => Fractional a where
  (/) :: a -> a -> a
  recip :: a -> a
  fromRational :: Rational -> a
  	-- Defined in `GHC.Real'
instance Fractional Float -- Defined in `GHC.Float'
instance Fractional Double -- Defined in `GHC.Float'
```

-------------------

```hs
Fractional --> Floating
:i Floating

class Fractional a => Floating a where
  pi :: a
  exp :: a -> a
  sqrt :: a -> a
  log :: a -> a
  (**) :: a -> a -> a
  logBase :: a -> a -> a
  sin :: a -> a
  tan :: a -> a
  cos :: a -> a
  asin :: a -> a
  atan :: a -> a
  acos :: a -> a
  sinh :: a -> a
  tanh :: a -> a
  cosh :: a -> a
  asinh :: a -> a
  atanh :: a -> a
  acosh :: a -> a
  	-- Defined in `GHC.Float'
instance Floating Float -- Defined in `GHC.Float'
instance Floating Double -- Defined in `GHC.Float'
```

----------------------------

```hs
Real a, Fractional a --> RealFrac a
:i RealFrac

class (Real a, Fractional a) => RealFrac a where
  properFraction :: Integral b => a -> (b, a)
  truncate :: Integral b => a -> b
  round :: Integral b => a -> b
  ceiling :: Integral b => a -> b
  floor :: Integral b => a -> b
  	-- Defined in `GHC.Real'
instance RealFrac Float -- Defined in `GHC.Float'
instance RealFrac Double -- Defined in `GHC.Float'

:i RealFloat
```

нестрогая семантика

```hs
sumIt :: Int -> Int -> Int
sumIt x y = x + y
```

если вызовем `sumIt (1+2) 3`, то результат будет 6
это можно достигнуть двумя стратегиями:
энергичная стратегия, используется в императивных языках:
сначала вычисляются все операнды, а затем вызывается функция

```
sumIt (2 + 3) 4
 ~> sumIt 5 4
 ~> 5 + 4
 ~> 9
```

мы можем сначала выполнить подстановку тела этой функции, а потом уже вычисление
эта стратегия называется ленивой

```
sumIt (2+3) 4
 ~> (2 + 3) + 4
 ~> 5 + 4
 ~> 9
```

в хаскеле принята ленивая стратегия
xank -- отложенное вычисление
вычисления происходят до тех пор, пока в выражение не остается редексов

нормальная и ненормальная формы

```
NF:
42
(3,4)
\x -> x + 2
```
нормальная форма -- форма без редексов

не NF
```
"Real " ++ "world"
sin (pi / 2)
(\x -> x + 2) 5
(3, 1+5)
```

слабая заголовочная нормальная форма WHNF

```
\x -> x + 2*3
(3,1+5)
(,4*5)
(+) (7^2)

f $! x = x `seq` f x
```

то есть это оператор применения, но x сначала приводится к слабой нормальной
форме и передается в f как параметр
эта штука позволяет избавиться от thunk-ов (глубоких отложенных вычислений)

его также называют строгим оператором применения

программа Haskel -- набор модулей
каждый модуль -- в отдельном файле
название модуля начинается с большой буквы

```hs
module Demo where
```

можем импортировать модули

```hs
import Data.Char -- в директории Data модуль Char
```
мы можем указать список импорта
```hs
import Data.Char (toUpper)

import Data.Char (toUpper,toLower)
```

можем импортировать все, кроме чего-то:

```hs
import Data.Char hiding (toLower)
```
```hs
import qualified Data.Set -- все функции должны использоваться с префиксом
```
можно назначать имя импортированного модуля:
```hs
import qualified Data.Set as Set
```

ограничить import каких-то функций, мы можем указать, что можно экспортировать
module Test (sumIt) where

```hs
sumIt x y = x + y
const42 = const 42
```

внешний интерфейс модуля

haskell source -> coa language -> stg machine code -> c-- -> asm -> machine code
