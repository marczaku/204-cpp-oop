# 4 Copy and Move

Whenever we copy a value from a to b
- an exact clone of the object is created
  - but this sometimes requires some extra manual work, as we will see
- this costs extra cpu-power and memory

We can pass on pointers and references to avoid copies
- but then, the memory is either limited to the current function scope
- or the object needs to be allocated (and de-allocated) on the heap
  - which costs extra performance and requires extra responsibility for freeing objects

We will learn, how to correctly
- create clones of objects when they are copied
- avoid objects being cloned by "moving" them instead

## Copy Semantics
After copying x to y they are
- equivalent: x == y
- independent: modifying x does not cause a modification of y

### Copying Values

```cpp
int foo(int input){
	int result = input+1; // copying (input+1) into result
	return result;
}

int a{5}; // initializing a
int b = a; // Copy Constructor: a into b
int c{a}; // Copy Constructor: a into c
int d = foo(a); // Copy Constructor: a into input; Copy Constructor: result into d
int e{3};
e = a; // Copy Assignment: a into e
```

### Copying Base Data Types
No surprises here, we know, how this works:
```cpp
int increase(int number){
	number++;
	return number;
}

int main() {
	int original{1};
	int result = increase(original); // copy original to number & return value to result
	printf("original: %d, result: %d", original, result); // copy original and result to function
}
```

### Copying PODs
Member-wise copy
- each member value is copied into the parameter
- effectively it's a bitwise copy from one address to another

```cpp
struct Vector2 {
	int x,y;
};

Vector2 increase(Vector2 p){
	p.x++;
	p.y++;
	return p;
}

int main() {
	Vector2 original{3, 5};
	Vector2 result = increase(original); // copies all 8 bytes (x and y) to p and the return value back to result
	printf("original: %d, result: %d", original.x, result.x);
}
```

### Copying Fully-Featured Classes
- what happens, when your String class gets copied?
- what happens, when one instance gets deallocated?

```cpp
int main() {
	String a{"Hello", 7};
	a.Print(); // a is fine
	{
		String b = a;
		a.Print(); // a is still fine
	}
	a.Print(); // now, a is broken!! :o
}
```

PROBLEM
- we copy a from b
  - this will copy length, maxSize and the pointer to the buffer from a to b
- the scope of b is left
  - b gets deconstructed
    - which causes b to destroy its buffer
	- which points to the same address on the heap as a's buffer
  - and deallocated
- now, we print a
  - but its buffer got deleted by b

### Copy Constructor
To fix this, we need more control over what happens, when one String gets copied to another.

```cpp
String(const String& other){
	/*...*/
}
```

Invoked When:
- copying into an uninitialized object

```cpp
String a{"Hello", 7};
String b = a; // copy constructor
```

#### Two ways of copying objects:
Shallow Copy:
- copy pointer address
- meaning, that both objects will reference the SAME address

This is needed, if e.g. you want to clone an Enemy which has a reference to the Player. You don't want to clone the player, then, right?

```cpp
Player* target;

Enemy(const Enemy& other){
	// shallow copy, don't want to clone the player as well:
	this.target = other.target;
}
```

Deep Copy:
- look up object at pointer
- allocate `new` object
- assign new object pointer
- copy data from old object to new

