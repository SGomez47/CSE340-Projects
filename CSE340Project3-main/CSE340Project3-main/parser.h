#ifndef __PARSER__H__
#define __PARSER__H__

#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>

#include "compiler.h"
#include "lexer.h"

using namespace std;

class Parser
{
    private:
        
    public:
        string variables[1000];
        LexicalAnalyzer lexer;
        struct InstructionNode* head;
        struct InstructionNode* currIn;

        void syntax_error();
		void expect(TokenType token);

        void parse_program();
        void parse_var();
        void parse_bracket();
        void parse_inputs();
        void parse_id_list();
        void parse_stmt_list();
        void parse_stmt();
        void parse_assign_stmt();
        void parse_for();
        void parse_if();
        void parse_while();
        void parse_switch();
        void parse_input();
        void parse_output();
        void parse_primary();
        void parse_exp();
        void parse_operations();
        void parse_condition();
        void parse_relop();
        void parse_case_list();
        void parse_default_case();
        void parse_case();
        void parse_num_list();

};

#endif  //__PARSER__H__