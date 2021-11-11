#include "objet.h"
#include "src/graphisme/couleur.h"
#include "src/svg/svgfile.h"
#include "room.h"
#include "piece.h"
#include <fstream> /// Pour les fichiers
#include <sstream> /// Pour les ostringstream
#include <iostream>
#include <string>
#include <algorithm>


/*---------------------------------------------------------------------------------------------*/
//Code entièrement réalisé par Alexis Martin, Zoé Crouzet et Noémie Devergne
//Nous nous sommes basé sur le chargement de M.Segado pour charger nos objets
//Nous avons aussi utilisé une fonction std::stoi qui transforme un string en entier trouvée sur internet
//De plus, nous avons utilisé une fonction de mise en pause du système trouvée sur internet
/*---------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------CONSTRUCTEURS-----------------------------------------------------------*/
Objet::Objet(Objet* parent)
: m_id{""}, m_longueur{0}, m_hauteur{0}, m_couleur{""}, m_refpos{0,0},
m_basepos{0.5,0.5}, m_endpos{-1,-1}, m_startpos{0,0}, m_precedentRefPos{0,0}, m_monParent{parent}, m_dessinerFlou{false}, m_tenir{false}
{
}

Objet::~Objet()
{
}

Objet::Objet(Objet* objet, int i)
{
    m_id = objet->getID();
    m_longueur = objet->getLongueur();
    m_hauteur = objet->getHauteur();
    m_couleur = objet->getCouleur();
    m_refpos.setX(objet->m_refpos.getX());
    m_refpos.setY(objet->m_refpos.getY());
    m_startpos.setX(objet->m_startpos.getX());
    m_startpos.setY(objet->m_startpos.getY());
    m_basepos.setX(objet->m_basepos.getX());
    m_basepos.setY(objet->m_basepos.getY());
    m_endpos.setX(objet->m_endpos.getX());
    m_endpos.setY(objet->m_endpos.getY());
    m_precedentRefPos.setX(objet->m_precedentRefPos.getX());
    m_precedentRefPos.setY(objet->m_precedentRefPos.getY());
}

/*-----------------------------------------------------------GETTERS-----------------------------------------------------------*/

//retourne l'enfant i d'un objet
Objet* Objet::getEnfant(int i)
{
    return m_mesEnfants[i];
}

//retourne le nombre d'enfants d'un objet
unsigned Objet::getNbrEnfants()
{
    return m_mesEnfants.size();
}

//retourne la couleur d'un objet
std::string Objet::getCouleur()
{
    return m_couleur;
}

//retourne l'ID de l'enfant i d'un objet
std::string Objet::getIDEnfant(int i)
{
    return m_mesEnfants[i]->getID();
}

//retourne la hauteur d'un objet
int Objet::getHauteur()
{
    return m_hauteur;
}

//retourne la longueur d'un objet
int Objet::getLongueur()
{
    return m_longueur;
}

//retourne le x absolu d'un objet
double Objet::getTLX()
{
    if(m_monParent!=NULL)
        return m_monParent->getTLX()+(m_monParent->getLongueur()*m_refpos.getX())-(m_basepos.getX()*m_longueur);
    else
        return 0;
}

//retourne le y absolu d'un objet
double Objet::getTLY()
{
    if(m_monParent!=NULL)
        return m_monParent->getTLY()+(m_monParent->getHauteur()*m_refpos.getY())-(m_basepos.getY()*m_hauteur);
    else
        return 0;
}

//retourne l'ID d'un objet
std::string Objet::getID()
{
    return m_id;
}

//retourne la lettre i constituant l'ID d'un objet
char Objet::getID_I(int i)
{
    return m_id[i];
}

//retourne le nombre de lettres consituants l'ID d'un objet
unsigned Objet::getSizeID()
{
    return m_id.size();
}

//retourne la direction selon l'axe x d'un objet
double Objet::getDirectionX()
{
    if(m_endpos.getX()-m_startpos.getX()>0)
        return 1;
    else
        return -1;
}

//retourne la direction selon l'axe y d'un objet
double Objet::getDirectionY()
{
    if(m_endpos.getY()-m_startpos.getY()>0)
        return 1;
    else
        return -1;
}

//retourne la longueur du parent d'un objet
int Objet::getLongueurParent()
{
    return m_monParent->getLongueur();
}

//retourne la hauteur du parent d'un objet
int Objet::getHauteurParent()
{
    return m_monParent->getHauteur();
}

//retourne le x du startpos d'un objet
double Objet::getStartposX()
{
    return m_startpos.getX();
}

//retourne le y du startpos d'un objet
double Objet::getStartposY()
{
    return m_startpos.getY();
}

//retourne le x du refpos d'un objet
double Objet::getRefposX()
{
    return m_refpos.getX();
}

//retourne le y du refpos d'un objet
double Objet::getRefposY()
{
    return m_refpos.getY();
}

//retourne le x du endpos d'un objet
double Objet::getEndposX()
{
    return m_endpos.getX();
}

//retourne le y du endpos d'un objet
double Objet::getEndposY()
{
    return m_endpos.getY();
}

//retourne le x du precedent refpos d'un objet
double Objet::getPrerefposX()
{
    return m_precedentRefPos.getY();
}

//retourne le y du precedent refpos d'un objet
double Objet::getPrerefposY()
{
    return m_precedentRefPos.getY();
}

//retourne le x du basepos d'un objet
double Objet::getBaseposX()
{
    return m_basepos.getX();
}

//retourne le y du basepos d'un objet
double Objet::getBaseposY()
{
    return m_basepos.getY();
}

//retourne le parent d'un objet
Objet*& Objet::getParent()
{
    return m_monParent;
}

//retourne l'ID du parent d'un objet
std::string Objet::getIDParent()
{
    return m_monParent->getID();
}

//retourne l'ID du parent du parent d'un objet
std::string Objet::getIDParentParent()
{
    return m_monParent->getIDParent();
}

//retourne le nombre d'enfants du parent d'un objet
unsigned Objet::getNbrEnfantsParent()
{
    return m_monParent->getNbrEnfants();
}

//retourne l'enfant i du parent d'un objet
Objet* Objet::getEnfantIParent(int i)
{
    return m_monParent->getEnfant(i);
}

//set le refposY d'un objet
void Objet::setRefposY(double y)
{
    m_refpos.setY(y);
}

//set le refposX d'un objet
void Objet::setRefposX(double x)
{
    m_refpos.setX(x);
}

//retourne tous les enfants d'un objet
std::vector<Objet*>& Objet::getEnfants()
{
    return m_mesEnfants;
}

void Objet::setParent(Objet* nvParent)
{
    m_monParent = nvParent;
}

double Objet::getTLXParent()
{
    return m_monParent->getTLX();
}

void Objet::setBaseposX(double x)
{
    m_basepos.setX(x);
}

void Objet::setBaseposY(double y)
{
    m_basepos.setY(y);
}


/*-----------------------------------------------------------COPIE-----------------------------------------------------------*/

//ajoute le nouveau parent en fonction de celui qui correspond à l'objet envoyé en paramètre, retrouvé grace au map
void Objet::ajouterParent(std::map<Objet*, Objet*> transpose, Objet* objet)
{
    m_monParent = transpose[objet];
}

//ajoute le nouvel enfant en fonction de celui qui correspond à l'objet envoyé en paramètre, retrouvé grace au map
void Objet::ajouterEnfant(std::map<Objet*, Objet*> transpose, Objet* objet)
{
    m_mesEnfants.push_back(transpose[objet]);
}

/*-----------------------------------------------------------SSPRGS-----------------------------------------------------------*/

