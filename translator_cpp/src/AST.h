/*! iter3 tag test */

#ifndef Node_H
#define Node_H

#include <string>
#include <iostream>

//#include "scanner.h"

class Node ;
class Stmts;
class Stmt;
class WhileStmt;
class ForStmt;
class PrintStmt;
class Decl;
class MtrxDecl;
class StandardDecl;
class EmptyStmts;
class Expr;
class VarName;
class Root;
class BinOpExpr;
class AnyConst;

// Node
class Node {
	public:
		virtual std::string unparse ( ) { return " This should be pure virtual ";} ;
		virtual std::string cppCode ( ) { return " This should be pure virtual" ; } ;
		virtual ~Node() { };
} ;

/*! Statements class, we didn't incorporate this into the main one either
*/
class Stmts : public Node {
public:
	Stmts() {};
	Stmts(Stmt *s, Stmts *t) : statement(s), statements(t) {};
	std::string unparse();
	std::string cppCode();
private:
	Stmt *statement;
	Stmts *statements;
};

class Stmt : public Node {
public:
private:
};

class DeclStmt : public Stmt {
public:
	DeclStmt(Decl *d) : decl(d) {};
	std::string unparse();
	std::string cppCode();
private:
	Decl *decl;
};

class BracketStmt : public Stmt {
public:
	BracketStmt(Stmts *k) : inBrackets(k) {};
	std::string unparse();
	std::string cppCode();
private:
	Stmts *inBrackets;
};

class IfStmt : public Stmt {
public:
	IfStmt(Expr *e, Stmt *s) : ifExpression(e), ifStmt(s) {};
	std::string unparse();
	std::string cppCode();
private:
	Expr *ifExpression;
	Stmt *ifStmt;
};

class IfElseStmt : public Stmt {
public:
	IfElseStmt(Expr *e, Stmt *s, Stmt *t) : ifExpression(e), ifStmt(s), elseStmt(t) {};
	std::string unparse();
	std::string cppCode();
private:
	Expr *ifExpression;
	Stmt *ifStmt;
	Stmt *elseStmt;
};

class VarMatrixExpr : public Stmt {
public:
	VarMatrixExpr(VarName *v, Expr *q, Expr *z, Expr *e) : varname(v), firstMatrixExpr(q), secondMatrixExpr(z), assignExpr(e) {};
	std::string unparse();
	std::string cppCode();
private:
	VarName *varname;
	Expr *firstMatrixExpr;
	Expr *secondMatrixExpr;
	Expr *assignExpr;
};
class VarAssignExpr : public Stmt {
public:
	VarAssignExpr(VarName *v, Expr *e) : varname(v), assignExpr(e) {};
	std::string unparse();
	std::string cppCode();
private:
	VarName *varname;
	Expr *assignExpr;
};

class PrintStmt : public Stmt {
public:
	PrintStmt(Expr *e) : expr(e) {};
	std::string unparse();
	std::string cppCode();
private:
	Expr *expr;
};

class ForStmt : public Stmt {
public:
	ForStmt(VarName *v, Expr *e, Expr *d, Stmt *s) : varName(v), firstExpr(e), secondExpr(d), stmt(s) {};
	std::string unparse();
	std::string cppCode();
private:
	VarName *varName;
	Expr *firstExpr;
	Expr *secondExpr;
	Stmt *stmt;
};

class WhileStmt : public Stmt {
public:
	WhileStmt(Expr *e, Stmt *s) : expr(e), stmt(s) {};
	std::string unparse();
	std::string cppCode();
private:
	Expr *expr;
	Stmt *stmt;
};

class Colon : public Stmt {
public: 
	Colon() {};
	std::string unparse();
	std::string cppCode();
};

/*! Root class. build the origin of the program*/
class Root : public Node {
public:
	Root(VarName *v, Stmts *s) : varName(v), stmts(s) {};
	std::string unparse();
	std::string cppCode();	//THIS IS NEW!!! FROM LAB
	virtual ~Root();
private:
	VarName *varName;
	Stmts *stmts;
};

class Decl : public Node {
public:
	Decl() {};
	Decl(MtrxDecl *k) : mtrx(k) {};
	Decl(StandardDecl *d) : stdr(d) {};
	std::string unparse();
	std::string cppCode();
private:
	StandardDecl *stdr;
	MtrxDecl *mtrx;
};

class MtrxDecl : public Decl {
public:
	MtrxDecl(VarName *v, Expr *e, Expr *e2, VarName *v2, VarName *v3, Expr *e3) : var1(v), exp1(e), exp2(e2), var2(v2), var3(v3), exp3(e3) {};
	MtrxDecl(VarName *v, Expr *e) : var1(v), exp1(e) {};
	std::string unparse();
	std::string cppCode();
private:
	VarName *var1;
	Expr *exp1;
	Expr *exp2;
	VarName *var2;
	VarName *var3;
	Expr *exp3;
};

class StandardDecl : public Decl {
public: 
	StandardDecl(std::string v, std::string z) : kwd(v), varName(z) {};
	std::string unparse();
	std::string cppCode();
private:
	std::string kwd;
	std::string varName;
};

/*! This is the Expr class, we weren't sure how to incorporate this into the main iteration
*/
class Expr : public Node {
};

class VarName : public Expr {
public:
	VarName(std::string _lexeme) : lexeme(_lexeme) {};
	std::string unparse();
	std::string cppCode();
private:
	std::string lexeme;
};

class EmptyStmts : public Stmts {
public:
	EmptyStmts(){};
	std::string unparse();
	std::string cppCode();
};

/*! This class deals with the binary operation expressions, it was given to us by the class authorites

*/
class BinOpExpr : public Expr {
public:
	BinOpExpr (Expr *_left, std::string *_op, Expr *_right) : left(_left), op(_op), right(_right) {};
	std::string unparse();
	std::string cppCode();
private:
	Expr *left;
	Expr *right;
	std::string *op;
};

class MtrxExpr : public Expr {
public:
	MtrxExpr(VarName *v, Expr *e, Expr *e2) : var(v), expr1(e), expr2(e2) {};
	std::string unparse();
	std::string cppCode();
private:
	VarName *var;
	Expr *expr1;
	Expr *expr2;
};

class VarFunctExpr : public Expr {
public:
	VarFunctExpr(VarName *v, Expr *e) : var(v), expr(e) {};
	std::string unparse();
	std::string cppCode();
private:
	VarName *var;
	Expr *expr;
};

class ParenExpr : public Expr {
public:
	ParenExpr(Expr *e) : expr(e) {};
	std::string unparse();
	std::string cppCode();
private:
	Expr *expr;
};

class LetInEnd : public Expr {
public:
	LetInEnd(Stmts *s, Expr *e) : stmts(s), expr(e) {};
	std::string unparse();
	std::string cppCode();
private:
	Stmts *stmts;
	Expr *expr;
};

class IfThenElse : public Expr {
public:
	IfThenElse(Expr *e1, Expr *e2, Expr *e3) : expr1(e1), expr2(e2), expr3(e3) {};
	std::string unparse();
	std::string cppCode();
private:
	Expr *expr1;
	Expr *expr2;
	Expr *expr3;
};

class NotExpr : public Expr {
public:
	NotExpr(Expr *e) : expr(e) {};
	std::string unparse();
	std::string cppCode();
private:
	Expr *expr;
};

/*! AnyConst class, given by the class authorities
*/
class AnyConst : public Expr {
public:
	AnyConst(std::string _s) : constString(_s) {};
	std::string unparse();
	std::string cppCode();
private:
	std::string constString;
};


#endif

