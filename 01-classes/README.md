# Fully-Featured Classes

```c++
class Player {
    static int playerCount; // static data member, shared across all players
    int health; // data member, encapsulated (private)
public: // public member declarator
    // parameterless constructor
    Player() : Player{100} {
    }
    // constructor with argument
    Player(int health) : health{health} {
        playerCount++;
    }
    
    ~Player() {
        cout << "Player died.";
        playerCount--;
    }
    
    // static method that can be accessed through type name
    static int getPlayerCount() { return playerCount; }
    
    int getHealth() { return health; } // getter-method
    void setHealth(int value) { // setter-method with data validation
        health = clamp(value, 0, 100);
    }
};
```

- Classes which contain Methods
- Allow for Encapsulation

## Methods

Member Functions
- Have access to all class Members

```c++
struct Time {
	void addYear() {
		year++;
	}
	int year;
};
```

Can only be invoked using
- the member-of operator `.`
- on a class instance (object)

```c++
int main() {
  	Time time;
	time.year = 2022;
	cout << "Time: " << time.year << "\n";
	time.addYear();
	cout << "Time: " << time.year << "\n";
}
```

## Access Control

Structs are `public` per default:

```c++
struct Player {
    int getHealth() { return health; } // getter-method
    void setHealth(int value) { // setter-method with data validation
        health = clamp(value, 0, 100);
    }
private: // private member declarator
    int health; // data member, encapsulated (private)
};
```

Classes are `private` per default:

```c++
class Player {
    int health; // data member, encapsulated (private)
public: // public member declarator
    int getHealth() { return health; } // getter-method
    void setHealth(int value) { // setter-method with data validation
        health = clamp(value, 0, 100);
    }
};
```

There is no other difference between `struct` and `class` in C++!

## Initializing Members

What's the problem in the following code?

```c++
int main() {
	Player one;
	cout << one.getHealth() << "\n";
	one.addHealth(3);
	cout << one.getHealth(); << "\n";
}
```

## Constructors

Give you control over how objects are constructed
- which arguments need to be provided
- how those arguments are used
- note, we will learn soon that this is not the right way of initializing members

```c++
Player() {
    health = 100;
}
Player(int _health) {
    health = _health;
}
```

```c++
int main() {
	Player one{};
	Player two{33};
}
```

## Initializing Class Members In Constructor

Problem:

```c++
#include <iostream>

using namespace std;

struct LevelTracker {
  int experience;
  LevelTracker() { 
      cout << "LevelTracker()\n"; 
      experience = 0; 
  }
  LevelTracker(int startExperience){
   cout << "LevelTracker(" << startExperience << ")\n";
    experience = startExperience;
  }
};

class Player{
  LevelTracker level;
  public:
    Player(){
      level = LevelTracker{5};
    }
};

int main(){
  Player one{};
}
```

Inefficient!

## Initializer List

Solution: Initializer List

```c++
class Player {
  LevelTracker level;
public:
  Player() : level{LevelTracker{5}} {}
};
```

Can be further shortened to:
```c++
Player() : level{5} {}
```

## Destructor

Function that's called when your object is freed ("destroyed")

Use Cases:
- release file handles
- flush network sockets
- free dynamic objects

```c++
struct Monster{
	~Monster(){
		cout << "<dramatic deathrattle>\n";
	}
}
```

```c++
int main(){
	cout << "creating monster.\n");
	{
		Monster monster;
	}
	cout << "done.\n";
}
```

## This-Pointer
Sometimes, you need to access the `current object` from within a method. As in, the object on which the Method is currently being invoked. e.g. when attacking:

```c++
#include <iostream>
struct Unit{
	char* name;
	Unit(char* name){
		// Use case for `this`: name is the name both for the constructor arg as well as the member variable
		this->name = name;
	}

	void attack(Unit* target){
		// Use case for `this`: pass on a pointer to ourselves to the attacked Unit
		target->attackedBy(this);
	}

	void attackedBy(Unit* attacker){
	    cout << name << " got attacked by " << attacker->name << ".\n";
	}
};

int main(){
	Unit hero{"Hero"};
	Unit monster{"Monster"};
	hero.attack(&monster);
}
```

C++ Behind the Scenes:
- `this`-pointer is an implicit, hidden parameter of every non-static member function
- access to data member or call to member function is done by dereferencing this pointer
- is done implicitly, can also be done explicitly

```c++
class Player {
    int health;
public:
    void takeDamage(int amount) {
        health -= amount;
    }
};

int main() {
    Player player{};
    player.takeDamage(5);
}
```

Translates to:

```c++
class Player {
    int health;
public:
    static void takeDamage(Player* _this, int amount) {
        _this->health -= amount;
    }
};

int main() {
    Player player{};
    Player::takeDamage(&player, 5);
}
```

