#include <iostream>
#include <limits>
#include <cstdlib>
#include <string>
#include <ctime>
#include <fstream>
#include "Grid.h"
#include "Living.h"
#include "Item.h"
#include "Spell.h"

using namespace std;

//TILES
Tile::~Tile(){}

//common
int CommonTile::isCommon()
{
    return 1;
}

int CommonTile::isMarket()
{
    return 0;
}

int CommonTile::isNonAcc()
{
    return 0;
}

//non-accessible
int NonAccTile::isCommon()
{
    return 0;
}

int NonAccTile::isMarket()
{
    return 0;
}

int NonAccTile::isNonAcc()
{
    return 1;
}

//market
int MarketTile::isCommon()
{
    return 0;
}

int MarketTile::isMarket()
{
    return 1;
}

int MarketTile::isNonAcc()
{
    return 0;
}

//GRID
Grid::Grid(int l, int h)
:length(l), height(h)
{
    grid = new Tile**[length];
    int i;
    for(i=0; i<length; i++){
        grid[i] = new Tile*[height];
    }

	srand(time(NULL));
	team_size = rand() % 3 + 1;
	hero = new Hero*[team_size];			//Hero!
	monster = new Monster*[team_size];		//Monster!
	game_flag=0;
	market_count=0;
}

void Grid::clear()
{
    //system("clear");
	int i;
	for(i=0; i<80; i++){
		cout << "\n";
	}
}

void Grid::makeMap()
{
    //i have a 2d array of tile pointers -> will randomly create tiles.
    int i, j, r;
    srand(time(NULL));

    for(i=0; i<length; i++){
        for(j=0; j<height; j++){
            r = rand() % 10 + 1;
            if(r<7 || (i==0 && j==0)){
                //common tile
                grid[i][j] = new CommonTile;
            }
            else if(r<9){
                //market tile
                grid[i][j] = new MarketTile;
				market_count++;
            }
            else{
                //non-accessible tile ->not at start
                grid[i][j] = new NonAccTile;
            }
        }
    }

    //position of hero team
    hero_pos[0] = 0;
    hero_pos[1] = 0;

    if(grid[0][1]->isNonAcc()==1 && grid[1][0]->isNonAcc()==1){
        //player cant move
        delete grid[0][1];
        grid[0][1] = new CommonTile;
    }

	//!!initialize market class
	if(market_count > 0){
		m = new Market(market_count);
		m->makeArrays();
	}
}

void Grid::displayMap()
{
    cout << "*--------------------------------------------------*" << endl;
    cout << "\nMap Legend:" << endl;
    cout << "\tAccessible tiles: -\n\tNon-Accessible tiles: x\n\tMarket: M\n\tYour Team: H\n" << endl;

    int i, j;
    cout << "\t";
    for(i=0; i<4*length; i++){
        cout << '_';
    }
    cout << "\n";
    for(j=0; j<height; j++){
        cout << "\t| ";
        for(i=0; i<length; i++){
            if(i==hero_pos[0] && j==hero_pos[1])
                cout << 'H';
            else if(grid[i][j]->isCommon()==1)
                cout << '-';
            else if(grid[i][j]->isMarket()==1)
                cout << 'M';
            else if(grid[i][j]->isNonAcc()==1)
                cout << 'x';
            if(i<length-1)
                cout << "   ";
        }
        if(j<height-1)
            cout << " |\n\n";
        else
            cout << " |\n";
    }
    cout << "\t";
    for(i=0; i<4*length; i++){
        cout << '_';
    }

    cout << "\n\n*--------------------------------------------------*" << endl;
}

void Grid::quitGame()
{
	game_flag = 1;
}

