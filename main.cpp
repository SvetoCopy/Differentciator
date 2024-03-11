#include "RuzalLib/include/Tree.h"
#include "Differentiator.h"
#include "LatexDump.h"
#include "Optimization.h"

const int MAX_FILE_SIZE     = 1000;
const int MAX_FILE_NAME_LEN = 1000;

int main(int argc, const char* argv[])
{ 
    if (argc < 7) {
        printf("Usage: %s tree.txt tree_logfile_name.gv LATEX_PROJECT/ latex_dump.tex taylor.jpeg tangent.jpeg");
        return -1;
    }

    FILE* file = fopen(argv[1], "r");
    char a[MAX_FILE_SIZE];
    fgets(a, MAX_FILE_SIZE, file);

    Node* ress = {};
    ReadNodeIN(a, &ress);

    ExprVar var = {};
    var.imm_value = 0;
    var.name = "x";

    Tree tree = {};
    ExprElem start_data2 = {};
    start_data2.type = NUM;
    start_data2.value.imm_value = 0;

    char path_to_latex[MAX_FILE_NAME_LEN] = "";
    snprintf(path_to_latex, MAX_FILE_NAME_LEN, "%s%s", argv[3], argv[4]);

    TreeCtor(&tree, argv[2], start_data2, path_to_latex);
    tree.root = ress;
    ExpressionOptimization(&tree);

    // TAYLOR
    Tree taylor_tree = {};
    taylor_tree.root = CopyNode(tree.root);
    taylor_tree.latex_logfile = tree.latex_logfile;

    ExpressionOptimization(&taylor_tree);
    PrintTreeExpr(&taylor_tree, stdout);
    printf("\n");

    TaylorExpr(&taylor_tree, var, 0);
    ExpressionOptimization(&taylor_tree);
    
    PrintTreeExpr(&taylor_tree, stdout);

    char path_to_taylor_img[MAX_FILE_NAME_LEN] = "";
    snprintf(path_to_taylor_img, MAX_FILE_NAME_LEN, "%s%s", argv[3], argv[5]);

    CreateExprGraph(&tree, &taylor_tree, var, path_to_taylor_img);
    AddImageLatex(&tree, argv[5]);

    // TANGENT
    Tree tangent_tree = {};
    tangent_tree.root = CopyNode(tree.root);
    tangent_tree.latex_logfile = tree.latex_logfile;

    GetTangentInPoint(&tangent_tree, var, 0);
    ExpressionOptimization(&tangent_tree);

    char path_to_tangent_img[MAX_FILE_NAME_LEN] = "";
    snprintf(path_to_tangent_img, MAX_FILE_NAME_LEN, "%s%s", argv[3], argv[6]);

    CreateExprGraph(&tree, &tangent_tree, var, path_to_tangent_img);
    AddImageLatex(&tree, argv[6]);

    TreeDtor(&tree);
}
