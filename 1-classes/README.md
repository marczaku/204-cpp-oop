# Fully-Featured Classes

- Classes which contain Methods
- Allow for Encapsulation

## Methods

- Member Functions
- Have access to all class Members

```cpp
struct Time {
	void addYear() {
		year++;
	}
	int year;
};
```

```cpp
int main() {
  	Time time;
	time.year = 2022;
	printf("Time: %d\n",time.year);
	time.addYear();
	printf("Time: %d\n",time.year);
}
```

## Access Control

- structs are `public` per default:

```cpp
struct Time {
	void addYear() {
		year++;
	}
	bool setYear(int newYear) {
		if(newYear < 1990) return false;
		year = newYear;
		return true;
	}
	int getYear() {
		return year;
	}
private:
	int year;
}
```

- classes are `private` per default:

```cpp
class Time {
	int year;
public:
	void addYear() {
		year++;
	}
	bool setYear(int newYear) {
		if(newYear < 1990) return false;
		year = newYear;
		return true;
	}
	int getYear() {
		return year;
	}
}
```

- there is no other difference between `struct` and `class` in C++!

## Initializing Members

What's the problem in the following code?

```cpp
int main() {
	Time time;
	clock.setYear(1980);
	printf("Time: %d\n", time.getYear());
	clock.addYear();
	printf("Time: %d\n", time.getYear());
}
```

## Constructors

```cpp
Time() {
	year = 2022;
}
Time(int in_year) {
	if(!setYear(in_year)) {
		year = 2022;
		// (better: throw an exception)
	}
}
```

```cpp
int main() {
	Time a();
	Time b(2030);
}
```

## Initialization

### Fundamental Type: Zero

```cpp
int a = 0;
int b {};
int c = {};
int d; // uninitialized
```

### Fundamental Type: Arbitrary Value

```cpp
int e = 42;
int f{42};
int g = {42};
int h(42);
```

### PODs

```cpp
struct Pod {
	uint64_t a;
	char b[256];
	bool c;
};

int main() {
	Pod pod1{};
	Pod pod2 = {};
	Pod pod3{42, "Hello"};
	Pod pod4{42, "Hello", true};
	// Pod pod5 = 0;
	// Pod pod6{"Hello", 42}; // invalid types/order
	// Pod pod7(42, "Hello", true); // does not work
}
```

#### Call-By-Value

```cpp
#include <cstdio>

struct Pod {
	int a;
	char b[256];
	bool c;
};

void modify(Pod pod) {
	pod.a = 99;
	pod.c = false;
	printf("Pod changed: %d, %s, %d\n", pod.a, pod.b, pod.c);
}

int main() {
	Pod pod{42, "Hello", true};
	printf("Pod before: %d, %s, %d\n", pod.a, pod.b, pod.c);
	modify(pod);
	printf("Pod after: %d, %s, %d\n", pod.a, pod.b, pod.c);
}
```

Whenever you assign a value to a new variable
- a copy is created
- if you modify that copy, the original remains unaffected

### Arrays

```cpp
int array1[]{1,2,3}; // 1, 2, 3
int array2[5]{}; // 0, 0, 0, 0, 0
int array3[5]{1,2,3}; // 1, 2, 3, 0, 0
int array4[5]; // uninitialized
```

### Fully Featured Classes

```cpp
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

```cpp
int main(){
	SuchClass s1;
	SuchClass s2{};
	SuchClass s3{'c'};
	SuchClass s4{255};
	SuchClass s5('g');
	SuchClass s6 = {'l'};
	SuchClass s7(); // Function Declaration :o) // most vexing parse
}
```

### Narrowing Conversions

```cpp
float a{1};
float b{2};
int narrowedResult(a/b); // ignored
int result{a/b}; // warning
```

#### Initializing Class Members

```cpp
struct Customer{
	bool active = true;
	int probationPeriod{6};
	char name[256] = {"unnamed"};
	// int notPossible(5);
};
```


### Initializing Class Members In Constructor
Problem:

```cpp
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
      _member = SomeMember(5);
    }
};

int main(){
  Inefficient inefficient;
}
```

Inefficient!

Solution: Initializer List
```cpp
class Efficient{
  SomeMember _member;
public:
  Efficient() : _member(SomeMember(5)) {}
};
```

### Brace yourself
- Always use braced initializers
- Uniform Initialization

## Destructor
- release file handles
- flush network sockets
- free dynamic objects

```cpp
struct Monster{
	~Monster(){
		printf("<dramatic deathrattle>\n");
	}
}
```

```cpp
int main(){
	printf("creating monster.\n");
	{
		Monster monster;
	}
	printf("done.\n");
}
```


## This-Pointer
Sometimes, you need to access the `current object` from a method. As in, the object on which the Method is currently being invoked. e.g. when attacking:

```cpp
#include <cstdio>
struct Unit{
	char* name;
	Unit(char* name){
		// Use case for `this`: name is the name both for the constructor arg as well as the field member
		this->name = name;
	}

	void attack(Unit* target){
		// Use case for `this`: pass on a pointer to ourselves to the attacked Unit
		target->attackedBy(this);
	}

	void attackedBy(Unit* attacker){
		printf("%s got attacked by %s.\n", name, attacker->name);
	}
};

int main(){
	Unit hero{"Hero"};
	Unit monster{"Monster"};
	hero.attack(&monster);
}
```

C++ Behind the Scenes:
- this-pointer is an implicit, hidden parameter of every non-static member function
- access to data member or call to member function is done by dereferencing this pointer
- is done implicitly, can also be done explicitly

## Static

```cpp
#include <cstdio>

struct Battery {
	int power;
	static int totalPower;
	Battery(int power) : power{power}
	{
		chargeTotalBattery(power);
	}
	static void chargeTotalBattery(int power) {
		totalPower += power;
		printf("Total Battery Power: %d\n", totalPower);
	}
};

int Battery::totalPower = 200;

int main() {
	Battery a{25};
	Battery b{30};
	Battery::chargeTotalBattery(50);
	printf("In the end, we have a power of: %d\n",Battery::totalPower);
}
```

- accessible through `TypeName::memberName`
- fields only initializable from global scope
  - unless they're `const`


## EXERCISE: CALCULATOR CLASS
- Make an Object-Oriented Calculator Application

## Exercise: String-Class!

Please do this exercise. We will continue working on this!

### private Members
- `xxx length;`
- `xxx buffer;`
- `xxx maxSize;`

### public Members
- `ctor(xxx maxSize)`
  - add a log so you see that an empty string gets constructed
  - create buffer
  - initialize length & maxSize
- `ctor(xxx defaultText, xxx maxSize)`
  - add a log so you see that a non-empty string gets constructed
  - create buffer
  - initialize length & maxSize
  - append defaultText
- `~()`
  - add a log so you see what string gets deconstructed
  - delete buffer
- `append(xxx text)`
  - checks for maxSize! -> exception
  - adds the given text to the string's buffer
  - updates length
- `appendLine(xxx text)`
  - checks for maxSize! -> exception
  - adds the given text + line break to the string's buffer
  - updates length
- `void print()`
  - prints the string that's currently buffered
- `xxx getString()`
  - returns a c-style string for the currently buffered contents

## EXERCISE: LINKED LIST

## EXERCISE: DYNAMIC ARRAY