void Grid::move()
{
    int sel, j;
    int up=-1, down=-1, right=-1, left=-1;

    cout << "\nWhere do you want to go?\n" << endl;
    j=1;

    if(hero_pos[1]!=0 && grid[hero_pos[0]][hero_pos[1]-1]->isNonAcc()==0){
        cout << j << ".Up\t";
        up=j;
        j++;
    }
    if(hero_pos[1]!=height-1 && grid[hero_pos[0]][hero_pos[1]+1]->isNonAcc()==0){
        cout << j << ".Down\t";
        down=j;
        j++;
    }
    if(hero_pos[0]!=0 && grid[hero_pos[0]-1][hero_pos[1]]->isNonAcc()==0){
        cout << j << ".Left\t";
        left=j;
        j++;
    }
    if(hero_pos[0]!=length-1 && grid[hero_pos[0]+1][hero_pos[1]]->isNonAcc()==0){
        cout << j << ".Right\t";
        right=j;
        j++;
    }
    cout << "\t(press 0 to end game)\n";

    //i got j-1 choices available
    cin >> sel;
	if(sel == 0){
		this->quitGame();
		return;
	}

    while(sel<0 || sel>j-1){
        cout << "Error. Try again." << endl;
        cin >> sel;
    }
    if(sel==up)
        hero_pos[1] -= 1;
    else if(sel==down)
        hero_pos[1] += 1;
    else if(sel==left)
        hero_pos[0] -= 1;
    else if(sel==right)
        hero_pos[0] += 1;

}

void Grid::market()
{
	m->getData(it, sp);

	int index, i, sel, buy, h_index, s;									//sel=input for buy/sell, buy=input for which item to buy, h_index=input for which hero
	clear();
    cout << "******************************************************************" << endl;
	cout << "\n\tWelcome to the Market!" << endl;
	while(1){
		//main loop
		index=1;																							//here!
		cout << "\n\tDo you want to Buy or Sell items and spells?" << endl;
		cout << "\n\t1.Buy\t2.Sell\t\t(press 0 to exit)" << endl;
		cin >> sel;
		while(sel<0 || sel>2){
			cout << "->Error. Try again." << endl;
			cout << "\n\t1.Buy\t2.Sell\t\t(press 0 to exit)" << endl;
			cin >> sel;
		}

		//select hero to buy / sell, only if sel!=0.
		if(sel != 0){
			cout << "\n--Select the Hero you want to use for the market" << endl;
			for(i=0; i<team_size; i++){
				cout << index << "." << hero[i]->getName() << " " << endl;
				index++;
			}
			cin >> h_index;
			while(h_index<1 || h_index>team_size){
				cout << "Error. Try again." << endl;
				cin >> h_index;
			}
			cout << "\n------->Your Hero's info:" << endl;
			hero[h_index-1]->printInfo();
		}
		index = 1;

		if(sel==0){
			//exiting
			cout << "\tBye bye!" << endl;
			return;
		}
		else if(sel==1){
			//Buy
			cout << "\n<<<------------------------------------------------------>>>" << endl;
			cout << "\n\t\tITEMS\n" << endl;
			for(i=0; i<6; i++){
				cout << "(" << index << ")";
				index++;
				it[i]->printInfo();
			}
			cout << "\n\t\tSPELLS\n" << endl;
			for(i=0; i<4; i++){
				cout << "(" << index << ")";
				index++;
				sp[i]->printInfo();
			}

			cout << "\n--Which one do you want to purchase? (1-10 / 0 to exit)" << endl;
			cin >> buy;
			while(buy<0 || buy>10){
				cout << "Error. Try again." << endl;
				cin >> buy;
			}
			//buy the corresponding item.
			if(buy>=1 && buy<=6){
				hero[h_index-1]->buy(it[buy-1]);
			}
			else if(buy>6){
				hero[h_index-1]->buy(sp[buy-7]);					//!
			}
			cout << "\n<<<------------------------------------------------------>>>" << endl;
		}
		else{
			//Sell
			cout << "\n";
			hero[h_index-1]->checkInventory();
			if(hero[h_index-1]->getItemCount() > 0){
				cout << "--Which item do you want to sell? (type corresponding number / 0 to exit)" << endl;
				cin >> s;
				while(s<0 || s>hero[h_index-1]->getItemCount()){
					cout << "Error. Try again" << endl;
					cin >> s;
				}
				if(s>0){
					hero[h_index-1]->sell(s-1);
					hero[index-1]->printInfo();
				}
			}

		}

		//check if user want to continue using market.
		cout << "\n\n---Continue??\n1.Yes\t2.No" << endl;
		cin >> sel;
		while(sel<1 || sel>2){
			cout << "Error.Try again." << endl;
			cin >> sel;
		}
		if(sel==2){
			break;
		}
		clear();
	}
}

