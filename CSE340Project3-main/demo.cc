#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include "compiler.h"
#include "lexer.h"
//#include "parser.h"

using namespace std;

LexicalAnalyzer lexer;
string variables[1000];

void expect(TokenType token);

struct InstructionNode* parse_program();
void parse_var();
struct InstructionNode* parse_bracket(); //BODY
void parse_inputs();
void parse_id_list();
struct InstructionNode* parse_stmt_list();
struct InstructionNode* parse_stmt();
struct InstructionNode* parse_assign_stmt();
struct InstructionNode* parse_for();
struct InstructionNode* parse_if();
struct InstructionNode* parse_while();
struct InstructionNode* parse_switch();
void parse_input();
void parse_output();
void parse_primary();
void parse_exp();
void parse_operations();
void parse_condition();
int parse_relop();
void parse_case_list();
void parse_default_case();
void parse_case();
void parse_num_list();
void find_location(string lex);
void add_mem(Token t);

void expect(TokenType token)
{
    Token t = lexer.peek(1);
    if (token == t.token_type)
    {
        t = lexer.GetToken();
    }
}

// Parser
struct InstructionNode* parse_program()
{
    struct InstructionNode* inst;
    parse_var();
    inst = parse_bracket();
    parse_inputs();
    return inst;
}

void parse_var()
{
    parse_id_list();
    Token t = lexer.peek(1);
    if (t.token_type == SEMICOLON)
    {
        expect(SEMICOLON);
    }

}

struct InstructionNode* parse_bracket(){ //BODY!!!
    struct InstructionNode* inst1;
    Token t = lexer.peek(1);
    if (t.token_type == LBRACE)
    {
        expect(LBRACE);

        inst1 = parse_stmt_list();

        t = lexer.peek(1);
        if (t.token_type == RBRACE)
        {
            expect(RBRACE);
        }
        return inst1;
    }

}

void parse_inputs()
{
    parse_num_list();
}

void parse_id_list()
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
}

struct InstructionNode* parse_stmt_list()
{
	struct InstructionNode* inst1;
    inst1 = parse_stmt();

    Token t = lexer.peek(1);
    if (t.token_type == ID ||
        t.token_type == OUTPUT ||
        t.token_type == INPUT ||
        t.token_type == WHILE ||
        t.token_type == IF ||
        t.token_type == SWITCH ||
        t.token_type == FOR) {

       inst1->next = parse_stmt_list();
       
    }
    return inst1;
}

struct InstructionNode* parse_stmt()
{
    Token t = lexer.peek(1);
    if (t.token_type == ID) {
        return parse_assign_stmt();
    }
    else if (t.token_type == OUTPUT) {
        return parse_output_stmt();
    }
    else if (t.token_type == INPUT) {
        return parse_input_stmt();
    }
    else if (t.token_type == WHILE) {
        return parse_while();
    }
    else if (t.token_type == IF) {
        return parse_if();
    }
    else if (t.token_type == SWITCH) {
        return parse_switch();
    }
    else if (t.token_type == FOR) {
        return parse_for();
    }
}

void parse_assign_stmt()
{
    struct InstructionNode* temp = new InstructionNode;
    temp->type = ASSIGN;
    temp->next = nullptr;
    Token t = lexer.peek(1);
    if (t.token_type == ID)
    {
        expect(ID);
        temp->assign_inst.left_hand_side_index = find_location(t.lexeme);
       
        t = lexer.peek(1);
        if (t.token_type == EQUAL)
        {
            expect(EQUAL);

            t = lexer.peek(1);
            Token t2 = lexer.peek(2);
            if (t2.token_type == SEMICOLON)
            {
                temp->assign_inst.operand1_index = parse_primary();
                temp->assign_inst.op = OPERATOR_NONE;
            }
            else
            {
                parse_exp(temp);
            }


            t = lexer.peek(1);
            if (t.token_type == SEMICOLON)
            {
                expect(SEMICOLON);
                return temp;
            }
        }
    }
}

