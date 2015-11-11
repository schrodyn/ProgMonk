If Statements

1) Avoid placing conditional branch code on the same line as the colon:

Harmful:

name = 'Jeff'
address = 'New York, NY'
if name: print(name)
print(address)

Idiomatic:

name = 'Jeff'
address = 'New York, NY'
if name:
    print(name)
print(address)

2) Avoid repeating variable name in if statement 

Harmful:

is_generic_name = False
name = 'Tom'
if name == 'Tom' or name == 'Dick' or name == 'Harry':
    is_generic_name = True 

Idiomatic:

name = 'Tom'
is_generic_name = name in ('Tom', 'Dick', 'Harry')

3) Avoid comparing directly to True , False , or None

For any object, be it a built-in or user defined, there is a “truthiness” 
associated with the object.
All of the following are considered False :

* None
* False
* zero for numeric types
* empty sequences
* empty dictionaries
* a value of 0 or False returned when either __len__ or __nonzero__ is called

Everything else is considered True.
To define how "truthiness" should work for any class you create, you should define
__len__ or __nonzero__ function for your class.

Instead of checking if a variable foo is True like this

if foo == True:

you should simply check `if foo:`.

There are a number of reasons for this. The most obvious is that if your code 
changes and foo becomes an int instead of True or False , your if statement still
works.

But at a deeper level, the reasoning is based on the difference between 
"equality" and "identity". Using == determines if two objects have the same value
(as defined by their `_eq` attribute). Using `is` determines if the two objects 
are actually the same object.

So avoid comparing directly to False and None and empty sequences
like [] , {} , and ().

If a list named my_list is empty, calling `if my_list:` will evaluate to False.

There are times, however, when comparing directly to None is not just recommended,
but required. A function checking if an argument whose default value is None was
actually set must compare directly to None like so:

def insert_value(value, position=None):
    """Inserts a value into my container, optionally at the
    specified position"""
    if position is not None:
	...

What’s wrong with `if position:` ? Well, if someone wanted to insert into 
position 0 , the function would act as if position hadn’t been set, since 0 
evaluates to False . 
Note the use of `is not :` comparisons against `None` (a singleton in Python). 
You should always use `is` or `is not` , not `==` to compare with `None`.

-------------------------
For loops

1) Use the `enumerate` function in loops instead of creating an “index” variable

Programmers coming from other languages are used to explicitly declaring a 
variable to track the index of a container in a loop. For example, in C++:

for (int i=0; i < container.size(); ++i)
{
    // Do stuff
}

In Python, the `enumerate` built-in function handles this role.

Harmful:

my_container = ['Larry', 'Moe', 'Curly']
index = 0
for element in my_container:
    print ('{} {}'.format(index, element))
    index += 1

Idiomatic:

my_container = ['Larry', 'Moe', 'Curly']
for index, element in enumerate(my_container):
    print ('{} {}'.format(index, element))


2) Use the `in` keyword to iterate over an `iterable`

Harmful:

my_list = ['Larry', 'Moe', 'Curly']
index = 0
while index < len(my_list):
    print (my_list[index])
    index += 1

Idiomatic:

my_list = ['Larry', 'Moe', 'Curly']
for element in my_list:
    print (element)


3) Use `else` to execute code after a `for` loop concludes

One of the lesser known facts about Python’s `for` loop is that it can include an
`else` clause. 

The `else` clause is executed after the iterator is exhausted, unless the loop 
was ended prematurely due to a `break` statement.

This allows you to check for a condition in a `for` loop, `break` if the 
condition is satisfied for an element, else take some action if the
condition fails for any of the elements being looped over.

In the scenario below, we are running a report to check if any of the email 
addresses our users registered are malformed (users can register multiple 
addresses).

Harmful:

for user in get_all_users():
    has_malformed_email_address = False
    print ('Checking {}'.format(user))
    for email_address in user.get_all_email_addresses():
	if email_is_malformed(email_address):
	   has_malformed_email_address = True
	   print ('Has a malformed email address!')
	   break
    if not has_malformed_email_address:
	print ('All email addresses are valid!')

