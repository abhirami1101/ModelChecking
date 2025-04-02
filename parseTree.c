#include "parseTree.h"
#include <stdio.h>

int node_id = 0;

treeNode* createNode(ops op, char prop_var, treeNode* left, treeNode* right){
	treeNode* new = (treeNode*) malloc(sizeof(treeNode));
	new->op = op;
	new->prop_var = prop_var;
	new->left = left;
	new->right = right;
	new->node_id = node_id++;
    new->sat = (HashSet*)malloc(sizeof(HashSet));
	initSet(new->sat);
	return new;
}

void printTree(treeNode* root,int depth) {
    if (!root) return;

    if (root->op == PROP_VAR_OP) {
        printf("%c", root->prop_var);
        return;
    }

    if (root->op == NOT_OP || root->op == EX_OP || root->op == EG_OP) {
        printf("(");
        switch (root->op) {
            case NOT_OP: printf("¬"); break;
            case EX_OP: printf("EX "); break;
            case EG_OP: printf("EG "); break;
            default: break;
        }
        printTree(root->right,0);
        printf(")");
        return;
    }

    printf("(");
    printTree(root->left,0);

    switch (root->op) {
        case AND_OP: printf(" ∧ "); break;
        case OR_OP: printf(" ∨ "); break;
        case IMPLIES_OP: printf(" → "); break;
        case EU_OP: printf(" EU "); break;
        default: break;
    }

    printTree(root->right,0);
    printf(")");
}
