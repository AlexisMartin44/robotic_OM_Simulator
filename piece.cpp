#include "piece.h"
#include <iterator>
#include <sstream>
#include <map>
#define WIN32
#include "ConsoleExtension.h"
#include <chrono>
#include <thread>
#include "script.h"

/*---------------------------------------------------------------------------------------------*/
//Code entièrement réalisé par Alexis Martin, Zoé Crouzet et Noémie Devergne
//Nous nous sommes basé sur le chargement de M.Segado pour charger nos objets
//Nous avons aussi utilisé une fonction std::stoi qui transforme un string en entier trouvée sur internet
//De plus, nous avons utilisé une fonction de mise en pause du système trouvée sur internet
/*---------------------------------------------------------------------------------------------*/


/*------------------------------------------------------CONSTRUCTEURS---------------------------------------------------------------------*/

//Constructeur de base d'une piece
Piece::Piece() : m_mesObjets{}, m_mesRooms{}, m_romActuelle{"sauvegarde.rom"}
{
}

//Destructeur
Piece::~Piece()
{
}

//Constructeur pour la copie, permet de mettre en place le système de Store / Restore
//Reçoit en paramètre la pièce a copier
Piece::Piece(Piece* piece) : m_mesObjets{}, m_mesRooms{}, m_romActuelle{"sauvegarde.rom"}
{
    std::map<Objet*, Objet*> transpose;               //on vient stocker tous les objets créés dans un map, en disant à quel objet il correspond pour pouvoir par la suite liés les bons enfants/parents
    for(unsigned i=0; i<piece->getSizeRoom(); ++i)
    {
        Room* nouveau = new Room(*piece->m_mesRooms[i]);    //On crée simplement une nouvelle room copie de l'ancienne
        transpose[piece->m_mesRooms[0]] = nouveau;
        m_mesRooms.push_back(nouveau);
    }

    for(unsigned i=0; i<piece->getSizeObjets(); ++i)
    {
        Objet* nouveau = new Objet(piece->m_mesObjets[i], 1);       //On recrée tous les objets en copiant les anciens, mais il manque les enfants et les parents
        transpose[piece->m_mesObjets[i]] = nouveau;

        m_mesObjets.push_back(nouveau);
    }
    for(unsigned i=0; i<m_mesObjets.size(); ++i)                    //On ajoute aux objets leurs parents et enfants en utilisant le map
    {
        for(unsigned j=0; j<m_mesObjets.size(); ++j)
        {
            if(piece->m_mesObjets[j]==piece->getParentObjet(i))
                m_mesObjets[i]->ajouterParent(transpose, piece->m_mesObjets[j]);

            if(piece->m_mesRooms[0]==piece->getParentObjet(i))
                m_mesObjets[i]->ajouterParent(transpose, piece->m_mesRooms[0]);

            for(unsigned k=0; k<piece->nbrEnfantObjet(i); ++k)
            {
                if(piece->m_mesObjets[j]==piece->getEnfantObjet(k, i))
                    m_mesObjets[i]->ajouterEnfant(transpose, piece->m_mesObjets[j]);
            }
        }
    }
}




/*------------------------------------------------------GETTERS---------------------------------------------------------------------*/

//retourne l'enfant k d'un objet i de la piece
Objet* Piece::getEnfantObjet(int k, int i)
{
    return m_mesObjets[i]->getEnfant(k);
}

//retourne le nombre d'enfants d'un objet i de la piece
unsigned Piece::nbrEnfantObjet(int i)
{
    return m_mesObjets[i]->getNbrEnfants();
}

//retourne le parent d'un objet i de la piece
Objet* Piece::getParentObjet(int i)
{
    return m_mesObjets[i]->getParent();
}

//retourne le nombre de rooms d'une pièce
unsigned Piece::getSizeRoom()
{
    return m_mesRooms.size();
}

//retourne le nombre d'objets de la piece
unsigned Piece::getSizeObjets()
{
    return m_mesObjets.size();
}






/*------------------------------------------------------SSPRGS---------------------------------------------------------------------*/