Idiomatic:

for user in get_all_users():
    print ('Checking {}'.format(user))
    for email_address in user.get_all_email_addresses():
	if email_is_malformed(email_address):
	    print ('Has a malformed email address!')
	    break
    else:
	print ('All email addresses are valid!')


---------------------
Functions

1) Avoid using a mutable object as the `default_value` for a function argument

When the Python interpreter encouters a function definition, default arguments 
are evaulated to determine their value. 
This evaluation, however, occurs only once.

_Calling_ the function does not trigger another evaluation of the arguments.

Since the computed value is used in all subsequent calls to the function, using a
 mutable object as a default value often yields unintended results.

A *mutable* object is one whose value can be changed directly.
`list` , `dict` , `set` , and most class instances are mutable.
We can *mutate* a list by calling, for example, `append` on it.

*Immutable* objects, by contrast, can not be altered after they are created.
`string` , `int` , and `tuple` objects are all examples of immutable objects.
We can’t directly change the value of a `string` , for example.

All `string` operations that would alter a `string` instead return *new* `string` 
objects.

So why does this matter for default arguments? For immutable objects
like `strings` , this is fine, since there is no way for us to change that value 
directly.
For mutable objects, however, changing the value of a default argument will be 
reflected in subsequent calls to the function.

Harmful:

def f(a, L=[]):
    L.append(a)
    return L

print(f(1))
print(f(2))
print(f(3))

# This will print
#
# [1]
# [1, 2]
# [1, 2, 3]


Idiomatic:

# If you don't want the default to be shared between subsequent
# calls, you can write the function like this instead:

def f(a, L=None):
    if L is None:
	L = []
    L.append(a)
    return L

print(f(1))
print(f(2))
print(f(3))

# This will print
# [1]
# [2]
# [3]



2) Use *args and **kwargs to accept arbitrary arguments

Using *args and **kwargs as parameters allows a function to accept an
arbitrary list of positional and keyword arguments, respectively.

The idiom is also useful when maintaining backwards compatibility in an API.
If our function accepts arbitrary arguments, we are free to add new arguments in 
a new version while not breaking existing code using fewer arguments.

Harmful:

def make_api_call(foo, bar, baz):
    if baz in ('Unicorn', 'Oven', 'New York'):
	return foo(bar)
    else:
	return bar(foo)

# I need to add another parameter to `make_api_call`
# without breaking everyone's existing code.

def so_many_options():
    # I can tack on new parameters, but only if I make
    # all of them optional...
    def make_api_call(foo, bar, baz, qux=None, foo_polarity=None,
		      baz_coefficient=None, quux_capacitor=None,
		      bar_has_hopped=None, true=None, false=None,
		      file_not_found=None):
	# ... and so on ad infinitum
	return file_not_found


Idiomatic:

def make_api_call(foo, bar, baz):
    if baz in ('Unicorn', 'Oven', 'New York'):
        return foo(bar)
    else:
        return bar(foo)

# I need to add another parameter to `make_api_call`
# without breaking everyone's existing code.
# Easy...

def new_hotness():
    def make_api_call(foo, bar, baz, *args, **kwargs):
	# Now I can accept any type and number of arguments
	# without worrying about breaking existing code.
	baz_coefficient = kwargs['baz_coefficient']
	
	# I can even forward my args to a different function without
	# knowing their contents!
	return baz_coefficient in new_function(args)



----------------------------
Exceptions

1) Don’t be Afraid to Use Exceptions
  
In many languages, exceptions are reserved for truly exceptional cases.
Many organizations have explicit coding standards
that forbid their use (see, for example, Google’s official C++ Style Guide).

Python takes a different view. Exceptions can be found in almost every popular 
third-party package, and the Python standard library makes liberal use of them.

For example, did you
know that any time you use a `for` loop in Python, you’re using exceptions?

