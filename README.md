# Simple Turn-Based RPG Game

This is a game i made with C++ as the last assignment of the Object-Oriented-Programming class in 2017.
<br/>It runs locally on the terminal and doesn't include any graphics library, as specified by the exercise, but instead is intended for practise of the basic principles of OOP. It has been made and tested on Linux (Ubuntu 17.10).

Below is a simple explanation of the rules and how to play.
***
At the start of the game, you are given a random team of heroes of different types, which can be anything between: Warrior, Sorcerer, Paladin. The game is played on a 2-dimension grid on the terminal, where the player can do 5 basic moves:
1. **Check Inventory**, which displays the gear of any hero selected.
2. **Print Info**, which displays all the general information about any hero (health, attack atributtes etc.)
3. **Equip**, which lets the user equip gear that has been previously purchased on any hero.
4. **Use**, from which the player selects a potion that has been previously purchased to give to any hero.
5. **Move**, which moves the team of heros to any selected destination
6. **Exit**, which terminates the game.

The randomly created small world that the player can explore consists of normal "tiles" (-) which represent places the team can visit, market "tiles" (M) where the player can buy and sell potions, weapons, armor and spells and non-accessible "tiles" (x), where the team can't go. While navigating through the grid the team may get attacked by a team of random monsters (Dragons, Exoskeletons, Spirits) and the battle immediatelly starts. The players can either choose to fight or end the game by losing. The fight ends when all the monsters are defeated and consequently each hero gets rewarded money and experience. The heros can always level up when acquiring sufficient experience, which results in a boost of their attributes and stronger enemies.

<br/>The folder **text-files** contains all the names used in the game for heroes, monsters, gear and spells, taken from several online name generators.

