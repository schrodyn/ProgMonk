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




