# 3 Classes and Files

<img width="270" alt="image" src="https://user-images.githubusercontent.com/7360266/226562162-90e9afce-3b48-4cf0-850e-8000339f0afc.png">


There is multiple ways of dealing with classes and Files.
- Generally, each class should be in its own `.h` and `.cpp` Files

## Version 1: Everything in header file
Functions will be inlined.
- the function that's defined in the header
- will have no code generated within the `.obj` file
- and instead have the code included in whatever file the method is used
- advantage: high performance
- duplicate: a lot of duplicate code in the binary
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

Circle::Circl(float r) : radius{r}{}

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

class Cat{
	void Catch(Mouse mouse);
};
```

```c++
#include "Cat.h"

class Mouse{
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
```c++
class A{
	friend void foo();
	void privateFunction(){}
};

void foo(){
	A a;
	a.privateFunction();
}
```
