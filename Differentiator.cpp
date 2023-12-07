#include "Differentiator.h"

double LogBase(double num, double base)
{
	return log(num) / log(base);
}

double Eval(const Node* node) {
	
	VerifyNode(node);

	if (!node) 
		return 0;
	if (node->data.type == VAR)
		return NODE_VAR_VALUE(node);
	if (node->data.type == NUM)
		return NODE_IMM_VALUE(node);

	double left  = Eval(node->left);
	double right = Eval(node->right);

	switch (NODE_CMD_CODE(node)) {
		#define DEF_EXPR_CMD(cmd_name, command, code, priority, args_num, handle, ...) \
		case code:                                                                     \
			return handle;

		#include "def_expr_cmd.h"

		#undef DEF_EXPR_CMD
	}
}

bool isEqual(double first, double second) {
	return abs(first - second) < EPSILON;
}

// CHECK_CMD(MUL) || CHECK_CMD(DIV)

#define CHECK_CMD(cmd)							       \
		(*node)->data.type == COMMAND &&		       \
		NODE_CMD_CODE((*node)) == cmd

#define CHECK_RIGHT_NUM(num)						   \
		(*node)->right->data.type == NUM &&            \
		isEqual(NODE_IMM_VALUE((*node)->right), num)   \

#define CHECK_LEFT_NUM(num)							   \
		(*node)->left->data.type == NUM &&             \
		isEqual(NODE_IMM_VALUE((*node)->left), 0)	   \

#define REPLACE_TO_LEFT_NODE(node)					   \
		Node* tmp = CopyNode((*node)->left);		   \
													   \
		NodeDtor(*node);							   \
		*node = tmp;

#define REPLACE_TO_RIGHT_NODE(node)				   	   \
		Node* tmp = CopyNode((*node)->right);		   \
													   \
		NodeDtor(*node);							   \
		*node = tmp;

#define REPLACE_TO_IMM_NODE(node, value)			   \
		NodeDtor(*node);							   \
		*node = CreateImmNode(value, nullptr, nullptr);

void MulNDivOptimization(Node** node, int* count_diff) {

	VerifyNode(*node);

	if (CHECK_CMD(MUL) && (CHECK_RIGHT_NUM(0) || CHECK_LEFT_NUM(0)))
	{
		(*count_diff)++;
		REPLACE_TO_IMM_NODE(node, 0);
	}

	else if (CHECK_CMD(MUL) || CHECK_CMD(DIV))
	{
		if (CHECK_RIGHT_NUM(1))
		{
			(*count_diff)++;
			REPLACE_TO_LEFT_NODE(node);
		}
			
		else if (CHECK_CMD(MUL) && CHECK_LEFT_NUM(1))
		{
			(*count_diff)++;
			REPLACE_TO_RIGHT_NODE(node);
		}
	}
}

void PowOptimization(Node** node, int* count_diff) {

	VerifyNode(*node);

	if (CHECK_CMD(POW)) {
		if (CHECK_RIGHT_NUM(0) || CHECK_LEFT_NUM(1))
		{
			(*count_diff)++;
			REPLACE_TO_IMM_NODE(node, 1);
		}

		else if (CHECK_RIGHT_NUM(1))
		{
			(*count_diff)++;
			REPLACE_TO_LEFT_NODE(node);
		}

		else if (CHECK_LEFT_NUM(0))
		{
			(*count_diff)++;
			REPLACE_TO_IMM_NODE(node, 0);
		}
	}
}

void AddNSubOptimization(Node** node, int* count_diff) {

	VerifyNode(*node);

	if (CHECK_CMD(ADD) || CHECK_CMD(SUB))
	{
		if (CHECK_RIGHT_NUM(0))
		{
			(*count_diff)++;
			REPLACE_TO_LEFT_NODE(node);
		}

		else if ((*node)->left->data.type == NUM && isEqual((*node)->left->data.value.imm_value, 0))
		{
			(*count_diff)++;
			REPLACE_TO_RIGHT_NODE(node)
		}
	}
}

void SecondNodeOptimization(Node** node, int* count_diff) {

	VerifyNode(*node);

	if (*node == nullptr)           return;
	if ((*node)->data.type == NUM)  return;

	int count = 0;
	MulNDivOptimization(node, &count);
	if (count != 0) *count_diff += count;
	
	count = 0;
	PowOptimization(node, &count);
	if (count != 0) *count_diff += count;

	count = 0;
	AddNSubOptimization(node, &count);
	if (count != 0) *count_diff += count;

	SecondNodeOptimization(&(*node)->left, count_diff);
	SecondNodeOptimization(&(*node)->right, count_diff);
}

