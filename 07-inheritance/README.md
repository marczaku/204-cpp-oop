# 7 Inheritance
```c++
class GameObject {
protected:
    Vector3 position;
    
    void move(Vector3 direction);
}

class Player : public GameObject {
    // inherited position and move()
    int health;
    
    void attack(Player player);
}
```

## Syntax

```c++
class DerivedClassName : accessModifier BaseClassName {
    
};
```
The accessModifier can reduce the visibility of inherited members
- default for classes: `private`
- default for structs: `public`

| Base Access Modifier | Inheritance Access Modifier | Result      |
|----------------------|-----------------------------|-------------|
| `private`            | any                         | `private`   |
| `protected`          | `private`                   | `private`   |
| `protected`          | `public`, `protected`       | `protected` |
| `public`             | `private`                   | `private`   |
| `public`             | `protected`                 | `protected` |
| `public`             | `public`                    | `public`    |

## Constructor Chaining
Base Constructors always get called first
- Default Constructor, if not specified manually
```c++
class Animal {
    const char* name;
public:
    Animal(const char* name) : name{name} {}
};
```

```c++
class Dog : public Animal {
    const char* favoriteTrick;
public:
    Dog(const char* name, const char* favoriteTrick)
        : Animal{ name }, 
        favoriteTrick{ favoriteTrick } 
    {}
};
```

## Multiple Inheritance
Classes can inherit from multiple classes!
- Which can bring problems with it
```c++

struct Bird {
	int flySpeed;
};

struct Horse {
	int walkSpeed;
};

struct Pegasus : Bird, Horse {
	bool isLegend;
}
```

### Name Clashes
```c++
struct Bird {
	float speed;
};

struct Horse {
	float speed;
};
```

Need to be fully qualified

```c++
cout << pegasus.Bird::speed << "\n";
cout << pegasus.Horse::speed << "\n";
```

# Quiz

What access modifier is applied to inherited members by default if the data member access modifier is protected?
- A) private
- B) protected
- C) public
- D) It depends on whether the derived class is a class or a struct.

In the Dog class constructor, which base class constructor is called?

```c++
class Animal { }
class Dog : Animal {
    Dog(const char* name) {
    }
};
```

- A) `Dog()`
- B) `Animal()`
- C) None
- D) `Animal(const char* name)`

Which statement about multiple inheritance is true?
- A) It allows a class to inherit from only one base class.
- B) It always resolves potential name clashes automatically.
- C) It can bring problems such as name clashes.
- D) It is not supported in C++.

How can name clashes be resolved in a derived class inheriting from both Bird and Horse structures?
- A) By renaming the conflicting member variables in the base classes.
- B) By using access specifiers in the derived class.
- C) By fully qualifying the member variables when accessing them.
- D) By inheriting only from one of the base classes.

Which access modifier for the base class results in `private` inheritance access modifier regardless of the base access modifier?
- A) `private`
- B) `protected`
- C) `public`
- D) It depends on the derived class's data member access specifier.

In constructor chaining, what happens if a derived class constructor doesn't explicitly call a base class constructor?
- A) The program will not compile.
- B) The default constructor of the base class will be called automatically.
- C) The default constructor of the derived class will be called.
- D) It will result in a runtime error.