#include <iostream>
#include <sstream>
#include <unordered_map>
#include <stack>

enum Tag {
    NUM = 256,
    ID = 257,
    TRUE = 258,
    FALSE = 259,
    RELOP = 260,
    ASSIGN = 261,
    GT = 262,
    GE = 263,
    EQ = 264,
    LT = 265,
    LE = 266,
    NE = 267,
    OTHER = 268 // 因為我們的 Scanner 還定義不完全，暫時用 OTHER 代表不在定義範圍內的一些符號
};

class Token {

public:
    Tag tag;
    Token(Tag t) { tag = t; }

};

class Num : public Token {

public:
    int value;
    Num(int v) : Token(Tag::NUM), value(v) {}

};

class Word : public Token {

public:
    std::string lexeme;
    Word(Tag t, std::string s) : Token(t), lexeme(s) {}

};

class Relop : public Token {
    
public:
    Relop(Tag t) : Token(t) {}

};

class Lexer {

public:
    int line;

    Lexer() : line(1), peek(' ') {
        reserve( new Word(Tag::TRUE, "true") );
        reserve( new Word(Tag::FALSE, "false") );
        buffer = std::stack<char>();
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
                        return Relop(Tag::GE);
                    }
                    return Relop(Tag::GT);
                }

                case '<':
                {
                    if (peek == '=') 
                    {
                        peek = getchar();
                        return Relop(Tag::LE);
                    }
                    return Relop(Tag::LT);
                }

                case '=':
                {
                    if (peek == '=') 
                    {
                        peek = getchar();
                        return Relop(Tag::EQ);
                    }
                    return Token(Tag::ASSIGN); // 單獨的 = 不是關係運算子，就建立一個新的 Token
                }

                case '!':
                {
                    if (peek == '=') 
                    {
                        peek = getchar();
                        return Relop(Tag::NE);
                    }
                    return Token(Tag::OTHER); // 單獨的 ! 不是關係運算子，就建立一個新的 Token
                }
            }
        }

        Token t = Token(Tag::OTHER);
        peek = ' ';
        return t;
    }
    

private:
    char peek;
    std::unordered_map<std::string, Token*> words;
    std::stack<char> buffer;

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