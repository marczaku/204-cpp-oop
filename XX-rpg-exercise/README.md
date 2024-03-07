# RPG Exercise

## Introduction
You joined a team that's a bout to develop a Console RPG Game.\
They have already prepared a basic prototype.\
You are to join the team and help them add some more interesting characters to the game.

[Full Source Code](rpg.cpp)

## Documentation

### Unit

#### Private members
- `const char* name_;`: The name of this Unit.
- `int health_;`: The current health of this Unit.

#### Public Members

`explicit unit(const char* name, const int health)`
- A Constructor that takes both
  - `const char* name`
  - `int health` (Clamped between 0 and 100)
- It prints That the Unit spawned using colored Text.
  - `\033[1;32m` marks that text with color `32` starts
  - `\033[0m` marks that colored text ends

```c++
explicit unit(const char* name, const int health) : name_{name}
{
    health_ = std::clamp(health, 0, 100);
    std::cout << name << " \033[1;32mspawned\033[0m with \033[1;34m" << health << " Health\033[0m.\n";
}
```

`~unit()`
- In the Destructor, we print that the Unit died.

```c++
~unit()
{
    std::cout << name_ << " \033[1;31mdied\033[0m.\n";
}
```

`void attack(unit& other)`
- Allows the Unit to attack another Unit.
- It prints the Attack to the Console.
- Currently, this Method always deals one Damage
  - `other.set_health(other.get_health()-1)`

```c++
void attack(unit& other)
{
    std::cout << name_ << " \033[1;31mattacks\033[0m " << other.name_ << ".\n";
    other.set_health(other.get_health()-1);
}
```

`bool is_dead() const`
- Returns `true` if the Health is `0` or less.

```c++
bool is_dead() const {return health_ <= 0;}
```

`int get_health() const`
- Returns the current Health (which is `private` to prevent tampering)

```c++
int get_health() const { return health_;}
```

`void set_health(const int value)`
- Allows changing the Health using Args:
  - `const int value`: The new Value of Health. Clamped between 0 and 100.
- Prints the Health Change to the Console.

```c++
void set_health(const int value)
{
    health_ = std::clamp(value, 0, 100);
    std::cout << name_ << " now has \033[1;34m" << health_ << " Health\033[0m.\n";
}
```

### Game

#### Create Hero
- Spawns Hero Unit with 100 Health and a Golden Name.
```c++
unit* create_hero()
{
    return new unit("\033[1;33mHero\033[0m", 100);
}
```

#### Create Unit
- Spawns an Enemy Unit with 3 Health.
```c++
unit* create_enemy()
{
    return new unit("Enemy", 3);
}
```

#### Initialization
- Creates the Hero and assigns him to local variable
- Creates an Enemy and assigns him to local variable
- Initializes Monster Kill Counter with `0`
- Stores a variable to check later whether user wants to continue
```c++
// initialization
unit* hero = create_hero();
unit* enemy = create_enemy();
int killed_monster_count{};
char should_continue;
```

#### Game Loop
- Game Loops at least one time
- And then continues if the user entered `y` when asked to continue
```c++
// game loop
do
{
    // ...
} while (should_continue == 'y');
```

#### Hero's Turn
- Sleep causes the Console to wait e.g. `400ms` for a while.
- The Hero attacks the enemy.
```c++
// hero's turn
Sleep(400);
hero->attack(*enemy);
Sleep(800);
```

#### Enemy Respawn
- If the Enemy has Died, then...
- The Enemy is deleted (which invokes the Destructor)
- Output is printed that states how many monsters were killed
  - This also increases `killed_monster_count` by one (`++`)
- It spawns a new Enemy using `create_enemy()`
```c++
if(enemy->is_dead())
{
    // spawn new enemy
    delete enemy;
    std::cout << "\033[1;36mYou killed a total of " << ++killed_monster_count << " Monsters!\033[0m\n";
    std::cout << "\n";
    enemy = create_enemy();
}
```

#### Enemy Turn
- If the Enemy did not die previously...
- Then he strikes back at the Hero
```c++
else // (if !enemy->is_dead())
{
    // enemy's turn
    std::cout << "\n";
    enemy->attack(*hero);
}
```

#### Choice to Continue
- The user is asked whether he wants to continue.
- The input is stored in `should_continue`
```c++
// choice to continue
Sleep(800);
std::cout << "\n";
std::cout << "Do you want to continue? y/n\n";
std::cin >> should_continue
```

## Your Turn!

Now it's your turn to add new Features to this Game.

### Skeleton Class

```
Skeleton spawned with 4 Health.
Hero attacks Skeleton.
The attack is very effective!
Skeleton now has 2 Health.

Skeleton attacks Hero.
Hero now has 99 Health.

Hero attacks Skeleton.
The attack is very effective!
Skeleton now has 0 Health.
Skeleton died.
```

Add a Skeleton Class!
- it inherits from `Unit`
- it has a parameter-less constructor
  - that assigns the `name` and `health` as you like to the `Unit`-constructor
- it takes double damage from all attacks. To achieve this:
  - add new `virtual` method `takeDamage(int)` to `Unit` class
  - `Unit`'s implementation reduces `health` by `int` argument
  - `Skeleton` `override`s the implementation and
    - outputs that the attack is very effective to the console
    - invokes the base Method passing the double value of `int` argument

### Infected Class

```
Infected spawned with 2 Health.
Hero attacks Infected.
Infected now has 1 Health.

Infected attacks Hero.
Hero now has 99 Health.

Hero attacks Infected.
Infected now has 0 Health.
Infected died.
Infected came back from the dead as Zombie with 1 Health.

Zombie attacks Hero.
hero now has 98 Health.

Hero attacks Zombie.
Zombie now has 0 Health.
Zombie died.
```

Add a Infected Class!
- it inherits from `Unit`
- it has a parameter-less constructor
  - that assigns the `name` and `health` as you like to the `Unit`-constructor
- it can revive itself once. To achieve this:
  - override `takeDamage(int)` and invoke the `Unit`'s implementation
  - if after that, the Infected `is_dead()`:
    - print a message saying that the Infected has risen from the dead.
    - set the `health` to `1`, change the `name`
    - and make sure to "remember" that the Infected has come back from the dead
    - so it won't do it again next time it dies.

### Random Spawn
Change `create_enemy` so it returns a random enemy (`Skeleton` or `Infected`). Expand the function as you add new enemy types.