//dessine un objet, ainsi que ses axes de translation et son nom si besoin
//reçoit donc le svgout, mais aussi xCollision et yCollision qui donne les coordonnées d'un emplacement ou l'on va dessiner des cercles si il y a une collision pour la signaler
void Objet::dessiner(Svgfile& svgout, double& xCollision, double& yCollision)
{
    int x1 = m_monParent->getTLX()+(m_monParent->getLongueur()*m_refpos.getX())-(m_basepos.getX()*m_longueur);      //coordonnée x absolue du bloc
    int y1 = m_monParent->getTLY()+(m_monParent->getHauteur()*m_refpos.getY())-(m_basepos.getY()*m_hauteur);        //coordonnée y absolue du bloc
    double distance, d;

    if(m_precedentRefPos.getX()!=m_refpos.getX()&&m_dessinerFlou)
    {
        distance = abs(m_refpos.getX()*100-m_precedentRefPos.getX()*100);
        d = distance/5;
        for(unsigned i=0; i<distance; i+=d)
        {
            if(m_precedentRefPos.getX()<m_refpos.getX())
                svgout.addRectangle(x1-i, y1, m_longueur, m_hauteur, m_couleur, 0.1, "black");
            else
               svgout.addRectangle(x1+m_longueur/2+i, y1, m_longueur, m_hauteur, m_couleur, 0.1, "black");
        }
    }
    if(m_precedentRefPos.getY()!=m_refpos.getY()&&m_dessinerFlou)
    {
        distance = abs(m_refpos.getY()*100-m_precedentRefPos.getY()*100);
        d = distance/10;
        for(unsigned i=0; i<distance; i+=d)
        {
            if(m_precedentRefPos.getY()<m_refpos.getY())
                svgout.addRectangle(x1, y1-i, m_longueur, m_hauteur, m_couleur, 0.2, "black");
            else
               svgout.addRectangle(x1, y1+m_hauteur+i, m_longueur, m_hauteur, m_couleur, 0.2, "black");
        }
    }
    svgout.addRectangle(x1, y1, m_longueur, m_hauteur, m_couleur, 1, "black");                                         //dessine le bloc

    m_dessinerFlou = false;

    if(svgout.getAffichageNoms())                                                                                   //si on veut afficher les noms
        svgout.addText(x1+(m_longueur/2), y1+(m_hauteur/2), m_id, "black");
    if(xCollision!=0&&yCollision!=0)                                            //si il y a eu une collision,dessine des cercles a l'emplacement de la collision
    {
        svgout.addCircle(100+xCollision, 100+yCollision, 15, 3, 1, "red");
        svgout.addCircle(100+xCollision, 100+yCollision, 20, 2, 0.25, "red");
        svgout.addCircle(100+xCollision, 100+yCollision, 25, 1, 0.1, "red");

    }
    if(svgout.getAffichageAxes())                                                                                   //si on veut les axes de translations
    {
        if(m_endpos.getX()!=-1)             //si le bloc est capable de se déplacer
        {
            svgout.addDisk(m_monParent->getTLX()+(m_monParent->getLongueur()*m_refpos.getX()), m_monParent->getTLY()+(m_monParent->getHauteur()*m_refpos.getY()), 3, "red");    //dessine un disque à l'emplacement de son basepos
            if(m_monParent->getTLY()+m_startpos.getY()*m_monParent->getHauteur()==m_monParent->getTLY()+m_endpos.getY()*m_monParent->getHauteur())                              //si le bloc se déplace selon l'axe x
            {
                if(m_endpos.getX()-m_startpos.getX()>0)             //si le bloc se déplace vers la droite
                {
                    svgout.addLine(m_monParent->getTLX()+m_endpos.getX()*m_monParent->getLongueur()-4, m_monParent->getTLY()+m_endpos.getY()*m_monParent->getHauteur()-4, m_monParent->getTLX()+m_endpos.getX()*m_monParent->getLongueur(), m_monParent->getTLY()+m_endpos.getY()*m_monParent->getHauteur(), "red");    //dessine les fleches au bout des lignes
                    svgout.addLine(m_monParent->getTLX()+m_endpos.getX()*m_monParent->getLongueur(), m_monParent->getTLY()+m_endpos.getY()*m_monParent->getHauteur(), m_monParent->getTLX()+m_endpos.getX()*m_monParent->getLongueur()-4, m_monParent->getTLY()+m_endpos.getY()*m_monParent->getHauteur()+4, "red");
                }
                else                                                //si le bloc se déplace vers la gauche
                {
                    svgout.addLine(m_monParent->getTLX()+m_endpos.getX()*m_monParent->getLongueur()+4, m_monParent->getTLY()+m_endpos.getY()*m_monParent->getHauteur()-4, m_monParent->getTLX()+m_endpos.getX()*m_monParent->getLongueur(), m_monParent->getTLY()+m_endpos.getY()*m_monParent->getHauteur(), "red");    //dessine les fleches au bout des lignes
                    svgout.addLine(m_monParent->getTLX()+m_endpos.getX()*m_monParent->getLongueur(), m_monParent->getTLY()+m_endpos.getY()*m_monParent->getHauteur(), m_monParent->getTLX()+m_endpos.getX()*m_monParent->getLongueur()+4, m_monParent->getTLY()+m_endpos.getY()*m_monParent->getHauteur()+4, "red");
                }

                svgout.addLine(m_monParent->getTLX()+m_startpos.getX()*m_monParent->getLongueur(), m_monParent->getTLY()+m_startpos.getY()*m_monParent->getHauteur()-7, m_monParent->getTLX()+m_startpos.getX()*m_monParent->getLongueur(), m_monParent->getTLY()+m_startpos.getY()*m_monParent->getHauteur()+7, "red");    //dessine la barre parallèle à l'axe sur le startpos
            }
            if(m_monParent->getTLX()+m_startpos.getX()*m_monParent->getLongueur()==m_monParent->getTLX()+m_endpos.getX()*m_monParent->getLongueur())                        //si le bloc se déplace selon l'axe y
            {
                if(m_endpos.getY()-m_startpos.getY()<0)           //si le bloc se déplace vers le haut
                {
                    svgout.addLine(m_monParent->getTLX()+m_endpos.getX()*m_monParent->getLongueur()-4, m_monParent->getTLY()+m_endpos.getY()*m_monParent->getHauteur()+4, m_monParent->getTLX()+m_endpos.getX()*m_monParent->getLongueur(), m_monParent->getTLY()+m_endpos.getY()*m_monParent->getHauteur(), "red");    //dessine les fleches au bout des lignes
                    svgout.addLine(m_monParent->getTLX()+m_endpos.getX()*m_monParent->getLongueur(), m_monParent->getTLY()+m_endpos.getY()*m_monParent->getHauteur(), m_monParent->getTLX()+m_endpos.getX()*m_monParent->getLongueur()+4, m_monParent->getTLY()+m_endpos.getY()*m_monParent->getHauteur()+4, "red");
                }
                else                                               //si le bloc se déplace vers le bas
                {
                    svgout.addLine(m_monParent->getTLX()+m_endpos.getX()*m_monParent->getLongueur()-4, m_monParent->getTLY()+m_endpos.getY()*m_monParent->getHauteur()-4, m_monParent->getTLX()+m_endpos.getX()*m_monParent->getLongueur(), m_monParent->getTLY()+m_endpos.getY()*m_monParent->getHauteur(), "red");    //dessine les fleches au bout des lignes
                    svgout.addLine(m_monParent->getTLX()+m_endpos.getX()*m_monParent->getLongueur(), m_monParent->getTLY()+m_endpos.getY()*m_monParent->getHauteur(), m_monParent->getTLX()+m_endpos.getX()*m_monParent->getLongueur()+4, m_monParent->getTLY()+m_endpos.getY()*m_monParent->getHauteur()-4, "red");
                }

                svgout.addLine(m_monParent->getTLX()+m_startpos.getX()*m_monParent->getLongueur()-7, m_monParent->getTLY()+m_startpos.getY()*m_monParent->getHauteur(), m_monParent->getTLX()+m_startpos.getX()*m_monParent->getLongueur()+7, m_monParent->getTLY()+m_startpos.getY()*m_monParent->getHauteur(), "red");    //dessine la barre parallèle à l'axe sur le startpos
            }
            svgout.addLine(m_monParent->getTLX()+m_startpos.getX()*m_monParent->getLongueur(), m_monParent->getTLY()+m_startpos.getY()*m_monParent->getHauteur(),m_monParent->getTLX()+m_endpos.getX()*m_monParent->getLongueur(), m_monParent->getTLY()+m_endpos.getY()*m_monParent->getHauteur(), "red");             //dessine l'axe de translation
        }
    }
}

