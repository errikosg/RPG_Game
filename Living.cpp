#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Living.h"
#include "Spell.h"
#include "Item.h"

using namespace std;

///Living
Living::Living(string n, int h)
:name(n), healthPower(h)
{
    level = 1;
    base_health = healthPower;
}

void Living::printInfo()
{
    cout << "Name: " << name << ", Level: " << level << "\nStatistics:\n\tHealth Power: " << healthPower;
}

int& Living::getHealth()
{
    return healthPower;
}

string Living::getName()
{
    return this->name;
}

///Hero
Hero::Hero(string n, int h, int mP, int str, int dex, int ag)
:Living(n, h), magicPower(mP), strength(str), dexterity(dex), agility(ag)
{
    money = 0;
    experience = 50;
    exp_cap = 100;
    up_value = 5;
    agility_cap = 80;                   //agility(dodge ratio) can be at most 80%.
    damage = strength;                  //basic damage the hero deals (if weapon is equipped, damage goes up but strength doesn't change
    base_magicPower = magicPower;       //this wont change.

    //capacity will always be 20;
    capacity = 20;
    item_count = 0;

	//NOT
	weap_pos=-1;
	armor_pos=-1;
	weapon=NULL;
	armor=NULL;
}

void Hero::printInfo()
{
    Living::printInfo();
    cout << "\n\tMagic Power: " << magicPower <<  "\n\tAttack Power: " << damage << "\n\tDexterity: "         //strength=strength+weapon
        << dexterity << "\n\tAgility: " << agility << endl;
    cout << "Money: " << money << ", Experience: " << experience << endl;
    ///this->checkInventory();
}

void Hero::getExp(int e)
{
    experience += e;
    while(experience >= exp_cap){
        //level up
        this->levelUp();
        exp_cap *= 2;       //double exp cap for next level;
    }
}

void Hero::getMoney(int m)
{
    money += m;
}

void Hero::checkInventory()
{
    cout << "Inventory:\n*-------------------------------------------------*" << endl;
    unsigned int i;
	int index=1;
    for(i=0; i<item.size(); i++){
		cout << index << ") ";
        item[i]->printInfo();
		index++;
		cout << endl;
    }
    cout << "*-------------------------------------------------*" << endl;
}

int Hero::getLevel()
{
	return this->level;
}

void Hero::equip()
{
    unsigned int i, sel, c_flag=0;
    int cw=0, ca=0;
    string answer1, answer2;
    //check for available items in inventory.
    for(i=0; i<item.size(); i++){
        if(item[i]->getType()=='w'){
            cw++;
        }
        if(item[i]->getType()=='a'){
            ca++;
        }
    }

    //weapons
    if(cw < 1){
        cout << "\nYou have no available weapons." << endl;
    }
    else{
        cout << "\nSelect weapon:" << endl;
        for(i=0; i<item.size(); i++){
            if(item[i]->getType()=='w'){
                cout << "(" << i+1 << ") ";
                item[i]->printInfo();
            }
        }
        cin >> sel;
        //check for right input
        while(sel<0 || sel>item.size() || item[sel-1]->getType()!='w' || item[sel-1]->getExp()>this->experience){
            if(sel<0 || sel>item.size() || item[sel-1]->getType()!='w'){
                cout << "\nError. Choose again(type the corresponding number)\n" << endl;
                for(i=0; i<item.size(); i++){
                    if(item[i]->getType()=='w'){
                        cout << "(" << i+1 << ") ";
                        item[i]->printInfo();
                    }
                }
                cin >> sel;
            }
            else if(item[sel-1]->getExp()>this->experience){
                cout << "You can't equip this item! Requires " << item[sel-1]->getExp() << " experience, whilst you have " << experience << endl;
                cout << "Want to choose again?(y/n)" << endl;
                cin >> answer1;
                if(answer1=="y" || answer1=="yes" || answer1=="Yes" || answer1=="YES"){
                    for(i=0; i<item.size(); i++){
                        if(item[i]->getType()=='w'){
                            cout << "(" << i+1 << ") ";
                            item[i]->printInfo();
                        }
                    }
                    cin >> sel;
                }
                else{
                    c_flag = 1;                 ///flag to check if the player wants to actually equip an item.
                    break;
                }
            }
        }
        if(c_flag==0){
            weapon = item[sel-1];
			weap_pos = sel-1;
            damage += weapon->getStat();
            cout << "--Equipped--" << endl;
        }
    }

    //armors
    c_flag=0;
    if(ca < 1){
        cout << "You have no available armor." << endl;
    }
    else{
        cout << "Select armor:" << endl;
        for(i=0; i<item.size(); i++){
            if(item[i]->getType()=='a'){
                cout << "(" << i+1 << ") ";
                item[i]->printInfo();
            }
        }
        cin >> sel;
        //check for right input
        while(sel<0 || sel>item.size() || item[sel-1]->getType()!='a' || item[sel-1]->getExp()>this->experience){
            if(sel<0 || sel>item.size() || item[sel-1]->getType()!='a'){
                cout << "\nError. Choose again\n" << endl;
                for(i=0; i<item.size(); i++){
                    if(item[i]->getType()=='a'){
                        cout << "(" << i+1 << ") ";
                        item[i]->printInfo();
                    }
                }
            }
            else if(item[sel-1]->getExp()>this->experience){
                cout << "You can't equip this item! Requires " << item[sel-1]->getExp() << " experience, whilst you have " << experience << endl;
                cout << "Want to choose again?(y/n)" << endl;
                cin >> answer2;
                if(answer2=="y" || answer2=="yes" || answer2=="Yes" || answer2=="YES"){
                    for(i=0; i<item.size(); i++){
                        if(item[i]->getType()=='a'){
                            cout << "(" << i+1 << ") ";
                            item[i]->printInfo();
                        }
                    }
                    cin >> sel;
                }
                else{
                    c_flag = 1;                 ///flag to check if the player wants to actually equip an item.
                    break;
                }
            }
            cin >> sel;
        }
        if(c_flag==0){
            armor = item[sel-1];
			armor_pos = sel-1;
            healthPower += armor->getStat();
            //base_health = healthPower;
            cout << "--Equipped--" << endl;
        }
    }
}

