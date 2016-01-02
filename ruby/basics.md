everything is objects

interpretator (not compile)

assignment `=`

arithmetics: `+`, `-`, `*`, `/`, `**`

print methods: `print`, `puts`

strings:

  * methods: `.length`, `.reverse`, `.upcase`, `.downcase`, `.capitalize`


comments: `#`

  *	multiline: `=begin "..some stuff.." =end`

	example:
	```ruby
	 =begin
	  I'm a comment!
	  I don't need any # symbols.
	 =end
	#No space beetween = and "begin" !!!
    ```

we can make chains of functions:

```ruby
name.method1.method2.method3
```

example:

```rb
"FlYp".downcase.reverse.upcase
```

gets input from the user:

```rb
variable_name = gets.chomp
		=begin
		 gets getting input, Ruby automatically
		 adds a newline after each input
		 chomp removes that extra line
		=end
```

printing variables:
```rb
first_name = "Kevin"
puts "Your name is #{first_name}!"
```

formatting with string methods:
```rb
print "This is my question?"
answer = gets.chomp
answer2 = answer.capitalize
answer.capitalize!
```

`!` modifies the value contained within the variable {answer} itself
equal to answer = answer.capitalize

##if statement:

```rb
if 4 < 25
    puts "I will be winner!"
end
```

we need "end" construction

```rb
if 4 < 25
    puts "I will be winner!"
else
    puts "No worry! You are already the winner!"
end
```

```rb
x = 7
if 4 < x
    puts "I will be winner!"
elsif 4 > x
    puts "No worry! You are already the winner!"
else
    puts "Almost all done!"
end
```

##unless statement:
```rb
unless hugnry
  puts "I'm writing Ruby programs!"
else
  puts "Time to eat!"
end
```

```rb
problem = false
print "Good to go!" unless problem
```

```rb
x = 2
y = 2
if x == y
  print "x and y are equal!"
end
```

```rb
is_true = 2 != 3

is_false = 2 == 3

test_1 = 17 > 16

test_2 = 21 < 30

test_3 = 9 >= 9

test_4 = -11 < 4
```

You can also use logical or boolean operators. Ruby has three: and (`&&`), or (`||`), and not (`!`). Boolean operators result in boolean values: true or false.

if we want to check whether the user's input contains an "s":
```rb
if string_to_check.include? "substring"
```

We can do that using Ruby's `.include?` method, which evaluates to true if it finds what it's looking for and false otherwise.

When we find "s", we may want Ruby to replace every instance of "s" it finds with "th". We can do this with the `.gsub!` method, which stands for global substitution.

The syntax looks like this:

```rb
string_to_change.gsub!(/s/, "th")
```

Remember, you want the `!` at the end of the method name so that Ruby will change the string in-place.

```rb
print "Enter your string"
user_input = gets.chomp

user_input.downcase!

if user_input.include? "s"
    user_input.gsub!(/s/, "th")
    puts "Hello, #{user_input}!"
else
    print "No s strin!"
end
```

##while statement:

```rb
counter = 1
while counter < 11
  puts counter
  counter = counter + 1
end
```

```rb
i = 1
while i <= 50 do
    print i
    i += 1
end
```

##until statement:

```rb
counter = 1
until counter > 10
  puts counter
  counter += 1
end
```

##for statement:

```rb
for num in 1...10
  puts num
end
```

three dots "..." tells Ruby to exclude the final number in the count
if we use two dots ".." this tells Ruby to include the final number

##iterator:

```rb
i = 20
loop do
  i -= 1
  print "#{i}"
  break if i <= 0
end
```

##next keyword:

For instance, if we don't want to print out the even numbers, we can write:

```rb
for i in 1..5
  next if i % 2 == 0
  print i
end
```

##arrays:

```rb
my_array = [1, 2, 3, 4, 5]
```

##.each iterator:

A more useful iterator is the .each method, which can apply an expression to each element of an object, one at a time. The syntax looks like this:

```rb
object.each { |item| # Do something }
```

the way to loop
```rb
numbers.each { |item| puts item }
```

```rb
# another way to loop
numbers.each do |item|
  puts item
end
```

##For instance:

```rb
array = [1,2,3,4,5]

array.each do |x|
  x += 10
  print "#{x}"
end
```

another example:

```rb
numbers = [1, 2, 3, 4, 5]
```

one way to loop
```rb
numbers.each { |item| puts item }
```

another way to loop
```rb
numbers.each do |item|
  puts item
end
```

