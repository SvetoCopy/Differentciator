#include "RuzalLib/include/Tree.h"
#include "Differentiator.h"
#include "LatexDump.h"
#include "Optimization.h"


int main()
{ 
    const char* a = " ( ( (  . x . )  sin . ) + ( ( ( . x . ) ^ ( . 2 . ) ) cos . ) )";

    char b[1000];
    strcpy_s(b, a);

    Node* ress = {};
    ReadNodeIN(b, &ress);

    ExprVar var = {};
    var.imm_value = 0;
    var.name = "x";

    Tree tree = {};
    ExprElem start_data2 = {};
    start_data2.type = NUM;
    start_data2.value.imm_value = 0;

    TreeCtor(&tree, "aaa.txt", start_data2, "LATEX_PROJECT/latex_dump.tex");
    tree.root = ress;
    ExpressionOptimization(&tree);

    // TAYLOR
    Tree teylor_tree = {};
    teylor_tree.root = CopyNode(tree.root);
    teylor_tree.latex_logfile = tree.latex_logfile;

    ExpressionOptimization(&teylor_tree);
    PrintTreeExpr(&teylor_tree, stdout);

    TeylorExpr(&teylor_tree, var, 0);
    ExpressionOptimization(&teylor_tree);
    
    PrintTreeExpr(&teylor_tree, stdout);

    CreateExprSchedule(&tree, &teylor_tree, var, "LATEX_PROJECT/teylor.jpeg");
    AddImageLatex(&tree, "teylor.jpeg");

    // TANGENT
    Tree tangent_tree = {};
    tangent_tree.root = CopyNode(tree.root);
    tangent_tree.latex_logfile = tree.latex_logfile;

    GetTangentInPoint(&tangent_tree, var, 0);
    ExpressionOptimization(&tangent_tree);
    CreateExprSchedule(&tree, &tangent_tree, var, "LATEX_PROJECT/tangent.jpeg");
    AddImageLatex(&tree, "tangent.jpeg");

    TreeDtor(&tree);
}
