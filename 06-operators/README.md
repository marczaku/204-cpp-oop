# 6 Operators
Here, we will learn, how to define custom operators for your classes in C++.
- The advantage is, that it makes code a lot more readable. 

Instead of:

```c++
Vector position{12, 12, 12};
Vector right{1, 0, 0};

position = position.Add(right.MultiplyWith(2));
```

You can then write:
```c++
position += right * 2;
```

## Define an Operator
Operators resolve to function calls
- `ReturnType operator<sign>(Arguments)`

There is two ways of defining custom operators:
- class member operator
  - e.g. `Vector operator+(Vector other)`
- global operator
  - e.g. `Vector operator+(Vector a, Vector b)`

### Class Member Operator
Advantage: Convenient to implement
```c++
class Vector{
	float x, y, z;
public:
	Vector(float x, float y, float z) : x{x}, y{y}, z{z} {}
	Vector operator+ (const Vector& other) const {
		return Vector{x+other.x, y+other.y, z+other.z};
	}
};
```

### Global Operator
Advantage: The first parameter can be non-class type!
- or a class that you haven't written yourself
```c++
class Vector{
	float x,y,z;
	// often, the operator needs access to private members. Use `friend` keyword:
	friend Vector operator*(float, const Vector&);
}

Vector operator*(float f, const Vector& v){
	return Vector(v.x*f, v.y*f, v.z*f);
}
```

## Reference Operators
Some Operators change the class on which they are invoked. e.g. `+=`. In those cases, you can return a reference to the value itself using `this`

This is necessary due to chaining like here:

`(a+=b) += c;`

```c++
X& operator+=(const X& rhs)
{
	/* addition of rhs to *this takes place here */
	return *this; // return the result by reference
}
```

## Prefix and Postfix operators
How is `++x` and `x++` distinguished?

Prefix:
```c++
X& operator++()
{
	// actual increment takes place here
	return *this; // return new value by reference
}
```
 
Postfix:
```c++
// unnecessary int argument just used to distinguish one from another
X operator++(int)
{
	X old = *this; // copy old value
	operator++();  // prefix increment
	return old;    // return old value
}
```

## Comparison Operators
Often, one operator implementation can be used for the implementation of the others:

If you define:
```c++
inline bool operator< (const X& lhs, const X& rhs) { /* do actual comparison */ }
```

You can define the others as such:
```c++
inline bool operator> (const X& lhs, const X& rhs) { return rhs < lhs; }
inline bool operator<=(const X& lhs, const X& rhs) { return !(lhs > rhs); }
inline bool operator>=(const X& lhs, const X& rhs) { return !(lhs < rhs); }
```

Also, if you define:
```c++
inline bool operator==(const X& lhs, const X& rhs) { /* do actual comparison */ }
```
You can use it:
```c++
inline bool operator!=(const X& lhs, const X& rhs) { return !(lhs == rhs); }
```

Since, C++20 there is even a Three-Way Comparison Operator that can be be used to implicitly define all other comparison operators:

```c++
friend auto operator<=>(const X&, const X&) = default;
```

## Cast Operator
Can be used to implicitly convert one type to another:
```c++
class Timer{
	int time;
public:
	operator int() { return time; }
}
```

```c++
Timer timer;
int i = timer; // implicit cast through operator
```

### Automatic Type Conversion
The Compiler can perform implicit, automatic type conversions:
```c++
class Player {
    const char* name;
public:
    Player(const char* name) : name{ name } {}
};

int main() {
    const char* name = "Marc";
    Player player = name; // implicit conversion
}
```

This is often not desired and can be prevented by the `explicit` keyword:

```c++
explicit Player(const char* name) : name{ name } {}
```

This now fixes:
```c++
    Player player = name; // COMPILE ERROR
```

But allows using the constructor explicitly:
```c++
	Player player{name};
}
```

## List of Operators:
Unary Arithmetic:
- `+` `-` `++` `--`

Binary Arithmetic:
- `+` `-` `*` `/` `%`

