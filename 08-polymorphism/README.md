# 8 Polymorphism

## Memory Layout

```c++
class Animal {
    int weight;
	int age;
};

class Dog : public Animal {
    int numberOfTricks;
	bool houseTrained;
};
```

```c++
void pet(Animal* animal) {/*...*/}

Animal* animal = new Dog{};

Dog* dog = new Dog{};
pet(dog);
```

![inheritance-memory-layout.png](inheritance-memory-layout.png)


- Allows for Polymorphism
- At the Address of `Dog`
- You find the Data of type `Animal`
- i.e. `Dog` IS AN `Animal`

### Conversion
- If you use the base class as a value...
- Type `Dog` "shrinks" to Type `Animal`:
```c++
Animal animal = Dog{};
```

![polymorphism-value-types.png](polymorphism-value-types.png)

### Inheritance Trees

```c++
class Chihuahua : public Dog {
  public:
    bool lovesBarking;
}
```

![inheritance-trees-memory-layout.png](inheritance-trees-memory-layout.png)

### Hiding Members
- Early Binding

```c++
struct Animal {
    void MakeSound() { cout << "...\n"; }
};

struct Dog : Animal {
    void MakeSound() { cout << "Woof!\n"; }
};
```

```c++
int main()
{
    Animal* dog = new Dog{};
    dog->MakeSound();
}
```

Remember it is the same as:

```c++
Animal* dog = new Dog{};
Animal::MakeSound(dog);
```

This will be solved by Virtualization!

## Multiple Inheritance

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

![multiple-inheritance-memory-layout.png](multiple-inheritance-memory-layout.png)

### Memory Layout
No Pointer Identity!
- `class` Bird
  - `flySpeed`;
- `class` Horse
  - `walkingSpeed`;
- `isLegend`

```c++
Pegasus value{};
Pegasus* pegasus = &value;
Horse* walking = &value;
Bird* flying = &value;
```

This will be solved by Virtualization!

### Dreaded Diamond
Class `Pegasus` will end up having TWO `age` variables
- This is obviously undesired!
```c++
struct Animal {
	int age;
}
struct Bird : public Animal {
	int flySpeed;
};

struct Horse : public Animal {
	int walkSpeed;
};

struct Pegasus : Bird, Horse {
    bool isLegend;
}
```

![dreaded-diamond-memory-layout.png](dreaded-diamond-memory-layout.png)


# Quiz

[Link to the Quiz](https://forms.gle/K3zqzuGrDRP9EmwT6)