#include <stdio.h>
#include <stdlib.h>
#include "../head/const.h"
#include "../head/structures.h"
#include "../head/fonctions.h"
#include "../head/parser.h"
#include "../head/../head/dpll.h"

Status dpll(liste cl2lt, liste lt2cl_pos , liste lt2cl_neg){
    int i,j,temp;
    int nbChoix = lt2cl_pos.nLitteraux;
    int choix = 1;
    int ordre_elt = 1;
    int *tabChoix;
    int *eltATraiter;
    int *elt_a_choisir;
    cellule *cell = NULL;
    Pile *historique = NULL;
    liste corbeille,temporaire,tmp_pos,tmp_neg;


    if(lt2cl_pos.structure == TYPE_STRUCT_CL2LT)
        return ERREUR_TYPE;

    if(lt2cl_neg.structure == TYPE_STRUCT_CL2LT)
        return ERREUR_TYPE;
    elt_a_choisir = (int*) malloc(sizeof(int)*lt2cl_pos.nLitteraux*2);
    tabChoix = (int*) malloc(sizeof(int)*lt2cl_pos.nLitteraux*2);
    eltATraiter = (int*) malloc(sizeof(int)*lt2cl_pos.nLitteraux*2);


    if(elt_a_choisir == NULL)
        return ERREUR_ALLOC;

    if(tabChoix == NULL)
        return ERREUR_ALLOC;

    if(eltATraiter == NULL)
        return ERREUR_ALLOC;

    for(i=0;i<lt2cl_pos.nLitteraux*2;i++){
        tabChoix[i] = 0;
        elt_a_choisir[i] = 0;
        eltATraiter[i] = 1;
    }


    while(!estVide(eltATraiter,lt2cl_pos.nLitteraux*2)){
        if( ((cell = find_mono_litteral(cl2lt)) != NULL) || ( ((cell = find_pure_litteral(cl2lt,lt2cl_pos,lt2cl_neg)) !=NULL)) ){ // S'il y a un mono litteral
            if(tabChoix[cell->val -1] == 0)
                temp = cell->val;
            else if(tabChoix[cell->val -1] == 1)
                temp = -cell->val;

            init_structures(cl2lt.nClauses,&corbeille,TYPE_STRUCT_CL2LT);
            if(cell->val > 0)
                choix = temp;
            else
                choix = (lt2cl_pos.nLitteraux - 1) + (-temp);

            elt_a_choisir[choix - 1] = ordre_elt;
            tabChoix[choix -1] = 1;
            eltATraiter[choix -1] = 0;
            ordre_elt++;

            printf ("Litteral choisi : %i\n",temp);
            for(i=0;i<cl2lt.nClauses;i++){

                if(element_exists_n(cl2lt,i,temp)){ // Si le monoliteral figure dans une clause, on supprime la clause
                    add_list_i(cl2lt,&corbeille,i);
                    del_list_i(&cl2lt,i); // on le supprime de clause -> litteraux

                    for(j=0;j<lt2cl_pos.nLitteraux;j++){
                        if(element_exists_n(lt2cl_pos,j,i))
                            del_list_element_by_value(&lt2cl_pos,j,i);
                        if(element_exists_n(lt2cl_neg,j,i))
                            del_list_element_by_value(&lt2cl_neg,j,i);
                    }

                }

                if(element_exists_n(cl2lt,i,(-temp))){// Si son opposé figure dans une clause, on le supprime
                    add_list_element_head(&corbeille,TYPE_ELEMENT_LT,i,(-temp));
                    del_list_element_by_value(&cl2lt,i,(-temp));
                    if(temp > 0)
                        del_list_element_by_value(&lt2cl_neg,temp-1,i);
                    else
                        del_list_element_by_value(&lt2cl_pos,(-temp)-1,i);
                }
            }
            display_structure(cl2lt);
            display_structure(lt2cl_neg);
            display_structure(lt2cl_pos);
            nbChoix--;
            pile_push(&historique,corbeille);
        }
        else{ // Si pas de mono litteral et pas de litteral pur
            if( (is_liste_vide(cl2lt)) && (nbChoix > 0) ){
                printf("Pas de solution, on backtrack ! \n");
                nbChoix++;
                choix = max_tab(elt_a_choisir,lt2cl_pos.nLitteraux*2);
                printf("Litteral precedent : %i\n",choix+1);

                temporaire = pile_pop(&historique);

                for(i=0;i<temporaire.nClauses;i++){
                    for(j=0;j<temporaire.nEltPerList[i];j++){
                             cell = select_list_element(temporaire,i,j);
                             add_list_element_head(&cl2lt,TYPE_ELEMENT_LT,i,cell->val);
                    }
                }

                init_structures(lt2cl_pos.nLitteraux,&tmp_pos,TYPE_STRUCT_LT2CL);
                init_structures(lt2cl_neg.nLitteraux,&tmp_neg,TYPE_STRUCT_LT2CL);
                graphe_symetrique(temporaire,&tmp_pos,&tmp_neg);

                for(i=0;i<tmp_pos.nLitteraux;i++){
                    for(j=0;j<tmp_pos.nEltPerList[i];j++){
                             cell = select_list_element(tmp_pos,i,j);
                             add_list_element_head(&lt2cl_pos,TYPE_ELEMENT_CL,i,cell->val);
                    }
                }

                for(i=0;i<tmp_neg.nLitteraux;i++){
                    for(j=0;j<tmp_neg.nEltPerList[i];j++){
                             cell = select_list_element(tmp_neg,i,j);
                             add_list_element_head(&lt2cl_neg,TYPE_ELEMENT_CL,i,cell->val);
                    }
                }

                display_structure(cl2lt);
                display_structure(lt2cl_pos);
                display_structure(lt2cl_neg);

                return OK;
            }
        }
        printf("--- --- -- -- - - - \n");
    }

    printf("Surprise MOFO !\n");
    return OK;
}

int max_tab(int *tab,int taille){
    int i;
    int maxi = 0;
    for(i=0;i<taille;i++){
        if(tab[i] > tab[maxi])
            maxi = i;
    }
    return maxi;
}

int estVide(int *tab, int taille){
    int i;
    for(i=0;i<taille;i++){
        if(tab[i] != 0)
            return 0;
    }
    return 1;
}
