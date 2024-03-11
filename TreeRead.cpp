#include "TreeRead.h"

int ReadExprElem(const char* str, Node** node) {

	assert(str != nullptr);

	double double_elem = 0;
	int    elem_read_size = 0;
	int    read_arg_count = sscanf(str, " %lf %n", &double_elem, &elem_read_size);

	if (read_arg_count == 0) {

		elem_read_size = 0;
		char str_elem[MAX_NODEINFO_SIZE] = {};
		read_arg_count = sscanf(str, " %s %n", &str_elem, &elem_read_size);

		if (strcmp(str_elem, DEFAULT_NIL) == 0) *node = nullptr;

		#define DEF_EXPR_CMD(cmd_name, str_command, cmd_code, ...) \
																   \
        else if (strcmp(str_elem, #str_command) == 0) {			   \
			ExprElem data = CreateExprCommand(cmd_code);		   \
			*node = OpNew(data);								   \
		}

		#include "def_expr_cmd.h"
		#undef DEF_EXPR_CMD

		else {
			ExprElem data = CreateExprVar(str_elem);
			*node = OpNew(data);
		}
	}
	else {
		ExprElem data = CreateExprImm(double_elem);
		*node = OpNew(data);
	}

	return elem_read_size;
}

static int DeleteCloseBracket(const char* str) {

	assert(str != nullptr);

	char symbol = {};
	int end_read_size = 0;

	sscanf(str, " %c %n", &symbol, &end_read_size);

	if (symbol == ')') return end_read_size;

	return 0;
}

#define MoveStr(num)														 \
																			 \
	res_size += num;														 \
	str      += num;   

#define ReadChild(child_name)                                                \
																			 \
	symbol = {};                                                             \
	open_bracket_size = 0;                                                   \
	sscanf(str, " %c %n", &symbol, &open_bracket_size);                      \
																			 \
	Node* child_name = {};                                                   \
	int child_name ## _size = 0;                                             \
																		     \
	if (symbol == '(') child_name ## _size = ReadNodeIN(str, &child_name);   \
	else               child_name ## _size = ReadExprElem(str, &child_name); \
		                                                                     \
	MoveStr(child_name ## _size);                                            \
		                                                                     \
	close_bracket_size = DeleteCloseBracket(str);                            \
	MoveStr(close_bracket_size);                 

int ReadNodeIN(const char* str, Node** res) {

	assert(str != nullptr);

	int res_size = 0;

	char symbol = {};
	int open_bracket_size = 0;
	sscanf(str, " %c %n", &symbol, &open_bracket_size);

	if (symbol == '(') {
		MoveStr(open_bracket_size);
	}

	int close_bracket_size = 0;
	ReadChild(left);

	int root_size = ReadExprElem(str, res);
	MoveStr(root_size);

	ReadChild(right);

	(*res)->left = left;
	(*res)->right = right;

	return res_size;
}