That may sound odd, but it’s true: exceptions are used for control flow throughout
 the Python language. 
Have you ever wondered how `for` loops know when to stop?
For things like `lists` that have an easily determined length the question seems 
trivial. But what about `generators` , which could produce values *ad infinitum*?

Any time you use `for` to iterate over an `iterable`
(basically, all sequence types and 
anything that defines __iter__() or __getitem__() ), it needs to know when to stop
iterating. Take a look at the code below:

words = ['exceptions', 'are', 'useful']
for word in words:
    print(word)

How does `for` know when it’s reached the last element in words and should stop 
trying to get more items? The answer may surprise you: the list raises a 
`StopIteration` exception.

In fact, all `iterables` follow this pattern. When a `for` statement is first 
evaluated, it calls `iter()` on the object being iterated over. 

This creates an `iterator` for the object,
capable of returning the contents of the object in sequence.

For the call to `iter()` to succeed, the object must either support the iteration 
protocol (by defining `__iter__()`) or the sequence protocol 
(by defining `__getitem__()` ).

As it happens, both the `__iter__()` and `__getitem__()` functions are required to
 raise an exception when the items to iterate over are exhausted.

`__iter__()` raises the `StopIteration` exception, as discussed earlier,
and `__getitem__()` raises the `IndexError` exception.
This is how `for` knows when to stop.

So remember this: 
for all but the most trivial programs, you’re probably using exceptions already.


2) Use Exceptions to Write Code in an “EAFP” Style

What is EAFP style?
EAFP stands for "Easier to ask for forgiveness than permission".
This is common Python coding style.

An example would be an attempt to access a dictionary key.
EAFP:

try:
    x = my_dict["key"]
except KeyError:
    # handle missing key

The technique contrasts with the LBYL style common to many other languages such 
as C. LBYL stands for "Look before you leap".

LBYL:

if "key" in my_dict:
    x = my_dict["key"]
else:
    # handle missing key

The LBYL version has to search the key inside the dictionary twice, and might also
 be considered slightly less readable.

Code that doesn’t use exceptions is always checking if it’s OK to do something.
Code written in this manner must ask a number of different questions before it is
convinced it’s OK to do something.

The `if` statements littered throughout the function give both the programmer and 
readers of the code a false sense of security. Calling it shouldn’t
require any error handling.

There’s an obvious problem with this approach: if the code doesn’t ask all of the
right questions, bad things happen.

What’s more, as the Python documentation says, code written in this style can fail
 badly in a multi-threaded environment.
A condition that was true in an `if` statement may be false by the next line.

Alternatively, code written according to the principle, “[It’s] Easier to Ask for
Forgiveness than Permission (EAFP),” assumes things will go well and catches
exceptions if they don’t.

Rather than seeing a string of `if` statements and needing to remember what each
checked before you even know what the code wants to do, EAFP-style code presents
the end goal first.
The error handling code that follows is easier to read;

Harmful:

def get_log_level(config_dict):
    if 'ENABLE_LOGGING' in config_dict:
	if config_dict['ENABLE_LOGGING'] != True:
	    return None
	elif not 'DEFAULT_LOG_LEVEL' in config_dict:
	    return None
	else: return config_dict['DEFAULT_LOG_LEVEL']
    else:
	return None


Idiomatic:

def get_log_level(config_dict):
    try:
	if config_dict['ENABLE_LOGGING']:
	    return config_dict['DEFAULT_LOG_LEVEL']
    except KeyError:
	# if either value wasn't present, a
	# KeyError will be raised, so
	# return None
	return None


3) Avoid “Swallowing” Useful Exceptions With Bare Except Clauses

A common mistake made by novices when using exceptions is to feel compelled to
catch any exception code could raise. Programmers encapsulate all uses of the 
package in `try` blocks followed by an `except` clause that doesn’t specify an 
exception (also known as a “bare” except clause).

