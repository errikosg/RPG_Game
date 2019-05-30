#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED

#include <string>

using namespace std;


class Item
{
    protected:
        string name;
        int value, min_exp;
    public:
        Item(string n, int v, int me);
        virtual ~Item(){};
        virtual void printInfo();
        virtual char getType()=0;
        virtual int getStat()=0;
        int getValue();
        int getExp();
};

class Weapon: public Item
{
    private:
        int damage, one_hand;       ///one_hand flag: 0 for two handed, 1 for one_handed.
    public:
        Weapon(string n, int v, int me, int dmg, int oh);
        int getStat();
        void printInfo();
        char getType();
};

class Armor: public Item
{
    private:
        int defense;
    public:
        Armor(string n, int v, int me, int d);
        int getStat();
        void printInfo();
        char getType();
};

class Potion: public Item
{
    private:
        int amount;
    public:
        Potion(string n, int v, int me, int am);
        int getStat();
        void printInfo();
        char getType();

};

#endif // ITEM_H_INCLUDED
