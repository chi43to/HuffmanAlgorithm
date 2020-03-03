/********************************************************
* File : huff.c
* fichier source correspondant à l'en-tête précompilé ;
* nécessaire au bon déroulement de la compilation
* 
* Author : Julien RAMBAL, julien.rambal@etu.umontpellier.fr
*	         Nikola STOILOV, nikola.stoilov@etu.umontpellier.fr
********************************************************/

/// @file 


#include "huff.h"



size_t nb_liste = 0; // pour oral mais c'est pas recommandé 
size_t longeur_codage = 0;

unsigned isfeuille(Cell* macell);

unsigned int is_emptyL(Liste const * liste);

void tri_insertion(Liste* maliste)
{

	Cell* beg = maliste->bege, *tmp = NULL;


	for (Cell* parcour = maliste->bege; parcour->suiv_L; parcour = tmp) {

		beg = maliste->bege;
		tmp = parcour->suiv_L;

		if (beg->occurence >(parcour->suiv_L->occurence)) {
			
			location_append2(NULL, parcour, maliste);


			

		}

		else {
			
			
			for (; beg != parcour && (((beg->suiv_L->occurence) <= (parcour->suiv_L->occurence))); beg = beg->suiv_L);
			if (beg != parcour)
				location_append2(beg, parcour, maliste);
			

		}


	}

}


void location_append2(Cell * cell_loca, Cell * cell_a, Liste* maliste)
{

	Cell* tmp = NULL;
	if (cell_a->suiv_L != maliste->ende)tmp = (cell_a->suiv_L)->suiv_L;
	else maliste->ende = cell_a;
	if (cell_loca == NULL) {
		(cell_a->suiv_L)->suiv_L = maliste->bege;
		maliste->bege = cell_a->suiv_L;



	}
	else if (cell_loca->suiv_L) {


		cell_a->suiv_L->suiv_L = cell_loca->suiv_L;


		cell_loca->suiv_L = cell_a->suiv_L; 
		if (!(cell_loca->suiv_L)) {  
		maliste->ende = cell_a->suiv_L; }


	}

	cell_a->suiv_L = tmp;




}


//Opens file n_fichier with param flag.
FILE *open_fichier(const char* n_fichier, const char* param) 
	{
		FILE *fptr = fopen(n_fichier,param);
		
		//! If file not opened
		if (fptr==NULL)
		{
			printf("Error File: %s  do not exist or corrupt\n", n_fichier);

			exit(EXIT_FAILURE);
		}
		else { return fptr; }
}

//Create Binaire structure initialisé par défaut
Binaire * creaBinaire() {
	Binaire* monBinaire = NULL;
	if (!(monBinaire = malloc(sizeof(Binaire))))
	{

		
		printf("Description de l erreur :\n");
		exit(EXIT_FAILURE);

	}
	else
	{
		monBinaire->cara = (char)0;
		monBinaire->bin = 0;
		monBinaire->taille = 1;
		if (!(monBinaire->b_tab = (int*)malloc(1 * sizeof(int)))) {
			printf("ERREUR , pas assez de memoire \n");
			exit(EXIT_FAILURE);
		}
		return monBinaire;
		
	}
}

Liste * creaListe()
{
	Liste * maListe = NULL;

	if (!(maListe = malloc(sizeof(Liste))))
	{

		printf("Description de l erreur :\n");
		exit(EXIT_FAILURE);

	}
	else
	{
		maListe->bege = maListe->ende = NULL;
		return maListe;
	}
}


Cell * creaCell(const size_t oc, const unsigned int c)
{
	Cell * maCell = NULL;
	if (!(maCell = malloc(sizeof(Cell))))
	{
		printf("Description de l erreur : \n"); 
		exit(EXIT_FAILURE);




	}
	else {
		maCell->occurence = oc;
		maCell->val_c = c;
		maCell->suiv_L = NULL;
		maCell->pere_A = NULL;
		maCell->filsD_A = NULL;
		maCell->filsG_A = NULL;
		return maCell;

	}
}