Exceptions have tracebacks and messages for a reason: to aid in debugging when
something goes wrong. If you need to know whenever an exception occurs but don’t 
intend to handle it (say for logging purposes), add a bare `raise` to the end 
of your `except` block. The bare `raise` re-raises the exception that was caught.

Harmful:

import requests
def get_json_response(url):
    try:
	r = requests.get(url)
	return r.json()
    except:
	print('Oops, something went wrong!')
	return None


Idiomatic:

import requests
def get_json_response(url):
    return requests.get(url).json()

# If we need to make note of the exception, we
# would write the function this way...
def alternate_get_json_response(url):
    try:
	r = requests.get(url)
	return r.json()
    except:
	# do some logging here, but don't handle the exception
	# ...
	raise


----------------------
Working With Data


1) Avoid using a temporary variable when performing a `swap` of two values

There is no reason to `swap` using a temporary variable in Python.
We can use tuples to make our intention more clear.

Harmful:

foo = 'Foo'
bar = 'Bar'
temp = foo
foo = bar
bar = temp


Idiomatic:

foo = 'Foo'
bar = 'Bar'
(foo, bar) = (bar, foo)


2) Chain `string` functions to make a simple series of transformations more clear

Too much chaining, however, can make your code harder to
follow. “No more than three chained functions” is a good rule of thumb.

Harmful:

book_info = ' The Three Musketeers: Alexandre Dumas'
formatted_book_info = book_info.strip()
formatted_book_info = formatted_book_info.upper()
formatted_book_info = formatted_book_info.replace(':', ' by')

Idiomatic:

book_info = ' The Three Musketeers: Alexandre Dumas'
formatted_book_info = book_info.strip().upper().replace(':', ' by')


3) Use ''.join when creating a single string for list elements

It’s faster, uses less memory, and you’ll see it everywhere anyway.
'' just means we wish to concatenate the elements with no characters between them.

Harmful:

result_list = ['True', 'False', 'File not found']
result_string = ''
for result in result_list:
    result_string += result


Idiomatic:

result_list = ['True', 'False', 'File not found']
result_string = ''.join(result_list)

4) Prefer the `format` function for formatting strings

There are three general ways of formatting strings:

1) Easily the worst approach is to use
the `+` operator to concatenate a mix of static strings and variables.

2) Using “old-style” string formatting; 
It makes use of a format string and the `%` operator to fill in values, much like 
`printf` does in other languages.

3) The clearest and most idiomatic way to format strings is to use the `format` 
function.
With the `format` function, we can use named placeholders, access their 
attributes, and control padding and string width. 
The `format` function makes string formatting clean and concise.

Harmful:

def get_formatted_user_info_worst(user):
    # Tedious to type and prone to conversion errors
    return 'Name: ' + user.name + ', Age: ' + \
	    str(user.age) + ', Sex: ' + user.sex

def get_formatted_user_info_slightly_better(user):
    # No visible connection between the format string placeholders
    # and values to use. Also, why do I have to know the type?
    # Don't these types all have __str__ functions?
    return 'Name: %s, Age: %i, Sex: %c' % (
	user.name, user.age, user.sex)


Idiomatic:

def get_formatted_user_info(user):
    # Clear and concise. At a glance I can tell exactly what
    # the output should be. Note: this string could be returned
    # directly, but the string itself is too long to fit on the
    # page.
    output = 'Name: {user.name}, Age: {user.age}, Sex: {user.sex}'.format(user)
    return output


5) Use a `list comprehension` to create a transformed version of an existing list

`list comprehension`s increase clarity in code that builds a list from existing 
data.
There are also (usually) performance benefits to using a `list comprehension`
(or alternately, a `generator expression` ) due to optimizations in the cPython 
interpreter.

Harmful:

some_other_list = range(10)
some_list = list()
for element in some_other_list:
    if is_prime(element):
	some_list.append(element + 5)


Idiomatic:

some_other_list = range(10)
some_list = [element + 5 for element in some_other_list if is_prime(element)]


6) Use the * operator to represent the “rest” of a list

