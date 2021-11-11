#ifndef PIECE_H_INCLUDED
#define PIECE_H_INCLUDED

#include <vector>
#include "objet.h"
#include "room.h"
#include "historique.h"

class Piece
{
private:
    std::vector<Objet*> m_mesObjets;
    std::vector<Room*> m_mesRooms;
    std::string m_romActuelle;
    Historique m_historique;
    Piece* m_copie;
public:
/*------------------------------------------------------CONSTRUCTEURS---------------------------------------------------------------------*/
    Piece();
    ~Piece();
    Piece(Piece* piece);
/*------------------------------------------------------GETTERS---------------------------------------------------------------------*/
    unsigned getSizeRoom();
    unsigned getSizeObjets();
    Objet* getParentObjet(int i);
    unsigned nbrEnfantObjet(int i);
    Objet* getEnfantObjet(int k, int i);
/*------------------------------------------------------SSPRGS---------------------------------------------------------------------*/
    void chargerScene();
    void dessinerSVG(bool showAxes, bool showNames, double& xCollision, double& yCollision);
    void testChuteObjets();
    void afficher();
/*------------------------------------------------------MENU---------------------------------------------------------------------*/
    void menu();
    void objetSelectionArobase(std::string& objetActuel, double& xCollision, double& yCollision, bool& showAxes, bool& showNames, std::string& tampon);
    void souvenirObjetPrecedent(std::string& tampon, std::string& objetActuel, double& xCollision, double& yCollision, bool& showNames, bool& showAxes);
    bool loadFichier(std::string& tampon, std::string& dernierROM);
    void undoAction(std::string& objetActuel, std::string& dernierROM, double& xCollision, double& yCollision, bool& showAxes, bool& showNames);
    void redoAction(std::string& objetActuel, double& xCollision, double& yCollision, bool& showAxes, bool& showNames, std::string& dernierROM);
/*------------------------------------------------------SCRIPTS---------------------------------------------------------------------*/
    void loadScript(std::string& tampon, double& xCollision, double& yCollision, std::vector<Objet*>& mesObjets, bool& showAxes, bool& showNames);
};

void envoiDesOrdres(std::string& objetActuel, std::vector<std::string>& elements, double& xCollision, double& yCollision, std::vector<Objet*>& mesObjets);


#endif // PIECE_H_INCLUDED
