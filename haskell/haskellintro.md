-- comment

{--
	multilyne comment
--}

how to define your function?

```hs
sqr x = x*x
```

```hs
fact 1 = 1
fact n = fact (n-1) * n
```

condition operator:

```hs
abs x = if x > 0
	then x
	else -x
```

you can define your own operators:

```hs
i @@ j = i*i + j*j
```

example call:

```hs
5 @@ 7
```

you can use arbitrary sequence of symbols except some reserved:
for example, `->` and `::`
