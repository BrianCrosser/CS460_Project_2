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
  lex->debug << "            program function called";
  int errors = 0;
  if(token == LPAREN_T){
    p2file << "Starting <program>. Current token = " + lex->GetTokenName(token);
    p2file << "\n";
    p2file << "Using rule 1\n";
    lex->debug << " on rule 1\n";
    errors += define();
    errors += more_defines();
  }
  
  if(token != EOF_T)
    {
      lex->ReportError("Expected end of file; " + lex->GetLexeme());
      errors++;
    }
  
  lex->debug << "            program function returning " << errors << " errors\n";
  p2file << "Ending <program>. Current token = " + lex->GetTokenName(token);
  p2file << ". Errors = " << errors;
  p2file << "\n";
  return errors;
}

int SyntacticalAnalyzer::define()
{
  lex->debug << "            define function called";
  p2file << "Starting <define>. Current token = " + lex->GetTokenName(token);
  p2file << "\n";
  int errors = 0;
  if(token == LPAREN_T) {
    token = lex->GetToken();
    if(token == DEFINE_T){
      token = lex->GetToken();
      if(token == LPAREN_T){
	token = lex->GetToken();
	if(token == IDENT_T){
	  token = lex->GetToken();
	  p2file << "Using rule 2\n";
	  lex->debug << " on rule 2\n";
	  errors += param_list();
	}    
	if(token == RPAREN_T) {
	  token = lex->GetToken();
	  errors += stmt();
	  errors += stmt_list();
	} else {
	  errors++;
	  lex->ReportError("Expected right parenthesis; " + lex->GetLexeme());
	  token = lex->GetToken();
	  errors += stmt();
	  errors =+ stmt_list();
	}
	
      }
    }
    if(token != RPAREN_T) {
      errors++;
      lex->ReportError("Expected right parenthesis; " + lex->GetLexeme());
    }
    token = lex->GetToken();
  }
  
  lex->debug << "                define function returning " << errors << " errors\n";
  p2file << "Ending <define>. Current token = " + lex->GetTokenName(token);
  p2file << ". Errors = " << errors;
  p2file << "\n";
  return errors;
}

int SyntacticalAnalyzer::more_defines()
{
  lex->debug << "            more_defines function called";
  p2file << "Starting <more_defines>. Current token = " + lex->GetTokenName(token);
  p2file << "\n";
  int errors = 0;
  
  switch(token){
  case LPAREN_T:
    p2file << "Using rule 3\n";
    lex->debug << " on rule 3\n";
    token = lex->GetToken();
    errors += define();
    errors += more_defines();
    if(token != RPAREN_T) {
      errors++;
      lex->ReportError("Expected right parenthesis; " + lex->GetLexeme());
    }
    token = lex->GetToken();
    break;
    
  case EOF_T:
    token = lex->GetToken();
    p2file << "Using rule 4\n";
    lex->debug << " on rule 4\n";
    break;
  }
  
  lex->debug << "            more_defines function returning " << errors << " errors\n";
  p2file << "Ending <more_defines>. Current token = " + lex->GetTokenName(token);
  p2file << ". Errors = " << errors;
  p2file << "\n";
  return errors;
}

int SyntacticalAnalyzer::param_list()
{
  lex->debug << "            param_list function called";
  p2file << "Starting <param_list>. Current token = " + lex->GetTokenName(token);
  p2file << "\n";
  int errors = 0;
  
  switch(token){
  case IDENT_T:
    p2file << "Using rule 15\n";
    lex->debug << " on rule 15\n";
    token = lex->GetToken();
    errors += param_list();
    break;
  }
  
  lex->debug << "            param_list function returning " << errors << " errors\n";
  p2file << "Ending <param_list>. Current token = " + lex->GetTokenName(token);
  p2file << ". Errors = " << errors;
  p2file << "\n";
  return errors;
}

