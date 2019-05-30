#include <iostream>
#include <string>
#include "Spell.h"

using namespace std;

///Spell
Spell::Spell(string n, int v, int me, int mr, int mid, int mad)
:name(n), value(v), min_exp(me), mp_req(mr), min_dmge(mid), max_dmge(mad)
{}

void Spell::printInfo()
{
    cout << "Name: " << name << ", costs: " << value << "\nRequires the hero to have at least " << min_exp << " experience." << endl;
    cout << "Requires " << mp_req << " magic power, and deals [" << min_dmge << "," << max_dmge << "] damage." << endl;
}

int Spell::getValue()
{
    return value;
}

int Spell::getExp()
{
    return min_exp;
}

int Spell::getMPReq()
{
    return mp_req;
}


///IceSpell
IceSpell::IceSpell(string n, int v, int me, int mr, int mid, int mad)
:Spell(n, v, me, mr, mid, mad)
{}

void IceSpell::printInfo()
{
    cout << "\nType: Ice spell" << endl;
    Spell::printInfo();
}

void IceSpell::cast(int dex, int& h, int& mid, int& mad, int& def, int& ag)
{
    int damage = min_dmge + ((max_dmge-min_dmge)*dex/100);                  //find the percentage
    h -= damage;

    mid -= 15;
    mad -= 15;
    cout << this->name << " cast:   -" << damage << endl;
    cout << "Lowered enemy's damage range by 15" << endl;           ///!
}

///FireSpell
FireSpell::FireSpell(string n, int v, int me, int mr, int mid, int mad)
:Spell(n, v, me, mr, mid, mad)
{}

void FireSpell::printInfo()
{
    cout << "\nType: Fire spell" << endl;
    Spell::printInfo();
}

void FireSpell::cast(int dex, int& h, int& mid, int& mad, int& def, int& ag)
{
    int damage = min_dmge + ((max_dmge-min_dmge)*dex/100);                  //find the percentage
    h -= damage;

    h -= def;
    def -= 10;
    h += def;       ///extract defense and update it-> its like lowering defense.
    cout << this->name << " cast:   -" << damage << endl;
    cout << "Lowered enemy's defense by 10" << endl;
}

///LightningSpell
LightningSpell::LightningSpell(string n, int v, int me, int mr, int mid, int mad)
:Spell(n, v, me, mr, mid, mad)
{}

void LightningSpell::printInfo()
{
    cout << "\nType: Lightning spell" << endl;
    Spell::printInfo();
}

void LightningSpell::cast(int dex, int& h, int& mid, int& mad, int& def, int& ag)
{
    int damage = min_dmge + ((max_dmge-min_dmge)*dex/100);                  //find the percentage
    h -= damage;

    ag -= 15;
    cout << this->name << " cast:   -" << damage << endl;
    cout << "Lowered enemy's agility by 15" << endl;
}