//affiche en console les données d'un objet
void Objet::afficher() const
{
    std::cout << m_id << " / " << m_longueur << " / " << m_hauteur << " / " << m_couleur << " / ";
    std::cout << "Refpos : ";
    m_refpos.afficher();
    std::cout << "\tPrecRefpos : ";
    m_precedentRefPos.afficher();
    std::cout << "\tBasepos : ";
    m_basepos.afficher();
    std::cout << "\tEndpos : ";
    m_endpos.afficher();
    std::cout << "\tStartpos : ";
    m_startpos.afficher();
    std::cout << "\tParent ID : " << m_monParent->getID();
    for(unsigned i=0; i<m_mesEnfants.size(); ++i)
        std::cout << "\tEnfant ID : "<< m_mesEnfants[i]->getID();
    std::cout <<"\n" << std::endl;

}

// La fonction suivante renvoie vrai pour une ligne vide (rien ou que des espaces)
inline bool blankLine(std::string line)
{
    std::istringstream iss(line);
    std::string mot;
    iss >> mot;
    return !mot.size();
}

void Objet::ajouter1Enfant(Objet* nvEnfant)
{
    m_mesEnfants.push_back(nvEnfant);
}

void Objet::supprimerEnfantParent()
{
    for(unsigned i=0; i<m_monParent->getNbrEnfants(); ++i)
        if(m_monParent->getEnfant(i)==this)
        {
            m_monParent->getEnfants().erase(m_monParent->getEnfants().begin()+i);
        }

}



/*-----------------------------------------------------------CHARGEMENT-----------------------------------------------------------*/

//permet de charger l'ensemble du fichier après avoir charger la room
//reçoit la position du curseur dans le fichier, et le tableau des objets de la piece pour ajouter les elements au fur et à mesure du chargement
void Objet::chargerData(std::ifstream& ifs, std::vector<Objet*>& mesObjets)
{
    int cpt = 0;                                //numero de la ligne (0 pour 1, etc...)
    std::string tampon;                         //tampon qu'on utilise pour stocker les données du fichier au fur et a mesure
    std::string tamponPrecedent = " ";          //stocke le tampon de la boucle precedente
    double tamponDouble;                        //tampon mais de double pas de string
    double temporaire = 0;                      //permet d'empecher une saisie de tampon indésirable à la suite d'actions
    int translateTemp = 0;                      //si pas de translation, en met une de base à 0.5
    int saut = 0;                               //permet de detecter le passage à un nouvel enfant ayant le meme parent que le dernier charge
    double x, y;                                //stock les coordonnes relatives
    std::string ligne;                          //contenu de la ligne, permet d'aller de ligne en ligne

    while ( std::getline(ifs, ligne))
    {
        std::istringstream iss{ligne};
        if ( blankLine(ligne)  )                //si saut de ligne, on charge un nouvel enfant ayant le meme parent que le dernier objet chargé
            saut = 1;

        if ( ligne.size()>=2 && ligne[0]=='/' && ligne[1]=='/' )    //si ligne de commentaire, on ignore
        {
            cpt--;
            continue;
        }

        if(cpt==0)                              //premiere ligne
        {
            do                                  //attribue au nouvel objet les donnees du fichier
            {
                tamponPrecedent = tampon;
                iss >> tampon;
                if(tampon=="id=")
                {
                    iss >> tampon;
                    m_id = tampon;
                }
                else if(tampon=="width=")
                {
                    iss >> tamponDouble;
                    m_longueur = tamponDouble;
                }
                else if(tampon=="width%=")
                {
                    iss >> tamponDouble;
                    m_longueur = (tamponDouble*m_monParent->getLongueur())/100;
                }
                else if(tampon=="height=")
                {
                    iss >> tamponDouble;
                    m_hauteur = tamponDouble;
                }
                else if(tampon=="height%=")
                {
                    iss >> tamponDouble;
                    m_hauteur = (tamponDouble*m_monParent->getHauteur())/100;
                }
                else if(tampon=="color=")
                {
                    iss >> tampon;
                    m_couleur = tampon;
                }
            }while(tampon!=tamponPrecedent);
        }
        else if(cpt==1)                         //deuxieme ligne
        {
            do
            {
                tamponPrecedent = tampon;
                if(temporaire==0)
                    iss >> tampon;
                temporaire = 0;
                if(tampon=="refpos=")
                {
                    iss >> tampon;
                    position(tampon, x, y);         //convertit les positions relatives en coordonnées relatives
                    m_refpos.setX(x);
                    m_refpos.setY(y);
                    m_startpos.setX(x);
                    m_startpos.setY(y);
                }
                else if(tampon=="refposx%=")
                {
                    iss >> tamponDouble;
                    m_refpos.setX(m_refpos.getX()+(tamponDouble/100));
                }
                else if(tampon=="refposy%=")
                {
                    iss >> tamponDouble;
                    m_refpos.setY(m_refpos.getY()+(tamponDouble/100));
                }
                else if(tampon=="refposx=")
                {
                    iss >> tamponDouble;
                    m_refpos.setX(m_refpos.getX()+(tamponDouble/m_monParent->getLongueur()));
                    iss >> tampon;
                    temporaire = 1;
                    if(tampon == "refposx=")
                        tampon = tamponPrecedent;
                }
                else if(tampon=="refposy=")
                {
                    iss >> tamponDouble;
                    m_refpos.setY(m_refpos.getY()+(tamponDouble/m_monParent->getHauteur()));
                    iss >> tampon;
                    temporaire = 1;
                    if(tampon == "refposy=")
                        tampon = tamponPrecedent;
                }
                else if(tampon=="basepos=")
                {
                    iss >> tampon;
                    position(tampon, x, y);
                    m_basepos.setX(x);
                    m_basepos.setY(y);
                }
                else if(tampon == "endpos=")
                {
                    iss >> tampon;
                    position(tampon, x, y);
                    m_endpos.setX(x);
                    m_endpos.setY(y);
                }
                else if(tampon == "translate=")
                {
                    iss >> tamponDouble;
                    translateTemp = 1;
                    m_refpos.setX(m_refpos.getX()+tamponDouble*(m_endpos.getX()-m_refpos.getX()));
                    m_refpos.setY(m_refpos.getY()+tamponDouble*(m_endpos.getY()-m_refpos.getY()));
                    iss >> tampon;
                    temporaire = 1;
                    if(tampon == "translate=")
                        tampon = tamponPrecedent;
                }
            }while(tampon!=tamponPrecedent);
            if(translateTemp==0&&(m_endpos.getX()!=-1))             //si le bloc peut se déplacer mais qu'il n'a pas eu de translate d'affecter, il est translate de 0.5
            {
                m_refpos.setX(m_refpos.getX()+0.5*(m_endpos.getX()-m_refpos.getX()));
                m_refpos.setY(m_refpos.getY()+0.5*(m_endpos.getY()-m_refpos.getY()));
            }
        }
        else
        {
            if(saut == 1)                                           //si saut de ligne, on cree un nouvel objet avec le meme parent que l'objet charge precedement
            {
                Objet* nouveau = new Objet(m_monParent);
                nouveau->chargerData(ifs, mesObjets);
                mesObjets.push_back(nouveau);
                m_monParent->m_mesEnfants.push_back(nouveau);
                break;
            }

            iss >> tampon;
            if(tampon=="[")                                         //si crochet ouvrant, on charge un nouvel objet avec comme parent l'objet qui vient d'etre cree
            {
                Objet* nouveau = new Objet(this);
                nouveau->chargerData(ifs, mesObjets);
                mesObjets.push_back(nouveau);
                m_mesEnfants.push_back(nouveau);
            }
            else if(tampon=="]")                                    //si crocher fermant, on quitte la boucle car l'objet est créé et son parent n'aura pas d'autres enfants
                break;
        }
        cpt++;
    }
}


