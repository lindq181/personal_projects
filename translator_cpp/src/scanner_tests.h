#include <cxxtest/TestSuite.h>

#include "readInput.h"
#include "scanner.h"
#include <stdio.h>

using namespace std;

class ScannerTestSuite : public CxxTest::TestSuite 
{
public:
    /* A Scanner object is created in the test_setup_code method.
       This is a test, beginning with "test_", so that it is executed
       by the testing framework.  The scanner is used in some tests of
       the method "scan".
     */
    Scanner *s ;
    void test_setup_code ( ) {
        s = new Scanner() ;
    }


    // Tests for components and functions used by "scan"
    // --------------------------------------------------

    /* You may need to write several tests to adequately test the
       functions that are called from "scan".  Once you are confident
       that the components used by "scan" work properly, then you can
       run tests on the "scan" method itself.
    */

    // Here you should place tests for these other functions.


    /* Below is one of the tests for these components in the project
       solution created by your instructor..  It uses a helper
       function function called "tokenMaker_tester", which you have
       not been given.  You are expected to design your own components
       for "scan" and your own mechanisms for easily testing them.

        void xtest_TokenMaker_leftCurly ( ) {
            tokenMaker_tester ("{ ", "^\\{", leftCurly, "{" ) ;
        }

        Note that this test is here named "xtest_Token..." The leading
        "x" is so that cxxTest doesn't scan the line above and think
        it is an actual test that isn't commented out.  cxxTest is
        very simple and doesn't even process block comments.
    */


    /* You must have at least one separate test case for each terminal
       symbol.  Thus, you need a test that will pass or fail based
       solely on the regular expression (and its corresponding code)
       for each terminal symbol.

       This requires a test case for each element of the enumerated
       type tokenType.  This may look something like the sample test
       shown in the comment above.
    */





    // Tests for "scan"
    // --------------------------------------------------

    /* Below are some helper functions and sample tests for testing the 
       "scan" method on Scanner.
    */

    // Test that a list of tokens has no lexicalError tokens.
    bool noLexicalErrors (Token *tks) {
        Token *currentToken = tks ;
        while (currentToken != NULL) {
            if (currentToken->terminal == lexicalError) {
                printf("priblem: %s\n, next lexeme: %s\n",currentToken->lexeme.c_str(), currentToken->next->lexeme.c_str());
				return false ;
            }
            else {
                currentToken = currentToken->next ;
            }
        }
        return true ;
    }

    /* A quick, but inaccurate, test for scanning files.  It only
       checks that no lexical errors occurred, not that the right
       tokens were returned. 
    */
    void scanFileNoLexicalErrors ( const char* filename ) {
        char *text =  readInputFromFile ( filename )  ;
        TS_ASSERT ( text ) ;
        Token *tks = s->scan ( text ) ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT (noLexicalErrors(tks));
    }


    /* This function checks that the terminal fields in the list of
       tokens matches a list of terminals.
    */
    bool sameTerminals (Token *tks, int numTerms, tokenType *ts) {
        Token *currentToken = tks ;
        int termIndex = 0 ;
        while (currentToken != NULL && termIndex < numTerms ) {
            if (currentToken->terminal != ts[termIndex]) {
            printf("%i: %i should be %i\n",termIndex,currentToken->terminal , ts[termIndex]);
			fflush(stdout);
                return false ;
            }
            else {
                ++ termIndex ;
                currentToken = currentToken->next ;
            }
        }
        return true ;
    }


    /* Below are the provided test files that your code should also
       pass.

       You may initially want to rename these tests to "xtest_..." so
       that the CxxTest framework does not see it as a test and won't
       run it as one.  This way you can focus on the tests that you'll
       write above for the individual terminal types first. Then focus
       on these tests.

    */

