#include "lex.h"

int lex::parseInt(std::string curStr){
    return atoi(curStr.c_str());
}

double lex::parseDouble(std::string intPart, std::string fracPart){
    std::string d = intPart+'.'+fracPart;
    return atof(d.c_str());
}

// read into curStr and parse
token lex::parseNext(){
    // consume blanks
    while(curIndex != expressionLen+1 && c==' '){
        c = nextChar();
    }
    if(curIndex == expressionLen+1){
        curToken = eof;
        return eof;
    }
    std::string curStr;

    switch (c)
    {
    case '+':
        curToken = plus;
        c = nextChar();
        return curToken;
    case '-':
        curToken = minus;
        c = nextChar();
        return curToken;
    case '*':
        curToken = times;
        c = nextChar();
        return curToken;
    case '/':
        curToken = divide;
        c = nextChar();
        return curToken;
    case '^':
        curToken = power;
        c = nextChar();
        return curToken;
    case '%':
        curToken = mod;
        c = nextChar();
        return curToken;
    case '=':
        curToken = equal;
        c = nextChar();
        return curToken;
    case '(':
        curToken = leftBra;
        c = nextChar();
        return curToken;
    case ')':
        curToken = rightBra;
        c = nextChar();
        return curToken;
    }

    // 此处是int 或者double
    if(isdigit(c) || c=='.'){
        while(isdigit(c)){
            curStr += c;
            c = nextChar();
        }
        if(c != '.'){
            curInt = parseInt(curStr);
            curToken = integer;
        }else{
            std::string intPart = curStr;
            curStr.clear();
            c = nextChar();
            while(isdigit(c)){
                curStr += c;
                c = nextChar();
            }
            curDouble = parseDouble(intPart, curStr);
            curToken = doubleValue;
        }
        curStr.clear();
        return curToken;
    }

    // 变量名
    if(isalpha(c)){
        curStr+=c;
        c = nextChar();
        while(isalpha(c) || isdigit(c)){
            curStr += c;
            c = nextChar();
        }
        curToken = var;
        curVar = curStr;
        return curToken;
    }

    // 全不匹配
    // 输出错误
    error_p("unknown character here");
    return err;
}

lex::lex(std::string expression):expression(expression){
    curIndex = 0;
    expressionLen = expression.size();
    c = nextChar();
}

// 返回当前位置的char，指针下移一位
char lex::nextChar(){
    // std::cout<<curIndex<<":"<<curIndex<<":\""<<expression[curIndex]<<"\""<<std::endl;
    return expression[curIndex++];
}

token lex::getCurToken(){
    return curToken;
}

std::string lex::getCurVar(){
    return curVar;
}

int lex::getCurInt(){
    return curInt;
}

double lex::getCurDouble(){
    return curDouble;
}

token lex::nextToken(){
    parseNext();
    return curToken;
}

bool lex::hasNext(){
    while(curIndex != expressionLen && expression[curIndex] == ' '){
        ++ curIndex;
    }
    return curIndex == expressionLen;
    if(curIndex != expressionLen ){
        c = nextChar();
        std::cout<<"hasNext:" << c<<std::endl;
        return true;
    }
    return false;
}

std::string lex::error_p(const std::string& msg){
    std::string errMsg;
    errMsg = errMsg+"  error:" + "\n    " + expression + "\n    ";
    errMsg = errMsg + std::string(curIndex-1,' ')+"^\n  " + msg + "\n";
    std::cout<<errMsg;
    exit(-1);
    return errMsg;
}

void lex::showCurrent(){
    switch (curToken)
    {
    case integer:
        std::cout<<"int: "<<curInt;
        break;
    case doubleValue:
        std::cout<<"double: "<<curDouble;
        break;
    case leftBra:
        std::cout<<"(";
        break;
    case rightBra:
        std::cout<<")";
        break;
    case plus:
        std::cout<<"+";
        break;
    case minus:
        std::cout<<"-";
        break;
    case times:
        std::cout<<"*";
        break;
    case divide:
        std::cout<<"/";
        break;
    case power:
        std::cout<<"^";
        break;
    case mod:
        std::cout<<"%";
        break;
    case equal:
        std::cout<<"=";
        break;
    case var:
        std::cout<<"var: " << curVar;
        break;
    default:
        std::cout<<"error token :" << curToken;
    }
    std::cout<<std::endl;
}