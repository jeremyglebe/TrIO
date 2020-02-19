# Object Oriented Programming in TermIO

### Is TermIO object-oriented?
The answer is not straight forward. Does TermIO conform to every convention of
OOP design? The answer is no. However, the TermIO library does utilize several
key concepts of OOP in a way that may practically demonstrate their usage.

### Best Example: Commands
The class `Command` has several inherited types. This was necessary to enact
**polymorphic** behavior in the IO object. Let's break it down. 
The IO class contains several overloads of the `<<` operator.
These are its basic method of output and terminal interaction. Outputting text,
numbers, color, and even cursor movement is fairly straightforward. But for any
special commands (such as sleep) we really need a function call.
We could just add a function to the TermIO library, but part of what makes
TermIO unique is that the majority of its' operations follow the same format:
Stream an object to the IO object. So we need to make a polymorphic function
which can execute all of these commands. Therefore, we need them all to share
a parent class. One which has a method that executes the special functionality.

See the IO class's overloaded operator declarations below:
```cpp
class IO
{
    /* ... 
     * Code truncated for brevity
     * ... */
public:
    IO &operator<<(string text);
    IO &operator<<(const char &letter);
    IO &operator<<(const int &number);
    IO &operator<<(const double &number);

    IO &operator<<(const Point &point);
    IO &operator<<(const Color &color);
    /** HERE is our polymorphism. 
     * Executes a command object. */
    IO &operator<<(Command &command);
    /* ... 
     * Code truncated for brevity
     * ... */
};
```
To satisfy the parameters of our command execution method, we need to create
classes which inherit from `Command`. Let's look at a simplified version of
the `Sleep` command:
(we will remove references to "singletons" which is a more advanced topic)
```cpp
class Sleep : public Command
{
public:
    Sleep();
    // Every command needs a method call() which is used in our previous
    // function, operator<<()
    // This function basically just does whatever the command is expected to
    // (whatever we WOULD put in a function, if we were going that route)
    void call() override;
    // This special operator for Sleep lets us set its ms data member, giving
    // the illusion that Sleep is some kind of function.
    Sleep &operator()(int ms);

private:
    unsigned int ms;
};
// Create instance slp which can now be passed to the IO object for use
// ex.
// io << slp(10);
static Sleep &sleep = Sleep();
static Sleep &slp = Sleep();
```
So let's take a quick OOP analysis of what we just looked at. The attentive
reader may observe all four pillars of OOP in some form.
**Inheritance**: Pretty straightforward, `Sleep` inherits from `Command`.
**Encapsulation**: Each of our classes/components has a designated role to
which they adhere strictly. It is the purpose of the `IO` object to handle
interactions and customization of the terminal. Objects passed to `IO` handle
specification and parameters of `IO` behavior. And `Command` objects handle
non-terminal related behavior that still has an impact on
input/output elements. (sleep will delay output)
**Polymorphism**: Despite having a potentially arbitrary number of classes 
inheriting from command (in reality, it is only 2 as of the time of writing),
we only need to write a single overloaded operator to handle all commands. We
can assume that the objects all have shared properties that we can utilize
regardless of their individual definitions.
**Abstraction**: This is all about the user presentation. When someone uses
the TermIO library, they don't have to worry about how strange and difficult
the implementation of Sleep was. All the need to know is that it follows the
same stream-like format as the rest of TermIO, and that Sleep in particular
has familiar syntax because it imitates a function call. All of the complex
work is taken out of the hands of the user and handled behind the scenes.