/*-----------------------------------------------------------ORDRES-----------------------------------------------------------*/

void Objet::lacherObjet(std::vector<Objet*>& mesObjets)
{
    unsigned numeroObjet = 0;
    double x1;
    bool stop = false;
    Objet* pointeur;
    unsigned j;

    while(!m_mesEnfants[0]->contact(mesObjets, numeroObjet, NULL))
    {
        m_mesEnfants[0]->setRefposY(m_mesEnfants[0]->getRefposY()+0.1);
        if(m_mesEnfants[0]->getTLY()>500)
        {
            stop = true;
            break;
        }
    }

    pointeur = m_mesEnfants[0];

    if(stop)
    {
        for(unsigned k=0; k<mesObjets.size(); k++)
        {
            if(mesObjets[k]=pointeur)
                mesObjets.erase(mesObjets.begin()+k);
        }
        delete m_mesEnfants[0];
        m_mesEnfants.erase(m_mesEnfants.begin());
    }
    else
    {
        x1 = m_mesEnfants[0]->getTLX();

        pointeur->setRefposX((x1-mesObjets[numeroObjet]->getTLX())/mesObjets[numeroObjet]->getLongueur());
        pointeur->setRefposY(0);
        pointeur->setBaseposX(0);
        pointeur->setBaseposY(1);
        pointeur->supprimerEnfantParent();
        pointeur->setParent(mesObjets[numeroObjet]);
        mesObjets[numeroObjet]->ajouter1Enfant(pointeur);

    }
}


//donne un ordre saisi par l'utilisateur à l'objet reçu en this
//reçoit tous les objets de la piece, et les elements qui constitue l'ordre a donner
void Objet::donnerOrdre(std::vector<std::string>& elements, std::vector<Objet*> mesObjets, double& xCollision, double& yCollision)
{
    double valeur, ecart, anciennePos;              //valeur : valeur du déplacement, ecart : si l'objet a depasse son endpos renvoie l'ecart entre sa position et le endpos pour le potention undo futur, aciennepos : position avant le deplacement pour le potention undo futur
    std::string temporaire, objetActuel, objetCible, finger = "finger";                         //tampon
    unsigned numeroObjet, numeroObjet2;
    int correspondance = 0, correspondance2 =0;
    bool validation = false, validation2 = false, validation3 = false;

    m_precedentRefPos.setX(m_refpos.getX());        //on stocke le refpos dans le precedent refpos car le bloc va se déplacer
    m_precedentRefPos.setY(m_refpos.getY());

    if(elements[1]=="move"&&m_endpos.getX()!=-1)                         //si l'ordre est move et que le bloc est capable de se déplacer
    {
        if(elements[2][0]=='+')                                          //si le déplacement est positif par rapport à l'axe de translation
        {
            elements[2].erase(elements[2].begin());
            valeur = std::stoi(elements[2]);                            //valeur du déplacement
            elements[2].insert(0,"+");
            if(m_endpos.getX()!=m_startpos.getX())
                m_refpos.setX(m_refpos.getX()+getDirectionX()*(valeur/100));    //déplace le bloc
            if(m_endpos.getY()!=m_startpos.getY())
                m_refpos.setY(m_refpos.getY()+getDirectionY()*(valeur/100));

            if(m_tenir)
            {
                lacherObjet(mesObjets);
            }


        }
        else if(elements[2][0]=='-'&&!m_tenir)                                    //si le déplacement est négatif par rapport à l'axe de translation
        {
            elements[2].erase(elements[2].begin());
            valeur = std::stoi(elements[2]);
            elements[2].insert(0,"-");
            if(m_endpos.getX()!=m_startpos.getX())
            {
                m_refpos.setX(m_refpos.getX()-(getDirectionX()*(valeur/100)));
            }
            if(m_endpos.getY()!=m_startpos.getY())
                m_refpos.setY(m_refpos.getY()-(getDirectionY()*(valeur/100)));
        }
        else if(!m_tenir)                                                          //si le déplacement est un pourcentage par rapport au startpos et endpos
        {
            valeur = std::stoi(elements[2]);
            if(m_endpos.getX()!=m_startpos.getX())
            {
                if(m_startpos.getX()!=0)
                    anciennePos = ((1-m_refpos.getX())+m_refpos.getX()-m_startpos.getX())*100;
                else
                    anciennePos = m_refpos.getX()*100;
                elements.push_back(std::to_string(anciennePos));        //on vient stocker (pour le undo) l'ancienne position du bloc pour pouvoir revenir avec un undo à l'ancienne pos
                m_refpos.setX(m_startpos.getX()+((m_endpos.getX()-m_startpos.getX())*valeur)/100);  //on définit la nouvelle position
            }
            if(m_endpos.getY()!=m_startpos.getY())
            {
                if(m_startpos.getY()!=0)
                    anciennePos = ((1-m_refpos.getY())+m_refpos.getY()-m_startpos.getY())*100;
                else
                    anciennePos = m_refpos.getY()*100;
                elements.push_back(std::to_string(anciennePos));
                m_refpos.setY(m_startpos.getY()+((m_endpos.getY()-m_startpos.getY())*valeur)/100);
            }
        }

        //m_dessinerFlou = true;

        if(m_refpos.getX()>1)           //blindage des positions, les blocs ne doivent pas pouvoir dépasser leur startpos ou endpos
        {
            ecart=((valeur/100)-(m_refpos.getX()-1))*100;
            temporaire = std::to_string(ecart);
            if(getDirectionX()==1)
                elements.push_back("+"+temporaire);
            else
                elements.push_back("-"+temporaire);
            m_refpos.setX(1);
        }
        if(m_refpos.getX()<0)
        {
            ecart=((valeur/100)+m_refpos.getX())*100;
            temporaire = std::to_string(ecart);
            if(getDirectionX()==1)
                elements.push_back("-"+temporaire);
            else
                elements.push_back("+"+temporaire);
            m_refpos.setX(0);
        }
        if(m_refpos.getY()>1)
        {
            ecart=((valeur/100)-(m_refpos.getY()-1))*100;
            temporaire = std::to_string(ecart);
            if(getDirectionY()==1)
                elements.push_back("+"+temporaire);
            else
                elements.push_back("-"+temporaire);
            m_refpos.setY(1);
        }
        if(m_refpos.getY()<0)
        {
            ecart=((valeur/100)+m_refpos.getY())*100;
            temporaire = std::to_string(ecart);
            if(getDirectionY()==1)
                elements.push_back("-"+temporaire);
            else
                elements.push_back("+"+temporaire);
            m_refpos.setY(0);
        }


    }
    else if(elements[1]=="pousser"&&m_endpos.getX()!=-1)
    {
        if(contact(mesObjets, numeroObjet, NULL))
        {
            if(mesObjets[numeroObjet]->getRefposX()>m_refpos.getX())
            {
                mesObjets[numeroObjet]->m_refpos.setX(mesObjets[numeroObjet]->getRefposX()+(0.05));
                m_refpos.setX(m_refpos.getX()+(0.05));
                if(mesObjets[numeroObjet]->contact(mesObjets, numeroObjet2, this))
                {
                    mesObjets[numeroObjet]->m_refpos.setX(mesObjets[numeroObjet]->getRefposX()-(mesObjets[numeroObjet]->getTLX()+mesObjets[numeroObjet]->getLongueur()-mesObjets[numeroObjet2]->getTLX())/mesObjets[numeroObjet]->getLongueurParent());
                    m_refpos.setX(m_refpos.getX()-(mesObjets[numeroObjet]->getTLX()+mesObjets[numeroObjet]->getLongueur()-mesObjets[numeroObjet2]->getTLX())/mesObjets[numeroObjet]->getLongueurParent());
                }
            }
            else
            {
                mesObjets[numeroObjet]->m_refpos.setX(mesObjets[numeroObjet]->getRefposX()-(0.05));
                m_refpos.setX(m_refpos.getX()-(0.05));
                if(mesObjets[numeroObjet]->contact(mesObjets, numeroObjet2, this))
                {
                    mesObjets[numeroObjet]->m_refpos.setX(mesObjets[numeroObjet]->getRefposX()+(mesObjets[numeroObjet2]->getTLX()+mesObjets[numeroObjet2]->getLongueur()-mesObjets[numeroObjet]->getTLX())/mesObjets[numeroObjet]->getLongueurParent());
                    m_refpos.setX(m_refpos.getX()+(mesObjets[numeroObjet2]->getTLX()+mesObjets[numeroObjet2]->getLongueur()-mesObjets[numeroObjet]->getTLX())/mesObjets[numeroObjet]->getLongueurParent());
                }
            }
        }
    }
    else if(elements[1]=="attrapper")
    {
        objetCible = elements[0];
        objetActuel = elements[0]+".finger";
        elements.clear();
        elements.push_back(objetActuel);
        elements.push_back("move");
        elements.push_back("0");

        envoiDesOrdres(objetActuel, elements, xCollision, yCollision, mesObjets);
        for(unsigned k=0; k<mesObjets.size(); ++k)
        {
            for(unsigned j=0; j<objetCible.size(); ++j)
            {
                if(objetCible[j]==mesObjets[k]->getID_I(j))
                    correspondance++;
                if(correspondance==objetCible.size())
                    validation = true;          //objet concerne par le deplacement
            }
            if(validation)
            {
                for(unsigned l=0; l<mesObjets.size();++l)
                {
                    for(unsigned m=0; m<6; ++m)
                    {
                        if(finger[m]==mesObjets[l]->getID_I(m))
                            correspondance2++;
                    }
                    if(correspondance2==finger.size())
                        validation2 = true;
                    correspondance2 = 0;
                    if(validation2)
                    {
                        if(mesObjets[l]->contact(mesObjets, numeroObjet, NULL))
                        {
                            if(mesObjets[numeroObjet]->contact(mesObjets, numeroObjet2, mesObjets[l]))
                            {
                                for(unsigned b=0; b<finger.size(); b++)
                                {
                                    if(mesObjets[numeroObjet2]->getID_I(b)==finger[b])
                                        correspondance2++;
                                    if(correspondance2==finger.size())
                                        validation3 = true;
                                }

                                if(validation3)
                                {
                                    mesObjets[numeroObjet]->setRefposX(1);
                                    mesObjets[numeroObjet]->setRefposY(((mesObjets[numeroObjet]->getTLY()-mesObjets[numeroObjet2]->getTLY())/mesObjets[numeroObjet2]->getHauteur())-0.1);
                                    mesObjets[numeroObjet]->setBaseposX(0);
                                    mesObjets[numeroObjet]->setBaseposY(0);
                                    mesObjets[numeroObjet]->supprimerEnfantParent();
                                    if(mesObjets[numeroObjet2]->getTLX()<mesObjets[l]->getTLX())
                                    {
                                        mesObjets[numeroObjet]->setParent(mesObjets[numeroObjet2]);
                                        mesObjets[numeroObjet2]->ajouter1Enfant(mesObjets[numeroObjet]);
                                        mesObjets[numeroObjet2]->m_tenir = true;
                                    }
                                    else
                                    {
                                        mesObjets[numeroObjet]->setParent(mesObjets[l]);
                                        mesObjets[l]->ajouter1Enfant(mesObjets[numeroObjet]);
                                        mesObjets[l]->m_tenir = true;
                                    }
                                }
                            }
                        }
                    }
                    correspondance2 = 0;
                    validation3 = false;
                    validation2 = false;
                }
            }
            validation = false;
            correspondance = 0;
        }
    }
//    else if(elements[1]=="detruire")
//    {
//        if(contact(mesObjets, numeroObjet, NULL))
//        {
//            pointeur = mesObjets[numeroObjet];
//            delete mesObjets[numeroObjet];
//            mesObjets.erase(mesObjets.begin()+numeroObjet);
//
//            for(unsigned k=0; k<mesObjets.size(); k++)
//            {
//                for(unsigned j=0; j<mesObjets[k]->getNbrEnfants(); j++)
//                {
//                    if(mesObjets[k]->getEnfant(j)==pointeur)
//                    {
//                        delete mesObjets[k]->getEnfant(j);
//                        mesObjets[k]->getEnfants().erase(mesObjets[k]->getEnfants().begin()+j);
//                    }
//                }
//            }
//        }
//    }
}