int SyntacticalAnalyzer::stmt()
{
  lex->debug << "            stmt function called";
  p2file << "Starting <stmt>. Current token = " + lex->GetTokenName(token);
  p2file << "\n";
  int errors = 0;
  
  switch(token){
  case LPAREN_T:
    p2file << "Using rule 9\n";
    lex->debug << " on rule 9\n";
    token = lex->GetToken();
    errors += action();
    if(token != RPAREN_T) {
      errors++;
      lex->ReportError("Expected right parenthesis; " + lex->GetLexeme());
    }
    token = lex->GetToken();
    break;
    
  case IDENT_T:
    p2file << "Using rule 8\n";
    lex->debug << " on rule 8\n";
    token = lex->GetToken();
    break;
    
  case NUMLIT_T:
    p2file << "Using rule 7\n";
    lex->debug << " on rule 7\n";
    token = lex->GetToken();
    errors += literal();
    break;
    
  case QUOTE_T:
    p2file << "Using rule 7\n";
    lex->debug << " on rule 7\n";
    //token = lex->GetToken();
    errors += literal();
    break;
    
  default:
    errors++;
    lex->ReportError("Expected right parenthesis or an identifier; " + lex->GetLexeme());
  }
  
  lex->debug << "            stmt function returning " << errors << " errors\n";
  p2file << "Ending <stmt>. Current token = " + lex->GetTokenName(token);
  p2file << ". Errors = " << errors;
  p2file << "\n";
  return errors;
}

int SyntacticalAnalyzer::stmt_list()
{
  lex->debug << "            stmt_list function called";
  p2file << "Starting <stmt_list>. Current token = " + lex->GetTokenName(token);
  p2file << "\n";
  int errors = 0;
  
  switch(token){
  case LPAREN_T:
    p2file << "Using rule 5\n";
    lex->debug << " on rule 5\n";
    //token = lex->GetToken();
    errors += stmt();
    break;
    
  case IDENT_T:
    p2file << "Using rule 5\n";
    lex->debug << " on rule 5\n";
    //token = lex->GetToken();
    errors += stmt();
    break;
    
  case RPAREN_T:
    p2file << "Using rule 6\n";
    lex->debug << " on rule 6\n";
    //token = lex->GetToken();
    errors += stmt();
    break;
    
  case NUMLIT_T:
    p2file << "Using rule 5\n";
    lex->debug << " on rule 5\n";
    //token = lex->GetToken();
    errors += stmt();
    break;
    
  case QUOTE_T:
    p2file << "Using rule 5\n";
    lex->debug << " on rule 5\n";
    //token = lex->GetToken();
    errors += stmt();
    break;
    
  default:
    errors++;
    lex->ReportError("Expected left or right parenthesis, number, quote, or an identifier; " + lex->GetLexeme());
  }
  
  lex->debug << "            stmt_list function returning " << errors << " errors\n";
  p2file << "Ending <stmt_list>. Current token = " + lex->GetTokenName(token);
  p2file << ". Errors = " << errors;
  p2file << "\n";
  return errors;
}

int SyntacticalAnalyzer::literal()
{
  lex->debug << "            literal function called";
  p2file << "Starting <literal>. Current token = " + lex->GetTokenName(token);
  p2file << "\n";
  int errors = 0;
  
  switch(token){
  case NUMLIT_T:
    p2file << "Using rule 10\n";
    lex->debug << " on rule 10\n";
    token = lex->GetToken();
    break;
    
  case QUOTE_T:
    p2file << "Using rule 11\n";
    lex->debug << " on rule 11\n";
    token = lex->GetToken();
    errors += quoted_lit();
    break;
    
  default:
    errors++;
    lex->ReportError("Expecting a quote or a number; " + lex->GetLexeme());
  }
  
  lex->debug << "            literal function returning " << errors << " errors\n";
  p2file << "Ending <literal>. Current token = " + lex->GetTokenName(token);
  p2file << ". Errors = " << errors;
  p2file << "\n";
  return errors;
}

int SyntacticalAnalyzer::quoted_lit()
{
  lex->debug << "            quoted_lit function called";
  p2file << "Starting <quoted_lit>. Current token = " + lex->GetTokenName(token);
  p2file << "\n";
  int errors = 0;
  
  switch(token){
  case EOF_T:
    errors++;
    lex->ReportError("Expecting any_other_token; " + lex->GetLexeme());
    break;
    
  case RPAREN_T:
    errors++;
    lex->ReportError("Expecting any_other_token; " + lex->GetLexeme());
    break;
    
  default:
    p2file << "Using rule 12\n";
    lex->debug << " on rule 12\n";
    //token = lex->GetToken();
    errors += any_other_token();
  }
  
  lex->debug << "            quoted_lit function returning " << errors << " errors\n";
  p2file << "Ending <quoted_lit>. Current token = " + lex->GetTokenName(token);
  p2file << ". Errors = " << errors;
  p2file << "\n";
  return errors;
}

