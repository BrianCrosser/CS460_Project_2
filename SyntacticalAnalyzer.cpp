/********************************************************************************/
/* Project: Project 2									*/
/* Author: Brian Crosser, Kevin Gantsweg, Michelle Padgett									*/
/* Date: Spring 2017									*/
/* Description:									*/
/********************************************************************************/

#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>
#include "SyntacticalAnalyzer.h"

using namespace std;

SyntacticalAnalyzer::SyntacticalAnalyzer (char * filename)
{
    /********************************************************************************/
    /* This function will								*/
    /********************************************************************************/
    lex = new LexicalAnalyzer (filename);
    int fnlength = strlen (filename);
    filename[fnlength-2] = 'p';
    filename[fnlength-1] = '2';
    p2file.open (filename);
    token = lex->GetToken();
    int errors = program ();
}

SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{
    /********************************************************************************/
    /* This function will								*/
    /********************************************************************************/
    delete lex;
}

int SyntacticalAnalyzer::program()
{
    lex->debug << "program function called\n";
    int errors = 0;
    if(token == LPAREN_T){
        p2file << "program on rule 1\n";
        token = lex->GetToken();
        errors += define();
        errors += more_defines();
    }
    
    if(token != EOF_T)
    {
        lex->ReportError("Expected end of file; " + lex->GetLexeme());
        errors++;
    }
    
    lex->debug << "program function returning " << errors << " errors\n";
    return errors;
}

int SyntacticalAnalyzer::define()
{
    lex->debug << "define function called\n";
    int errors = 0;
    if(token == DEFINE_T){
            token = lex->GetToken();
            if(token == LPAREN_T){
                token = lex->GetToken();
                if(token == IDENT_T){
                    token = lex->GetToken();
                    p2file << "define on rule 2\n";
                    errors += param_list();
                    if(token == RPAREN_T){
                        token = lex->GetToken();
                        errors += stmt();
                        errors += stmt_list();
                    } else {
                        errors++;
                        lex->ReportError("Expected right parenthesis; " + lex->GetLexeme());
                    }
                }
            }
    }
    
    if(token != RPAREN_T){
        errors++;
        lex->ReportError("Expected right parenthesis; " + lex->GetLexeme());
    }
    
    // Token check in follow set
    if(token != LPAREN_T){
        lex->ReportError("Exptected LPAREN_T in define's follow set; " + lex->GetLexeme());
    }
    else{
        p2file << "define on rule 1/3\n";	// how to tell if it is rule 1 or rule 3 in the follow set?
    }
    
    lex->debug << "define function returning " << errors << " errors\n";
    return errors;
}

int SyntacticalAnalyzer::more_defines()
{
    lex->debug << "more_defines function called\n";
    int errors = 0;
    
    switch(token){
        case LPAREN_T:
            p2file << "more_defines on rule 3\n";
            token = lex->GetToken();
            errors += define();
            errors += more_defines();
            
        case EOF_T:
            token = lex->GetToken();
            p2file << "more_defines on rule 4\n";
    }
    
    lex->debug << "more_defines function returning " << errors << " errors\n";
    return errors;
}

int SyntacticalAnalyzer::param_list()
{
    lex->debug << "param_list function called\n";
    int errors = 0;
    
    switch(token){
        case IDENT_T:
            p2file << "param_list on rule 15\n";
            token = lex->GetToken();
            errors += param_list();
    }
    
    lex->debug << "param_list function returning " << errors << " errors\n";
    return errors;
}

int SyntacticalAnalyzer::stmt()
{
    lex->debug << "stmt function called\n";
    int errors = 0;
    
    switch(token){
        case LPAREN_T:
            p2file << "stmt on rule 9\n";
            token = lex->GetToken();
            errors += action();
            
        case IDENT_T:
            p2file << "stmt on rule 8\n";
            token = lex->GetToken();
            
        case NUMLIT_T:
            p2file << "stmt on rule 7\n";
            token = lex->GetToken();
            errors += literal();
            
        case QUOTE_T:
            p2file << "stmt on rule 7\n";
            //token = lex->GetToken();
            errors += literal();
            
        default:
            errors++;
            lex->ReportError("Expected right parenthesis or an identifier; " + lex->GetLexeme());
    }
    
    lex->debug << "stmt function returning " << errors << " errors\n";
    return errors;
}

