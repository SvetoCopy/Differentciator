#define _CRT_SECURE_NO_WARNINGS
#include "../include/Tree.h"

Node* OpNew(NodeInfo_t data) {
	Node* node = (Node*)calloc(1, sizeof(Node));
	node->data = data;
	return node;
}

void OpDelete(Node* node) {
	assert(node != nullptr);
	//free(node->data);
	free(node);
}

void _NodeDtor(Node* node, const char* file, size_t line, const char* func) {
	//printf("%s : line %zu func: %s\n", file, line, func);

	if (node == nullptr)
		return;

	NodeDtor(node->left);
	NodeDtor(node->right);


	OpDelete(node);
	
	node->left = nullptr;
	node->right = nullptr;
}

int TreeCtor(Tree* tree, const char* graph_logfile_name, 
			 NodeInfo_t start_data, const char* latex_file_name) {

	

	tree->root = OpNew(start_data);
	tree->graph_logfile = fopen(graph_logfile_name, "w");
	tree->size = 0;

	tree->latex_logfile = fopen(latex_file_name, "w");

	fprintf(tree->latex_logfile, "\\documentclass[a4paper, 12pt]{article}\n"
		"\\usepackage[T2A]{ fontenc }\n"
		"\\usepackage[utf8]{ inputenc }\n"
		"\\usepackage[english, russian]{babel}\n"
		"\\usepackage{amsmath,amsfonts,amssymb,amsthm,mathtools}\n"
		"\\usepackage[colorlinks, linkcolor = blue]{hyperref}\n"
		"\\usepackage{upgreek}\n"
		"\\usepackage[left = 2cm, right = 2cm, top = 2cm, bottom = 3cm, bindingoffset = 0cm]{geometry}\n"
		"\\usepackage{graphicx}\n"
		"\\usepackage{multirow}\n"
		"\\usepackage{xcolor}\n"
		"\\usepackage{tabularx}\n"
		"\\title{ Differenciator }\n"
		"\\author{Valiev Ruzal}\n"
		"\\date{December 2023}\n"
		"\\begin{document}\n"
		"\\maketitle\n"
		"\\section{Expression}\n");

	TreeVerify(tree);

	return 0;
}

int TreeDtor(Tree* tree) {

	assert(tree != nullptr);

	NodeDtor(tree->root);

	fclose(tree->graph_logfile);

	fprintf(tree->latex_logfile, "\\end{document}");
	fclose(tree->latex_logfile);

	return 0;
}

static int GetCommandArgsNum(const Node* node) {

	assert(node != nullptr);

	#define DEF_EXPR_CMD(cmd_name, command, cmd_code, priority, args_num, ...)  \
				if (node->data.value.command_type == cmd_code) {                \
					return args_num;                                            \
				}
	#include "../../def_expr_cmd.h"
	#undef DEF_EXPR_CMD
}

void VerifyNode(const Node* node) {
	
	if (node == nullptr) return;

	if (node->left == nullptr && node->right == nullptr) {
		if (node->data.type != NUM && node->data.type != VAR){
			assert(!"Leaf is not NUM\n");
		}
	}

	if (node->data.type == COMMAND) {
		int args_num = GetCommandArgsNum(node);
		if (args_num == 1 && (node->right != nullptr || node->left == nullptr))
			assert(!"Command with one arg have 0 or 2 arg\n");

		if (args_num == 2 && (node->right == nullptr || node->left == nullptr))
			assert(!"Command with two arg have 0 or 1 arg\n");
	}

	VerifyNode(node->left);
	VerifyNode(node->right);
}

void TreeVerify(Tree* tree) {

	assert(tree->latex_logfile != nullptr && "Latex Logfile is null");
	assert(tree != nullptr && "Tree is null");

	VerifyNode(tree->root);
}

Node* CopyNode(const Node* node) {

	if (node == nullptr) return nullptr;

	Node* res = OpNew(node->data);

	res->left  = CopyNode(node->left);
	res->right = CopyNode(node->right);

	return res;
}