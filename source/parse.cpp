#include "parse.h"

CalculateAST* parse::parseCalculateAST(){
    CalculateAST* left = parseCalculateTerminal();
    std::stack<CalculateAST*> leftExpressions;
    std::stack<OP> ops;
    leftExpressions.push(left);
    return parseCalculateRightAST(ops, leftExpressions);

}

CalculateAST* parse::parseCalculateTerminal(){
    if(curToken == minus || curToken == plus){
        char op = (curToken == minus ) ? '-' : '+';
        nextToken();
        CalculateAST *unary = parseCalculateTerminal();
        return new UnaryAST(op, unary);
    }

    if(curToken == leftBra){
//        ops.push(LBAR);
        nextToken();
        CalculateAST* ret = parseCalculateAST();
        if(curToken != rightBra){
            myLex->error_p("Expect ')' here. ");
            return new CalcuErrorAST();
        }
        nextToken();
        return ret;
    }

    switch(curToken){
    case integer:
        return parseIntAST();
    case doubleValue:
        return parseDoubleAST();
    case var:
        return parseVarAST();
    }
    myLex->error_p("Expression expected here");
    return new CalcuErrorAST;
}

void parse::genCalculateAST(CalculateAST* left, OP op, std::stack<OP>& ops,
                            std::stack<CalculateAST*>& leftExpressions){
    while( ! ops.empty() 
        && ( (op==POW && priority[op] < priority[ops.top()]) ||
            (op!=POW && priority[op] <= priority[ops.top()]) )){
        CalculateAST *expression_r = leftExpressions.top();
        leftExpressions.pop();
        CalculateAST *expression_l = leftExpressions.top();
        leftExpressions.pop();
        OP curOp = ops.top();
        ops.pop();
        leftExpressions.push(
            new BinaryAST(curOp, expression_l, expression_r));
    }
    leftExpressions.push(left);
    ops.push(op);
}

// 辅助构造CalculateAST
CalculateAST* parse::parseCalculateRightAST(std::stack<OP>& ops,
                                            std::stack<CalculateAST*>& leftExpressions){
    if(curToken == eof || curToken == rightBra){
        CalculateAST *ret = leftExpressions.top();
        leftExpressions.pop();
        return ret;
    }

    while(curToken != eof && curToken!=rightBra){
        OP curOp;
        switch(curToken){
//        case leftBra:
//            curOp=LBAR;
//            break;
//        case rightBra:
//            curOp = RBAR;
//            break;
        case plus:
            curOp = ADD;
            break;
        case minus:
            curOp = MINUS;
            break;
        case times:
            curOp = MUL;
            break;
        case divide:
            curOp = DIV;
            break;
        case mod:
            curOp = MOD;
            break;
        case power:
            curOp = POW;
            break;
        }

        nextToken();
        CalculateAST* left = parseCalculateTerminal();
        if(ops.empty()){
            ops.push(curOp);
            leftExpressions.push(left);
        }else{
            genCalculateAST(left, curOp, ops, leftExpressions);
        }
    }

    genCalculateAST(new CalcuErrorAST(), END, ops, leftExpressions);
    ops.pop();
    leftExpressions.pop();
    CalculateAST* retCalc = leftExpressions.top();
    leftExpressions.pop();
    return retCalc;
}
// UnaryAST parse::parseUnaryAST(){
// }

BinaryAST* parse::parseBinaryAST(){
    return nullptr;
}

IntAST* parse::parseIntAST(){
    nextToken();
    return new IntAST(myLex->getCurInt());
}

VarAST* parse::parseVarAST(){
    nextToken();
    VarAST* ret = new VarAST(myLex->getCurVar());
    if(!ret->exist()){
        myLex->error_p("var doesn't exist");
    }
    return ret;
}

DoubleAST* parse::parseDoubleAST(){
    nextToken();
    return new DoubleAST(myLex->getCurDouble());
}

DeclareAST* parse::parseDeclareAST(){
    if(curToken != integer && curToken != doubleValue && curToken != var){
        myLex->error_p("Error when parse declare statement");
    }

    CalculateAST* value = parseCalculateAST();
    std::string varName = curVar.top();
    curVar.pop();
    return new DeclareAST(varName, value);
}

bool isBinaryOperator(token);

AST* parse::parseAst(){
//    clearStacks();
    switch(curToken){
    case var:
        nextToken();
        if(curToken == equal){
            curVar.push(myLex->getCurVar());
            nextToken();
            if(curToken!=eof)
            return parseDeclareAST();
        } else if(isBinaryOperator(curToken)) {
            std::stack<CalculateAST*> leftExpressions;
            std::stack<OP> ops;
            leftExpressions.push(new VarAST(myLex->getCurVar()));
            return parseCalculateRightAST(ops, leftExpressions);
        }else{
            myLex->error_p("= or binary operator expected here");
        }
        break;
    default:
        CalculateAST *ret = parseCalculateAST();
        if(curToken!=eof){
            myLex->error_p("expression should end at here");
        }
        return ret;
    }
}

token parse::nextToken(){
    curToken = myLex->nextToken();
    return curToken;
}

bool isBinaryOperator(token t){
    switch(t){
        case plus:
        case minus:
        case times:
        case divide:
        case mod:
        case power:
            return true;
        default:
            return false;
    }
}