//affiche tous les éléments de la piece
void Piece::afficher()
{
    m_mesRooms[0]->afficher();
    for(unsigned i=0; i<m_mesObjets.size(); ++i)
        m_mesObjets[i]->afficher();
}

//dessine l'état actuel de la piece, donc toutes les rooms et objets
//reçoit en paramètre showAxes (=1 si on veut afficher les axes), showNames (=1 si on veut afficher les noms), xCollision (x de l'objet dans lequel rentre l'objet deplace) et yCollision (idem avec y)
void Piece::dessinerSVG(bool showAxes, bool showNames, double& xCollision, double& yCollision)
{
    Svgfile svgout(showAxes, showNames);
    svgout.addGrid();
    m_mesRooms[0]->dessiner(svgout);                                                                 //dessine la room
    for(int i=m_mesObjets.size()-1; i>=0; --i)                                                      //dessine les objets
        m_mesObjets[i]->dessiner(svgout, xCollision, yCollision);
}

//charge la piece depuis la dernière sauvegarde
void Piece::chargerScene()
{
    double xCollision=0, yCollision=0;
    Room* nouveau = new Room();                     //on crée une nouvelle room
    m_mesRooms.push_back(nouveau);

    m_mesRooms[0]->chargerData(m_mesObjets, m_mesRooms[0], "sauvegarde.rom");        //on vient ensuite charger les éléments du fichier
    m_mesRooms[0]->afficher();                                                      //On affiche les élements chargés dans la console
    for(unsigned i=0; i<m_mesObjets.size(); ++i)
        m_mesObjets[i]->afficher();
    dessinerSVG(false, false, xCollision, yCollision);                              //on vient pour la première fois dessiner la scene

}