void Hero::use()
{
    ///a potion will randomly change one of your stats.
    int counter=0;
    unsigned int i, sel;
    //check if any potions left.
    for(i=0; i<item.size(); i++){
        if(item[i]->getType()=='p'){
            cout << "(" << i+1 << ") ";
            item[i]->printInfo();
            counter++;
        }
    }

    if(counter==0){
        cout << "No potions left!" << endl;
        return;
    }
    else{
        cin >> sel;
        while(sel<0 || sel>item.size() || item[sel-1]->getType()!='p'){
            cout << "\nError. Choose again\n" << endl;
            for(i=0; i<item.size(); i++){
                if(item[i]->getType()=='p'){
                    cout << "(" << i+1 << ") ";
                    item[i]->printInfo();
                }
            }
            cin >> sel;
        }
        srand(time(NULL));
        int r = rand() % 3;

        if(r == 0){
            strength += item[sel-1]->getStat();
            cout << "\n-->Your Strenth increased by " << item[sel-1]->getStat() << endl;
        }
        else if(r == 1){
            dexterity += item[sel-1]->getStat();
            cout << "\n-->Your Dexterity increased by " << item[sel-1]->getStat() << endl;
        }
        else{
            agility += item[sel-1]->getStat();
            cout << "\n-->Your Agility increased by " << item[sel-1]->getStat() << endl;
        }
        item.erase(item.begin() + (sel-1));

    }
}

void Hero::attack(int& health, int ag)
{
    srand(time(NULL)+health);
    int r = rand() % 100 + 1;
    if(r < ag){
        cout << "Enemy dodged " << this->name << "'s attack!" << endl;
    }
    else{
        health -= damage;
        cout << "\n--" << this->name << " attacks:   -" << damage << endl;
    }
}

void Hero::buy(Item *i)
{
    if(i->getValue() > money){
        cout << "\n-You have not enough money to purchase this!" << endl;
        return;
    }
    else if(item_count==capacity){
        cout << "\n-You dont have enough space in your inventory!" << endl;
    }
    else{
        item.push_back(i);
        cout << "\nPurchased item: " << endl;
        cout << "___________________________";
        i->printInfo();
        cout << "___________________________" << endl;
        item_count++;
		money -= i->getValue();
    }
}

void Hero::buy(Spell *s)
{
    if(s->getValue() > money){
        cout << "\n-You have not enough money to purchase this!" << endl;
        return;
    }
    else if(item_count==capacity){
        cout << "\n-You dont have enough space in your inventory!" << endl;
    }
    else{
        spell.push_back(s);
        cout << "\nPurchased spell: " << endl;
        cout << "___________________________";
        s->printInfo();
        cout << "___________________________" << endl;
        item_count++;
		money -= s->getValue();
    }
}