(Only for python 3)
Often times, it’s useful to extract a few elements at the beginning (or end) of a
list while keeping the “rest” for use later.
Python 3 allows you to use the * operator on the left hand side of an
assignment to represent the rest of a sequence .

Harmful:

some_list = ['a', 'b', 'c', 'd', 'e']
(first, second, rest) = some_list[0], some_list[1], some_list[2:]
print(rest)
(first, middle, last) = some_list[0], some_list[1:-1], some_list[-1]
print(middle)
(head, penultimate, last) = some_list[:-2], some_list[-2], some_list[-
print(head)

Idiomatic:

some_list = ['a', 'b', 'c', 'd', 'e']
(first, second, *rest) = some_list
print(rest)
(first, *middle, last) = some_list
print(middle)
(*head, penultimate, last) = some_list
print(head)


7) Use a `dict` as a substitute for a `switch...case` statement 

Unlike many other languages, Python doesn’t have a `switch...case` construct.
The naive alternative in Python is to write a series of `if...else` statements.

Thankfully, functions are first-class objects in Python, so we can treat them
the same as any other variable.
Rather than trying to emulate the exact functionality, we can take advantage of 
the fact that functions are first-class object and can be stored as values in a 
`dict`.

Using this method, one could create a Factory class that chooses
which type to instantiate via a parameter.
Or it could be used to store states and their transitions when building a state 
machine.

Harmful:

def apply_operation(left_operand, right_operand, operator):
    if operator == '+':
	return left_operand + right_operand
    elif operator == '-':
	return left_operand - right_operand
    elif operator == '*':
	return left_operand * right_operand
    elif operator == '/':
	return left_operand / right_operand


Idiomatic:

def apply_operation(left_operand, right_operand, operator):
    import operator as op
    operator_mapper = {'+': op.add, '-': op.sub,
	    '*': op.mul, '/': op.truediv}
    return operator_mapper[operator](left_operand, right_operand)


8) Use the `default` parameter of `dict.get` to provide default values

Harmful:

log_severity = None
if 'severity' in configuration:
    log_severity = configuration['severity']
else:
    log_severity = 'Info'


Idiomatic:

log_severity = configuration.get('severity', 'Info')

9) Use a `dict comprehension` to build a `dict` clearly and efficiently

Harmful:

user_email = {}
for user in users_list:
    if user.email:
	user_email[user.name] = user.email

Idiomatic:

user_email = {user.name: user.email for user in users_list if user.email}

--------------
Sets

10) Understand and use the mathematical `set` operations

Like a `dict` with keys but no values, the `set` class implements the `Iterable` 
and `Container` interfaces. Thus, a `set` can be used in a `for` loop or as the 
subject of an `in` statement.

Set operations in Python:

* Union
  The set of elements in A , B , or both A and B (written A | B in Python)
* Intersection
  The set of elements in both A and B (written A & B in Python)
* Difference
  The set of elements in A but not B (written A - B in Python)
* Symmetric Difference
  The set of elements in either A or B , but not both A and B (written A ^ B in 
  Python)

When working with lists of data, a common task is finding the elements that 
appear in all of the lists. Look to use a `set`.

Harmful:

def get_both_popular_and_active_users():
    # Assume the following two functions each return a
    # list of user names
    most_popular_users = get_list_of_most_popular_users()
    most_active_users = get_list_of_most_active_users()
    popular_and_active_users = []
    for user in most_active_users:
	if user in most_popular_users:
	    popular_and_active_users.append(user)

    return popular_and_active_users


Idiomatic:

def get_both_popular_and_active_users():
    # Assume the following two functions each return a
    # list of user names
    return (set(
	get_list_of_most_active_users()) & set(
	    get_list_of_most_popular_users()))


11) Use a `set comprehension` to generate sets concisely

Harmful:

users_first_names = set()
for user in users:
    users_first_names.add(user.first_name)


Idiomatic:

users_first_names = {user.first_name for user in users}

12) Use sets to eliminate duplicate entries from `Iterable` containers

