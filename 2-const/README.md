# 7 Const
- short for "constant"
- i.e. "cannot be modified"
- gives compiler errors if violated

## Const Arguments

```cpp
#include <cstdio>
void printButDoNotModify(const char* name){
	printf("%s", name);
	// name[0] = 'K'; // Won't compile!
}

int main() {
	char* name = "Marc";
	printButDoNotModify(name);
}
```

## Const Methods

The following code won't compile:

```cpp
class DateTime {
	int year{1990};
public:
	DateTime(int year){
		this->year = year;
	}
	void addYear() {
		year++;
	}
	int getYear() {
		return year;
	}
};

void printButDoNotModify(const DateTime& dateTime){
	printf("Year: %d", dateTime.getYear());
}

int main() {
	DateTime time{1925};
	printButDoNotModify(time);
}
```

Error: `this` argument to member function `getYear` has type `const DateTime`, but function is not marked `const`

Reason is, that only `const` methods can be invoked on `const` variables.

### Definition

```cpp
int getYear() const {
	return year;
}
```

### Usage

```cpp
int getYear() const {
	return ++year; // Won't compile - error: cannot assign to non-static data member within const member function 'getYear'
}
```

## Const Members

```cpp
struct Unit {
	const int maxLevel = 100;
	DateTime birthDate;
}
```

### Initializer List

```cpp
#include <cstdio>
class DateTime {
	int year{1990};
public:
	DateTime(int year){
		this->year = year;
	}
	void addYear() {
		year++;
	}
	int getYear() const {
		return year;
	}
};

struct Unit {
	const int maxLevel = 100;
	const int id;
	const DateTime birthDate;
	Unit(int id, int year) : id{id}, birthDate{year}{

	}
};

int main(){
	Unit unit{1337, 1990};
	printf("Unit #%d, born %d", unit.id, unit.birthDate.getYear());
}
```

- member initializations execute before constructor's body
  - ensures validity of all members before constructor code
  - members initialize only once

  ## Mutable Members
  Overrides/ignores const function access. Useful for:
  - access counters
  - caching

  ```cpp
  class Circle {
	mutable int accessCounter;
	float radius;
	public:
	float getRadius() const {
		accessCounter++;
		return radius;
	}
  }
  ```

## Volatile Members
Tells the program that a variable may change outside the context of a program
- therefore, the cpu cannot cache it
- and instead must read it from RAM every time
Used for:
- memory-mapped hardware ports
- memory shared between threads/processes
```cpp
volatile int i;
```

## Static Members
- Not bound to the instance of a class
- Effectively global
- but within the scope of a class

## Static
static has no less than 4 meanings in C++

- static member functions (C++)
- static data members (C++)
- static global functions (C)
  - become file-local
- static local variable  (C)
  - effectively become global; value survives across calls
  - use still restricted to function scope
  - no name clashes with outside

## EXERCISE: UPDATE STRING CLASS
- Make methods and arguments const if they don't have to be non-const

## EXERCISE: DO THE SAME FOR YOUR OTHER CLASSES
