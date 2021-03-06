#Подпрограммы

Термином «подпрограмма» будем называть и функции, которые возвращают 
значение, и функции, не возвращающие значение (`void proc(…)`). 
Подпрограммы нужны для достижения одной простой цели — избежать 
дублирования кода. В ассемблере есть две команды для организации работы
подпрограмм.

```
call  метка
```

Используется для вызова подпрограммы, код которой находится по адресу 
`метка`. Принцип работы:

  * Поместить в стек адрес следующей за `call` команды. Этот адрес 
  называется адресом возврата.

  * Передать управление на метку.

Для возврата из подпрограммы используется команда `ret`.

```
ret
ret число
```

Принцип работы:

  * Извлечь из стека новое значение регистра `%eip` (то есть передать 
  управление на команду, расположенную по адресу из стека).

  * Если команде передан операнд число, `%esp` увеличивается на это 
  число. Это необходимо для того, чтобы подпрограмма могла убрать из 
  стека свои параметры.

Существует несколько способов передачи аргументов в подпрограмму.

  * **При помощи регистров**. Перед вызовом подпрограммы вызывающий код
  помещает необходимые данные в регистры. У этого способа есть явный 
  недостаток: число регистров ограничено, соответственно, ограничено и 
  максимальное число передаваемых параметров. Также, если передать 
  параметры почти во всех регистрах, подпрограмма будет вынуждена 
  сохранять их в стек или память, так как ей может не хватить регистров
  для собственной работы. Несомненно, у этого способа есть и 
  преимущество: доступ к регистрам очень быстрый.

  * **При помощи общей области памяти**. Это похоже на глобальные 
  переменные в Си. Современные рекомендации написания кода (а часто и 
  стандарты написания кода в больших проектах) запрещают этот метод. Он
  не поддерживает многопоточное выполнение кода. Он использует 
  глобальные переменные неявным образом — смотря на определение функции
  типа `void func(void)` невозможно сказать, какие глобальные 
  переменные она изменяет и где ожидает свои параметры. Вряд ли у этого
  метода есть преимущества. Не используйте его без крайней 
  необходимости.

  * **При помощи стека**. Это самый популярный способ. Вызывающий код 
  помещает аргументы в стек, а затем вызывает подпрограмму.

Рассмотрим передачу аргументов через стек подробнее. Предположим, нам 
нужно написать подпрограмму, принимающую три аргумента типа `long` 
(4 байта). Код:

```
sub:
		pushl %ebp              /* запоминаем текущее значение 
								регистра %ebp, при этом %esp -= 4 */
		movl  %esp, %ebp        /* записываем текущее положение 
								вершины стека в %ebp              */

		/* пролог закончен, можно начинать работу */

		subl  $8, %esp          /* зарезервировать место для локальных 
								переменных                        */


		movl  8(%ebp),  %eax    /* что-то cделать с параметрами      */
		movl  12(%ebp), %eax
		movl  16(%ebp), %eax


		/* эпилог */

		movl  %ebp, %esp        /* возвращем вершину стека в исходное 
								положение                         */
		popl  %ebp              /* восстанавливаем старое значение 
								%ebp, при этом %esp += 4          */
		ret

main:
		pushl $0x00000010       /* поместить параметры в стек        */
		pushl $0x00000020
		pushl $0x00000030
		call  sub               /* вызвать подпрограмму              */
		addl  $12, %esp
```

С вызовом всё ясно: помещаем аргументы в стек и даём команду `call`. 
А вот как в подпрограмме удобно достать параметры из стека? Вспомним 
про регистр `%ebp`.

Мы сохраняем предыдущее значение регистра `%ebp`, а затем записываем 
в него указатель на текущую вершину стека. Теперь у нас есть указатель
на стек в известном состоянии. Сверху в стек можно помещать сколько 
угодно данных, `%esp` поменяется, но у нас останется доступ к 
параметрам через `%ebp`. Часто эта последовательность команд в начале 
подпрограммы называется «прологом».

```
.                      .
.                      .
.                      .
+----------------------+ 0x0000F040 <-- новое значение %ebp
| старое значение %ebp |
+----------------------+ 0x0000F044 <-- %ebp + 4
|    адрес возврата    |
+----------------------+ 0x0000F048 <-- %ebp + 8
|      0x00000030      |
+----------------------+ 0x0000F04C <-- %ebp + 12
|      0x00000020      |
+----------------------+ 0x0000F050 <-- %ebp + 16
|      0x00000010      |
+----------------------+ 0x0000F054
.                      .
.                      .
.                      .
```

Используя адрес из `%ebp`, мы можем ссылаться на параметры:

```
8(%ebp) = 0x00000030
12(%ebp) = 0x00000020
16(%ebp) = 0x00000010
```

