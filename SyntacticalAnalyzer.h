/********************************************************************************/
/* Project: Project 2									*/
/* Author: Brian Crosser, Kevin Gantsweg, Michelle Padgett	        			*/
/* Date: Spring 2017									*/
/* Description:									*/
/********************************************************************************/

#ifndef SYN_H
#define SYN_H

#include <iostream>
#include <fstream>
#include "LexicalAnalyzer.h"

using namespace std;

class SyntacticalAnalyzer 
{
    public:
	SyntacticalAnalyzer (char * filename);
	~SyntacticalAnalyzer ();
    private:
	LexicalAnalyzer * lex;
	ofstream p2file;
	token_type token;
	int program ();
    int define();
    int more_defines();
    int param_list();
    int stmt();
    int stmt_list();
    int literal();
    int quoted_lit();
    int any_other_token();
    int else_part();
    int action();
    int more_tokens();
};
	
#endif