int SyntacticalAnalyzer::more_tokens()
{
  lex->debug << "            more_tokens function called";
  p2file << "Starting <more_tokens>. Current token = " + lex->GetTokenName(token);
  p2file << "\n";
  int errors = 0;
  
  switch(token){
  case RPAREN_T:
    p2file << "Using rule 14\n";
    lex->debug << " on rule 14\n";
    token = lex->GetToken();
    lex->debug << "Current Token: " + token << endl;
    break;
    
    /*        case IF_T:
	      errors++;
	      lex->ReportError("Unexpected token; " + lex->GetLexeme());
	      break;
	      
	      case LISTOP_T:
	      errors++;
	      lex->ReportError("Unexpected token; " + lex->GetLexeme());
	      break; */
    
  case EOF_T:
    errors++;
    lex->ReportError("Unexpected token; " + lex->GetLexeme());
    break;
    
  default:
    p2file << "Using rule 13\n";
    lex->debug << " on rule 13\n";
    //token = lex->GetToken();
    errors += any_other_token();
    errors += more_tokens();
  }    
  
  lex->debug << "            more_tokens function returning " << errors << " errors\n";
  p2file << "Ending <more_tokens>. Current token = " + lex->GetTokenName(token);
  p2file << ". Errors = " << errors;
  p2file << "\n";
  return errors;
}


