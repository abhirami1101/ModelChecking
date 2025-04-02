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

void checkEX(KripkeStructure* ks, treeNode* node){
	StateNode* states = ks->states;
	while(states){
		state* State = states->s;
		state** adj = State->adj;
		int num = State->num_adj;
		int flag = 0;
		for(int i = 0; i < num; i++){
			if (contains(node->right->sat, adj[i]))
			{
				flag = 1;
				break;
			}
		}
		if (flag == 1){
			add(node->sat, State);
		}
		states = states->next;

	}

}

// void dfsEG(state* State, treeNode* node, HashSet* visited) {
//     if (contains(visited, State)) return;
//     add(visited, State);
//     add(node->sat, State);
//     for (int i = 0; i < State->num_adj; i++) {
//         if (contains(node->right->sat, State->adj[i])) {
//             dfsEG(State->adj[i], node, visited);
//         }
//     }
// }

// bool traverseEG(state* State, treeNode* node,HashSet* visited){
// 	if(!contains(visited, State)){
// 		add(visited, State);
// 		state** adj = State->adj;
// 		int num = State->num_adj;
// 		for (int i = 0; i < num; i++){
// 			if(contains(node->right, adj[i])){
// 			if (traverseEG(adj[i], node, visited)){
// 				return true;
// 			}
// 		}
// 		}

// 	}
// 	else{
// 		return false;
// 	}
// 	return false;
// }

// void checkEG(KripkeStructure* ks, treeNode* node){
// 	StateNode* states = ks->states;
// 	HashSet* Set;
// 	Set = (HashSet* ) malloc(sizeof(HashSet));
// 	initSet(Set);
//     while (states) {
//         state* State = states->s;
//         if (contains(node->right->sat, State)) {
// 			add(Set,State);
//         }
//         states = states->next;
//     }
// 	int update = 0;
// 	while(true){
// 		for (int i = 0; i < TABLE_SIZE; i++) {
// 			Node* element = Set->buckets[i];
// 			Node* prev = NULL;
			
// 			while (!element) {
// 				HashSet* visited = (HashSet* ) malloc(sizeof(HashSet));
// 				initSet(visited);
// 				if (!traverseEG(element->key, node, visited)) {
// 					removeElement(Set, element->key);
// 				} else {
// 					prev = element;
// 					element = element->next;
// 				}
// 			}
// 		}
// 	}
// }

bool traverseEG(state* State, treeNode* node, HashSet* visited) {
    if (contains(visited, State)) return true;

    add(visited, State);
    state** adj = State->adj;
    int num = State->num_adj;
    for (int i = 0; i < num; i++) {
        if (contains(node->right->sat, adj[i])) {
            if (traverseEG(adj[i], node, visited)) {
                return true;
            }
        }
    }
    return false;
}


void checkEG(KripkeStructure* ks, treeNode* node) {
    StateNode* states = ks->states;
    HashSet* Set;
	Set = (HashSet* ) malloc(sizeof(HashSet));
    initSet(Set);

    
    while (states) {
        state* State = states->s;
        if (contains(node->right->sat, State)) {
            add(Set, State);
        }
        states = states->next;
    }

    bool updated;
    do {
        updated = false;
        for (int i = 0; i < TABLE_SIZE; i++) {
            Node* element = Set->buckets[i];
            Node* prev = NULL;

            while (element) {
                HashSet visited;
                initSet(&visited);

                if (!traverseEG(element->key, node, &visited)) {
                    Node* toRemove = element;
                    element = element->next;
                    removeElement(Set, toRemove->key);
                    updated = true;
                } else {
                    prev = element;
                    element = element->next;
                }
            }
        }
    } while (updated);

    node->sat = Set;
}

void checkEU(KripkeStructure* ks, treeNode* node){
	StateNode* states = ks->states;
    HashSet* Set;
	Set = (HashSet* ) malloc(sizeof(HashSet));
    initSet(Set);

    
    while (states) {
        state* State = states->s;
		printf("%s", State->name);
        if (contains(node->right->sat, State)) {
            add(Set, State);
        }
        states = states->next;
    }

	bool updated;
    do {
        updated = false;
		states = ks->states; 
		while (states) {
			state* State = states->s;
			if (contains(node->left->sat, State)) {
				state** adj = State->adj;
				for (int i = 0; i < State->num_adj; i++){
					if (contains(Set,adj[i])){
						if (!contains(Set, State) && contains(Set, adj[i])) { 
							add(Set, State);
							updated = true;
							break;
						}
					}
				}
	
			}
			states = states->next;
		}
        
    } while (updated);

    node->sat = Set;


}






