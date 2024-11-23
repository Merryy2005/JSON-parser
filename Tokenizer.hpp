#ifndef TOKENIZER
#define TOKENIZER

#include <string>
#include <vector>

//CurlyOpen {
//CurlyClose }
//SquareOpen [
//SquareClose ]
//Colon :
//Comma ,

enum class TokenType {CurlyOpen, CurlyClose, SquareOpen, SquareClose, Colon, Comma, String, Number, Boolean, Null};
#endif //TOKENIZER