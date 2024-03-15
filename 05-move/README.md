# 5 Move

```c++
class Enemy {
    Item* item;

    // define a move constructor in which we move the item from other to us
    Enemy(const Enemy&& other) noexcept item{other.item} {
        other.item = nullptr;
    }
    
    // define a copy assignment operator
    Enemy& operator=(const Enemy&& other) {
        if (this == &other) return *this; // performance benefit if `a = a`
        if(item) delete item; // delete our old item
        item = other.item; // now move the other's item to us
        other.item = nullptr; // and remove it from other
        return *this;
    }
};
```

```c++
Enemy registeredEnemy;
// make a function that accepts a rvalue reference enemy
void registerSpawnedEnemy(Enemy&& enemy) {
    // move the enemy instead of copying them
   registeredEnemy = enemy;
}
```

In this chapter, we will learn about special situations in programming in which copying a value (which may consume a lot of resources in case of deep copies) can be avoided by moving a value instead.

We will learn:
- when the compiler can find out on its own, that a value can be moved
- how we can tell the compiler manually, that a value can be safely moved
- how we add support for moving values to our classes

## Move Semantics
- copying costs a lot of resources (CPU and RAM)
- sometimes, you only want to move an object from `a` to `b`
- moving an object means:
  - moving all the information from `a` to `b`
  - to further use `b`
  - and not use `a` anymore

## An extreme example

```c++
int main() {
    std::vector<Player> heroes;
    heroes.push_back(Player{"Hercules", new Item{"Golden Sword"}});
    heroes.push_back(Player{"Odysseus", new Item{"Wooden Sword"}});
    Player prometheus {"Odysseus", new Item{"Titan Sword"}};
    heroes.push_back(prometheus);
    return 0;
}
```

Output:
```
Item(Golden Sword)
Constructing Player()
Deep Copy Player(&): name: Hercules item: Golden Sword
~Player: name: Hercules item: Golden Sword
~Item(Golden Sword)
Item(Wooden Sword)
Constructing Player()
Deep Copy Player(&): name: Odysseus item: Wooden Sword
Deep Copy Player(&): name: Hercules item: Golden Sword
~Player: name: Hercules item: Golden Sword
~Item(Golden Sword)
~Player: name: Odysseus item: Wooden Sword
~Item(Wooden Sword)
Item(Titan Sword)
Constructing Player()
Deep Copy Player(&): name: Odysseus item: Titan Sword
Deep Copy Player(&): name: Hercules item: Golden Sword
Deep Copy Player(&): name: Odysseus item: Wooden Sword
~Player: name: Hercules item: Golden Sword
~Item(Golden Sword)
~Player: name: Odysseus item: Wooden Sword
~Item(Wooden Sword)
~Player: name: Odysseus item: Titan Sword
~Item(Titan Sword)
~Player: name: Hercules item: Golden Sword
~Item(Golden Sword)
~Player: name: Odysseus item: Wooden Sword
~Item(Wooden Sword)
~Player: name: Odysseus item: Titan Sword
~Item(Titan Sword)
```

We are creating:
- 4 Hercules instances
- 4 Golden Swords
- 3 Odysseus instances
- 3 Wooden Swords
- 2 Prometheus instances
- 1 Titan Sword

That's a lot of resources wasted!
- We only needed one copy of each

By learning about moving, we'll be able to reduce this to:

```
Item(Golden Sword)
Constructing Player()
Move Player(&): name: Hercules item: Golden Sword
~Player: name: <> item: <>
Item(Wooden Sword)
Constructing Player()
Move Player(&): name: Odysseus item: Wooden Sword
Move Player(&): name: Hercules item: Golden Sword
~Player: name: <> item: <>
~Player: name: <> item: <>
Item(Titan Sword)
Constructing Player()
Move Player(&): name: Odysseus item: Titan Sword
Move Player(&): name: Hercules item: Golden Sword
Move Player(&): name: Odysseus item: Wooden Sword
~Player: name: <> item: <>
~Player: name: <> item: <>
~Player: name: <> item: <>
~Player: name: Hercules item: Golden Sword
~Item(Golden Sword)
~Player: name: Odysseus item: Wooden Sword
~Item(Wooden Sword)
~Player: name: Odysseus item: Titan Sword
~Item(Titan Sword)

```

## A simpler example
Above Code is quite complex and understanding exactly how `vector::push_back` works is not so easy, so here's a simpler code snippet to continue with:

