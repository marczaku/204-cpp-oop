# 8 Compile-Time Polymorphism
Similar to Generics in C#

## Template Class

### Defining a Template Class

```c++
template<typename THealth>
struct Player
{
	THealth health = 20;
	Player(THealth health) : health{health} {}
	void DealDamage(THealth health)
	{
		this->health -= health;
	}
};
```

### Using a Template Class

```c++
int main()
{
	Player<int> player{10};
	player.DealDamage(5);
	printf("Player Health: %d\n", player.health);
	Player<float> player2{100.2f};
	player2.DealDamage(59.99f);
	printf("Player2 Health: %f\n", player2.health);
}
```

#### Invalid Type Arguments

```c++
struct Foo{};
int main()
{
	Player<Foo> player{Foo{}};
	player.DealDamage(Foo{});
}
```

#### Nested Generics

```c++

<template typename T>
struct container{
    T value;
    container(T t) : value{t} {}
};

// ERROR: (Why?)
container<container<int>>{container<int>{5}};
// No Error:
container< container< int > >{container< int >{5}};
```

### Template Instantiation

Every time, you use a template class with different type arguments
- an exact copy of the class is created
- and the type arguments are replaced with the provided ones
- each copy is an independent class of its own
- meaning, that `container<int>` and `container<bool>` are two independent classes after compilation:

```c++

template<typename T>
class container<T>{
    T t;
public:
    container(T t) : t{t} {}
    T get() {return t;}
};

int main(){
    container<int>{5};
    container<bool>{true};
}
```

```c++
// Instantiation with int:
class container<int> {
    int t;
public:
    container(int t) : t{t} {}
    int get() { return t; }
};

// Instantiation with bool:
class container<bool> {
    bool t;
public:
    container(bool t) : t{t} {}
    bool get() { return t; }
};
```

Force Compiler to instantiate Template with given parameters

```c++
// force instantiation with float
template class container<float>;
```

### Compilation
Not compiled like ordinary code
- compiled when first used
- therefore, must be in a header-file
  - to be visible from the code instantiating the template
  - therefore, template code needs to be in header files

Template code in `cpp` file can only be used within that file
  - otherwise linker error

```c++
// Player.h
#pragma once

template<typename THealth>
struct Player
{
	THealth health;

	void foo(){}
	void bar();
};
```

```c++
// Player.cpp
#include "Player.h"
template<typename THealth>
void Player<THealth>::bar(){}
```

```c++
// Game.cpp
#include "Player.h"

int main() {
	Player<float> player{};
	player.foo();
	player.bar(); // LINKER ERROR :(
}
```

Can fix above error by forcing template instantiation in Player Header or Cpp File:

```c++
template struct Player<float>;
```

But you'd have to know about all possible type arguments that you might use in the future.

## Template Function

### Defining a Template Function

```c++
template<typename TMessage>
void PrintMessage(const TMessage& message)
{
	printf("Print: %s\n", message.ToString());
}
```

### Using a Template Function

```c++
struct ErrorMessage
{
	char* ToString() const
	{
		return new char[]{"There has been an error."};
	}
};
int main()
{
	PrintMessage(ErrorMessage{});
}
```

## Examples: Named Conversion Functions

### const_cast

Can be used to convert a const object to a non-constant one:
- generally not a good idea!
```c++
void TakeDamage(const Unit& attacker, int damage) {
	Unit& nonConstAttacker = const_cast<Unit&>(attacker);
	attacker.TakeDamage(*this, 5);
}
```

### static_cast

Reverses an implicit conversion
- a.k.a. down-casting
```c++
void Attack(Unit* target){
	Ghost* ghost = static_cast<Ghost*>(target);
}
```

### reinterpret_cast

Allows to reinterpret memory as a different type, e.g. to look at the bytes of a `float`:

```c++
#include <cstdio>

int main()
{
    float f = 1;
    printf("%x", *reinterpret_cast<int*>(&f));
}
```

### dynamic_cast

The most powerful casting operator
- it looks at the runtime type of an object
- it uses RTTI (run-time type information) for this
  - this is information that is generated during compilation for all polymorphic classes
  - it allows analysis of the type's information during runtime
  - it requires that your class has at least one `virtual` member
  - similar to C#'s `Reflection`
- it works similar to C#'s type casting
- it has no trouble with multiple inheritance:

```c++
struct IFoo {
    virtual void foo() = 0;
};

struct IBar {
    virtual void bar() = 0;
};

struct FooBar : IFoo, IBar {
    void foo(){}
    void bar(){}
};

int main()
{
    FooBar* foobar = new FooBar{};  // 4a30
    IFoo* foo = foobar;             // 4a30
    IBar* bar = foobar;             // 4a38 offset of v-table pointer

    void* a = static_cast<void*>(foo); // 4a30 -> GOOD!
    void* b = static_cast<void*>(bar); // 4a38 -> does not point to foobar :(

    void* c = dynamic_cast<void*>(foo); // 4a30 -> GOOD!
    void* d = dynamic_cast<void*>(bar); // 4a30 -> fixed: now, same as foobar :)
}
```

