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
	StateNode* states = ks->states; // for checking the states
	if (node->prop_var == 'T'){ // if the formula to be checked is T
		while(states){ // we can add all the states to the formula
			state* State = states->s;
			add(node->sat, State);	// for adding the state to satisfying set of the formula	
			states = states->next;
		}
		return;
	}
	if (node->prop_var == 'F'){ // if the formula is F, no states would be added
		return;
	}
	while(states){ // if not any of the above, traverse through states
		state* State = states->s; 
		label* label = State->labelset; // get the label set of that state
		while (label){ // traverse through the labels
			if (label->prop_var == node->prop_var){ // if label = the prop variable
				add(node->sat, State); // add the state to the Satisfying set of the formula node
				break;
			}
			label = label->next;
		}
		states = states->next;

	}
}

void checkAnd(KripkeStructure* ks, treeNode* node){
	StateNode* states = ks->states; // for checking the states
	while(states){ 
		state* State = states->s;
		bool flag1 = contains(node->left->sat,State); // check if the formula1 is satisfied by the state
		// if flag1 is true, that means the state would be present in the satisying set of the formula1
		bool flag2 = contains(node->right->sat,State); // check if the formula2 is satisfied by the state
		// if flag2 is true, that means the state would be present in the satisying set of the formula2
		if (flag1 && flag2){ // if both flags are true, we can add the state to the satisying set of the formula
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

bool traverseEG(state* State, treeNode* node, HashSet* visited) { // returns true 
	// if theres a path from that state where in all states in the path, satisying the formula 
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
		// printf("%s", State->name);
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






