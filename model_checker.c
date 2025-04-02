#include "kripke.h"
#include "parseTree.h"
#include "model_checker.h"
#include "Set.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_STATES 100 

HashSet stateSatisfies[MAX_STATES];

HashSet* HandleEU(KripkeStructure* ks, treeNode* formula){
	HashSet* sformula;
	initSet(sformula);
	StateNode* states = ks->states;
	while (states){
		
		if(checkSat(formula->right, states->s))
			{add(sformula, states->s);
			addCTL( formula, states->s);}
	}
	states = ks->states;
	while (states){
		if(checkSat(formula, states->s)){
			continue;
		}
		for (int i = 0; i < states->s->num_adj; i++){
			state* neighbour =  states->s->adj[i];
			if (checkSat(formula->left, neighbour)){
				int flag = 0;
				for (int j = 0; j <neighbour->num_adj; j++ ){
					if(checkSat(formula, neighbour->adj[j]))
					{	flag = 1;
						break;
					}
				}
				if (flag == 1){
					add(sformula, );
				}
			}
		}
	}







}

