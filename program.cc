#include <iostream>

#include <map>

#include <cstdlib>

#include <stdio.h>

#include <stdlib.h>

#include <stdarg.h>

#include <ctype.h>

#include <string.h>


#include "compiler.h"

#include "lexer.h"


using namespace std;


LexicalAnalyzer lex;


struct VarNode

{

    std::string name;

    int value;

};


struct VarNode* table[1000];

int var_count = 0;


bool match(TokenType t);


struct InstructionNode* create_noop();

struct InstructionNode* parse_inst();



struct InstructionNode* create_noop()

{

    struct InstructionNode* result = new InstructionNode;

    result->type = NOOP;

    result->next = NULL;

    return result;

}


enum ConditionalOperatorType get_conditional(TokenType t)

{

    enum ConditionalOperatorType result;

    switch (t)

    {

    case NOTEQUAL:

        result = CONDITION_NOTEQUAL;

        break;

    case GREATER:

        result = CONDITION_GREATER;

        break;

    case LESS:

        result = CONDITION_LESS;

        break;

    default:

        break;

    }

    return result;

}





void parse_vars()

{

    Token t;

    loop:

    if(match(ID))

    {

        Token t = lex.GetToken();

        table[var_count] = new VarNode{};

        table[var_count]->name = t.lexeme;

        table[var_count]->value = 0;

        var_count++;

        //cout<<"Got Var: " << table[var_count - 1]->name <<endl;

    }

    if(match(COMMA))

    {

        Token t = lex.GetToken();

        goto loop;

    }

    t = lex.GetToken();

}


struct InstructionNode* get_last(InstructionNode* i)

{

    while(i->next != NULL)

    {

        //cout<<"LOOP"<<endl;

        i = i->next;

    }

    return i;

}


bool match(TokenType t)

{

    Token tok = lex.peek(1);

    if(tok.token_type == t)

    {

        return true;

    }

    return false;

}



void pass_inputs()

{

    //cout<<"Passing Inputs"<<endl;

    loop:

    if(match(NUM))

    {

        Token t = lex.GetToken();

        inputs.push_back(stoi(t.lexeme));

        goto loop;

    }

}


int create_constant(int num)

{

    int result = var_count;

    table[var_count] = new VarNode{};

    table[var_count]->name = "";

    table[var_count]->value = num;

    var_count++;

    return result;

}


int get_index(Token t)

{

    int result = -1;

    switch (t.token_type)

    {

        case ID:

            for(int i = 0; i < var_count; i++)

            {

                

                if(table[i]->name == t.lexeme)

                {

                    result = i;

                    break;

                }

            }

            break;

        case NUM:

            result = create_constant(stoi(t.lexeme));

            break;

    }

    

    

    return result;

}



enum ArithmeticOperatorType get_op(Token t)

{

    enum ArithmeticOperatorType result;

    switch(t.token_type)

    {

        case PLUS:

            result = OPERATOR_PLUS;

            break;

        case MINUS:

            result = OPERATOR_MINUS;

            break;

        case MULT:

            result = OPERATOR_MULT;

            break;

        case DIV:

            result = OPERATOR_DIV;

            break;

        default:

            result = OPERATOR_NONE;

            break;

    }

    return result;

}



struct InstructionNode* parse_assign()

{

    if(!match(ID))

    {

        cout<<"NULL"<<endl;

        return nullptr;

    }

    //cout<<"Parsing assign"<<endl;

    Token t = lex.GetToken();

    struct InstructionNode* il = new InstructionNode;

    il = create_noop();

    

    il->type = ASSIGN;

    il->assign_inst.left_hand_side_index = get_index(t);

    if(match(EQUAL))

    {

        

        t = lex.GetToken();

        t = lex.GetToken();

        il->assign_inst.operand1_index = get_index(t);

        if(!match(SEMICOLON))

        {

            

            t = lex.GetToken();

            il->assign_inst.op = get_op(t);

            t = lex.GetToken();

            il->assign_inst.operand2_index = get_index(t);

            

        }

        else

        {

            il->assign_inst.op = OPERATOR_NONE;

            

        }

        t = lex.GetToken();

        

    }

    return il;

}


struct InstructionNode* parse_input()

{

    if(!match(INPUT))

    {

        cout<<"NULL"<<endl;

        return nullptr;

    }

    

    //cout<<"Parsing input"<<endl;

    Token t = lex.GetToken();

    t = lex.GetToken();

    struct InstructionNode* il = new InstructionNode;

    il = create_noop();

    il->type = IN;

    il->input_inst.var_index = get_index(t);

    t = lex.GetToken();

    return il;

}


struct InstructionNode* parse_output()

{

    if(!match(OUTPUT))

    {

        cout<<"NULL"<<endl;

        return nullptr;

    }

    //cout<<"Parsing output"<<endl;