int SyntacticalAnalyzer::stmt_list()
{
    lex->debug << "stmt_list function called\n";
    int errors = 0;
    
    switch(token){
        case LPAREN_T:
            p2file << "stmt_list on rule 5\n";
            token = lex->GetToken();
            errors += stmt();
            
        case IDENT_T:
            p2file << "stmt_list on rule 5\n";
            token = lex->GetToken();
            errors += stmt();
            
        case RPAREN_T:
            p2file << "stmt_list on rule 6\n";
            token = lex->GetToken();
            errors += stmt();
            
        case NUMLIT_T:
            p2file << "stmt_list on rule 5\n";
            token = lex->GetToken();
            errors += stmt();
            
        case QUOTE_T:
            p2file << "stmt_list on rule 5\n";
            //token = lex->GetToken();
            errors += stmt();
            
        default:
            errors++;
            lex->ReportError("Expected left or right parenthesis, number, quote, or an identifier; " + lex->GetLexeme());
    }
    
    lex->debug << "stmt_list function returning " << errors << " errors\n";
    return errors;
}

int SyntacticalAnalyzer::literal()
{
    lex->debug << "literal function called\n";
    int errors = 0;
    
    switch(token){
        case NUMLIT_T:
            p2file << "literal on rule 10\n";
            token = lex->GetToken();
            
        case QUOTE_T:
            p2file << "literal on rule 11\n";
            token = lex->GetToken();
            errors += quoted_lit();
            
        default:
            errors++;
            lex->ReportError("Expecting a quote or a number; " + lex->GetLexeme());
    }
                             
    lex->debug << "literal function returning " << errors << " errors\n";
    return errors;
}
                             
int SyntacticalAnalyzer::quoted_lit()
{
                lex->debug << "quoted_lit function called\n";
                int errors = 0;
                
                switch(token){
                    case EOF_T:
                        errors++;
                        lex->ReportError("Expecting any_other_token; " + lex->GetLexeme());
                        
                    case RPAREN_T:
                        errors++;
                        lex->ReportError("Expecting any_other_token; " + lex->GetLexeme());
                        
                    default:
                        p2file << "quoted_lit on rule 12\n";
                        token = lex->GetToken();
                        errors += any_other_token();
                }
                
                lex->debug << "quoted_lit function returning " << errors << " errors\n";
                return errors;
            }
                             
int SyntacticalAnalyzer::more_tokens()
{
    lex->debug << "more_tokens function called\n";
    int errors = 0;

    switch(token){
        case RPAREN_T:
            p2file << "more_tokens on rule 14\n";
            token = lex->GetToken();
            lex->debug << "Current Token: " + token << endl;

        case IF_T:
            errors++;
            lex->ReportError("Unexpected token; " + lex->GetLexeme());
        
        case LISTOP_T:
            errors++;
            lex->ReportError("Unexpected token; " + lex->GetLexeme());
        
        case EOF_T:
            errors++;
            lex->ReportError("Unexpected token; " + lex->GetLexeme());

        default:
            p2file << "more_tokens on rule 13\n";
            token = lex->GetToken();
            errors += any_other_token();
            errors += more_tokens();
    }    

    lex->debug << "more_tokens function returning " << errors << " errors\n";
    return errors;
}


