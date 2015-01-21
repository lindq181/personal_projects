#include "AST.h"
//#include "Matrix.h"
#include <string>
#include<algorithm>
using namespace std;


Root::~Root() {}

std::string Root::unparse() {
	return varName->unparse() + " () {" + stmts->unparse() + "}";
}

std::string Root::cppCode() {
	return "#include <iostream>\n#include \"Matrix.h\"\n#include <math.h>\nusing namespace std;\nint " + varName->cppCode() + "() {" + "\n" + stmts->cppCode() + "\n" + "}";
}

std::string StandardDecl::unparse() {
	//std::string temp = std::transform(kwd.begin(), kwd.end(), kwd.begin(), ::tolower);
	return kwd + " " + varName + ";";
}

std::string StandardDecl::cppCode() {
	std::string temp = "";
	if (kwd == "Int") {
		temp += "int";
	} else if (kwd == "Float") {
		temp += "float";
	} else if (kwd == "Str") {
		temp += "string";
	} else {
		temp += "while";
	}
	return temp + " " + varName + ";\n";
}

std::string MtrxDecl::unparse() {
	if (var2) {
		return "Matrix " + var1->unparse() + "[" + exp1->unparse() + "," + exp2->unparse() + "]" + var2->unparse() + "," + var3->unparse() + " = " + exp3->unparse() + ";";
	} else {
		return "Matrix " + var1->unparse() + " = " + exp1->unparse() + ";";
	}
}

std::string MtrxDecl::cppCode(){
	if (var2){
		return "Matrix " + var1->cppCode() + "(" + exp1->cppCode() + "," + exp2->cppCode() + ");\n"
		 + " for(int i = 0; i < " + exp1->cppCode() + "; i++){\n\t for(rint j=0; j< " 
		+ exp2->cppCode() + "; j++){\n\t\t " + "*("+  var1->cppCode() + ".access (i,j)) =  (" + exp3->cppCode() + ");\n }\n}";
		
	}
	else{
		return "Matrix " + var1->cppCode() + " = " + exp1->cppCode() + ";";
	}
} 

////cppCode for Root class
////NEW FROM LAB
//std::string Root::cppCode(){
//	return "#include <iostrean>\n#include \"Matrix.h\"\n#include <math.h>\nusing namespace std; \nint main() {\n"+
//	stmts->cppCode() +
//	"\n}\n";
//}


std::string VarName::unparse () { return lexeme;}

std::string VarName::cppCode () {return lexeme;}

std::string Decl::unparse() {
	if (stdr) {
		return stdr->unparse();
	} else if (mtrx) {
		return mtrx->unparse();
	}
}

std::string Decl::cppCode() {
	if (stdr) {
		return stdr->cppCode();
	} else if (mtrx) {
		return mtrx->cppCode();
	}
}

std::string EmptyStmts::unparse() {
	return "";
}

std::string EmptyStmts::cppCode() {
	return "";
}

std::string Stmts::unparse() {
		return statement->unparse() + " " + statements->unparse();
}

std::string Stmts::cppCode() {
		return statement->cppCode() + "\n" + statements->cppCode();
}

std::string DeclStmt::unparse() {
	return decl->unparse();
}

std::string DeclStmt::cppCode() {
	return decl->cppCode();
}

std::string BracketStmt::unparse() {
	return "{" + inBrackets->unparse() + "}";
}

std::string BracketStmt::cppCode() {
	return "{\n" + inBrackets->cppCode() + "\n}";
}

std::string IfStmt::unparse() {
	return "if (" + ifExpression->unparse() + ") " + ifStmt->unparse();
}

std::string IfStmt::cppCode(){
	return "if (" + ifExpression->cppCode() + ") \n\t " + ifStmt->cppCode();
}

std::string IfElseStmt::unparse() {
	return "if (" + ifExpression->unparse() + ") " + ifStmt->unparse() + " else " + elseStmt->unparse();
}

