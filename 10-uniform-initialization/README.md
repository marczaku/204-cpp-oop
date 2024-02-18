
# Initialization

TLDR: Always initialize using Braces `{}` (uniform initializer)

## Fundamental Type: Zero

```c++
int b {}; // c++
int a = 0; // c#
// int c = {}; // also possible
int d; // uninitialized
```

## Fundamental Type: Arbitrary Value

```c++
int f{42}; // c++
int e = 42; // c#
// int g = {42}; // also possible
// int h(42); // also possible
```

## PODs

```c++
struct Pod {
	uint64_t a;
	char b[256];
	bool c;
};
```

```c++
int main() {
	Pod pod1{}; // default values
	Pod pod3{42, "Hello"}; // some variables
	Pod pod4{42, "Hello", true}; // all variables
	// Pod pod5 = 0;
	// Pod pod6{"Hello", 42}; // invalid types/order
	// Pod pod7(42, "Hello", true); // does not work
	// Pod pod1; // uninitialized
}
```

## Arrays

```c++
int array1[]{1,2,3}; // 1, 2, 3
int array2[5]{}; // 0, 0, 0, 0, 0
int array3[5]{1,2,3}; // 1, 2, 3, 0, 0
int array4[5]; // uninitialized
```

## Fully Featured Classes

```c++
struct SuchClass {
	SuchClass() {
		printf("(no argument)\n");
	}
	SuchClass(char c) {
		printf("char: %c\n", c);
	}
	SuchClass(int i) {
		printf("int: %d\n", i);
	}
}
```

```c++
int main(){
	SuchClass s1; // implicit default constructor
	SuchClass s2{}; // explicit default constructor
	SuchClass s3{'c'}; // constructor #2
	SuchClass s4{255}; // constructor #3
	// SuchClass s5('g'); // also possible
	// SuchClass s6 = {'l'}; // also possible
	SuchClass s7(); // Function Declaration :o) // most vexing parse
}
```

## Narrowing Conversions

When not using Braces, you sometimes don't notice when you accidentally lose information
- e.g. here, the result should be `0.5f`, but as `int` it is `0`

```c++
float a{1};
float b{2};
int narrowedResult(a/b); // warning or ignored
int result{a/b}; // error or warning
```

### Initializing Class Members

```c++
struct Customer{
	int probationPeriod{6}; // c++
	bool active = true; // c#
	// char name[256] = {"unnamed"}; // also possible
	
	// int notPossible(5);
};
```


## Initializing Class Members In Constructor

Problem:

```c++
#include <stdio.h>

struct SomeMember{
  int member;
  SomeMember() { 
      printf("SomeMember()\n"); 
      member = 0; 
  }
  SomeMember(int i){
    printf("SomeMember(%d)\n", i);
    member = i;
  }
};

class Inefficient{
  SomeMember _member;
  public:
    Inefficient(){
      _member = SomeMember{5};
    }
};

int main(){
  Inefficient inefficient{};
}
```

Inefficient!

## Initializer List

Solution: Initializer List

```c++
class Efficient{
  SomeMember _member;
public:
  Efficient() : _member{SomeMember{5}} {}
};
```

## Brace yourself
- Always use braced initializers
- Uniform Initialization
