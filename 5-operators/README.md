# 5 Operators
Here, we will learn, how to define custom operators for your classes in C++.

The advantage is, that it makes code a lot more readable. Instead of:

```cpp
Vector position{12, 12, 12};
Vector right{1, 0, 0};

position = position.Add(right.MultiplyWith(2));
```

You can then write:
```cpp
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

```cpp
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
```cpp
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

```cpp
X& operator+=(const X& rhs)
{
	/* addition of rhs to *this takes place here */
	return *this; // return the result by reference
}
```

## Prefix and Postfix operators
How is `++x` and `x++` distinguished?

Prefix:
```cpp
X& operator++()
{
	// actual increment takes place here
	return *this; // return new value by reference
}
```
 
Postfix:
```cpp
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
```cpp
inline bool operator< (const X& lhs, const X& rhs) { /* do actual comparison */ }
```

You can define the others as such:
```cpp
inline bool operator> (const X& lhs, const X& rhs) { return rhs < lhs; }
inline bool operator<=(const X& lhs, const X& rhs) { return !(lhs > rhs); }
inline bool operator>=(const X& lhs, const X& rhs) { return !(lhs < rhs); }
```

Also, if you define:
```cpp
inline bool operator==(const X& lhs, const X& rhs) { /* do actual comparison */ }
```
You can use it:
```cpp
inline bool operator!=(const X& lhs, const X& rhs) { return !(lhs == rhs); }
```

Since, C++20 there is even a Three-Way Comparison Operator that can be be used to implicitly define all other comparison operators:

```cpp
friend auto operator<=>(const X&, const X&) = default;
```

## Cast Operator
Can be used to implicitly convert one type to another:
```cpp
class Timer{
	int time;
public:
	operator int() { return time; }
}
```

```cpp
Timer timer;
int  = timer; // implicit cast through operator
```

### Automatic Type Conversion
The Compiler can perform implicit, automatic type conversions:
```cpp
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

This is sometimes not desired and can be prevented by the `explicit` keyword:

```cpp
explicit Player(const char* name) : name{ name } {}
```
This now fixes:
```cpp
    Player player = name; // COMPILE ERROR
```

But allows using the constructor explicitly:
```cpp
	Player player{name};
}
```

## List of Operators:
Unary:
`+` `-` `++` `--`

Binary Arithmetic:
`+` `-` `*` `/` `%`

Binary Bitwise:
`^` `&` `|` `~`

Unary Logical:
`!` `&&` `||`

Assignment:
`=`

Comparison:
`<` `>` `==` `!=` `<=` `>=` `<=>`

Compound:
`+=` `-=` `*=` `/=` `%=` `^=` `&=` `|=`

Insertion and Extraction (Streaming):
`<<` `>>` `>>=` `<<=`
- especially used for c++'s version of C#'s `ToString()`:

```cpp
ostream& operator << (ostream &os, const Student &s) {
    return (os << "Name: " << s.name << "\n Age: " << s.age << "\n Final Grade: " << s.finalGrade  << std::endl);
}
```

```cpp
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

```cpp
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

## EXERCISE: VECTOR CLASS
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
- copy-constructor
- assignment operator `=`
- move constructor not necessary. Why?

## EXERCISE: IMPROVE STRING CLASS
Add all operators that make sense.

For example, but not limited to:
- `+` operator to combine two strings to a new string
- `+=` to append one string to another
- `[]` to access the string's characters.
- `<<` for printing the string using `cout`
- `<` to find out, which string comes alphabetically first