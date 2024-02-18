# Const

```c++
class Player {
    const int maxHealth;
    const int id;
    int health;
    
    // const methods mark a method 
    void getHealth() const {
        return health;
    }
    // a const reference argument avoids copies while
    // still ensuring that variables aren't changed
    void greetPlayer(const Player& other) const {
        cout << "Player " << id " greets " << other.id << "\n";
    }
};
```

Short for "constant"
- i.e. "cannot be modified"
- gives compiler errors if violated

Useful for:
- passing in references/pointers without the value being modifiable
- allowing compiler-optimizations vor variables that never change
- reducing complexity (no need to debug values that can't change)

## Const Arguments

```c++
#include <iostream>
void printButDoNotModify(const char* name){
	cout << name;
	name[0] = 'K'; // ERROR
}

int main() {
	char* name = "Marc";
	printButDoNotModify(name);
}
```

## Const Methods

The following code won't compile:

```c++
class Player {
	int health{100};
public:
	int getHealth() {
		return health;
	}
};

void printButDoNotModify(const Player& player){
    cout << "Health: " << player.getHealth() << "\n";
}

int main() {
	Player one{};
	printButDoNotModify(one);
}
```

Error: `this` argument to member function `getHealth` has type `const Player`, but method is not marked `const`

Reason is, that only `const` methods can be invoked on `const` variables.

### Definition

```c++
int getHealth() const {
	return health;
}
```

### Usage

```c++
int getHealth() const {
	return ++health; // Won't compile - error: cannot assign to non-static data member within const member function 'getYear'
}
```

## Const Members

```c++
struct Unit {
	const int maxLevel = 100;
	DateTime birthDate;
}
```

### Initializer List
Within an initializer list, you can assign to `const` data members
```c++
#include <iostream>

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

## Mutable Members
Overrides/ignores const function access. 

Useful for:
- access counters
- caching
- debugging

```c++
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
```c++
volatile int i;
```

## Static Members
- Not bound to the instance of a class
- Effectively global
- but within the namespace of a class

## Static
`static` has no less than 4 meanings in C++

- static member functions (C++)
- static data members (C++)
- static global functions (C)
  - become file-local
- static local variable  (C)
  - effectively become global; value survives across calls
  - use still restricted to function scope
  - no name clashes with outside

# Quiz

What is the primary purpose of using const in C++?
- A) To allow for variables to be modified freely.
- B) To indicate that a variable cannot be modified.
- C) To improve performance by optimizing variables.
- D) To specify that a variable is initialized at compile-time.

Which of the following is NOT a valid use case for const in C++?
- A) Passing in references/pointers without allowing modification.
- B) Allowing compiler optimizations for variables that never change.
- C) Increasing complexity by allowing variables to change frequently.
- D) Reducing complexity by avoiding the need to debug values that can't change.

What happens if you try to modify a const argument within a function?
- A) The compiler will optimize the modification for better performance.
- B) It will result in a compilation error.
- C) The modification will be silently ignored.
- D) The program will crash at runtime.

Which of the following is NOT a valid reason for using const methods in C++?
- A) To ensure that a method does not modify the object's state.
- B) To allow the method to be invoked on const objects.
- C) To override const function access.
- D) To allow compiler optimizations for member functions.

How can you define a const method in a C++ class?
- A) By using the mutable keyword in the method definition.
- B) By specifying const after the method name and parameters.
- C) By using the const keyword after the method name in the class definition.
- D) By declaring the method with no parameters.

What is the purpose of a mutable member variable in a C++ class?
- A) To indicate that the variable cannot be modified.
- B) To allow the variable to be modified within const methods.
- C) To ensure that the variable is always initialized.
- D) To override compiler optimizations for member variables.

When should you use a volatile member variable in C++?
- A) When you want to prevent modifications to the variable.
- B) When the variable may change outside the context of the program.
- C) When you want to optimize memory access for the variable.
- D) When the variable is used for caching purposes.

What is the purpose of a static member in C++?
- A) To bind the member to a specific instance of a class.
- B) To allow access to the member only within the class.
- C) To ensure that the member is always initialized.
- D) To create a member that is not associated with any particular instance of a class.

How many meanings does the static keyword have in C++?
- A) 2
- B) 3
- C) 4
- D) 5

What is the primary reason for using static local variables in C++?
- A) To ensure that the variable is accessible from any scope.
- B) To improve performance by reducing memory usage.
- C) To initialize a variable once and retain its value across function calls.
- D) To prevent name clashes with variables outside the function.

# Exercise
Update your `String` class for `const`-correctness
- which data members can be `const`?
- which function arguments?
- which functions can be made `const`?