//donne les ordres à une lignee, donc permet de bouger plusieurs blocs en utilisant des chemins([exemple : @bot.arm.finger])
//reçoit la lignee (decomposition des elements donc bot arm etc...), les elements de l'ordre, tous les objets pour les collisions, et x/yCollision
void Objet::donnerOrdreLignee(std::vector<std::string>& lignee, std::vector<std::string>& elements, std::vector<Objet*> mesObjets, unsigned a, double& xCollision, double& yCollision)
{
    bool validation = false, selectionModulo2 = false, test=false;
    unsigned correspondance = 0;
    std::vector<unsigned> objetsATester;            //numero des objets ayant bougé pour tester ensuite les collisions
    std::vector<std::string> modulo;                //stock les valeurs importantes pour la selection par modulo
    unsigned tailleModulo;
    int valeurObjet;
    std::string str;
    char temporaire;

    for(unsigned i=0; i<lignee[a].size();++i)           //test si il y a selection par modulo
    {
        if(lignee[a][i]=='%')
            selectionModulo2 = true;
    }

    if(!selectionModulo2)       //si pas de selection par modulo
    {
        for(unsigned i=0; i<m_mesEnfants.size(); ++i)
        {
            for(unsigned j=0; j<lignee[lignee.size()-1].size(); ++j)
            {
                if(lignee[lignee.size()-1][j]==m_mesEnfants[i]->getID_I(j))
                {
                    correspondance++;
                }
                if(correspondance==lignee[lignee.size()-1].size())
                {
                    validation = true;          //objet concerne par le deplacement
                }
            }
            if(validation)
            {
                m_mesEnfants[i]->donnerOrdre(elements, mesObjets, xCollision, yCollision); //on lui donne donc l'ordre si il est le dernier objet de la lignee concerne
                objetsATester.push_back(i);
                validation = false;
            }
            correspondance=0;
        }
        for(unsigned i=0; i<m_mesEnfants.size(); ++i)
        {
            for(unsigned b=0; b<objetsATester.size();++b)
            {
                if(i==objetsATester[b])
                    test = true;
            }
            if(!test)
            {
                for(unsigned j = 0; j<lignee[a].size(); ++j)
                {
                    if(lignee[a][j]==m_mesEnfants[i]->getID_I(j))
                        correspondance++;
                }
                if(correspondance==lignee[a].size())
                    m_mesEnfants[i]->donnerOrdreLignee(lignee, elements, mesObjets, a+1, xCollision, yCollision);

                m_mesEnfants[i]->donnerOrdreLignee(lignee, elements, mesObjets, a, xCollision, yCollision);
            }
            test = false;
        }
    }
    else            //selection par modulo
    {
        std::istringstream iss(lignee[a]);
        while(std::getline(iss, str, '%'))
            modulo.push_back(str);

        tailleModulo = modulo[0].size();
        for(unsigned i=0; i<tailleModulo-1; ++i)
            modulo[0].erase(modulo[0].begin());

        for(int i=0; i<3; ++i)
            lignee[a].erase(lignee[a].begin()+lignee[a].size()-1);


        for(size_t i=0; i<m_mesEnfants.size(); ++i)
        {
            for(unsigned j=0; j<lignee[a].size(); ++j)
            {
                if(lignee[a][j]==m_mesEnfants[i]->getID_I(j))
                {
                    correspondance++;
                }
                if(correspondance==lignee[a].size())
                    validation = true;
            }
            if(validation)              //si le bloc est concerné par l'ordre
            {
                temporaire = m_mesEnfants[i]->getID_I(m_mesEnfants[i]->getSizeID()-1);
                if(temporaire>=49&&temporaire<=57)
                {
                    valeurObjet = temporaire-48;
                    if(valeurObjet%std::stoi(modulo[1])==std::stoi(modulo[0]))
                    {
                        if(lignee.size()<a+2)       //si il s'agit du dernier bloc concerné par l'ordre
                        {
                            m_mesEnfants[i]->donnerOrdre(elements, mesObjets, xCollision, yCollision);     //on lui donne l'ordre
                            objetsATester.push_back(i);     //on conserve son numéro pour tester par la suite ses collisions
                        }
                        else
                            m_mesEnfants[i]->donnerOrdreLignee(lignee, elements, mesObjets, a+1, xCollision, yCollision);      //sinon on continue pour aller jusqu'au dernier membre de la lignee concern

                        m_mesEnfants[i]->donnerOrdreLignee(lignee, elements, mesObjets, a, xCollision, yCollision);
                    }
                }

                validation = false;
            }
            correspondance=0;
        }
    }
    for(unsigned i=0; i<objetsATester.size(); ++i)                                          //on test les collisions des blocs qu'on a déplacé
        mesObjets[objetsATester[i]]->testCollision(mesObjets, xCollision, yCollision, elements);
}



