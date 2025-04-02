%{
int yylex();
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parseTree.h"
#include "kripke.h"
#include "model_checker.h"
void yyerror(const char *s);
treeNode* root_ctl;
// KripkeStructure* ks;
int no_of_states = 0;
extern int node_id;
KripkeStructure* ks;

%}

%union{
	char prop_var;
	struct treeNode* node;
	char* state_var;
	struct state* State;
	struct StateNode* States;
	struct transition* tran;
	struct KripkeStructure* Kripke;
	struct label* label;
	struct labelling* Labelling;
	char* Prop_list;
}


%token TOP BOTTOM NOT AND OR IMPLIES EX EG EF AX AG AF 
%token BEGCTL ENDCTL BEGK ENDK 
%token STATES TRANS LS
%token A E U LPAREN RPAREN
%token <prop_var> PROP_VAR
%token <state_var> STATE_VAR
%type <node> formula
%type <node> ctlformula
%type <State> state 
%type <States> states state_list
%type <Kripke> kripke 
%type <tran> trans tran_list tran
%type <Labelling> label label_state label_states
%type <label> prop_list prop


%left IMPLIES
%left OR
%left AND
%right NOT
%right AX EX AG EG AF EF A E U

%%

Input : kripke ctlformula{ 
	ks = $1;
    printf("\n----------------------------\n");
    printf("The Kripke Structure is \n");
	printKripke(ks);
	root_ctl = $2;
    printf("\n----------------------------\n");
    printf("The CTL formula is (infix of the tree) \n");
	printTree(root_ctl, 0);
    printf("\n----------------------------\n");
    printf("The States satisfying the formula is \n");
    printSet(root_ctl->sat);
	// evaluateAndPrintCTL(root_ctl, ks);

}

kripke : BEGK states trans label ENDK { 
   ks = (KripkeStructure*) malloc(sizeof(KripkeStructure));
    if (!ks) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }

    ks->num_states = 0;
    ks->states = $2;
    
    StateNode* current = ks->states;
    while (current) {
        ks->num_states++;
        current = current->next;
    }

    ks->initial = ($2) ? $2->s : NULL;
    addTransition(ks, $3);

    labelling* Label = $4;

	while(Label){
		// printf("%s == ", Label->state);
		label* prop;
		prop = Label->label;
		state* st = findState(ks ,Label->state );
		st->labelset = prop;
		Label = Label->next;
	}
    
    $$ = ks;
};

states : STATES ':' '[' state_list ']' {
    $$ = $4;
};

state_list : state ',' state_list {
    StateNode* node = (StateNode*) malloc(sizeof(StateNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }
    node->s = $1;
    node->next = $3;
    $$ = node;
}
| state {
    StateNode* node = (StateNode*) malloc(sizeof(StateNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }
    node->s = $1;
    node->next = NULL;
    $$ = node;
};

state : STATE_VAR {
    $$ = createState($1);
};

trans : TRANS ':' '[' tran_list ']' {
    $$ = $4; 
};

tran_list : tran ',' tran_list {
    transition* head = $1;
    transition* temp = head;
    while (temp->next) {
        temp = temp->next;
    }
    temp->next = $3;
    $$ = head;
}
| tran {
    $$ = $1;
};

tran : STATE_VAR ':' STATE_VAR {
    transition* trans = (transition*) malloc(sizeof(transition));
    if (!trans) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }
    trans->from = strdup($1);
    trans->to = strdup($3);
    trans->next = NULL;
    $$ = trans;
};

label : {}
	 | LS ':' '[' label_states ']' {
    // printf("heyyy\n");
    $$ = $4;
}
;

label_states : label_state ',' label_states {
    labelling* label = $1;
    while (label->next) {
        label = label->next;
    }
    label->next = $3;
    $$ = $1;
}
| label_state {
    $$ = $1;
};

label_state :  STATE_VAR ':' '[' prop_list ']'  {
    labelling* label = (labelling*) malloc(sizeof(labelling));
    if (!label) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }
    label->state = strdup($1);
    label->next = NULL; 
	label->label = $4; 
    $$ = label;
};

prop_list : prop ',' prop_list {
	$$ = $1;
	while ($1->next)
		$1 = $1->next;
	$1->next = $3;
}
| prop {
    $$ = $1;
};

prop : PROP_VAR {
    label* prop = (label*) malloc(sizeof(label));
	prop->prop_var = $1;
	prop->next = NULL;
	$$ = prop;
};


ctlformula : BEGCTL formula ENDCTL {
	$$ = $2; 
}

