/*******************************************************
Nom ......... : structures.c
Role ........ : Definition des fonctions pour gerer
                les structures de donnees
Auteur(s) ...... : HRUSTIC Emir, BOUAZIZ Mehdi
Version ..... : V0.1 du 15/03/2014
Licence ..... : GPL

Compilation :
makefile, make clean
Pour exécuter, tapez : ./bin/DPLL
********************************************************/

#include "../head/structures.h"
#include "../head/const.h"

#include <stdlib.h>
#include <stdio.h>


/**
*   Fonction d'initialisation d'une structure du type literaux > clauses
*   @param nLit taille du tableau a allouer (allocation dynamique)
*   @param init_list pointeur sur la structure à initialiser
*   @param type Correspond au type de structure de donnees : structure = TYPE_STRUCT_LT2CL -> litteraux > Clauses
*                                                            structure = TYPE_STRUCT_CL2LT -> Clauses > Litteraux
*   @return renvoie un status , OK si tout s'est bien deroule sinon une ERREUR definie dans const.h
*/

Status init_structures(int n, liste *init_list, Type_struct structure){
        int i;
        /* Initialisation des variables de la structure */

        /*Initialisation pour la structure Litteraux > Clauses */

        if(structure == TYPE_STRUCT_LT2CL){
            init_list->nLitteraux = n;
            init_list->nClauses = 0;
            init_list->structure = TYPE_STRUCT_LT2CL;
        }
        /* Initialisation pour la stucture Clauses > Litteraux */
        else{
            init_list->nClauses = n;
            init_list->nLitteraux = 0;
            init_list->structure = TYPE_STRUCT_CL2LT;
        }

        /* Allocations dynamiques */

        init_list->l = (liste_chainee*)malloc(sizeof(liste_chainee)*n);
        init_list->last = (cellule**)malloc(sizeof(cellule*)*n);
        init_list->nEltPerList = (int*)malloc(sizeof(int)*n);
        if(init_list->l == NULL || init_list->last == NULL || init_list->nEltPerList == NULL)
            return ERREUR_ALLOC;

        /*initialisation de chaque premier element des liste chainees a NULL */

        for(i=0;i<n;i++){
            init_list->l[i] = NULL;
            init_list->last[i] = NULL;
            init_list->nEltPerList[i] = 0;
        }

        return OK;
}

/**
*   Fonction d'ajout d'elements en debut de liste chainnee
*   @param linked_list pointeur sur la strtucutrede donnees
*   @param element type d'elements a ajouter : Clause = TYPE_ELEMENT_CL / Litteral = TYPE_ELEMENT_LT
*   @param n Correspond a l'indice du tableau de liste chainee auquel ajouter l'element
*   @return renvoie un status , OK si tout s'est bien deroule sinon une ERREUR definie dans const.h dans eval Status
*/

Status add_list_element_head(liste *linked_list,Type_elt element, int n, int value){
    cellule *cell;

    /* Verifications preliminaires */

    if(linked_list->structure == TYPE_STRUCT_CL2LT){
        if(element != TYPE_ELEMENT_LT)
            return ERREUR_TYPE;
        if(n > (linked_list->nClauses - 1))
            return ERREUR_DEPASSEMENT_MEMOIRE;
    }
    else{
        if(element != TYPE_ELEMENT_CL)
            return ERREUR_TYPE;
        if(n > (linked_list->nClauses - 1))
            return ERREUR_DEPASSEMENT_MEMOIRE;
    }

    /*Creation d'un nouvel element */
        cell = (cellule*)malloc(sizeof(cellule));
        if(cell == NULL)
            return ERREUR_ALLOC;
        cell->val = value;
        cell->next = linked_list->l[n];
        cell->prev = NULL;
        cell->element = element;
        linked_list->l[n] = cell;
        linked_list->nEltPerList[n]++;

    return OK;
}

/**
*   Fonction d'ajout d'elements en fin de liste chainnee
*   @param linked_list pointeur sur la strtucutrede donnees
*   @param element type d'elements a ajouter : Clause = TYPE_ELEMENT_CL / Litteral = TYPE_ELEMENT_LT
*   @param n Correspond a l'indice du tableau de liste chainee auquel ajouter l'element
*   @return renvoie un status , OK si tout s'est bien deroule sinon une ERREUR definie dans const.h dans eval Status
*/


Status add_list_element_tail(liste *linked_list,Type_elt element, int n, int value){
    cellule *cell;

    /* Verifications preliminaires */

    if(linked_list->structure == TYPE_STRUCT_CL2LT){
        if(element != TYPE_ELEMENT_LT)
            return ERREUR_TYPE;
        if(n > (linked_list->nClauses - 1))
            return ERREUR_DEPASSEMENT_MEMOIRE;
    }
    else{
        if(element != TYPE_ELEMENT_CL)
            return ERREUR_TYPE;
        if(n > (linked_list->nClauses - 1))
            return ERREUR_DEPASSEMENT_MEMOIRE;
    }

    /*Creation d'un nouvel element */
        cell = (cellule*)malloc(sizeof(cellule));
        if(cell == NULL)
            return ERREUR_ALLOC;
        cell->val = value;
        cell->next = NULL;
        cell->prev = linked_list->last[n];
        linked_list->last[n]->next = cell;
        linked_list->last[n] = cell;
        cell->element = element;
        linked_list->nEltPerList[n]++;
    return OK;
}
