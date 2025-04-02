
#ifndef MODEL_CHECKER_H
#define MODEL_CHECKER_H
#include "kripke.h"
#include "parseTree.h"

void checkProp(KripkeStructure* ks, treeNode* node);
void checkAnd(KripkeStructure* ks, treeNode* node);
void checkNot(KripkeStructure* ks, treeNode* node);
void checkEX(KripkeStructure* ks, treeNode* node);

#endif