/*-----------------------------------------------------------COLLISIONS-----------------------------------------------------------*/

//teste les collisions avec des blocs n'appartenant pas à la famille
//reçoit tous les objets a tester,  x/yCollision qui retourne les coordonnées de l'emplacement de la collision pour la signaler visuellement plus tard
void Objet::testCollision(std::vector<Objet*> mesObjets, double& xCollision, double& yCollision, std::vector<std::string>& elements)
{
    int x1 = m_monParent->getTLX()+(m_monParent->getLongueur()*m_refpos.getX())-(m_basepos.getX()*m_longueur);          //coordonées absolu du bloc
    int y1 = m_monParent->getTLY()+(m_monParent->getHauteur()*m_refpos.getY())-(m_basepos.getY()*m_hauteur);
    unsigned f;                     //permet de stocker la valeur de l'objet avec lequel il y a eu contact dans le tableau des objets de la piece

    double directionX=0, directionY=0;      //donne les directions de deplacement du bloc de base

    int xObjet;                             //stock les coordonnées absolues du bloc à tester mais sous forme de int, pour avoir des données moins précise et donc pouvoir tester une collision
    int yObjet;

    bool contact = false, validation = true;            //booleens qui permettent de réaliser des actions si il y a contact et si le bloc à tester n'est pas de la famille du bloc qui se déplace
    double stockX, stockY, distanceX=0, distanceY=0;    //stock les coordonnées absolues du bloc avec lequel il y a eu contact, mais cette fois sous forme de double pour avoir des données plus précises, ainsi que les distances à corriger suite à une collision



    for(unsigned k=0; k<mesObjets.size(); ++k)          //on test tous les objets
    {
        for(unsigned n=0; n<m_mesEnfants.size();++n)
        {
            if(mesObjets[k]==m_mesEnfants[n])
                validation = false;
        }
        if(mesObjets[k]==m_monParent)
            validation = false;                          //on test si le bloc n'est pas de la famille
        if(mesObjets[k]==this)
            validation = false;
        if(testParente(mesObjets[k], m_monParent))
            validation = false;

        if(validation)                          //si le bloc n'est pas de la famille
        {

            xObjet = mesObjets[k]->getTLX();        //on stock donc les coordonnées absolue du bloc qui n'est pas de la famille donc qui peut avoir des collisions
            yObjet = mesObjets[k]->getTLY();

            for(double m = xObjet+1; m<xObjet+mesObjets[k]->getLongueur(); ++m)          //on test tous les points des deux blocs
            {
                for(double g=yObjet+1; g<yObjet+mesObjets[k]->getHauteur(); ++g)
                {
                    for(double i=x1+1; i<x1+m_longueur; ++i)
                    {
                        for(double j=y1+1; j<y1+m_hauteur;++j)
                        {
                            if((i==m)&&(j==g))           //si il y a collision
                            {
                                if(elements[1]!="pousser")
                                {
                                   if(m_monParent->getTLX()+(m_monParent->getLongueur()*m_precedentRefPos.getX())-(m_basepos.getX()*m_longueur)>xObjet+mesObjets[k]->getLongueur())
                                        xCollision = xObjet+mesObjets[k]->getLongueur();
                                    else
                                        xCollision = xObjet;
                                    yCollision = j;
                                }
                                f = k;
                                stockX = xObjet;
                                stockY= yObjet;
                                contact=true;               //on valide le contact
                            }
                        }
                    }
                }
            }
        }
        validation = true;
    }

    if(m_startpos.getX()!=m_endpos.getX())      //si deplacement en x, on donne les directions
    {
        if(m_refpos.getX()>m_precedentRefPos.getX())
            directionX = 1;
        else
            directionX = -1;
    }
    if(m_startpos.getY()!=m_endpos.getY())       //si deplacement en y, on donne les directions
    {
        if(m_refpos.getY()>m_precedentRefPos.getY())
            directionY = 1;
        else
            directionY = -1;
    }


    //std::cout << contact << std::endl;
    if(contact)                                 //si il y a contact
    {
        if(m_startpos.getX()!=m_endpos.getX())              //on vient déplacer le bloc de la distance en trop pour qu'il se colle au bloc avec lequel il entre en collision
        {
            if(m_refpos.getX()>m_precedentRefPos.getX())
                m_refpos.setX(m_refpos.getX()-(x1+m_longueur-stockX)/m_monParent->getLongueur());
            else
                m_refpos.setX(m_refpos.getX()+((stockX+mesObjets[f]->getLongueur()-x1)/m_monParent->getLongueur()));
        }
        if(m_startpos.getY()!=m_endpos.getY())
        {
            if(m_refpos.getY()>m_precedentRefPos.getY())
                m_refpos.setY(m_refpos.getY()-(y1+m_hauteur-stockY)/m_monParent->getHauteur());
            else
                m_refpos.setY(m_refpos.getY()+((stockY+mesObjets[f]->getHauteur()-y1)/m_monParent->getHauteur()));
        }
    }

    for(unsigned i=0; i<m_mesEnfants.size(); ++i)           //si on a pas eu de contact, on veut quand meme tester si les enfants deplacer en meme temps que le bloc entrent en contact avec des blocs n'appartenant pas à la famille
    {
        if(m_mesEnfants[i]->testCollisionEnfant(mesObjets, distanceX, distanceY, this, directionX, directionY, xCollision, yCollision, elements))     //si les enfants entrent en contact avec d'autres blocs, on déplace le bloc de base de la distance retournée
        {
            if(m_startpos.getX()!=m_endpos.getX())
            {
                if(m_refpos.getX()>m_precedentRefPos.getX())
                    m_refpos.setX(m_refpos.getX()-distanceX/m_monParent->getLongueur());
                else
                    m_refpos.setX(m_refpos.getX()+distanceX/m_monParent->getLongueur());
            }
            if(m_startpos.getY()!=m_endpos.getY())
            {
                if(m_refpos.getY()>m_precedentRefPos.getY())
                    m_refpos.setY(m_refpos.getY()-distanceY/m_monParent->getHauteur());
                else
                    m_refpos.setY(m_refpos.getY()+distanceY/m_monParent->getHauteur());
            }
        }
    }

}