//s'occupe d'envoyer les ordres à tous les blocs concernés par la commande
//reçoit de nombreux paramètres, l'objetActuel qui est le string designant les objets concernés par le déplacement
//elements qui décompose tous les ordre ([exemple : elements[0] = bot.arm.finger; elements[1] = move etc....]),
// xCollision qui est le x de l'objet contre lequel l'objet déplacé se cogne, yCollision idem avec y
void envoiDesOrdres(std::string& objetActuel, std::vector<std::string>& elements, double& xCollision, double& yCollision, std::vector<Objet*>& mesObjets)
{
    bool selectionModulo = false, selectionModulo2 = false;         //si on veut sélectionner des objets par % en fonction de leur numéro, =1
    std::vector<std::string> modulo;                                //stock les valeurs de l'opération saisie ([exemple: @bot1%3 on stock 1 et 3])
    std::vector<int> objetsATester;                                 //on stocke ici les valeurs de tous les objets déplacés pour ensuite détecter d'éventuelles collisions
    unsigned tailleModulo;                                          //permet de supprimer les infos superflus dans la récupération des données de l'objet
    int valeurObjet;                                                //stock la valeur de chaque objet ([exemple : 1 si l'objet est arm1])
    char temporaire;                                         //sert de tampon
    std::vector<std::string> lignee;                                //décompose la lignee ([exemple: lignee[0] = bot ; lignee[1] = arm etc....])
    std::string str;                                                //sert aussi de tampon pour les décompositions
    unsigned correspondance = 0;                                    //permet de selectionner tous les objets donc l'ID commence par la saisie utilisateur ([exemple : @bot sélectionne bot1 bot2 etc...])
    bool validation = false, selectionEnfant = false;               //booleens qui permettent de réaliser des actions en fonction de si l'objet est concerne et si il y a une selection par enfant

    for(unsigned i=0; i<objetActuel.size(); ++i)                    //si il y a un . dans l'objet saisie par l'utilisateur, alors il y a une lignee donc on fera d'autres opérations
    {
        if(objetActuel[i]=='.')
            selectionEnfant = true;
    }

    for(unsigned i=0; i<objetActuel.size();++i)                     //si il y a un . dans l'objet saisie par l'utilisateur, alors il y aura une sélection par modulo
    {
        if(objetActuel[i]=='%')
        {
            selectionModulo = true;
        }
    }

    if(selectionEnfant==false)
    {
        if(selectionModulo==true)                                   //pas de lignee mais selection par modulo
        {
            std::istringstream iss(objetActuel);
            while(std::getline(iss, str, '%'))                      //on décompose les éléments et on les stock dans modulo
                modulo.push_back(str);

            tailleModulo = modulo[0].size();
            for(unsigned i=0; i<tailleModulo-1; ++i)                //on enleve l'objet avant le numéro du % ([exemple : bot1%3 on stock 1 pas bot1])
                modulo[0].erase(modulo[0].begin());
            for(int i=0; i<3; ++i)
                objetActuel.erase(objetActuel.begin()+objetActuel.size()-1);


            for(size_t i=0; i<mesObjets.size(); ++i)
            {
                for(unsigned j=0; j<objetActuel.size(); ++j)
                {
                    if(elements[0][j]==mesObjets[i]->getID_I(j))      //on test si les ID des objets commencent comme l'objet saisie par l'utilisateur
                    {
                        correspondance++;
                    }
                    if(correspondance==objetActuel.size())              //si c'est le cas, on valide
                        validation = true;
                }
                if(validation)                                          //si on a validé, alors les objets concernés vont subir un mvt
                {
                    temporaire = mesObjets[i]->getID_I(mesObjets[i]->getSizeID()-1);
                    if(temporaire>=49&&temporaire<=57)
                    {
                        valeurObjet = temporaire-48;
                        if(valeurObjet%std::stoi(modulo[1])==std::stoi(modulo[0]))      //On ne veut que les objets concordant avec les résultats du modulo
                        {
                            mesObjets[i]->donnerOrdre(elements, mesObjets, xCollision, yCollision);         //si il valident le test, alors on leur donne l'ordre de se déplacer
                            objetsATester.push_back(i);                                 //on stock la valeur de ces objets pour ensuite tester les collisions
                        }
                        validation = false;
                    }
                }
                correspondance=0;
            }
            for(unsigned i=0; i<objetsATester.size(); ++i)
                mesObjets[objetsATester[i]]->testCollision(mesObjets, xCollision, yCollision, elements);
            objetsATester.clear();
        }
        else                                                                        //pas de lignee et pas de selection modulo
        {
            for(size_t i=0; i<mesObjets.size(); ++i)
            {
                for(unsigned j=0; j<objetActuel.size(); ++j)
                {
                    if(elements[0][j]==mesObjets[i]->getID_I(j))                  //on test si les ID des objets commencent comme l'objet saisie par l'utilisateur
                    {
                        correspondance++;
                    }
                    if(correspondance==objetActuel.size())
                        validation = true;
                }
                if(validation)
                {
                    mesObjets[i]->donnerOrdre(elements, mesObjets, xCollision, yCollision);
                    objetsATester.push_back(i);
                    validation = false;
                }
                correspondance=0;
            }
            for(unsigned i=0; i<objetsATester.size(); ++i)
                mesObjets[objetsATester[i]]->testCollision(mesObjets, xCollision, yCollision, elements);
            objetsATester.clear();

        }
    }
    else                                                                         //selection par lignee
    {
        std::istringstream iss(objetActuel);
        while(std::getline(iss, str, '.'))                                      //on décompose les élements, lignee stock chaque objet de la lignee un par un
            lignee.push_back(str);

        for(unsigned i=0; i<lignee[0].size();++i)                               //verifie si il y a une selection par modulo
        {
            if(lignee[0][i]=='%')
                selectionModulo2 = true;
        }
        if(!selectionModulo2)                                                   //si pas de selection par modulo
        {

            for(unsigned i=0; i<mesObjets.size(); ++i)
            {
                for(unsigned j=0; j<lignee[0].size(); ++j)
                {
                    if(lignee[0][j]==mesObjets[i]->getID_I(j))                //on test si les ID des objets commencent comme l'objet saisie par l'utilisateur
                        correspondance++;
                    if(correspondance==lignee[0].size())
                        validation = true;
                }
                if(validation)
                {
                    mesObjets[i]->donnerOrdreLignee(lignee, elements, mesObjets, 1, xCollision, yCollision);    //si oui, on envoie les objets suivants dans la fonction donnerOrdreLignee
                    validation = false;
                }
                correspondance = 0;
            }
        }
        else                                                                    //si selection par lignee et par modulo, identique a plus haut
        {
            std::istringstream iss(lignee[0]);
            while(std::getline(iss, str, '%'))
                modulo.push_back(str);

            tailleModulo = modulo[0].size();
            for(unsigned i=0; i<tailleModulo-1; ++i)
                modulo[0].erase(modulo[0].begin());
            for(int i=0; i<3; ++i)
                lignee[0].erase(lignee[0].begin()+lignee[0].size()-1);


            for(size_t i=0; i<mesObjets.size(); ++i)
            {
                for(unsigned j=0; j<lignee[0].size(); ++j)
                {
                    if(elements[0][j]==mesObjets[i]->getID_I(j))
                    {
                        correspondance++;
                    }
                    if(correspondance==lignee[0].size())
                        validation = true;
                }
                if(validation)
                {
                    temporaire = mesObjets[i]->getID_I(mesObjets[i]->getSizeID()-1);
                    valeurObjet = temporaire-48;
                    if(valeurObjet%std::stoi(modulo[1])==std::stoi(modulo[0]))
                    {
                        mesObjets[i]->donnerOrdreLignee(lignee, elements, mesObjets, 1, xCollision, yCollision);
                        objetsATester.push_back(i);
                    }
                    validation = false;
                }
                correspondance=0;
            }
            for(unsigned i=0; i<objetsATester.size(); ++i)
                mesObjets[objetsATester[i]]->testCollision(mesObjets, xCollision, yCollision, elements);
            objetsATester.clear();
        }

        lignee.clear();
        selectionEnfant = false;
    }
}






