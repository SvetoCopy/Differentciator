#define _CRT_SECURE_NO_WARNINGS
#ifndef TREE_DED
#define TREE_DED

#include "stdio.h"
#include "assert.h"
#include <stdlib.h>
#include <string.h>

const int TREE_ERROR = -1;

enum ExprElemType {
	NUM = 0,
	COMMAND = 1,
	VAR = 2,
	DELETED = -1
};

struct ExprVar {
	const char* name;
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
	FILE*  latex_logfile;
};

Node* OpNew(NodeInfo_t data);
void  OpDelete(Node* node);
Node* CopyNode(const Node* node);

void _NodeDtor(Node* node, const char* file, size_t line, const char* func);

#define NodeDtor(node) _NodeDtor(node, __FILE__, __LINE__, __FUNCTION__)

int TreeCtor(Tree* tree, const char* graph_logfile_name, NodeInfo_t start_data, const char* latex_file_name);
int TreeDtor(Tree* tree);
void TreeVerify(Tree* tree);

void VerifyNode(const Node* node);

#endif // !TREE_DED
