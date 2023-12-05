#include "LatexDump.h"

static int CommandPriority(int cmd_code) {
	switch (cmd_code) {
		#define DEF_EXPR_CMD(cmd_name, command, cmd_code, priority, ...) \
			case cmd_code:                                               \
				return priority;        
		#include "def_expr_cmd.h"
		#undef DEF_EXPR_CMD
	}
}

void PrintCommand(int cmd_code, FILE* file) {
	switch (cmd_code) {
		#define DEF_EXPR_CMD(cmd_name, command, cmd_code, priority, ...)  \
			case cmd_code: {                                              \
				printf(" " #command " ");                                 \
				break;                                                    \
			}
		#include "def_expr_cmd.h"
		#undef DEF_EXPR_CMD
	}
}

bool isNeedBrackets(Node* child, Node* parent) {

	assert(child != nullptr);

	if (parent == nullptr ||
		CommandPriority(NODE_CMD_CODE(child)) < CommandPriority(NODE_CMD_CODE(parent)))
		return false;

	return true;
}

bool isSqrtExpr(Node* node) {
	if (node->right->data.type == COMMAND &&
		NODE_CMD_CODE(node) == DIV &&
		node->right->left->data.type == NUM &&
		NODE_IMM_VALUE(node->right->left) == 1) {

		return true;

	}

	return false;
}

void PrintLatexNode(Node* child, Node* parent, FILE* file) {
	assert(child != nullptr);

	if (child->data.type == VAR) {
		fprintf(file, "%s", NODE_VAR_NAME(child));
		return;
	}
	if (child->data.type == NUM) {
		fprintf(file, "%.2lf", NODE_IMM_VALUE(child));
		return;
	}
	if (child->data.type == COMMAND) {
		bool is_need_brackets = isNeedBrackets(child, parent);
		
		if (is_need_brackets)
			fprintf(file, " ( ");

		switch (NODE_CMD_CODE(child)) {
		case SUB:
			PrintLatexNode(child->left, child, file);
			fprintf(file, " - ");
			PrintLatexNode(child->right, child, file);
			break;

		case ADD:
			PrintLatexNode(child->left, child, file);
			fprintf(file, " + ");
			PrintLatexNode(child->right, child, file);
			break;

		case MUL:
			PrintLatexNode(child->left, child, file);
			fprintf(file, " \\cdot ");
			PrintLatexNode(child->right, child, file);
			break;

		case DIV:
			fprintf(file, " \\dfrac{");
			PrintLatexNode(child->left, child, file);
			fprintf(file, " }{ ");
			PrintLatexNode(child->right, child, file);
			fprintf(file, " } ");
			break;

		case POW:
			if (isSqrtExpr(child)) {
				fprintf(file, " \\sqrt[");
				PrintLatexNode(child->right, child, file);
				fprintf(file, " ]{ ");
				PrintLatexNode(child->left, child, file);
				fprintf(file, " } ");
			}
			else {
				fprintf(file, " { ");
				PrintLatexNode(child->left, child, file);
				fprintf(file, " } ^ {\\small ");
				PrintLatexNode(child->right, child, file);
				fprintf(file, " } ");
				break;
			}

		case LOG:
			fprintf(file, " \log_{ ");
			PrintLatexNode(child->left, child, file);
			fprintf(file, " }{ ");
			PrintLatexNode(child->right, child, file);
			fprintf(file, " } ");
			break;

		case COS:
			fprintf(file, " \cos( ");
			PrintLatexNode(child->left, child, file);
			fprintf(file, " ) ");
			break;

		case SIN:
			fprintf(file, " \sin( ");
			PrintLatexNode(child->left, child, file);
			fprintf(file, " ) ");
			break;
		}

		if (is_need_brackets)
			fprintf(file, " ) ");
	}
}

void PrintLatexExpr(Tree* tree, const char* latex_file_name) {
	assert(tree != nullptr);
	
	FILE* file = {};
	fopen_s(&file, latex_file_name, "w");

	fprintf(file, "\\documentclass[a4paper, 12pt]{article}\n"
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
				"\\date{November 2023}\n"
				"\\begin{document}\n"
				"\\maketitle\n"
				"\\section{Expression}\n");

	fprintf(file, "$");
	PrintLatexNode(tree->root, nullptr, file);
	fprintf(file, "$\n");

	fprintf(file, "\\end{document}");
}