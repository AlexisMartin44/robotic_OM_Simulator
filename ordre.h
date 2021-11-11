#ifndef ORDRE_H_INCLUDED
#define ORDRE_H_INCLUDED

#include <string>
#include <vector>
#include "objet.h"
#include "piece.h"

class Ordre
{
private:
    std::string m_nom;
    std::vector<std::string> m_mesActions;
public:
    Ordre(std::string nom);
    ~Ordre();
    void chargerOrdre(std::ifstream& ifs);
    void afficher();
    std::string getNom();
    void lancerActions(double& xCollision, double& yCollision, std::vector<Objet*>& mesObjets, bool& showAxes, bool& showNames, Piece* piece);
};

#endif // ORDRE_H_INCLUDED
