#include <iostream>
#include <string>
#include "Item.h"

using namespace std;

///Item
Item::Item(string n, int v, int me)
:name(n), value(v), min_exp(me)
{}

void Item::printInfo()
{
    cout << "Name: " << name << ", costs: " << value << "\nRequires the hero to have at least " << min_exp << " experience." << endl;
}

int Item::getValue()
{
    return value;
}

int Item::getExp()
{
    return min_exp;
}

///Weapon
Weapon::Weapon(string n, int v, int me, int dmg, int oh)
:Item(n, v, me), damage(dmg), one_hand(oh)
{}

void Weapon::printInfo()
{
    cout << "\nType: Weapon." << endl;
    Item::printInfo();
    cout << "Damage: " << damage << ", ";
    if(one_hand==1){
        cout << "one-handed." << endl;
    }
    else{
        cout << "two-handed" << endl;
    }
}

int Weapon::getStat()
{
    return damage;
}

char Weapon::getType()
{
    return 'w';
}

///Armor
Armor::Armor(string n, int v, int me, int d)
:Item(n, v, me), defense(d)
{}

void Armor::printInfo()
{
    cout << "\nType: Armor" << endl;
    Item::printInfo();
    cout << "Defense value: " <<  defense << endl;
}

int Armor::getStat()
{
    return defense;
}

char Armor::getType()
{
    return 'a';
}

///Potion
Potion::Potion(string n, int v, int me, int am)
:Item(n, v, me), amount(am)
{}

void Potion::printInfo()
{
    cout << "\nType: Potion" << endl;
    Item::printInfo();
    cout << "-Will increase a random statistic by " << amount << endl;
}

char Potion::getType()
{
    return 'p';
}

int Potion::getStat()
{
    return amount;
}
