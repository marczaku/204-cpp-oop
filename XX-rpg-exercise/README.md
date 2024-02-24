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