void Grid::action(int sel)
{
	//action player takes when in common tile.
	int h, i;

	cout << "\n-Select your Hero." << endl;
	for(i=0; i<team_size; i++){
		cout << i+1 << "." << hero[i]->getName() << endl;
	}
	cin >> h;
	while(h<1 || h>team_size){
		cout << "Error.Try again." << endl;
		cin >> h;
	}

	if(sel==1){
		//checkInventory
		hero[h-1]->checkInventory();
	}
	else if(sel==2){
		//printinfo
		hero[h-1]->printInfo();
	}
	else if(sel==3){
		//equip
		hero[h-1]->equip();
	}
	else if(sel==4){
		//use
		hero[h-1]->use();
	}

	cout << "\n\nPress Enter to continue." << endl;
	cin.ignore();
	cin.ignore();
}

void Grid::createTeam()
{
	srand(time(NULL));
	int i, r, index=0;
	string n[team_size];

	//create names -> 'randomly'
	ifstream file;
	file.open("text_files/names.txt");
	if(file.is_open()){
		while(getline(file, n[index])){
			if((r = rand() % 5 + 1) == 1){
				index++;
				if(index == team_size)
					break;
			}
		}
	}
	file.close();

	//create arrays with other attributes, give them respectively + heroes.
	int he=100, mp=100, s, d, a;
	for(i=0; i<team_size; i++){
		s = rand() % 11 + 10;
		d = rand() % 11 + 10;
		a = 5;						//!

		r = rand() % 3 + 1;
		if(r==1){
			hero[i] = new Warrior(n[i], he, mp, s, d, a);
		}
		else if(r==2){
			hero[i] = new Sorcerer(n[i], he, mp, s, d, a);
		}
		else{
			hero[i] = new Paladin(n[i], he, mp, s, d, a);
		}
		hero[i]->getMoney(100);
	}

}

void Grid::createEnemyTeam()
{
	int i, r, index=0, type, lvl=0;
	int h, mid, mad, def, ag, mean_dmge=0, mean_ag=0, mean_health=0;
	string n[team_size];
	ifstream file;
	srand(time(NULL));

	for(i=0; i<team_size; i++){
        lvl += hero[i]->getLevel();
		mean_dmge += hero[i]->getDmge();				//mean damage
		mean_ag += hero[i]->getAg();					//mean agility
		mean_health += hero[0]->getHealth();
	}
	lvl = lvl/team_size;
	mean_dmge = mean_dmge/team_size;
	mean_ag = mean_ag/team_size;
	mean_health = mean_health/team_size;			//fix stats!

	//make objects..
	for(i=0; i<team_size; i++){
		type = rand() % 3 + 1;
		if(type==1){
			//dragon
			file.open("text_files/monsters/dragons.txt");
		}
		else if(type==2){
			//exoskeleton
			file.open("text_files/monsters/exoskeletons.txt");
		}
		else if(type==3){
			//spirits
			file.open("text_files/monsters/spirits.txt");
		}
		//names
		if(file.is_open()){
			while(getline(file, n[index])){
				if((r=rand() % 3 + 1)==1){
					index++;
					break;
				}
			}
		}
		file.close();
		//stats
		h = rand() % 21 + (mean_health-10);
		mid = rand() % (mean_dmge*10/100) + (mean_dmge - mean_dmge*20/100);
		mad = rand() % (mean_dmge-mid+1) + (mid);
		def = rand() % (mean_health*30/100) + (mean_health*20/100);
		ag = rand() % (mean_ag*30/100) + (mean_ag*10/100);

		if(type==1){
			monster[i] = new Dragon(n[i], h, mid, mad, def, ag);
		}
		else if(type==2){
			monster[i] = new Exoskeleton(n[i], h, mid, mad, def, ag);
		}
		else{
			monster[i] = new Spirit(n[i], h, mid, mad, def, ag);
		}
	}
}

void Grid::clearTeam()
{
	//for enemies
	int i;
	for(i=0; i<team_size; i++){
		delete monster[i];
	}
}