/*------------------------------------------------------MENU---------------------------------------------------------------------*/

//permet de donner un ordre aux objets concernes par la saisie*
//reçoit objetActuel qui est l'objet (ou les objets) concerne par la saisie, xCollision, yCollision, showAxes, showNames (expliques plus haut)
//tampon qui est l'ensemble de la saisie utilisateur
void Piece::objetSelectionArobase(std::string& objetActuel, double& xCollision, double& yCollision, bool& showAxes, bool& showNames, std::string& tampon)
{
    std::string str;
    objetActuel = "";
    std::vector<std::string> elements;
    std::istringstream iss(tampon);

    while(std::getline(iss, str, ' '))                                     //decompose les elements avant et apres les espaces
        elements.push_back(str);

    elements[0].erase(elements[0].begin());                             //supprime @ qui est genant
    objetActuel = elements[0];

    envoiDesOrdres(objetActuel, elements, xCollision, yCollision, m_mesObjets);      //on appelle la fonction d'envoie des ordres
    if(xCollision!=0&&yCollision!=0)                                    //si il y a eu une collision, les valeurs de xCollision et yCollision ne sont plus à 0 donc on va vouloir dessiner des cercles à l'emplacement de la collision pour la signaler
    {
        dessinerSVG(showAxes, showNames, xCollision, yCollision);
        std::this_thread::sleep_for(std::chrono::milliseconds(750));    //petite pause du systeme pour avoir le temps de voir où a lieu la collision
        dessinerSVG(showAxes, showNames, xCollision, yCollision);
        xCollision = 0;
        yCollision = 0;
    }

    if(elements.size()<4)                                               //on ajoute l'action à l'historique
        m_historique.ajouterUndo(tampon);
    else
        m_historique.ajouterUndo("@"+objetActuel+" move "+elements[3]);
    m_historique.supprimerRedo();
}


