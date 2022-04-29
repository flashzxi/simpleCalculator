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

    std::unique_ptr<BinaryAST> parseBinaryAST();

    std::unique_ptr<IntAST> parseIntAST();

    std::unique_ptr<VarAST> parseVarAST();

    std::unique_ptr<DoubleAST> parseDoubleAST();

    std::unique_ptr<DeclareAST> parseDeclareAST();

    std::unique_ptr<CalculateAST> parseCalculateAST();

    std::unique_ptr<CalculateAST> parseCalculateRightAST(std::stack<OP>& ops, std::stack<std::unique_ptr<CalculateAST>>& leftExpressions);

    std::unique_ptr<CalculateAST> parseCalculateTerminal();
    // UnaryAST parseUnaryAST();

    void genCalculateAST(std::unique_ptr<CalculateAST> left, OP op, std::stack<OP>& ops, std::stack<std::unique_ptr<CalculateAST>>& leftExpressions);

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

    std::unique_ptr<AST> parseAst();

};

#endif