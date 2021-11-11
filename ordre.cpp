#include "ordre.h"
#include "objet.h"
#include <fstream> /// Pour les fichiers
#include <sstream> /// Pour les ostringstream
#include "piece.h"
#include <chrono>
#include <thread>


/*---------------------------------------------------------------------------------------------*/
//Code entièrement réalisé par Alexis Martin, Zoé Crouzet et Noémie Devergne
//Nous nous sommes basé sur le chargement de M.Segado pour charger nos objets
//Nous avons aussi utilisé une fonction std::stoi qui transforme un string en entier trouvée sur internet
//De plus, nous avons utilisé une fonction de mise en pause du système trouvée sur internet
/*---------------------------------------------------------------------------------------------*/

Ordre::Ordre(std::string nom) : m_nom{nom}, m_mesActions{}
{
}


Ordre::~Ordre()
{
}

//charge les ordres d'un script
void Ordre::chargerOrdre(std::ifstream& ifs)
{
    std::string tampon, objetCible, ligne, signe, valeur, tamponPrecedent;

    while ( std::getline(ifs, ligne))
    {
        std::istringstream iss(ligne);
        iss >> tampon;
        tamponPrecedent = tampon;
        if(tampon[0]=='@')
            objetCible = tampon;
        else
        {
            iss >> tampon;
            if(tampon==tamponPrecedent)
                m_mesActions.push_back(objetCible+" "+tampon);
            else
            {
                if(tampon[0]=='+')
                {
                    signe = "+";
                    for(unsigned i=1; i<tampon.size(); ++i)
                        valeur+=tampon[i];
                }
                else if(tampon[0]=='-')
                {
                    signe = "-";
                    for(unsigned i=1; i<tampon.size(); ++i)
                        valeur+=tampon[i];
                }
                else
                {
                    signe = "";
                    for(unsigned i=0; i<tampon.size(); ++i)
                        valeur+=tampon[i];
                }


                m_mesActions.push_back(objetCible+" "+tamponPrecedent+" "+signe+valeur);
            }
        }

        if (blankLine(ligne))                //si ligne vide, passe a la suivante
            break;

        valeur.clear();
    }
}

//affiche toutes les actions d'un ordre
void Ordre::afficher()
{
    std::cout << "Nom de l'ordre : " << m_nom << std::endl;
    for(unsigned i=0; i<m_mesActions.size(); ++i)
    {
        std::cout << "\t" << m_mesActions[i] << std::endl;
    }
}

//retourne le nom d'un ordre
std::string Ordre::getNom()
{
    return m_nom;
}

//lance les actions d'un ordre
void Ordre::lancerActions(double& xCollision, double& yCollision, std::vector<Objet*>& mesObjets, bool& showAxes, bool& showNames, Piece* piece)
{
    std::vector<std::string> elements;
    std::string str, action;

    for(unsigned i=0; i<m_mesActions.size(); ++i)   //chaque action est executee
    {
        action = m_mesActions[i];
        std::istringstream iss(action);
        while(std::getline(iss, str, ' '))                                     //decompose les elements avant et apres les espaces
            elements.push_back(str);

        elements[0].erase(elements[0].begin());
        piece->objetSelectionArobase(elements[0], xCollision, yCollision, showAxes, showNames, m_mesActions[i]);
        piece->dessinerSVG(showAxes, showNames, xCollision, yCollision);
        elements.clear();
    }
    piece->dessinerSVG(showAxes, showNames, xCollision, yCollision);

}