//retourne vrai si les enfants sont entres en collision avec des objets de la scene
//reçoit tous les objets de la piece a tester, les distancesX/Y qui permettent de savoir de quelle distance on va déplacer l'objet de base pour se coller contre le bloc en collision
//l'objet de base en question donc, les directionsX/Y de l'objet de base pour bien décaler les blocs des bons cotés, et enfin x/yCollision qui retourne les coordonnées de l'emplacement de la collision pour la signaler visuellement plus tard
//elements qui est le tableau décomposant les ordres, ici il nous permet de ne pas afficher les cercles de collision si on a juste pousser
bool Objet::testCollisionEnfant(std::vector<Objet*> mesObjets, double& distanceX, double& distanceY, Objet* objetBase, double& directionX, double& directionY, double& xCollision, double& yCollision, std::vector<std::string>& elements)
{
    int x1 = m_monParent->getTLX()+(m_monParent->getLongueur()*m_refpos.getX())-(m_basepos.getX()*m_longueur)-distanceX;        //coordonées absolu du bloc
    int y1 = m_monParent->getTLY()+(m_monParent->getHauteur()*m_refpos.getY())-(m_basepos.getY()*m_hauteur)-distanceY;
    unsigned f;             //permet de stocker la valeur de l'objet avec lequel il y a eu contact dans le tableau des objets de la piece

    int xObjet;             //stock les coordonnées absolues du bloc à tester mais sous forme de int, pour avoir des données moins précise et donc pouvoir tester une collision
    int yObjet;

    bool contact = false, validation = true;        //booleens qui permettent de réaliser des actions si il y a contact et si le bloc à tester n'est pas de la famille du bloc qui se déplace
    double stockX, stockY;                          //stock les coordonnées absolues du bloc avec lequel il y a eu contact, mais cette fois sous forme de double pour avoir des données plus précises



    for(unsigned k=0; k<mesObjets.size(); ++k)      //on test tous les objets
    {
        for(unsigned n=0; n<m_mesEnfants.size();++n)
        {
            if(mesObjets[k]==m_mesEnfants[n])
                validation = false;
        }
        if(mesObjets[k]==m_monParent)                       //on test si le bloc n'est pas de la famille
            validation = false;
        if(mesObjets[k]==this)
            validation = false;
        if(testParente(mesObjets[k], m_monParent))
            validation = false;

        if(validation)                                      //si le bloc n'est pas de la famille
        {

            xObjet = mesObjets[k]->getTLX();                //on stock donc les coordonnées absolue du bloc qui n'est pas de la famille donc qui peut avoir des collisions
            yObjet = mesObjets[k]->getTLY();

            for(double m = xObjet+1; m<xObjet+mesObjets[k]->getLongueur(); ++m)      //on test tous les points des deux blocs
            {
                for(double g=yObjet+1; g<yObjet+mesObjets[k]->getHauteur(); ++g)
                {
                    for(double i=x1+1; i<x1+m_longueur; ++i)
                    {
                        for(double j=y1+1; j<y1+m_hauteur;++j)
                        {
                            if((i==m)&&(j==g))              //si il y a collision
                            {
                                if(elements[1]!="pousser")
                                {
                                   if(m_monParent->getTLX()+(m_monParent->getLongueur()*m_precedentRefPos.getX())-(m_basepos.getX()*m_longueur)>xObjet+mesObjets[k]->getLongueur())
                                        xCollision = xObjet+mesObjets[k]->getLongueur();
                                    else
                                        xCollision = xObjet;
                                    yCollision = j;
                                }
                                f = k;
                                stockX = xObjet;
                                stockY = yObjet;
                                contact = true;             //on valide le contact
                            }
                        }
                    }
                }
            }
        }
        validation = true;
    }

    if(contact)         //si il y a eu contact avec le bloc
    {
        if(directionX!=0)       //si deplacement en x
        {
            if(directionX==1)   //déplacement vers la droite
            {
                distanceX += (x1+m_longueur-stockX);        //distance que l'on va retirer à la position de l'objet de base
                for(unsigned i=0; i<m_mesEnfants.size();++i)
                {
                    if(m_mesEnfants[i]->getLongueur()+m_mesEnfants[i]->getTLX()-distanceX>=xObjet)  //si les coordonnes absolues des enfants entrent en contact avec le bloc, on veut continuer de déplacer vers la gauche le bloc
                        m_mesEnfants[i]->testCollisionEnfant(mesObjets, distanceX, distanceY, objetBase, directionX, directionY, xCollision, yCollision, elements);
                }
            }
            else            //deplacement vers la gauche
            {
                distanceX += (stockX+mesObjets[f]->getLongueur()-x1);
                for(unsigned i=0; i<m_mesEnfants.size();++i)
                {
                    if(objetBase->getTLX()+distanceX+m_mesEnfants[i]->getLongueur()<xObjet+mesObjets[f]->getLongueur())
                        m_mesEnfants[i]->testCollisionEnfant(mesObjets, distanceX, distanceY, objetBase, directionX, directionY, xCollision, yCollision, elements);
                }
            }
        }
        if(directionY!=0)       //si deplacement en y
        {
            if(directionY==1)
            {
                distanceY += (y1+m_hauteur-stockY);
                for(unsigned i=0; i<m_mesEnfants.size();++i)
                    if(objetBase->getTLY()-distanceY+m_mesEnfants[i]->getHauteur()>yObjet)
                        m_mesEnfants[i]->testCollisionEnfant(mesObjets, distanceX, distanceY, objetBase, directionX, directionY, xCollision, yCollision, elements);
            }
            else
            {
                distanceY +=(stockY+mesObjets[f]->getHauteur()-y1);
                for(unsigned i=0; i<m_mesEnfants.size();++i)
                    if(objetBase->getTLY()+distanceY+m_mesEnfants[i]->getHauteur()<yObjet+mesObjets[f]->getHauteur())
                        m_mesEnfants[i]->testCollisionEnfant(mesObjets, distanceX, distanceY, objetBase, directionX, directionY, xCollision, yCollision, elements);
            }
        }


        return 1;           //il y a eu contact donc on return 1
    }

    for(unsigned i=0; i<m_mesEnfants.size(); ++i)
    {
        if(m_mesEnfants[i]->testCollisionEnfant(mesObjets, distanceX, distanceY, this, directionX, directionY, xCollision, yCollision, elements))     //on test à nouveau avec les enfants de l'enfant cette fois pour parcourir toute la famille à partir du bloc déplacé de base
        {
            if(m_startpos.getX()!=m_endpos.getX())      //si deplacement en x                                       //on ajuste les positions avec les distances retournés de trop
            {
                if(m_refpos.getX()>m_precedentRefPos.getX())
                    m_refpos.setX(m_refpos.getX()-distanceX/m_monParent->getLongueur());
                else
                    m_refpos.setX(m_refpos.getX()+distanceX/m_monParent->getLongueur());
            }
            if(m_startpos.getY()!=m_endpos.getY())      //si deplacement en y
            {
                if(m_refpos.getY()>m_precedentRefPos.getY())    //deplacement vers le bas
                    m_refpos.setY(m_refpos.getY()-distanceY/m_monParent->getHauteur());
                else                                           //deplacement vers le haut
                    m_refpos.setY(m_refpos.getY()+distanceY/m_monParent->getHauteur());
            }
            return 1;       //il y a eu contact donc on return 1
        }
    }
    return 0;       //pas de contact
}

