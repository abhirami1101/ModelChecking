
#ifndef MODEL_CHECKER_H
#define MODEL_CHECKER_H
#include "kripke.h"
#include "parseTree.h"

bool stateHasLabel(state* s, char prop_var);
bool evaluateCTL(treeNode* formula, state* s, KripkeStructure* ks);
bool checkEG(state* s, treeNode* subformula, KripkeStructure* ks, bool visited[MAX_STATES]);
bool checkEU(state* s, treeNode* left, treeNode* right, KripkeStructure* ks, bool visited[MAX_STATES]) ;
void evaluateAndPrintCTL(treeNode* formula, KripkeStructure* ks);

#endif