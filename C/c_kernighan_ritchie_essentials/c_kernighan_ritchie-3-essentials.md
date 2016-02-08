#Control Flow

The control-flow of a language specify the order in which computations 
are performed.

#Statements and Blocks

An expression such as `x = 0` or `i++` or `printf(...)` becomes a 
*statement* when it is followed by a semicolon, as in

```
x = 0;
i++;
printf(...);
```

In C, the semicolon is a statement terminator

Braces `{` and `}` are used to group declarations and statements 
together into a *compound statement*, or *block*, so that they are 
syntactically equivalent to a single statement.

The braces that surround the statements of a function are one obvious 
example

braces around multiple statements after an `if`, `else`, `while`, or 
`for` are another. Variables can be declared inside *any* block.

#If-Else

Because the else part of an `if-else` is optional,there is an ambiguity
when an `else if` omitted from a nested `if` sequence. This is resolved
by associating the `else` with the closest previous `else`-less `if`. 
For example, in

```
if(n > 0)
	if(a > b)
		z = a;
	else
		z = b;
```

the `else` goes to the inner `if`, as we have shown by indentation. 
If that isn't what you want, braces must be used to force the proper 
association:

```
if(n > 0) {
	if (a > b)
		z = a;
}
else
	z = b;
```

#Switch

The `switch` statement is a multi-way decision that tests whether an 
expression matches one of a number of *constant* **integer** values, 
and branches accordingly.

```
switch (expression) {
	case const-expr: statements
	case const-expr: statements
	default: statements
}
```

Each case is labeled by one or more integer-valued constants or 
constant expressions. If a case matches the expression value, execution
starts at that case. 

All case expressions must be different. The case labeled default is 
executed if none of the other cases are satisfied. A default is 
optional; if it isn't there and if none of the cases match, no action 
at all takes place. 

Cases and the default clause can occur in any order.

The `break` statement causes an immediate exit from the switch. Because
cases serve just as labels, after the code for one case is done, 
execution *falls through* to the next unless you take explicit action 
to escape. `break` and `return` are the most common ways to leave a 
switch. A `break` statement can also be used to force an immediate exit
from `while`, `for`, and `do` loops.

#Loops - While and For

the comma "`,`" is the C operator, which most often finds use in the 
`for` statement. A pair of expressions separated by a comma is 
evaluated left to right, and the type and value of the result are the 
**type** and **value** of the **right** operand. 

Thus in a `for` statement, it is possible to place multiple expressions
in the various parts, for example to process two indices in parallel.

The commas that separate function arguments, variables in 
declarations, etc., are **not** comma operators, and do not guarantee left to right evaluation.

comma operators should be used sparingly. The most suitable uses are 
for constructs strongly related to each other, as in the `for` loop in
reverse, and in macros where a multistep computation has to be a 
single expression.

#Break and Continue

It is sometimes convenient to be able to exit from a loop other than 
by testing at the top or bottom. The `break` statement provides an 
early exit from `for`, `while`, and `do`, just as from `switch`. 
A `break` causes the innermost enclosing loop or switch to be exited 
immediately.

The `continue` statement is related to `break`, but less often used; 
it causes the next iteration of the enclosing `for`, `while`, or `do` 
loop to begin. In the `while` and `do`, this means that the 
**test part** is executed immediately; in the `for`, control passes 
to the **increment step**. 

The `continue` statement applies only to loops, not to `switch`. 
A `continue` inside a `switch` inside a loop causes the next loop 
iteration.

#Goto and labels

C provides the `goto` statement, and labels to branch to. Formally, 
the goto statement is never necessary, and in practice it is almost 
always easy to write code without it.

Nevertheless, there are a few situations where `goto`s may find a 
place. The most common is to abandon processing in some deeply nested 
structure, such as breaking out of two or more loops at once. 

The `break` statement cannot be used directly since it only exits from
the innermost loop. Thus:

```
	for ( ... )
		for ( ... ) {
			...
			if (disaster)
				goto error;
		}
	...
error:
	/* clean up the mess */
```

This organization is handy if the error-handling code is non-trivial, 
and if errors can occur in several places.

A *label* has the same form as a variable name, and is followed by a 
*colon*. It can be attached to any statement in the same function as 
the `goto`. The scope of a label is the entire function.
