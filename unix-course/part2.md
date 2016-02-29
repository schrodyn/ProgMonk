#Разные команды для работы с текстовыми файлами

Удобной командой для побуквенной транслитерации является команда `tr`. 
Она читает стандартный ввод, и заменяет в нем символы, входящие в ее 
первый параметр, на символы из ее второго параметра с теми же номерами.
Символы, которые не входят в первый параметр, выводятся как есть. Вывод
данной команды всегда поступает на стандартный вывод. В параметрах 
допустимо использовать диапазоны, как в прямом, так и обратном порядке,
а также escape-последовательности языка C.

Пример

```
tr a-z A-Z
```

Имеются две достаточно удобные команды, `head` и `tail`, позволяющие 
оставлять из файла начало и конец соответственно. Причем объем 
оставляемого можно задавать как указанное число строк или байтов.

Для сортировки текстовых файлов используется программа `sort`.

##Соединение нескольких файлов

Важной операцией над базами данных является операция соединения двух 
файлов. Эта операция важна, поскольку хранение одной базы данных в 
нескольких связанных по смыслу файлах (в терминологии баз данных, 
таблицах) позволяет в большинстве случаев избавиться от избыточной, 
т. е. присутствующей в базе неоднократно, информации. Хранение 
избыточной информации опасно, поскольку оно провоцирует ошибки, в 
первую очередь противоречивость данных — например, если обновляется 
только одна запись из многих, хранящих одинаковую информацию.
Поэтому для избежания избыточности набор файлов базы данных 
нормализуется, т. е. последовательно приводится к очередной нормальной 
форме. Всего их пять, и чем больше номер, тем меньше избыточность; 
пятая нормальная форма используется очень редко, поскольку опирается на
весьма нетривиальные и трудно обнаруживаемые зависимости между данными.

Однако, при работе с базами данных иногда возникает необходимость 
собрать обратно одну таблицу из нескольких файлов, в совокупности 
содержащих требуемую информацию. Для этого в Linux предусмотрена 
специальная команда `join`, соединяющая две таблицы в одну.

Команда join имеет следующую командную строку:

```
join options file1 file2
```

Действие этой команды состоит в том, что на стандартный вывод выдаются 
все строки, которые получаются объединением одной строки первого файла 
и одной строки второго; строки можно объединить, если значение 
фиксированного поля одной строки совпадает со значением фиксированного 
поля другой строки. Номера сравниваемых полей в обоих файлах задаются 
опциями. Для правильной работы этой команды оба входных файла должны 
быть отсортированы по значениям этих полей.

Пример.

Создайте текстовый файл `colors.txt`, содержащий следующий текст:

```
apple red
pear green
grape green
plum yellow
orange orange
carrot red
cucumber green
cabbage white
```

и файл `price.txt`, содержащий следующий текст:

```
apple 0.25$
pear 0.20$
grape 0.45$
plum 0.05$
orange 0.50$
carrot 0.10$
cucumber 0.15$
cabbage 0.8$
```

Отсортируйте оба этих файла по первому полю, и соедините обе таблицы в 
одну при помощи команды `join`.

#Поиск

Для поиска имеется утилита `grep`. Она ищет во входном файле строки, 
содержащие текст, соответствующий указанному регулярному выражению, и 
выводит их в стандартный поток вывода.

Кроме того, многие символы из состава регулярных выражений понимаются 
как управляющие командным интерпретатором, и потому вместо описания 
регулярного выражения непредсказуемо меняют команду. Чтобы этого
избежать, в BASH проще всего заключать регулярные выражения в 
апострофы, в подавляющем большинстве случаев этого достаточно. Также 
уместно здесь напомнить, что до запуска команды `grep` можно проверить
, что будет подано ей в качестве командной строки, при помощи команды 
`echo`.

Командная строка при вызове `grep` выглядит как

```
grep опции регулярное-выражение имена-файлов
```

Имена входных файлов могут отсутствовать. Если это так, то входная 
информация читается со стандартного ввода, что позволяет использовать 
`grep` как фильтр. Выходная информация всегда поступает на стандартный
вывод, так что если нужно сохранить ее в файле, нужно перенаправить 
стандартный вывод в файл.

