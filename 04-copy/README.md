# 4 Copy

```c++
class Enemy {
    Item* item;
    
    // define a copy constructor in which the item gets cloned
    Enemy(const Enemy& other) : Item{new Item{other.item}} { }
    
    // define a copy assignment operator
    Enemy& operator=(const Enemy& other) {
        if (this == &other) return *this; // performance benefit if `a = a`
        delete item; // delete our old item
        item = new Item{other.item}; // assign a copy of the other's item
        return *this;
    }
};
```

## Introduction

Whenever we pass a value from a to b
- an exact clone of the object should be created

The problem is: Some objects might contain references (pointers) to other objects
- e.g. the Monster carries an Item

```c++
class Monster {
   Item* item;
};

Monster original{new Item{}}; // Monster #1 carries Item #1
```

Now, if you copy the Monster, the Monster will be copied, but the Item will still be the same:

```c++
Monster clone = original; // Monster #2 carries Item #1 as well!
```

This can lead to problems, e.g. if Monster b gets destructed:

```c++
class Monster {
  ~Monster(){ delete item; } // Monster deletes its item on death
};

delete clone; // Monster #2 deletes Item #1
original.tryAttack(); // Monster #1's Item #1 has now been destroyed :(
```

What happened above is what's called a Shallow Copy
- That is, a copy which copies all Members
- But if those Members are References, they will still point to the same instance

What we need, is a Deep Copy
- That is, a copy which not only copies all Members
- But also clones all Members, if those are References (Pointer Types)

## Copy Semantics
After copying `a` to `b` they should be
- equivalent: `a == b`
- independent: modifying `a` does not cause a modification of `b`

## Copying Values
Values are constantly copied whenever you assign them to another variable or a function parameter:
```c++
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

## Copying Base Data Types
No surprises here, we know, how this works:
```c++
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

## Copying PODs
Member-wise copy
- each member value is copied into the parameter
- effectively it's a bitwise copy from one address to another

```c++
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

## Copying Fully-Featured Classes
- what happens, when your String class gets copied?
- what happens, when one instance gets deallocated?

```c++
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
- we construct a
  - which dynamically allocates a buffer at lets say address `#117 `
- we copy a to b
  - this will copy length, maxSize and the pointer to the buffer `#117` from a to b
  - a -> buffer `#117`
  - b -> buffer `#117`
- the scope of b is left
  - b gets deconstructed
    - which causes b to destroy its buffer `#117`
    - even though it is still used by a...
  - and deallocated
- now, we print a
  - but its buffer `#117` got deleted by b
  - so it prints some random text

### Copy Constructor
To fix this, we need more control over what happens, when one String gets copied to another.

```c++
String(const String& other){
	/*...*/
}
```

Invoked When:
- copying into an uninitialized object

```c++
String a{"Hello", 7};
String b = a; // copy constructor, because b gets constructed wit the value of a
```

There are two ways of copying members:

#### Shallow Copy
- copy pointer address
- meaning, that both objects will reference the SAME address

This is needed, if e.g. you want to clone an Enemy which has a reference to the Player. You don't want to clone the player, then, right?

```c++
Player* target;

Enemy(const Enemy& other){
	// shallow copy, don't want to clone the player as well:
	this.target = other.target;
}
```

#### Deep Copy
- look up object at pointer
- allocate `new` object
- assign new object pointer
- copy data from old object to new

