/*! A recursive descent parser with operator precedence.

   Author: Eric Van Wyk
   Modified: Robert Edge        

   This algorithm is based on the work of Douglas Crockford in "Top
   Down Operator Precedence", a chapter in the book "Beautiful Code".
   Mr. Crockford describes in his chapter how his work is based on an
   algorithm described Vaughan Pratt in "Top Down Operator
   Precedence", presented at the ACM Symposium on Principles of
   Programming Languages.

   Douglas Crockford's chapter is available at 
    http://javascript.crockford.com/tdop/tdop.html

   Vaughan Pratt's paper is available at 
    http://portal.acm.org/citation.cfm?id=512931

   These are both quite interesting works and worth reading if you
   find the problem of parsing to be an interesting one.

   Last modified: Oct 13, 2013.

*/

#include "parser.h"
#include "scanner.h"
#include <stdbool.h>
#include "extToken.h"
#include <stdio.h>
#include <assert.h>
#include "AST.h"
using namespace std ;

Parser::~Parser() {
    if (s) delete s ;

    ExtToken *extTokenToDelete ;
    currToken = tokens ;
    while (currToken) {
        extTokenToDelete = currToken ;
        currToken = currToken->next ;
        delete extTokenToDelete ;
    }

    Token *tokenToDelete ;
    Token *currScannerToken = stokens ;
    while (currScannerToken) {
        tokenToDelete = currScannerToken ;
        currScannerToken = currScannerToken->next ;
        delete tokenToDelete ;
    }

}



Parser::Parser ( ) { 
    currToken = NULL; prevToken = NULL ; tokens = NULL; 
    s = NULL; stokens = NULL; 
}

ParseResult Parser::parse (const char *text) {
    assert (text != NULL) ;

    ParseResult pr ;
    try {
        s = new Scanner() ;
        stokens = s->scan (text) ;        
        tokens = extendTokenList ( this, stokens ) ;

        assert (tokens != NULL) ;
        currToken = tokens ;
        pr = parseProgram( ) ;
    }
    catch (string errMsg) {
        pr.ok = false ;
        pr.errors = errMsg ;
        pr.ast = NULL ;
    }
    return pr ;
}

/*! 
  parse methods for non-terminal symbols
  --------------------------------------
 */


// Program
//note: I uncommented some of the changes. they have // after them so we know where they are
ParseResult Parser::parseProgram () {
    ParseResult pr ;
    // root
//    Program ::= varName '(' ')' '{' Stmts '}'	
    match(variableName) ;
    string name( prevToken->lexeme ) ;

    VarName *varname = new VarName(name);	// looks correct by lec slides

    match(leftParen) ;
    match(rightParen) ;
    match(leftCurly);
    ParseResult prStmts = parseStmts() ;

	Stmts *s = NULL;
	if (prStmts.ast) {	//
		s = dynamic_cast<Stmts *>(prStmts.ast);	//
		if (!s) throw ((string)"Bad cast of Stmts state in parseProgram"); //
	}	


    match(rightCurly);
    match(endOfFile) ;

	pr.ast = new Root(varname,s);	//
    
    return pr ;
}


// MatrixDecl
// identical purpose of parseDecl, handles special matrix syntax.
ParseResult Parser::parseMatrixDecl () {
    ParseResult pr ;
    match(matrixKwd);
    match(variableName) ;
	string name1( prevToken->lexeme ) ;
	VarName *var1 = new VarName(name1);

    // Decl ::= 'Matrix' varName '[' Expr ',' Expr ']' varName ',' varName  '=' Expr ';'
    if(attemptMatch(leftSquare)){
        ParseResult prExpr = parseExpr(0);
	Expr *e1 = NULL;

	if (prExpr.ast) {
		e1 = dynamic_cast<Expr *>(prExpr.ast);
		if (!e1) throw ((string) "Bad cast of Expr state in \"parseStmt\" in parseProgram");
	}
        match(comma);
        prExpr = parseExpr(0);
	Expr *e2 = NULL;

	if (prExpr.ast) {
		e2 = dynamic_cast<Expr *>(prExpr.ast);
		if (!e2) throw ((string) "Bad cast of Expr state in \"parseStmt\" in parseProgram");
	}
        match(rightSquare);
        parseVariableName();
	string name2(prevToken->lexeme);
	VarName *var2 = new VarName(name2);
        match(comma);
        parseVariableName();
	string name3(prevToken->lexeme);
	VarName *var3 = new VarName(name3);
        match(assign);
        prExpr = parseExpr(0);
	Expr *e3 = NULL;

	if (prExpr.ast) {
		e3 = dynamic_cast<Expr *>(prExpr.ast);
		if (!e3) throw ((string) "Bad cast of Expr state in \"parseStmt\" in parseProgram");
	}

	pr.ast = new MtrxDecl(var1, e1, e2, var2, var3, e3);
    }
    // Decl ::= 'Matrix' varName '=' Expr ';'
    else if(attemptMatch(assign)){
        ParseResult prExpr = parseExpr(0);
	Expr *e = NULL;

	if (prExpr.ast) {
		e = dynamic_cast<Expr *>(prExpr.ast);
		if (!e) throw ((string) "Bad cast of Expr state in \"parseStmt\" in parseProgram");
	}

	pr.ast = new MtrxDecl(var1, e);
    }
    else{
        throw ( (string) "Bad Syntax of Matrix Decl in in parseMatrixDecl" ) ;
    }   

    match(semiColon) ;

    return pr ;
}
//standardDecl 
//Decl ::= integerKwd varName | floatKwd varName | stringKwd varName
ParseResult Parser::parseStandardDecl(){
    ParseResult pr ;
	//dynamic_cast<standardDecl *>(pr.ast);
    std::string trial = "";
    //ParseResult prType = parseType() ;
    if ( attemptMatch(intKwd) ) {
        // Type ::= intKwd
    } 
    else if ( attemptMatch(floatKwd) ) {
        // Type ::= floatKwd
    }
    else if ( attemptMatch(stringKwd) ) {
        // Type ::= stringKwd
    }
    else if ( attemptMatch(boolKwd) ) {
        // Type ::= boolKwd
    }
	trial = prevToken->lexeme;
    match(variableName) ;
	pr.ast = new StandardDecl(trial, prevToken->lexeme);
    match(semiColon) ;
    return pr ;
}

// Decl
ParseResult Parser::parseDecl () {
    ParseResult pr ;
    // Decl :: Matrix variableName ....
    if(nextIs(matrixKwd)){
        //pr = parseMatrixDecl();
	ParseResult mDecl = parseMatrixDecl();

	MtrxDecl *k = NULL;

	if (mDecl.ast) {	//
		k = dynamic_cast<MtrxDecl *>(mDecl.ast);	//
		if (!k) throw ((string)"Bad cast of Std Decl state in parseProgram"); //
	}

	pr.ast = new Decl(k);
    } 
    // Decl ::= Type variableName semiColon
    else{
	//pr.ast = Decl();
        ParseResult stdrDecl = parseStandardDecl();
        //pr = parseStandardDecl();

	StandardDecl *d = NULL;

	if (stdrDecl.ast) {	//
		d = dynamic_cast<StandardDecl *>(stdrDecl.ast);	//
		if (!d) throw ((string)"Bad cast of Std Decl state in parseProgram"); //
	}

	pr.ast = new Decl(d);
    }
    return pr ;
}



// Stmts
ParseResult Parser::parseStmts () {
    ParseResult pr ;
    if ( ! nextIs(rightCurly) && !nextIs(inKwd)  ) {
        // Stmts ::= Stmt Stmts
	//pr.ast = Stmts;
        ParseResult prStmt = parseStmt() ;

	Stmt *s = NULL;	//looks good
	if (prStmt.ast) {	//
		s = dynamic_cast<Stmt *>(prStmt.ast);	//
		if (!s) throw ((string)"Bad cast of Stmt in \"parseStmts\" state in parseProgram"); //
	}

        ParseResult prStmts = parseStmts() ;

	Stmts *t = NULL;	//looks good
	if (prStmts.ast) {	//
		t = dynamic_cast<Stmts *>(prStmts.ast);	//
		if (!t) throw ((string)"Bad cast of Stmts in \"parseStmts\" state in parseProgram"); //
	}

	pr.ast = new Stmts(s, t); // build Stmts constructor
    }
    else {
        // Stmts ::= 
        // nothing to match.
	pr.ast = new EmptyStmts();
    }
    return pr ;
}


// Stmt
ParseResult Parser::parseStmt () {
    ParseResult pr ;
	bool test = false;

    //Stmt ::= Decl
    if(nextIs(intKwd)||nextIs(floatKwd)||nextIs(matrixKwd)||nextIs(stringKwd)||nextIs(boolKwd)){
        ParseResult declStmt = parseDecl();

	Decl *d = NULL;	//looks good

	if (declStmt.ast) {	//
		d = dynamic_cast<Decl *>(declStmt.ast);	//
		if (!d) throw ((string)"Bad cast of Decl state in parseProgram"); //
	}

	pr.ast = new DeclStmt(d); 
	//pr = parseDecl();
    }
    //Stmt ::= '{' Stmts '}'
    else if (attemptMatch(leftCurly)){
        ParseResult prStmts = parseStmts();

	Stmts *s = NULL;

	if (prStmts.ast) {
		s = dynamic_cast<Stmts *>(prStmts.ast);
		if (!s) throw ((string) "Bad cast of Stmts state in \"parseStmt\" in parseProgram");
	}
 
        match(rightCurly);

	pr.ast = new BracketStmt(s);
    }   
    //Stmt ::= 'if' '(' Expr ')' Stmt
    //Stmt ::= 'if' '(' Expr ')' Stmt 'else' Stmt
    else if (attemptMatch(ifKwd)){
        match(leftParen);

	ParseResult prExpr = parseExpr(0);
	Expr *e = NULL;

	if (prExpr.ast) {
		e = dynamic_cast<Expr *>(prExpr.ast);
		if (!e) throw ((string) "Bad cast of Expr state in \"parseStmt\" in parseProgram");
	}

        match(rightParen);

        ParseResult prStmt = parseStmt();
	Stmt *s = NULL;

	if (prStmt.ast) {
		s = dynamic_cast<Stmt *>(prStmt.ast);
		if (!s) throw ((string) "Bad cast of Stmt2 state in \"parseStmt\" in parseProgram");
	}
        
        if(attemptMatch(elseKwd)){
        	ParseResult prStmt2 = parseStmt();

		Stmt *s2 = NULL;

		if (prStmt2.ast) {
			s2 = dynamic_cast<Stmt *>(prStmt2.ast);
			if (!s2) throw ((string) "Bad cast of Stmt2 state in \"parseStmt\" in parseProgram");
		}

		pr.ast = new IfElseStmt(e, s, s2);
        } else {
		pr.ast = new IfStmt(e, s);
	}
    }
    //Stmt ::= varName '=' Expr ';'  | varName '[' Expr ',' Expr ']' '=' Expr ';'
    else if  ( attemptMatch (variableName) ) {

    	string name( prevToken->lexeme ) ;
	VarName *varName = new VarName(name);
	ParseResult prExpr;
	Expr *q = NULL;
	Expr *z = NULL;

        if (attemptMatch ( leftSquare ) ) {
		test = true;
              prExpr = parseExpr(0);

		if (prExpr.ast) {
			q = dynamic_cast<Expr *>(prExpr.ast);
			if (!q) throw ((string) "Bad cast of Expr state in \"parseStmt\" in parseProgram");
		}
              match ( comma ) ;
              prExpr = parseExpr (0) ;

		if (prExpr.ast) {
			z = dynamic_cast<Expr *>(prExpr.ast);
			if (!z) throw ((string) "Bad cast of Expr state in \"parseStmt\" in parseProgram");
		}
              match  ( rightSquare ) ;
        }
        match(assign);

        prExpr = parseExpr(0);
	Expr *e = NULL;

	if (prExpr.ast) {
		e = dynamic_cast<Expr *>(prExpr.ast);
		if (!e) throw ((string) "Bad cast of Expr state in \"parseStmt\" in parseProgram");
	}
	//if the boolean marker is set, meaning it is a matrix
	if (test) {
		pr.ast = new VarMatrixExpr(varName, q, z, e);
	} else {
		pr.ast = new VarAssignExpr(varName, e);
	}
	

        match(semiColon);

    }
    //Stmt ::= 'print' '(' Expr ')' ';'
    else if ( attemptMatch (printKwd) ) {
        match (leftParen) ;
        ParseResult prExpr = parseExpr(0);
	Expr *e = NULL;

	if (prExpr.ast) {
		e = dynamic_cast<Expr *>(prExpr.ast);
		if (!e) throw ((string) "Bad cast of Expr state in \"parseStmt\" in parseProgram");
	}
        match (rightParen) ;
        match (semiColon) ;
	pr.ast = new PrintStmt(e);
    }
    //Stmt ::= 'for' '(' varName '=' Expr ':' Expr ')' Stmt
    else if ( attemptMatch (forKwd) ) {
        match (leftParen) ;
        match (variableName) ;
		string name( prevToken->lexeme ) ;
		VarName *varName = new VarName(name);
        match (assign) ;
		ParseResult prExpr = parseExpr (0) ;
		Expr *e = NULL;

		if (prExpr.ast) {
			e = dynamic_cast<Expr *>(prExpr.ast);
			if (!e) throw ((string) "Bad cast of Expr state in \"parseStmt\" in parseProgram");
		}
        match (colon) ;
        prExpr = parseExpr (0) ;
		Expr *e2 = NULL;

		if (prExpr.ast) {
			e2 = dynamic_cast<Expr *>(prExpr.ast);
			if (!e2) throw ((string) "Bad cast of Expr state in \"parseStmt\" in parseProgram");
		}
        match (rightParen) ;
		ParseResult prStmt = parseStmt();
		Stmt *s = NULL;

		if (prStmt.ast) {
			s = dynamic_cast<Stmt *>(prStmt.ast);
			if (!s) throw ((string) "Bad cast of Stmt2 state in \"parseStmt\" in parseProgram");
		}
	pr.ast = new ForStmt(varName, e, e2, s);
    }
    //Stmt ::= 'while' '(' Expr ')' Stmt
    else if (attemptMatch(whileKwd)) {
        match(leftParen);
        ParseResult prExpr = parseExpr(0);
	Expr *e = NULL;

	if (prExpr.ast) {
		e = dynamic_cast<Expr *>(prExpr.ast);
		if (!e) throw ((string) "Bad cast of Expr state in \"parseStmt\" in parseProgram");
	}
        match(rightParen);

        ParseResult prStmt = parseStmt();
	Stmt *s = NULL;

	if (prStmt.ast) {
		s = dynamic_cast<Stmt *>(prStmt.ast);
		if (!s) throw ((string) "Bad cast of Stmt2 state in \"parseStmt\" in parseProgram");
	}
	pr.ast = new WhileStmt(e, s);
    }
    //Stmt ::= ';
    else if ( attemptMatch (semiColon) ) {
        // parsed a skip
	pr.ast = new Colon();
    }
    else{
        throw ( makeErrorMsg ( currToken->terminal ) + " while parsing a statement" ) ;
    }
    // Stmt ::= variableName assign Expr semiColon
    return pr ;
}



// Expr
ParseResult Parser::parseExpr (int rbp) {
    /* Examine current token, without consuming it, to call its
       associated parse methods.  The ExtToken objects have 'nud' and
       'led' methods that are dispatchers that call the appropriate
       parse methods.*/
    ParseResult left = currToken->nud() ;
   
    while (rbp < currToken->lbp() ) {
        left = currToken->led(left) ;
    }	
//	Expr *e = NULL;

//	if (left.ast) {
//		e = dynamic_cast<Expr *>(left.ast);
//		if (!e) throw ((string) "Bad cast of Expr state in \"parseStmt\" in parseProgram");
//	}

//	left.ast = new Expr(e);

    return left ;
}


/*
 * parse methods for Expr productions
 * ----------------------------------
 */

 // Expr ::= trueKwd
 ParseResult Parser::parseTrueKwd ( ) {
     ParseResult pr ;
     match ( trueKwd ) ;
	pr.ast = new AnyConst(prevToken->lexeme);
     return pr ;
 }

 // Expr ::= trueKwd
 ParseResult Parser::parseFalseKwd ( ) {
     ParseResult pr ;
     match ( falseKwd ) ;
	pr.ast = new AnyConst(prevToken->lexeme);
     return pr ;
 }

// Expr ::= intConst
ParseResult Parser::parseIntConst ( ) {
    ParseResult pr ;
    match ( intConst ) ;
	pr.ast = new AnyConst(prevToken->lexeme);
    return pr ;
}

// Expr ::= floatConst
ParseResult Parser::parseFloatConst ( ) {
    ParseResult pr ;
    match ( floatConst ) ;
	pr.ast = new AnyConst(prevToken->lexeme);
    return pr ;
}

// Expr ::= stringConst
ParseResult Parser::parseStringConst ( ) {
    ParseResult pr ;
    match ( stringConst ) ;
	pr.ast = new AnyConst(prevToken->lexeme);
    return pr ;
}

// Expr ::= variableName .....
ParseResult Parser::parseVariableName ( ) {
    ParseResult pr ;
    match ( variableName ) ;
    string name( prevToken->lexeme ) ;
	
    if(attemptMatch(leftSquare)){
		VarName *varName = new VarName(name);
	ParseResult prExpr = parseExpr(0);
		Expr *e1 = NULL;

		if (prExpr.ast) {
			e1 = dynamic_cast<Expr *>(prExpr.ast);
			if (!e1) throw ((string) "Bad cast of Expr state in \"parseStmt\" in parseProgram");
		}
        match(comma);
       	prExpr = parseExpr(0);
		Expr *e2 = NULL;

		if (prExpr.ast) {
			e2 = dynamic_cast<Expr *>(prExpr.ast);
			if (!e2) throw ((string) "Bad cast of Expr state in \"parseStmt\" in parseProgram");
		}
        match(rightSquare);
	pr.ast = new MtrxExpr(varName, e1, e2);
    }
    //Expr ::= varableName '(' Expr ')'        //NestedOrFunctionCall
    else if(attemptMatch(leftParen)){
	VarName *varName = new VarName(name);
        ParseResult prExpr = parseExpr(0);
		Expr *e = NULL;

		if (prExpr.ast) {
			e = dynamic_cast<Expr *>(prExpr.ast);
			if (!e) throw ((string) "Bad cast of Expr state in \"parseStmt\" in parseProgram");
		}
        match(rightParen);
	pr.ast = new VarFunctExpr(varName, e);
    }
    //Expr := variableName
    else{
        // variable 
	pr.ast = new VarName(name);
    }
    return pr ;
}


// Expr ::= leftParen Expr rightParen
ParseResult Parser::parseNestedExpr ( ) {
    ParseResult pr ;
    match ( leftParen ) ;
//    pr = parseExpr(0) ; 
 	ParseResult prExpr = parseExpr(0);
		Expr *e = NULL;

		if (prExpr.ast) {
			e = dynamic_cast<Expr *>(prExpr.ast);
			if (!e) throw ((string) "Bad cast of Expr state in \"parseStmt\" in parseProgram");
		}
	pr.ast = new ParenExpr(e);
    match(rightParen) ;
    return pr ;
}

//Expr ::= 'if' Expr 'then' Expr 'else' Expr  
ParseResult Parser::parseIfExpr(){  
     ParseResult pr ;
	ParseResult prExpr; 
    
    match(ifKwd);
    prExpr = parseExpr(0);
		Expr *e1 = NULL;

		if (prExpr.ast) {
			e1 = dynamic_cast<Expr *>(prExpr.ast);
			if (!e1) throw ((string) "Bad cast of Expr state in \"parseStmt\" in parseProgram");
		}
    match(thenKwd);
    prExpr = parseExpr(0);
		Expr *e2 = NULL;

		if (prExpr.ast) {
			e2 = dynamic_cast<Expr *>(prExpr.ast);
			if (!e2) throw ((string) "Bad cast of Expr state in \"parseStmt\" in parseProgram");
		}
    match(elseKwd);
    prExpr = parseExpr(0);
		Expr *e3 = NULL;

		if (prExpr.ast) {
			e3 = dynamic_cast<Expr *>(prExpr.ast);
			if (!e3) throw ((string) "Bad cast of Expr state in \"parseStmt\" in parseProgram");
		}
	pr.ast = new IfThenElse(e1, e2, e3);
    return pr;
}


// Expr ::= 'let' Stmts 'in' Expr 'end' 
ParseResult Parser::parseLetExpr(){
   ParseResult pr ;
	ParseResult parseIt;
   match(letKwd);
   parseIt = parseStmts();
	Stmts *s = NULL;

	if (parseIt.ast) {
		s = dynamic_cast<Stmts *>(parseIt.ast);
		if (!s) throw ((string) "Bad cast of Stmt2 state in \"parseStmt\" in parseProgram");
	}
   match(inKwd);
   parseIt = parseExpr(0);
		Expr *e = NULL;

		if (parseIt.ast) {
			e = dynamic_cast<Expr *>(parseIt.ast);
			if (!e) throw ((string) "Bad cast of Expr state in \"parseStmt\" in parseProgram");
		}
   match(endKwd);

	pr.ast = new LetInEnd(s, e);
   return pr;
}

// Expr ::= '!' Expr 
ParseResult Parser::parseNotExpr () {
    ParseResult pr ;
    match ( notOp ) ;
    ParseResult prExpr = parseExpr( 0 );
		Expr *e = NULL;

		if (prExpr.ast) {
			e = dynamic_cast<Expr *>(prExpr.ast);
			if (!e) throw ((string) "Bad cast of Expr state in \"parseStmt\" in parseProgram");
		}
	pr.ast = new NotExpr(e);
    return pr ;

}
// Expr ::= Expr plusSign Expr
ParseResult Parser::parseAddition ( ParseResult prLeft ) {
    // parser has already matched left expression 
    ParseResult pr ;
	//do same for right Expr
	 Expr *left = dynamic_cast<Expr *>(prLeft.ast);
    match ( plusSign ) ;
	 string *op = new string(prevToken->lexeme);
    ParseResult prRight = parseExpr( prevToken->lbp() );
	Expr *right = dynamic_cast<Expr *>(prRight.ast);
	pr.ast = new BinOpExpr(left, op, right); 
    return pr ;
}

// Expr ::= Expr star Expr
ParseResult Parser::parseMultiplication ( ParseResult prLeft ) {
    // parser has already matched left expression 
    ParseResult pr ;
	 Expr *left = dynamic_cast<Expr *>(prLeft.ast);
    match ( star ) ;
	 string *op = new string(prevToken->lexeme);
    ParseResult prRight = parseExpr( prevToken->lbp() );
	Expr *right = dynamic_cast<Expr *>(prRight.ast);
	pr.ast = new BinOpExpr(left, op, right); 
    return pr ;
}

// Expr ::= Expr dash Expr
ParseResult Parser::parseSubtraction ( ParseResult prLeft ) {
    // parser has already matched left expression 
    ParseResult pr ;
	Expr *left = dynamic_cast<Expr *>(prLeft.ast);
    match ( dash ) ;
	string *op = new string(prevToken->lexeme);
    ParseResult prRight = parseExpr( prevToken->lbp() );
	Expr *right = dynamic_cast<Expr *>(prRight.ast);
	pr.ast = new BinOpExpr(left, op, right); 
    return pr ;
}

// Expr ::= Expr forwardSlash Expr
ParseResult Parser::parseDivision ( ParseResult prLeft ) {
    // parser has already matched left expression 
    ParseResult pr ;
	Expr *left = dynamic_cast<Expr *>(prLeft.ast);
    match ( forwardSlash ) ;
	string *op = new string(prevToken->lexeme);
    ParseResult prRight = parseExpr( prevToken->lbp() );
	Expr *right = dynamic_cast<Expr *>(prRight.ast);
	pr.ast = new BinOpExpr(left, op, right); 
    return pr ;
}


// Expr ::= Expr equalEquals Expr
// Expr ::= Expr lessThanEquals Expr
// Expr ::= Expr greaterThanEquals Expr
// Expr ::= Expr notEquals Expr
// Expr ::= Expr leftAngle Expr
// Expr ::= Expr rightAngle Expr
/* Notice that for relational operators we use just one parse
   function.  This shows another possible means for implementing
   expressions, as opposed to the method used for arithmetic
   expressions in which each operation has its own parse method.  It
   will depend on what we do in iteration 3 in building an abstract
   syntax tree to decide which method is better.
 */
ParseResult Parser::parseRelationalExpr ( ParseResult prLeft ) {
    // parser has already matched left expression 
    ParseResult pr ;
	Expr *left = dynamic_cast<Expr *>(prLeft.ast);

    nextToken( ) ;
    // just advance token, since examining it in parseExpr caused
    // this method being called.
    string *op = new string(prevToken->lexeme) ;

 //   parseExpr( prevToken->lbp() );

  	 ParseResult prRight = parseExpr( prevToken->lbp() );
	Expr *right = dynamic_cast<Expr *>(prRight.ast);
	pr.ast = new BinOpExpr(left, op, right);  
    return pr ;
}


// Helper function used by the parser.

void Parser::match (tokenType tt) {
    if (! attemptMatch(tt)) {
        throw ( makeErrorMsgExpected ( tt ) ) ;
    }
}

bool Parser::attemptMatch (tokenType tt) {
    if (currToken->terminal == tt) { 
        nextToken() ;
        return true ;
    }
    return false ;
}

bool Parser::nextIs (tokenType tt) {
    return currToken->terminal == tt ;
}

void Parser::nextToken () {
    if ( currToken == NULL ) 
        throw ( string("Internal Error: should not call nextToken in unitialized state"));
    else 
    if (currToken->terminal == endOfFile && currToken->next == NULL) {
        prevToken = currToken ;
    } else if (currToken->terminal != endOfFile && currToken->next == NULL) {
        throw ( makeErrorMsg ( "Error: tokens end with endOfFile" ) ) ;
    } else {
        prevToken = currToken ;
        currToken = currToken->next ;
    }
}

string Parser::terminalDescription ( tokenType terminal ) {
    Token *dummyToken = new Token ("",terminal, NULL) ;
    ExtToken *dummyExtToken = extendToken (this, dummyToken) ;
    string s = dummyExtToken->description() ;
    delete dummyToken ;
    delete dummyExtToken ;
    return s ;
}

string Parser::makeErrorMsgExpected ( tokenType terminal ) {
    string s = (string) "Expected " + terminalDescription (terminal) +
        " but found " + currToken->description() ;
    return s ;
}

string Parser::makeErrorMsg ( tokenType terminal ) {
    string s = "Unexpected symbol " + terminalDescription (terminal) ;
    return s ;
}

string Parser::makeErrorMsg ( const char *msg ) {
    return msg ;
}