//si un objet a deja subit un ordre precedement, on se souvient de cet objet pour lui donner de nouveaux ordres sans le resaisir
//reçoit des variables expliques precedemment
void Piece::souvenirObjetPrecedent(std::string& tampon, std::string& objetActuel, double& xCollision, double& yCollision, bool& showNames, bool& showAxes)
{
    std::vector<std::string> elements;
    std::string str;
    unsigned correspondance=0;
    bool validation = false;
    std::vector<int> objetsATester;

    std::istringstream iss(tampon);
    elements.push_back(objetActuel);

    while(std::getline(iss, str, ' '))
        elements.push_back(str);


    for(size_t i=0; i<m_mesObjets.size(); ++i)
    {
        for(unsigned j=0; j<objetActuel.size(); ++j)
        {
            if(elements[0][j]==m_mesObjets[i]->getID_I(j))          //on test si les ID des objets commencent comme l'objet saisie par l'utilisateur
            {
                correspondance++;
            }
            if(correspondance==objetActuel.size())
                validation = true;
        }
        if(validation)
        {
            m_mesObjets[i]->donnerOrdre(elements, m_mesObjets, xCollision, yCollision);     //si objet concerne, on lui donne l'ordre
            objetsATester.push_back(i);
            validation = false;
        }
        correspondance=0;
    }
    for(unsigned i=0; i<objetsATester.size(); ++i)
        m_mesObjets[objetsATester[i]]->testCollision(m_mesObjets, xCollision, yCollision, elements);      //on test les collisions des objets ayant ete deplace

    if(xCollision!=0&&yCollision!=0)
    {
        dessinerSVG(showAxes, showNames, xCollision, yCollision);
        std::this_thread::sleep_for(std::chrono::milliseconds(750));
        dessinerSVG(showAxes, showNames, xCollision, yCollision);
        xCollision = 0;
        yCollision = 0;
    }
    m_historique.ajouterUndo("@"+objetActuel+" "+tampon);
    m_historique.supprimerRedo();
}

//permet de charger un fichier.rom a tout moment
//reçoit la saisie utilisateur et la derniere ROM
bool Piece::loadFichier(std::string& tampon, std::string& dernierROM)
{
    std::vector<std::string> elements;
    std::string str;

    std::istringstream iss(tampon);
    while(std::getline(iss, str, ' '))
        elements.push_back(str);
    m_mesRooms.clear();                     //on nettoie les rooms et objets precedents
    m_mesObjets.clear();
    dernierROM = m_romActuelle;             //on sauvegarde le nom de l'ancienne rom
    m_romActuelle = elements[1];

    if(!m_mesRooms[0]->chargerData(m_mesObjets, m_mesRooms[0], elements[1]))    //on charge le nouveau fichier.rom
        return false;
    m_historique.ajouterUndo("load "+dernierROM);       //on ajoute l'action a l'historique
    m_historique.supprimerRedo();

    return true;
}


//permet de simuler un ctrl z -> annule la dernière action
//reçoit des paramètres présentés précédemment
void Piece::undoAction(std::string& objetActuel, std::string& dernierROM, double& xCollision, double& yCollision, bool& showAxes, bool& showNames)
{
    std::vector<std::string> elements;
    std::string str;

    if(m_historique.getSizeUndo()>0)
    {
        m_historique.copierUndoRedo();                                                      //le dernier undo devient le premier redo
        std::istringstream iss(m_historique.getUndoI(m_historique.getSizeUndo()-1));
        while(std::getline(iss, str, ' '))
            elements.push_back(str);
        if(elements[1]=="move")
        {
            elements[0].erase(elements[0].begin());                                         //on vient exercer sur l'objet l'action contraire a celle qui a été réalisé
            objetActuel = elements[0];
            if(elements[2][0]=='+')
            {
                elements[2].erase(elements[2].begin());
                elements[2].insert(0, "-");
            }
            else if(elements[2][0]=='-')
            {
                elements[2].erase(elements[2].begin());
                elements[2].insert(0, "+");
            }
            std::cout << elements[0] << std::endl;
            envoiDesOrdres(objetActuel, elements, xCollision, yCollision, m_mesObjets);
        }
        else if(m_historique.getUndoI(m_historique.getSizeUndo()-1)=="enable axes"||m_historique.getUndoI(m_historique.getSizeUndo()-1)=="disable axes")
        {
            if(m_historique.getUndoI(m_historique.getSizeUndo()-1)=="enable axes")
                showAxes = false;
            else
                showAxes = true;
        }
        else if(m_historique.getUndoI(m_historique.getSizeUndo()-1)=="enable names"||m_historique.getUndoI(m_historique.getSizeUndo()-1)=="disable names")
        {
            if(m_historique.getUndoI(m_historique.getSizeUndo()-1)=="enable names")
                showNames = false;
            else
                showNames = true;
        }
        else if(elements[0]=="load")    //si on avait load un fichier, reload l'ancien
        {
            m_mesRooms.clear();
            m_mesObjets.clear();
            dernierROM = m_romActuelle;
            m_romActuelle = elements[1];
            m_mesRooms[0]->chargerData(m_mesObjets, m_mesRooms[0], elements[1]);
        }

        m_historique.eraseDernierUndo();
        objetActuel="";
    }
}

