#include "Lexer.hpp"

int main()
{
    Lexer lexer = Lexer();
    
    std::cout << "Please Key in some words: \n";

    while (true) {
        Token token = lexer.scan();
        
        if (feof(stdin)) break;
        
        std::cout << "Tag: " << token.tag;
        
        if (token.tag == Tag::NUM) {
            std::cout << " (NUM)";
        } else if (token.tag == Tag::ID) {
            std::cout << " (ID)";
        } else if (token.tag == Tag::RELOP) {
            std::cout << " (RELOP)";
        } else if (token.tag < 256) {
            std::cout << " (WORD ('" << (char)token.tag << "'))";
        }
        std::cout << std::endl;
    }
    

    return 0;
}