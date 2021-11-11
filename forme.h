#ifndef FORME_H_INCLUDED
#define FORME_H_INCLUDED

#include "src/svg/svgfile.h"

class Forme
{
public:
    Forme();
    virtual ~Forme() = default;
    virtual void dessiner(Svgfile& svgout) = 0;
};

class Cercle : public Forme
{
public:
    Cercle();
    virtual void dessiner(Svgfile& svgout);
};

class Rectangle : public Forme
{
public:
    Rectangle();
    virtual void dessiner(Svgfile& svgout);
};

#endif // FORME_H_INCLUDED
