#ifndef LIVING_H_INCLUDED
#define LIVING_H_INCLUDED

#include <string>
#include <vector>
#include "Spell.h"
#include "Item.h"

using namespace std;

class Living
{
    protected:
        string name;
        int level, healthPower;
        int base_health;
    public:
        Living(string n, int h);
        virtual void printInfo();
        int& getHealth();
        string getName();
};


//Heroes
class Hero: public Living
{
    protected:
        int magicPower, strength, dexterity, agility;               //stats
        int money, experience, exp_cap, up_value, damage;           //ex_cap = max exp before level up / up_value=value by which stats will be augmented

        vector<Item*> item;
        vector<Spell*> spell;										//items and spells
        int capacity, item_count;                                   //capacity of inventory, # of items present
        Item *weapon, *armor;                                       //equipped weapon/armor.
        int agility_cap, base_magicPower;                           //because agility is the dodge rate of the hero, it cant be unlimited.

		int weap_pos, armor_pos;
    public:
        Hero(string n, int h, int mP, int str, int dex, int ag);	//parameters are the stats above in the same order
        virtual ~Hero(){};
        virtual void printInfo();
        void getExp(int e);
        void getMoney(int m);
        virtual void levelUp()=0;					//hero gains level - all stats increased ("strong" stat increased twice as much)
		int getLevel();
        void checkInventory();
        void equip();                               //equip armor/weapons.
        void use();                                 //use potion.
        void attack(int& health, int ag);
        void buy(Item *i);
        void buy(Spell *s);                         //buy items/spells.
		void sell(int index);						//sell item - spells can't be sold
        void castSpell(int& h, int& mid, int& mad, int& def, int& ag);
        void resetStats();							//resets stats to base stats (if they got decreased)

        int getAg();								//get agility
		int getDmge();
		int getItemCount();
        void loseMoney();                      		//secondary
};

class Warrior: public Hero
{
    public:
        Warrior(string n, int h, int mP, int str, int dex, int ag);
        void printInfo();
        void levelUp();
};

class Sorcerer: public Hero
{
    public:
        Sorcerer(string n, int h, int mP, int str, int dex, int ag);
        void printInfo();
        void levelUp();
};

class Paladin: public Hero
{
    public:
        Paladin(string n, int h, int mP, int str, int dex, int ag);
        void printInfo();
        void levelUp();
};


//Monsters
class Monster: public Living
{
    protected:
        int min_dmge, max_dmge, defense, agility;                          	//main stats, can be decreased by the effect of a hero spell
        int base_min_dmge, base_max_dmge, base_defense, base_agility;     	//base stats, will never decrease by spell's effect
    public:
        Monster(string n, int h, int mid, int mad, int def, int ag);
        virtual ~Monster(){};
        virtual void printInfo();
        void attack(int& h, int ag, string n);
        void resetStats();													//resets stats to base stats (if they got decreased)
        void fixLevel(int lvl);
        int& getMin_dam();
        int& getMax_dam();
        int& getDef();
        int& getAg();
};

class Dragon: public Monster
{
    public:
        Dragon(string n, int h, int mid, int mad, int def, int ag);
        void printInfo();
};

class Exoskeleton: public Monster
{
    public:
        Exoskeleton(string n, int h, int mid, int mad, int def, int ag);
        void printInfo();
};

class Spirit: public Monster
{
    public:
        Spirit(string n, int h, int mid, int mad, int def, int ag);
        void printInfo();
};

#endif // LIVING_H_INCLUDED