It’s quite common to have a `list` or `dict` with duplicate values.
Three aspects of sets make them the perfect answer to our
problem:

1) A `set` contains only unique elements
2) Adding an already existing element to a `set` is essentially “ignored”
3) A `set` can be built from any `Iterable` whose elements are hashable

Harmful:

unique_surnames = []
for surname in employee_surnames:
    if surname not in unique_surnames:
	unique_surnames.append(surname)

def display(elements, output_format='html'):
    if output_format == 'std_out':
	for element in elements:
	    print(element)
    elif output_format == 'html':
	as_html = '<ul>'
	for element in elements:
	    as_html += '<li>{}</li>'.format(element)
	return as_html + '</ul>'
    else:
	raise RuntimeError('Unknown format {}'.format(output_format))


Idiomatic:

unique_surnames = set(employee_surnames)

def display(elements, output_format='html'):
    #as previous example...


------------------------
Tuples

13) Use `collections.namedtuple` to make `tuple`-heavy code more clear

The `tuple` is a fantastically useful data structure in Python.
Most database libraries, for example, use `tuples` to represent a single row in a 
database table.
Queries that return multiple rows are represented as lists of `tuples`.

A `namedtuple` is a normal tuple with a few extra capabilities. Most importantly,
`namedtuples` give you the ability to access fields by names rather than by index.

`collections.namedtuple` is a powerful tool for increasing the readability and
maintainability of code.

Harmful:

# Assume the 'employees' table has the following columns:
# first_name, last_name, department, manager, salary, hire_date
def print_employee_information(db_connection):
    db_cursor = db_connection.cursor()
    results = db_cursor.execute('select * from employees').fetchall()
    for row in results:
	# It's basically impossible to follow what's getting printed
	print(row[1] + ', ' + row[0] + ' was hired '
	'on ' + row[5] + ' (for $' + row[4] + ' per annum)'
	' into the ' + row[2] + ' department and reports to ' + row[3])


Idiomatic:

# Assume the 'employees' table has the following columns:
# first_name, last_name, department, manager, salary, hire_date
from collections import namedtuple

EmployeeRow = namedtuple('EmployeeRow', ['first_name',
'last_name', 'department', 'manager', 'salary', 'hire_date'])

EMPLOYEE_INFO_STRING = '{last}, {first} was hired on {date} \
  ${salary} per annum) into the {department} department and reports to \
ager}'

def print_employee_information(db_connection):
    db_cursor = db_connection.cursor()
    results = db_cursor.execute('select * from employees').fetchall()
    for row in results:
	employee = EmployeeRow._make(row)
	
	# It's now almost impossible to print a field in the wrong place
	print(EMPLOYEE_INFO_STRING.format(
	    last=employee.last_name,
	    first=employee.first_name,
	    date=employee.hire_date,
	    salary=employee.salary,
	    department=employee.department,
	    manager=employee.manager))


14) Use _ as a placeholder for data in a tuple that should be ignored

Harmful:

(name, age, temp, temp2) = get_user_info(user)
if age > 21:
    output = '{name} can drink!'.format(name=name)
# "Wait, where are temp and temp2 being used?"

Idiomatic:

(name, age, _, _) = get_user_info(user)
if age > 21:
    output = '{name} can drink!'.format(name=name)
# "Clearly, only name and age are interesting"


15) Use `tuples` to unpack data 

In Python, it is possible to “unpack” data for multiple assignment.

Harmful:

list_from_comma_separated_value_file = ['dog', 'Fido', 10]
animal = list_from_comma_separated_value_file[0]
name = list_from_comma_separated_value_file[1]
age = list_from_comma_separated_value_file[2]
output = ('{name} the {animal} is {age} years old'.format(
	animal=animal, name=name, age=age))


Idiomatic:

list_from_comma_separated_value_file = ['dog', 'Fido', 10]
(animal, name, age) = list_from_comma_separated_value_file
output = ('{name} the {animal} is {age} years old'.format(
	animal=animal, name=name, age=age))