```rb
odds = [1,3,5,7,9]

odds.each {|x| print x*2}
```

##.times iterator:

The `.times` method is like a super compact for loop: it can perform a task on each item in an object a specified number of times.

For example, if we wanted to print out "Chunky bacon!" ten times, we might type

```rb
10.times { print "Chunky bacon!" }
```

##.split method

we want to divide the user's input into individual words.

Ruby has a built-in method for this called .split; it takes in a string and returns an array. If we pass it a bit of text in parentheses, .split will divide the string wherever it sees that bit of text, called a delimiter. For example,

```rb
text.split(",")
```

```rb
letters = ['a', 'b', 'c', 'd']
letters.each do |letter|
  print letter
end
```

##Access by Index:

```rb
array = [5, 7, 9, 2, 0]
array[2]
```
returns "9", since "9" is at index 2


##Array of arrays:

```rb
multi_d_rray = [[0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0]]
```

##Hashes:

```rb
my_hash = { "name" => "Eric",
  "age" => 26,
  "hungry?" => true
}

puts my_hash["name"]
puts my_hash["age"]
puts my_hash["hungry?"]
```

it's a literal notation
that because you literally describe what you want in the hash: you give it a name and you set it equal to a bunch of key => value pairs inside curly brackets iterating over.

You can also create a hash using Hash.new, like so:
```rb
my_hash = Hash.new
```

##Adding to a Hash:

```rb
pets = Hash.new
pets["Stevie"] = "cat"
```
Adds the key "Stevie" with the value "cat" to the hash

##Iterate hashes:

```rb
friends = ["Milhouse", "Ralph", "Nelson", "Otto"]

family = { "Homer" => "dad",
  "Marge" => "mom",
  "Lisa" => "sister",
  "Maggie" => "sister",
  "Abe" => "grandpa",
  "Santa's Little Helper" => "dog"
}

friends.each { |x| puts "#{x}" }
family.each { |x, y| puts "#{x}: #{y}" }
```

##Iterating over arrays:

```rb
numbers = [1, 2, 3, 4, 5]
numbers.each { |element| puts element }
```

##Iterating over multidimensional arrays:

let's assume `s = [["ham", "swiss"], ["turkey", "cheddar"], ["roast beef", "gruyere"]]`

if we want to access "swiss" we can type `s[0][1]`
if we want to iterate through all items of all elements we could type:

```rb
s.each{|sandwich| sandwich.each{|item| puts item} }
```

##Iterating over hashes:

When iterating over hashes, we need two placeholder variables to represent each key/value pair:

```rb
restaurant_menu = {
  "noodles" => 4,
  "soup" => 3,
  "salad" => 2
}

restaurant_menu.each do |item, price|
  puts "#{item}: #{price}"
end
```

hash has a default value

```rb
h = Hash.new("nothing here")
```

##sorting by:

```rb
colors = {"blue" => 3, "green" => 1, "red" => 2}
colors = colors.sort_by do |color, count|
  count
end
colors.reverse!
```

##converting to string

convert the value from a number to a string using `.to_s`.

##Methods in Ruby:

```rb
def welcome
  puts "Welcome to Ruby!"
end
```

P.S. we can also use it `(1..10).each { |i| puts i }`

```rb
def square(n)
  puts n ** 2
end

def double(n)
  return n * 2
end

output = double(6)
output += 2
```

#BLOCKS

Blocks Are Like Nameless Methods

Blocks can be defined with either the keywords do and end or with curly braces ({}).

Example:

```rb
1.times do
  puts "I'm a code block!"
end

1.times { puts "As am I!" }
```

```rb
# method that capitalizes a word
def capitalize(string)
  puts "#{string[0].upcase}#{string[1..-1]}"
end

capitalize("ryan") # prints "Ryan"
capitalize("jane") # prints "Jane"
```

```rb
[1, 2, 3, 4, 5].each { |i| puts 5*i }
```

##Sorting

```rb
my_array = [3, 4, 8, 7, 1, 6, 5, 9, 2]
my_array.sort!
```

##Comparison Operator

looks like this: `<=>`

It returns 0 if the first operand (item to be compared) equals the second, 1 if first operand is greater than the second, and -1 if the first operand is less than the second.

```rb
books = ["Charlie and the Chocolate Factory", "War and Peace", "Utopia", "A Brief History of Time", "A Wrinkle in Time"]

# To sort our books in ascending order, in-place
books.sort! { |firstBook, secondBook| firstBook <=> secondBook }

# Sort your books in descending order, in-place below
books.sort! {|firstBook, secondBook| secondBook <=> firstBook}
```