    Token t = lex.GetToken();

    t = lex.GetToken();

    struct InstructionNode* il = new InstructionNode;

    il = create_noop();

    il->type = OUT;

    il->output_inst.var_index = get_index(t);

    t = lex.GetToken();

    return il;

}


struct InstructionNode* parse_if()

{

    if(!match(IF))

    {

        cout<<"NULL"<<endl;

        return nullptr;

    }

    

    //cout<<"Parsing if"<<endl;

    Token t = lex.GetToken();

    t = lex.GetToken();

    struct InstructionNode* il = new InstructionNode;

    il = create_noop();

    il->type = CJMP;

    il->cjmp_inst.operand1_index = get_index(t);

    t = lex.GetToken();

    il->cjmp_inst.condition_op = get_conditional(t.token_type);

    t = lex.GetToken();

    il->cjmp_inst.operand2_index = get_index(t);

    if(match(LBRACE))

    {

        t = lex.GetToken();

        //cout<<"Parsing if body!"<<endl;

        il->next = parse_inst();

        t = lex.GetToken();

    }

    

    il->cjmp_inst.target = get_last(il);

    return il;

}


struct InstructionNode* parse_while()

{

    if(!match(WHILE))

    {

        cout<<"NULL"<<endl;

        return nullptr;

    }

    Token t = lex.GetToken();

    //cout<<"Parsing while"<<endl;

    struct InstructionNode* il = new InstructionNode;

    il = create_noop();

    il->type = CJMP;

    t = lex.GetToken();

    il->cjmp_inst.operand1_index = get_index(t);

    t = lex.GetToken();

    il->cjmp_inst.condition_op = get_conditional(t.token_type);

    t = lex.GetToken();

    il->cjmp_inst.operand2_index = get_index(t);

    if(match(LBRACE))

    {

        t = lex.GetToken();

        //cout<<"Parsing while body!"<<endl;

        il->next = parse_inst();

        t = lex.GetToken();

    }

    struct InstructionNode* gt = new InstructionNode;

    gt = create_noop();

    gt->type = JMP;

    gt->jmp_inst.target = il;

    get_last(il)->next = gt;

    get_last(il)->next = create_noop();

    il->cjmp_inst.target = get_last(il);

    return il;

}



struct InstructionNode* parse_for()

{

    if(!match(FOR))

    {

        cout<<"NULL"<<endl;

        return nullptr;

    }

    Token t = lex.GetToken();

    t = lex.GetToken();

    t = lex.GetToken();

    //cout<<"Parsing for"<<endl;

    struct InstructionNode* il = new InstructionNode;

    il = create_noop();

    il->type = ASSIGN;

    struct InstructionNode* cj = new InstructionNode;

    cj = create_noop();

    cj->type = CJMP;

    struct InstructionNode* loop = new InstructionNode;

    loop = create_noop();

    loop->type = JMP;

    struct InstructionNode* inc = new InstructionNode;

    inc = create_noop();

    inc->type = ASSIGN;

    il->assign_inst.left_hand_side_index = get_index(t);

    t = lex.GetToken();

    t = lex.GetToken();

    il->assign_inst.operand1_index = get_index(t);

    t = lex.GetToken();

    if(get_op(t) == OPERATOR_NONE)

    {

        il->assign_inst.op = OPERATOR_NONE;

    }

    else

    {

        il->assign_inst.op = get_op(t);

        t = lex.GetToken();

        il->assign_inst.operand2_index = get_index(t);

        t = lex.GetToken();

    }

    t = lex.GetToken();

    cj->cjmp_inst.operand1_index = get_index(t);

    t = lex.GetToken();

    cj->cjmp_inst.condition_op = get_conditional(t.token_type);

    t = lex.GetToken();

    cj->cjmp_inst.operand2_index = get_index(t);

    t = lex.GetToken();

    t = lex.GetToken();

    inc->assign_inst.left_hand_side_index = get_index(t);

    t = lex.GetToken();

    t = lex.GetToken();

    inc->assign_inst.operand1_index = get_index(t);

    t = lex.GetToken();

    inc->assign_inst.op = get_op(t);

    t = lex.GetToken();

    inc->assign_inst.operand2_index = get_index(t);

    t = lex.GetToken();

    t = lex.GetToken();

    get_last(il)->next = cj;

    loop->jmp_inst.target = cj;


    if(match(LBRACE))

    {

        t = lex.GetToken();

        //cout<<"Parsing while body!"<<endl;

        get_last(il)->next = parse_inst();

        t = lex.GetToken();

    }

    get_last(il)->next = inc;

    get_last(il)->next = loop;

    get_last(il)->next = create_noop();

    cj->cjmp_inst.target = get_last(il);

    return il;

}



struct InstructionNode* parse_case(int var, struct InstructionNode* end_inst)

{

    if(!match(CASE))

