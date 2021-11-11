#ifndef OBJET_H_INCLUDED
#define OBJET_H_INCLUDED

#include "src/geometrie/coords.h"
#include "src/svg/svgfile.h"
#include <string.h>
#include <vector>
#include <iostream>
#include <map>

class Objet
{
protected:
    std::string m_id;
    int m_longueur;
    int m_hauteur;
    std::string m_couleur;
    Coords m_refpos;
    Coords m_basepos;
    Coords m_endpos;
    Coords m_startpos;
    Coords m_precedentRefPos;
    std::vector<Objet*> m_mesEnfants;
    Objet* m_monParent;
    bool m_dessinerFlou;
    bool m_tenir;

public:
/*-----------------------------------------------------------CONSTRUCTEURS-----------------------------------------------------------*/
    Objet(Objet* parent);
    ~Objet();
    Objet(const Objet&) = default;
    Objet(Objet* objet, int i);
/*-----------------------------------------------------------GETTERS/SETTERS-----------------------------------------------------------*/
    int getHauteur();
    int getLongueur();
    double getTLX();
    double getTLY();
    double getDirectionY();
    double getDirectionX();
    unsigned getSizeID();
    std::string getCouleur();
    Objet*& getParent();
    unsigned getNbrEnfants();
    unsigned getNbrEnfantsParent();
    Objet* getEnfant(int i);
    Objet* getEnfantIParent(int i);
    std::string getIDParent();
    std::string getIDParentParent();
    double getStartposX();
    double getStartposY();
    double getRefposX();
    double getRefposY();
    double getEndposX();
    double getEndposY();
    double getPrerefposX();
    double getPrerefposY();
    double getBaseposX();
    double getBaseposY();
    int getLongueurParent();
    int getHauteurParent();
    std::string getIDEnfant(int i);
    std::string getID();
    char getID_I(int i);
    void setRefposY(double y);
    void setRefposX(double x);
    std::vector<Objet*>& getEnfants();
    void setParent(Objet* nvParent);
    void ajouter1Enfant(Objet* nvEnfant);
    void supprimerEnfantParent();
    double getTLXParent();
    void setBaseposX(double x);
    void setBaseposY(double y);
/*-----------------------------------------------------------COPIE-----------------------------------------------------------*/
    void ajouterParent(std::map<Objet*, Objet*> transpose, Objet* objet);
    void ajouterEnfant(std::map<Objet*, Objet*> transpose, Objet* objet);
/*-----------------------------------------------------------SSPRGS-----------------------------------------------------------*/
    void afficher() const;
    void dessiner(Svgfile& svgout, double& xCollision, double& yCollision);
/*-----------------------------------------------------------CHARGEMENT-----------------------------------------------------------*/
    void chargerData(std::ifstream& ifs, std::vector<Objet*>& mesObjets);
/*-----------------------------------------------------------ORDRES-----------------------------------------------------------*/
    void lacherObjet(std::vector<Objet*>& mesObjets);
    void donnerOrdre(std::vector<std::string>& elements, std::vector<Objet*> mesObjets, double& xCollision, double& yCollision);
    void donnerOrdreLignee(std::vector<std::string>& lignee, std::vector<std::string>& elements, std::vector<Objet*> mesObjets, unsigned a, double& xCollision, double& yCollision);
/*-----------------------------------------------------------COLLISIONS-----------------------------------------------------------*/
    bool testCollisionEnfant(std::vector<Objet*> mesObjets, double& distanceX, double& distanceY, Objet* objetBase, double& directionX, double& directionY, double& xCollision, double& yCollision, std::vector<std::string>& elements);
    void testCollision(std::vector<Objet*> mesObjets, double& directionX, double& directionY, std::vector<std::string>& elements);
    bool contact(std::vector<Objet*> mesObjets, unsigned& numeroObjet, Objet* exceptionObjet);
/*-----------------------------------------------------------SAUVEGARDE-----------------------------------------------------------*/
    void sauvegarder(unsigned niveau, std::vector<Objet*>& mesObjets,std::ofstream &sauvegarde);
};
void ecrirePositionFichier(Coords position,std::ofstream &sauvegarde);


bool blankLine(std::string line);
bool testParente(Objet*& objetActuel, Objet*& parent);
bool testEnfante(Objet* enfant, Objet* objetActuel);


#endif // OBJET_H_INCLUDED