This is needed e.g. when each unit has a Weapon and you want both the copied Unit to have a copy of the item, not share the same instance (An Item can't be in two places at the same time, right?)

```cpp
Item* item;

Enemy(const Enemy& other){
	// deep copy. We create a new Item on the HEAP and copy the other item into it:
	this.item = new Item{other.item};
}
```

### Copy Assignment
Unfortunately, that's not all. There's also a Copy Assignment Operator:

```cpp
String& operator=(const String& other) {
	if (this == &other) return *this; // performance benefit if `a = a`
	// first, clean up this object, e.g. delete Items, Buffers, etc.
	// then, clone the other object, e.g. copy their Items, Buffers etc.
	return *this;
}
```

Invoked When:
- copying into initialized object

```cpp
String a{"Hello", 7};
String b{"World", 9}; // b is initialized
b = a; // Copy Assignment, because b has already been initialized
```

This time, you need to ensure to clean up all objects that you deep copied:

```cpp
Item* item;

Enemy& operator=(const Enemy& other) {
	if (this == &other) return *this; // performance benefit if `a = a`
	delete this.item; // delete the item that we had before copying the other object into us
	this.item = new Item{other.item}; // now, copy the other item and use it
}
```

But beware, this doesn't go for shallow copied objects:

```cpp
Player* target;

Enemy& operator=(const Enemy& other) {
	if (this == &other) return *this; // performance benefit if `a = a`
	// delete this.target; DO NOT DO THIS, THIS WOULD DELETE THE PLAYER!!
	this.target = other.target; // shallow copy the other enemy's target:
}
```

### Default Copy
Why does the following code work, even though we haven't defined a copy constructor or assignment operator?
```cpp
struct Vector2{
	int x,y;
};

int main(){
	Vector2 a{2, 3};
	Vector2 b = a; // copy constructor
	b = Vector2{4, 5}; // coppy assignment
}
```

The compiler generates default implementations for copy construction anc copy assignment
- but that can be unintended

To make your intention obvious
- explicitly define that you want to use default implementations

```cpp
struct Vector2{
	int x,y;
	Vector2(const Vector2&) = default;
	Vector& operator=(const Vector&) = default;
}
```

### No Copy
Some classes, you never want to be copied
- e.g. a GameGrid which takes up a lot of Memory
- mark the copy constructor and assignment operator as `delete`

```cpp
struct GameGrid {
	GameGrid(const GameGrid&) = delete;
	GameGrid& operator=(const GameGrid&) = delete;
}
```

It will generate compile errors when trying to copy your class:

```cpp
GameGrid a;
GameGrid b = a; // Compile Error
```

### Copy Guidelines

#### Correctness
Ensure that no exceptions happen (e.g. double deletion of buffers)

#### Independence
Further changes to the copy should not affect the original and vice versa.

#### Equivalence
The original and the copy should have equal values.

## Move Semantics
- copying costs a lot of resources (CPU and RAM)
- sometimes, you only want to move an object from x to y:

```cpp
int main() {
	std::vector<String> heroes;
	heroes.push_back(String{ 100,"Hercules" });
	heroes.push_back(String{ 100,"Odysseus" });
	String prometheus{100,"Prometheus"};
	heroes.push_back{prometheus};
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

Let's look at an easier example to understand what's happening:

```cpp
struct Hero {
	Hero(const String& name) : name{ name } {

	}
	String name;
};

int main() {
	String name{100, "Hercules" };
	Hero hero{ name };
	return 0;
}
```

### Value Categories

#### Motivation
Find a set of rules that define, whether an object can be "moved" instead of "copied"

#### Background
What properties of values are independent?
- `has identity` i.e. an address, a pointer, the user can determine, whether two copies are identical
- `can be moved from`

#### Base Value Categories
Above determination allows 3+1 kinds of values:
- `iM` has identity and CANNOT be moved from
- `im` has identity and can be moved from
- `Im` DOESNT have identity and can be moved from
- (`IM` DOESNT have identity and CANNOT be moved from)

#### Summary

```
    i         m
  /     \  /     \
iM       im      Im
```

#### Naming
Old rules:
- Every `value` is either an `lvalue` or an `rvalue`
- An `lvalue` is not an `rvalue` and an `rvalue` is not an `lvalue`

`m` and `Im` used to be called `rvalues`
- the general movability, so `m` was named `rvalue`
- `Im` is a specific, pure kind of rvalues, so it became `prvalue`

`i` and `iM` used to be called `lvalues`
- moving didn't exist back then, so `iM` was named `lvalue`
- `i` is the generalization of lvalues, so it became `glvalue`

Which leaves only the bottom middle; values that
- have an identity
- and can be moved

There was no good name available, so it was named `xvalue`
- the unknown
- the strange
- the xpert only
- or even x-rated
- now: x-piring


```
     expression
     /        \
  glvalue   rvalue
  /     \  /     \
lvalue xvalue prvalue
```

### References

```cpp
#include <cstdio>
void refType(int& x) {
	printf("lvalue reference %d\n", x);
}
void refType(int&& x) {
	printf("rvalue reference %d\n", x);
}

int main() {
	auto x = 1;
	refType(x);
	refType(2);
	refType(x+2);
}
```

### std::move
`std::move` allows you to cast a lvalue type to rvalue type

```cpp
#include <utility>
/*...*/
refType(std::move(x));
```

You should not use moved-from objects except to reassign or destruct them.

### Move Constructor

```cpp
String(String&& other) noexcept {
	// assign other values to own values
	// set other values to null (to avoid destruction)
}
```

`noexcept` is very important, else the compiler might prefer using the copy constructor over the move constructor

### Move Assignment

```cpp
String& operator=(String&& other) noexcept {
	if(this == other) return *this;
	// clean up own values
	// assign other values to own values
	// set other values to null
	return *this;
}
```

### Fixing the Sample Code

```cpp
struct Hero {
	Hero(String&& name) : name{ std::move(name) } {

	}
	String name;
};
```

### Compiler-Generated Methods
- Destructor
- Copy Constructor
- Move Constructor
- Copy Assignment Operator
- Move Assignment Operator

If you don't define any of these, then all of these will be generated by the compiler. If you define any of these, only some of the other methods will be generated by the compiler.

You can explicitly ask the compiler to generate some of these methods (or not):

```cpp
String(String&& other) noexcept = default;
String(String& other) = delete;
```