Как видите, если идти от вершины стека в сторону аргументов, то мы 
будем встречать аргументы в обратном порядке по отношению к тому, как 
их туда поместили. Нужно сделать одно из двух: или помещать аргументы 
в обратном порядке (чтобы доставать их в прямом порядке), или 
учитывать обратный порядок аргументов в подпрограмме. В Си принято при
вызове помещать аргументы в обратном порядке. Так как операционная 
система Linux и большинство библиотек для неё написаны именно на Си, 
для обеспечения переносимости и совместимости лучше использовать 
«сишный» способ передачи аргументов и в ваших ассемблерных программах.

Подпрограмме могут понадобится собственные локальные переменные. Их 
принято держать в стеке, так как в этом случае легко обеспечить 
необходимое время жизни локальных переменных: достаточно в конце 
подпрограммы вытолкнуть их из стека. Для того, чтобы зарезервировать 
для них место, мы просто уменьшим содержимое регистра `%esp` на размер
наших переменных. Это действие эквивалентно использованию 
соответствующего количества команд `push`, только быстрее, так как не 
требует записи в память. 

Предположим, что нам нужно 2 переменные типа `long` (4 байта), итого 
2 × 4 = 8 байт. Таким образом, регистр `%esp` нужно уменьшить на 8. 
Теперь стек выглядит так:

```
.                        .
.                        .
.                        .
+------------------------+ 0x0000F038 <-- %ebp - 8
| локальная переменная 2 |
+------------------------+ 0x0000F03C <-- %ebp - 4
| локальная переменная 1 |
+------------------------+ 0x0000F040 <-- %ebp
|  старое значение %ebp  |
+------------------------+ 0x0000F044 <-- %ebp + 4
|     адрес возврата     |
+------------------------+ 0x0000F048 <-- %ebp + 8
|       0x00000030       |
+------------------------+ 0x0000F04C <-- %ebp + 12
|       0x00000020       |
+------------------------+ 0x0000F050 <-- %ebp + 16
|       0x00000010       |
+------------------------+ 0x0000F054
.                        .
.                        .
.                        .
```

Вы не можете делать никаких предположений о содержимом локальных 
переменных. Никто их для вас не инициализировал нулём. Можете для себя
считать, что там находятся случайные значения.

При возврате из процедуры мы восстанавливаем старое значение `%ebp` из
стека, потому что после возврата вызывающая функция вряд ли будет рада
найти в регистре `%ebp` неизвестно что (а если серьёзно, этого требует
ABI).

Для этого необходимо, чтобы старое значение `%ebp` было на вершине 
стека. Если подпрограмма что-то поместила в стек после старого `%ebp`,
она должна это убрать. К счастью, мы не должны считать, сколько байт 
мы поместили, сколько достали и сколько ещё осталось. Мы можем просто поместить значение регистра `%ebp` в регистр `%esp`, и стек станет 
точно таким же, как и после сохранения старого `%ebp` в начале 
подпрограммы. 

После этого команда `ret` возвращает управление вызывающему коду. Эта 
последовательность команд часто называется «эпилогом» подпрограммы.

Остаётся одна маленькая проблема: в стеке всё ещё находятся аргументы 
для подпрограммы. Это можно решить одним из следующих способов:

  * использовать команду `ret` с аргументом;
  * использовать необходимое число раз команду `pop` и выбросить 
  результат;
  * увеличить `%esp` на размер всех помещенных в стек параметров.

В Си используется последний способ. Так как мы поместили в стек 3 
значения типа `long` по 4 байта каждый, мы должны увеличить `%esp` на 
12, что и делает команда `addl` сразу после `call`.

Заметьте, что не всегда обязательно выравнивать стек. Если вы 
вызываете несколько подпрограмм подряд (но не в цикле!), то можно 
разрешить аргументам «накопиться» в стеке, а потом убрать их всех 
одной командой. Если ваша подпрограмма не содержит вызовов других 
подпрограмм в цикле и вы уверены, что оставшиеся аргументы в стеке не 
вызовут проблем переполнения стека, то аргументы можно не убирать 
вообще. Всё равно это сделает команда эпилога, которая восстанавливает
`%esp` из `%ebp`. С другой стороны, если не уверены — лучше уберите 
аргументы, от одной лишней команды программа медленнее не станет.

И последнее: если вы хотите использовать вашу подпрограмму за 
пределами данного файла, не забудьте сделать её **глобальной** с 
помощью директивы `.globl`.

Посмотрим на код, который выводил содержимое регистра `%eax` на экран,
вызывая функцию стандартной библиотеки Си `printf(3)`. Вы его уже 
видели в предыдущих программах, но там он был приведен без объяснений.
Для справки привожу цитату из `man`:

```
PRINTF(3)            Linux Programmer's Manual            PRINTF(3)
 
 NAME
        printf - formatted output conversion
		 
		 SYNOPSIS
		        #include <stdio.h>
				 
				int printf(const char *format, ...);
```

```
.data
printf_format:
		.string "%d\n"
		 
.text
		/* printf(printf_format, %eax); */
		pushl %eax              /* аргумент, подлежащий печати  */
		pushl $printf_format    /* аргумент format  */
		call  printf            /* вызов printf()  */
		addl  $8, %esp          /* выровнять стек  */
```

