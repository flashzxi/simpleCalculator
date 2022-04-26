#include<iostream>
#include<string>
#include"parse.h"
#include"lex.h"
#include"AST.h"

int main(){
    std::string buffer;
    std::cout<<"inpuet expression or declare a variable:"<<std::endl;
    std::cout<<">>> ";
    std::getline(std::cin,buffer);
    while(buffer!="exit"){
        lex* myLex = new lex(buffer);
        parse myParser(myLex);
        AST *ast = myParser.parseAst();
        ast->exec();
        if(ast->getType() == CALCU){
            std::cout<< result << std::endl;
        }

        delete myLex;
        std::cout<<">>> ";
        buffer.clear();
        std::getline(std::cin,buffer);
    }
}