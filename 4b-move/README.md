# Move
In this chapter, we will learn about special situations in programming in which copying a value (which costs extra performance) can be replaced by moving a value instead.

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

```cpp
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

That's a lot of resources wasted!

## Simpler Sample
Above Code is quite complex and understanding exactly how `vector::push_back` works is not so easy, so here's a simpler code snippet to continue with:

```cpp
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

```cpp
    String zeusName{ "Zeus", 100 }; // string is constructed: Zeus
    Hero zeus{ zeusName }; // deep copy string: Zeus from zeusName -> name
```

Here, the `String` gets copied when it gets passed as an Argument to he Hero-Constructor

### Copy to Class Field

```cpp
    Hero(String name) :
	_name{ name } {  // deep copy string: Zeus from name -> _name

	} // String Zeus(name) gets deconstructed
```

Here, the `String`-Argument `name` gets copied when it gets assigned to the Hero's Field `_name`

When the Constructor ends, the `name` Argument is not used anymore and gets Destructed.

### No Copy to Constructor Argument

```cpp
Hero hercules{ String{ "Hercules", 100 } };
```

In the case of Hercules, no copy is created when the Hero-Constructor is invoked. This is because the Variable gets directly constructed at the right address of the argument. Cool!

### Copy to Class Field (Again)

```cpp
    Hero(String name) :
	_name{ name } {  // deep copy string: Hercules

	} // String Hercules gets deconstructed
```

But the problem within the Hero Constructor still exists.

## Use Reference For Constructor Argument
First of all, we should use a reference as a constructor Argument. There is no need that the argument gets cloned when passed as a constructor argument, if it gets cloned when being assigned to the member variable anyways:

```cpp
    Hero(const String& name) :
	_name{ name } {  // deep copy string: Hercules

	} // String Hercules gets deconstructed
```

```cpp
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
  - move the buffer from the argument to the field
  - set the buffer on the argument to null
- and then, when the argument gets destructed, it won't destroy the field's buffer

## Value Categories
There is many value types:
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

`lvalue`: a value with an identity, a variable. It usually can't be moved, because you might want to continue using the original value:

```cpp
String zeusName{"Zeus", 7}; // zeusName is an lvalue
Hero zeus{zeusName}; // the value can't be moved instead of copied...
zeusName.append("7"); // ... because we can still manipulate zeusName

zeusName = String{"Odysseus", 9}; // zeusName can stand on the left side of an assignment
String odysseusName = zeusName; // but also on the right side
```

### Prvalue

`prvalue`: a pure right value, which means, a value that can never stand on the left side of an assignment, e.g.:

```cpp
String{"Zeus", 7}; // this is a pr value

String zeusName = String{"Zeus", 7};
Hero{String{"Zeus", 7}};
String{"Zeus", 7} = zeusName; // not possible

5; // this one also is a pr value
5 = i; // also not possible
```

### XValue

`xvalue`: these are more problematic. These are values that can be moved, even though they have an identity. The Compiler can't be sure, whether the value can be moved or not, so we need to specify it:

```cpp
Hero zeus{};
{
	String zeusName{"Zeus", 7}; // we create a variable
	zeus = Hero{zeusName}; // and we only use want to pass it on to the Hero constructor
	// we know, that the value can be moved, because we don't use zeusName anymore after that
}
```

But the compiler can't know this on its own. But we can tell the compiler that the variable can be safely moved:

```cpp
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
```cpp
#include <cstdio>
void refType(int& x) {
	printf("lvalue reference %d\n", x);
}
```

### RValue Reference
```cpp
void refType(int&& x) {
	printf("rvalue reference %d\n", x);
}
```

### Sample Code:
```cpp
int main() {
	auto x = 1;
	refType(x); // lvalue
	refType(2); // rvalue
	refType(x+2); // rvalue
}
```

## std::move
`std::move` allows you to cast an lvalue type to rvalue type

```cpp
#include <utility>
/*...*/
refType(std::move(x));
```

You should not use moved-from objects except to reassign or destruct them.

We can use this knowledge to improve our Hero Constructor. We add a new RValue Reference Constructor:
```cpp
    Hero(String&& name) :
	_name{ std::move(name) } {  // tell the compiler, that `name` can safely be moved
	} // String (empty) gets deconstructed
```

### Move Constructor

But we also need to define a Move Constructor in our String class. Else, the compiler won't know, how to move our class and create copies instead:

```cpp
String(String&& other) noexcept { // noexcept is necessary, because else the compiler will prefer using the String& constructor
	printf("Moving %s\n", other.buffer);
	// move all arguments from the other string to this string
	// set the arguments to null on the other string
	// to ensure that the other string won't delete our buffer when it gets destructed
}
```

Don't forget to add null checks in the String's destructor now:

```cpp
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

```cpp
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

```cpp
	String zeus{"Zeus", 7};
	String hercules{"Hercules", 7}; // here, hercules gets constructed
	// moves value from zeus to hercules using move assignment operator:
	hercules = std::move(zeus);
	// should not use value of zeus anymore!

```

This is how to:
```cpp
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

```cpp
String(String&& other) noexcept = default; // explicitly ask for default generated method
String(String& other) = delete; // explicitly remove generation
```
