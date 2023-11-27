#ifndef DIFFERCIATOR_DED
#define DIFFERCIATOR_DED
#include "RuzalLib/include/Tree.h"
#include <math.h>
#include <cstdlib>

const double EXP     = 2.7182818284590451;
const double EPSILON = 0.01;

enum ExprCommands {
	#define DEF_EXPR_CMD(cmd_name, command, code, priority, handle, ...) cmd_name = code,

	#include "def_expr_cmd.h"

	#undef DEF_EXPR_CMD
};

void ExpressionOptimization(Tree* tree);

double LogBase(double num, double base);
double Eval(const Node* node);

Node* DiffExpr(const Node* node);
#endif // !DIFFERCIATOR_DED
