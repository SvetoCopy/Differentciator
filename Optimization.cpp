#include "Optimization.h"

bool isEqual(double first, double second) {
	return abs(first - second) < EPSILON;
}

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

	if (*node == nullptr)          return;
	if ((*node)->right == nullptr) return;
	if ((*node)->data.type == NUM) return;

	if ((*node)->data.type == COMMAND &&
		(*node)->left->data.type == NUM &&
		(*node)->right->data.type == NUM) {

		*count_diff++;

		double value = Eval(*node);
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