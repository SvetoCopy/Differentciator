#ifndef LATEXDUMP_HPP
#define LATEXDUMP_HPP

#include "RuzalLib/include/Tree.h"
#include "Differentiator.h"

struct ExprLabel {
	char* name;
	char* value;
};



void PrintLatexExpr(Tree* tree, const char* latex_file_name);
#endif // !LATEXDUMP_HPP