Cell * creaNode(Cell * min1, Cell   * min2)
{
	Cell * maCell = NULL;
	if (!(maCell = malloc(sizeof(Cell))))
	{
		printf("Description de l erreur :\n");
		exit(EXIT_FAILURE);


	}
	else {
		//! Nouveau occurence = la somme des deux plus petites
		maCell->occurence = min1->occurence + min2->occurence;

		maCell->val_c = (isfeuille(min1) ? (isfeuille(min2) ? 2 : min2->val_c + 1) : (isfeuille(min2) ? min1->val_c + 1 : min1->val_c + min2->val_c));

		maCell->suiv_L = NULL;
		maCell->pere_A = NULL;

		maCell->filsG_A = min1;
		maCell->filsD_A = min2;
		
		return maCell;

	}
}
void default_append(Cell* c, Liste* maliste)
{
	//! Si liste est vide, le beg = end = c;
	if (is_emptyL(maliste)) {
		maliste->ende = c;
		maliste->bege = c;
	}
	else {
		//! Sinon on mets la cell C apres derniere position
		(maliste->ende->suiv_L) = c;
		maliste->ende = c;
	}
}


Cell* chercheL(Cell *C, Liste* L)
{
	Cell* beg = L->bege;

	for (; beg->suiv_L != NULL &&( (beg->suiv_L->occurence < C->occurence) || 
		((beg->suiv_L->occurence == C->occurence) && !isfeuille(beg->suiv_L)) || (isfeuille(beg->suiv_L) && (beg->suiv_L->val_c < C->val_c)));
		beg = beg->suiv_L);

	return beg;
}



void location_append(Cell * cell_loca, Cell * cell_a,Liste* maliste)
{
	if (cell_loca->suiv_L) {

		cell_a->suiv_L = cell_loca->suiv_L;

		//! Relie cell_loca au nouveau
		cell_loca->suiv_L = cell_a; 
	}
	else 
	{
		cell_loca->suiv_L = cell_a;
	}
	


}
void appendtri(Cell * cell_a,Cell* min2, Liste* liste_n)
{

	if (is_emptyL(liste_n))
	{
		min2->suiv_L = cell_a;
		cell_a->suiv_L = NULL;

		liste_n->bege = cell_a;
		liste_n->ende = cell_a;
	
	}
	else if(liste_n->bege->occurence > cell_a->occurence || ((liste_n->bege->occurence == cell_a->occurence) && ((isfeuille(liste_n->bege) || !isfeuille(liste_n->bege)) && (liste_n->bege->val_c > cell_a->val_c)))) {
		min2->suiv_L = cell_a;
		cell_a->suiv_L = liste_n->bege;
		liste_n->bege = cell_a;
	}
	else
	{
		location_append(chercheL(cell_a, liste_n), cell_a,liste_n);
	}

}

Liste* recupchar(FILE* fptr, FILE* trad) 
{

	fseek(fptr, 0, SEEK_SET);
	Liste* maliste = creaListe();
	size_t counter = 0;
	//! Le char qui va nous servir a remplir la liste
	char huff = getc(fptr); 
	//! La cell qu va nous permettre de parcourir la liste qui se fait remplir 
	Cell* parcour = maliste->bege; 
	//! on fait la boucle tant qu il reste des char dans le fichier
	while (huff != EOF) 
	{

		for (parcour = maliste->bege; parcour && huff != (parcour->val_c); parcour = parcour->suiv_L);
		
		if(parcour)++parcour->occurence;
		else{
			//! on cree une nouvel cell avec la valeur de huff et on l append dans la liste trie
			default_append(creaCell(1, huff), maliste);
			++counter;
		}
		++nb_liste;
		huff = getc(fptr);
	}
	fprintf(trad, "%zu ", counter);
	//! on return la liste complete 
	return maliste;

}


