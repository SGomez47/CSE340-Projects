#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include "compiler.h"
#include "lexer.h"
#include "parser.h"

string tokenString[] =
{
    "END_OF_FILE",
    "VAR", "FOR", "IF", "WHILE", "SWITCH", "CASE", "DEFAULT", "INPUT", "OUTPUT", "ARRAY",
    "PLUS", "MINUS", "DIV", "MULT",
    "EQUAL", "COLON", "COMMA", "SEMICOLON",
    "LBRAC", "RBRAC", "LPAREN", "RPAREN", "LBRACE", "RBRACE",
    "NOTEQUAL", "GREATER", "LESS",
    "NUM", "ID", "ERROR"
};

void Parser::syntax_error()
{
    debug("SYNTAX ERROR !!!");
    exit(EXIT_FAILURE);
}

void Parser::expect(TokenType token)
{
    Token t = lexer.peek(1);
    if (token == t.token_type)
    {
        t = lexer.GetToken();
    }
    else
    {
        syntax_error();
    }
}

/* Parser */
//-------------------------------------------------------------------------------------------------
void Parser::parse_program()
{
    parse_var();
    parse_bracket();
    parse_inputs();
}

void Parser::parse_var()
{
    parse_id_list();
    Token t = lexer.peek(1);
    if (t.token_type == SEMICOLON)
    {
        expect(SEMICOLON);
    }

    else syntax_error();
}

void Parser::parse_bracket()
{
    Token t = lexer.peek(1);
    if (t.token_type == LBRACE)
    {
        expect(LBRACE);

        parse_stmt_list();

        t = lexer.peek(1);
        if (t.token_type == RBRACE)
        {
            expect(RBRACE);
        }

        else syntax_error();
    }

    else syntax_error();
}

void Parser::parse_inputs()
{
    parse_num_list();
}

void Parser::parse_id_list()
{
    Token t = lexer.peek(1);
    if (t.token_type == ID)
    {
        expect(ID);

        if (next_available < 1000)
        {
            variables[next_available] = t.lexeme;
            mem[next_available] = 0;
            next_available++;
        }

        t = lexer.peek(1);
        if (t.token_type == COMMA)
        {
            expect(COMMA);
            parse_id_list();
        }
    }
    else syntax_error();
}

void Parser::parse_stmt_list()
{
    parse_stmt();

    Token t = lexer.peek(1);
    if (t.token_type == ID ||
        t.token_type == OUTPUT ||
        t.token_type == INPUT ||
        t.token_type == WHILE ||
        t.token_type == IF ||
        t.token_type == SWITCH ||
        t.token_type == FOR){

        parse_stmt_list();
    }
}

void Parser::parse_stmt()
{
    Token t = lexer.peek(1);
    if (t.token_type == ID) { 
        parse_assign_stmt() 
    }
    else if (t.token_type == OUTPUT) { 
        parse_output_stmt() 
    }
    else if (t.token_type == INPUT) { 
        parse_input_stmt() 
    }
    else if (t.token_type == WHILE) { 
        parse_while() 
    }
    else if (t.token_type == IF) { 
        parse_if() 
    }
    else if (t.token_type == SWITCH) {
        parse_switch()
    }
    else if (t.token_type == FOR) { 
        parse_for() 
    }
    else
    {
        debug("SYNTAX ERROR !!!");
        exit(EXIT_FAILURE);
    }
}

void Parser::parse_assign_stmt()
{
    Token t = lexer.peek(1);
    if (t.token_type == ID)
    {
        expect(ID);

        t = lexer.peek(1);
        if (t.token_type == EQUAL)
        {
            expect(EQUAL);

            t = lexer.peek(1);
            Token t2 = lexer.peek(2);
            if (t2.token_type == SEMICOLON)
            {
                parse_primary();
            }
            else
            {
                parse_expr();
            }

            t = lexer.peek(1);
            if (t.token_type == SEMICOLON)
            {
                expect(SEMICOLON);
            }
            else syntax_error(SEMICOLON, t);
        }
        else syntax_error(EQUAL, t);
    }
    else syntax_error(ID, t);
}

void Parser::parse_for()
{
    Token t = lexer.peek(1);
    if (t.token_type == FOR)
    {
        expect(FOR);

        t = lexer.peek(1);
        if (t.token_type == LPAREN)
        {
            expect(LPAREN);
            parse_assign_stmt();
            parse_condition();

            t = lexer.peek(1);
            if (t.token_type == SEMICOLON)
            {
                expect(SEMICOLON);

                parse_assign_stmt();

                t = lexer.peek(1);
                if (t.token_type == RPAREN)
                {
                    expect(RPAREN);

                    parse_bracket();
                }
                else syntax_error(RPAREN, t);
            }
            else syntax_error(SEMICOLON, t);
        }
        else syntax_error(LPAREN, t);
    }
    else syntax_error(FOR, t);
}

void Parser::parse_if()
{
    Token t = lexer.peek(1);
    if (t.token_type == IF)
    {
        expect(IF);
        parse_condition();
        parse_bracket();
    }
    else syntax_error();
}