Наиболее важные опции:

  * `-E` означает использование расширенного языка регулярных 
  выражений. Расширенный язык регулярных выражений отличается от 
  обычного (на Linux) только тем, какие символы воспринимаются как 
  управляющие, и какие — как обычные. В любом случае поменять статус 
  символа (превратить управляющий символ в обычный и наоборот) можно,
  поставив перед ним `\`.

  * `-F` означает использование в качестве регулярных выражений только
  строк из обычных символов (подавляет все управляющие символы). Это 
  позволяет иметь очень много строк поиска и работать очень быстро.

  * `-P` означает использование регулярных выражений языка Perl.

  * `-f` имя-файла позволяет читать регулярные выражения из указанного
  файла, по одному на строку. В этом случае ищутся строки, содержащие 
  текст, соответствующий любому из содержащихся в файле регулярных 
  выражений.

  * `-e` регулярное-выражение позволяет задавать регулярные выражения,
  начинающиеся с «-», или иметь несколько регулярных выражений, 
  заданных в командной строке.

  * `-A` число означает вывод указанного числа строк после совпадения.
  Выводит также строку, содержащую «−−» между группами 
  последовательных совпадений.

  * `-B` число аналогично предыдущей опции, но выводит строки до 
  совпадения.

  * `-С` число — выводит указанное число строк контекста (аналогично 
  двум предыдущим).

  * `-v` изменить смысл поиска на противоположный: выводить 
  несоответствующие строки.
  
  * `-w` искать только те соответствия, которые образуют целые слова.

  * `-x` аналогично предыдущей, только не слова, а целые строки.

Пример.

Создайте текстовый файл `f.txt`, содержащий следующий текст:

```
apple
pear
grape
plum
orange
carrot
cucumber
cabbage
```

Что выведет команда `grep p f.txt`? Запустите ее и проверьте 
правильность своей догадки. А команда `grep r f.txt`?
Команда `grep -v r f.txt`?

`ˆ` — начало строки, `$` — конец строки, `<` и `>` — начало и конец 
слова соответственно.

Для того же файла f.txt, что выдаст команда `grep 'ˆc' f.txt`? А 
команда `grep 'e$' f.txt`? В чем отличие этих команд от `grep c f.txt`
и `grep e f.txt`? Зачем в этих командах регулярное выражение 
заключается в апострофы?

Конструкции, соответствующие символу из некоторого набора. В самом 
простейшем варианте — просто набор символов, заключенных в квадратные 
скобки (например, `[abcdef]` соответствует a, b, c, d, e или f). 
Допустимо указывать диапазоны, но результат зависит от выбранного 
языка (локали, поскольку от этого зависит порядок символов). Т. е. 
предыдущий пример можно сократить до `[a-f]`. Также, поставив в начало
такого набора символ `ˆ`, смысл набора меняется на противоположный 
(т. е. такая конструкция соответствует всем символам, не 
присутствующим внутри квадратных скобок). Например, выражение 
`[ˆA-Za-z]` соответствует любому символу, не являющемуся латинской 
буквой. Имеются также удобные конструкции для сокращения такого рода 
регулярных выражений, например `.` соответствует любому одинарному 
символу (кроме символа перевода строки). Также, имеются обозначения 
для наборов символов из наиболее часто встречающихся классов, таких, 
как буквы, цифры, печатные символы и т. д.

Например, конструкция `[[:alnum:]]` соответствует любому 
буквенно-цифровому символу. В этой конструкции внешняя пара `[]` 
указывает на то, что это регулярное выражение соответствует символу из
набора, а `[:alnum:]` — на конкретный набор символов.

Для того же файла `f.txt`, что выдаст команда `grep '[ar]' f.txt`? 
Напишите команду, выдающую те строки из этого файла, которые содержат 
по крайней мере одну букву из набора b, l, m. Напишите еще одну 
команду, которая выдает те строки из `f.txt`, в которых после r стоит 
a или o.

Различные конструкции, определяющие количество повторений регулярного 
выражения. Если есть некоторое регулярное выражение, из него можно 
построить новое, которому будут соответствовать строки, состоящие из 
определенного числа частей, каждая из которых соответствует исходному 
регулярному выражению. 

Например, если `r` — это регулярное выражение, то
регулярному выражению `r?` будет соответствовать пустая строка (0 раз 
`r`) или строка, соответствующая `r` (один раз `r`). 

Аналогично, операция `*` означает ноль или более повторений своего 
параметра- регулярного выражения; `+` — одно и более повторений. Можно
даже явно указывать диапазон требуемого числа повторений при помощи 
операции `{m, n}`, где m и n — границы числа повторений. При 
использовании этой конструкции вполне возможно, что разным повторениям
регулярного выражения будут соответствовать разные строки.
Например, строка `abc` соответствует регулярному выражению `[abc]+` 
(первому повторению регулярного выражения `[abc]` соответствует a, 
второму — b и третьему — с). Приоритет операций, задающих число 
повторений той или иной конструкции, выше, чем у конкатенации, так что
регулярное выражение `ab+` соответствует строкам `ab`, `abb`, `abbb` и
т. д., а не `ab`, `abab`, `ababab` и т. д.

Пример.

Создайте текстовый файл `g.txt`, содержащий следующий текст:

```
xza
xyz
xyyz
xyyyza
xyyyyza
tyyyyza
xyyyyta
zxdfa
```

Что выведет команда `grep 'xy*z' g.txt`? А команда 
`grep 'xy\+z' g.txt`? А команда `grep -x 'xy*z' g.txt`? Почему?
Что выведет команда `grep 'xy\{2,3\}z' g.txt`?

Конструкция выбора альтернативы `|`. Регулярное выражение, 
составленное из других при помощи этой конструкции, соответствует 
строкам, соответствующим любой из альтернатив. Например, `a|b|c` — это
то же самое, что и `[abc]`. Приоритет у этой операции ниже, чем у 
конкатенации, так что регулярное выражение `begin|end` соответствует
строкам `begin` и `end`, а не `beginnd` и `begiend`.

Пример.

Создайте текстовый файл `h.txt`, содержащий следующий текст:

```
one tree
two trees
three apples
four plums
five rabbits
six wires
seven dolls
eight balls
```

Напишите команду, выдающую строки, содержащие текст tree или ball, 
тремя способами. Подсказка: можно использовать конструкцию выбора 
альтернативы, два регулярных выражения в одной командной строке или 
чтение регулярных выражений из файла. 

```
grep 'tree\|ball' h.txt
```

Увы, в языке регулярных выражений, понимаемом большинством программ 
для Linux, нет операции  «и», т. е. нельзя потребовать, чтобы текст 
соответствовал нескольким регулярным выражениям сразу. Обойдя эту 
трудность, напишите команду, выдающую на экран те строки файла `h.txt`
, которые содержат как букву `a`, так и букву `s`, в любом месте 
строки и в любом порядке. 

Подсказка: можно воспользоваться операцией выбора альтернативы, а 
можно — программным конвейером.

```
grep '.*a.*s.*\|.*s.*a.*' h.txt
```

Операция группирования. Часто при составлении регулярных выражений 
возникает необходимость использовать операции не в том порядке, в 
котором указывают их приоритеты. Для решения этой проблемы можно 
использовать операцию группирования (круглые скобки). Например, 
регулярное выражение `(begin)+` соответствует строкам из нескольких 
идущих подряд друг за другом слов `begin`. Также операция 
группирования используется для ограничения области действия 
конструкции выбора альтернативы. Например, `abc(de|fg)hij` 
соответствует `abcdehij` или `abcfghij`. 

Пример.

Создайте текстовый файл `i.txt`, содержащий следующий текст:

```
beginbeginbegin
beginendbegin
endendendend
a line end
begin++++++end
begin of the text
```

Напишите команду, выдающую на экран строки, состоящие только из 
повторений слова `begin`. 

```
grep '^\(begin\)\+$' i.txt
```

Также, выдайте на экран строки, состоящие из повторений слова 
`begin` или слова `end` (смесь тех и других не допускается). 

```
grep '^\(begin\)\+$\|^\(end\)\+$' i.txt
```

Наконец, выдайте на экран строки, состоящие из нескольких повторений 
слова `begin` или `end`, причем в одной строке допустимо чередовать 
и то, и другое слово в любых сочетаниях.

```
grep '^\(begin\|end\)\+$' i.txt
```

#Манипуляции — sed

Утилита `sed` — это так называемый потоковый редактор; он позволяет 
редактировать проходящий через него поток информации при помощи 
определенного набора команд в пакетном (т. е. не интерактивном) 
режиме.

Он имеет два буфера для данных, которые называются пространствами 
шаблонов и задержки соответственно. Принцип его работы
состоит в следующем: очередная строка копируется в пространство 
шаблонов, после чего выполняются команды редактирования. Каждая 
команда редактирования может иметь определенное условие на входную 
строку; выполняются только те команды, условия которых истинны. После 
этого (если нет опции `-n`) содержимое пространства шаблонов 
записывается в стандартный поток вывода. Наконец, пространство 
шаблонов очищается, и начинается следующий цикл.

При этом та информация, которую необходимо сохранить между циклами, 
должна быть помещаема в пространство задержек, так как его содержимое 
не очищается в момент начала нового цикла.

Командная строка при вызове `sed` должна содержать опции, скрипт с 
командами редактирования, а также имена входных файлов (если 
информация читается из файлов, а не из стандартного ввода).

#Более сложные манипуляции — awk

`awk` представляет собой обобщение `sed`; сохраняя ту же цель, он 
представляет собой полномасштабный язык программирования.

`awk` изначально ориентирован на обработку текстовых баз данных. Это 
означает, что в нем присутствуют средства для обработки записей 
(в терминологии баз данных) и составляющих их полей. Обычно, в 
большинстве текстовых баз данных, записями являются строки, которые 
делятся на поля либо группами подряд идущих пробельных символов, либо 
некоторым специальным символом. Однако, для удобства программирования,
awk поддерживает режим, в котором полями являются строки, а записи 
(группы полей) отделяются друг от друга пустой строкой.

Командная строка awk выглядит так:

```
awk опции программа входные-файлы
```

Среди опций можно отметить:

  * `-F разделитель-полей` определяет символ, используемый для 
  разделения полей в пределах одной записи.

  * `-f файл` означает, что программа должна быть взята из указанного 
  файла, а не из соответствующего аргумента командной строки.

  * `-v имя=значение` выполнить указанное присваивание до начала 
  работы программы.


Программа выглядит как набор элементов, разделенных переводом строки. 
Элементы бывают двух типов — определения пользовательских функций и 
пары `образец действие`. Образец определяет, когда необходимо 
выполнить соответствующее действие, и может иметь следующий вид:

`BEGIN` соответствующее действие выполняется в самом начале работы 
программы, до обработки входных файлов. Обычно используется для 
инициализации процесса обработки, например для присваивания начальных 
значений переменным. Например, если нам нужно вычислить сумму значений
некоторого поля, здесь уместно присвоить той переменной, в которой 
накапливается сумма, нулевое значение.

`END` действие выполняется в конце работы программы, после обработки 
входных файлов. Обычно используется для завершения процесса обработки,
например для вывода результатов, содержащих обобщающую информацию по
всей совокупности входных данных. Например, если нам нужно вычислить 
сумму значений некоторого поля, здесь уместно вывести ее окончательное
значение.

выражение может быть любым допустимым в awk выражением, и действие 
выполняется для тех записей, которые удовлетворяют указанному 
выражению (т. е. на которых оно истинно). Например, выражение `NR==20`
означает 20-ю запись (забегая вперед, `NR` — встроенная переменная, 
которая означает номер текущей записи).

`образец1, образец2` означает, что действие выполняется для диапазона 
записей, определяемого образцами, включительно.