formula  
    : LPAREN formula RPAREN { $$ = $2; }
    | formula AND formula { $$ = createNode(AND_OP, 0, $1, $3); 
    checkAnd(ks,$$);
    printf("For and : \n");
    printSet($$->sat);}
    | formula OR formula {
        treeNode* node1 = createNode(NOT_OP, 0, NULL, $1);
        checkNot(ks,node1);
        treeNode* node2 = createNode(NOT_OP, 0, NULL, $3);
        checkNot(ks,node2);
        treeNode* node3 = createNode(AND_OP, 0, node1, node2);
        checkAnd(ks,node3);
		$$ = createNode(NOT_OP, 0, NULL, node3); 
        checkNot(ks,$$);
        printf("For or : \n");
        printSet($$->sat);

}
    | formula IMPLIES formula { 
		// $$ = createNode(IMPLIES_OP, 0, $1, $3);
        treeNode* node1 = createNode(NOT_OP, 0, NULL, $3);
        checkNot(ks,node1);
        treeNode* node2 = createNode(AND_OP, 0, $1, node1);
        checkAnd(ks,node2);
		$$ = createNode(NOT_OP, 0, NULL, node2);
        checkNot(ks,$$);
        printf("For implies : \n");
        printSet($$->sat);


 }
    | NOT formula %prec NOT { $$ = createNode(NOT_OP, 0, NULL, $2); 
    checkNot(ks,$$);
    printf("For not : \n");
    printSet($$->sat);
}
    | PROP_VAR { $$ = createNode(PROP_VAR_OP, $1, NULL, NULL); 
    // printKripke(ks);
    checkProp(ks, $$); 
    printf("For variable : \n");
    printSet($$->sat);
 }
    | TOP { $$ = createNode(PROP_VAR_OP, 'T', NULL, NULL); 
    checkProp(ks, $$);
    printf("For top : \n");
    printSet($$->sat);
 }
    | BOTTOM { $$ = createNode(PROP_VAR_OP, 'F', NULL, NULL); 
    checkProp(ks, $$);}
    | AG formula  %prec AG  { 
		treeNode* node1 = createNode(NOT_OP, 0, NULL, $2);
        checkNot(ks,node1);
        treeNode* node2 = createNode(PROP_VAR_OP, 'T', NULL,NULL);
        checkProp(ks, node2);
		treeNode* node3 = createNode(EU_OP, 0, node2, node1);
        checkEU(ks,node3);
		$$ = createNode(NOT_OP, 0, NULL, node3 ); 
        checkNot(ks,$$);
        printf("for AG\n");
        printSet($$->sat);
}
    | EG formula  %prec EG { 
        $$ = createNode(EG_OP, 0, NULL, $2);
        checkEG(ks, $$);
        printf("for EG\n");
        printSet($$->sat);
 }
    | AF formula  %prec AF { 

		treeNode* node1 = createNode(NOT_OP, 0, NULL, $2);
        checkNot(ks, node1);
	    treeNode* node2 = createNode(EG_OP, 0, NULL, node1);
        checkEG(ks, node2);
	    $$ = createNode(NOT_OP, 0, NULL, node2);
        checkNot(ks, $$);
        printf("for AF\n");
        printSet($$->sat);
	 }
    | EF formula  %prec EF { 
        treeNode* node1 = createNode(PROP_VAR_OP, 'T', NULL,NULL);
        checkProp(ks, node1);
        $$ = createNode(EU_OP, 0, node1, $2);
        checkEU(ks, $$);
        printf("for EF\n");
        printSet($$->sat);
}
    | AX formula  %prec AX { 

		treeNode* node0 = createNode(NOT_OP, 0, NULL, $2);
        checkNot(ks, node0);
		treeNode* node1 = createNode(EX_OP, 0, NULL, node0);
        checkEX(ks,node1);
		 $$ = createNode(NOT_OP, 0, NULL, node1);
         checkNot(ks,$$);
         printf("for AX : \n");
         printSet($$->sat);
		}
    | EX formula  %prec EX { 

		$$ = createNode(EX_OP, 0, NULL, $2);
        checkEX(ks,$$);
        printf("for EX : \n");
         printSet($$->sat);
	 }
    | A '[' formula U formula ']'{ treeNode* node1 = createNode(NOT_OP, 0, NULL, $5);
    checkNot(ks, node1);
	treeNode* node2 = createNode(EG_OP, 0, NULL, node1);
    checkEG(ks, node2);
	treeNode* node2a = createNode(NOT_OP, 0, NULL, $3);
    checkNot(ks, node2a);
    treeNode* node2b = createNode(NOT_OP, 0, NULL, node2);
    checkNot(ks, node2b);
	treeNode* node3 = createNode(AND_OP, 0, node2a,node1);
    checkAnd(ks, node3);
	treeNode* node4 = createNode(EU_OP, 0, node1, node3);
    checkEU(ks, node4);
    treeNode* node4a = createNode(NOT_OP, 0, NULL, node4);
    checkNot(ks, node4a);
    treeNode* node5a = createNode(AND_OP, 0, node4a, node2b);
    checkAnd(ks, node5a);
	treeNode* node5 = createNode(NOT_OP, 0, NULL, node5a); 
    checkNot(ks, node5);
	$$ = createNode(NOT_OP, 0, NULL, node5);
    checkNot(ks, $$);
    printf("for AU : \n");
    printSet($$->sat);
	 }

    | E '[' formula U formula ']' { 

		$$ = createNode(EU_OP, 0, $3, $5);
        checkEU(ks, $$);
        printf("for EU\n");
        printSet($$->sat); }


%%

int main() {

    yyparse();
	printf("\nTotal no of nodes = %d\n", node_id);

    return 0;
}

void yyerror(const char *s) {
    fprintf(stderr, "Parse error: %s\n", s);
}
