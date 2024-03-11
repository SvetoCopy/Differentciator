#ifndef TREE_READ_HPP
#define TREE_READ_HPP

#include "RuzalLib/include/Tree.h"

const int			  MAX_NODEINFO_SIZE  = 100;
constexpr const char* DEFAULT_NIL		 = ".";

enum ParseStatus {
	FOUND = 1,
	UNFOUND = 2
};

enum Order {
	POST = 1,
	IN = 2,
	PRE = 3
};

enum NodeElem {
	LEFT_ELEM = 0,
	ROOT_ELEM = 1,
	RIGHT_ELEM = 2
};

int  ReadNodeIN(const char* str, Node** res);
#endif // !TREE_READ_HPP
