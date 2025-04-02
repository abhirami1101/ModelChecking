#include "kripke.h"
#include "parseTree.h"
#include "model_checker.h"
#include "Set.h"
#include <sys/queue.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_STATES 100 


void checkProp(KripkeStructure* ks, treeNode* node){
	StateNode* states = ks->states;
	if (node->prop_var == 'T'){
		while(states){
			state* State = states->s;
			add(node->sat, State);		
			states = states->next;
		}
		return;
	}
	if (node->prop_var == 'F'){
		return;
	}
	while(states){
		state* State = states->s;
		label* label = State->labelset;
		while (label){
			if (label->prop_var == node->prop_var){
				add(node->sat, State);
				break;
			}
			label = label->next;
		}
		states = states->next;

	}
}

void checkAnd(KripkeStructure* ks, treeNode* node){
	StateNode* states = ks->states;
	while(states){
		state* State = states->s;
		bool flag1 = contains(node->left->sat,State);
		bool flag2 = contains(node->right->sat,State);
		if (flag1 && flag2){
			add(node->sat, State);
		}
		states = states->next;

	}
}

void checkNot(KripkeStructure* ks, treeNode* node){
	StateNode* states = ks->states;
	while(states){
		state* State = states->s;
		bool flag1 = contains(node->right->sat,State);
		if (!flag1){
			add(node->sat, State);
		}
		states = states->next;

	}
}









