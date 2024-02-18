# 3 Classes and Files

```c++
// included class header because value is used in header
#include "Level.h"
// forward-declarred class because only a pointer is used in header
class Item;

class Player {
    int health;
    Item* item;
    Level leve;
public:
    // method inlined in header
    int getHealth() const { return health; }
    // method declaration, definition in cpp file
    void attack(Player& other);
    // method inlined in header, outside class (see below)
    void setHealth(int value);
};

inline void Player::setHealth(int value) {
    health = value;
}
```

<img width="270" alt="image" src="https://user-images.githubusercontent.com/7360266/226562162-90e9afce-3b48-4cf0-850e-8000339f0afc.png">


There is multiple ways of dealing with classes and Files.
- Generally, each class should be in its own `.h` and `.cpp` Files

## Version 1: Everything in header file
Functions will be inlined.
- the function that's defined in the header
- will have no code generated within the `.obj` file
- and instead have the code included in whatever file the method is used
- advantage: high performance
- disadvantage: a lot of duplicate code in the binary
```c++
// Circle.h
class Circle{
	float radius;
public:
	Circle() : radius{0} {}
	Circle(float r) : radius{r} {}
	float GetRadius() const {return radius;}
};
```

## Version 2: Definition in Source File
Functions and data member declaration in header
- function definition in source file
- source file must include matching header file
  - but can use short names inside the function code

```c++
// Circle.h
class Circle{
	float radius;
public:
	Circle();
	Circle(float r);
	float GetRadius() const;
};
```

```c++
// Circle.cpp
#include "circle.h"
Circle::Circle() {
	radius = 0;
}

Circle::Circle(float r) : radius{r}{}

float Circle::GetRadius() const {
	return radius;
}
```

## Version 3: Inlined outside class body
Functions inlined, but defined outside of the class body
- matter of taste: keeps the class body "clean"
- must use `inline` keyword
- must use fully qualified function names

```c++
// Circle.h
class Circle{
	float radius;
	public:
	Circle();
	Circle(float r);
	float GetRadius() const;
};

inline Circle::Circle() : radius{0} {}
inline Circle::Circle(float r) : radius{r} {}
inline float GetRadius() const { return radius; }
```

## Version 4: Inline Files
Put definition into inline files (e.g. `*.inl` there is no standard defined)
- keeps header completely clean
- header must include inline file

```c++
// Circle.h
class Circle{
	float radius;
	public:
	Circle();
	Circle(float r);
	float GetRadius() const;
};

#include "circle.inl"
```

```c++
// Circle.inl
inline Circle::Circle() : radius{0} {}
inline Circle::Circle(float r) : radius{r} {}
inline float GetRadius() const { return radius; }
```

## Version 5: Mix and Mingle!
- can and should mix all versions
- inline files are a matter of taste
- small functions should be inlined
- larger functions should go to the cpp file


## Class Forward Declarations
- classes can be forward declared
  - just like functions
- sometimes necessary when two classes depend on each other
  - because bi-directional include doesn't work!

```c++
#include "Mouse.h"

class Cat {
	void Catch(Mouse mouse);
};
```

```c++
#include "Cat.h"

class Mouse {
	void FleeFrom(Cat cat);
};
```

### Solution:
- forward declaration: `class ClassName;`
- sufficient to declare pointers or references to this class
- not sufficient if you need to use value members
  - because byte layout of the type is not known, then

```c++
class Mouse;

class Cat{
	void Catch(Mouse mouse);
};
```

### Rule of Thumb
Try to avoid includes in header files
- include structure grows exponentially
- increases compile time
- makes dependency structure unclear

Use Forward Declarations whenever possible
- unless classes need to use other classes as value members

## Friend Classes
Grants friend class access to private members
- use it scarcely (circumvents encapsulation)

```c++
class A{
	friend class B;
	void privateFunction(){}
};
class B{
	void test(A& a){
		a.privateFunction(); // can access private member here!
	}
}
```

## Friend Functions
Same as above, but with functions
- Useful e.g. for implementing operators

```c++
class Cash {
    int euros;
    friend operator*(float k, Cash cash);
    
public:
    Cash(int euros) : euros{euros} {}
};

Cash operator*(float k, Cash cash){
    return Cash{cash.euros*k}
}
```

# Quiz

What is the primary advantage of Version 1 (Everything in header file) when dealing with classes and files in C++?
- A) It reduces compile time by avoiding the need for separate header and source files.
- B) It allows for cleaner separation of interface and implementation.
- C) It improves performance by inlining functions directly into the calling code.
- D) It prevents duplicate code in the binary by generating code only once.

What are disadvantages of Version 1 (Everything in header file) (2)?
- A) It increases compile time due to duplicate code generation.
- B) It leads to poor encapsulation of class members.
- C) It makes it difficult to understand the interface of a class.
- D) It results in a larger binary size due to inlined functions.

In Version 2 (Definition in Source File), where are the function definitions of a class typically placed?
- A) In the header file along with the function declarations.
- B) In a separate source file that includes the header file.
- C) In an inline file with the extension .inl.
- D) In a separate header file with the extension .h.

What are advantages of Version 3 (Inlined outside class body) when defining functions in C++ classes (2)?
- A) It allows for cleaner separation of interface and implementation.
- B) It improves performance by inlining functions while keeping the class body clean.
- C) It prevents code duplication in the binary by generating code only once.
- D) It reduces compile time by avoiding the need for separate header and source files.

How can you prevent header files from becoming cluttered with function definitions in C++?
- A) By using forward declarations of classes.
- B) By including inline files with function definitions.
- C) By using friend classes to access private members.
- D) By defining functions directly within the class body.

What is the primary purpose of using forward declarations in C++ classes?
- A) To improve performance by reducing the size of header files.
- B) To prevent circular dependencies between classes.
- C) To allow access to private members from other classes.
- D) To inline function definitions directly into the calling code.

When should you use friend classes in C++?
- A) When you want to improve encapsulation by restricting access to private members.
- B) When you need to prevent circular dependencies between classes.
- C) When you want to grant access to private members to a specific class.
- D) When you want to define operators outside of a class definition.

What is the primary use case for friend functions in C++?
- A) To prevent circular dependencies between classes.
- B) To improve encapsulation by restricting access to private members.
- C) To allow non-member functions to access private members of a class.
- D) To provide access to protected members from derived classes.

Which version of organizing classes and files is recommended for small functions?
- A) Version 1: Everything in header file
- B) Version 2: Definition in Source File
- C) Version 3: Inlined outside class body
- D) Version 4: Inline Files

What is the rule of thumb for including header files in C++ (2 answers)?
- A) Include all necessary header files in the main source file to improve performance.
- B) Use forward declarations whenever possible to avoid circular dependencies.
- C) Include header files directly within other header files to reduce compile time.
- D) Avoid including header files in other header files to prevent exponential growth in include structure.

# Exercise
- Move your `String` function implementations into the cpp file
- Feel free to mix & match and leave some functions inline inside and outside the class, too