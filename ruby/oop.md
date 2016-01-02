Ruby is an object-oriented programming language
(Almost) everything in Ruby is an object!

Objects have methods and attributes, which are just data.


```rb
class Language
  def initialize(name, creator)
    @name = name
    @creator = creator
  end

  def description
    puts "I'm #{@name} and I was created by #{@creator}!"
  end
end

ruby = Language.new("Ruby", "Yukihiro Matsumoto")
python = Language.new("Python", "Guido van Rossum")
javascript = Language.new("JavaScript", "Brendan Eich")

ruby.description
python.description
javascript.description
```

```rb
class NewClass
  # Class magic here
end
```

By convention, class names start with a capital letter and use CamelCase instead
of relyingonunderscores.

In Ruby, we use `@` before a variable to signify that it's an instance variable.


```rb
class Person
    def initialize(name)
        @name = name
    end
end

matz = Person.new("Yukihiro")
```

The scope of a variable is the context in which it's visible to the program.

When dealing with classes, you can have variables that are available everywhere
(global variables)
ones that are only available certain methods (local variables),
others that are members of a certain class (class variables),
and variables that are only available to particular instances of a class
(instance variables).

Always start your instance variables with `@`.

Class variables always start with two `@s`, like so:

```rb
@@files
```

If you want to make a variable global from inside a method or class, just start it
 with a `$`, like so: `$matz`.

Because there's only one copy of a class variable shared by all instances of a
class, we can use them to pull off some cool Ruby tricks.
For example, we can use a class variable to keep track of the number of instances
of that class we've created.


Inheritance is the process by which one class takes on the attributes and methods
of another

We've defined a class, `ApplicationError`, as well as a `SuperBadError` class that
inherits from `ApplicationError`.

```rb
class ApplicationError
  def display_error
    puts "Error! Error!"
  end
end

class SuperBadError < ApplicationError
end

err = SuperBadError.new
err.display_error
```

In Ruby, inheritance works like this:

```rb
class DerivedClass < BaseClass
  # Some stuff!
end
```

We can override methods:

```rb
class Creature
  def initialize(name)
    @name = name
  end

  def fight
    return "Punch to the chops!"
  end
end

# Add your code below!
class Dragon < Creature
    def fight
        return "Breathes fire!"
    end
end
```


You can directly access the attributes or methods of a superclass with Ruby's
built-in super keyword.

```rb
class Creature
  def initialize(name)
    @name = name
  end

  def fight
    return "Punch to the chops!"
  end
end

# Add your code below!
class Dragon < Creature
    def fight
        puts "Instead of breathing fire..."
        super
    end
end
```

Any given Ruby class can have only one superclass.

```rb
class Message
    @@messages_sent = 0
    def initialize(from, to)
        @from = from
        @to = to
        @@messages_sent += 1
    end
end

class Email < Message
    def initialize(from, to)
        super(from, to)
    end
end

my_message = Message.new("192.168.1.24", "192.168.1.225")
```

```rb
class Computer
    @@users = Hash.new
    def initialize(username, password)
        @username = username
        @password = password
        @@users[username] = password
        @files = Hash.new
    end

    def create(filename)
        time = Time.now
        @files[filename] = time
        puts "#{filename} was created!"
    end

    def Computer.get_users
        return @@users
    end
end

my_computer = Computer.new("lol","_assembly_exploit_god")
```


Ruby allows you to explicitly make some methods public and others private.
Public methods allow for an interface with the rest of the program;

Methods are public by default in Ruby, so if you don't specify public or private,
your methods will be public.

```rb
class ClassName
  # Some class stuff

  public
  # Public methods go here
  def public_method; end

  private
  # Private methods go here
  def private_method; end
end
```

```rb
class Dog
    public
    def initialize(name, breed)
        @name = name
        @breed = breed
    end

    def bark
        puts "Woof!"
    end
    private
    def id
        @id_number = 12345
    end
end
```

We can use attr_reader to access a variable and attr_writer to change it.

```rb
class Person
  attr_reader :name
  attr_writer :name
  def initialize(name)
    @name = name
  end
end
```

Ruby does something like this for us automatically:

```rb
def name
  @name
end

def name=(value)
  @name = value
end
```

you're allowed to put an = sign in a method name. That's just a Ruby convention
saying, "hey, this method sets a value!"

##attr_accessor

If we want to both read and write a particular variable,  We can use attr_accessor

```rb
class Person
  attr_reader :name
  attr_accessor :job

  def initialize(name, job)
    @name = name
    @job = job
  end
end
```

##What's a Module?

You can think of a module as a toolbox that contains a set methods and constants.

You can think of modules as being very much like classes, only modules can't
create instances and can't have subclasses.

Modules are super easy to make! You just use the module keyword, like so:

```rb
module ModuleName
  # Bits 'n pieces
end
```

One of the main purposes of modules is to separate methods and constants into
named spaces. (This is called namespacing)

```rb
Math::PI
Circle::PI
```

That's called the scope resolution operator

```rb
require 'date'

puts Date.today
```

We can also include it!

you no longer have to prepend your constants and methods with the module name.
you can simply write PI instead of Math::PI.

##Mixin

When a module is used to mix additional behavior and information into a class,
it's called a mixin.

```rb
module Action
  def jump
    @distance = rand(4) + 2
    puts "I jumped forward #{@distance} feet!"
  end
end

class Rabbit
  include Action
  attr_reader :name
  def initialize(name)
    @name = name
  end
end

class Cricket
  include Action
  attr_reader :name
  def initialize(name)
    @name = name
  end
end

peter = Rabbit.new("Peter")
jiminy = Cricket.new("Jiminy")

peter.jump
jiminy.jump
```

This gives us the ability to mimic inheriting from more than one class


```rb
1_000_000
```
(one million). Ruby allows this, and it makes it easier to read big numbers! Cool, no?

```rb
def initialize(name, balance=100)
  @name = name
  @balance = balance
end
```

It's an optional parameter
