# 7 Runtime Polymorphism
Inheritance-Based Polymorphism like in C#

## Inheritance
```cpp
class Animal {

}

class Dog : public Animal {
	// inherits all members from animal

	// plus, can define its own members
}
```

### Constructor Chaining
Base Constructors get always called first
- Default Constructor, if not specified manually
```cpp
class Animal {
    const char* name;
public:
    Animal(const char* name) : name{name} {}
};

class Dog : public Animal {
    const char* favoriteTrick;
public:
    Dog(const char* name, const char* favoriteTrick) : Animal{ name }, favoriteTrick{ favoriteTrick } {}
};
```

### Memory Layout
- Polymorphism
  - Dog IS AN Animal
```cpp
class Animal {
    float weight;
	int age;
};

class Dog : public Animal {
    int numberOfTricks;
	bool houseTrained;
};
```

#### Conversion
- Type `Dog` "shrinks" to Type `Animal`
```cpp
Animal animal = Dog{};
```

### Hiding Members
- Early Binding

```cpp
struct Animal {
    void MakeSound() { printf("...\n"); }
};

struct Dog : public Animal {
    void MakeSound() { printf("Woof\n"); }
};

int main()
{
    Animal* dog = new Dog{};
    dog->MakeSound();
}
```

## Virtualization

### Virtual Methods
- Late Binding

```cpp
struct Animal {
    virtual void MakeSound() { printf("...\n"); }
};

struct Dog : public Animal {
    virtual void MakeSound() override { printf("Woof\n"); }
};

int main()
{
    Animal* dog = new Dog{};
    dog->MakeSound();
}
```

### V-Table
- LookUp Function Name -> Function Instance
  - Exists in Memory once for each type
- Referenced by each class instance
  - Size of Type Changes
- Initialized in Constructor
  - Should not directly invoke constructor manually
  - Can't call virtual functions in constructor

### Value Types
What's wrong with this code?
```cpp
int main()
{
    Animal dog = Dog{};
    dog.MakeSound();
}
```

### Co-Variant Virtual Return Type
You can change virtual function's return types
- If the Type is co-variant

```cpp
struct Animal {
    virtual Animal* Clone() {/*...*/};
};

struct Dog : public Animal {
    virtual Dog* Clone() override {/*...*/};
};
```

## Destructor
Memory Leak:

```cpp
struct Animal {
    virtual void MakeSound() {}
};

struct Dog : public Animal {
    char* name;
    Dog() : name {new char[100]} { }
    ~Dog() { delete[] name; }
    virtual void MakeSound() override { printf("Woof\n"); }
};

int main()
{
    Animal* dog = new Dog{};
    dog->MakeSound();
    delete dog;
}
```

Fix: Virtual Destructor

```cpp
struct Animal {
    virtual void MakeSound() {}
    virtual ~Animal() {};
};

struct Dog : public Animal {
    char* name;
    Dog() : name{ new char[100] } { }
    ~Dog() override { delete[] name; }
    virtual void MakeSound() override { printf("Woof\n"); }
};

int main()
{
    Animal* dog = new Dog{};
    dog->MakeSound();
    delete dog;
}
```

## Abstraction
- Cannot be instantiated
- But classes can inherit from this class
  - And those can be instantiated
```cpp
struct Animal {
    virtual void MakeSound() = 0;
};

struct Dog : public Animal {
    virtual void MakeSound() override { printf("Woof\n"); }
};

int main()
{
    Animal* dog = new Dog{};
    dog->MakeSound();
}
```

### Abstract Type Variables
Why does this code not compile?
```cpp
struct Animal {
    virtual void MakeSound() = 0;
};

struct Dog : public Animal {
    virtual void MakeSound() override { printf("Woof\n"); }
};

int main(){
	Animal dog = Dog{};
	dog.MakeSound();
}
```

## Multiple Inheritance

```cpp
struct Animal {

};

struct FlyingAnimal {
	float flyingSpeed;
	virtual void Fly(){}
};

struct WalkingAnimal{
	float walkingSpeed;
	virtual void Walk(){}
};

struct Pegasus : public FlyingAnimal, public WalkingAnimal{
	int value;
}
```

### Memory Layout
No Pointer Identity!
- FlyingAnimal
  - VTable
  - flyingSpeed;
- WalkingAnimal
  - VTable
  - walkingSpeed;
- Pegasus

```cpp
Pegasus value;
Pegasus* pegasus = &value;
WalkingAnimal* walking = &value;
FlyingAnimal* flying = &value;
```

### Name Clashes
```cpp
struct FlyingAnimal {
	float flyingSpeed;
	virtual void Fly(){}
	virtual float GetSpeed() {return flyingSpeed;}
};

struct WalkingAnimal{
	float walkingSpeed;
	virtual void Walk(){}
	virtual float GetSpeed() {return walkingSpeed;}
};
```

Need to be fully qualified

