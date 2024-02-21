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

[Link to the Quiz](https://forms.gle/85oexPWLzkBfezNL9)