int SyntacticalAnalyzer::any_other_token()
{
            lex->debug << "any_other_token function called\n";
                int errors = 0;
                
                switch(token){
                    case LPAREN_T:
                        p2file << "any_other_token on rule 44\n";
                        token = lex->GetToken();
                       errors += more_tokens();
                        if(token != RPAREN_T){
                            errors++;
                            lex->ReportError("Expected right parenthesis; " + lex->GetLexeme());
                        }
                        
                    case IDENT_T:
                        p2file << "any_other_token on rule 45\n";
                        token = lex->GetToken();
                        
                    case NUMLIT_T:
                        p2file << "any_other_token on rule 46\n";
                        token = lex->GetToken();
                        
                    case CONS_T:
                        p2file << "any_other_token on rule 47\n";
                        token = lex->GetToken();
                        
                    case IF_T:
                        p2file << "any_other_token on rule 48\n";
                        token = lex->GetToken();
                        
                    case DISPLAY_T:
                        p2file << "any_other_token on rule 49\n";
                        token = lex->GetToken();
                        
                    case NEWLINE_T:
                        p2file << "any_other_token on rule 50\n";
                        token = lex->GetToken();
                        
                    case LISTOP_T:
                        p2file << "any_other_token on rule 51\n";
                        token = lex->GetToken();
                        
                    case AND_T:
                        p2file << "any_other_token on rule 52\n";
                        token = lex->GetToken();
                        
                    case OR_T:
                        p2file << "any_other_token on rule 53\n";
                        token = lex->GetToken();
                        
                    case NOT_T:
                        p2file << "any_other_token on rule 54\n";
                        token = lex->GetToken();
                        
                    case DEFINE_T:
                        p2file << "any_other_token on rule 55\n";
                        token = lex->GetToken();
                        
                    case NUMBERP_T:
                        p2file << "any_other_token on rule 56\n";
                        token = lex->GetToken();
                        
                    case SYMBOLP_T:
                        p2file << "any_other_token on rule 57\n";
                        token = lex->GetToken();
                        
                    case LISTP_T:
                        p2file << "any_other_token on rule 58\n";
                        token = lex->GetToken();
                        
                    case ZEROP_T:
                        p2file << "any_other_token on rule 59\n";
                        token = lex->GetToken();
                        
                    case NULLP_T:
                        p2file << "any_other_token on rule 60\n";
                        token = lex->GetToken();
                        
                    case CHARP_T:
                        p2file << "any_other_token on rule 61\n";
                        token = lex->GetToken();
                        
                    case STRINGP_T:
                        p2file << "any_other_token on rule 62\n";
                        token = lex->GetToken();
                        
                    case PLUS_T:
                        p2file << "any_other_token on rule 63\n";
                        token = lex->GetToken();
                        
                    case MINUS_T:
                        p2file << "any_other_token on rule 64\n";
                        token = lex->GetToken();
                        
                    case DIV_T:
                        p2file << "any_other_token on rule 65\n";
                        token = lex->GetToken();
                        
                    case MULT_T:
                        p2file << "any_other_token on rule 66\n";
                        token = lex->GetToken();
                        
                    case EQUALTO_T:
                        p2file << "any_other_token on rule 67\n";
                        token = lex->GetToken();
                        
                    case GT_T:
                        p2file << "any_other_token on rule 68\n";
                        token = lex->GetToken();
                        
                    case LT_T:
                        p2file << "any_other_token on rule 69\n";
                        token = lex->GetToken();
                        
                    case GTE_T:
                        p2file << "any_other_token on rule 70\n";
                        token = lex->GetToken();
                        
                    case LTE_T:
                        p2file << "any_other_token on rule 71\n";
                        token = lex->GetToken();
                        
                    case QUOTE_T:
                        p2file << "any_other_token on rule 72\n";
                        token = lex->GetToken();
                        
                    default:
                        errors++;
                        lex->ReportError("Unexpected token; " + lex->GetLexeme());
                                         
                                         }
                                         
                                         lex->debug << "any_other_token function returning " << errors << " errors\n";
                                         return errors;
                                         }
                                         
int SyntacticalAnalyzer::else_part()
                        {
                            lex->debug << "else_part function called\n";
                            int errors = 0;
                            
                            switch(token){
                                case RPAREN_T:
                                    p2file << "else_part on rule 18\n";
                                    token = lex->GetToken();
                                    
                                default:
                                    p2file << "else_part on rule 17\n";
                                    token = lex->GetToken();
                                    errors += stmt();
                                    
                                    
                            }
                        }
                                         
