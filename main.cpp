#include <iostream>
#include "src/svg/svgfile.h"
#include "piece.h"


/*---------------------------------------------------------------------------------------------*/
//Code enti�rement r�alis� par Alexis Martin, Zo� Crouzet et No�mie Devergne
//Nous nous sommes bas� sur le chargement de M.Segado pour charger nos objets
//Nous avons aussi utilis� une fonction std::stoi qui transforme un string en entier trouv�e sur internet
//De plus, nous avons utilis� une fonction de mise en pause du syst�me trouv�e sur internet
/*---------------------------------------------------------------------------------------------*/

int main()
{
    Piece piece;
    piece.chargerScene();
    piece.menu();
    return 0;
}