void creaArbre(Liste* maListe)
{
	Cell* min1 = NULL, *min2 = NULL, *node = NULL;
	//! Si la liste a plus d'un elem
	while ((maListe->bege) != (maListe->ende))
	{	//! min 1 et min 2 prennent les valeurs des premiers elem car la liste est trié 
		min1 = maListe->bege;
		min2 = min1->suiv_L;
		//! dans le cas 2 elem maListe->bege sera egal a NULL donc le isemptyliste va proc
		maListe->bege = min2->suiv_L; 


		//! creer le node
		node = creaNode(min1, min2); 
		//!  rattache le node a ses fils 
		min1->pere_A = min2->pere_A = node;

		
	//! rattache le node a la liste
	 appendtri(node,min2, maListe);
	}
	

}
void printArbre(Liste* maliste)
{
	printf("Root :%p Fils Gauche :%p Fils Droit :%p Probabilite :%f \n", (void*)maliste->bege, (void*)maliste->bege->filsG_A, (void*)maliste->bege->filsD_A,(float)maliste->bege->occurence/nb_liste);
	Cell* root = maliste->bege,*curr=root->filsG_A;
	if (isfeuille(curr)) {
		printf("Feuille :%p Pere :%p Caractere :%c Probabilite :%f\n", (void*)curr,(void*)curr->pere_A, curr->val_c, (float)curr->occurence / nb_liste);
		if (isfeuille(curr->suiv_L)) {
			printf("Feuille :%p Pere :%p Caractere :%c Probabilite :%f\n", (void*)curr->suiv_L,(void*)curr->suiv_L->pere_A, curr->suiv_L->val_c, (float)curr->suiv_L->occurence / nb_liste);
			return;
		}
		else {
			printf("Node :%p Fils Gauche :%p Fils Droit :%p Probabilite :%f\n", (void*)curr->suiv_L,(void*)curr->suiv_L->filsG_A, (void*)curr->suiv_L->filsD_A,(float)curr->suiv_L->occurence / nb_liste);
			curr = root->suiv_L;

		}
	}
	else {
		printf("Node :%p Fils Gauche :%p Fils Droit :%p Probabilite :%f\n", (void*)curr,(void*)curr->filsG_A,(void*)curr->filsD_A,(float)curr->occurence / nb_liste);
		if (isfeuille(curr->suiv_L)) printf("Feuille :%p cara :%c occurence :%f\n ",(void*) curr->suiv_L, curr->suiv_L->val_c, (float)curr->suiv_L->occurence / nb_liste);
		else printf("Node :%p Fils Gauche :%p Fils Droit :%p Probabilite :%f\n", (void*)curr->suiv_L, (void*)curr->suiv_L->filsG_A, (void*)curr->suiv_L->filsD_A, (float)curr->suiv_L->occurence / nb_liste);
	}
	printArbre2(root,curr);
	
}
void printArbre2(Cell* root,Cell*curr)
{
	if (curr != NULL){
		Cell* parcour = curr->filsG_A;
		
	if (isfeuille(parcour)) {
		for (; parcour != root->filsG_A && isfeuille(parcour) ; parcour = parcour->suiv_L)
		{
			printf("Feuille :%p Pere :%p Caractere :%c Probabilite :%f\n", (void*)parcour,(void*)(void*)parcour->pere_A, parcour->val_c,(float)parcour->occurence / nb_liste);
		}
		if (parcour == root->filsG_A)return;
	}
	
	Cell* tmp = parcour;
	for (; parcour != root->filsG_A; parcour = parcour->suiv_L)
	{
	
		if(isfeuille(parcour))printf("Feuille :%p Pere :%p Caractere :%c Probabilite :%f\n", (void*)parcour, (void*)parcour->pere_A, parcour->val_c, (float)parcour->occurence / nb_liste);
		else printf("Node :%p Fils Gauche :%p Fils Droit :%p Probabilite :%f\n", (void*)parcour,(void*)parcour->filsG_A, (void*)parcour->filsD_A, (float)parcour->occurence / nb_liste);
	}
	printArbre2(curr,tmp);
	}
}
void binary_char(Liste  * maListe, FILE* trad){
	if ((maListe->bege) != (maListe->ende)) { printf("Erreur vous ne pouvez pas appeler cette fonction sur une liste comptenant plus de 2 elem\n"); exit(EXIT_FAILURE); }
	else {
		parcourandbit2(maListe, trad);
	}

}

void parcourandbit2(Liste  * maListe, FILE*trad) {
	unsigned int p_bitG = 0;// a voir si on fait pas des alloc dynamique 
	unsigned int p_bitD = 0;
	size_t prof = 0;
	Cell* root = maListe->bege;
	parcourandbit(root, &p_bitG, &p_bitD, prof, trad);
	free(maListe);
	//printf("La liste : %p a ete delete\n", maListe);//gt
	maListe = NULL;
}