void Parser::parse_while()
{
    Token t = lexer.peek(1);
    if (t.token_type == WHILE)
    {
        expect(WHILE);
        parse_condition();
        parse_bracket();
    }
    else syntax_error();
}


void Parser::parse_switch()
{
    Token t = lexer.peek(1);
    if (t.token_type == SWITCH)
    {
        expect(SWITCH);

        t = lexer.peek(1);
        if (t.token_type == ID)
        {
            expect(ID);

            t = lexer.peek(1);
            if (t.token_type == LBRACE)
            {
                expect(LBRACE);

                parse_case_list();

                t = lexer.peek(1);
                if (t.token_type == DEFAULT)
                {
                    parse_default_case();

                    t = lexer.peek(1);
                    if (t.token_type == RBRACE)
                    {
                        expect(RBRACE);
                    }
                    else syntax_error();
                }
                else if (t.token_type == RBRACE)
                {
                    expect(RBRACE);
                }
                else syntax_error();
            }
            else syntax_error();
        }
        else syntax_error();
    }
    else syntax_error();
}

void Parser::parse_input_stmt()
{
    Token t = lexer.peek(1);
    if (t.token_type == INPUT)
    {
        expect(INPUT);

        t = lexer.peek(1);
        if (t.token_type == ID)
        {
            expect(ID);

            t = lexer.peek(1);
            if (t.token_type == SEMICOLON)
            {
                expect(SEMICOLON);
            }
            else syntax_error();
        }
        else syntax_error();
    }
    else syntax_error();
}

void Parser::parse_output_stmt()
{
    Token t = lexer.peek(1);
    if (t.token_type == OUTPUT)
    {
        expect(OUTPUT);

        t = lexer.peek(1);
        if (t.token_type == ID)
        {
            expect(ID);

            t = lexer.peek(1);
            if (t.token_type == SEMICOLON)
            {
                expect(SEMICOLON);
            }
            else syntax_error();
        }
        else syntax_error();
    }
    else syntax_error();
}

void Parser::parse_primary()
{
    Token t = lexer.peek(1);
    if (t.token_type == ID)
    {
        expect(ID);
    }
    else if (t.token_type == NUM)
    {
        expect(NUM);
    }
    else
    {
        debug("SYNTAX ERROR !!!");
        exit(EXIT_FAILURE);
    }
}

void Parser::parse_exp()
{
    parse_primary();
    parse_operations();
    parse_primary();
}

void Parser::parse_operations()
{
    Token t = lexer.peek(1);
    if (t.token_type == PLUS)
    {
        expect(PLUS);
    }
    else if (t.token_type == MINUS)
    {
        expect(MINUS);
    }
    else if (t.token_type == MULT)
    {
        expect(MULT);
    }
    else if (t.token_type == DIV)
    {
        expect(DIV);
    }
    else
    {
        debug("SYNTAX ERROR !!!");
        exit(EXIT_FAILURE);
    }
}

void Parser::parse_condition()
{
    parse_primary();
    parse_relop();
    parse_primary();
}

void Parser::parse_relop()
{
    Token t = lexer.peek(1);
    if (t.token_type == GREATER)
    {
        expect(GREATER);
    }
    else if (t.token_type == LESS)
    {
        expect(LESS);
    }
    else if (t.token_type == NOTEQUAL)
    {
        expect(NOTEQUAL);
    }
    else
    {
        debug("SYNTAX ERROR !!!);
        exit(EXIT_FAILURE);
    }
}

void Parser::parse_case_list()
{
    parse_case();

    Token t = lexer.peek(1);
    if (t.token_type == CASE)
    {
        parse_case_list();
    }
}

void Parser::parse_default_case()
{
    Token t = lexer.peek(1);
    if (t.token_type == DEFAULT)
    {
        expect(DEFAULT);

        t = lexer.peek(1);
        if (t.token_type == COLON)
        {
            expect(COLON);
            parse_bracket();
        }
        else syntax_error();
    }
    else syntax_error();
}

void Parser::parse_case()
{
    Token t = lexer.peek(1);
    if (t.token_type == CASE)
    {
        expect(CASE);

        t = lexer.peek(1);
        if (t.token_type == NUM)
        {
            expect(NUM);

            t = lexer.peek(1);
            if (t.token_type == COLON)
            {
                expect(COLON);
                parse_bracket();
            }
            else syntax_error();
        }
        else syntax_error();
    }
    else syntax_error();
}

void Parser::parse_num_list()
{
    Token t = lexer.peek(1);
    if (t.token_type == NUM)
    {
        expect(NUM);

        /* Deal with Input section at the end */
        int input = stoi(t.lexeme);
        inputs.insert(inputs.end(), input);

        t = lexer.peek(1);
        if (t.token_type == NUM)
        {
            parse_num_list();
        }
    }
    else syntax_error();
}


struct InstructionNode * parse_generate_intermediate_representation()
{
    Parser parser;
    parser.parse_program();
}