```cpp
printf("%f", pegasus.WalkingAnimal::GetSpeed());
printf("%f", pegasus.FlyingAnimal::GetSpeed());
```


### Dreaded Diamond
```cpp
struct Animal {
	int age;
}
struct FlyingAnimal : public Animal {
	float flyingSpeed;
	virtual void Fly(){}
	virtual float GetSpeed() {return flyingSpeed;}
};

struct WalkingAnimal : public Animal {
	float walkingSpeed;
	virtual void Walk(){}
	virtual float GetSpeed() {return walkingSpeed;}
};
```

### Virtual Base Class
- Virtual Base Classes are only inherited once
```cpp
struct Animal {
	int age;
}
struct FlyingAnimal : virtual public Animal {
	float flyingSpeed;
	virtual void Fly(){}
	virtual float GetSpeed() {return flyingSpeed;}
};

struct WalkingAnimal : virtual public Animal {
	float walkingSpeed;
	virtual void Walk(){}
	virtual float GetSpeed() {return walkingSpeed;}
};
```

Bad if Mixed!

### Casting
Not possible using C-Style cast!
```cpp
Pegasus* pegasus = new Pegasus();
Animal* animal = pegasus; // OK
Pegasus* pegasusAgain = (Pegasus*) animal; // ERROR
```

Use `dynamic_cast`:

```cpp
Pegasus* pegasusAgain = dynamic_cast<Pegasus*>(animal); // OK
```

### Conclusion
Is Multiple Inheritance Bad?
- Not necessarily, it may or may not fit your needs
- There are alternatives
  - nested generalization
  - bridge pattern (aggregation)

Be aware of the problems of multiple inheritance
- No Pointer Identity
- Member Name Clashes possible
- Diamond-Problem

## Interfaces
Good Use Case of Multiple Inheritance
- abstract class
  - no data members
  - all functions pure virtual

Advantages
- no data member classes
- diamond not harmful 
  - cannot inherit data members multiple times

- still no pointer identity!
  - dynamic_cast necessary!

## An Example
Now follows an example for Polymorphism:

### Problem without Polymorphism

```cpp
#include <cstdio>

struct BaseLogger{
	void Log(const char* message)
	{
		printf("BaseLogger: %s\n", message);
	}	
};

struct DerivedLogger : BaseLogger
{
	void Log(const char* message)
	{
		printf("DerivedLogger: %s\n", message);
	}
};

int main()
{
	DerivedLogger logger{};
	BaseLogger& sameLogger = logger;
	logger.Log("This is a DerivedLogger class instance.");
	sameLogger.Log("This is also a DerivedLogger class instance.");
}
```

### Virtual Methods
Allows deriving class to override a method implementation

```cpp
#include <cstdio>

struct BaseLogger{
	virtual void Log(const char* message)
	{
		printf("BaseLogger: %s\n", message);
	}	
};

struct DerivedLogger : BaseLogger
{
	void Log(const char* message) override
	{
		BaseLogger::Log(message);
		printf("DerivedLogger: %s\n", message);
	}
};

int main()
{
	DerivedLogger logger{};
	logger.Log("Hello, World.");
}
```

### Abstract Method

```cpp
struct ILogger{
	virtual void Log(const char* message) = 0;
};
```

### Defining Interfaces

You define interfaces in C++ by defining a purely virtual class:

```cpp
struct ILogger {
	virtual ~ILogger() = default;
	virtual void Log(const char* message) = 0;
};
```

### Virtual Destructor

If you don't define a virtual constructor in the base class, the runtime can't invoke the correct Destructor if you do:

```cpp
ILogger logger = new ConsoleLogger{};
delete logger;
```

### Implementing Interfaces

```cpp
struct ConsoleLogger : ILogger {
	void Log(const char* message) override {
		printf("Log: %s", message);
	}
};
```

### Using Interfaces

#### Constructor Injection

```cpp
struct Player {
	Player(ILogger& logger) : logger{logger} {}

	void Spawn() {logger.Log("Player spawned!");}
private:
	ILogger& logger;
};
```

```cpp
int main()
{
	ConsoleLogger logger{};
	Player player{logger};
	player.Spawn();
}
```

#### Property Injection

```cpp
struct Player {
	void SetLogger(ILogger* logger){
		this->logger = logger;
	}
	void Spawn() {
		if(logger)
			logger->Log("Player spawned!");
	}
private:
	ILogger* logger{};
};
```

```cpp
int main()
{
	ILogger* logger = new ConsoleLogger{};
	Player player{};
	player.SetLogger(logger);
	player.Spawn();
}
```

### Or Both

```cpp
struct Player {
	Player(ILogger* logger) : logger{logger} {}
	void SetLogger(ILogger* logger){
		this->logger = logger;
	}
	void Spawn() {
		if(logger)
			logger->Log("Player spawned!");
	}
private:
	ILogger* logger{};
};
```