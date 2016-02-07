#C Operator Precedence

##Precedence 1

Associativity: Left-to-right

Operator | Description
---------|-------------
`++`,`--` | Suffix/postfix increment and decrement
`()`	| 	Function call
`[]`	|	Array subscripting
`.`		|	Structure and union member access
`->`	|	Structure and union member access through pointer

##Precedence 2

Associativity: Right-to-left

Operator | Description
---------|-------------
`++`,`--` | Prefix increment and decrement
`+`,`-`	 |	Unary plus and minus
`!`, `~` |	Logical NOT and bitwise NOT
`(type)` |	Type cast
`*`		|	Indirection (dereference)
`&`		|	Address-of
`sizeof` |	Size-of


##Precedence 3

Associativity: Left-to-right

`*`,`/`,`%` -- Multiplication, division, and remainder

##Precedence 4

Associativity: Left-to-right

`+`,`-` -- Addition and subtraction

##Precedence 5

Associativity: Left-to-right

`<<`,`>>` -- Bitwise left shift and right shift

##Precedence 6

Associativity: Left-to-right

`<`,`<=`,`>`,`>=` -- Relational operators

##Precedence 7

Associativity: Left-to-right

`==`,`!=` -- For relational `=` and "not" `=` respectively

##Precedence 8

Associativity: Left-to-right

`&` -- Bitwise AND

##Precedence 9

Associativity: Left-to-right

`^` -- Bitwise XOR (exclusive or)

##Precedence 10

Associativity: Left-to-right

`|` -- Bitwise OR (inclusive or)

##Precedence 11

Associativity: Left-to-right

`&&` -- Logical AND

##Precedence 12

Associativity: Left-to-right

`||` -- Logical OR

##Precedence 13

Associativity: Right-to-left

`?:` -- Ternary conditional

##Precedence 14

Associativity: Right-to-left

Operator | Description
---------|------------
`=`		|	Simple assignment
`+=`,`-=` | Assignment by sum and difference
`*=`,`/=`,`%=` | Assignment by product, quotient, and remainder
`<<=`,`>>=`		| Assignment by bitwise left shift and right shift
`&=`,`^=`,`|=`	| Assignment by bitwise AND, XOR, and OR

##Precedence 15

Associativity: Left-to-right

`,` -- Comma
