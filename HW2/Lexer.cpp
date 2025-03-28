#include <iostream>
#include <sstream>
#include <fstream>
#include <unordered_map>

class Token {

public:
    int tag;
    Token(int t) { tag = t; }

};

class Tag {

public:
    static int NUM;
    static int ID;
    static int TRUE;
    static int FALSE;
    static int RELOP;

};

int Tag::NUM = 256;
int Tag::ID = 257;
int Tag::TRUE = 258;
int Tag::FALSE = 259;
int Tag::RELOP = 260;

class Num : public Token {

public:
    int value;
    Num(int v) : Token(Tag::NUM), value(v) {}

};

class Word : public Token {

public:
    std::string lexeme;
    Word(int t, std::string s) : Token(t), lexeme(s) {}

};

class Relop : public Token {
    
public:
    std::string label;
    Relop(std::string l) : Token(Tag::RELOP), label(l) {}

};

class Lexer {

public:
    int line;

    Lexer() : line(1), peek(' ') {
        reserve( new Word(Tag::TRUE, "true") );
        reserve( new Word(Tag::FALSE, "false") );
    }

    void reserve(Word* t) {
        words[t->lexeme] = t;
    }

    Token scan() {

        for(;; peek = getchar())
        {
            if( peek == ' ' || peek == '\t' ) continue;
            else if( peek == '\n' ) line = line + 1;
            else break;
        }

        if( is_digit(peek) )
        {
            int v = 0;
            do
            {
                v = 10 * v + peek - '0';
                peek = getchar();
            } while( isdigit(peek) );
            return Num(v);
        }

        if( is_letter(peek) )
        {
            std::stringstream ss;
            do 
            {
                ss << peek;
                peek = getchar();
            } while( is_letter(peek) || is_digit(peek) );
            std::string s = ss.str();
            Word* w;
            if(words.find(s) == words.end())
            {
                w = new Word(Tag::ID, s);
                words[s] = w;
            }
            else
            {
                w = (Word*)words[s];
            }
            return *w;
        }

        if( is_relop(peek) )
        {
            char op = peek;
            peek = getchar();

            switch(op)
            {
                case '>':
                {
                    if (peek == '=')
                    {
                        peek = getchar();
                        return Relop(">=");
                    }
                    return Relop(">");
                }

                case '<':
                {
                    if (peek == '=') 
                    {
                        peek = getchar();
                        return Relop("<=");
                    }
                    return Relop("<");
                }

                case '=':
                {
                    if (peek == '=') 
                    {
                        peek = getchar();
                        return Relop("==");
                    }
                    return Token(op); // 單獨的 = 不是關係運算子，就建立一個新的 Token
                }

                case '!':
                {
                    if (peek == '=') 
                    {
                        peek = getchar();
                        return Relop("==");
                    }
                    return Token(op); // 單獨的 ! 不是關係運算子，就建立一個新的 Token
                }
            }
        }

        Token t = Token(peek);
        peek = ' ';
        return t;
    }
    

private:
    char peek;
    std::unordered_map<std::string, Token*> words;

    bool is_letter(char c) {
        return std::isalpha(c) || c == '_';
    }

    bool is_digit(char c) {
        return std::isdigit(c);
    }

    bool is_relop(char c) {
        return c == '<' || c == '>' || c == '=' || c == '!';
    }

};

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