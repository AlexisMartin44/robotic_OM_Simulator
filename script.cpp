#include "script.h"
#include "objet.h"
#include <iostream>
#include <fstream> /// Pour les fichiers
#include <sstream> /// Pour les ostringstream


/*---------------------------------------------------------------------------------------------*/
//Code enti�rement r�alis� par Alexis Martin, Zo� Crouzet et No�mie Devergne
//Nous nous sommes bas� sur le chargement de M.Segado pour charger nos objets
//Nous avons aussi utilis� une fonction std::stoi qui transforme un string en entier trouv�e sur internet
//De plus, nous avons utilis� une fonction de mise en pause du syst�me trouv�e sur internet
/*---------------------------------------------------------------------------------------------*/

Script::Script() : m_mesOrdres{}
{
}

Script::~Script()
{
}


//charge un script
void Script::chargerScript(std::ifstream& ifs)
{
    std::string ligne;

    while ( std::getline(ifs, ligne) )
    {
        if ( blankLine(ligne)  )                //si ligne vide, passe a la suivante
            continue;
        if ( ligne.size()>=2 && ligne[0]=='/' && ligne[1]=='/' )        //si la ligne correspond a des commentaires, ne la lit pas passe � la suivante
            continue;

        std::istringstream iss{ligne};

        Ordre* nouveau = new Ordre(ligne);
        nouveau->chargerOrdre(ifs);
        m_mesOrdres.push_back(nouveau);

    }
}

//affiche tous les ordres d'un script
void Script::afficher()
{
    for(unsigned i=0; i<m_mesOrdres.size(); ++i)
    {
        m_mesOrdres[i]->afficher();
        std::cout << std::endl;
    }
}

//retourne le nombre d'ordres
unsigned Script::getSizeOrdres()
{
    return m_mesOrdres.size();
}

//affiche les possibilit�s sur la console
void Script::afficherConsole()
{
    for(unsigned i=0; i<m_mesOrdres.size(); ++i)
    {
        std::cout << i+1 << " : " << m_mesOrdres[i]->getNom() << std::endl;
    }
    std::cout << m_mesOrdres.size() << " fonctions chargees" << std::endl;
}

//lance les actions d'un ordre si il correspond, re�oit en param�tres les param�tres g�n�raux de objet, voir objet.h pour plus d'informations
void Script::lancerOrdre(std::string& tampon, double& xCollision, double& yCollision, std::vector<Objet*>& mesObjets, bool& showAxes, bool& showNames, Piece* piece)
{
    for(unsigned i=0; i<m_mesOrdres.size(); ++i)
    {
        if(tampon==m_mesOrdres[i]->getNom())
            m_mesOrdres[i]->lancerActions(xCollision, yCollision, mesObjets, showAxes, showNames, piece);
    }
}