void parcourandbit(Cell  * maListe, unsigned int * p_bitG, unsigned int * p_bitD, size_t prof, FILE* trad)
{
	//! Si c un node <=> pas une feuille 
	if (!isfeuille(maListe->filsD_A)) { 

		fprintf(trad, "%d:", (maListe->filsD_A)->val_c);
		(setbitD(p_bitD));
		++prof;
		unsigned int p_bitG2 = *p_bitD;



		parcourandbit(maListe->filsD_A, &p_bitG2, p_bitD, prof, trad); 

	}
	//! si c une feuille
	else { 
		
		
		fprintf(trad, "%zu %c", ++prof,(char)maListe->filsD_A->val_c);
		setbitD(p_bitD);
		printf("Caractere :%c  codage binaire :", maListe->filsD_A->val_c);//pour oral
		print_bin_file(p_bitD ,prof, trad);
		printf("\n");//pour oral
		free(maListe->filsD_A);

		maListe->filsD_A = NULL;
		
	}
	if (!isfeuille(maListe->filsG_A)) {


		(setbitG(p_bitG));

		unsigned int p_bitD2 = *p_bitG;


		parcourandbit(maListe->filsG_A, &p_bitD2, p_bitG, prof, trad);
		free(maListe);
		maListe = NULL;
		
	}
	else {
		setbitG(p_bitG);
		fprintf(trad,"%zu %c",prof,(char)(maListe->filsG_A->val_c));
		printf("Caractere :%c  codage binaire :", maListe->filsG_A->val_c);//pour oral
		print_bin_file(p_bitG, prof, trad);
		printf("\n");//pour oral
		free(maListe->filsG_A);
		maListe->filsG_A = NULL;
	
		free(maListe);
		maListe = NULL;

	}



}

void print_bin_file(unsigned int const *binary, size_t prof, FILE* recup)// ecrit le binaire dans un fichier avec un espace devant 
{
	longeur_codage += prof; //pour oral
	/*char *str=malloc(prof+1) ;
	for (size_t i=0; i<prof; i++)
	{str[i] = (*binary & (1u <<(prof-i-1)) ? '1' : '0');
	}
	str[prof] = '\0';
	printf("%s\n", str);
	fprintf(recup, "%s ",str);*/

	for (size_t i = 0; i < prof; i++)
	{
		fprintf(recup, "%c", (*binary & (1u << (prof - i - 1)) ? '1' : '0'));
		printf("%c", (*binary & (1u << (prof - i - 1)) ? '1' : '0'));
	}
    
}


unsigned int**  creatabfromfile(FILE *trad) 
{
	fseek(trad, 0, SEEK_SET);//flush le trad
	size_t sz_row = 0;
	fscanf(trad, "%zu", &sz_row);
	printf("Longeur moyenne codage : %f\n", (float)longeur_codage / sz_row); //pour oral
	getc(trad);
	char recup = (char)0;
	unsigned int **tab = malloc(3 * sizeof(unsigned int)); // pour chaque caractere on creer une struct R et on la mets dans un tableau pour un acess + facile  remarque : marche aussi avec liste simplement chaine mais deja fait

	if (tab == NULL) {  // verif alloc bonne 
		printf("Description de l erreur :\n"); 
		exit(EXIT_FAILURE);
	}
	else {
		for (size_t i = 0; i < 3; i++) {
			tab[i] = malloc(sz_row * sizeof(unsigned int));

		}
	

		for (size_t i = 0; i < sz_row; i++)
		{
            
			fscanf(trad, "%u", &(tab[2][i]));
			while ((recup = getc(trad)) && (recup == ':')) {
				fscanf(trad, "%u", &(tab[2][i]));
				
			}
			tab[0][i] = getc(trad);
			for (size_t k = 0; k < tab[2][i] && (recup = getc(trad)) && recup != EOF; k++) {//&& recup!='\n'

				(((int)recup - 48) ? setbitD(&(tab[1][i])) : setbitG(&(tab[1][i])));
			}
			

		}

	}
	return tab;

}



Binaire** creatabfromfileB(FILE *trad, size_t sz_row) // sz row = nb_liste
{
	char recup = (char)0;
	Binaire** tab = malloc(sz_row * sizeof(Binaire*)); // pour chaque caractere on creer une struct R et on la mets dans un tableau pour un acess + facile  remarque : marche aussi avec liste simplement chaine mais deja fait

	if (tab == NULL) {  // verif alloc bonne 
		printf("Description de l erreur :\n"); // faudrait  longjump vers un switch avec une id pour code
		exit(EXIT_FAILURE);
	}
	else {
	
		size_t j = 0, lol = 0, counter = 0;
		// consome le saut de ligne volontaire getc(fptr);
		for (size_t i = 0; i < sz_row; i++)
		{
			tab[i] = creaBinaire();
			j = 0;
			counter = 0;
			fscanf(trad, "%zu", &(lol));
			while ((recup = getc(trad)) && (recup == ':')) {
				++counter;
				if (counter >= tab[i]->taille) {
					if (!(tab[i]->b_tab = (int*)realloc(tab[i]->b_tab, (2 * counter) * sizeof(int)))) {
						printf("ERREUR , pas assez de memoire \n");
						exit(EXIT_FAILURE);
					}
					else
					{
						tab[i]->taille = 2 * counter;
					}
				}
				tab[i]->b_tab[j++] = lol;
				fscanf(trad, "%zu", &(lol));

			}
			tab[i]->b_tab[j++] = 1;
			for (; j < tab[i]->taille; j++)tab[i]->b_tab[j] = 0;
			
			tab[i]->taille = lol;
			tab[i]->cara = getc(trad);
			for (size_t k = 0; k < tab[i]->taille && (recup = getc(trad)) && recup != EOF; k++) {//&& recup!='\n'

				(((int)recup - 48) ? setbitD(&(tab[i]->bin)) : setbitG(&(tab[i]->bin)));
			}
		
			
			

		}

	}
	return tab;
}

