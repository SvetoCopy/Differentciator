#include "Differentiator.h"

double LogBase(double num, double base)
{
	return log(num) / log(base);
}

double Eval(const Node* node) {
	
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

#define CHECK_CMD(cmd)                            \
		node->data.type == COMMAND &&             \
		NODE_CMD_CODE(node) == cmd

#define CHECK_RIGHT_NUM(num)                      \
		node->right->data.type == NUM &&          \
		isEqual(NODE_IMM_VALUE(node->right), num) \

#define CHECK_LEFT_NUM(num)                       \
		node->left->data.type == NUM &&           \
		isEqual(NODE_IMM_VALUE(node->left), 0)    \

Node* MulNDivOptimization(Node* node, int* count_diff) {
	if (CHECK_CMD(MUL) && (CHECK_RIGHT_NUM(0) || CHECK_LEFT_NUM(0)))
	{
		(*count_diff)++;
		return CreateImmNode(0, nullptr, nullptr);
	}

	if (CHECK_CMD(MUL) || CHECK_CMD(DIV))
	{
		if (CHECK_RIGHT_NUM(1))
		{
			(*count_diff)++;
			return node->left;
		}
			
		if (CHECK_LEFT_NUM(1))
		{
			(*count_diff)++;
			return node->right;
		}
	}
}

Node* PowOptimization(Node* node, int* count_diff) {
	if (CHECK_CMD(POW)) {
		if (CHECK_RIGHT_NUM(0) || CHECK_LEFT_NUM(1))
		{
			(*count_diff)++;
			return CreateImmNode(1, nullptr, nullptr);
		}

		if (CHECK_RIGHT_NUM(1))
		{
			(*count_diff)++;
			return node->left;
		}

		if (CHECK_LEFT_NUM(0))
		{
			(*count_diff)++;
			return CreateImmNode(0, nullptr, nullptr);
		}
	}
}

Node* AddNSubOptimization(Node* node, int* count_diff) {
	if (CHECK_CMD(ADD) || CHECK_CMD(SUB))
	{
		if (CHECK_RIGHT_NUM(0))
		{
			(*count_diff)++;
			return node->left;
		}

		if (CHECK_LEFT_NUM(0))
		{
			(*count_diff)++;
			return node->right;
		}
	}
}

Node* SecondNodeOptimization(Node* node, int* count_diff) {
	if (node == nullptr)        return nullptr;
	if (node->data.type == NUM) return node;

	int count = 0;
	Node* changed_node = MulNDivOptimization(node, &count);
	if (count != 0) {
		*count_diff += count;
		return changed_node;
	}

	count = 0;
	changed_node = PowOptimization(node, &count);
	if (count != 0) {
		*count_diff += count;
		return changed_node;
	}

	count = 0;
	changed_node = AddNSubOptimization(node, &count);
	if (count != 0) {
		*count_diff += count;
		return changed_node;
	}

	node->left  = SecondNodeOptimization(node->left, count_diff);
	node->right = SecondNodeOptimization(node->right, count_diff);

	return node;
}

Node* FirstNodeOptimization(Node* node, int* count_diff) {
	if (node == nullptr)        return nullptr;
	if (node->right == nullptr) return node;
	if (node->data.type == NUM) return node;

	if (node->data.type == COMMAND   && 
		node->left->data.type == NUM && 
		node->right->data.type == NUM) {
			
		*count_diff++;
		return CreateImmNode(Eval(node), nullptr, nullptr);

	}
	
	node->left  = FirstNodeOptimization(node->left, count_diff);
	node->right = FirstNodeOptimization(node->right, count_diff);

	return node;
}

void ExpressionOptimization(Tree* tree) {
	int count_diff = 0;
	do {
		count_diff = 0;
		tree->root = FirstNodeOptimization(tree->root, &count_diff);
		tree->root = SecondNodeOptimization(tree->root, &count_diff);
	} while (count_diff != 0);
}

Node* DiffExpr(const Node* node, ExprVar diff_var) {

	assert(node != nullptr);
	
	if (node->data.type == NUM)
		return CreateImmNode(0, nullptr, nullptr);

	if (node->data.type == VAR && !isEqualVar(node->data.value.var, diff_var))
		return CreateImmNode(0, nullptr, nullptr);

	else if (node->data.type == VAR && isEqualVar(node->data.value.var, diff_var))
		return CreateImmNode(1, nullptr, nullptr);

	switch (NODE_CMD_CODE(node)) {
		#define DEF_EXPR_CMD(command_name, command_str, int_code, priority, args_num, handle, diff, ...)  \
				case int_code: {                                                                          \
					diff                                                                                  \
				}
		#include "def_expr_cmd.h"

		#undef DEF_EXPR_CMD
	}
}