/*
struct InstructionNode* parse_if()
{
    struct InstructionNode* inst1 = new InstructionNode;
    Token t = lexer.peek(1);
    if (t.token_type == IF)
    {
        expect(IF);
        inst1->type = CJMP;
        parse_condition();
        parse_bracket();
    }
}

void parse_for()
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
            }
        }
    }
}

void parse_while()
{
    Token t = lexer.peek(1);
    if (t.token_type == WHILE)
    {
        expect(WHILE);
        parse_condition();
        parse_bracket();
    }
}


void parse_switch()
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
                }
                else if (t.token_type == RBRACE)
                {
                    expect(RBRACE);
                }
            }
        }
    }
}
*/
struct InstructionNode* parse_input_stmt()
{
    struct InstructionNode* inst1 = new InstructionNode;
    Token t = lexer.peek(1);
    if (t.token_type == INPUT)
    {
        expect(INPUT);
        inst1->type = IN;

        t = lexer.peek(1);
        if (t.token_type == ID)
        {
            expect(ID);
            inst1->input_inst.var_index = find_location(t.lexeme);
            inst1->next = nullptr;
            t = lexer.peek(1);
            if (t.token_type == SEMICOLON)
            {
                expect(SEMICOLON);
                return inst1;
            }
        }
    }
}

struct InstructionNode* parse_output_stmt()
{
    struct InstructionNode* inst1 = new InstructionNode;

    Token t = lexer.peek(1);
    if (t.token_type == OUTPUT)
    {
        expect(OUTPUT);
        inst1->type = OUT;

        t = lexer.peek(1);
        if (t.token_type == ID)
        {
            expect(ID);
            inst1->input_inst.var_index = find_location(t.lexeme);
            inst1->next = nullptr;
            t = lexer.peek(1);
            if (t.token_type == SEMICOLON)
            {
                expect(SEMICOLON);
                return inst1;
            }
        }
    }
}

int parse_primary()
{
    Token t = lexer.peek(1);
    if (t.token_type == ID)
    {
        expect(ID);
        return find_location(t.lexeme);
    }
    else if (t.token_type == NUM)
    {
        expect(NUM);
        add_mem(t);
        return find_location(t.lexeme);

    }
}

void add_mem(Token t) {
    if (t.token_type == NUM) {
        variables[next_available] = t.lexeme;
        mem[next_available] = stoi(t.lexeme);
        next_available++;
   }
   
}

void parse_exp(struct InstructionNode* inst)
{
    inst->assign_inst.operand1_index = parse_primary();
    inst->assign_inst.op = parse_operations();
    temp->assign_inst.operand2_index = parse_primary();
}

ArithmeticOperatorType parse_operations()
{
    
    Token t = lexer.peek(1);
    if (t.token_type == PLUS)
    {
        expect(PLUS);
        return OPERATOR_PLUS;
        
    }
    else if (t.token_type == MINUS)
    {
        expect(MINUS);
        return OPERATOR_MINUS;
    }
    else if (t.token_type == MULT)
    {
        expect(MULT);
        return MULT;
    }
    else if (t.token_type == DIV)
    {
        expect(DIV);
        return DIV;
    }
}

void parse_condition()
{
    parse_primary();
    parse_relop();
    parse_primary();
}

int parse_relop()
{
    Token t = lexer.peek(1);
    if (t.token_type == GREATER)
    {
        expect(GREATER);
        return 345;
    }
    else if (t.token_type == LESS)
    {
        expect(LESS);
        return 346;
    }
    else if (t.token_type == NOTEQUAL)
    {
        expect(NOTEQUAL);
        return 347;
    }

}

void parse_case_list()
{
    parse_case();

    Token t = lexer.peek(1);
    if (t.token_type == CASE)
    {
        parse_case_list();
    }
}

void parse_default_case()
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
    }
}

void parse_case()
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
        }
    }
}

void parse_num_list()
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
}

int find_location(string lex) {
    for (int i = 0; i < 1000; i++) {
        if (lex.compare(variabales[i])) {
            return i;
        }
    }
}