##OBJECT ID

The `.object_id` method gets the ID of an object—it's how Ruby knows whether two objects are the exact same object.

##SYMBOLS

`:s_name`

In Ruby, a string is mutable, whereas a symbol is immutable. That means that only one copy of a symbol needs to be created. Thus, if you have

```rb
x = :my_str
y = :my_str
```

:my_str will only be created once, and x and y point to the same area of memory.

```rb
sounds = {
  :cat => "meow",
  :dog => "woof",
  :computer => 10010110,
}
```

Symbols make good hash keys for a few reasons:

  1. They're immutable, meaning they can't be changed once they're created;
  2. Only one copy of any symbol exists at a given time, so they save memory;
  3. Symbol-as-keys are faster than strings-as-keys because of the above two reasons.

Converting Between Symbols and Strings:

```rb
:sasquatch.to_s
# ==> "sasquatch"

"sasquatch".to_sym
# ==> :sasquatch
```


```rb
strings = ["HTML", "CSS", "JavaScript", "Python", "Ruby"]

# Add your code below!
symbols = []
strings.each do |language|
    symbols.push(language.to_sym)
end
```


the hash syntax has changed in Ruby 1.9
The good news is that the new syntax is easier to type than the old hash rocket syntax :)

Example:
```rb
movies = {
  one: "Santa Barabara",
  two: "Monica in the Sun",
}
```

##SELECT FROM HASH

what if we want to filter a hash for values that meet certain criteria?
For that, we can use .select.

```rb
grades = { alice: 100,
  bob: 92,
  chris: 95,
  dave: 97
}

grades.select {|name, grade| grade < 97}
# ==> {:bob=>92, :chris=>95}

grades.select { |k, v| k == :alice }
# ==> {:alice=>100}

.select does not modify the hash.
```

Example 2

```rb
movie_ratings = {
  memento: 3,
  primer: 3.5,
  the_matrix: 5,
  truman_show: 4,
  red_dawn: 1.5,
  skyfall: 4,
  alex_cross: 2,
  uhf: 1,
  lion_king: 3.5
}
# Add your code below!

good_movies = movie_ratings.select {|name, rating| rating > 3}
```

Can we iterate over just keys or just values?

Of course yes.

Ruby includes two hash methods, .each_key and .each_value, that do exactly what you'd expect:

```rb
my_hash = { one: 1, two: 2, three: 3 }

my_hash.each_key { |k| print k, " " }
# ==> one two three

my_hash.each_value { |v| print v, " " }
# ==> 1 2 3
```

##CASE STATEMENT

```rb
case language
when "JS"
  puts "Websites!"
when "Python"
  puts "Science!"
when "Ruby"
  puts "Web apps!"
else
  puts "I don't know!"
end
```

##DELETE from Hash

to remove a movie/rating pair from our hash: we just write `movies.delete(title)!`

```rb
ruby_is_eloquent = true
puts "Ruby is eloquent!" if ruby_is_eloquent
```

##ternary conditional expression

```rb
puts 3 < 4 ? "3 is less than 4!" : "3 is not less than 4."
```

##When and Then Case statement

```rb
case language
  when "JS" then puts "Websites!"
  when "Python" then puts "Science!"
  when "Ruby" then puts "Web apps!"
  else puts "I don't know!"
end
```

##Conditional Assignment

```rb
favorite_book = nil
favorite_book ||= "Cat's Cradle"
```

Since the value of the variable was nothing before, Ruby goes ahead and sets it

JavaScript return *undefined* by default. For Python, the default return value is
None. But Ruby's methods will return the result of the last evaluated expression.

```rb
def add(a,b)
  return a + b
end

def add(a,b)
  a + b
end
```
Are the same


```rb
def a
  puts "A was evaluated!"
  return true
end

def b
  puts "B was also evaluated!"
  return true
end

puts a || b
puts "------"
puts a && b
```

```rb
my_array = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
my_array.each{ |x| puts x if x % 2 == 0}

95.upto(100) { |num| print num, " " }
# Prints 95 96 97 98 99 100
```

and we can use .downto to do the same thing with descending values.

```rb
'L'.upto('P') {|ch| puts ch}
```


`.respond_to?` takes a symbol and returns true if an object can receive that method
 and false otherwise.

```rb
[1, 2, 3].respond_to?(:push)
```