### EXERCISE: narrow_cast

> In Bjarne Stroustrup's "The C++(11) programming language" book, section "11.5 Explicit Type Conversion" you can see what it is.
> 
> Basically, it is a homemade explicit templated conversion function, used when values could be narrowed throwing an exception in this case, whereas static_cast doesn't throw one.
> 
> It makes a static cast to the destination type, then converts the result back to the original type. If you get the same value, then the result is OK. Otherwise, it's not possible to get the original result, hence the value was narrowed losing information.

### EXERCISE: average
- Implement a function that calculates the average of an array of doubles
- Implement the same function for type int
- Now, make the function runtime-polymorphic

## Template Type Deduction
Useful: Often, the compiler can deduct
- the template type arguments
- from the arguments provided to a function

In that case, the template type arguments do not need to be specified.

### Example 1

Given:
```c++
int a = 255;
```

```c++
std::byte b = narrow_cast<int, std::byte>(a);
```

Can be reduced to:
```c++
std::byte b = narrow_cast<std::byte>(a);
```

### Example 2

Given:
```c++
int a[]{1,2,3,4,5,6};
```

```c++
int average = average<int>(a/*...*/)
```

Can be reduced to:
```c++
int average = average(a);
```

## Concepts

- C++20 language feature
- Allows to define constraints on template parameters

Advantages:
- more readable and expressive code
- better error messages
- improved compile-time performance

### Problem: Type Checking in Templates

```c++
template<typename T>
T sqr(T value){
    return value * value;
}

int main() {
    int i = 5;
    sqr(i);
    sqr(&i); // ERROR
}
```

- typechecking happens during template instantiation
  - step during compilation
- `sqrt<int>` compiles, since operator `int * int` is defined
- `sqrt<int*>` does not compile, since operator `int* * int*` is not defined

Similar to duck-typing
> If it looks like a duck and quacks like a duck, then it must be type duck.

Problem: Error messages look cryptic
- Solution comes later