struct InstructionNode * parse_generate_intermediate_representation()
{
    // Sample program for demonstration purpose only
    // Replace the following with a parser that reads the program from stdin &
    // creates appropriate data structures to be executed by execute_program()
    // This is the imaginary input for the following construction:
    
    // a, b, c, d;
    // {
    //     input a;
    //     input b;
    //     c = 10;
    // 
    //     IF c <> a
    //     {
    //         output b;
    //     }
    // 
    //     IF c > 1
    //     {
    //         a = b + 900;
    //         input d;
    //         IF a > 10
    //         {
    //             output d;
    //         }
    //     }
    // 
    //     d = 0;
    //     WHILE d < 4
    //     {
    //         c = a + d;
    //         IF d > 1
    //         {
    //             output d;
    //         }
    //         d = d + 1;
    //     }
    // }
    // 1 2 3 4 5 6

    
    parse_program();

    // Assigning location for variable "a"
    int address_a = next_available;
    mem[next_available] = 0;
    next_available++;

    // Assigning location for variable "b"
    int address_b = next_available;
    mem[next_available] = 0;
    next_available++;

    // Assigning location for variable "c"
    int address_c = next_available;
    mem[next_available] = 0;
    next_available++;

    // Assigning location for variable "d"
    int address_d = next_available;
    mem[next_available] = 0;
    next_available++;

    // Assigning location for constant 10
    int address_ten = next_available;
    mem[next_available] = 10;
    next_available++;

    // Assigning location for constant 1
    int address_one = next_available;
    mem[next_available] = 1;
    next_available++;

    // Assigning location for constant 900
    int address_ninehundred = next_available;
    mem[next_available] = 900;
    next_available++;

    // Assigning location for constant 3
    int address_three = next_available;
    mem[next_available] = 3;
    next_available++;

    // Assigning location for constant 0
    int address_zero = next_available;
    mem[next_available] = 0;
    next_available++;

    // Assigning location for constant 4
    int address_four = next_available;
    mem[next_available] = 4;
    next_available++;

    struct InstructionNode * i1 = new InstructionNode;
    struct InstructionNode * i2 = new InstructionNode;
    struct InstructionNode * i3 = new InstructionNode;
    struct InstructionNode * i4 = new InstructionNode;
    struct InstructionNode * i5 = new InstructionNode;
    struct InstructionNode * i6 = new InstructionNode;
    struct InstructionNode * i7 = new InstructionNode;
    struct InstructionNode * i8 = new InstructionNode;
    struct InstructionNode * i9 = new InstructionNode;
    struct InstructionNode * i10 = new InstructionNode;
    struct InstructionNode * i11 = new InstructionNode;
    struct InstructionNode * i12 = new InstructionNode;
    struct InstructionNode * i13 = new InstructionNode;
    struct InstructionNode * i14 = new InstructionNode;
    struct InstructionNode * i15 = new InstructionNode;
    struct InstructionNode * i16 = new InstructionNode;
    struct InstructionNode * i17 = new InstructionNode;
    struct InstructionNode * i18 = new InstructionNode;
    struct InstructionNode * i19 = new InstructionNode;
    struct InstructionNode * i20 = new InstructionNode;
    struct InstructionNode * i21 = new InstructionNode;
    struct InstructionNode * i22 = new InstructionNode;

    i1->type = IN;                                      // input a
    i1->input_inst.var_index = address_a;
    i1->next = i2;

    i2->type = IN;                                      // input b
    i2->input_inst.var_index = address_b;
    i2->next = i3;

    i3->type = ASSIGN;                                  // c = 10
    i3->assign_inst.left_hand_side_index = address_c;
    i3->assign_inst.op = OPERATOR_NONE;
    i3->assign_inst.operand1_index = address_ten;
    i3->next = i4;

    i4->type = CJMP;                                    // if c <> a
    i4->cjmp_inst.condition_op = CONDITION_NOTEQUAL;
    i4->cjmp_inst.operand1_index = address_c;
    i4->cjmp_inst.operand2_index = address_a;
    i4->cjmp_inst.target = i6;                          // if not (c <> a) skip forward to NOOP
    i4->next = i5;

