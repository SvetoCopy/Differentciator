#include "RuzalLib/include/Tree.h"
#include "Differentiator.h"
#include "LatexDump.h"

int main()
{ 
    const char* a = " ( ( (  . y . )  sin . ) + ( ( ( . x . ) ^ ( . 2 . ) ) cos . ) )";

    char b[1000];
    strcpy_s(b, a);

    Node* ress = {};
    ReadNodeIN(b, &ress);
    ress->left->left->data.value.var.imm_value = 1;

    Tree tree = {};
    ExprElem start_data = {};
    start_data.type = NUM;
    start_data.value.imm_value = 0;
    TreeCtor(&tree, "aaa.txt", start_data);
    tree.root = ress;

    PrintTreeExpr(&tree);
    ExprVar var = {};
    var.imm_value = 0;
    var.name = "y";
    Node* diff_ress = DiffExpr(ress, var);

    Tree tree2 = {};
    ExprElem start_data2 = {};
    start_data2.type = NUM;
    start_data2.value.imm_value = 0;
    TreeCtor(&tree2, "aaa.txt", start_data2);
    tree2.root = diff_ress;
    printf("\n");
    PrintTreeExpr(&tree2);
    printf("\n");
    ExpressionOptimization(&tree2);
    PrintTreeExpr(&tree2);

    PrintLatexExpr(&tree2, "latex_dump.tex");

    printf("\n%lf", Eval(ress));
}
