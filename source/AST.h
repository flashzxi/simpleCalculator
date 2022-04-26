#ifndef _AST_H
#define _AST_H

#include<map>
#include<cmath>
#include<string>

enum OP {
    ADD,
    MINUS,
    DIV,
    MUL,
    MOD,
    POW,
    LBAR,
    RBAR,
    END //最低优先级，看到代表运算结束
};

enum ASTType{
    DECLARE,
    CALCU,
    ERROR
};

extern std::map<std::string , double> valueMap;

extern double result;

class AST{
public:
    virtual void exec() = 0;
    virtual ASTType getType() = 0;
    virtual bool hasError() = 0;
};

class CalculateAST:public AST {
public:
    virtual double getValue() = 0;
    void exec() override{ 
        result = getValue();
    }

    ASTType getType() override{
        return CALCU;
    }

    bool hasError() override{
        return false;
    }
};

class DeclareAST: public AST{
    std::string varname;
    CalculateAST* value;

public:
    DeclareAST(std::string varname, CalculateAST* value):varname(varname){ this->value = value;}
    void exec() override{
        valueMap[varname] = value->getValue();
    }

    ASTType getType() override{
        return DECLARE;
    }

    bool hasError() override{
        return false;
    }
};

class BinaryAST: public CalculateAST{
private:
    OP op;
    CalculateAST* left;
    CalculateAST* right;

public:
    BinaryAST(OP op, CalculateAST* left, CalculateAST* right):
        op(op), left(left), right(right){ }

    double getValue() override{
        switch (op)
        {
        case ADD:
            return left->getValue() + right->getValue();
        case MINUS:
            return left->getValue() - right->getValue();
        case MUL:
            return left->getValue() * right->getValue();
        case DIV:
            return left->getValue() / right->getValue();
        case POW:
            return pow(left->getValue(), right->getValue());
        case MOD:
            return (int)left->getValue() % (int)right->getValue();
        default:
            // this should never happen
            exit(-1);
        }
    }

    bool hasError() {
        return left->hasError() || right->hasError();
    }
};

class IntAST: public CalculateAST{
private:
    int value;

public:
    IntAST(int value):value(value){}

    double getValue() override{
        return value;
    }
};

class DoubleAST: public CalculateAST{
private:
    double value;

public:
    double getValue() override{
        return value;
    };

    DoubleAST(double value):value(value){}
};

class VarAST: public CalculateAST{
private:
    
    std::string value;

public:
    VarAST(std::string value): value(value){};

    double getValue() override{
        return valueMap[value];
    }
};

class UnaryAST: public CalculateAST{
private:
    char op;
    CalculateAST* value;

public:
    UnaryAST(char op, CalculateAST* value):op(op), value(value){ }

    double getValue() override{
        if(op=='-'){
            return - value->getValue();
        }
        return value->getValue();
    }

    bool hasError(){
        return value->hasError();
    }
};

class ErrorAST: public AST{
public:
    ErrorAST() {}
    void exec() override{}
    ASTType getType() override{
        return ERROR;
    }

    bool hasError() override{
        return true;
    }
};

class CalcuErrorAST: public CalculateAST{
public:
    CalcuErrorAST(){}
    double getValue() override{
        return 0;
    }

    void exec() override {}
    ASTType getType() override{
        return ERROR;
    }

    bool hasError() override{
        return true;
    }
};

#endif