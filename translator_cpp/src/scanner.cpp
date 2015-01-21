//test
//Authors: Jonathan Lindquist, Julia Haubleit 

#include <iostream>
#include <stdlib.h>
#include <string>

#include "scanner.h"
#include "readInput.h"
#include "regex.h"
#include <sys/stat.h>
#include <sys/types.h> 

using namespace std;

Token::Token(std::string k, tokenEnumType term, Token *gulf) {
	terminal = term;
	lexeme = k;
	next = gulf;
}

int consumeWhiteSpaceAndComments (regex_t *whiteSpace, 
                                  regex_t *blockComment,
                                  regex_t *lineComment,
                                  const char *text) ;

	regex_t* whiteSpace = makeRegex ("^[\n\t\r ]+");

	regex_t* blockComment = makeRegex ("^/\\*([^\\*]|\\*+[^\\*/])*\\*+/");

	regex_t* lineComment = makeRegex ("^//[^\n]*\n");

	//regex_t* word = makeRegex ("^[a-zA-Z]+");
	regex_t* intWord = makeRegex ("^(Int)");
	regex_t* stringWord = makeRegex ("^(Str)");
	regex_t* floatWord = makeRegex ("^(Float)");
	regex_t* matrixWord = makeRegex ("^(Matrix)");
	regex_t* letWord = makeRegex ("^(let)");
	regex_t* inWord = makeRegex ("^(in)");
	regex_t* endWord = makeRegex ("^(end)");
	regex_t* ifWord = makeRegex ("^(if)");
	regex_t* thenWord = makeRegex ("^(then)");
	regex_t* elseWord = makeRegex ("^(else)");
	regex_t* forWord = makeRegex ("^(for)");
	regex_t* whileWord = makeRegex ("^(while)");
	regex_t* printWord = makeRegex ("^(print)");
	regex_t* boolWord = makeRegex ("^(Bool)");
	regex_t* trueWord = makeRegex ("^(True)");
	regex_t* falseWord = makeRegex ("^(False)");
	

	//consts
	regex_t* floatRegex = makeRegex ("^[0-9]+\\.[0-9]+");
	regex_t* integerRegex = makeRegex ("^[0-9]+");
	regex_t* stringRegex = makeRegex ("^\"([^\"\\\\]|\\\\.)*\"");

	//variables
	regex_t* variableWord = makeRegex ("^[a-zA-Z\\_0-9]+");

	//punctuation
	regex_t* leftCurlyPunc = makeRegex ("^\\{");
	regex_t* rightCurlyPunc = makeRegex ("^\\}");
	regex_t* leftParenPunc = makeRegex ("^\\(");
	regex_t* rightParenPunc = makeRegex ("^\\)");
	regex_t* leftSquarePunc = makeRegex ("^\\[");
	regex_t* rightSquarePunc = makeRegex ("^\\]");
	regex_t* commaPunc = makeRegex ("^,");
	regex_t* semiColonPunc = makeRegex ("^;");
	regex_t* colonPunc = makeRegex ("^:");

	//operations
	regex_t* assignOp = makeRegex ("^\\=");
	regex_t* plusSignOp = makeRegex ("^\\+");
	regex_t* starOp = makeRegex ("^\\*");
	regex_t* dashOp = makeRegex ("^\\-");
	regex_t* forwardSlashOp = makeRegex ("^/");
	regex_t* lessThanOp = makeRegex ("^<");
	regex_t* lessThanEqualOp = makeRegex ("^<=");
	regex_t* greaterThanOp = makeRegex ("^>");
	regex_t* greaterThanEqualOp = makeRegex ("^>=");
	regex_t* equalEqualsOp = makeRegex ("^==");
	regex_t* notEqualsOp = makeRegex ("^!=");
	regex_t* theAndOp = makeRegex ("^&&");
	regex_t* theOrOp = makeRegex ("^\\|\\|");
	regex_t* theNotOp = makeRegex ("^!");

