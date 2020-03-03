#include <time.h>
#include <string.h>
#include "huff.h"



int main(int argc, char** argv)
{
	const char* delim = "-";
	if(argc!=3) {
		printf("Usage: ./huffman [-c/-d] name_file \n");
		exit(EXIT_FAILURE);
	}

	else {
		char *l = strtok(argv[1],delim);
		clock_t t1,t2;
		if(l[0]=='c') {
			t1 = clock();
			compress(argv[2]);
			t2 = clock();
			printf("Time: %f",(double) (t2-t1)/(double) CLOCKS_PER_SEC);
		} else if (l[0]=='d') {
			char *compress1 = malloc(sizeof(char)*strlen(argv[2])+1);
			strcpy(compress1,argv[2]);
			char* idk = malloc(sizeof(char)*(strlen(compress1)-4));
			idk = strtok(compress1,".");
			char* resultat = malloc(sizeof(char)*strlen(idk)+6);
			strcat(resultat,idk);
			strcat(resultat,"_.txt");
			decompression(argv[2],resultat);
		}
	}
	
	
	return 0;
	
}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Conseils pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
