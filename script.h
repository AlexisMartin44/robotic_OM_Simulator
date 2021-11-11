#ifndef SCRIPT_H_INCLUDED
#define SCRIPT_H_INCLUDED

#include <vector>
#include "ordre.h"

class Script
{
private:
    std::vector<Ordre*> m_mesOrdres;
public:
    Script();
    ~Script();
    void chargerScript(std::ifstream& ifs);
    void afficher();
    unsigned getSizeOrdres();
    void afficherConsole();
    void lancerOrdre(std::string& tampon, double& xCollision, double& yCollision, std::vector<Objet*>& mesObjets, bool& showAxes, bool& showNames, Piece* piece);
};


#endif // SCRIPT_H_INCLUDED