//retourne vrai si l'objet envoyé en tant que objetActuel est l'enfant envoyé en paramètre ou l'enfant de l'enfant envoyé en paramètre
//permet de tester tous les enfants de la lignée
bool testEnfante(Objet* enfant, Objet* objetActuel)
{
    if(objetActuel==enfant)
    {
        return 1;
    }
    if(enfant->getNbrEnfants()>0)
    {
        for(unsigned i=0; i<enfant->getNbrEnfants(); ++i)
        {
           if(testEnfante(enfant->getEnfant(i), objetActuel))
                return 1;
        }
    }

    return 0;
}

//retourne vrai si l'objetActuel testé appartient à la lignée pour les collisions
//si c'est le cas on ne veut pas prendre en compte les collisions avec cet objet
bool testParente(Objet*& objetActuel, Objet*& parent)
{
    if(parent->getParent()!=NULL&&parent->getIDParent()!="room")
    {
        if(parent->getIDParentParent()!="room")
            for(unsigned i=0; i<parent->getNbrEnfantsParent(); ++i)
            {
                if(testEnfante(parent->getEnfantIParent(i), objetActuel))
                    return 1;
            }

        if(objetActuel==parent->getParent())
            return 1;

        testParente(objetActuel, parent->getParent());
    }

    return 0;
}

//retourne vrai si il y a un contact avec un bloc qui n'est pas de la famille et permet de connaitre le numero de ce bloc
//pas d'autre action
bool Objet::contact(std::vector<Objet*> mesObjets, unsigned& numeroObjet, Objet* exceptionObjet)
{
    int x1 = m_monParent->getTLX()+(m_monParent->getLongueur()*m_refpos.getX())-(m_basepos.getX()*m_longueur);        //coordonées absolu du bloc
    int y1 = m_monParent->getTLY()+(m_monParent->getHauteur()*m_refpos.getY())-(m_basepos.getY()*m_hauteur);
    bool validation = true, contact = false;
    int xObjet, yObjet;

    for(unsigned k=0; k<mesObjets.size(); ++k)          //on test tous les objets
    {
        if(exceptionObjet!=mesObjets[k])
        {
            for(unsigned n=0; n<m_mesEnfants.size();++n)
            {
                if(mesObjets[k]==m_mesEnfants[n])
                    validation = false;
            }
            if(mesObjets[k]==m_monParent)
                validation = false;                          //on test si le bloc n'est pas de la famille
            if(mesObjets[k]==this)
                validation = false;
            if(testParente(mesObjets[k], m_monParent))
                validation = false;

            if(validation)                          //si le bloc n'est pas de la famille
            {

                xObjet = mesObjets[k]->getTLX();        //on stock donc les coordonnées absolue du bloc qui n'est pas de la famille donc qui peut avoir des collisions
                yObjet = mesObjets[k]->getTLY();

                for(double m = xObjet; m<=xObjet+mesObjets[k]->getLongueur(); ++m)          //on test tous les points des deux blocs
                {
                    for(double g=yObjet; g<=yObjet+mesObjets[k]->getHauteur(); ++g)
                    {
                        for(double i=x1; i<=x1+m_longueur; ++i)
                        {
                            for(double j=y1; j<=y1+m_hauteur;++j)
                            {
                                if((i==m)&&(j==g))           //si il y a collision
                                {
                                    numeroObjet = k;
                                    contact=true;               //on valide le contact
                                }
                            }
                        }
                    }
                }
            }
            validation = true;
        }

    }

    if(contact)
        return 1;

    return 0;
}

/*-----------------------------------------------------------SAUVEGARDE-----------------------------------------------------------*/

//convertit les coordonnées relatives en positions relatives
//reçoit le fichier dans lequel écrire et les coordonnées à convertir
void ecrirePositionFichier(Coords position,std::ofstream &sauvegarde)
{
    if(position.getX()==0 && position.getY()==0)
        sauvegarde << "tl";
    if(position.getX()==0.5 && position.getY()==0)
        sauvegarde << "tc";
    if(position.getX()==1 && position.getY()==0)
        sauvegarde << "tr";
    if(position.getX()==0 && position.getY()==0.5)
            sauvegarde << "ml";
    if(position.getX()==0.5 && position.getY()==0.5)
            sauvegarde << "mc";
    if(position.getX()==1 && position.getY()==0.5)
            sauvegarde << "mr";
    if(position.getX()==0 && position.getY()==1)
            sauvegarde << "bl";
    if(position.getX()==0.5 && position.getY()==1)
            sauvegarde << "bc";
    if(position.getX()==1 && position.getY()==1)
            sauvegarde << "br";
}

//sauvegarde tous les objets
//reçoit le niveau dans le fichier, tous les objets à sauvegarder et le fichier dans lequel sauvegarder
void Objet::sauvegarder(unsigned niveau, std::vector<Objet*>& mesObjets,std::ofstream &sauvegarde)
{
    if ( !sauvegarde )
        throw std::runtime_error( "Can't read/open test.txt" );

    else
    {
        for (unsigned j=0;j<niveau+1;j++)
            sauvegarde << "\t";

        if(m_id != "")                  //sauvegarde les données du bloc dans le fichier
        {
            sauvegarde << "id= ";
            sauvegarde << m_id;
            sauvegarde << " ";
        }
        sauvegarde << "width= ";
        sauvegarde << m_longueur;
        sauvegarde << " height= ";
        sauvegarde << m_hauteur;
        sauvegarde << " color= ";
        sauvegarde << m_couleur << std::endl;

        for (unsigned j=0;j<niveau+1;j++)
            sauvegarde << "\t";

        sauvegarde << "refpos= ";
        ecrirePositionFichier(m_startpos,sauvegarde);
        if(m_refpos.getX()!=m_startpos.getX())
        {
            sauvegarde << " refposx%= ";
            sauvegarde << (m_refpos.getX() - m_startpos.getX())*100;
        }
        if(m_refpos.getY()!=m_startpos.getY())
        {
            sauvegarde << " refposy%= ";
            sauvegarde << (m_refpos.getY() - m_startpos.getY())*100;
        }
        if(m_endpos.getX()!=-1)
        {
            sauvegarde << " endpos= ";
            ecrirePositionFichier(m_endpos,sauvegarde);
        }
        sauvegarde << " basepos= ";
        ecrirePositionFichier(m_basepos,sauvegarde);
        if(m_endpos.getX()!=-1)
            sauvegarde << " translate= 0";

        sauvegarde << std::endl;
    }

    if(m_mesEnfants.size()!=0)                      //si le bloc en train d'être sauvegardé à des enfants
    {
        for (unsigned j=0; j<niveau+1; j++)
            sauvegarde << "\t";
        sauvegarde << "[" << std::endl;             //on ajoute un crochet ouvrant pour sauvegarder ses enfants
        niveau++;                                   //on change de niveau
        for(unsigned j=0; j<m_mesEnfants.size();++j)
        {
            m_mesEnfants[j]->sauvegarder(niveau, mesObjets, sauvegarde);        //on sauvegarde tous les enfants
            if(j!=m_mesEnfants.size()-1)
                sauvegarde << std::endl;                                        //tant qu'il reste des enfants on saute des lignes
            else                                                                //quand il n'y a plus d'enfants, on ferme le crochet et donc on arrete de sauvegarder des enfants par rapport à l'objet
            {
                for (unsigned j=0; j<niveau; j++)
                    sauvegarde << "\t";
                sauvegarde << "]" << std::endl;
            }
        }
    }
}
