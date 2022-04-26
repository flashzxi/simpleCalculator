#ifndef _PARSE_H
#define _PARSE_H

#include"lex.h"
#include "AST.h"
#include"stack"
#include<map>
class parse{

private:
    std::map<OP, int> priority;

//    std::stack<OP> ops;

    std::string errorMsg;

//    std::stack<CalculateAST*> leftExpressions;

    lex* myLex;

    token curToken;

    int curInt;

    double curDouble;

    std::stack<std::string> curVar;

    token nextToken();

    BinaryAST* parseBinaryAST();

    IntAST* parseIntAST();

    VarAST* parseVarAST();

    DoubleAST* parseDoubleAST();

    DeclareAST* parseDeclareAST();

    CalculateAST* parseCalculateAST();

    CalculateAST* parseCalculateRightAST(std::stack<OP>& ops, std::stack<CalculateAST*>& leftExpressions);

    CalculateAST* parseCalculateTerminal();
    // UnaryAST parseUnaryAST();

    void genCalculateAST(CalculateAST* left, OP op, std::stack<OP>& ops, std::stack<CalculateAST*>& leftExpressions);

    void initPriorityMap(){
        priority[END] = INT32_MIN;
        priority[ADD] = 1;
        priority[MINUS] = 1;
        priority[MUL] = 2;
        priority[DIV] = 2;
        priority[MOD] = 2;
        priority[POW] = 3;
        priority[LBAR] = INT32_MAX;
        priority[RBAR] = INT32_MAX;
    }

public:

    std::string getErrorMsg(){
        return errorMsg;
    }

    parse(lex* myLex):myLex(myLex){
//        static bool needInit = true;
//        if(needInit){
//            initPriorityMap();
//            needInit = false;
//        }
        initPriorityMap();
        nextToken();
    };

    AST* parseAst();

};

#endif