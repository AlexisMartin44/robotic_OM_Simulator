#include "room.h"
#include "objet.h"
#include "src/svg/svgfile.h"
#include <fstream> /// Pour les fichiers
#include <sstream> /// Pour les ostringstream
#include <iostream>
#include <string.h>

/*---------------------------------------------------------------------------------------------*/
//Code entièrement réalisé par Alexis Martin, Zoé Crouzet et Noémie Devergne
//Nous nous sommes basé sur le chargement de M.Segado pour charger nos objets
//Nous avons aussi utilisé une fonction std::stoi qui transforme un string en entier trouvée sur internet
//De plus, nous avons utilisé une fonction de mise en pause du système trouvée sur internet
/*---------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------CONSTRUCTEUR-----------------------------------------------------------*/

//construit une room
Room::Room() : Objet(NULL), m_bordure{""}
{
    m_monParent = NULL;
}

//destructeur des rooms
Room::~Room()
{
}

/*------------------------------------------------------SSPRGS---------------------------------------------------------------------*/

//dessine la room avec sa bordure
//reçoit le svgout
void Room::dessiner(Svgfile& svgout)
{
    svgout.addTriangle(75, 100, 300, 0, 525, 100, "brown", 1, "black");
    svgout.addDisk(300, 50, 20, "lightblue");
    svgout.addLine(180, -50, 220, -50, "black");
    svgout.addLine(200, -70, 200, -30);
    svgout.addRectangle(0, 0, m_longueur, m_hauteur, m_couleur, 1, m_bordure);
}

//affiche les informations de la room
void Room::afficher() const
{
    std::cout << m_id << " / " << m_longueur << " / " << m_hauteur << " / " << m_couleur << " / " << m_bordure << " / ";
    m_refpos.afficher();
    std::cout << "\t";
    m_basepos.afficher();
    std::cout << "\t";
    m_endpos.afficher();
    std::cout << std::endl;
}

//convertit les strings de positions relatives en coordonnees relatives
//reçoit donc le tampon qui correspond  au string de position relative, ainsi que x et y qui seront les variables résultants de la conversion en coordonnes
void position(std::string tampon, double& x, double& y)
{
    if(tampon=="tl")
    {
        x = 0;
        y = 0;
    }
    if(tampon=="tc")
    {
        x = 0.5;
        y = 0;
    }
    if(tampon=="tr")
    {
        x = 1;
        y = 0;
    }
    if(tampon=="ml")
    {
        x = 0;
        y = 0.5;
    }
    if(tampon=="mc")
    {
        x = 0.5;
        y = 0.5;
    }
    if(tampon=="mr")
    {
        x = 1;
        y = 0.5;
    }
    if(tampon=="bl")
    {
        x = 0;
        y = 1;
    }
    if(tampon=="bc")
    {
        x = 0.5;
        y = 1;
    }
    if(tampon=="br")
    {
        x = 1;
        y = 1;
    }

}

/*-----------------------------------------------------------CHARGEMENT/SAUVEGARDE-----------------------------------------------------------*/

//charge la room d'un fichier.rom
//reçoit tous les objets de la piece, la room et le nom du fichier a charger
bool Room::chargerData(std::vector<Objet*>& mesObjets, Objet room, std::string nomFichier)
{
    int cpt = 0;
    std::string tampon, ligne, tamponPrecedent = " ";
    int tamponInt;
    double x, y;
    std::ifstream ifs{nomFichier};

    if(!ifs)
        return false;
    else
    {
        while ( std::getline(ifs, ligne) )
        {
            if ( blankLine(ligne)  )                //si ligne vide, passe a la suivante
                continue;
            if ( ligne.size()>=2 && ligne[0]=='/' && ligne[1]=='/' )        //si la ligne correspond a des commentaires, ne la lit pas passe à la suivante
                continue;

            std::istringstream iss{ligne};

            if(cpt==0)                                                      //premiere ligne
            {
                do
                {
                    tamponPrecedent = tampon;                               //on attribut aux variables les bonnes donnés
                    iss >> tampon;
                    if(tampon=="id=")
                    {
                        iss >> tampon;
                        m_id = tampon;
                    }
                    else if(tampon=="width=")
                    {
                        iss >> tamponInt;
                        m_longueur = tamponInt;
                    }
                    else if(tampon=="height=")
                    {
                        iss >> tamponInt;
                        m_hauteur = tamponInt;
                    }
                    else if(tampon=="color=")
                    {
                        iss >> tampon;
                        m_couleur = tampon;
                    }
                    else if(tampon=="border=")
                    {
                        iss >> tampon;
                        m_bordure = tampon;
                    }
                }while(tampon!=tamponPrecedent);
            }
            else if(cpt==1)                                             //deuxième ligne
            {
                do
                {
                    tamponPrecedent = tampon;
                    iss >> tampon;
                    if(tampon=="refpos=")
                    {
                        iss >> tampon;
                        position(tampon, x, y);                        //convertit les positions relatives en coordonnées relatives
                        m_refpos.setX(x);
                        m_refpos.setY(y);
                    }
                    else if(tampon=="basepos=")
                    {
                        iss >> tampon;
                        position(tampon, x, y);
                        m_basepos.setX(x);
                        m_basepos.setY(y);
                    }
                }while(tampon!=tamponPrecedent);
            }
            else
            {
                iss >> tampon;                                      //on charge tous les objets de la scene

                Objet* nouveau = new Objet(this);
                nouveau->chargerData(ifs, mesObjets);
                mesObjets.push_back(nouveau);
                m_mesEnfants.push_back(nouveau);

                cpt = 0;
            }

            cpt++;

        }
    }

    return true;

}

//sauvegarde la scene actuelle dans un fichier
void Room::sauvegarder()
{
    std::ofstream sauvegarde("sauvegarde.rom");         //ouvre le fichier de sauvegarde

    if ( !sauvegarde )
        throw std::runtime_error( "Can't read/open test.txt" );

    else                                                //on récupère les données de la room et on les écrit dans le fichier
    {
        sauvegarde << "id= ";
        sauvegarde << m_id;
        sauvegarde << " width= ";
        sauvegarde << m_longueur;
        sauvegarde << " height= ";
        sauvegarde << m_hauteur;
        sauvegarde << " color= ";
        sauvegarde << m_couleur;
        sauvegarde << " border= ";
        sauvegarde << m_bordure << std::endl;

        sauvegarde << "refpos= ";
        ecrirePositionFichier(m_startpos,sauvegarde);           //on convertit cette fois les coordonnées relatives en positions relatives

        sauvegarde << " basepos= ";
        ecrirePositionFichier(m_basepos,sauvegarde);
        sauvegarde << std::endl;
    }
}
