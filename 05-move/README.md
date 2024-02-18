# 5 Move

```c++
class Enemy {
    Item* item;

    // define a move constructor in which we move the item from other to us
    Enemy(const Enemy&& other) noexcept : Item{std::move{other.item}} { }
    
    // define a copy assignment operator
    Enemy& operator=(const Enemy&& other) {
        if (this == &other) return *this; // performance benefit if `a = a`
        delete item; // delete our old item
        item = std::move{other.item} // now move the other's item to us
        return *this;
    }
};
```

```c++
Enemy registeredEnemy;
// make a function that accepts a rvalue reference enemy
void registerSpawnedEnemy(Enemy&& enemy) {
    // move the enemy instead of copying them
   registeredEnemy = std::move(enemy);
}
```

In this chapter, we will learn about special situations in programming in which copying a value (which may consume a lot of resources in case of deep copies) can be avoided by moving a value instead.

We will learn:
- when the compiler can find out on its own, that a value can be moved
- how we can tell the compiler manually, that a value can be safely moved
- how we add support for moving values to our classes

## Move Semantics
- copying costs a lot of resources (CPU and RAM)
- sometimes, you only want to move an object from `a` to `b`
- moving an object means:
  - moving all the information from `a` to `b`
  - to further use `b`
  - and not use `a` anymore

## An extreme example

```c++
int main() {
	std::vector<String> heroes;
	heroes.push_back(String{ 100,"Hercules" });
	heroes.push_back(String{ 100,"Odysseus" });
	String prometheus{100,"Prometheus"};
	heroes.push_back{prometheus}; // copy
	return 0;
}
```

Output:
```
String(int, "Hercules")
Deep Copy String(const String& "Hercules")
~String:"Hercules"
String(int, "Odysseus")
Deep Copy String(const String& "Odysseus")
Deep Copy String(const String& "Hercules")
~String:"Hercules"
~String:"Odysseus"
String(int, "Prometheus")
Deep Copy String(const String& "Prometheus")
Deep Copy String(const String& "Hercules")
Deep Copy String(const String& "Odysseus")
~String:"Hercules"
~String:"Odysseus"
~String:"Prometheus"
~String:"Hercules"
~String:"Odysseus"
~String:"Prometheus"
```

We are creating:
- 4 Hercules instances
- 3 Odysseus instances
- 2 Prometheus instances

That's a lot of resources wasted!
- We only needed one copy of each

## A simpler example
Above Code is quite complex and understanding exactly how `vector::push_back` works is not so easy, so here's a simpler code snippet to continue with:

```c++
class Hero {
    String _name;
public:
    Hero(String name) : _name{ name } { }
};

int main() {
    String zeusName{ "Zeus", 100 };
    Hero zeus{ zeusName };
    Hero hercules{ String{ "Hercules", 100 } };
    return 0;
}
```

Output:
```
String(): Zeus
deep copy: Zeus
deep copy: Zeus
~String(): Zeus
String(): Hercules
deep copy: Hercules
~String(): Hercules
~String(): Hercules
~String(): Zeus
~String(): Zeus
```

We have multiple issues in above code:

### Copy to Constructor Argument

```c++
    String zeusName{ "Zeus", 100 }; // string is constructed: Zeus
    Hero zeus{ zeusName }; // deep copy string: Zeus from zeusName -> name
```

Here, the `String` gets copied when it gets passed as an Argument to he Hero-Constructor

### Copy to Class Data Member

```c++
    Hero(String name) :
	    _name{ name } {  // deep copy string: Zeus from name -> _name

	} // String Zeus(name) gets deconstructed
```

Here, the `String`-Argument `name` gets copied when it gets assigned to the Hero's Member Variable `_name`

When the Constructor ends, the `name` Argument is not used anymore and gets Destructed.

### No Copy to Constructor Argument

```c++
Hero hercules{ String{ "Hercules", 100 } };
```

In the case of Hercules, no copy is created when the Hero-Constructor is invoked. This is because the Variable gets directly constructed at the right address of the argument. Cool!

### Copy to Class Member Variable (Again)

```c++
    Hero(String name) :
	_name{ name } {  // deep copy string: Hercules

	} // String Hercules gets deconstructed
```