## Static

Variables and Methods that are not associated with a class instance
- can not access `this`

```c++
#include <iostream>

struct Gryffindor {
	int earnedPoints{};
	static int housePoints;
	void gainPoints(int points) {
		earnedPoints += points;
		gainHousePoints(points);
	}
	static void gainHousePoints(int points) {
		housePoints += points;
	}
};

int Gryffindor::housePoints = 100; // starting points

int main() {
	Gryffindor harry{};
	Gryffindor ronald{};
	harry.gainPoints(50);
    ronald.gainPoints(50);
	Gryffindor::gainHousePoints(50);
}
```

- accessible through `TypeName::memberName`
- static member variables only initializable from global scope
  - unless they're `const`


# Quiz

What is the purpose of the `static` keyword in C++?
- A) It indicates that a variable or method is associated with a class instance.
- B) It specifies that a variable or method can only be accessed within the class.
- C) It indicates that a variable or method is not associated with a class instance.
- D) It allows variables and methods to access the this pointer.

Which of the following statements about constructors in C++ is true?
- A) Constructors cannot have parameters.
- B) Constructors are called explicitly when an object is destroyed.
- C) Constructors can be used to initialize object members.
- D) Constructors cannot be overloaded.

What is the primary use of the `this` pointer in C++?
- A) To access static member variables.
- B) To access the object that a class method is invoked on.
- C) To define constants within a class.
- D) To create dynamic objects.

Which of the following best describes the purpose of a destructor in C++?
- A) To initialize object members.
- B) To allocate memory for an object.
- C) To perform cleanup tasks when an object is destroyed.
- D) To define static member variables.

How can you access a static member variable in C++?
- A) By using the `.` operator on an object of the class.
- B) By using the `->` operator on a pointer to an object of the class.
- C) By using the `::` operator with the class name.
- D) By using the `this` pointer within a class method.

What is the difference between a struct and a class in C++?
- A) Structs can only have public members, while classes can have private members.
- B) Classes are private by default, while structs are public by default.
- C) There is no difference; they can be used interchangeably.
- D) Structs are used for object-oriented programming, while classes are used for procedural programming.

Which of the following is NOT a recommended way to initialize class members in C++?
- A) Using a constructor initializer list.
- B) Using an assignment statement within the constructor body.
- C) Using a static member function.
- D) Using a default constructor.

What is the purpose of access control in C++ classes?
- A) To restrict access to certain members of a class.
- B) To allow access to all members of a class.
- C) To define the visibility of a class in different compilation units.
- D) To define the order of initialization of class members.

# Exercises

## Calculator Class
- Make an Object-Oriented Calculator Application
  - Add a class named `Calculator`
  - Implement the following functions:

```cs
Calculator calculator;
calculator.SetNumber(5); // change the state (member variable)
calculator.SetOperation(Operation::Add); // enum
calculator.SetNumber(3);
cout << calculator.getNumber() << "\n"; // should say 8
calculator.Clear();
calculator.SetNumber(2);
calculator.SetOperation(Operation::Multiply);
calculator.SetNumber(3);
calculator.SetOperation(Operation::Subtract);
calculator.SetNumber(1);
cout << calculator.getNumber() << "\n"; // should say 5
```

## String-Class!

The String Class will be used to make using strings easier in C++!

It will allow us to easily:
- Create Strings
- Modify Strings
- Combine Strings
- Compare Strings

For now, we will have one limitation:
- In the constructor, the user of the class must provide a `maxLength` for each string
  - This `maxLength` may not be exceeded while using the class instance
  - This makes it easier for you, so you won't have to resize the internal buffers
- The reason is, that strings are internally arrays of characters, as you may know
  - So, if you wanted your `string` class to be able to hold any string without the user providing a `maxLength`...
  - ...you would have to implement something like a List / DynamicArray in C#
  - which automatically resizes as new elements are added

### Example Usage
```c++
String helloWorld{"Hello", 100};
helloWorld.appendLine(", World!");
helloWorld.append("Alli");
helloWorld.appendLine("hopa!");
helloWorld.print();
```

Output:
```
Hello, World!
Allihopa!
```

And all of that without having to think about Pointers or Arrays ever again

In fact, C++ comes with its own OOP `string` class which we will use in the future!

### Private Members
You need to figure out the types yourself:
- `xxx length;` 
  - the length will tell us, how long the currently stored string is
  - e.g. `4` for `"Marc"`
- `xxx maxSize;`
  - this variable will be provided by the user in the constructor
  - and set a limitation on how long this string can ever become
  - it may not be exceeded as the user appends more to this string
- `xxx buffer;`
  - the buffer is used to store the characters of this string
  - you can use `maxSize` for the size of the buffer

### Public Members
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