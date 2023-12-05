#define _ADD(left, ...) CreateCommandNode(ADD, left, __VA_ARGS__)
#define _SUB(left, ...) CreateCommandNode(SUB, left, __VA_ARGS__) 
#define _MUL(left, ...) CreateCommandNode(MUL, left, __VA_ARGS__)
#define _DIV(left, ...) CreateCommandNode(DIV, left, __VA_ARGS__)
#define _POW(left, ...) CreateCommandNode(POW, left, __VA_ARGS__)
#define _LOG(left, ...) CreateCommandNode(LOG, left, __VA_ARGS__)
#define _COS(left)      CreateCommandNode(COS, left, nullptr)
#define _SIN(left)      CreateCommandNode(SIN, left, nullptr)

#define _IMM(num) CreateImmNode(num, nullptr, nullptr)

//         name      priority
//           |          |  +-------------- args_num
//           |          |  |                          Differentiation----------------------------+
//      cmd -|----+  +--|--- cmd code                                                            |
//           |    |  |  |  |             handle                            latex dump            |
//           |    v  v  v  v               |                                   |                 |
DEF_EXPR_CMD(MUL, *, 0, 1, 2,//            |								   |                 |
	left * right,                      //<-+                                   |                 |
	                                                                //		   |                 |
	// (f(x) * g(x))' = f'(x) * g(x) + f(x) * g'(x)                            |                 |
	return _ADD(                                                    //         |                 |
		_MUL(DiffExpr(node->left, diff_var), CopyNode(node->right)),// <-------|-----------------+
		_MUL(CopyNode(node->left), DiffExpr(node->right, diff_var))//		   |                          
	); ,															//         |
																	//         |
 	PrintLatexNode(child->left, child, file); //                               |
	fprintf(file, " \\cdot ");                // <-----------------------------+
	PrintLatexNode(child->right, child, file);//

)

DEF_EXPR_CMD(ADD, +, 1, 2, 2, 
	left + right,

	// (f(x) + g(x))' = f'(x) + g'(x) 
	return _ADD(DiffExpr(node->left, diff_var), DiffExpr(node->right, diff_var)); ,

	PrintLatexNode(child->left, child, file);
	fprintf(file, " + ");
	PrintLatexNode(child->right, child, file);

)

DEF_EXPR_CMD(SUB, -, 2, 2, 2,
	left - right,

	// (f(x) - g(x))' = f'(x) - g'(x) 
	return _SUB(DiffExpr(node->left, diff_var), DiffExpr(node->right, diff_var)); ,

	PrintLatexNode(child->left, child, file);
	fprintf(file, " - ");
	PrintLatexNode(child->right, child, file);

)

DEF_EXPR_CMD(DIV, /, 3, 1, 2,
	left / right,

	//  f(x)    f'(x) * g(x) - f(x) * g'(x)
	//  ---- =  ---------------------------
	//  g(x)              g(x)^2
	return _DIV(
		_SUB(
			_MUL(DiffExpr(node->left, diff_var), CopyNode(node->right)),
			_MUL(CopyNode(node->left), DiffExpr(node->right, diff_var))
		),
		_MUL(CopyNode(node->right), CopyNode(node->right))
	); ,

	fprintf(file, " \\dfrac{");
	PrintLatexNode(child->left, child, file);
	fprintf(file, " }{ ");
	PrintLatexNode(child->right, child, file);
	fprintf(file, " } ");

)

DEF_EXPR_CMD(POW, ^, 4, 0, 2,
	pow(left, right),

	// 1) (f(x)^C)' = C * (f(x) ^ (C - 1)) 
	if (!ÑheckVarInNode(node->right, diff_var))
		return _MUL(
					_MUL(
						_IMM(NODE_IMM_VALUE(node->right)),
						_POW(
							CopyNode(node->left),
							_SUB(CopyNode(node->right), _IMM(1))
							)
						),
					DiffExpr(node->left, diff_var)
					);

	// 1) (a^f(x))' = a^(f(x)) * ln(a) * f'(x)
	if (!ÑheckVarInNode(node->left, diff_var))
		return _MUL(
					DiffExpr(node->right, diff_var),
					_MUL(
						CopyNode(node),
						_LOG(_IMM(EXP), CopyNode(node->left))
						)
				   );
	
	// 1) (f(x)^g(x))' =  f(x)^g(x) * (g'(x) * ln(f(x)) + g(x) * ( f'(x) / f(x) ) )
	return _MUL(
				CopyNode(node),
				_ADD(
					_MUL(
						DiffExpr(node->right, diff_var),
						_LOG(
							_IMM(EXP),
							CopyNode(node->left)
							)
						),
					_DIV(
						_MUL(CopyNode(node->right), DiffExpr(node->left, diff_var)),
						CopyNode(node->left)
						)
					)
			  ); ,

	if (isSqrtExpr(child)) {
				fprintf(file, " \\sqrt[");
				PrintLatexNode(child->right, child, file);
				fprintf(file, " ]{ ");
				PrintLatexNode(child->left, child, file);
				fprintf(file, " } ");
			}
			else {
				fprintf(file, " { ");
				PrintLatexNode(child->left, child, file);
				fprintf(file, " } ^ {\\small ");
				PrintLatexNode(child->right, child, file);
				fprintf(file, " } ");
				break;
			}

)

DEF_EXPR_CMD(LOG, log, 5, 0, 2,
	LogBase(left, right),

	// (log_a(f(x)) = f'(x) / (f(x) * ln(a)) 
	return _DIV(
				DiffExpr(node->left, diff_var),
				_MUL(
					CopyNode(node->right),
					_LOG(_IMM(EXP), CopyNode(node->left))
					)
			  ); ,

	fprintf(file, " \log_{ ");
	PrintLatexNode(child->left, child, file);
	fprintf(file, " }{ ");
	PrintLatexNode(child->right, child, file);
	fprintf(file, " } ");
	
)

DEF_EXPR_CMD(COS, cos, 6, 0, 1,
	cos(left),

	// (cos(f(x)))' = - sin(f(x)) * f'(x) 
	return	_MUL(
				_MUL(
					_IMM(-1),
					_SIN(CopyNode(node->left))
					),
				DiffExpr(node->left, diff_var)
				); ,

	fprintf(file, " \cos( ");
	PrintLatexNode(child->left, child, file);
	fprintf(file, " ) ");

)

DEF_EXPR_CMD(SIN, sin, 7, 0, 1,
	sin(left),

	// (sin(f(x)))' = cos(f(x)) * f'(x)
	return	_MUL(
				_COS(CopyNode(node->left)),
				DiffExpr(node->left, diff_var)
				); ,

	fprintf(file, " \sin( ");
	PrintLatexNode(child->left, child, file);
	fprintf(file, " ) ");

)


#undef _ADD
#undef _SUB
#undef _MUL
#undef _DIV