int SyntacticalAnalyzer::action()
                        {
                            lex->debug << "action function called\n";
                            int errors = 0;
                            
                            switch(token){  
                                case IDENT_T:
                                    p2file << "action on rule 41\n";
                                    token = lex->GetToken();
                                    errors += stmt_list();
                                    
                                case IF_T:
                                    p2file << "action on rule 19\n";
                                    token = lex->GetToken();
                                    errors += stmt();
                                    errors += stmt();
                                    errors += else_part();
                                    
                                case LISTOP_T:
                                    p2file << "action on rule 20\n";
                                    token = lex->GetToken();
                                    errors += stmt();
                                    
                                case CONS_T:
                                    p2file << "action on rule 21\n";
                                    token = lex->GetToken();
                                    errors += stmt();
                                    errors += stmt();
                                    
                                case AND_T:
                                    p2file << "action on rule 22\n";
                                    token = lex->GetToken();
                                    errors += stmt_list();
                                    
                                case OR_T:
                                    p2file << "action on rule 23\n";
                                    token = lex->GetToken();
                                    errors += stmt_list();    
                                    
                                case NOT_T:
                                    p2file << "action on rule 24\n";
                                    token = lex->GetToken();
                                    errors += stmt();    
                                    
                                case NUMBERP_T:
                                    p2file << "action on rule 25\n";
                                    token = lex->GetToken();
                                    errors += stmt();    
                                    
                                case SYMBOLP_T:
                                    p2file << "action on rule 26\n";
                                    token = lex->GetToken();
                                    errors += stmt();    
                                    
                                case LISTP_T:
                                    p2file << "action on rule 27\n";
                                    token = lex->GetToken();
                                    errors += stmt();    
                                    
                                case ZEROP_T:
                                    p2file << "action on rule 28\n";
                                    token = lex->GetToken();
                                    errors += stmt();
                                    
                                case NULLP_T:
                                    p2file << "action on rule 29\n";
                                    token = lex->GetToken();
                                    errors += stmt();    
                                    
                                case CHARP_T:
                                    p2file << "action on rule 30\n";
                                    token = lex->GetToken();
                                    errors += stmt();    
                                    
                                case STRINGP_T:
                                    p2file << "action on rule 31\n";
                                    token = lex->GetToken();
                                    errors += stmt();    
                                    
                                case PLUS_T:
                                    p2file << "action on rule 32\n";
                                    token = lex->GetToken();
                                    errors += stmt_list();
                                    
                                case DISPLAY_T:
                                    p2file << "action on rule 42\n";
                                    token = lex->GetToken();
                                    errors += stmt();    
                                    
                                case NEWLINE_T:
                                    p2file << "action on rule 43\n";
                                    token = lex->GetToken();
                                    
                                case MINUS_T:
                                    p2file << "action on rule 33\n";
                                    token = lex->GetToken();
                                    errors += stmt();
                                    errors += stmt_list();    
                                    
                                case DIV_T:
                                    p2file << "action on rule 34\n";
                                    token = lex->GetToken();
                                    errors += stmt();
                                    errors += stmt_list();    
                                    
                                case MULT_T:
                                    p2file << "action on rule 35\n";
                                    token = lex->GetToken();
                                    errors += stmt_list();
                                    
                                case EQUALTO_T:
                                    p2file << "action on rule 36\n";
                                    token = lex->GetToken();
                                    errors += stmt_list();    
                                    
                                case GT_T:
                                    p2file << "action on rule 37\n";
                                    token = lex->GetToken();
                                    errors += stmt_list();    
                                    
                                case LT_T:
                                    p2file << "action on rule 38\n";
                                    token = lex->GetToken();
                                    errors += stmt_list();    
                                    
                                case GTE_T:
                                    p2file << "action on rule 30\n";
                                    token = lex->GetToken();
                                    errors += stmt_list();    
                                    
                                case LTE_T:
                                    p2file << "action on rule 40\n";
                                    token = lex->GetToken();
                                    errors += stmt_list();        
                            }
                            
                            lex->debug << "action function returning " << errors << " errors\n";
                            return errors;
                        }
            
