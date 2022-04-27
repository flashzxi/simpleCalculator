#ifndef _LEX_H
#define _LEX_H

#include<iostream>
#include<string>
#include<cmath>
#include <cstdlib>

enum token {
    leftBra, 
    rightBra, 
    equal, 
    plus,
    minus,
    times,
    divide,
    mod,
    power,
    integer,
    doubleValue,
    dot,
    var,
    eof,
    err
    };

// 词法解析部分

class lex{
private:
    std::string expression;
    int curInt;
    std::string curVar;
    double curDouble;
    token curToken;
    int curIndex;
    int expressionLen;
    char c;

    int parseInt(std::string curStr);

    double parseDouble(std::string intPart, std::string fracPart);

    // read into curStr and parse
    token parseNext();

public:
    lex(lex& other){
        lex(other.getExpression());
    }

    lex(std::string expression);

    const std::string getExpression(){
        return expression;
    }

    // 返回当前位置的char，指针下移一位
    char nextChar();

    token getCurToken();

    std::string getCurVar();

    int getCurInt();

    double getCurDouble();

    token nextToken();

    bool hasNext();

    void showCurrent();

    std::string error_p(const std::string& msg=std::string());
};

#endif