### Predefined Concepts
[You can find them here](https://en.cppreference.com/w/cpp/concepts)

### Custom Concepts
Syntax:
```c++
template <typename T, typename U, ...>
concept concept_name = requires (arg1, arg2, ...) {
    expression -> return_type;
    expression -> return_type;
    ...
};
```

For example, a concept that validates that a Type can be multiplied with itself:
```c++
template <typename T>
concept multipliable = requires (T a, T b) {
    a*b;
};
```

The expression needs to compile. It is not actually evaluated.

### Using Concepts as Type Requirements
Postfix:
```c++
template<typename T>
	requires multipliable<T>
T square(T a)
{
    return a * a;
}
```

Inline:
```c++
template<multipliable T>
T cubed(T a)
{
    return a * a * a;
}
```

```c++
int main()
{
    int number = 3;
    auto result = square(number);
    auto result2 = square(&number);
}
```

### Using Concept as Parameter Type Constraint

Postfix:
```c++
size_t index_of_max(auto* values, size_t count)
	requires std::totally_ordered<decltype(values)>
{
    size_t max_index{};
    for (size_t i{1}; i < count; ++i)
    {
        if(values[i] > values[max_index]) max_index = i;
    }
    return max_index;
}
```

Inline:
```c++
size_t index_of_min(std::totally_ordered auto* values, size_t count)
{
    size_t min_index{};
    for (size_t i{1}; i < count; ++i)
    {
        if(values[i] < values[min_index]) min_index = i;
    }
    return min_index;
}
```

### Concept Resolution
The function specialization with the most specific constraints is chosen.

#### Problem:
In this case, both functions are viable for type `Y`
- `has_x` is one `concept` that checks for `x`
- `coord` is one `concept` that checks for `x` and `y`
```c++
template <typename T>
concept has_x = requires (T v) {
    v.x;
};
template <typename T>
concept coord = requires (T v) {
    v.x;
    v.y;
};
void function(has_x auto x) {}
void function(coord auto x) {}

struct X {
    int x;
};

struct Y {
    int x;
    int y;
};

int main() {
    function(X{}); // OK, only one viable candidate
    function(Y{}); // Fails, ambiguous
}
```

#### Solution:

Here, `coord` uses the `has_x` concept in its definition:
- `has_x` is one `concept` that checks for `x`
- `coord` is two `concepts`:
  - `has_x`
  - and one that checks for `y`

It is therefore more specialized than `has_x`
```c++
template <typename T>
concept has_x = requires (T v) {
    v.x;
};
template <typename T>
concept coord = has_x<T> && requires (T v) {
    v.y;
};
void function(has_x auto x) {}
void function(coord auto x) {}
struct X {
    int x;
};
struct Y {
    int x;
    int y;
};
int main() {
    function(X{}); // OK, only one viable candidate
    function(Y{}); // OK, coord is more specific
}
```

### EXERCISE: BUILD AN AVERAGEABLE CONCEPT AND USE IT
Template:
```c++
template<typename T>
concept bool Averageable = std::something_with_default_constructible_v<T>
        && requires (T a, T b) {
            // insert mathematical operations and result type here
            // like this: `{ a % b } -> T;`
        };
```

## static_assert
- gets evaluated at compile time
- throws compile errors if invalid

```c++
// validate during compile-time that integral division rounds towards zero:
static_assert(-5 / 2 == -2);
```

## Non-Type Template Parameters
You can use Non-Type Template Arguments
- you can then pass constant values as template arguments

The following program uses this technique for compile-time validated array access:
```c++
#include <cstdio>

template <size_t Index, typename T, size_t Length>
T& get(T (&arr)[Length])
{
    static_assert(Index < Length, "Out-of-bounds access");
    printf("Accessing element %zd of %zd\n", Index, Length);
    return arr[Index];
}

int main()
{
    int numbers[]{1, 2, 3, 4, 5};
    printf("%d\n", get<2>(numbers));
    printf("%d\n", get<5>(numbers)); // compile error
}
```

### Example: Array Size Checking Through Templates

```c++
#include <cstdio>

template<size_t N>
void print_all(int (&a)[N])
{
    for(const auto b : a)
    {
        printf("%d, ", b);
    }
}

int main()
{
    int numbers[]{1, 2, 3, 4, 5};
    print_all(numbers);
}
```

### Example: GameGrid
```c++
template<int cols, int rows>
class Grid{
    Cell cells[cols][rows];
};
```

## Variadic Template
You can write template functions that take unlimited number of arguments:

```c++
// C++ program to demonstrate working of
// Variadic function Template
#include <iostream>
using namespace std;
 
void log() { cout << endl; };
template <typename T, typename... Types>
void log(T first, Types... others)
{
    cout << first;
    log(others...);
}
 
int main()
{
    log(1, 2, 3.14, true, "Anything else");
    return 0;
}
```

## Template Specialization
You can specialize template classes and functions for specific arguments
- if you want it to behave differently for some arguments
- e.g. a generic average function
  - which would only for int arguments also round the result
- similar to function overloading

```c++
#include <iostream>
using namespace std;
 
template <class T>
void fun(T a)
{
   cout << "The main template fun(): "
        << a << endl;
}
 
template<>
void fun(int a)
{
    cout << "Specialized Template for int type: "
         << a << endl;
}
 
int main()
{
    fun<char>('a');
    fun<int>(10);
    fun<float>(10.14);
}
```

## Template Instantiation
It's interesting to know, when and why templates get instantiated and how to control it explicitly.

```c++
template struct container<int>;
```

## Name Binding
Depending on whether a template function depends on the template argument or not, the identifiers it binds to (variables, functions) is determined
- the moment the generic template class is defined
  - if it does not depend on the argument
- the moment the template is instantiated
  - if it does depend on the argument

```c++
#include <iostream>
using namespace std;

void f(double) { cout << "Function f(double)" << endl; }

template <class A> struct container { // point of definition of container
    void independentFunction() { f(1); } // f(int) not defined, yet
    void dependentFunction(A arg) { f(arg); } // will be evaluated on instantiation
};


void f(int) { cout << "Function f(int)" << endl; }
template struct container<int>; // point of instantiation of container<int>

int main(void) {
    container<int> test;
    test.independentFunction(); // f double
    test.dependentFunction(10); // f int
    return 0;
}
```

## Template Meta-Programming
The Combination of
- Template Specialization
- Non-Type Template Arguments

Allows you to write Code using Templates!

Why is this special? Because Templates get evaluated at Compile-Time
- i.e. their results get calculated before the program is run
- which means that there is no overhead during runtime

It is proven that Template Meta-Programming is Turing-Complete
- i.e. any computation expressible by a computer program can be computed by a template meta-program

This is a template that calculates, whether a number is a prime number:
```c++
#include <iostream>
using namespace std;

template <int N, int D>
struct is_prime_internal {
    static const bool result = (N % D) && is_prime_internal<N, D - 1>::result;
};

template <int N>
struct is_prime_internal<N, 1> {
    static const bool result = true;
};

template <int N>
struct is_prime {
    static const bool result = is_prime_internal<N, N - 1>::result;
};

int main() {
    cout << "Is 17 Prime: " << is_prime<17>::result << endl;
    cout << "Is 17 Prime: " << is_prime<21>::result << endl;
}
```

## EXERCISE: UPDATE YOUR LINKED-LIST AND/OR DYNAMIC ARRAY
- Transform them into generic classes

## EXERCISE: WRITE A GENERIC SORT FUNCTION
- It takes a generic array as an argument
- Or your Dynamic Array class
- And then sorts the contents using a sorting algorithm of your choice