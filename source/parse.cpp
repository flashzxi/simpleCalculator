#include "parse.h"

#include <memory>

std::unique_ptr<CalculateAST> parse::parseCalculateAST(){
    std::unique_ptr<CalculateAST> left = std::move(parseCalculateTerminal());
    std::stack<std::unique_ptr<CalculateAST>> leftExpressions;
    std::stack<OP> ops;
    leftExpressions.push(std::move(left));
    return parseCalculateRightAST(ops, leftExpressions);

}

std::unique_ptr<CalculateAST> parse::parseCalculateTerminal(){
    if(curToken == minus || curToken == plus){
        char op = (curToken == minus ) ? '-' : '+';
        nextToken();
        std::unique_ptr<CalculateAST> unary = parseCalculateTerminal();
        return std::move(std::make_unique<UnaryAST>(op, std::move(unary)));
    }

    if(curToken == leftBra){
//        ops.push(LBAR);
        nextToken();
        std::unique_ptr<CalculateAST> ret = parseCalculateAST();
        if(curToken != rightBra){
            myLex->error_p("Expect ')' here. ");
            return std::make_unique<CalcuErrorAST>();
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
    default:
        myLex->error_p("Expression expected here");
        return std::make_unique<CalcuErrorAST>();
    }
}

void parse::genCalculateAST(std::unique_ptr<CalculateAST> left, OP op, std::stack<OP>& ops,
                            std::stack<std::unique_ptr<CalculateAST>>& leftExpressions){
    while( ! ops.empty() 
        && ( (op==POW && priority[op] < priority[ops.top()]) ||
            (op!=POW && priority[op] <= priority[ops.top()]) )){
        // TODO: Maybe don't need to pop
        std::unique_ptr<CalculateAST>  expression_r = std::move(leftExpressions.top());
        leftExpressions.pop();
        std::unique_ptr<CalculateAST> expression_l = std::move(leftExpressions.top());
        leftExpressions.pop();
        OP curOp = ops.top();
        ops.pop();
        leftExpressions.push(
            std::make_unique<BinaryAST>(curOp, std::move(expression_l), std::move(expression_r)));
    }
    leftExpressions.push(std::move(left));
    ops.push(op);
}

// 辅助构造CalculateAST
std::unique_ptr<CalculateAST> parse::parseCalculateRightAST(std::stack<OP>& ops,
                                            std::stack<std::unique_ptr<CalculateAST>>& leftExpressions){
    if(curToken == eof || curToken == rightBra){
        std::unique_ptr<CalculateAST> ret = std::move(leftExpressions.top());
        leftExpressions.pop();
        return ret;
    }

    while(curToken != eof && curToken!=rightBra){
        OP curOp;
        switch(curToken){
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
        default:
            myLex->error_p("Expected binary operator here");
        }

        nextToken();
        std::unique_ptr<CalculateAST> left = parseCalculateTerminal();
        if(ops.empty()){
            ops.push(curOp);
            leftExpressions.push(std::move(left));
        }else{
            genCalculateAST(std::move(left), curOp, ops, leftExpressions);
        }
    }

    genCalculateAST(std::make_unique<CalcuErrorAST>(), END, ops, leftExpressions);
    ops.pop();
    leftExpressions.pop();
    std::unique_ptr<CalculateAST> retCalc = std::move(leftExpressions.top());
    leftExpressions.pop();
    return retCalc;
}
// UnaryAST parse::parseUnaryAST(){
// }

std::unique_ptr<BinaryAST> parse::parseBinaryAST(){
    return nullptr;
}

std::unique_ptr<IntAST> parse::parseIntAST(){
    nextToken();
    return std::make_unique<IntAST>(myLex->getCurInt());
}

std::unique_ptr<VarAST> parse::parseVarAST(){
    nextToken();
    std::unique_ptr<VarAST> ret = std::make_unique<VarAST>(myLex->getCurVar());
    if(!ret->exist()){
        myLex->error_p("var doesn't exist");
    }
    return ret;
}

std::unique_ptr<DoubleAST> parse::parseDoubleAST(){
    nextToken();
    return std::make_unique<DoubleAST>(myLex->getCurDouble());
}

std::unique_ptr<DeclareAST> parse::parseDeclareAST(){
    if(curToken != integer && curToken != doubleValue && curToken != var){
        myLex->error_p("Error when parse declare statement");
    }

    std::unique_ptr<CalculateAST> value = parseCalculateAST();
    std::string varName = curVar.top();
    curVar.pop();
    return std::make_unique<DeclareAST>(varName, std::move(value));
}

bool isBinaryOperator(token);

std::unique_ptr<AST> parse::parseAst(){
//    clearStacks();
    switch(curToken){
    case var:
        nextToken();
        if(curToken == equal){
            curVar.push(myLex->getCurVar());
            nextToken();
            if(curToken!=eof)
            return parseDeclareAST();
        } else if(isBinaryOperator(curToken) || curToken == eof) {
            std::stack<std::unique_ptr<CalculateAST>> leftExpressions;
            std::stack<OP> ops;
            leftExpressions.push(std::make_unique<VarAST>(myLex->getCurVar()));
            return parseCalculateRightAST(ops, leftExpressions);
        }else{
            myLex->error_p("= or binary operator expected here");
        }
        break;
    default:
        std::unique_ptr<CalculateAST> ret = parseCalculateAST();
        if(curToken!=eof){
            myLex->error_p("expression should end at here");
        }
        return ret;
    }
    return nullptr;
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
