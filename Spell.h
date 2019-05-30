#ifndef SPELL_H_INCLUDED
#define SPELL_H_INCLUDED

#include <string>

using namespace std;

class Spell
{
    protected:
        string name;
        int value, min_exp, mp_req;
        int min_dmge, max_dmge;
    public:
        Spell(string n, int v, int me, int mr, int mid, int mad);
        virtual ~Spell(){};
        virtual void printInfo();
        int getValue();                             //get cost of purchase
        int getExp();                               //get experience required to use
        int getMPReq();                             //get magic power requirements.
        virtual void cast(int dex, int& h, int& mid, int& mad, int& def, int& ag)=0;
};


class IceSpell: public Spell
{
    private:
    public:
        IceSpell(string n, int v, int me, int mr, int mid, int mad);
        void printInfo();
        void cast(int dex, int& h, int& mid, int& mad, int& def, int& ag);
};

class FireSpell: public Spell
{
    private:
    public:
        FireSpell(string n, int v, int me, int mr, int mid, int mad);
        void printInfo();
        void cast(int dex, int& h, int& mid, int& mad, int& def, int& ag);
};

class LightningSpell: public Spell
{
    private:
    public:
        LightningSpell(string n, int v, int me, int mr, int mid, int mad);
        void printInfo();
        void cast(int dex, int& h, int& mid, int& mad, int& def, int& ag);
};

#endif // SPELL_H_INCLUDED