    // The "endOfFile" token is always the last one in the list of tokens.
    void test_scan_empty ( ) {
        Token *tks = s->scan ("  ") ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, endOfFile) ;
        TS_ASSERT (tks->next == NULL) ;
    }

    void test_scan_empty_comment ( ) {
        Token *tks = s->scan (" /* a comment */ ") ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, endOfFile) ;
        TS_ASSERT (tks->next == NULL) ;
    }

    // When a lexical error occurs, the scanner creates a token with a 
    // single-character lexeme and lexicalError as the terminal.
   void test_scan_lexicalErrors ( ) {
        Token *tks = s->scan ("$&1  ") ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, lexicalError) ;
        TS_ASSERT_EQUALS (tks->lexeme, "$");
        tks = tks->next ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, lexicalError) ;
        TS_ASSERT_EQUALS (tks->lexeme, "&");
        tks = tks->next ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, intConst) ;
        TS_ASSERT_EQUALS (tks->lexeme, "1");
        tks = tks->next ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, endOfFile) ;
        TS_ASSERT (tks->next == NULL) ;
    }


    // A test for scanning numbers and a variable.
    void test_scan_nums_vars ( ) {
        Token *tks = s->scan (" 123 x 12.34 ") ;
        TS_ASSERT (tks != NULL) ;
        tokenType ts[] = { intConst, variableName, floatConst, endOfFile } ;
        TS_ASSERT ( sameTerminals ( tks, 4, ts ) ) ;
    }



    /* This test checks that the scanner returns a list of tokens with
       the correct terminal fields.  It doesn't check that the lexeme
       are correct.
     */