would return true, since you can call .push on an array object.

However, `[1, 2, 3].respond_to?(:to_sym)` would return false, since you can't turn
an array into a symbol.


to add an element to the end of an array:
you can use .push method or

```rb
[1, 2, 3] << 4
# ==> [1, 2, 3, 4]

"Yukihiro " << "Matsumoto"
# ==> "Yukihiro Matsumoto"
```

##Conditional assignment

```rb
favorite_animal = nil
favorite_animal ||= "Bear"
```

if it's false then assign

```rb
def square(n)
    n*n
end
```

The collect method takes a block and applies the expression in the block to every
element in an array.

```rb
my_nums = [1, 2, 3]
my_nums.collect { |num| num ** 2 }
# ==> [1, 4, 9]
```

If we look at the value of my_nums, though, we'll see it hasn't changed:

```rb
my_nums
# ==> [1, 2, 3]
```

This is because .collect returns a copy of my_nums
To overwrite my_nums
```rb
my_nums.collect! { |num| num ** 2 }
```

```rb
fibs = [1, 1, 2, 3, 5, 8, 13, 21, 34, 55]
doubled_fibs = fibs.collect {|x| x*2}
```

##yield

```rb
def double(n)
    yield 2*n
    yield 4*n
end

double(2) {|x| puts "2 is doubled and now #{x}!"}
```

##Procs

```rb
multiples_of_3 = Proc.new do |n|
  n % 3 == 0
end

(1..100).to_a.select(&multiples_of_3)
```

Procs are easy to define! You just call Proc.new and pass in the block you want to
save.

Here's how we'd create a proc called cube that cubes a number :
```rb
cube = Proc.new { |x| x ** 3 }

[1, 2, 3].collect!(&cube)
# ==> [1, 8, 27]

[4, 5, 6].map!(&cube)
# ==> [64, 125, 216]
```

The .collect! and .map! methods do the exact same thing.


The & is used to convert the cube proc into a block

```rb
floats = [1.2, 3.45, 0.91, 7.727, 11.42, 482.911]
round_down = Proc.new {|x| x.floor}
ints = floats.collect(&round_down)
```

The .floor method rounds a float down to the nearest integer.

we can call procs directly by using Ruby's .call method.
```rb
test = Proc.new { # does something }
test.call

hi = Proc.new {puts "Hello!"}
hi.call
```

```rb
strings = ["1", "2", "3"]
nums = strings.map(&:to_i)
# ==> [1, 2, 3]
```


#The Ruby Lambda

Like procs, lambdas are objects.

```rb
lambda { puts "Hello!" }
```

Is just about the same as

```rb
Proc.new { puts "Hello!" }


def lambda_demo(a_lambda)
  puts "I'm the method!"
  a_lambda.call
end

lambda_demo(lambda { puts "I'm the lambda!" })
```

Lambdas are defined using the following syntax:

```rb
lambda { |param| block }


strings = ["leonardo", "donatello", "raphael", "michaelangelo"]

symbolize = lambda {|s| s.to_sym}

symbols = strings.collect(&symbolize)
# => [:leonardo, :donatello, :raphael, :michaelangelo]
```

##Lambdas vs. Procs

There are only two main differences.
First, a lambda checks the number of arguments passed to it, while a proc does
not. This means that a lambda will throw an error if you pass it the wrong number
of arguments

Second, when a lambda returns, it passes control back to the calling method;
when a proc returns without going back to the calling method.


`.is_a?` method, which returns true if an object is the type of object named and
false otherwise:

```rb
:hello.is_a? Symbol
# ==> true

my_array = ["raindrops", :kettles, "whiskers", :mittens, :packages]

# Add your code below!
symbol_filter = lambda {|s| s.is_a? Symbol}
symbols = my_array.select(&symbol_filter)
```

##Quick Review

A block is just a bit of code between do..end or {}. It's not an object on its own
, but it can be passed to methods like .each or .select.

A proc is a saved block we can use over and over.

A lambda is just like a proc, only it cares about the number of arguments it gets
and it returns to its calling method rather than returning immediately.

```rb
crew = {
  captain: "Picard",
  first_officer: "Riker",
  lt_cdr: "Data",
  lt: "Worf",
  ensign: "Ro",
  counselor: "Troi",
  chief_engineer: "LaForge",
  doctor: "Crusher"
}
# Add your code below!
first_half = lambda {|key, value| value <= 'M'}
a_to_m = crew.select(&first_half)
```