void Hero::sell(int index)
{
	//'sell' item in index position of items array

	int temp = item[index]->getValue();
	temp = temp/2;
	money += temp;

	if(index == weap_pos){
		weapon = NULL;
		damage = strength;
		weap_pos=-1;
	}
	else if(index == armor_pos){
		armor = NULL;
		healthPower = base_health;
		armor_pos=-1;
	}

	item.erase(item.begin() + index);
	cout << "--Item Sold!" << endl;
	item_count--;

	//move positions
	if(weap_pos>index)
		weap_pos--;
	if(armor_pos>index)
		armor_pos--;

}

void Hero::castSpell(int& h, int& mid, int& mad, int& def, int& ag)
{
    if(spell.size()==0){
        cout << "-You dont have any spells to use!" << endl;
    }
    else{
        unsigned int i, sel, c_flag=0;
        string answer;

        cout << "\n-Which spell will you use?" << endl;
        for(i=0; i<spell.size(); i++){
            cout << "(" << i+1 << ") ";
            spell[i]->printInfo();
        }
        cin >> sel;
        //check for errors-rare occasions
        while(sel<0 || sel>spell.size() || spell[sel-1]->getExp()>this->experience || spell[sel-1]->getMPReq()>magicPower){
            if(sel<0 || sel>spell.size()){
                cout << "\nError. Choose again\n" << endl;
                for(i=0; i<spell.size(); i++){
                    cout << "(" << i+1 << ") ";
                    spell[i]->printInfo();
                }
                cin >> sel;
            }
            else{
                if(spell[i]->getExp()>this->experience){
                    cout << "You cant use this spell! It requires " << spell[i]->getExp() << " experience, whilst you have " << experience << endl;
                }
                else{
                    cout << "You cant use this spell! Insufficient Magic Power! (" << magicPower << ")" << endl;
                }
                cout << "Want to choose again?(y/n)" << endl;
                cin >> answer;;
                if(answer=="y" || answer=="yes" || answer=="Yes" || answer=="YES"){
                    for(i=0; i<spell.size(); i++){
                        cout << "(" << i+1 << ") ";
                        spell[i]->printInfo();
                    }
                    cin >> sel;
                }
                else{
                    c_flag=1;
                    break;
                }
            }
        }
        if(c_flag==0){
            spell[sel-1]->cast(dexterity, h, mid, mad, def, ag);
            magicPower -= spell[sel-1]->getMPReq();
        }
    }
}

void Hero::resetStats()
{
    //reset fully
    magicPower = base_magicPower;
    if(armor_pos != -1){
        healthPower = base_health + armor->getStat();
    }
    else{
        healthPower = base_health;
    }
}

int Hero::getAg()
{
    return this->agility;
}

int Hero::getDmge()
{
	return this->damage;
}

int Hero::getItemCount()
{
	return item_count;
}

void Hero::loseMoney()
{
    money = money/2;
}


//Warrior
Warrior::Warrior(string n, int h, int mP, int str, int dex, int ag)
:Hero(n, h, mP, str+20, dex, ag)						//!
{
    agility += 5;
}

void Warrior::printInfo()
{
    cout << "\nWarrior: " << endl;
    Hero::printInfo();
}

void Warrior::levelUp()
{
	//health
    base_health += 2*base_health/3;						//get a little bit more health
	if(armor_pos != -1){
		healthPower = base_health + armor->getStat();
	}
	else{
		healthPower = base_health;
	}

	//mp
    base_magicPower += base_magicPower/2;
    magicPower = base_magicPower;

	//strength
    strength += up_value*2;								//strong stat
	if(weap_pos != -1){
		damage = strength + weapon->getStat();
	}
	else{
		damage = strength;
	}

	//dexterity
    dexterity += up_value;
	//agility
    if((agility+(up_value/2)) <= agility_cap)
        agility += up_value/2;
    else
        agility = agility_cap;

    up_value += (up_value/2);
    level++;
    cout << "\n-Your Hero Leveled Up!\n\t"<< this->name << "\t" << level-1 << " -> " << level << endl;
    this->printInfo();
}


///Sorcerer
Sorcerer::Sorcerer(string n, int h, int mP, int str, int dex, int ag)
:Hero(n, h, mP, str, dex, ag)
{
    dexterity += 20;
    agility += 10;
}

void Sorcerer::printInfo()
{
    cout << "\nSorcerer: " << endl;
    Hero::printInfo();
}

void Sorcerer::levelUp()
{
	//health
    base_health += base_health/2;
	if(armor_pos != -1){
		healthPower = base_health + armor->getStat();
	}
	else{
		healthPower = base_health;
	}

	//mp
    base_magicPower += 2*base_magicPower/3;					//gain a little bit more mp
    magicPower = base_magicPower;

	//strength
    strength += up_value;
	if(weap_pos != -1){
		damage = strength + weapon->getStat();
	}
	else{
		damage = strength;
	}

	//dexterity
    dexterity += up_value*2;								//strong stat
	//agility
    if((agility+(up_value/2)) <= agility_cap)
        agility += up_value/2;
    else
        agility = agility_cap;

    up_value += (up_value/2);
    level++;
    cout << "\n-Your Hero Leveled Up!\n\t"<< this->name << "\t" << level-1 << " -> " << level << endl;
    this->printInfo();
    cout << "\n";
}


///Paladin
Paladin::Paladin(string n, int h, int mP, int str, int dex, int ag)
:Hero(n, h, mP, str, dex, ag)
{
    strength += 15;
    dexterity += 15;
}

void Paladin::printInfo()
{
    cout << "\nPaladin: " << endl;
    Hero::printInfo();
}

void Paladin::levelUp()
{
	//health
    base_health += 2*base_health/3;
	if(armor_pos != -1){
		healthPower = base_health + armor->getStat();
	}
	else{
		healthPower = base_health;
	}

	//mp
    base_magicPower += 2*base_magicPower/3;
    magicPower = base_magicPower;

	//strength
    strength += 3*up_value/2;
	if(weap_pos != -1){
		damage = strength + weapon->getStat();
	}
	else{
		damage = strength;
	}

	//dexterity
    dexterity += 3*up_value/2;
	//agility
    if((agility+up_value) <= agility_cap)
        agility += up_value;
    else
        agility = agility_cap;

    up_value += (up_value/3);
    level++;
    cout << "\n-Your Hero Leveled Up!\n\t"<< this->name << "\t" << level-1 << " -> " << level << endl;
    this->printInfo();
}


///Monster
Monster::Monster(string n, int h, int mid, int mad, int def, int ag)
:Living(n, h), min_dmge(mid), max_dmge(mad), defense(def), agility(ag)
{
    base_min_dmge = min_dmge;
    base_max_dmge = max_dmge;
    base_defense = defense;
    base_agility = agility;

    base_health = healthPower;
    healthPower += defense;
}

void Monster::printInfo()
{
    Living::printInfo();
    cout << ", Damage range: [" << min_dmge << "," << max_dmge << "], Defense: " << defense << ", Agility: " << agility << endl;
}

void Monster::attack(int& h, int ag, string n)
{
    srand(time(NULL)+h);
    int r = rand() % 100 + 1;

    if(r < ag){
        cout << n << " dodged the attack!" << endl;
    }
    else{
        int d = rand() % ((max_dmge+1)-min_dmge) + min_dmge;
        h -= d;
        cout << "\n--" << this->name << " attacks:   -" << d << endl;
    }
}

void Monster::resetStats()
{
    min_dmge = base_min_dmge;
    max_dmge = base_max_dmge;
    defense = base_defense;
    agility = base_agility;

    cout << "-" << this->name << " stats refreshed!" << endl;
}

void Monster::fixLevel(int lvl)
{
    this->level = lvl;
}

int& Monster::getMax_dam()
{
    return max_dmge;
}

int& Monster::getMin_dam()
{
    return min_dmge;
}

int& Monster::getDef()
{
    return defense;
}

int& Monster::getAg()
{
    return agility;
}

///Dragon
Dragon::Dragon(string n, int h, int mid, int mad, int def, int ag)
:Monster(n, h, mid, mad, def, ag)
{
    mid += 15;
    mad += 15;
}

void Dragon::printInfo()
{
    cout << "\nDragon: " << endl;
    Monster::printInfo();
}

///Exoskeleton
Exoskeleton::Exoskeleton(string n, int h, int mid, int mad, int def, int ag)
:Monster(n, h, mid, mad, def, ag)
{
    defense += 20;
}

void Exoskeleton::printInfo()
{
    cout << "\nExoskeleton: " << endl;
    Monster::printInfo();
}

///Spirit
Spirit::Spirit(string n, int h, int mid, int mad, int def, int ag)
:Monster(n, h, mid, mad, def, ag)
{
    agility += 15;
}

void Spirit::printInfo()
{
    cout << "\nSpirit: " << endl;
    Monster::printInfo();
}
