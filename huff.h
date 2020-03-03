/********************************************************
* File : huff.h
* Author : Julien RAMBAL, julien.rambal@etu.umontpellier.fr
*	         Nikola STOILOV, nikola.stoilov@etu.umontpellier.fr
********************************************************/


#ifndef PCH_H
#define PCH_H
#include<stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>





//! structure cellule destiné à mimer un noeud/feuille de l'arbre 
typedef struct C
{
	
	size_t occurence;
	//! represantation en int du charactere pour une feuille ou le nombre de feuille affilié à un noeud 
	unsigned int val_c;
	//!Next element in list
	struct C* suiv_L;


	//!pere  arbre
	struct C * pere_A;
	//! Fils Droit:
	struct C *filsD_A;
	//! Fils Gauche:
	struct C *filsG_A;


}Cell;


//!Defines structure Liste
typedef struct L
{
	//!Beggining of list : premier element 
	Cell* bege;
	//!End of list dernier element ende->suiv_L==NULL
	Cell* ende;


}Liste;


//! Defines structure Binaire

typedef struct B
{
	//!character associer au codage binaire
	char cara;
	//!binary representation
	unsigned int bin;
	//! taille  du codage
	size_t taille;
	//!tableau representant la place de "l'arbre mirroir" au k ème bit 
	int* b_tab;

}Binaire;



//!test si macell est une feuille 
inline unsigned isfeuille(Cell* macell){return (macell->filsD_A || macell->filsG_A ? 0 : 1);}

//! si liste == listevide alors return 1 else 0
inline unsigned int is_emptyL(Liste const * liste){return (liste->bege == NULL);}

//! Implementation de InsertionSort algorithm
void tri_insertion(Liste*);

void location_append2(Cell*, Cell*, Liste*);

//! Create Binaire structure initialisé par défaut 
Binaire * creaBinaire();


//! open le file avec minimum securite
FILE *open_fichier(const char* n_fichier, const char* param);


//! Creer une liste vide , creation par défaut 
Liste * creaListe();

//! on cree une nouvelle cell avec oc et c 
Cell * creaCell(const size_t oc , const unsigned int c);


//!creer un noeud avec les 2 fils connu et on actualise son nombre de feuille 
Cell * creaNode(Cell  * min1, Cell  * min2);


//! Appends la cell c at end of liste
void default_append(Cell* c, Liste* maliste);


//! return l'antecedent de C->occurence dans la liste
Cell* chercheL(Cell *C, Liste* L);


//!ajoute cell_a apres cell_loca 
void location_append(Cell * cell_loca, Cell * cell_a,Liste*);


//! Ajoute cell_a de maniere trier dans la liste si 2 noeud on le même poids on les compare grace à leur profondeur rapport : poids/profondeur
void appendtri(Cell * cell_a, Cell* min2, Liste* liste_n);

	
//! Creer une liste de cell non trié et écrit le nombre different de char lu  dans un fichier et un espace
Liste* recupchar(FILE *,FILE*);


//!Creer arbre avec les elements de la  liste sans briser les liens de la liste 
void creaArbre(Liste* maListe);


//! Fonction parent 
void printArbre(Liste* root);

//!fonction fille : utilise les liens de la liste pour écrire l'arbre par étage ( parcour en longueur ) en partant de l'élément le plus petit == fils gauche 
//! Si le fils_gauche est une feuille alors nous cherchons le premier noeud de la ligne dont fait parti le fils_G
//! L'algorithme s'arrête quand la ligne ne contient plus que des feuilles 
void printArbre2(Cell* maliste, Cell*);


//!fonction securite (inutile)
void binary_char(Liste  * maListe, FILE*);

// !fonction parent : permet de lancer la premiere iteration de la fonction fille grace à des parametre initialisé par defaut
void parcourandbit2(Liste * maListe, FILE*);


//! parcoure l'arbre  recursivement et ecrit en partant de l'element le plus puissant(fils_D) qui est vraisembablement le moins profond
//!optimise la compression : les caracteres les plus frequent etant les plus puissant ils sont marqués en premier 
void parcourandbit(Cell  * maListe, unsigned int * p_bitG, unsigned int * p_bitD, size_t, FILE*);



//! se declenche si le binaire est associer a une feuille 
//!ecrit le binaire dans un fichier après le caractere qui lui est associer 
void print_bin_file(unsigned int const *binary, size_t, FILE*);


//! creer un tab qui contient : 
//!0 : les caractere dans la premiere ligne 
//!1 : le codage en binaire du caractere dans la seconde ligne
//!2 : la taille du codage dans la troisieme ligne 
//! Remarque : le nombre de feuille des noeuds parents n'étant pas utiliser dans cette étape ils sont  volontairement oublié
unsigned int** creatabfromfile(FILE * traducteur);



//!Même procéder que pour l'autre tableau mais pour la decompression cette fois ci 
//!0 : les caractere dans la premiere ligne 
//!1 : le codage en binaire du caractere dans la seconde ligne
//!2 : la taille du codage dans la troisieme ligne 
//!3 : tableau d'int contenant les nb_feuilles des nodes parents 
Binaire** creatabfromfileB(FILE *trad, size_t);



//setbitD,setbitG - fonctions pour lire l'arbre
inline void setbitG(unsigned int *bit) { *bit = (*bit) << 1; }

inline void setbitD(unsigned int * bit) { *bit = (*bit) << 1; (*bit) |= 1; }


// compare le taille_tab bit de tab avec mychar 
inline int bitwise(int tab, char mychar, size_t taille, size_t taille_tab){
	return((tab & (1u << (taille_tab - taille))) ? (((int)mychar - 48) ? 1 : 0) : (((int)mychar - 48) ? 0 : 1));
}



//! En tête : ex : nb_char_liste |2:4:|5| |caractere|codage_binaire| .... 
//!                                1   2     
//! 1- le premier champ servira pour gagner de la performance l'ors de la decompression en accédant directement au binaire correspondant : "aux k ème caracteres dèjà lu "
//! 2- le second champ est la taille de l'encodage

//! Compression :
//! fichier compresser grâce a une recherche lineaire des caracteres  

//! Compresses le file n_fichier et comme output -> n_compress.hman
void compress(const char* n_compress);
//! fichier compresser grâce a une recherche lineaire des caracteres 




//Decompress le fichier nom.hman en nom_.txt
//! chaque bit étant associé à un element de l'arbre il comporte une adresse symboliser par son codage
//! donc si le n ème caractere lu n'est pas valide avec la n ème place du binaire étudier nous pouvons grâce a la troisième ligne du tableau de binaire accéder à l'adresse correcte
//! pour ce fait nous incrémentons un compteur qui compte le nombre de place valide et si le bit n'est plus correcte nous récuperons le tab[i][compteur] pour accéder a l'encodage correspondant 
void decompression(const char* nom, const char* compresse);


#endif //PCH_H