    i5->type = OUT;                                     // output b
    i5->output_inst.var_index = address_b;
    i5->next = i6;

    i6->type = NOOP;                                    // NOOP after IF
    i6->next = i7;

    i7->type = CJMP;                                    // if c > 1
    i7->cjmp_inst.condition_op = CONDITION_GREATER;
    i7->cjmp_inst.operand1_index = address_c;
    i7->cjmp_inst.operand2_index = address_one;
    i7->cjmp_inst.target = i13;                         // if not (c > 1) skip forward to NOOP (way down)
    i7->next = i8;

    i8->type = ASSIGN;                                  // a = b + 900
    i8->assign_inst.left_hand_side_index = address_a;
    i8->assign_inst.op = OPERATOR_PLUS;
    i8->assign_inst.operand1_index = address_b;
    i8->assign_inst.operand2_index = address_ninehundred;
    i8->next = i9;

    i9->type = IN;                                      // input d
    i9->input_inst.var_index = address_d;
    i9->next = i10;

    i10->type = CJMP;                                   // if a > 10
    i10->cjmp_inst.condition_op = CONDITION_GREATER;
    i10->cjmp_inst.operand1_index = address_a;
    i10->cjmp_inst.operand2_index = address_ten;
    i10->cjmp_inst.target = i12;                        // if not (a > 10) skipp forward to NOOP
    i10->next = i11;

    i11->type = OUT;                                    // output d
    i11->output_inst.var_index = address_d;
    i11->next = i12;

    i12->type = NOOP;                                   // NOOP after inner IF
    i12->next = i13;

    i13->type = NOOP;                                   // NOOP after outer IF
    i13->next = i14;

    i14->type = ASSIGN;                                 // d = 0
    i14->assign_inst.left_hand_side_index = address_d;
    i14->assign_inst.op = OPERATOR_NONE;
    i14->assign_inst.operand1_index = address_zero;
    i14->next = i15;

    i15->type = CJMP;                                   // if d < 4
    i15->cjmp_inst.condition_op = CONDITION_LESS;
    i15->cjmp_inst.operand1_index = address_d;
    i15->cjmp_inst.operand2_index = address_four;
    i15->cjmp_inst.target = i22;                        // if not (d < 4) skip whole WHILE body
    i15->next = i16;

    i16->type = ASSIGN;                                 // c = a + d
    i16->assign_inst.left_hand_side_index = address_c;
    i16->assign_inst.op = OPERATOR_PLUS;
    i16->assign_inst.operand1_index = address_a;
    i16->assign_inst.operand2_index = address_d;
    i16->next = i17;

    i17->type = CJMP;                                   // if d > 1
    i17->cjmp_inst.condition_op = CONDITION_GREATER;
    i17->cjmp_inst.operand1_index = address_d;
    i17->cjmp_inst.operand2_index = address_one;
    i17->cjmp_inst.target = i19;                        // if not (d > 1) skip body of IF
    i17->next = i18;

    i18->type = OUT;                                    // output d
    i18->output_inst.var_index = address_d;
    i18->next = i19;

    i19->type = NOOP;                                   // NOOP after body of IF
    i19->next = i20;

    i20->type = ASSIGN;                                 // d = d + 1
    i20->assign_inst.left_hand_side_index = address_d;
    i20->assign_inst.op = OPERATOR_PLUS;
    i20->assign_inst.operand1_index = address_d;
    i20->assign_inst.operand2_index = address_one;
    i20->next = i21;

    i21->type = JMP;
    i21->jmp_inst.target = i15;
    i21->next = i22;

    i22->type = NOOP;                                   // NOOP after body of WHILE
    i22->next = NULL;

    // Inputs
    inputs.push_back(1);
    inputs.push_back(2);
    inputs.push_back(3);
    inputs.push_back(4);
    inputs.push_back(5);
    inputs.push_back(6);

    return i1;
}