```c++
class World {
    Player _god;
public:
    World(Player god) : _god{ name } { }
};

int main() {
    Player zeus{"Zeus", new Item{"Lightning"}};
    World olymp{ zeus };
    World earth{ Player{"Hercules", new Item{"Golden Sword"}}};
    return 0;
}
```

Output:
```
Player() Zeus
Deep Copy Zeus
Deep Copy Zeus
~Player(): Zeus
Player() Hercules
Deep Copy Hercules
~Player(): Hercules
~Player(): Hercules
~Player(): Zeus
~Player(): Zeus
```

We have multiple issues in above code:

### Copy to Constructor Argument

```c++
    Player zeus{"Zeus"}; // Player is constructed: Zeus
    World olymp{ zeus }; // deep copy Player: Zeus from zeus -> god
```

Here, the `Player` gets copied when it gets passed as an Argument to he Hero-Constructor

### Copy to Class Data Member

```c++
    World(Player god) :
	    _god{ god } {  // deep copy Player: Zeus from god -> _god
	} // god (Zeus) gets deconstructed
```

Here, the `Player`-Argument `god` gets copied when it gets assigned to the World's Member Variable `_god`

When the Constructor ends, the `god` Argument is not used anymore and gets Destructed.

### No Copy to Constructor Argument

```c++
World earth{ Player{"Hercules", new Item{"Golden Sword"}}};
```

In the case of earth, no copy is created when the World-Constructor is invoked. This is because the Variable gets directly constructed at the right address of the argument. Cool!

### Copy to Class Member Variable (Again)

```c++
    World(Player god) :
	    _god{ god } {  // deep copy Player: Hercules from god -> _god
	} // god (Zeus) gets deconstructed
```

But the problem within the Hero Constructor still exists.


## Use Reference For Constructor Argument
First of all, we should use a reference as a constructor Argument.
- There is no need that the argument gets cloned when passed as a constructor argument
- if it gets cloned when being assigned to the member variable anyways:

```c++
    World(const Player& god) :
	_god{ god } {  // deep copy Player: Zeus

	} // Player Zeus gets deconstructed
```

```c++
    Player zeus{"Zeus", new Item{"Lightning"}}; // Player is constructed: Zeus
    World olymp{ zeus }; // zeus is passed as a reference, no more copy!
```

Output:
```
Player() Zeus
                                Deep Copy Zeus
Deep Copy Zeus
                                ~Player(): Zeus
Player() Hercules
Deep Copy Hercules
~Player(): Hercules
~Player(): Hercules
~Player(): Zeus
~Player(): Zeus
```

## Move Instead of Copy
Idea: Within the World's Constructor, we know, that the `god` value is used for nothing else but for assigning it to `_god`, so how about we use that knowledge and tell C++ to:
- not create a deep copy of the Player and their Item
- but instead:
  - move the Item from the argument to the member variable
  - set the Item on the argument to `nullptr`
    - so that, when the argument gets destructed
    - it won't destroy the Item (it's `nullptr`)

## Value Categories
There are many value types:
```
     expression
     /        \
  glvalue   rvalue
  /     \  /     \
lvalue xvalue prvalue
```

Generally, you can group the values by:
- do they have an identity? i.e. a variable name?
- are they movable

### LValue

`lvalue`: a value with an identity, a variable
- Named so, because it can stand on the left side of an assignment
- Usually can't be moved
- because you might want to continue using the original value:

```c++
Player zeus{"Zeus"}; // zeus is an lValue
World olymp{zeus}; // the value can't be moved instead of copied...
zeus.isAngry = true; // ... because we might still use/manipulate zeus

zeus = Player{"Odysseus"}; // zeus can stand on the left side of an assignment
Player odysseus = zeus; // but also on the right side
```

### Prvalue

`prvalue`: a pure right value, which means, a value that can never stand on the left side of an assignment, e.g.:

```c++
Player{"Zeus"}; // this is a pr value

Player zeus = Player{"Zeus"}; // it can stand on the right side...
Player {"Zeus"} = zeus; // ... but not on the left side of an assignment

5; // this one also is a pr value
5 = i; // also not possible
```

### XValue

`xvalue`: these are more problematic
- These are values that can be moved, even though they have an identity
- The Compiler can't be sure, whether the value can be moved or not
- so we need to specify it:

```c++
World olymp{};
{
	Player zeus{"Zeus"}; // we create a variable
	olymp = World{zeus}; // and we only use want to pass it on to the World constructor
	// we know, that the value can be moved, because we don't use zeus variable anymore after that
}
```

