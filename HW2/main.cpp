#include "Lexer.hpp"

// 只是一個簡單的應用，在執行的時候可以在終端機互動輸入文字，並將輸入的句子依各文字轉換成對應的 Token

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