But the problem within the Hero Constructor still exists.



## Use Reference For Constructor Argument
First of all, we should use a reference as a constructor Argument.
- There is no need that the argument gets cloned when passed as a constructor argument
- if it gets cloned when being assigned to the member variable anyways:

```c++
    Hero(const String& name) :
	_name{ name } {  // deep copy string: Hercules

	} // String Hercules gets deconstructed
```

```c++
    String zeusName{ "Zeus", 100 }; // string is constructed: Zeus
    Hero zeus{ zeusName }; // zeusName is passed as a reference, no more copy!
```

Output:
```
String(): Zeus
// --------- FIXED: deep copy: Zeus
deep copy: Zeus
// --------- FIXED: ~String(): Zeus
String(): Hercules
deep copy: Hercules
~String(): Hercules
~String(): Hercules
~String(): Zeus
~String(): Zeus
```

## Move Instead of Copy
Idea: Within the Hero's Constructor, we know, that the `name` value is used for nothing else but for assigning it to `_name`, so how about we use that knowledge and tell c++ to:
- not create a deep copy of the String and its Buffer
- but instead:
  - move the buffer from the argument to the member variable
  - set the buffer on the argument to null
- and then, when the argument gets destructed, it won't destroy the member variable's buffer

## Value Categories
There are many value types:
```
     expression
     /        \
  glvalue   rvalue
  /     \  /     \
lvalue xvalue prvalue
```

Generally, you can group the values by:
- do they have an identity? i.e. a variable name?
- are they movable

### LValue

`lvalue`: a value with an identity, a variable
- Named so, because it can stand on the left side of an assignment
- Usually can't be moved
- because you might want to continue using the original value:

```c++
String zeusName{"Zeus", 7}; // zeusName is an lvalue
Hero zeus{zeusName}; // the value can't be moved instead of copied...
zeusName.append("7"); // ... because we can still manipulate zeusName

zeusName = String{"Odysseus", 9}; // zeusName can stand on the left side of an assignment
String odysseusName = zeusName; // but also on the right side
```

### Prvalue

`prvalue`: a pure right value, which means, a value that can never stand on the left side of an assignment, e.g.:

```c++
String{"Zeus", 7}; // this is a pr value

String zeusName = String{"Zeus", 7};
Hero{String{"Zeus", 7}};
// String{"Zeus", 7} = zeusName; // not possible

5; // this one also is a pr value
5 = i; // also not possible
```

### XValue

`xvalue`: these are more problematic
- These are values that can be moved, even though they have an identity
- The Compiler can't be sure, whether the value can be moved or not
- so we need to specify it:

```c++
Hero zeus{};
{
	String zeusName{"Zeus", 7}; // we create a variable
	zeus = Hero{zeusName}; // and we only use want to pass it on to the Hero constructor
	// we know, that the value can be moved, because we don't use zeusName anymore after that
}
```

But the compiler can't know this on its own. But we can tell the compiler that the variable can be safely moved:

```c++
Hero zeus{};
{
	String zeusName{"Zeus", 7}; // we create a variable
	zeus = Hero{std::move(zeusName)}; // and we only use want to pass it on to the Hero constructor
	// we know, that the value can be moved, because we don't use zeusName anymore after that
}
```

## References
When we define reference arguments, there is two ways of defining them:

### LValue Reference
```c++
#include <iostream>
void refType(int& x) {
	cout << "lvalue reference " << x << "\n";
}
```

### RValue Reference
```c++
void refType(int&& x) {
	cout << "rvalue reference " << x << "\n";
}
```

### Sample Code:
```c++
int main() {
	auto x = 1;
	refType(x); // lvalue
	refType(2); // rvalue
	refType(x+2); // rvalue
}
```

## std::move
`std::move` allows you to cast an lvalue type to rvalue type

```c++
#include <utility>
/*...*/
refType(std::move(x));
```

You should not use moved-from objects except to reassign or destruct them.

We can use this knowledge to improve our Hero Constructor. We add a new RValue Reference Constructor:
```c++
    Hero(String&& name) noxexcept :
	    _name{ std::move(name) } {  // tell the compiler, that `name` can be safely moved
	} // String (empty) gets deconstructed
```