Binary Bitwise:
- `^` `&` `|` `~`

Unary & Binary Logical:
- `!` `&&` `||`

Assignment:
- `=`

Comparison:
- `<` `>` `==` `!=` `<=` `>=` `<=>`

Compound:
- `+=` `-=` `*=` `/=` `%=` `^=` `&=` `|=`

Insertion and Extraction (Streaming):
- `<<` `>>` `>>=` `<<=`
- especially used for c++'s version of C#'s `ToString()`:

```c++
ostream& operator << (ostream &os, const Student &s) {
    return (os << "Name: " << s.name << "\n Age: " << s.age << "\n Final Grade: " << s.finalGrade  << std::endl);
}
```

```c++
Student student{"Marc", 32, 1};
std::cout << student;
```

Function Call:
`( )`
- allows you to use your class instance like a function:
  - `instance()`
  - `instance(3, 2)`

Array Subscript:
`[ ]`
- allows you to use your class like an array or dictionary:
  - `saveGame["Gold"] = 100`
  - `printf(weaknesses[Element::Water])`

Member Access Operator
`->`
- allows you to access a member of a class.
- imagine a class called `Box` that can contain an `Item`
- you could overload the operator, so you could do:

```c++
Box box{};
box.put(Item{}); // put item in the box
box.get().Use(); // you can now access the item through the box
box->Use(); // or directly through the overloaded operator
```

Member Access through Pointer Operator
`->*`
- works the same as above, but specifically, if you have a `Box*` variable.
  - not recommended to overload
  - because it makes normal member access on pointer types difficult

# Quiz

Which of the following is NOT an advantage of defining custom operators for classes in C++?
- a) Improved code readability
- b) Enhanced performance
- c) Increased convenience
- d) More natural syntax

How is a class member operator typically defined?
- a) As a static member function
- b) As a global function
- c) As a non-member function
- d) As a member function

What keyword is used to grant access to private members in a global operator function?
- a) `public`
- b) `protected`
- c) `private`
- d) `friend`

Which operator is used for reference assignment in C++?
- a) `=`
- b) `==`
- c) `+=`
- d) `*=`

In C++, how is the prefix increment operator implemented?
- a) `operator++(int)`
- b) `operator++()`
- c) `operator--()`
- d) `operator--(int)`

What is the purpose of the `friend` keyword in C++ class definitions?
- a) To define a class that does not throw exceptions
- b) To allow access to private members
- c) To declare a static member function
- d) To specify a base class

Which C++ standard introduced the Three-Way Comparison Operator?
- a) C++98
- b) C++11
- c) C++17
- d) C++20

What is the purpose of the `explicit` keyword in C++ constructors?
- a) To specify a default argument
- b) To allow implicit type conversion
- c) To prevent implicit type conversion
- d) To declare a virtual function

Which of the following is NOT a unary arithmetic operator in C++?
- a) `+`
- b) `-`
- c) `++`
- d) `*`

What is the purpose of the Member Access through Pointer Operator `->*` in C++?
- a) To access public members of a class through a pointer
- b) To access private members of a class through a pointer
- c) To access members of a class through a reference
- d) To access members of a class through a smart pointer


# Exercises

## Vector Class
- geometrical 3d vector
- vector addition `+`
- vector subtraction `-`
- dot product `*`
- scalar product `*`
  - both ways!
- scalar division `/`
- comparison `==` `!=`
- vector inversion `-` (unary)
- compound assignment `+=` `-=` `*=` `/=`
  - `*=` for both products
- constructors
- assignment operator `=`
- move constructor not necessary. Why?

# String Class
Add all operators that make sense.

For example, but not limited to:
- `+` operator to combine two `Strings` to a new `String`
- `+=` to append one `String` to another
- `[]` to access the `String`'s characters.
- `<<` for printing the `String` using `cout`
- Comparison Operators to find out, which `String` comes alphabetically first
  - e.g.: `bool a = String{"Hello"} < String{"World"};`
  - should return `true`, because `H` comes before `W`
