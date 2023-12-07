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

bool isNeedBrackets(const Node* child, const Node* parent) {

	assert(child != nullptr);

	if (parent == nullptr ||
		CommandPriority(NODE_CMD_CODE(child)) <= CommandPriority(NODE_CMD_CODE(parent)))
		return false;

	return true;
}

bool isSqrtExpr(const Node* node) {

	assert(node != nullptr);

	if (node->right->data.type == COMMAND &&
		NODE_CMD_CODE(node) == DIV &&
		node->right->left->data.type == NUM &&
		NODE_IMM_VALUE(node->right->left) == 1) {

		return true;
	}

	return false;
}

bool isIntValue(double num) {
	return (num - (int)num < EPSILON) ? true : false;
}

void CreateExprSchedule(Tree* first_tree, Tree* second_tree, ExprVar var, const char* schedule_name) {

	assert(first_tree != nullptr);

	FILE* python_file = fopen("shedule.py", "w");

	assert(python_file != nullptr);

	fprintf(python_file, "# -*- coding: utf-8 -*-\n");
	fprintf(python_file, "import sympy as sp\n");
	fprintf(python_file, "import matplotlib.pyplot as plt\n");
	fprintf(python_file, "import numpy as np\n\n");

	fprintf(python_file, "%s = sp.symbols('%s')\n", var.name, var.name);
	fprintf(python_file, "str_expr_1 = \"");

	PrintTreeExpr(first_tree, python_file);

	fprintf(python_file, "\"\n");
	fprintf(python_file, "expr_1 = sp.sympify(str_expr_1)\n");
	fprintf(python_file, "f_1 = sp.lambdify(%s, expr_1, modules=[\"numpy\"])\n\n", var.name);
	if (second_tree != nullptr) {
		fprintf(python_file, "str_expr_2 = \"");

		PrintTreeExpr(second_tree, python_file);

		fprintf(python_file, "\"\n");
		fprintf(python_file, "expr_2 = sp.sympify(str_expr_2)\n");
		fprintf(python_file, "f_2 = sp.lambdify(%s, expr_2, modules=[\"numpy\"])\n\n", var.name);
	}
	
	fprintf(python_file, "y_vals = np.linspace(-5, 5, 1000)\n");
	fprintf(python_file, "result_1 = f_1(y_vals)\n");

	if (second_tree != nullptr)
		fprintf(python_file, "result_2 = f_2(y_vals)\n\n");

	fprintf(python_file, "plt.figure(figsize=(8, 6))\n");
	fprintf(python_file, "plt.plot(y_vals, result_1, label=str_expr_1)\n");
	if (second_tree != nullptr)
		fprintf(python_file, "plt.plot(y_vals, result_2, label=str_expr_2)\n");
	fprintf(python_file, "plt.xlabel('%s')\n", var.name);
	fprintf(python_file, "plt.ylabel('f(%s)')\n", var.name);
	fprintf(python_file, "plt.title('RUZAL DIFFERENCIATOR')\n");
	fprintf(python_file, "plt.legend()\n");
	fprintf(python_file, "plt.grid(True)\n");
	fprintf(python_file, "plt.savefig(\"%s\")\n", schedule_name);

	fclose(python_file);

	system("python shedule.py");
}

void AddImageLatex(Tree* tree, const char* image_name) {
	fprintf(tree->latex_logfile, "\\begin{figure} [!ht]\n");
	fprintf(tree->latex_logfile, "\\begin{flushleft}\n");
	fprintf(tree->latex_logfile, "\\includegraphics[scale = %lf]{%s}\n", IMAGE_SCALE, image_name);
	fprintf(tree->latex_logfile, "\\end{flushleft}\n");
	fprintf(tree->latex_logfile, "\\end{figure}\n");
}

void PrintLatexNode(const Node* child, const Node* parent, FILE* file) {

	assert(child != nullptr);

	if (child->data.type == VAR) {
		fprintf(file, "%s", NODE_VAR_NAME(child));
		return;
	}

	if (child->data.type == NUM) {

		if (isIntValue(NODE_IMM_VALUE(child)))
			fprintf(file, "%d", (int)NODE_IMM_VALUE(child));
		else
			fprintf(file, "%.2lf", NODE_IMM_VALUE(child));

		return;
	}

	if (child->data.type == COMMAND) {
		bool is_need_brackets = isNeedBrackets(child, parent);

		if (is_need_brackets)
			fprintf(file, " ( ");

		switch (NODE_CMD_CODE(child)) {
		#define DEF_EXPR_CMD(command_name, command_str, int_code, priority, args_num, handle, diff, ...) \
		case command_name:																				 \
			__VA_ARGS__																	                 \
			break;
		#include "def_expr_cmd.h"
		
		}

		if (is_need_brackets)
			fprintf(file, " ) ");
	}
}

void PrintLatexExpr(const Node* node, FILE* latex_logfile) {

	assert(node != nullptr);

	FileInfo phrases_file = FileInfoCtor("MathStandartPhrases.txt");

	fprintf(latex_logfile, "%s\\newline\n", phrases_file.text[rand() % phrases_file.n_lines]);

	fprintf(latex_logfile, "$");
	PrintLatexNode(node, nullptr, latex_logfile);
	fprintf(latex_logfile, "$\\newline\n");

	FileInfoDtor(&phrases_file);
}