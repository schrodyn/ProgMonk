(Avoiding) Flow Control
=======================

In typical imperative Python programs a block of
code generally consists of some outside loops ( for or while ), assign‐
ment of state variables within those loops, modification of data
structures like dicts, lists, and sets, and
some branch statements ( if / elif / else or try / except / finally ).

The imperative flow control is much more about the “how” than the “what”

Encapsulation
=============

One obvious way of focusing more on “what” than “how” is simply
to refactor code, and to put the data construction in a more isolated
place

for example, instead of:
# configure the data to start with
collection = get_initial_state()
state_var = None
for datum in data_set:
	blalblala...


# Now actually work with the data
for thing in collection:
	process(thing)


do this:
# tuck away construction of data
def make_collection(data_set):
	collection = get_initial_state()
	state_var = None
	for datum in data_set:
		blablabla...
	
	return collection

# Now actually work with the data
for thing in make_collection(data_set):
	process(thing)

Comprehensions
==============

Using comprehensions is often a way both to make code more com‐
pact and to shift our focus from the “how” to the “what.”

For example:
collection = list()
for datum in data_set:
	if condition(datum):
		collection.append(datum)
	else:
		new = modify(datum)
		collection.append(new)

Somewhat more compactly we could write this as:
collection = [d if condition(d) else modify(d)
		for d in data_set]

Far more important than simply saving a few characters and lines is
the mental shift enacted by thinking of what collection is, and by
avoiding needing to think about or debug “What is the state of col
lection at this point in the loop?”

Generators
==========

Generator comprehensions have the same syntax as list comprehensions—
other than that there are no square brackets around them (but parentheses 
are needed  syntactically in some contexts, in place of brackets)—but they are 
also lazy.

That is to say that they are
merely a description of “how to get the data” that is not realized
until one explicitly asks for it, either by calling .next() on the
object, or by looping over it.

This often saves memory for large sequences and defers computation until it is 
actually needed.

log_lines = (line for line in read_line(huge_log_file)
		if complex_condition(line))

For typical uses, the behavior is the same as if you had constructed a
list, but runtime behavior is nicer.

Obviously, this generator comprehension also has imperative versions.
We could do this using "iterator protocol" with a
class that had .__next__() and .__iter__() methods. For example: 

class GetLogLines(object):
	def __init__(self, log_file):
		self.log_file = log_file
		self.line = None
	def __iter__(self):
		return self
	def __next__(self):
		if self.line is None:
			self.line = read_line(log_file)
		while not complex_condition(self.line):
			self.line = read_line(self.log_file)
		return self.line

log_lines = GetLogLines(huge_log_file)

the reader should see that the comprehension focuses attention much better on the “what,” whereas the imperative version retains the focus on the “how.”

Dicts and Sets
==============

In the same fashion that lists can be created in comprehensions
rather than by creating an empty list, looping, and calling .append(),
dictionaries and sets can be created “all at once”
rather than by repeatedly calling .update() or .add() in a loop.

>>> {i:chr(65+i) for i in range(6)}
{0: 'A', 1: 'B', 2: 'C', 3: 'D', 4: 'E', 5: 'F'}
>>> {chr(65+i) for i in range(6)}
{'A', 'B', 'C', 'D', 'E', 'F'}

Recursion
=========

Functional programmers often put weight in expressing flow control through 
recursion rather than through loops.

in considering using recursive styles we should distinguish between the cases 
where recursion is just “iteration by another name” and those where a problem 
can readily be partitioned into smaller problems, each approached in a
similar way.

Python is simply comparatively slow at recursion, and has a limited
stack depth limit. Yes, you can change this with sys.setrecursionlimit() 
to more than the default 1000; but if you find yourself
doing so it is probably a mistake.

Python lacks an internal feature
called tail call elimination that makes deep recursion computationally 
efficient in some languages.

As a footnote, the fastest version I know of for factorial() in
Python is in a functional programming style, and also expresses the
“what” of the algorithm well once some higher-order functions are
familiar:

from functools import reduce
from operator import mul
def factorialHOF(n):
	return reduce(mul, range(1, n+1), 1)

Where recursion is compelling, and sometimes even the only really
obvious way to express a solution, is when a problem offers itself to
a “divide and conquer” approach.

For example, the quicksort algorithm is very elegantly
expressed without any state variables or loops, but wholly through
recursion:

def quicksort(lst):
	"Quicksort over a list-like sequence"
	if len(lst) == 0:
		return lst
	pivot = lst[0]
	pivots = [x for x in lst if x == pivot]
	small = quicksort([x for x in lst if x < pivot])
	large = quicksort([x for x in lst if x > pivot])
	return small + pivots + large

Eliminating Loops
=================

Just for fun, let us take a quick look at how we could take out all
loops from any Python program.

If we simply call a function inside a for loop, the built-in higher-
order function map() comes to our aid:

# statement-based loop
for e in it:
	func(e)

The following code is entirely equivalent to the functional version:

# map()-based "loop"
map(func, it)

A similar technique is available for a functional approach to sequential 
program flow.

Most imperative programming consists of statements that amount to “do this, then 
do that, then do the other thing.” If those individual actions are wrapped in functions, map() lets us do just this:

# let f1, f2, f3 (etc) be functions that perform actions
# an execution utility function
do_it = lambda f, *args: f(*args)
# map()-based action sequence
map(do_it, [f1, f2, f3])

We can combine the sequencing of function calls with passing arguments from 
iterables:

>>> hello = lambda first, last: print("Hello", first, last)
>>> bye = lambda first, last: print("Bye", first, last)
>>> _ = list(map(do_it, [hello, bye],
>>>			['David','Jane'], ['Mertz','Doe']))
Hello David Mertz
Bye Jane Doe

Removing while loop
Let's see a concrete example of eliminating statements:

# imperative version of "echo()"
def echo_IMP():
	while 1:
		x = input("IMP -- ")
	if x == 'quit':
		break
	else:
		print(x)
echo_IMP()

Now let’s remove the while loop for the functional version:

# FP version of "echo()"
def identity_print(x):	# "identity with side-effect"
	print(x)
	return x

echo_FP = lambda: identity_print(raw_input("FP -- "))=='quit' or echo_FP()
echo_FP()

What we have accomplished is that we have managed to express a
little program that involves I/O, looping, and conditional statements
as a pure expression with recursion (in fact, as a function object that
can be passed elsewhere if desired). We do still utilize the utility
function identity_print() , but this function is completely general,
and can be reused in every functional program expression we might
create later (it’s a one-time cost).

Eliminating Recursion
=====================

sometimes we can perform “recursion without recursion” by using functools.reduce() or other folding operations
A recursion is often simply a way of
combining something simpler with an accumulated intermediate
result, and that is exactly what reduce() does at heart.
A slightly longer discussion of functools.reduce() occurs in the chapter on
higher-order functions.

Callables
---------
Python actually gives us several different ways to create functions. They are:

1) Regular functions created with def and given a name at definition time
2) Anonymous functions created with lambda
3) Instances of classes that define a __call()__ method
4) Closures returned by function factories
5) Static methods of instances, either via the @staticmethod decorator or via the
  class __dict__
6) Generator functions

Named Functions and Lambdas:
The only inprinciple difference between them is simply whether they have
a .__qualname__ attribute, since both can very well be bound to one
or more names.

Closures and Callable Instances:
There is a saying in computer science that a class is “data with operations 
attached” while a closure is “operations with data attached.”

Let us construct a toy example that shows this:

# A class that creates callable adder instances
class Adder(object):
    def __init__(self, n):
        self.n = n
    def __call__(self, m):
        return self.n + m
add5_i = Adder(5) # "instance" or "imperative"

We have constructed something callable that adds five to an argument passed in.

Let us also try it as a closure:

def make_adder(n):
    def adder(m):
        return m + n
    return adder
add5_f = make_adder(5)	# "functional"

So far these seem to amount to pretty much the same thing, but the
mutable state in the instance provides a attractive nuisance:

>>> add5_i.n = 10	# state is readily changeable
>>> add5_i(10)	# result is dependent on prior flow
20

Once the object exists, the closure behaves in a pure functional
way, while the class instance remains state dependent.

Methods of Classes
------------------
All methods of classes are callables. However, calling a method of an instance 
goes against the grain of functional programming styles.
Since each call to a method may produce a different result that
varies independently of the arguments passed to it.

Accessors and Operators
Even accessors, whether created with the @property decorator or
otherwise, are technically callables

class Car(object):
    def __init__(self):
        self._speed = 100

    @property
    def speed(self):
	print("Speed is", self._speed)
	return self._speed

    @speed.setter
    def speed(self, value):
	print("Setting to", value)
	self._speed = value

>> car = Car()
>>> car.speed = 80 # Odd syntax
Setting to 80
>>> x = car.speed
Speed is 80 

Operator in Python:

class TalkativeInt(int):
    def __lshift__(self, other):
	print("Shift", self, "by", other)
	return int.__lshift__(self, other)

>>> t = TalkativeInt(8)
>>> t << 3
Shift 8 by 3
64

Every operator in Python is basically a method call “under the hood.”

Static Methods of Instances
One use of classes and their methods is to use them simply as
namespaces to hold a variety of related functions:

import math
class RightTriangle(object):
   "Class used solely as namespace for related functions"
   @staticmethod
   def hypotenuse(a, b):
	return math.sqrt(a**2 + b**2)

   @staticmethod
   def sin(a, b):
	return a / RightTriangle.hypotenuse(a, b)

   @staticmethod
   def cos(a, b):
	return b / RightTriangle.hypotenuse(a, b)

Keeping this functionality in a class avoids polluting the global (or
module, etc.) namespace. For example:

>>> RightTriangle.hypotenuse(3,4)
5.0
>>> rt = RightTriangle()
>>> rt.sin(3,4)
0.6
>>> rt.cos(3,4)
0.8

By far the most straightforward way to define static methods is with
the decorator named @staticmethod.

However, in Python 3.x, you can write this:

import functools, operator
class Math(object):
    def product(*nums):
	return functools.reduce(operator.mul, nums)
    def power_chain(*nums):
	return functools.reduce(operator.pow, nums)

>>> Math.product(3,4,5)
60
>>> Math.power_chain(3,4,5)
3486784401

Without @staticmethod , however, this will not work on the instances 
since they still expect to be passed self :

>>> m = Math()
>>> m.product(3,4,5)
TypeError

Generator Functions
-------------------