    {

        cout<<"NULL"<<endl;

        return nullptr;

    }

    Token t = lex.GetToken();

    t = lex.GetToken();

    struct InstructionNode* il = new InstructionNode;

    il = create_noop();

    il->type = CJMP;

    struct InstructionNode* j = new InstructionNode;

    j = create_noop();

    struct InstructionNode* start_case = new InstructionNode;

    start_case = create_noop();

    struct InstructionNode* end_case = new InstructionNode;

    end_case = create_noop();

    struct InstructionNode* next_case = new InstructionNode;

    next_case = create_noop();

    end_case->type = JMP;

    j->type = JMP;

    j->jmp_inst.target = end_inst;

    il->cjmp_inst.operand1_index = var;

    il->cjmp_inst.condition_op = CONDITION_NOTEQUAL;

    il->cjmp_inst.operand2_index = get_index(t);

    il->cjmp_inst.target = start_case;

    end_case->jmp_inst.target = next_case;

    t = lex.GetToken();

    if(match(LBRACE))

    {

        //cout<<"Parsing case"<<endl;

        t = lex.GetToken();

        get_last(il)->next = end_case;

        get_last(il)->next = start_case;

        get_last(il)->next = parse_inst();

        t = lex.GetToken();

        get_last(il)->next = j;

        get_last(il)->next = next_case;

    }

    return il;

}



struct InstructionNode* parse_switch()

{

    if(!match(SWITCH))

    {

        cout<<"NULL"<<endl;

        return nullptr;

    }

    //cout<<"Parsing switch!"<<endl;

    Token t = lex.GetToken();

    t = lex.GetToken();

    int var_index = get_index(t);

    struct InstructionNode* il = new InstructionNode;

    il = create_noop();

    struct InstructionNode* end_inst = new InstructionNode;

    end_inst = create_noop();

    if(match(LBRACE))

    {

        t = lex.GetToken();

    }

    while(match(CASE))

    {

        get_last(il)->next = parse_case(var_index, end_inst);

    

    }

    

    if(match(DEFAULT))

    {

        t = lex.GetToken();

        t = lex.GetToken();

        if(match(LBRACE))

        {

            t = lex.GetToken();

            get_last(il)->next = parse_inst();

            t = lex.GetToken();

        }

    }

    t = lex.GetToken();

    

    get_last(il)->next = end_inst;

    

    return il;

    

}





struct InstructionNode* parse_inst()

{

    

    struct InstructionNode* in = new InstructionNode;

    //in->next = new InstructionNode;

    in = create_noop();

    Token t = lex.peek(1);

    //t.Print();

    

    switch (t.token_type)

    {

        case ID:

            in = parse_assign();

            break;

        case INPUT:

            in = parse_input();

            break;

        case OUTPUT:

            in = parse_output();

            break;

        case IF:

            in = parse_if();

            break;

        case WHILE:

            in = parse_while();

            break;

        case FOR:

            in = parse_for();

            break;

        case SWITCH:

            in = parse_switch();

            break;

        default:

            break;

    }

    //lex.peek(1).Print();

    if(match(RBRACE))

    {

        

        if(in->next != nullptr)

        {

            get_last(in)->next = create_noop();

        }

        else

        {

            if(lex.peek(2).token_type != NUM)

            {

                //cout<<"next is null"<<endl;

                in->next = new InstructionNode;

                in->next = create_noop();

            }

            

        }

        

    }


    if(!match(RBRACE))

    {

        get_last(in)->next = parse_inst();

    }

    

    return in;

}



struct InstructionNode* parse_body()

{

    if(!match(LBRACE))

    {

        return nullptr;

    }

    

    struct InstructionNode* in = new InstructionNode;

    in = create_noop();

    Token t = lex.GetToken();

    in = parse_inst();

    return in;


}




void pass_vars()

{

    for(int i = 0; i < var_count; i++)

    {

        mem[i] = table[i]->value;

    }

}


struct InstructionNode * parse_generate_intermediate_representation()

{

    

    struct InstructionNode* instruction = create_noop();

    

    

    parse_vars();


    instruction = parse_body();

    Token t = lex.GetToken();

    


    if(match(NUM))

    {

        pass_inputs();

    }


    pass_vars();

    struct InstructionNode* temp = new InstructionNode;

    temp = instruction;

    // while(temp->next != NULL)

    // {

    //     cout<<temp<<endl;

    //     if(temp->type == CJMP)

    //     {

    //         cout<<"Target: "<<temp->cjmp_inst.target<<endl;

    //         cout<<"Condition: "<<temp->cjmp_inst.condition_op<<endl;

    //     }

    //     if(temp->type == JMP)

    //     {

    //         cout<<"Target: "<<temp->jmp_inst.target<<endl;

    //     }

    //     temp = temp->next;

    // }

    

    

    return instruction;

}