//	void test_scan_sample_test ( ) {
//        const char *filename = "../samples/sample_test.dsl" ;
//        char *text =  readInputFromFile ( filename )  ;
//        TS_ASSERT ( text ) ;
//        Token *tks = s->scan ( text ) ;
//        TS_ASSERT (tks != NULL) ;
//	while (tks != NULL) {
//		cout << tks->lexeme << endl;
//		tks = tks->next;
//	}
//	}
	
    
    void test_scan_bad_syntax_good_tokens ( ) {
        const char *filename = "../samples/bad_syntax_good_tokens.dsl" ;
        char *text =  readInputFromFile ( filename )  ;
        TS_ASSERT ( text ) ;
        Token *tks = s->scan ( text ) ;
        TS_ASSERT (tks != NULL) ;
//	int number = 0;
//	while (tks != NULL) {
//	number++;
//	cout << tks->lexeme << endl;
//	tks = tks->next;
//	}
//	cout << number << endl;
        tokenType ts[] = { 
            intConst, intConst, intConst, intConst, 

            stringConst, stringConst, stringConst,

            floatConst, floatConst, floatConst,

                   matrixKwd,



            semiColon, comma, colon,
            leftCurly, leftParen, rightCurly, rightParen,

            plusSign, star, dash, forwardSlash, 

            equalsEquals, lessThanEqual, 
            greaterThanEqual, notEquals, 
            assign,

            variableName, variableName, variableName, variableName, 
            variableName, variableName, variableName,


            intKwd, floatKwd,  stringKwd,  whileKwd,

            endOfFile
       };
        int count = 39; 
        TS_ASSERT (sameTerminals (tks, count, ts));
    }

    void test_scan_sample_forestLoss ( ) {
        scanFileNoLexicalErrors ("../samples/forest_loss_v2.dsl") ;
    }


	//starting the Punctuation tests

	 void test_terminal_leftParen ( ) {
        	Token *tks = s->scan ("(ghg") ;
        	TS_ASSERT (tks != NULL) ;
        	TS_ASSERT_EQUALS (tks->terminal, leftParen);
		TS_ASSERT_EQUALS (tks->lexeme, "(");
    	}

	void test_terminal_rightParen ( ) {
        	Token *tks = s->scan (")ghg") ;
        	TS_ASSERT (tks != NULL) ;
        	TS_ASSERT_EQUALS (tks->terminal, rightParen);
		TS_ASSERT_EQUALS (tks->lexeme, ")");
    	}
	
	void test_terminal_leftCurly ( ) {
        	Token *tks = s->scan ("{ghg") ;
        	TS_ASSERT (tks != NULL) ;
        	TS_ASSERT_EQUALS (tks->terminal, leftCurly);
		TS_ASSERT_EQUALS (tks->lexeme, "{");
    	}

	void test_terminal_rightCurly ( ) {
        	Token *tks = s->scan ("}ghg") ;
        	TS_ASSERT (tks != NULL) ;
        	TS_ASSERT_EQUALS (tks->terminal, rightCurly);
		TS_ASSERT_EQUALS (tks->lexeme, "}");
    	}

	void test_terminal_leftSquare ( ) {
        	Token *tks = s->scan ("[ghg") ;
        	TS_ASSERT (tks != NULL) ;
        	TS_ASSERT_EQUALS (tks->terminal, leftSquare);
		TS_ASSERT_EQUALS (tks->lexeme, "[");
    	}

	void test_terminal_rightSquare ( ) {
        	Token *tks = s->scan ("]ghg") ;
        	TS_ASSERT (tks != NULL) ;
        	TS_ASSERT_EQUALS (tks->terminal, rightSquare);
		TS_ASSERT_EQUALS (tks->lexeme, "]");
    	}

	void test_terminal_comma ( ) {
        	Token *tks = s->scan (",ghg") ;
        	TS_ASSERT (tks != NULL) ;
        	TS_ASSERT_EQUALS (tks->terminal, comma);
		TS_ASSERT_EQUALS (tks->lexeme, ",");
    	}

	void test_terminal_colon ( ) {
        	Token *tks = s->scan (":ghg") ;
        	TS_ASSERT (tks != NULL) ;
        	TS_ASSERT_EQUALS (tks->terminal, colon);
		TS_ASSERT_EQUALS (tks->lexeme, ":");
    	}

	void test_terminal_semiColon ( ) {
        	Token *tks = s->scan (";ghg") ;
        	TS_ASSERT (tks != NULL) ;
        	TS_ASSERT_EQUALS (tks->terminal, semiColon);
		TS_ASSERT_EQUALS (tks->lexeme, ";");
    	}

	void test_terminal_assign ( ) {
        	Token *tks = s->scan ("=ghg") ;
        	TS_ASSERT (tks != NULL) ;
        	TS_ASSERT_EQUALS (tks->terminal, assign);
		TS_ASSERT_EQUALS (tks->lexeme, "=");

    	}

	void test_terminal_plusSign ( ) {
        	Token *tks = s->scan ("+ghg") ;
        	TS_ASSERT (tks != NULL) ;
        	TS_ASSERT_EQUALS (tks->terminal, plusSign);
		TS_ASSERT_EQUALS (tks->lexeme, "+");
    	}

	void test_terminal_star ( ) {
        	Token *tks = s->scan ("*ghg") ;
        	TS_ASSERT (tks != NULL) ;
        	TS_ASSERT_EQUALS (tks->terminal, star);
		TS_ASSERT_EQUALS (tks->lexeme, "*");
    	}

	void test_terminal_dash ( ) {
        	Token *tks = s->scan ("-ghg") ;
        	TS_ASSERT (tks != NULL) ;
        	TS_ASSERT_EQUALS (tks->terminal, dash);
		TS_ASSERT_EQUALS (tks->lexeme, "-");
    	}

	void test_terminal_forwardSlash ( ) {
        	Token *tks = s->scan ("/ghg") ;
        	TS_ASSERT (tks != NULL) ;
        	TS_ASSERT_EQUALS (tks->terminal, forwardSlash);
		TS_ASSERT_EQUALS (tks->lexeme, "/");
    	}

	void test_terminal_lessThan ( ) {
        	Token *tks = s->scan ("<ghg") ;
        	TS_ASSERT (tks != NULL) ;
        	TS_ASSERT_EQUALS (tks->terminal, lessThan);
		TS_ASSERT_EQUALS (tks->lexeme, "<");
    	}

	void test_terminal_lessThanEqual ( ) {
        	Token *tks = s->scan ("<=ghg") ;
        	TS_ASSERT (tks != NULL) ;
        	TS_ASSERT_EQUALS (tks->terminal, lessThanEqual);
		TS_ASSERT_EQUALS (tks->lexeme, "<=");
    	}

	void test_terminal_greaterThan ( ) {
        	Token *tks = s->scan (">") ;
        	TS_ASSERT (tks != NULL) ;
        	TS_ASSERT_EQUALS (tks->terminal, greaterThan);
		TS_ASSERT_EQUALS (tks->lexeme, ">");
    	}

	void test_terminal_greaterThanEqual ( ) {
        	Token *tks = s->scan (">=") ;
        	TS_ASSERT (tks != NULL) ;
        	TS_ASSERT_EQUALS (tks->terminal, greaterThanEqual);
		TS_ASSERT_EQUALS (tks->lexeme, ">=");
    	}

	void test_terminal_equalsEquals ( ) {
        	Token *tks = s->scan ("==") ;
        	TS_ASSERT (tks != NULL) ;
        	TS_ASSERT_EQUALS (tks->terminal, equalsEquals);
		TS_ASSERT_EQUALS (tks->lexeme, "==");
    	}

	void test_terminal_notEquals ( ) {
        	Token *tks = s->scan ("!=ghg") ;
        	TS_ASSERT (tks != NULL) ;
        	TS_ASSERT_EQUALS (tks->terminal, notEquals);
		TS_ASSERT_EQUALS (tks->lexeme, "!=");
    	}

	void test_terminal_andOp ( ) {
        	Token *tks = s->scan ("&&ghg") ;
        	TS_ASSERT (tks != NULL) ;
        	TS_ASSERT_EQUALS (tks->terminal, andOp);
		TS_ASSERT_EQUALS (tks->lexeme, "&&");
    	}

	void test_terminal_orOp ( ) {
        	Token *tks = s->scan ("||ghg") ;
        	TS_ASSERT (tks != NULL) ;
        	TS_ASSERT_EQUALS (tks->terminal, orOp);
		TS_ASSERT_EQUALS (tks->lexeme, "||");
    	}

	void test_terminal_notOp ( ) {
        	Token *tks = s->scan ("!ghg") ;
        	TS_ASSERT (tks != NULL) ;
        	TS_ASSERT_EQUALS (tks->terminal, notOp);
		TS_ASSERT_EQUALS (tks->lexeme, "!");
    	}
	


	// start the word tests
	void test_terminal_intKwd () {
		Token *tks = s ->scan("Int f");
		TS_ASSERT (tks != NULL) ;
                TS_ASSERT_EQUALS (tks->terminal, intKwd);
                TS_ASSERT_EQUALS (tks->lexeme, "Int");
        }
       void test_terminal_floatKwd ( ) {
                Token *tks = s->scan ("Float f") ;
                TS_ASSERT (tks != NULL) ;
                TS_ASSERT_EQUALS (tks->terminal, floatKwd);
                TS_ASSERT_EQUALS (tks->lexeme, "Float");
        }
	 void test_terminal_stringKwd ( ) {
                Token *tks = s->scan ("Str s") ;
                TS_ASSERT (tks != NULL) ;
                TS_ASSERT_EQUALS (tks->terminal, stringKwd);
                TS_ASSERT_EQUALS (tks->lexeme, "Str");
        }
	 void test_terminal_matrixKwd ( ) {
                Token *tks = s->scan ("Matrix m") ;
                TS_ASSERT (tks != NULL) ;
                TS_ASSERT_EQUALS (tks->terminal, matrixKwd);
                TS_ASSERT_EQUALS (tks->lexeme, "Matrix");
        }
	 void test_terminal_letKwd ( ) {
                Token *tks = s->scan ("let") ;
                TS_ASSERT (tks != NULL) ;
                TS_ASSERT_EQUALS (tks->terminal, letKwd);
                TS_ASSERT_EQUALS (tks->lexeme, "let");
        }
	 void test_terminal_inKwd ( ) {
                Token *tks = s->scan ("in") ;
                TS_ASSERT (tks != NULL) ;
                TS_ASSERT_EQUALS (tks->terminal, inKwd);
                TS_ASSERT_EQUALS (tks->lexeme, "in");
        }
	 void test_terminal_endKwd ( ) {
                Token *tks = s->scan ("end") ;
                TS_ASSERT (tks != NULL) ;
                TS_ASSERT_EQUALS (tks->terminal, endKwd);
                TS_ASSERT_EQUALS (tks->lexeme, "end");
        }
	 void test_terminal_ifKwd ( ) {
                Token *tks = s->scan ("if") ;
                TS_ASSERT (tks != NULL) ;
                TS_ASSERT_EQUALS (tks->terminal, ifKwd);
                TS_ASSERT_EQUALS (tks->lexeme, "if");
        }
	 void test_terminal_thenKwd ( ) {
                Token *tks = s->scan ("then") ;
                TS_ASSERT (tks != NULL) ;
                TS_ASSERT_EQUALS (tks->terminal, thenKwd);
                TS_ASSERT_EQUALS (tks->lexeme, "then");
        }
	 void test_terminal_elseKwd ( ) {
                Token *tks = s->scan ("else") ;
                TS_ASSERT (tks != NULL) ;
                TS_ASSERT_EQUALS (tks->terminal, elseKwd);
                TS_ASSERT_EQUALS (tks->lexeme, "else");
        }
	 void test_terminal_forKwd ( ) {
                Token *tks = s->scan ("for(i=1)") ;
                TS_ASSERT (tks != NULL) ;
                TS_ASSERT_EQUALS (tks->terminal, forKwd);
                TS_ASSERT_EQUALS (tks->lexeme, "for");
        }
	 void test_terminal_whileKwd ( ) {
                Token *tks = s->scan ("while(1)") ;
                TS_ASSERT (tks != NULL) ;
                TS_ASSERT_EQUALS (tks->terminal, whileKwd);
                TS_ASSERT_EQUALS (tks->lexeme, "while");
        }
	 void test_terminal_printKwd ( ) {
                Token *tks = s->scan ("print f") ;
                TS_ASSERT (tks != NULL) ;
                TS_ASSERT_EQUALS (tks->terminal, printKwd);
                TS_ASSERT_EQUALS (tks->lexeme, "print");
        }
	void test_terminal_boolKwd ( ) {
                Token *tks = s->scan ("Bool f") ;
                TS_ASSERT (tks != NULL) ;
                TS_ASSERT_EQUALS (tks->terminal, boolKwd);
                TS_ASSERT_EQUALS (tks->lexeme, "Bool");
        }
	void test_terminal_trueKwd ( ) {
                Token *tks = s->scan ("True f") ;
                TS_ASSERT (tks != NULL) ;
                TS_ASSERT_EQUALS (tks->terminal, trueKwd);
                TS_ASSERT_EQUALS (tks->lexeme, "True");
        }
	void test_terminal_falseKwd ( ) {
                Token *tks = s->scan ("False f") ;
                TS_ASSERT (tks != NULL) ;
                TS_ASSERT_EQUALS (tks->terminal, falseKwd);
                TS_ASSERT_EQUALS (tks->lexeme, "False");
        }
	 void test_terminal_variableName ( ) {
                Token *tks = s->scan ("myVar") ;
                TS_ASSERT (tks != NULL) ;
                TS_ASSERT_EQUALS (tks->terminal, variableName);
                TS_ASSERT_EQUALS (tks->lexeme, "myVar");
        }

	//start num tests
	 void test_terminal_intConst ( ) {
                Token *tks = s->scan ("12345") ;
                TS_ASSERT (tks != NULL) ;
                TS_ASSERT_EQUALS (tks->terminal, intConst);
                TS_ASSERT_EQUALS (tks->lexeme, "12345");
        }
	 void test_terminal_floatConst ( ) {
                Token *tks = s->scan ("1.24") ;
                TS_ASSERT (tks != NULL) ;
                TS_ASSERT_EQUALS (tks->terminal, floatConst);
                TS_ASSERT_EQUALS (tks->lexeme, "1.24");
        }
	 void test_terminal_stringConst ( ) {
                Token *tks = s->scan ("\"this\"") ;
                TS_ASSERT (tks != NULL) ;
                TS_ASSERT_EQUALS (tks->terminal, stringConst);
                TS_ASSERT_EQUALS (tks->lexeme, "\"this\"");
        }

} ;
