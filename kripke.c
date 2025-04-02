// Create a new state

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kripke.h"

state* createState(char* name) {
    state* newState = (state*)malloc(sizeof(state));
    newState->name = strdup(name);
    newState->num_adj = 0;
    newState->adj = (state**)malloc(MAX_STATES * sizeof(state*)); 
    return newState;
}



void add_initial (state* st, KripkeStructure* ks){
	ks->initial = st;
}

void addLabel(state* St, char label_val){
	label* l = St->labelset;
	label* new = (label*) malloc(sizeof(label));
	while (l->next){
		l = l->next;
	}
	new->prop_var = label_val;
	l->next = NULL;
}

void addTransition(KripkeStructure* ks, transition* ts) {
	// printf("hey");
	while (ts){
		// printf("from -- %s to -- %s\n", ts->from,ts->to);
		state* from = findState(ks , ts->from);
		state* to = findState(ks , ts->to);
		// printf("from_ks -- %s to -- %s\n", from->name, to->name);
		from->adj[from->num_adj++] = to;
		ts = ts->next;
	}
}




void printKripke(KripkeStructure* ks) {
    printf("Kripke Structure with %d states and initial state %s\n", ks->num_states, ks->initial->name);
    StateNode* current = ks->states;

    while (current) {
        state* s = current->s;
        printf("State: %s\n", s->name);
		printf("reachable_states : ");
        for (int i = 0; i < s->num_adj; i++){
			printf("%s,", s->adj[i]->name);
		}
		// printf("\n");
        printf("\n  Labeling: ");
		label* pointer = s->labelset;
		while (pointer){
			printf("%c, ", pointer->prop_var);
			pointer = pointer->next;
		}
		printf("\n\n");
        current = current->next;
    }
}

state* findState(KripkeStructure* ks, char* name) {
    StateNode* current = ks->states;
    while (current) {
        if (strcmp(current->s->name, name) == 0) {
            return current->s;
        }
        current = current->next;
    }
    return NULL;
}