Обратите внимание на обратный порядок аргументов и очистку стека от 
аргументов.

До этого момента мы обходились общим термином «подпрограмма». Но если 
подпрограмма — функция, она должна как-то передать возвращаемое 
значение. Это принято делать при помощи регистра `%eax`. Перед началом
эпилога функция должна поместить в `%eax` возвращаемое значение.

##Программа: вычисление факториала

Теперь напишем рекурсивную функцию для вычисления факториала. Она основана на следующей формуле: `0! = 1, n! = n * (n-1)!`

```
.data
printf_format:
		.string "%d\n"
		 
.text
/* int factorial(int) */
factorial:
		pushl %ebp
		movl  %esp, %ebp

		/* извлечь аргумент в %eax */
		movl  8(%ebp), %eax

		/* факториал 0 равен 1 */
		cmpl  $0, %eax
		jne   not_zero

		movl  $1, %eax
		jmp   return
not_zero:

		/* следующие 4 строки вычисляют выражение
		%eax = factorial(%eax - 1) */

		decl  %eax
		pushl %eax
		call  factorial
		addl  $4, %esp

		/* извлечь в %ebx аргумент и вычислить %eax = %eax * %ebx */

		movl  8(%ebp), %ebx
		mull  %ebx

		/* результат в паре %edx:%eax, но старшие 32 бита нужно 
		отбросить, так как они не помещаются в int */

return:
		movl  %ebp, %esp
		popl  %ebp
		ret

.globl main
main:
		pushl %ebp
		movl  %esp, %ebp

		pushl $5
		call  factorial

		pushl %eax
		pushl $printf_format
		call  printf

		/* стек можно не выравнивать, это будет сделано
		во время выполнения эпилога */

		movl  $0, %eax                  /* завершить программу */

		movl  %ebp, %esp
		popl  %ebp
		ret
```

Любой программист знает, что если существует очевидное итеративное 
(реализуемое при помощи циклов) решение задачи, то именно ему следует 
отдавать предпочтение перед рекурсивным. Итеративный алгоритм 
нахождения факториала даже проще, чем рекурсивный; он следует из 
определения факториала: 

```
n! = 1*2*...*n
```

Говоря проще, нужно перемножить все числа от 1 до n.

Функция — на то и функция, что её можно заменить, при этом не изменяя 
вызывающий код. Для запуска следующего кода просто замените функцию из
предыдущей программы вот этой новой версией:

```
factorial:
		movl  4(%esp), %ecx
		 
		cmpl  $0, %ecx
		jne   not_zero

		movl  $1, %eax
		ret

not_zero:

		movl  $1, %eax
loop_start:
		mull  %ecx
		loop  loop_start

		ret
```

Что же здесь изменено? Рекурсия переписана в виде цикла. Кадр стека 
больше не нужен, так как в стек ничего не перемещается и другие 
функции не вызываются. Пролог и эпилог поэтому убраны, при этом 
регистр `%ebp` не используется вообще. Но если бы он использовался, 
сначала нужно было бы сохранить его значение, а перед возвратом 
восстановить.

##Системные вызовы

Программа, которая не взаимодействует с внешним миром, вряд ли может 
сделать что-то полезное. Вывести сообщение на экран, прочитать данные 
из файла, установить сетевое соединение — это всё примеры действий, 
которые программа не может совершить без помощи операционной системы. 
В Linux пользовательский интерфейс ядра организован через системные 
вызовы. Системный вызов можно рассматривать как функцию, которую для 
вас выполняет операционная система.

Теперь наша задача состоит в том, чтобы разобраться, как происходит 
системный вызов. Каждый системный вызов имеет свой номер. Все они 
перечислены в файле `/usr/include/asm-i386/unistd.h`.

Системные вызовы считывают свои параметры из регистров. Номер 
системного вызова нужно поместить в регистр `%eax`. Параметры 
помещаются в остальные регистры в таком порядке:

  * первый — в `%ebx`;
  * второй — в `%ecx`;
  * третий — в `%edx`;
  * четвертый — в `%esi`;
  * пятый — в `%edi`;
  * шестой — в `%ebp`.

Таким образом, используя все регистры общего назначения, можно 
передать максимум 6 параметров. Системный вызов производится вызовом 
прерывания `0x80`. Такой способ вызова (с передачей параметров через 
регистры) называется `fastcall`. В других системах (например, *BSD) 
могут применяться другие способы вызова.

Следует отметить, что не следует использовать системные вызовы везде, 
где только можно, без особой необходимости. В разных версиях ядра 
порядок аргументов у некоторых системных вызовов может отличаться, и 
это приводит к ошибкам, которые довольно трудно найти. 

Только если вы пишете маленький участок самого нагруженного кода и 
для вас недопустимы накладные расходы, вносимые вызовом стандартной 
библиотеки Си, — только тогда стоит использовать системные вызовы 
напрямую.