//permet de simuler un ctrl y -> revient à la dernière action
//reçoit des paramètres présentés précédemment
void Piece::redoAction(std::string& objetActuel, double& xCollision, double& yCollision, bool& showAxes, bool& showNames, std::string& dernierROM)
{
    std::vector<std::string> elements;
    std::string str;

    if(m_historique.getSizeRedo()>0)
    {
        m_historique.copierRedoUndo();
        std::istringstream iss(m_historique.getRedoI(m_historique.getSizeRedo()-1));
        while(std::getline(iss, str, ' '))
            elements.push_back(str);
        if(elements[1]=="move")
        {
            elements[0].erase(elements[0].begin());                             //redonne le dernier ordre saisi
            objetActuel = elements[0];
            envoiDesOrdres(objetActuel, elements, xCollision, yCollision, m_mesObjets);
        }
        else if(m_historique.getRedoI(m_historique.getSizeRedo()-1)=="enable axes"||m_historique.getRedoI(m_historique.getSizeRedo()-1)=="disable axes")
        {
            if(m_historique.getRedoI(m_historique.getSizeRedo()-1)=="enable axes")
                showAxes = true;
            else
                showAxes = false;
        }
        else if(m_historique.getRedoI(m_historique.getSizeRedo()-1)=="enable names"||m_historique.getRedoI(m_historique.getSizeRedo()-1)=="disable names")
        {
            if(m_historique.getRedoI(m_historique.getSizeRedo()-1)=="enable names")
                showNames = true;
            else
                showNames = false;
        }
        else if(elements[0]=="load")
        {
            m_mesRooms.clear();
            m_mesObjets.clear();
            m_mesRooms[0]->chargerData(m_mesObjets, m_mesRooms[0], dernierROM);
            std::swap(dernierROM, m_romActuelle);
        }
        m_historique.eraseDernierRedo();
        objetActuel="";
    }
}

//charge un script donné
void Piece::loadScript(std::string& tampon, double& xCollision, double& yCollision, std::vector<Objet*>& mesObjets, bool& showAxes, bool& showNames)
{
    std::vector<std::string> elements;
    std::string str, tamponMenu;
    bool continuer = true;

    std::istringstream iss(tampon);
    while(std::getline(iss, str, ' '))
        elements.push_back(str);

    Script monScript;

    std::ifstream ifs{elements[1]};

    if(ifs)
    {
        monScript.chargerScript(ifs);

        do
        {
            system("cls");      //menu du script
            ConsoleExtension::SetConsoleTextColor(ConsoleExtension::ConsoleColor::Red);
            std::cout << "> script " << elements[1] << std::endl;
            ConsoleExtension::SetConsoleTextColor(ConsoleExtension::ConsoleColor::Default_Color);
            monScript.afficherConsole();
            std::cout << "\n\n> ";
            ConsoleExtension::SetConsoleTextColor(ConsoleExtension::ConsoleColor::Green);ConsoleExtension::SetConsoleTextColor(ConsoleExtension::ConsoleColor::Green);      //met la saisie utilisateur en vert
            std::getline(std::cin, tamponMenu);                                                                                                                                 //saisie utilisateur
            ConsoleExtension::SetConsoleTextColor(ConsoleExtension::ConsoleColor::Green);ConsoleExtension::SetConsoleTextColor(ConsoleExtension::ConsoleColor::Default_Color);

            if(tamponMenu=="quitter")
                continuer = false;
            else
                monScript.lancerOrdre(tamponMenu, xCollision, yCollision, mesObjets, showAxes, showNames, this);        //on lance les ordres
        }while(continuer);
    }
}