void Grid::fight()
{
    int end_flag=0, hero_flag=0, enemy_flag=0, h_sel, m_sel, sel, act, i, j, round=1;
    int selected[team_size], index, spell_flag=0, spell_round, spell_target;
    int force_brk;      //break if no enemies

    cout << "\n\n******************************************************************" << endl;
    cout << "\nYou encountered enemies! The fight begins!!!\n" << endl;
    cout << "\n(press enter to continue)" << endl;
    cin.ignore(1, ' ');
    //cin.ignore();

    createEnemyTeam();

    while(end_flag==0){
        clear();
        cout << "\n\tROUND: " << round << endl;
        force_brk=0;
        round++;
        cout << "\n1.Display Stats\t2.Take Action\t0.End Game" << endl;
        cin >> sel;
        while(sel<0 || sel>2){
            cout << "Error.Try again." << endl;
            cin >> sel;
        }

        if(sel==0){
            this->quitGame();
            return;
        }
        else if(sel==1){
            //display stats
            cout << "\n\n\tHeroes:" << endl;
            for(i=0; i<team_size; i++){
                hero[i]->printInfo();
            }
            cout << "\n\n\tMonsters:" << endl;
            for(i=0; i<team_size; i++){
                monster[i]->printInfo();
            }
            cout << "\n(press enter to continue)" << endl;
            cin.ignore();
            cin.ignore();
            continue;
        }
        else{
            //take action
            //initialize selected
            for(i=0; i<team_size; i++){
                selected[i]=0;
            }

            for(j=0; j<team_size; j++){
                //--select hero
                cout << "\n-Select Hero:" << endl;
                index=-1;
                for(i=0; i<team_size; i++){
                    if(hero[i]->getHealth()>0){
                        cout << i+1 << ". " << hero[i]->getName() << endl;
                    }
                }
                cin >> h_sel;
                while(selected[h_sel-1]==1){
                    cout << "--You used this hero. Select again." << endl;
                    cin >> h_sel;
                }

                while(h_sel<1 || h_sel>team_size || hero[h_sel]->getHealth()<=0){		//if illegal number or hero dead
                    cout << "Error. Try again." << endl;
                    cin >> h_sel;
                }
                selected[h_sel-1]=1;

                //--action
                cout << "\n\nWhich action do you want to take? (choose only 1)" << endl;
                cout << "\t1.Attack\t2.Cast Spell\t3.Use\t4.Equip\t0.End Game" << endl;
                cin >> act;
                while(act<0 || act>4){
                    cout << "Error.Try again." << endl;
                    cin >> act;
                }
                if(act==0){
                    this->quitGame();
                    return;
                }
                else if(act==1 || act==2){
                    //attack - cast Spell
                    index = -1;
                    cout << "\n\n-Select enemy: " << endl;
                    for(i=0; i<team_size; i++){
                        if(monster[i]->getHealth()>0){
                            force_brk++;
                            cout << i+1 << ". " << monster[i]->getName() << endl;
                            if(index==-1){ index = i;}
                        }
                    }
                    if(force_brk==0)
                        break;
                    cin >> m_sel;
                    while(m_sel<index || m_sel>team_size){
                        cout << "Error. Try again." << endl;
                        cin >> m_sel;
                    }

                    if(act==1){
                        hero[h_sel-1]->attack(monster[m_sel-1]->getHealth(), monster[m_sel-1]->getAg());
                    }
                    else{
                        hero[h_sel-1]->castSpell(monster[m_sel-1]->getHealth(), monster[m_sel-1]->getMin_dam(), monster[m_sel-1]->getMax_dam(), monster[m_sel-1]->getDef(), monster[m_sel-1]->getAg());
                        spell_flag=1;
                        spell_round = round-1;
                        spell_target = m_sel;
                    }
                }
                else if(act==3){
                    //use
                    hero[h_sel-1]->use();
                }
                else{
                    //equip
                    hero[h_sel-1]->equip();
                }
                cout << "\n(press enter to continue)" << endl;
                cin.ignore();
                cin.ignore();
            }
            ///check if there are monsters living
            for(i=0; i<team_size; i++){
                if(monster[i]->getHealth()<=0){
                    enemy_flag=1;
                }
                else{
                    enemy_flag=0;
                    break;
                }
            }
            if(enemy_flag==0){
                cout << "Monsters' turn!\n" << endl;

                for(i=0; i<team_size; i++){
                    index = 0;
                    while(hero[index]->getHealth()<=0){
                        index++;
                    }
                    if(monster[i]->getHealth()>0){
                        monster[i]->attack(hero[index]->getHealth(), hero[index]->getAg(), hero[index]->getName());
                    }
                }
                cout << "\n(press enter to continue)" << endl;
                cin.ignore();
                cin.ignore();
            }
        }

        //fix monsters
        if(spell_flag==1 && round==(spell_round+1)){
            spell_flag=0;
            monster[m_sel-1]->resetStats();
        }
        //end loop?
        for(i=0; i<team_size; i++){
            if(hero[i]->getHealth()<=0){
                hero_flag=1;
            }
            else{
                hero_flag=0;
                break;
            }
        }
        for(i=0; i<team_size; i++){
            if(monster[i]->getHealth()<=0){
                enemy_flag=1;
            }
            else{
                enemy_flag=0;
                break;
            }
        }
        if(hero_flag==1 || enemy_flag==1){
            end_flag=1;
        }
    }


    if(hero_flag==1 && enemy_flag==0){
        ///you lose
        cout << "YOU LOSE..." << endl;
        for(i=0; i<team_size; i++){
            hero[i]->loseMoney();
        }
    }
    else if(hero_flag==0 && enemy_flag==1){
        ///you win
        cout << "YOU WIN!!!" << endl;
        //give money
        for(i=0; i<team_size; i++){
            hero[i]->getMoney(hero[i]->getLevel()*70);
        }
    }
    clearTeam();
    //give exp
    int exp;
    srand(time(NULL));
    for(i=0; i<team_size; i++){
        if(hero[i]->getHealth()>0){
            //get lots of exp
            exp = rand() % 41 + 80;
        }
        else{
            exp = rand() % 41 + 60;
        }
        hero[i]->getExp(hero[i]->getLevel()*exp);
    }
    //reset hero stats
    for(i=0; i<team_size; i++){
        hero[i]->resetStats();
    }
}