Token* Scanner::scan(const char* text) {

	Token *head = new Token();
	Token *current = new Token();

	if (text==NULL) {
        	cout << "text was null, exiting" << endl;
		exit(1);
	}


	int numMatchedChars = 0;
	numMatchedChars = consumeWhiteSpaceAndComments (whiteSpace, blockComment, lineComment, text);


	text = text + numMatchedChars;

	int maxNumMatchedChars;

	

	while ( text[0] != '\0' ) {
        	maxNumMatchedChars = 0; 
		//matchType = noMatch;

		Token *holder = new Token();

	// test all the regex's to find the best match represented by most numbers matched
        // Try to match an integer word
		numMatchedChars = matchRegex (intWord, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = intKwd;
        	}

        // Try to match an string word
		numMatchedChars = matchRegex (stringWord, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = stringKwd;
        	}

        // Try to match an float word
		numMatchedChars = matchRegex (floatWord, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = floatKwd;
        	}

        // Try to match an matrix constant
		numMatchedChars = matchRegex (matrixWord, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = matrixKwd;
        	}

        // Try to match an let constant
		numMatchedChars = matchRegex (letWord, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = letKwd;
        	}

        // Try to match an in constant
		numMatchedChars = matchRegex (inWord, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = inKwd;
        	}

        // Try to match an end word
		numMatchedChars = matchRegex (endWord, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = endKwd;
        	}

        // Try to match an if word
		numMatchedChars = matchRegex (ifWord, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = ifKwd;
        	}

        // Try to match an then word
		numMatchedChars = matchRegex (thenWord, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = thenKwd;
        	}

        // Try to match an else word
		numMatchedChars = matchRegex (elseWord, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = elseKwd;
        	}

        // Try to match an for word
		numMatchedChars = matchRegex (forWord, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = forKwd;
        	}

        // Try to match an while word
		numMatchedChars = matchRegex (whileWord, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = whileKwd;
        	}

        // Try to match an print word
		numMatchedChars = matchRegex (printWord, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = printKwd;
        	}

	// Try to match an bool word
		numMatchedChars = matchRegex (boolWord, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = boolKwd;
        	}

	// Try to match an true word
		numMatchedChars = matchRegex (trueWord, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = trueKwd;
        	}

	// Try to match an false word
		numMatchedChars = matchRegex (falseWord, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = falseKwd;
        	}

        // Try to match an integer constant
		numMatchedChars = matchRegex (integerRegex, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = intConst;
        	}

	// Try to match a string constant
		numMatchedChars = matchRegex (stringRegex, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = stringConst;
        	}
	
	// Try to match an float constant
		numMatchedChars = matchRegex (floatRegex, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = floatConst;
        	}

        // Try to match an left curly
		numMatchedChars = matchRegex (leftCurlyPunc, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = leftCurly;
        	}

        // Try to match an right curly
		numMatchedChars = matchRegex (rightCurlyPunc, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = rightCurly;
        	}

        // Try to match an left paren
		numMatchedChars = matchRegex (leftParenPunc, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = leftParen;
        	}

        // Try to match an right paren
		numMatchedChars = matchRegex (rightParenPunc, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = rightParen;
        	}

        // Try to match an left square
		numMatchedChars = matchRegex (leftSquarePunc, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = leftSquare;
        	}

        // Try to match an right square
		numMatchedChars = matchRegex (rightSquarePunc, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = rightSquare;
        	}

        // Try to match an comma
		numMatchedChars = matchRegex (commaPunc, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = comma;
        	}

        // Try to match an semi-colon
		numMatchedChars = matchRegex (semiColonPunc, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = semiColon;
        	}

        // Try to match an colon
		numMatchedChars = matchRegex (colonPunc, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = colon;
        	}

 	// Try to match an variable
		numMatchedChars = matchRegex (variableWord, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = variableName;
        	}

        // Try to match an assign op
		numMatchedChars = matchRegex (assignOp, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = assign;
        	}

        // Try to match an plus op
		numMatchedChars = matchRegex (plusSignOp, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = plusSign;
        	}

        // Try to match an star op
		numMatchedChars = matchRegex (starOp, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = star;
        	}

        // Try to match an dash op
		numMatchedChars = matchRegex (dashOp, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = dash;
        	}

        // Try to match an forward slash op
		numMatchedChars = matchRegex (forwardSlashOp, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = forwardSlash;
        	}

        // Try to match an less than op
		numMatchedChars = matchRegex (lessThanOp, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = lessThan;
        	}	

        // Try to match an less than equals op
		numMatchedChars = matchRegex (lessThanEqualOp, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = lessThanEqual;
        	}

        // Try to match an greater than op
		numMatchedChars = matchRegex (greaterThanOp, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = greaterThan;
        	}

        // Try to match an greater than equals op
		numMatchedChars = matchRegex (greaterThanEqualOp, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = greaterThanEqual;
        	}

        // Try to match an equals equals op
		numMatchedChars = matchRegex (equalEqualsOp, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = equalsEquals;
        	}

        // Try to match an not equals op
		numMatchedChars = matchRegex (notEqualsOp, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = notEquals;
        	}

        // Try to match an and op
		numMatchedChars = matchRegex (theAndOp, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = andOp;
        	}

        // Try to match an or op
		numMatchedChars = matchRegex (theOrOp, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = orOp;
        	}

        // Try to match an not op
		numMatchedChars = matchRegex (theNotOp, text);
		if (numMatchedChars > maxNumMatchedChars) {
            		maxNumMatchedChars = numMatchedChars;
			holder->terminal = notOp;
        	}

		//build string of matched characters to pass into token construction functions
		string compare = "";
		for (int k=0; k<maxNumMatchedChars; k++) {
			compare += *(text + k);
		}

		if (maxNumMatchedChars == 0) {
			holder->terminal = lexicalError;
			holder->lexeme = compare + *text;
			holder->next = NULL;
			maxNumMatchedChars = 1;
		} else {		
			holder->lexeme = compare;
			holder->next = NULL;
		}

		//If list is empty, node becomes head, else tack in on after tail
		if (head->lexeme.empty()) {
			head = holder;
			current = holder;
		} else {
			current->next = holder;
			current = holder;
		}
           	// Consume the characters that were matched.
            	text = text + maxNumMatchedChars;

       
        	numMatchedChars = consumeWhiteSpaceAndComments (whiteSpace, blockComment, lineComment, text);
        	text = text + numMatchedChars;
	}

	Token *tail = new Token();
	tail->terminal = endOfFile;
	//tail->lexeme = "\0";
	tail->next = NULL;

	if (head->lexeme.empty()) {
		head = tail;
	} else {
		current->next = tail;
	}

	return head;

}


