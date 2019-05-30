#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include "Item.h"
#include "Spell.h"
#include "Living.h"
#include "Grid.h"

using namespace std;

int main()
{
    Grid g(5, 7);

	g.game();

    return 0;

}
