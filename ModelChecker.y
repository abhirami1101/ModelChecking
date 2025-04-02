%{
int yylex();
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parseTree.h"
#include "kripke.h"
// #include "model_checker.h"
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
	printKripke(ks);
	root_ctl = $2;
	printTree(root_ctl, 0);
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
    | formula AND formula { $$ = createNode(AND_OP, 0, $1, $3); }
    | formula OR formula {
        treeNode* node1 = createNode(NOT_OP, 0, NULL, $1);
        treeNode* node2 = createNode(NOT_OP, 0, NULL, $3);
        treeNode* node3 = createNode(AND_OP, 0, node1, node2);
		$$ = createNode(NOT_OP, 0, NULL, node3); 
}
    | formula IMPLIES formula { 
		// $$ = createNode(IMPLIES_OP, 0, $1, $3);
        treeNode* node1 = createNode(NOT_OP, 0, NULL, $3);
        treeNode* node2 = createNode(AND_OP, 0, $1, node1);
		$$ = createNode(NOT_OP, 0, NULL, node2);

 }
    | NOT formula %prec NOT { $$ = createNode(NOT_OP, 0, NULL, $2); 
}
    | PROP_VAR { $$ = createNode(PROP_VAR_OP, $1, NULL, NULL); 
 }
    | TOP { $$ = createNode(PROP_VAR_OP, 'T', NULL, NULL); 
 }
    | BOTTOM { $$ = createNode(PROP_VAR_OP, 'F', NULL, NULL); }
    | AG formula  %prec AG  { 
		treeNode* node1 = createNode(NOT_OP, 0, NULL, $2);
        treeNode* node2 = createNode(PROP_VAR_OP, 'T', NULL,NULL);
		treeNode* node3 = createNode(EU_OP, 0, node2, node1);
		$$ = createNode(NOT_OP, 0, NULL, node3 ); 
}
    | EG formula  %prec EG { $$ = createNode(EG_OP, 0, NULL, $2);
 }
    | AF formula  %prec AF { 

		treeNode* node1 = createNode(NOT_OP, 0, NULL, $2);
	treeNode* node2 = createNode(EG_OP, 0, NULL, node1);
	$$ = createNode(NOT_OP, 0, NULL, node2);
	 }
    | EF formula  %prec EF { $$ = createNode(EU_OP, 0, createNode(PROP_VAR_OP, 'T', NULL,NULL), $2); 
}
    | AX formula  %prec AX { 

		treeNode* node0 = createNode(NOT_OP, 0, NULL, $2);
		treeNode* node1 = createNode(EX_OP, 0, NULL, node0);
		 $$ = createNode(NOT_OP, 0, NULL, node1);
		}
    | EX formula  %prec EX { 

		$$ = createNode(EX_OP, 0, NULL, $2);
	 }
    | A '[' formula U formula ']'{ treeNode* node1 = createNode(NOT_OP, 0, NULL, $5);

	treeNode* node2 = createNode(EG_OP, 0, NULL, node1);
	treeNode* node2a = createNode(NOT_OP, 0, NULL, $3);
    treeNode* node2b = createNode(NOT_OP, 0, NULL, node2);
	treeNode* node3 = createNode(AND_OP, 0, node2a,node1);
	treeNode* node4 = createNode(EU_OP, 0, node1, node3);
    treeNode* node4a = createNode(NOT_OP, 0, NULL, node4);
    treeNode* node5a = createNode(AND_OP, 0, node4a, node2b);
	treeNode* node5 = createNode(NOT_OP, 0, NULL, node5a); 
	$$ = createNode(NOT_OP, 0, NULL, node5);
	 }

    | E '[' formula U formula ']' { 

		$$ = createNode(EU_OP, 0, $3, $5); }


%%

int main() {
    // printf("Enter CTL formula: ");
    yyparse();
	printf("\nTotal no of nodes = %d\n", node_id);
	// bottomUpParser(root);
    return 0;
}

void yyerror(const char *s) {
    fprintf(stderr, "Parse error: %s\n", s);
}