void Grid::game()
{
	int i, sel;
	srand(time(NULL));

	//create team
	if(team_size>1)
		cout << "Your team consists of " << team_size << " heroes" << endl;
	else
		cout << "Your team consists of " << team_size << " hero" << endl;


	this->createTeam();												//create the hero team
	for(i=0; i<team_size; i++){
		hero[i]->printInfo();
	}
	//create map
	this->makeMap();

	cout << "\n\nPress Enter to start the game!" << endl;
	cin.ignore();
	while(game_flag==0){
		//main loop of the game!

		clear();
		displayMap();					//display the current map

		if(grid[hero_pos[0]][hero_pos[1]]->isMarket()==1){
			//tile is market
			cout << "\n\nDo you want to visit this Market?\n\t1.Yes\t2.No" << endl;
			cin >> sel;
			while(sel<1 || sel>2){
				cout << "Error.Try again." << endl;
				cin >> sel;
			}
			if(sel==1){
				market();
			}
			clear();
			displayMap();
			//move();
		}
		else if(grid[hero_pos[0]][hero_pos[1]]->isCommon()==1){
			//tile is common
			chance = rand() % 4;

			if(chance==0){
                //fight!
                fight();
                if(game_flag==1)
                    break;
                cout << "Press Enter to continue." << endl;
                cin.ignore(1, ' ');

                clear();
                displayMap();
            }
		}

        //normal action - no fight
        cout << "\nWhich action do you want to take?\n" << endl;
        cout << "1.Check Inventory\t2.Print Info\t\t3.Equip\t\t4.Use\t\t5.Move\t\t0.End Game" << endl;
        cin >> sel;
        if(sel == 0){
            this->quitGame();
            break;
        }
        while(sel<0 || sel>5){
            cout << "Error.Try again." << endl;
            cin >> sel;
        }
        if(sel!=5){
            action(sel);
        }
        else{
            clear();
            displayMap();
            move();
        }
	}

	cout << "\n\nGAME OVER!" << endl;

}

Grid::~Grid()
{
    int i, j;
    for(i=0; i<length; i++){
        for(j=0; j<height; j++){
            delete grid[i][j];
        }
    }
	for(i=0; i<team_size; i++){
		delete hero[i];
	}
	/*for(i=0; i<6; i++){
		delete it[i];
	}
	for(i=0; i<4; i++){
		delete sp[i];
	}*/
    delete grid;
	delete hero;
	delete monster;
	delete m;
}



