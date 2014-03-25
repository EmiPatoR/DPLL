/*******************************************************
Nom ......... : main.c
Role ........ : Main
Auteur(s) ...... : HRUSTIC Emir, BOUAZIZ Mehdi
Version ..... : V0.1 du 15/03/2014
Licence ..... : GPL

Compilation :
makefile, make clean
Pour exécuter, tapez : ./bin/DPLL
********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "../head/const.h"
#include "../head/structures.h"
#include "../head/fonctions.h"
#include "../head/parser.h"


int main(int argc, char* argv[]){
    Status st;
    if(LOGS_ACTIVE){
        printf("Logs actif.\n");
        Create_log();
    }

    // tests();

    gestion_erreur((st = args(argc,argv)));
    if(st == OK){

        if(LOGS_ACTIVE){
            fprintf(recup_fichier_log(),"Fichier passe en parametre detecte.\nNom de fichier passe en parametre : %s\n",argv[1]);
        }
        printf ("Fichier selectione : %s\n",argv[1]);
        if(argv[1][0] == '/'){
            if(LOGS_ACTIVE)
                fprintf(recup_fichier_log(),"Chemin absolu detecte. Recherche du fichier %s...\n", argv[1]);
                gestion_erreur(existance_fichier(argv[1],PATH_ABSOLUTE));
        }
        else{
            if(LOGS_ACTIVE)
                fprintf(recup_fichier_log(),"Pas de chemin absolu, recherche du fichier %s dans le repertoire files...\n",argv[1]);
                gestion_erreur(existance_fichier(argv[1],PATH_RELATIVE));
        }
    }

       printf("\n\n------------------DEBUT TESTS-----------------------\n\n");
    /*
    --- Test du Parser ---
        a) Ouverture d'un fichier saisi en ligne de commande
        b) Parser le fichier dans les structures de données
        c) Afficher les structures
    */

    // a)  /!\ Lancer avec le fichier "test.data"
    FILE *fichier = recup_fichierAct();

    // b)
    liste liste_cl2lt;
    liste liste_lt2cl_pos;
    liste liste_lt2cl_neg;

    parser_depuis_fichier(fichier,
                          &liste_cl2lt,
                          &liste_lt2cl_pos,
                          &liste_lt2cl_neg);

    graphe_symetrique(liste_cl2lt, &liste_lt2cl_pos, &liste_lt2cl_neg);

    // c)
    printf("Clauses -> Litteraux \n");
    printf("Nombre de Litteraux : %i\nNombre de Clauses : %i\n",liste_cl2lt.nLitteraux,liste_cl2lt.nClauses);
    display_structure(liste_cl2lt);

    printf("Litteraux -> Clauses - POS \n");
    printf("Nombre de Litteraux : %i\nNombre de Clauses : %i\n",liste_lt2cl_pos.nLitteraux,liste_lt2cl_pos.nClauses);
    display_structure(liste_lt2cl_pos);

    printf("Litteraux -> Clauses - NEG \n");
    printf("Nombre de Litteraux : %i\nNombre de Clauses : %i\n",liste_lt2cl_neg.nLitteraux,liste_lt2cl_neg.nClauses);
    display_structure(liste_lt2cl_neg);

    printf("\n\n------------------FIN TESTS-----------------------\n\n");

    return EXIT_SUCCESS;
}

