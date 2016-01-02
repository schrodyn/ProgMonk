ghci

pi -- constant in haskell

we can change command prompt with:
```
:set prompt "GHCi> "
```

to declare module we write:
```
module Test where

... here source code ...

:l module -- load module

:reload module -- to reload module
or :r	module
```

```hs
acos (cos pi)
```

max 5 42 <==> (max 5) 42
(max 5) -- частичное применение функции

to declare function in ghci:
```hs
let sumSquares x y = x ^ 2 + y ^ 2
```

любая функция, не принимающая аргумент --- константная функция

в функциональных языках всегда должны присутствовать then и else

```
max 6 7 -- prefix <==> 6 `max` 7 --infix
6 + 7 -- infix <==> (+) 6 7 -- prefix
```

прииритет операций в Haskell представляет число от 0 до 9
функция имеет приоритет 10

любой оператор имеет приоритет меньше, чем применение функций
чтобы задать ассоциативность оператора в Haskell нужно:
для левоассоциативных, то есть, например, 3 - 9 - 5 = (3 - 9) - 5

```
infixl
```

для правоассоциативных -- infixr \
далее указываем приоритет оператора и сам оператор

infix -- отсутствие ассоциативности, например, ==, /=, >, >=, <, <=
то есть они не могут участвовать в длинной цепочке

^ -- правоассоциативен

определение своих операторов

```hs
module Demo where
```

```hs
infixl 6 *+*
a *+* b = a ^ 2 + b ^ 2
```

`(2 /)` -- сечение оператора

```hs
(2 /) 4

(/ 2) 4
```

```hs
f $ x = f x
```

оператор $ уже определен в стандартной библиотеке

$ -- нулевой приоритет
поэтому его можно использовать для избавления от избыточных скобок

```
sin (pi / 2) <==> sin $ pi / 2
```

мы как бы "снижаем приоритет" пробела

$ -- правоассоциативный оператор, именно поэтому

```
f (g x (h y)) == f $ g x $ h y
```

:type -- чтобы опреределить тип
:type 'c'
тип Char и Bool

:type True

можно писать :t
:t 3
3 :: Num a => a -- контекст, 3 должна быть типа Num

```hs
let x = 3 :: Int
let y = 3 :: Double
```

:t 3.5 -- полиморфный тип
```hs
3.5 :: Fractional a => a
```
тип Fractional: Double и Float
тип Num: Int и Integer
Integer -- неограниченная длина, можно производить любые вычисления

```hs
not False == True
:t not
not :: Bool -> Bool

(&&) False True
(&&) :: Bool -> Bool -> Bool
```

оператор -> -- правоассоциативный, поэтому
```hs
Bool -> Bool -> Bool <==> Bool -> (Bool -> Bool)
```

чтобы заимпортить надо написать
```hs
import Data.Char
```

```hs
test = isDigit '7'
```

```hs
import Data.Complex
```
кортеж -- упорядоченный набор элементов фиксированной длины
тип элементов может быть произвольным
(2, True) -- двухэлементный
(2, True, 'c')

```hs
fst (2, True) = 2
snd (2, True) = True

:t ('x', True)
('x', True) :: (Char, Bool)
```

единичного кортежа нет
но есть пустой кортеж

списки -- гомогенные, то есть все элементы одного типа
длина списка не фиксирована и не известна компилятору

```hs
[1,2,3]
[False, True]
:t [False, True]
[False, True] :: [Bool]
```

тип не зависит от длины списка

```hs
:t "Hi"
"Hi" :: [Char]
```

```hs
let str = 'H': "ello"
```

(++) -- оператор конкатенации двух списков

:info (++) -- узнать инфу об операторе в GHCi

error "ABC" -- выводит строку в диагностический поток
undefined -- выводит стандартное сообщение

часто используется в качестве заглушки для недописанных частей программы
при этом компиляция пройдет, так как undefined возвращает тип "bottom"

```hs
guards:
factorial''' n 	| n < 0 = error "arg must be >= 0"
		| n > 0 = n * factorial''' (n - 1)


factorial4 n	| n == 0  = 1
		| n > 0   = n * factorial4 (n - 1)
		| otherwise = error "arg must be >= 0"
```

база рекурсии -- терминирующее условие

GHCi позволяет отслеживать использование памяти и затраты времени на вычисление
выражения, для этого следует выполнить команду :set +s:
```
*Main> :set +s
*Main> fibonacci 30
832040
(4.06 secs, 936953200 bytes)
```

Tab == 8 пробелам в независимости от настроек текстового редактора

если отступ становится нулевым, это означает, что начинается новое глобальное
объявление

конструкция let in

```hs
roots' a b c =
  let d = sqrt (b ^ 2 - 4 * a * c) in
  ((-b - d) / (2 * a), (-b + d) / (2 * a))
```

можно задавать три связывания:

```hs
roots'' a b c =
  let {d = sqrt(b ^ 2 - 4* a * c); x1 = (-b - d)/ (2 * a); x2 = (-b + d)/ (2 * a)}
  in (x1, x2)

roots''' a b c =
  let
	x1 = (-b - d) / aTwice
	x2 = (-b + d) / aTwice
	d = sqrt $ b ^ 2 - 4 * a * c
	aTwice = 2 * a
  in (x1, x2)
```

локальные функции

```hs
factorial6 n
	| n >= 0 = let
		helper acc 0 = acc
		helper acc n = helper (acc * n) (n - 1)
	  in helper 1 n
	| otherwise = error "arg must be >= 0"
```

отступы должны быть одинаковыми

связываем несколько элементов пары

```hs
rootsDiff a b c = let
	(x1, x2) = roots a b c
	in x2 - x1
```

конструкция where

```hs
roots'''' a b c = (x1, x2) where
	x1 = (-b - d) / aTwice
	x2 = (-b + d) / aTwice
	d = sqrt $ b ^ 2 - 4 * a * c
	aTwice = 2 * a
```

однако let in является выражением, а констукция where выражением не является
то есть можно, например, написать так

```hs
(let x = 2 in x^2)^2
```

но нельзя так

```hs
x^2 where x = 2
```