int SyntacticalAnalyzer::any_other_token()
{
  lex->debug << "            any_other_token function called";
  p2file << "Starting <any_other_token>. Current token = " + lex->GetTokenName(token);
  p2file << "\n";
  int errors = 0;
  
  switch(token){
  case LPAREN_T:
    p2file << "Using rule 44\n";
    lex->debug << " on rule 44\n";
    token = lex->GetToken();
    errors += more_tokens();
    if(token != RPAREN_T){
      errors++;
      lex->ReportError("Expected right parenthesis; " + lex->GetLexeme());
    }
    break;
    
  case IDENT_T:
    p2file << "Using rule 45\n";
    lex->debug << " on rule 45\n";
    token = lex->GetToken();
    break;
    
  case NUMLIT_T:
    p2file << "Using rule 46\n";
    lex->debug << " on rule 46\n";
    token = lex->GetToken();
    break;
    
  case CONS_T:
    p2file << "Using rule 47\n";
    lex->debug << " on rule 47\n";
    token = lex->GetToken();
    break;
    
  case IF_T:
    p2file << "Using rule 48\n";
    lex->debug << " on rule 48\n";
    token = lex->GetToken();
    break;
    
  case DISPLAY_T:
    p2file << "Using rule 49\n";
    lex->debug << " on rule 49\n";
    token = lex->GetToken();
    break;
    
  case NEWLINE_T:
    p2file << "Using rule 50\n";
    lex->debug << " on rule 50\n";
    token = lex->GetToken();
    break;
    
  case LISTOP_T:
    p2file << "Using rule 51\n";
    lex->debug << " on rule 51\n";
    token = lex->GetToken();
    break;
    
  case AND_T:
    p2file << "Using rule 52\n";
    lex->debug << " on rule 52\n";
    token = lex->GetToken();
    break;
    
  case OR_T:
    p2file << "Using rule 53\n";
    lex->debug << " on rule 53\n";
    token = lex->GetToken();
    break;
    
  case NOT_T:
    p2file << "Using rule 54\n";
    lex->debug << " on rule 54\n";
    token = lex->GetToken();
    break;
    
  case DEFINE_T:
    p2file << "Using rule 55\n";
    lex->debug << " on rule 55\n";
    token = lex->GetToken();
    break;
    
  case NUMBERP_T:
    p2file << "Using rule 56\n";
    lex->debug << " on rule 56\n";
    token = lex->GetToken();
    break;
    
  case SYMBOLP_T:
    p2file << "Using rule 57\n";
    lex->debug << " on rule 57\n";
    token = lex->GetToken();
    break;
    
  case LISTP_T:
    p2file << "Using rule 58\n";
    lex->debug << " on rule 58\n";
    token = lex->GetToken();
    break;
    
  case ZEROP_T:
    p2file << "Using rule 59\n";
    lex->debug << " on rule 59\n";
    token = lex->GetToken();
    break;
    
  case NULLP_T:
    p2file << "Using rule 60\n";
    lex->debug << " on rule 60\n";
    token = lex->GetToken();
    break;
    
  case CHARP_T:
    p2file << "Using rule 61\n";
    lex->debug << " on rule 61\n";
    token = lex->GetToken();
    break;
    
  case STRINGP_T:
    p2file << "Using rule 62\n";
    lex->debug << " on rule 62\n";
    token = lex->GetToken();
    break;
    
  case PLUS_T:
    p2file << "Using rule 63\n";
    lex->debug << " on rule 63\n";
    token = lex->GetToken();
    break;
    
  case MINUS_T:
    p2file << "Using rule 64\n";
    lex->debug << " on rule 64\n";
    token = lex->GetToken();
    break;
    
  case DIV_T:
    p2file << "Using rule 65\n";
    lex->debug << " on rule 65\n";
    token = lex->GetToken();
    break;
    
  case MULT_T:
    p2file << "Using rule 66\n";
    lex->debug << " on rule 66\n";
    token = lex->GetToken();
    break;
    
  case EQUALTO_T:
    p2file << "Using rule 67\n";
    lex->debug << " on rule 67\n";
    token = lex->GetToken();
    break;
    
  case GT_T:
    p2file << "Using rule 68\n";
    lex->debug << " on rule 68\n";
    token = lex->GetToken();
    break;
    
  case LT_T:
    p2file << "Using rule 69\n";
    lex->debug << " on rule 69\n";
    token = lex->GetToken();
    break;
    
  case GTE_T:
    p2file << "Using rule 70\n";
    lex->debug << " on rule 70\n";
    token = lex->GetToken();
    break;
    
  case LTE_T:
    p2file << "Using rule 71\n";
    lex->debug << " on rule 71\n";
    token = lex->GetToken();
    break;
    
  case QUOTE_T:
    p2file << "Using rule 72\n";
    lex->debug << " on rule 72\n";
    token = lex->GetToken();
    break;
    
  default:
    errors++;
    lex->ReportError("Unexpected token; " + lex->GetLexeme());
    
  }
  
  lex->debug << "            any_other_token function returning " << errors << " errors\n";
  p2file << "Ending <any_other_token>. Current token = " + lex->GetTokenName(token);
  p2file << ". Errors = " << errors;
  p2file << "\n";
  return errors;
}

int SyntacticalAnalyzer::else_part()
{
  lex->debug << "            else_part function called";
  p2file << "Starting <else_part>. Current token = " + lex->GetTokenName(token);
  p2file << "\n";
  int errors = 0;
  
  switch(token){
  case RPAREN_T:
    p2file << "Using rule 18\n";
    lex->debug << " on rule 18\n";
    token = lex->GetToken();
    break;
    
  default:
    p2file << "Using rule 17\n";
    lex->debug << " on rule 17\n";
    token = lex->GetToken();
    errors += stmt();
  }
  
  lex->debug << "            else_part function returning " << errors << " errors\n";
  p2file << "Ending <else_part>. Current token = " + lex->GetTokenName(token);
  p2file << ". Errors = " << errors;
  p2file << "\n";
  return errors;
}

