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
//           |    |  |  |  |             handle                                                  |
//           |    v  v  v  v               |                                                     |
DEF_EXPR_CMD(MUL, *, 0, 1, 2,//            |                                                     |
	left * right,                      //<-+                                                     |
	                                                                                           //|
	// (f(x) * g(x))' = f'(x) * g(x) + f(x) * g'(x)                                            //|
	return _ADD(                                                                               //|
		_MUL(DiffExpr(node->left), CopyNode(node->right)), //               <--------------------+
		_MUL(CopyNode(node->left), DiffExpr(node->right))
	);

)

DEF_EXPR_CMD(ADD, +, 1, 2, 2, 
	left + right,

	// (f(x) + g(x))' = f'(x) + g'(x) 
	return _ADD(DiffExpr(node->left), DiffExpr(node->right));

)

DEF_EXPR_CMD(SUB, -, 2, 2, 2,
	left - right,

	// (f(x) - g(x))' = f'(x) - g'(x) 
	return _SUB(DiffExpr(node->left), DiffExpr(node->right));

)

DEF_EXPR_CMD(DIV, /, 3, 1, 2,
	left / right,

	//  f(x)    f'(x) * g(x) - f(x) * g'(x)
	//  ---- =  ---------------------------
	//  g(x)              g(x)^2
	return _DIV(
		_SUB(
			_MUL(DiffExpr(node->left), CopyNode(node->right)),
			_MUL(CopyNode(node->left), DiffExpr(node->right))
		),
		_MUL(CopyNode(node->right), CopyNode(node->right))
	);

)

DEF_EXPR_CMD(POW, ^, 4, 0, 2,
	pow(left, right),

	// 1) (f(x)^C)' = C * (f(x) ^ (C - 1)) 
	if (!ÑheckVarInNode(node->right))
		return _MUL(
					_MUL(
						_IMM(NODE_IMM_VALUE(node->right)),
						_POW(
							CopyNode(node->left),
							_SUB(CopyNode(node->right), _IMM(1))
							)
						),
					DiffExpr(node->left)
					);

	// 1) (a^f(x))' = a^(f(x)) * ln(a) * f'(x)
	if (!ÑheckVarInNode(node->left))
		return _MUL(
					DiffExpr(node->right),
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
						DiffExpr(node->right),
						_LOG(
							_IMM(EXP),
							CopyNode(node->left)
							)
						),
					_DIV(
						_MUL(CopyNode(node->right), DiffExpr(node->left)),
						CopyNode(node->left)
						)
					)
			  );

)

DEF_EXPR_CMD(LOG, log, 5, 0, 2,
	LogBase(left, right),

	// (log_a(f(x)) = f'(x) / (f(x) * ln(a)) 
	return _DIV(
				DiffExpr(node->left),
				_MUL(
					CopyNode(node->right),
					_LOG(_IMM(EXP), CopyNode(node->left))
					)
			  );
	
)

DEF_EXPR_CMD(COS, cos, 6, 0, 1,
	cos(left),

	// (cos(f(x)))' = - sin(f(x)) * f'(x) 
	return	_MUL(
				_MUL(
					_IMM(-1),
					_SIN(CopyNode(node->left))
					),
				DiffExpr(node->left)
				);

)

DEF_EXPR_CMD(SIN, sin, 7, 0, 1,
	sin(left),

	// (sin(f(x)))' = cos(f(x)) * f'(x)
	return	_MUL(
				_COS(CopyNode(node->left)),
				DiffExpr(node->left)
				);

)


#undef _ADD
#undef _SUB
#undef _MUL
#undef _DIV