std::string IfElseStmt::cppCode(){
	return "if (" + ifExpression->cppCode() + ") \n\t" + ifStmt->cppCode() + "\n" + "else \n\t" + elseStmt->cppCode();
}

std::string VarMatrixExpr::unparse() {
	return varname->unparse() + "[ " + firstMatrixExpr->unparse() + " , " + secondMatrixExpr->unparse() + " ]" + " = " + assignExpr->unparse() + ";";
}

std::string VarMatrixExpr::cppCode() {
	return varname->cppCode() + "( " + firstMatrixExpr->cppCode() + " , " + secondMatrixExpr->cppCode() + " )" + " = " + assignExpr->cppCode() + ";";
}

std::string VarAssignExpr::unparse() {
	return varname->unparse() + " = " + assignExpr->unparse() + ";";
}

std::string VarAssignExpr::cppCode() {
	return varname->cppCode() + " = " + assignExpr->cppCode() + ";\n";
}

std::string Colon::unparse() {
	return ";";
}

std::string Colon::cppCode() {
	return ";";
}

std::string WhileStmt::unparse() {
	return "while (" + expr->unparse() + ")" + stmt->unparse();
}

std::string WhileStmt::cppCode() {
	return "while (" + expr->cppCode() + ") \n \t" + stmt->cppCode();
}

std::string ForStmt::unparse() {
	return "for (" + varName->unparse() + " = " + firstExpr->unparse() + ":" + secondExpr->unparse() + ")" + stmt->unparse();
}

std::string ForStmt::cppCode() {
	std::string temp = varName->cppCode();

	return "for (" + temp + " = " + firstExpr->cppCode() + ";" + temp + " <= " + secondExpr->cppCode() + ";" + temp + "++ " + ")" + stmt->cppCode() + "\n";
}

std::string PrintStmt::unparse() {
	return "print (" + expr->unparse() + ") ;";
}

std::string PrintStmt::cppCode() {
	return "cout << " + expr->cppCode() + ";\n";
}

std::string BinOpExpr::unparse() {

	return "(" + left->unparse() + " " + *op + " " + right->unparse() + ")";
}

std::string BinOpExpr::cppCode() {

	return "(" + left->cppCode() + " " + *op + " " + right->cppCode() + ")";
}

std::string MtrxExpr::unparse() {
	return var->unparse() + "[" + expr1->unparse() + "," + expr2->unparse() + "]";
}

std::string MtrxExpr::cppCode() {
	return var->cppCode() + "(" + expr1->cppCode() + "," + expr2->cppCode() + ")";
}

std::string VarFunctExpr::unparse() {
	return var->unparse() + "(" + expr->unparse() + ")";
}

std::string VarFunctExpr::cppCode() {
	return var->cppCode() + "(" + expr->cppCode() + ")";
}

std::string ParenExpr::unparse() {
	return expr->unparse();
}

std::string ParenExpr::cppCode() {
	return "(" + expr->cppCode() + ");\n";
}

std::string LetInEnd::unparse() {
	return "let " + stmts->unparse() + " in " + expr->unparse() + " end ";
}

std::string LetInEnd::cppCode() {
	return "({ " + stmts->cppCode() + "\n" + expr->cppCode() + ";})\n";
}

std::string IfThenElse::unparse() {
	return "if " + expr1->unparse() + " then " + expr2->unparse() + " else " + expr3->unparse();
}

std::string IfThenElse::cppCode() {
	return "if " + expr1->cppCode() + " then " + expr2->cppCode() + "\n" + "else " + expr3->cppCode();
}

std::string NotExpr::unparse() {
	return "!" + expr->unparse();
}

std::string NotExpr::cppCode() {
	return "!" + expr->cppCode();
}

std::string AnyConst::unparse() {
	return constString;
}

std::string AnyConst::cppCode() {
	return constString;
}