int consumeWhiteSpaceAndComments (regex_t *whiteSpace, 
                                  regex_t *blockComment, 
				  regex_t *lineComment,
                                  const char *text) {
    int numMatchedChars = 0 ;
    int totalNumMatchedChars = 0 ;
    int stillConsumingWhiteSpace ;

    do {
        stillConsumingWhiteSpace = 0 ;  // exit loop if not reset by a match

        // Try to match white space
        numMatchedChars = matchRegex (whiteSpace, text) ;
        totalNumMatchedChars += numMatchedChars ;
        if (numMatchedChars > 0) {
            text = text + numMatchedChars ;
            stillConsumingWhiteSpace = 1 ;
        }

        // Try to match block comments
        numMatchedChars = matchRegex (blockComment, text) ;
        totalNumMatchedChars += numMatchedChars ;
        if (numMatchedChars > 0) {
            text = text + numMatchedChars ;
            stillConsumingWhiteSpace = 1 ;
        }

        // Try to match single-line comments
        numMatchedChars = matchRegex (lineComment, text) ;
        totalNumMatchedChars += numMatchedChars ;
        if (numMatchedChars > 0) {
            text = text + numMatchedChars ;
            stillConsumingWhiteSpace = 1 ;
        }
    }
    while ( stillConsumingWhiteSpace ) ;    

    return totalNumMatchedChars ;
}
