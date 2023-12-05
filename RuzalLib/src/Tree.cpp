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

void NodeDtor(Node* node) {
	if (node == nullptr)
		return;
	//free(node->data);

	NodeDtor(node->left);
	NodeDtor(node->right);

	node->left = nullptr;
	node->right = nullptr;
}

int TreeCtor(Tree* tree, const char* file_name, NodeInfo_t start_data) {
	if (TreeVerify(tree) == TREE_ERROR) return TREE_ERROR;

	tree->root = OpNew(start_data);
	tree->graph_logfile = fopen(file_name, "w");
	tree->size = 0;

	return 0;
}

int TreeDtor(Tree* tree) {
	assert(tree != nullptr);
	NodeDtor(tree->root);
	free(tree);
	fclose(tree->graph_logfile);
	return 0;
}

int TreeVerify(Tree* tree) {
	if (tree == nullptr) {
		fprintf(stderr, "!!!!!tree is null!!!!!\n");
		return TREE_ERROR;
	}
	return 0;
}

Node* CopyNode(const Node* node) {
	if (node == nullptr) return nullptr;

	Node* res = OpNew(node->data);

	res->left  = CopyNode(node->left);
	res->right = CopyNode(node->right);

	return res;
}