16) Use a `tuple` to return multiple values from a function

While it’s true that only one object can be returned from a function, that object
may contain multiple logical values.
A `tuple` is ideally suited to be used as a way to return multiple values from a 
function.

Harmful:

from collections import Counter

STATS_FORMAT = """Statistics:
Mean: {mean}
Median: {median}
Mode: {mode}"""

def calculate_mean(value_list):
    return float(sum(value_list) / len(value_list))

def calculate_median(value_list):
    return value_list[int(len(value_list) / 2)]

def calculate_mode(value_list):
    return Counter(value_list).most_common(1)[0][0]

values = [10, 20, 20, 30]
mean = calculate_mean(values)
median = calculate_median(values)
mode = calculate_mode(values)

print(STATS_FORMAT.format(mean=mean, median=median, mode=mode))


Idiomatic:

from collections import Counter

STATS_FORMAT = """Statistics:
Mean: {mean}
Median: {median}
Mode: {mode}"""

def calculate_staistics(value_list):
    mean = float(sum(value_list) / len(value_list))
    median = value_list[int(len(value_list) / 2)]
    mode = Counter(value_list).most_common(1)[0][0]
    return (mean, median, mode)

(mean, median, mode) = calculate_staistics([10, 20, 20, 30])
print(STATS_FORMAT.format(mean=mean, median=median, mode=mode))


----------------------
Classes

1) Use leading underscores in function and variable names 
to denote “private” data

All attributes of a class, be they data or functions, are inherently “public” in 
Python. 
A number of widely followed conventions have arisen to make the author’s 
intention more explicit and help avoid unintentional naming conflicts.

First, attributes to be ‘protected’, which are not meant to be used directly by 
clients, should be prefixed with a single underscore.
Second, ‘private’ attributes not meant to be
accessible by a subclass should be prefixed by *two underscores*.

Of course, these are (mostly) merely conventions. Nothing would stop a client 
from being able to access your ‘private’ attributes.
If `Foo` is a class, the definition `def __bar()` will
be ‘mangled’ to `_classname__attributename` in subclasses.

class Foo():
    def __init__(self):
	self.id = 8
	self.value = self.get_value()

    def get_value(self):
	pass

    def should_destroy_earth(self):
	return self.id == 42

class Baz(Foo):
    def get_value(self, some_new_parameter):
	"""Since 'get_value' is called from the base class's
	__init__ method and the base class definition doesn't
	take a parameter, trying to create a Baz instance will
	fail.

	"""
	pass

class Qux(Foo):
    """We aren't aware of Foo's internals, and we innocently
    create an instance attribute named 'id' and set it to 42.
    This overwrites Foo's id attribute and we inadvertently
    blow up the earth.

    """

    def __init__(self):
	super(Qux, self).__init__()
	self.id = 42
	# No relation to Foo's id, purely coincidental

q = Qux()
b = Baz() # Raises 'TypeError'
q.should_destroy_earth() # returns True
q.id == 42 # returns True


Idiomatic:

class Foo():
    def __init__(self):
	"""Since 'id' is of vital importance to us, we don't
	want a derived class accidentally overwriting it. We'll
	prepend with double underscores to introduce name
	mangling.
	"""
	self.__id = 8
	self.value = self.__get_value() # Our 'private copy'

    def get_value(self):
	pass

    def should_destroy_earth(self):
	return self.__id == 42

    # Here, we're storing a 'private copy' of get_value,
    # and assigning it to '__get_value'. Even if a derived
    # class overrides get_value in a way incompatible with
    # ours, we're fine
    __get_value = get_value


class Baz(Foo):
    def get_value(self, some_new_parameter):
	pass

class Qux(Foo):
    def __init__(self):
	"""Now when we set 'id' to 42, it's not the same 'id'
	that 'should_destroy_earth' is concerned with. In fact,
	if you inspect a Qux object, you'll find it doesn't
	have an __id attribute. So we can't mistakenly change
	Foo's __id attribute even if we wanted to.

	"""
	self.id = 42
	# No relation to Foo's id, purely coincidental
	super(Qux, self).__init__()