//market
Market::Market(int c)
{
	//c represents market count of grid
	item_count = c*6;
	spell_count = c*4;
	it = new Item*[item_count];
	sp = new Spell*[spell_count];
	item_index=0;
	spell_index=0;
}

void Market::makeArrays()
{
	int i, index, type, r, v, me, dmg, def, oh, am, mr, mid, mad;				//variables -> will be given random values for the items / spells
	string item_n, spell_n;
	srand(time(NULL));


	//! read names from files.
	ifstream wfile, afile, pfile, sfile;
	//Items
	for(i=0; i<item_count; i++){
		type = rand() % 3 + 1;
		if(type==1){
			//weapon
			wfile.open("text_files/weapons.txt");
			if(wfile.is_open()){												//alternative way to "randomly" select from file?
				r = rand() % 5 + 1;
				while(getline(wfile, item_n)){
					r--;
					if(r==0)
						break;
					item_n.clear();
				}
				//create
				v = rand() % 31 + 20;
				me = rand() % 101 + 25;
				dmg = rand() % 11 + 10;
				oh = rand() % 2;

				it[i] = new Weapon(item_n, v, me, dmg, oh);

				item_n.clear();

			}

			wfile.close();
		}
		else if(type==2){
			//armor
			afile.open("text_files/armors.txt");

			if(afile.is_open()){
				r = rand() % 5 + 1;
				while(getline(afile, item_n)){
					r--;
					if(r==0)
						break;
					item_n.clear();
				}
				//create
				v = rand() % 31 + 20;
				me = rand() % 101 + 25;
				def = rand() % 26 + 15;
				it[i] = new Armor(item_n, v, me, def);
				item_n.clear();
			}
			afile.close();
		}
		else if(type==3){
			//potion
			pfile.open("text_files/potions.txt");
			if(pfile.is_open()){
				r = rand() % 5 + 1;
				while(getline(pfile, item_n)){
					r--;
					if(r==0)
						break;
					item_n.clear();
				}
				//create

				v = rand() % 31 + 20;
				me = rand() % 101 + 25;
				am = rand() % 6 + 5;
				it[i] = new Potion(item_n, v, me, am);
				item_n.clear();
			}
			pfile.close();
		}
	}
	//Spells
	for(i=0; i<spell_count; i++){
		type = rand() % 3 + 1;
		if(type==1)
			sfile.open("text_files/spells/firespells.txt");
		else if(type==2)
			sfile.open("text_files/spells/icespells.txt");
		else
			sfile.open("text_files/spells/lightningspells.txt");

		if(sfile.is_open()){
			r = rand() % 10 + 1;
			while(getline(sfile, spell_n)){
				r--;
				if(r==0)
					break;
				spell_n.clear();
			}

			//create
			v = rand() % 31 + 20;
			me = rand() % 101 + 25;
			mr = rand() % 51 + 20;
			mid = rand() % 6 + 25;
			mad = rand() % 6 + 45;
			if(type==1){
				sp[i] = new FireSpell(spell_n, v, me, mr, mid, mad);
                //cout << ">>made firespell with value: " << v << ", min dmge: " << mid << endl;
			}
			else if(type==2){
				sp[i] = new IceSpell(spell_n, v, me, mr, mid, mad);
			}
			else{
				sp[i] = new LightningSpell(spell_n, v, me, mr, mid, mad);
			}
			spell_n.clear();
		}
		sfile.close();
	}
}

void Market::getData(Item **item, Spell **spell)
{
	//here we just make the two arrays item, spell (from Grid) equal to values of the Market arrays -> this way no info is lost

	int i, j=0;
	for(i=item_index; i<item_index+6; i++){
		item[j] = it[i];
		j++;
	}
	item_index += 6;
	j=0;

	for(i=spell_index; i<spell_index+4; i++){
		spell[j] = sp[i];
		j++;
	}
	spell_index += 4;
}

Market::~Market()
{
	int i;
	for(i=0; i<item_count; i++){
		delete it[i];
	}
	for(i=0; i<spell_count; i++){
		delete sp[i];
	}
	delete it;
	delete sp;
}
