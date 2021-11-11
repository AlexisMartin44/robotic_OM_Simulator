#include "historique.h"


/*-----------------------------------------------------------CONSTRUCTEURS-----------------------------------------------------------*/
//Constructeur
Historique::Historique() : m_undo{}, m_redo{}
{
}

//Destructeur
Historique::~Historique()
{
}

/*-----------------------------------------------------------GETTERS-----------------------------------------------------------*/
//retourne le nombre d'actions dans le undo
unsigned Historique::getSizeUndo()
{
    return m_undo.size();
}

//retourne le nombre d'actions dans le redo
unsigned Historique::getSizeRedo()
{
    return m_redo.size();
}

//retourne l'action du undo numero i
std::string Historique::getUndoI(int i)
{
    return m_undo[i];
}

//retourne l'action du redo numero i
std::string Historique::getRedoI(int i)
{
    return m_redo[i];
}


/*-----------------------------------------------------------SSPRGS-----------------------------------------------------------*/

//ajoute le tampon au tableau de undo
void Historique::ajouterUndo(std::string tampon)
{
    m_undo.push_back(tampon);
}

//affiche l'action du undo numero i
void Historique::afficherUndoI(int i)
{
    std::cout << m_undo[i] << std::endl;
}

//supprime le dernier undo
void Historique::eraseDernierUndo()
{
    m_undo.erase(m_undo.begin()+m_undo.size()-1);
}

//supprime le dernier redo
void Historique::eraseDernierRedo()
{
    m_redo.erase(m_redo.begin()+m_redo.size()-1);
}

//copie le dernier undo comme dernier redo
void Historique::copierUndoRedo()
{
    m_redo.push_back(m_undo[m_undo.size()-1]);
}

//copie le dernier redo comme dernier undo
void Historique::copierRedoUndo()
{
    m_undo.push_back(m_redo[m_redo.size()-1]);
}

//supprime tous les redo
void Historique::supprimerRedo()
{
    m_redo.clear();
}