But the compiler can't know this on its own.
- we can tell the compiler that the variable can be safely moved:

```c++
World olymp{};
{
	Player zeus{"Zeus"}; // we create a variable
	olymp = World{std::move(zeus)}; // and we only use want to pass it on to the World constructor
	// we know, that the value can be moved, because we don't use zeus anymore after that
}
```

## References
When we define reference arguments, there is two ways of defining them:

### LValue Reference
```c++
#include <iostream>
void refType(int& x) {
	cout << "lvalue reference " << x << "\n";
}
```

### RValue Reference
```c++
void refType(int&& x) {
	cout << "rvalue reference " << x << "\n";
}
```

### Sample Code:
```c++
int main() {
	auto x = 1;
	refType(x); // lvalue
	refType(2); // rvalue
	refType(x+2); // rvalue
}
```

## std::move
`std::move` allows you to cast an lvalue type to rvalue type

```c++
#include <utility>
/*...*/
refType(std::move(x));
```

You should not use moved-from objects except to reassign or destruct them.

We can use this knowledge to improve our World Constructor. We add a new RValue Reference Constructor:
```c++
    World(Player&& god) noxexcept :
	    _god{ std::move(god) } {  // tell the compiler, that `god` can be safely moved
	} // Player (empty) gets deconstructed
```

### Move Constructor

But we also need to define a Move Constructor in our Player class. Else, the compiler won't know, how to move our class and create copies instead:

```c++
Player(Player&& other) noexcept { // noexcept is necessary, because else the compiler will prefer using the Player& constructor
	printf("Moving %s\n", other.buffer);
	// move all arguments from the other player to this player
	// set the arguments to null on the other player
	// to ensure that the other player won't delete our references when it gets destructed
}
```

Don't forget to add `nullptr` checks in the Player's destructor now:

```c++
~Player(){
	// ...
	if(item) delete[] item;
	// ...
}
```

Result:
```
Player() Zeus
                                Deep Copy Zeus
Deep Copy Zeus
                                ~Player(): Zeus
Player() Hercules
Move Hercules          -----    Deep Copy Hercules
~Player(): <>          -----    ~Player(): Hercules
~Player(): Hercules
~Player(): Zeus
~Player(): Zeus
```

Great, this fixed the issue for Player Hercules. This is, because it is an RValue
- it has no identity, no variable name.

But Zeus is an LValue
- it has an identity, a variable name `zeus`

But we can convert it using `std::move`:

```c++
int main() {
    Player zeus{"Zeus", new Item{"Lightning"}};
    World olymp{ zeus };
    World earth{ Player{"Hercules", new Item{"Golden Sword"}}};
    return 0;
}
```

Result:
```
Player() Zeus
                                Deep Copy Zeus
Move Zeus              -----    Deep Copy Zeus
                                ~Player(): Zeus
Player() Hercules
Move Hercules          -----    Deep Copy Hercules
~Player(): <>          -----    ~Player(): Hercules
~Player(): Hercules
~Player(): Zeus
~Player(): <>          -----   ~Player(): Zeus
```

## Move Assignment
Furthermore, you should define the move assignment operator, for cases like the following:

```c++
	Player zeus{"Zeus", 7};
	Player hercules{"Hercules", 7}; // here, hercules gets constructed
	// moves value from zeus to hercules using move assignment operator:
	hercules = std::move(zeus);
	// should not use value of zeus anymore!

```

This is how to:
```c++
Player& operator=(Player&& other) noexcept { //noexcept is needed again
	if(this == other) return *this;
	// clean up own values
	// assign other values to own values
	// set other values to null
	return *this;
}
```

## Compiler-Generated Methods
- Destructor
- Copy Constructor
- Move Constructor
- Copy Assignment Operator
- Move Assignment Operator

If you don't define any of these
- then all of these will be generated by the compiler. 

If you define any of these
- only some of the other methods will be generated by the compiler.

You can explicitly ask the compiler to generate some of these methods (or not):

```c++
Player(Player&& other) noexcept = default; // explicitly ask for default generated method
Player(Player& other) = delete; // explicitly remove generation
```


# Quiz

[Link to the Quiz](https://forms.gle/XXweScEiStrnRwY38)

# Exercises
- Implement Move Semantics into your `String` class
  - What does it mean to move the values of your `String`?
  - Think especially about the `buffer`!
  - Implement both the Move Constructor and Move Assignment Operator