void FirstNodeOptimization(Node** node, int* count_diff) {

	VerifyNode(*node);

	if (*node == nullptr)        return;
	if ((*node)->right == nullptr) return;
	if ((*node)->data.type == NUM) return;

	if ((*node)->data.type == COMMAND   && 
		(*node)->left->data.type == NUM &&
		(*node)->right->data.type == NUM) {
			
		*count_diff++;

		double value = Eval(*node);

		//NodeDtor(*node);
		*node = CreateImmNode(value, nullptr, nullptr);
	}
	else {
		FirstNodeOptimization(&(*node)->left, count_diff);
		FirstNodeOptimization(&(*node)->right, count_diff);
	}
}

void ExpressionOptimization(Tree* tree) {

	TreeVerify(tree);

	int count_diff = 0;

	do {
		count_diff = 0;
		FirstNodeOptimization(&tree->root, &count_diff);

		SecondNodeOptimization(&tree->root, &count_diff);

	} while (count_diff != 0);
}

void GetTangentInPoint(Tree* tree, ExprVar diff_var, double point) {

	TreeVerify(tree);

	double f_value_in_point = 0;
	SetVarInNode(tree->root, diff_var, point);
	f_value_in_point = Eval(tree->root);

	DiffExprTree(tree, diff_var);
	SetVarInNode(tree->root, diff_var, point);

	tree->root = _ADD(
					 _MUL(
						 _IMM(Eval(tree->root)),
						 _SUB(CreateVarNode(diff_var, nullptr, nullptr), _IMM(point))
  						 ),
					 _IMM(f_value_in_point)
					 );
}

void DiffExprTree(Tree* tree, ExprVar diff_var) {

	TreeVerify(tree);

	PrintLatexExpr(tree->root, tree->latex_logfile);

	Node* tmp = DiffExprNode(tree->root, diff_var, tree->latex_logfile);
	NodeDtor(tree->root);

	tree->root = tmp;

	PrintLatexExpr(tree->root, tree->latex_logfile);

}

Node* DiffExprNode(Node* node, ExprVar diff_var, FILE* latex_logfile) {

	assert(node != nullptr);
	VerifyNode(node);
	
	Node* new_node = {};

	if (node->data.type == NUM) {
		new_node = CreateImmNode(0, nullptr, nullptr);
	}

	else if (node->data.type == VAR && !isEqualVar(node->data.value.var, diff_var)) {
		new_node = CreateImmNode(0, nullptr, nullptr);
	}

	else if (node->data.type == VAR && isEqualVar(node->data.value.var, diff_var)) {
		new_node = CreateImmNode(1, nullptr, nullptr);
	}

	else {
		switch (NODE_CMD_CODE(node)) {
		#define DEF_EXPR_CMD(command_name, command_str, int_code, priority, args_num, handle, diff, ...)\
				case int_code: {                                                                        \
					diff;																				\
					break;																				\
				}
		#include "def_expr_cmd.h"
		#undef DEF_EXPR_CMD
		}
	}
	
	return new_node;

}

void DiffExprNTimes(int N, Tree* tree, ExprVar diff_var) {

	TreeVerify(tree);

	for (int i = 0; i < N; i++) {
		DiffExprTree(tree, diff_var);
	}
}

Node* Factorial(double value) {

	Node* node_var = _IMM(1);

	for (int i = 2; i <= value; i++) {
		node_var = _MUL(node_var, _IMM(i));
	}

	return node_var;
}

void SetVarInNode(Node* node, ExprVar var, double point) {

	VerifyNode(node);

	if (node == nullptr) return;

	if (node->data.type == VAR && isEqualVar(node->data.value.var, var))
		node->data.value.var.imm_value = point;

	SetVarInNode(node->left, var, point);
	SetVarInNode(node->right, var, point);
}

void TeylorExpr(Tree* expr, ExprVar var, double point) {

	TreeVerify(expr);

	Node* res = _IMM(0);
	Tree tree_diff = *expr;
	for (int i = 0; i < MAX_TEYLOR_LEN; i++) {

		tree_diff.root = CopyNode(expr->root);

		DiffExprNTimes(i, &tree_diff, var);
		ExpressionOptimization(&tree_diff);
		SetVarInNode(tree_diff.root, var, point);

		res =  _ADD(
					res,
					_DIV(
						_MUL(
							_IMM(Eval(tree_diff.root)),
							_POW(
								_SUB(
									CreateVarNode(var, nullptr, nullptr),
									_IMM(point)
									),
								_IMM(i)
								)
							),
						Factorial(i)
						)
					);
	}
	
	expr->root = res;
	ExpressionOptimization(expr);
}