### Move Constructor

But we also need to define a Move Constructor in our String class. Else, the compiler won't know, how to move our class and create copies instead:

```c++
String(String&& other) noexcept { // noexcept is necessary, because else the compiler will prefer using the String& constructor
	printf("Moving %s\n", other.buffer);
	// move all arguments from the other string to this string
	// set the arguments to null on the other string
	// to ensure that the other string won't delete our buffer when it gets destructed
}
```

Don't forget to add null checks in the String's destructor now:

```c++
~String(){
	// ...
	if(buffer) delete[] buffer;
	// ...
}
```

Result:
```
String(): Zeus
// --------- FIXED: deep copy: Zeus
deep copy: Zeus
// --------- FIXED: ~String(): Zeus
String(): Hercules
move: Hercules  // -------- FIXED: deep copy: hercules
~String(): null // -------- FIXED: ~String(): Hercules
~String(): Hercules
~String(): Zeus
~String(): Zeus
```

Great, this fixed the issue for String Hercules. This is, because it is an RValue
- it has no identity, no variable name.

But Zeus is an LValue
- it has an identity, a variable name `zeusName`

But we can convert it using `std::move`:

```c++
int main() {
	// we know, that we don't use zeusName after the constructor anymore.
    String zeusName{ "Zeus", 100 };
	// therefore, we can tell the compiler that it's safe to move the value
    Hero zeus{ std::move(zeusName) };
    Hero hercules{ String{ "Hercules", 100 } };
    return 0;
}
```

Result:
```
String(): Zeus
// --------- FIXED: deep copy: Zeus
move: Zeus --------- FIXED: deep copy: Zeus
// --------- FIXED: ~String(): Zeus
String(): Hercules
move: Hercules  // -------- FIXED: deep copy: hercules
~String(): null // -------- FIXED: ~String(): Hercules
~String(): Hercules
~String(): Zeus
~String(): null // -------- FIXED: ~String(): Zeus
```

## Move Assignment
Furthermore, you should define the move assignment operator, for cases like the following:

```c++
	String zeus{"Zeus", 7};
	String hercules{"Hercules", 7}; // here, hercules gets constructed
	// moves value from zeus to hercules using move assignment operator:
	hercules = std::move(zeus);
	// should not use value of zeus anymore!

```

This is how to:
```c++
String& operator=(String&& other) noexcept { //noexcept is needed again
	if(this == other) return *this;
	// clean up own values
	// assign other values to own values
	// set other values to null
	return *this;
}
```

## Compiler-Generated Methods
- Destructor
- Copy Constructor
- Move Constructor
- Copy Assignment Operator
- Move Assignment Operator

If you don't define any of these
- then all of these will be generated by the compiler. 

If you define any of these
- only some of the other methods will be generated by the compiler.

You can explicitly ask the compiler to generate some of these methods (or not):

```c++
String(String&& other) noexcept = default; // explicitly ask for default generated method
String(String& other) = delete; // explicitly remove generation
```


# Quiz

What is the purpose of move semantics in C++?
- A) To copy values efficiently
- B) To move values instead of copying, saving resources
- C) To ensure deep copying of values
- D) To prevent copying of values

Which type of value can stand on the left side of an assignment and usually cannot be moved?
- A) LValue
- B) RValue
- C) Prvalue
- D) XValue

What does `std::move` allow you to do in C++?
- A) Create a new instance of a value
- B) Prevent copying of values
- C) Move an LValue to an RValue
- D) Convert an LValue to an RValue, allowing it to be moved

When defining a move constructor or move assignment operator, why is `noexcept` often used?
- A) To indicate that the operation may throw exceptions
- B) To ensure that the operation does not throw exceptions
- C) To explicitly allow exceptions to be thrown
- D) To prevent the compiler from generating default methods

What happens if you don't define any of the special member functions (destructor, copy constructor, move constructor, copy assignment operator, move assignment operator) in a C++ class?
- A) All of these will be implicitly generated by the compiler
- B) None of these will be generated by the compiler
- C) Only the destructor will be generated by the compiler
- D) Only the copy constructor and copy assignment operator will be generated by the compiler

# Exercises
- Implement Move Semantics into your `String` class