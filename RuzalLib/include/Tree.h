#define _CRT_SECURE_NO_WARNINGS
#ifndef TREE_DED
#define TREE_DED

#include "stdio.h"
#include "assert.h"
#include <stdlib.h>
#include <string.h>

const int   TREE_ERROR = -1;
const int   MAX_BD_SIZE = 1000;
const int   DEFAULT_TRACK_SIZE = 10;
const int   MAX_NODEINFO_SIZE = 100;
constexpr const char* DEFAULT_NIL = ".";

#define NODE_IMM_VALUE(node) node->data.value.imm_value
#define NODE_CMD_CODE(node)  node->data.value.command_type
#define NODE_VAR_VALUE(node) node->data.value.var.imm_value
#define NODE_VAR_NAME(node)  node->data.value.var.name

enum ParseStatus {
	FOUND = 1,  
	UNFOUND = 2  
};

enum Order {
	POST = 1,
	IN   = 2,
	PRE  = 3
};

enum NodeElem {
	LEFT_ELEM = 0,
	ROOT_ELEM = 1,
	RIGHT_ELEM = 2
};

enum ExprElemType {
	NUM     = 0,
	COMMAND = 1,
	VAR     = 2,
};

struct ExprVar {
	char* name;
	double imm_value;
};

union Value_t {
	ExprVar var;
	int     command_type;
	double  imm_value;
};

struct ExprElem {
	ExprElemType type;
	Value_t      value;
};

typedef ExprElem NodeInfo_t;

struct Node {
	NodeInfo_t data;
	Node*      left;
	Node*      right;
};

struct Tree {
	Node*  root;
	size_t size;
	FILE*  graph_logfile;
};

// Only for ExprElem type
int  ReadNodeIN(char* str, Node** res);
void PrintTreeExpr(Tree* tree);

Node* CopyNode(const Node* node);
bool ÑheckVarInNode(Node* node);

Node* CreateCommandNode(int command_code, Node* left, Node* right);
Node* CreateImmNode(double imm_value, Node* left, Node* right);
Node* CreateVarNode(ExprVar var, Node* left, Node* right);

Node* OpNew(NodeInfo_t data);
void  OpDelete(Node* node);

int TreeCtor(Tree* tree, const char* file_name, NodeInfo_t start_data);
int TreeDtor(Tree* tree);
int TreeVerify(Tree* tree);


#endif // !TREE_DED
