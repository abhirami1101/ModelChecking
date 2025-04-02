#ifndef KRIPKE_H
#define KRIPKE_H

#include <stdlib.h>
#include <stdbool.h>

#define MAX_STATES 100
#define MAX_PROPS 26
#define MAX_TRANSITIONS 100

typedef struct label{
	char prop_var;
	struct label* next;
}label;



typedef struct state{
	char* name;
	struct state** adj;
	label* labelset;
	int num_adj;
}state; 

typedef struct transition{
	char* from;
	char* to;
	struct transition* next;
}transition;


typedef struct labelling{
	char* state;
	label* label;
	struct labelling* next;
}labelling;


typedef struct StateNode {
    state* s;
    struct StateNode* next;
} StateNode;

typedef struct KripkeStructure {
	StateNode* states;
	int num_states;
    state* initial;
}KripkeStructure;


state* createState(char* name);
void add_initial (state* st, KripkeStructure* ks);
void addLabel(state* St, char label);
void addTransition(KripkeStructure* ks, transition* ts);
void printKripke(KripkeStructure* ks);
state* findState(KripkeStructure* ks, char* name);

#endif