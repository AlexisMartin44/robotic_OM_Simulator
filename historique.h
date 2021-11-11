#ifndef HISTORIQUE_H_INCLUDED
#define HISTORIQUE_H_INCLUDED

#include <iostream>
#include <vector>

class Historique
{
private:
    std::vector<std::string> m_undo;
    std::vector<std::string> m_redo;
public:
/*-----------------------------------------------------------CONSTRUCTEURS-----------------------------------------------------------*/
    Historique();
    ~Historique();
/*-----------------------------------------------------------GETTERS-----------------------------------------------------------*/
    unsigned getSizeUndo();
    unsigned getSizeRedo();
    std::string getUndoI(int i);
    std::string getRedoI(int i);
/*-----------------------------------------------------------SSPRGS-----------------------------------------------------------*/
    void afficherUndoI(int i);
    void ajouterUndo(std::string tampon);
    void eraseDernierUndo();
    void eraseDernierRedo();
    void copierUndoRedo();
    void copierRedoUndo();
    void supprimerRedo();
};


#endif // HISTORIQUE_H_INCLUDED