//menu principal
void Piece::menu()
{
    bool continuer = true, erreur = false;
    bool showAxes = false, showNames = false;
    std::string objetActuel = "";
    std::string tampon, str, mot = "load", mot2= "script", dernierROM;
    unsigned load = 0, script = 0;
    double xCollision = 0, yCollision = 0;

    do
    {
        system("cls");          //efface la console
        load = 0;
        script = 0;
        testChuteObjets();
        dessinerSVG(showAxes, showNames, xCollision, yCollision);           //dessine la scene
        std::this_thread::sleep_for(std::chrono::milliseconds(750));
        dessinerSVG(showAxes, showNames, xCollision, yCollision);
        ConsoleExtension::SetConsoleTextColor(ConsoleExtension::ConsoleColor::Default_Color);           //met la couleur console par default
        std::cout << "|------------------------------------------|\n|\t\tPossibilites :\t\t   |" << std::endl;
        std::cout << "|1. Move\t\t\t\t   |\n|2. Pousser\t\t\t\t   |\n|3. Attrapper\t\t\t\t   |\n|4. Enable/Disable axes\t\t\t   |\n|5. Enable/Disable names\t\t   |\n|6. Load/Reload fichier.rom\t\t   |\n|7. Undo/Redo\t\t\t\t   |\n|8. Store/Restore\t\t\t   |" << std::endl;
        std::cout << "|9. Scripts\t\t\t\t   |\n|10. Quitter\t\t\t\t   |\n|------------------------------------------|\n" << std::endl;     //affiche le menu
        if(erreur)
        {
            std::cout << "Fichier inexistant" << std::endl;
            erreur = false;
        }

        if(objetActuel=="")
            std::cout << "> ";
        else
            std::cout << objetActuel << "> ";
        ConsoleExtension::SetConsoleTextColor(ConsoleExtension::ConsoleColor::Green);ConsoleExtension::SetConsoleTextColor(ConsoleExtension::ConsoleColor::Green);      //met la saisie utilisateur en vert
        std::getline(std::cin, tampon);                                                                                                                                 //saisie utilisateur
        ConsoleExtension::SetConsoleTextColor(ConsoleExtension::ConsoleColor::Green);ConsoleExtension::SetConsoleTextColor(ConsoleExtension::ConsoleColor::Default_Color);

        for(unsigned i=0; i<4; ++i)
            if(tampon[i]==mot[i])
                load++;

        for(unsigned i=0; i<6; ++i)
            if(tampon[i]==mot2[i])
                script++;

        if(tampon[0]=='@')                                                                                              //exerce des actions en fonction de la saisie utilisateur
        {
            objetSelectionArobase(objetActuel, xCollision, yCollision, showAxes, showNames, tampon);
            testChuteObjets();
        }
        else if(tampon=="enable axes"||tampon=="disable axes")
        {
            if(tampon=="enable axes")
                showAxes = true;
            else
                showAxes = false;

            m_historique.ajouterUndo(tampon);
            m_historique.supprimerRedo();
        }
        else if(tampon=="enable names"||tampon=="disable names")
        {
            if(tampon=="enable names")
                showNames = true;
            else
                showNames = false;

            m_historique.ajouterUndo(tampon);
            m_historique.supprimerRedo();
        }
        else if(objetActuel!="")
        {
            if(tampon=="deselectionner")
            {
                objetActuel= "";
                continue;
            }
            souvenirObjetPrecedent(tampon, objetActuel, xCollision, yCollision, showNames, showAxes);
            testChuteObjets();
        }
        else if(load==4)
        {
            if(!loadFichier(tampon, dernierROM))
                erreur = true;
        }
        else if(tampon=="reload")
        {
            m_mesRooms.clear();
            m_mesObjets.clear();
            m_mesRooms[0]->chargerData(m_mesObjets, m_mesRooms[0], dernierROM);
            std::swap(dernierROM, m_romActuelle);

            m_historique.ajouterUndo(tampon);
            m_historique.supprimerRedo();

        }
        else if(tampon=="quitter")
        {
            for(unsigned i=0;i<(m_mesObjets.size())/2;i++)
                std::swap(m_mesObjets[i],m_mesObjets[m_mesObjets.size()-i-1]);
            std::ofstream sauvegarde("sauvegarde.rom", std::ios::app);
            m_mesRooms[0]->sauvegarder();
            sauvegarde << "[" << std::endl;
            m_mesObjets[0]->sauvegarder(0, m_mesObjets,sauvegarde);
            sauvegarde << std::endl << "]";
            for(unsigned i=0; i<m_mesObjets.size(); ++i)
                delete m_mesObjets[i];
            continuer = false;
        }
        else if(tampon=="afficher")
            afficher();
        else if(tampon=="undo"&&m_historique.getSizeUndo()>0)
        {
            undoAction(objetActuel, dernierROM, xCollision, yCollision, showAxes, showNames);
        }
        else if(tampon=="redo"&&m_historique.getSizeRedo()>0)
        {
            redoAction(objetActuel, xCollision, yCollision, showAxes, showNames, dernierROM);
        }
        else if(tampon=="store")
        {
            Piece* nouveau = new Piece(this);
            m_copie = nouveau;
        }
        else if(tampon=="restore")
        {
            m_mesObjets = m_copie->m_mesObjets;
            m_mesRooms = m_copie->m_mesRooms;
        }
        else if(script==6)
            loadScript(tampon, xCollision, yCollision, m_mesObjets, showAxes, showNames);
    }while(continuer);
}