This is needed e.g. when each unit has a Weapon and you want both the copied Unit to have a copy of the item, not share the same instance (An Item can't be in two places at the same time, right?)

```c++
Item* item;

Enemy(const Enemy& other){
	// deep copy. We create a new Item on the HEAP and copy the other item into it:
	this.item = new Item{other.item};
}
```

### Copy Assignment
Unfortunately, that's not all. There's also a Copy Assignment Operator:

```c++
String& operator=(const String& other) {
	if (this == &other) return *this; // performance benefit if `a = a`
	// first, clean up this object, e.g. delete existing Items, Buffers, etc.
	// then, clone the other object, e.g. copy their Items, Buffers etc.
	return *this;
}
```

Invoked When:
- copying into already initialized object

```c++
String a{"Hello", 7};
String b{"World", 9}; // b is initialized
b = a; // Copy Assignment, because b has already been initialized
```

#### Deep Copy

This time, you need to ensure to clean up all objects that you deep copied:

```c++
Item* item;

Enemy& operator=(const Enemy& other) {
	if (this == &other) return *this; // performance benefit if `a = a`
	delete this.item; // delete the item that we had before copying the other object into us
	this.item = new Item{other.item}; // now, copy the other item and use it
}
```

#### Shallow Copy

But beware, this doesn't go for shallow copied objects:

```c++
Player* target;

Enemy& operator=(const Enemy& other) {
	if (this == &other) return *this; // performance benefit if `a = a`
	// delete this.target; DO NOT DO THIS, THIS WOULD DELETE THE PLAYER!!
	this.target = other.target; // shallow copy the other enemy's target:
}
```

### Default Copy
Why does the following code work, even though we haven't defined a copy constructor or assignment operator?
```c++
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

```c++
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

```c++
struct GameGrid {
	GameGrid(const GameGrid&) = delete;
	GameGrid& operator=(const GameGrid&) = delete;
}
```

It will generate compile errors when trying to copy your class:

```c++
GameGrid a;
GameGrid b = a; // Compile Error
```

## Copy Guidelines

### Correctness
Ensure that no exceptions happen (e.g. double deletion of buffers)

### Independence
Further changes to the copy should not affect the original and vice versa.

### Equivalence
The original and the copy should have equal values.


# Quiz

What is the primary problem with shallow copying in C++ classes?
- A) It leads to excessive memory usage.
- B) It results in slower performance due to deep copying.
- C) It causes objects to share the same memory address.
- D) It prevents objects from being copied entirely.

What is the primary difference between shallow copy and deep copy in C++?
- A) Shallow copy copies the object's value, while deep copy copies the object's memory address.
- B) Shallow copy copies only the object's pointers, while deep copy clones all referenced objects.
- C) Shallow copy creates a new instance of the object, while deep copy updates the existing instance.
- D) Shallow copy is used for simple data types, while deep copy is used for complex data structures.

What are the requirements for copy semantics in C++?
- A) Objects should be identical but may be dependent.
- B) Objects should be equivalent and independent.
- C) Objects should share the same memory address.
- D) Objects should have different values but share resources.

How are values copied in C++ when passed to functions or assigned to variables?
- A) By creating a new instance of the value.
- B) By copying only the memory address.
- C) By using shallow copy semantics.
- D) By performing a bitwise copy of the value.

What happens when a Plain Old Data (POD) type is passed to a function in C++?
- A) The entire object is copied, including its memory address.
- B) Only the object's size is copied, not its contents.
- C) Each member value is copied into the function parameter.
- D) The object's pointer is copied, not its data.

What happens when a fully-featured class with dynamic memory allocation is copied in C++?
- A) The object's memory address is shared between the original and the copy.
- B) Both the original and the copy point to the same dynamically allocated memory.
- C) The copy constructor allocates new memory for each member of the copy.
- D) The copy constructor performs a bitwise copy of the original object.

What is the purpose of a copy constructor in C++?
- A) To initialize a new object using an existing object.
- B) To assign values from one object to another object.
- C) To delete the original object after copying its values.
- D) To compare two objects for equality. 

When is the copy constructor invoked in C++?
- A) When creating a new object without specifying initial values.
- B) When copying an uninitialized object to an initialized object.
- C) When copying an already initialized object to an uninitialized object.
- D) When deleting an object after its scope ends.

What is the primary purpose of the copy assignment operator in C++?
- A) To initialize a new object using an existing object.
- B) To assign values from one object to another object.
- C) To delete the original object after copying its values.
- D) To compare two objects for equality.

What happens if a class's copy constructor and copy assignment operator are not explicitly defined in C++?
- A) The compiler generates default implementations for both.
- B) The class cannot be copied to another object.
- C) The class automatically performs deep copying for all members.
- D) The class automatically performs shallow copying for all members.

# Exercise
- Implement Copy Semantics into your `String` class