void setbitD(unsigned int *bit);

void setbitG(unsigned int * bit);


int bitwise(int tab, char mychar, size_t taille, size_t taille_tab);
	

void compress(const char* n_compress)
{
	char *compress = malloc(sizeof(char)*strlen(n_compress)+1);
	strcpy(compress,n_compress);
	const char* delim = ".";
	char* idk = malloc(sizeof(compress)-4);
	idk = strtok(compress,delim);
	char* result = malloc(sizeof(char)*strlen(idk)+6);
	strcat(result,idk);
	strcat(result,".hman");

	//! ouvre le fichier en mode ecriture + lecture et écrase son contenu
	FILE* fptr = open_fichier(n_compress, "a+"), *comp = open_fichier(result, "w+");

	//!creer liste feuille
	Liste* maliste = recupchar(fptr, comp);
	tri_insertion(maliste);

	
    creaArbre(maliste);
	printArbre(maliste); //oral
	

	//! Creer fichier de trad
	binary_char(maliste, comp);
	
	
	char mychar = '\0';
	size_t i = 0;

	//! Creation d'un tab
	unsigned int** tab = creatabfromfile(comp); 
	
	fseek(fptr, 0, SEEK_SET);

	//! Recupere les char du fichier non compresse
	while (!(feof(fptr))) 
	{


		while (fscanf(fptr, "%c", &mychar) != EOF)
		{
			i = 0;
			while (mychar != (char)tab[0][i]) {
				
				i++; 
			}

			for (int k = tab[2][i] - 1; k >= 0; k--)
			{
				fprintf(comp, "%c", (tab[1][i] & (1u << k) ? '1' : '0'));

			}



		}



	}

	

	for (size_t k = 0; k < 3; k++)
	{

		free(tab[k]);
		tab[k] = NULL;
	}
	free(tab);
	tab = NULL;

	fclose(comp);
	comp = NULL;
	fclose(fptr);
	fptr = NULL;



} 


void decompression(const char * compress,const char* decompress)
{
	FILE *comp = open_fichier(compress, "r"), *result = open_fichier(decompress, "w");
	size_t sz_row = 0;
	fscanf(comp, "%zu", &sz_row);
	getc(comp);
	Binaire** tab = creatabfromfileB(comp, sz_row);
	size_t i = 0;
	//unsigned int binaire = 0;
	size_t taille_bin = 0;
	char mychar = (char)0;
	size_t k = 0;
	
	
	while (!(feof(comp))) //recupere les char du fichier non compresse
	{


		//binaire = 0;
		i = 0;
		taille_bin = 0;
		k = 0;
		while ((tab[i]->taille != taille_bin )&&(mychar = getc(comp)) && mychar != EOF )
		{


			++taille_bin;
			
			if(!(bitwise(tab[i]->bin, mychar, taille_bin, tab[i]->taille)))
			{
				
			    i += tab[i]->b_tab[k];
				
				k = 0;
			}else k++;

		}
		//! Sinon on continue la comparaison  avec binaire avec 1 bit en plus
		if (taille_bin > 0) {
			fprintf(result, "%c", tab[i]->cara);
		}


	}
	for (size_t k = 0; k < sz_row; k++)
	{

		free((tab[k]->b_tab));
		tab[k]->b_tab = NULL;
		free(tab[k]);
		tab[k] = NULL;


	}
	free(tab);
	tab = NULL;
	//! ferme les flux :
	fclose(comp);
	comp = NULL;
	fclose(result);
	result = NULL;
}
