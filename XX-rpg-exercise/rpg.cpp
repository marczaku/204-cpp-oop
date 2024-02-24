
#include <algorithm>
#include <iostream>
#include<windows.h>

class unit
{
public:
    explicit unit(const char* name, const int health) : name_{name}
    {
        health_ = std::clamp(health, 0, 100);
        std::cout << name << " \033[1;32mspawned\033[0m with \033[1;34m" << health << " Health\033[0m.\n";
    }

    ~unit()
    {
        std::cout << name_ << " \033[1;31mdied\033[0m.\n";
    }

    void attack(unit& other)
    {
        std::cout << name_ << " \033[1;31mattacks\033[0m " << other.name_ << ".\n";
        other.set_health(other.get_health()-1);
    }

    bool is_dead() const {return health_ <= 0;}
    int get_health() const { return health_;}
    void set_health(const int value)
    {
        health_ = std::clamp(value, 0, 100);
        std::cout << name_ << " now has \033[1;34m" << health_ << " Health\033[0m.\n";
    }

private:
    const char* name_;
    int health_;
};

unit* create_hero()
{
    return new unit("\033[1;33mHero\033[0m", 100);
}

unit* create_enemy()
{
    return new unit("Enemy", 3);
}

int main()
{
    // initialization
    unit* hero = create_hero();
    unit* enemy = create_enemy();
    int killed_monster_count{};
    char should_continue;

    // game loop
    do
    {
        // hero's turn
        Sleep(400);
        hero->attack(*enemy);
        Sleep(800);

        if(enemy->is_dead())
        {
            // spawn new enemy
            delete enemy;
            std::cout << "\033[1;36mYou killed a total of " << ++killed_monster_count << " Monsters!\033[0m\n";
            std::cout << "\n";
            enemy = create_enemy();
        }
        else // (if !enemy->is_dead())
        {
            // enemy's turn
            std::cout << "\n";
            enemy->attack(*hero);
        }

        // choice to continue
        Sleep(800);
        std::cout << "\n";
        std::cout << "Do you want to continue? y/n\n";
        std::cin >> should_continue
    } while (should_continue == 'y');

    // good game
    std::cout << "Thanks for playing!" << std::endl;
}