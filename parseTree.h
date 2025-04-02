#ifndef PARSETREE_H
#define PARSETREE_H

#include <stdlib.h>
#include <stdbool.h>
#include "kripke.h"
#include "Set.h"

typedef enum {EX_OP, EG_OP, EU_OP, AND_OP, OR_OP, NOT_OP, IMPLIES_OP, PROP_VAR_OP } ops;

typedef struct treeNode{
	ops op;
	char prop_var;
	int node_id;
	HashSet* sat;
	struct treeNode*  left;
	struct treeNode* right; 
} treeNode;

treeNode* createNode(ops op, char prop_var, treeNode* left, treeNode* right);

void printTree(treeNode* root, int depth);

#endif