q = Qux()
b = Baz() # Works fine now
q.should_destroy_earth() # returns False
q.id == 42 # returns True
with pytest.raises(AttributeError):
    getattr(q, '__id')


2) Use `properties` to “future-proof” your class implementation

Oftentimes, it is convenient to provide direct access to a class’s data 
attributes. 
Point class, for example, may have x and y attributes rather than using 
“getter” and “setter” functions.
However, there’s a reason “getters” and “setters” exist: you never know when what 
once was pure data will require calculation instead.
Suppose we have a `Product` class that is initizialized with a product’s name 
and its price

If we are later asked to apply tax to a product’s price
automatically, does that mean we go through the code and change all occurrences 
of `product.price` to `product.price * TAX_RATE` ?
Not if we thought ahead and made `price` a property!

Harmful:

class Product():
    def __init__(self, name, price):
	self.name = name
	# We could try to apply the tax rate here, but the object's price
	# may be modified later, which erases the tax
	self.price = price


Idiomatic:

class Product():
    def __init__(self, name, price):
	self.name = name
	self._price = price

    @property
    def price(self):
	# now if we need to change how price is calculated, we can do it
	# here (or in the "setter" and __init__)
	return self._price * TAX_RATE

    @price.setter
    def price(self, value):
	# The "setter" function must have the same name as the property
	self._price = value


3) Define `__str__` in a class to show a human-readable representation

When defining a class that is likely to be used with `print()` , the default 
Python representation isn’t too helpful. 
By defining a `__str__` method, you can control how
calling `print` on an instance of your class will look.

Harmful:

class Point():
    def __init__(self, x, y):
	self.x = x
	self.y = y

p = Point(1, 2)
print (p)
# Prints '<__main__.Point object at 0x91ebd0>'


Idiomatic:

class Point():
    def __init__(self, x, y):
	self.x = x
	self.y = y

    def __str__(self):
	return '{0}, {1}'.format(self.x, self.y)

p = Point(1, 2)
print (p)
# Prints '1, 2'


-----------------------
Context Managers

1) Use a `context manager` to ensure resources are properly managed

`context managers` (objects meant to be used with the `with` statement) can make 
resource management both safer and more explicit. 
The canonical example is file IO.

There are a number of classes in the standard library that support or use a 
`context manager`. In addition, user defined classes can be easily made to work 
with a `context manager` by defining `__enter__` and `__exit__` methods.

Harmful:

file_handle = open(path_to_file, 'r')
for line in file_handle.readlines():
    if raise_exception(line):
	print('No! An Exception!')


Idiomatic:

with open(path_to_file, 'r') as file_handle:
    for line in file_handle:
	if raise_exception(line):
	    print('No! An Exception!')



-------------------------------
Generators

1) Prefer a `generator expression` to a `list comprehension` for simple iteration

When dealing with a `sequence`, it is common to need to iterate over a slightly 
modified version of the `sequence` a single time.

A `list comprehension` seems ideal, but there’s an even better Python built-in: 
a `generator expression`.

The main difference? A `list comprehension` generates a list object and fills in 
all of the elements immediately. For large lists, this can be prohibitively 
expensive.
The `generator` returned by a `generator expression`, on the other hand, 
generates each element “on-demand”.


Harmful:

for uppercase_name in [name.upper() for name in get_all_usernames()]:
    process_normalized_username(uppercase_name)


Idiomatic:

for uppercase_name in (name.upper() for name in get_all_usernames()):
    process_normalized_username(uppercase_name)


2) Use a `generator` to lazily load infinite sequences

Often, it’s useful to provide a way to iterate over a `sequence` that’s 
essentially infinite.
A `generator` is a special type of coroutine which returns an `iterable`.
The state of the `generator` is saved, so that the next call
into the `generator` continues where it left off.

Harmful:


