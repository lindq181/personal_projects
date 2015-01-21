/* A small meaningless comment */
#ifndef SCANNER_H
#define SCANNER_H

#include <regex.h>
#include <string>


class Token;

/* This enumerated type is used to keep track of what kind of
   construct was matched. 
 */



enum tokenEnumType { 


	intKwd, floatKwd, boolKwd, 
	trueKwd, falseKwd,  stringKwd, matrixKwd,
	letKwd, inKwd, endKwd, ifKwd, thenKwd, elseKwd,
	forKwd, whileKwd, printKwd,

	// Constants
	intConst, floatConst, stringConst, 

	// Names
	variableName,

	// Punctuation
	leftParen, rightParen, 
	leftCurly, rightCurly, 
	leftSquare, rightSquare,

	comma, semiColon, colon,

	//Operators
	assign, 
	plusSign, star, dash, forwardSlash,
	lessThan, lessThanEqual, greaterThan, greaterThanEqual,
	equalsEquals, notEquals,
	andOp, orOp, notOp,


	// Special terminal types
	endOfFile ,
	lexicalError
} ;
typedef enum tokenEnumType tokenType ;

// Below you need to write your class definitions for Token and Scanner.
class Token {
	public:
		Token () {};
		Token(std::string k, tokenEnumType term, Token *gulf);
		tokenType terminal;
		std::string lexeme;
		Token *next;
};

//Token::Token() {
//	next = NULL;
//}

//Token::Token(std::string k, tokenEnumType term, Token *gulf) {
//	terminal = term;
//	lexeme = k;
//	next = gulf;
//}

class Scanner {
	public:
		Token *scan (const char * );
};



#endif /* SCANNER_H */