int SyntacticalAnalyzer::action()
{
  lex->debug << "            action function called";
  p2file << "Starting <action>. Current token = " + lex->GetTokenName(token);
  p2file << "\n";
  int errors = 0;
  
  switch(token){  
  case IDENT_T:
    p2file << "Using rule 41\n";
    lex->debug << " on rule 41\n";
    token = lex->GetToken();
    errors += stmt_list();
    break;
    
  case IF_T:
    p2file << "Using rule 19\n";
    lex->debug << " on rule 19\n";
    token = lex->GetToken();
    errors += stmt();
    errors += stmt();
    errors += else_part();
    break;
    
  case LISTOP_T:
    p2file << "Using rule 20\n";
    lex->debug << " on rule 20\n";
    token = lex->GetToken();
    errors += stmt();
    break;
    
  case CONS_T:
    p2file << "Using rule 21\n";
    lex->debug << " on rule 21\n";
    token = lex->GetToken();
    errors += stmt();
    errors += stmt();
    break;
    
  case AND_T:
    p2file << "Using rule 22\n";
    lex->debug << " on rule 22\n";
    token = lex->GetToken();
    errors += stmt_list();
    break;
    
  case OR_T:
    p2file << "Using rule 23\n";
    lex->debug << " on rule 23\n";
    token = lex->GetToken();
    errors += stmt_list();
    break;
    
  case NOT_T:
    p2file << "Using rule 24\n";
    lex->debug << " on rule 24\n";
    token = lex->GetToken();
    errors += stmt();
    break;
    
  case NUMBERP_T:
    p2file << "Using rule 25\n";
    lex->debug << " on rule 25\n";
    token = lex->GetToken();
    errors += stmt();
    break;
    
  case SYMBOLP_T:
    p2file << "Using rule 26\n";
    lex->debug << " on rule 26\n";
    token = lex->GetToken();
    errors += stmt();
    break;
    
  case LISTP_T:
    p2file << "Using rule 27\n";
    lex->debug << " on rule 27\n";
    token = lex->GetToken();
    errors += stmt();
    break;
    
  case ZEROP_T:
    p2file << "Using rule 28\n";
    lex->debug << " on rule 28\n";
    token = lex->GetToken();
    errors += stmt();
    break;
    
  case NULLP_T:
    p2file << "Using rule 29\n";
    lex->debug << " on rule 29\n";
    token = lex->GetToken();
    errors += stmt();
    break;
    
  case CHARP_T:
    p2file << "Using rule 30\n";
    lex->debug << " on rule 30\n";
    token = lex->GetToken();
    errors += stmt();
    break;
    
  case STRINGP_T:
    p2file << "Using rule 31\n";
    lex->debug << " on rule 31\n";
    token = lex->GetToken();
    errors += stmt();
    break;
                                    
  case PLUS_T:
    p2file << "Using rule 32\n";
    lex->debug << " on rule 32\n";
    token = lex->GetToken();
    errors += stmt_list();
    break;
    
  case DISPLAY_T:
    p2file << "Using rule 42\n";
    lex->debug << " on rule 42\n";
    token = lex->GetToken();
    errors += stmt();
    break;
    
  case NEWLINE_T:
    p2file << "Using rule 43\n";
    lex->debug << " on rule 43\n";
    token = lex->GetToken();
    break;
                                    
  case MINUS_T:
    p2file << "Using rule 33\n";
    lex->debug << " on rule 33\n";
    token = lex->GetToken();
    errors += stmt();
    errors += stmt_list();
    break;
                                    
  case DIV_T:
    p2file << "Using rule 34\n";
    lex->debug << " on rule 34\n";
    token = lex->GetToken();
    errors += stmt();
    errors += stmt_list();
    break;
    
  case MULT_T:
    p2file << "Using rule 35\n";
    lex->debug << " on rule 35\n";
    token = lex->GetToken();
    errors += stmt_list();
    break;
    
  case EQUALTO_T:
    p2file << "Using rule 36\n";
    lex->debug << " on rule 36\n";
    token = lex->GetToken();
    errors += stmt_list();
    break;
    
  case GT_T:
    p2file << "Using rule 37\n";
    lex->debug << " on rule 37\n";
    token = lex->GetToken();
    errors += stmt_list();
    break;
    
  case LT_T:
    p2file << "Using rule 38\n";
    lex->debug << " on rule 38\n";
    token = lex->GetToken();
    errors += stmt_list();
    break;
    
  case GTE_T:
    p2file << "Using rule 30\n";
    lex->debug << " on rule 30\n";
    token = lex->GetToken();
    errors += stmt_list();
    break;
    
  case LTE_T:
    p2file << "Using rule 40\n";
    lex->debug << " on rule 40\n";
    token = lex->GetToken();
    errors += stmt_list();
    break;
  }
  
  lex->debug << "            action function returning " << errors << " errors\n";
  p2file << "Ending <action>. Current token = " + lex->GetTokenName(token);
  p2file << ". Errors = " << errors;
  p2file << "\n";
  return errors;
}

