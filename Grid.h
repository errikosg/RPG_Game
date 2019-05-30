#ifndef GRID_H_INCLUDED
#define GRID_H_INCLUDED

#include "Living.h"
#include <string>
#include <vector>

using namespace std;

class Market;

class Tile
{
    public:
        virtual ~Tile();

        virtual int isCommon()=0;
        virtual int isNonAcc()=0;
        virtual int isMarket()=0;				//flags to distinguish between the diferent tile types
};

class CommonTile: public Tile
{
    public:
        int isCommon();
        int isNonAcc();
        int isMarket();
};

class NonAccTile: public Tile
{
    public:
        int isCommon();
        int isNonAcc();
        int isMarket();
};

class MarketTile: public Tile
{
    public:
        int isCommon();
        int isNonAcc();
        int isMarket();
};

class Grid
{
    private:
        Tile ***grid;
        Hero **hero;
		Monster **monster;
		Market *m;
		Item *it[6];
		Spell *sp[4];

        int length, height, team_size, game_flag, market_count, chance;     //!!
        int hero_pos[2];                                    //hero position

        void clear();
		void createTeam();
		void createEnemyTeam();								//create both teams - heroes at start of the game - enemies in every fight
		void clearTeam();									//only for enemies.
		void quitGame();
    public:
        Grid(int l, int h);
        ~Grid();
        void makeMap();
        void displayMap();
        void move();
		void market();								//!
		void action(int sel);						//!
        void game();								//!
        void fight();
};

class Market
{
	private:
		Item **it;
		Spell **sp;
		int item_count, spell_count;
		int item_index, spell_index;
	public:
		Market(int c);
		void makeArrays();
		void getData(Item **item, Spell **spell);
		~Market();
};

#endif // GRID_H_INCLUDED