//test si les objets de la piece ne repose plus sur leur parent et donc tombe = gravite
void Piece::testChuteObjets()
{
    unsigned numeroObjet, numeroObjetException;
    bool stop = false;
    double x1;
    Objet* pointeur;

    for(unsigned i=0; i<m_mesObjets.size();++i) //on test tous les objets
    {
        if((m_mesObjets[i]->getTLX()>m_mesObjets[i]->getTLXParent()+m_mesObjets[i]->getLongueurParent()||m_mesObjets[i]->getTLX()+m_mesObjets[i]->getLongueur()<m_mesObjets[i]->getTLXParent())&&m_mesObjets[i]->getEndposX()==-1)  //si l'objet ne repose plus sur son parent
        {
            m_mesObjets[i]->contact(m_mesObjets, numeroObjetException, NULL);

            while(!m_mesObjets[i]->contact(m_mesObjets, numeroObjet, m_mesObjets[numeroObjetException]))    //on test si il entre en contact avec un objet pendant sa chute
            {
                m_mesObjets[i]->setRefposY(m_mesObjets[i]->getRefposY()+0.1);
                if(m_mesObjets[i]->getTLY()>m_mesRooms[0]->getHauteur()+100)        //si il n'a pas rencontré d'objet, on le supprime
                {
                    stop = true;
                    break;
                }
            }

            if(stop)        //on supprime l'objet
            {
                pointeur = m_mesObjets[i];
                delete m_mesObjets[i];
                m_mesObjets.erase(m_mesObjets.begin()+i);

                for(unsigned k=0; k<m_mesObjets.size(); k++)
                {
                    for(unsigned j=0; j<m_mesObjets[k]->getNbrEnfants(); j++)
                    {
                        if(m_mesObjets[k]->getEnfant(j)==pointeur)
                            m_mesObjets[k]->getEnfants().erase(m_mesObjets[k]->getEnfants().begin()+j);
                    }
                }
            }
            else        //sinon le bloc sur lequel il tombe devient son parent
            {
                x1 = m_mesObjets[i]->getTLX();

                m_mesObjets[i]->setRefposX((x1-m_mesObjets[numeroObjet]->getTLX())/m_mesObjets[numeroObjet]->getLongueur());
                m_mesObjets[i]->setRefposY(0);
                m_mesObjets[i]->setBaseposX(0);
                m_mesObjets[i]->setBaseposY(1);
                m_mesObjets[i]->supprimerEnfantParent();
                m_mesObjets[i]->setParent(m_mesObjets[numeroObjet]);
                m_mesObjets[numeroObjet]->ajouter1Enfant(m_mesObjets[i]);

            }
        }
    }
}





