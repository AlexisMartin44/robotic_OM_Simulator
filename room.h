#ifndef ROOM_H_INCLUDED
#define ROOM_H_INCLUDED

#include <string.h>
#include "objet.h"
#include "vector"

class Room : public Objet
{
protected:
    std::string m_bordure;
public:
/*-----------------------------------------------------------CONSTRUCTEUR-----------------------------------------------------------*/
    Room();
    ~Room();
    Room(const Room&) = default;
/*------------------------------------------------------------------SSPRGS---------------------------------------------------------------------*/
    void afficher() const;
    void dessiner(Svgfile& svgout);
/*-----------------------------------------------------------CHARGEMENT/SAUVEGARDE-----------------------------------------------------------*/
    bool chargerData(std::vector<Objet*>& mesObjets, Objet parent, std::string nomFichier);
    void sauvegarder();
};

void position(std::string tampon, double& x, double& y);

#endif // ROOM_H_INCLUDED
