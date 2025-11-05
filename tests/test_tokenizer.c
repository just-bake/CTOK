#include "ctok/tokenizer.h"
#include <stdio.h>

int main(void) {
  ctok_TokenType types[] = {
      {"NUMBER", "^[0-9]+"},
      {"IDENTIFIER", "^[a-zA-Z_][a-zA-Z0-9_]*"},
      {"SPACE", "^[ \t\n]+"},
  };

  const char *input = "foo 123 bar42";

  ctok_Tokenizer *tokenizer = ctok_create_tokenizer(input, types, 3);

  ctok_Token token;
  while ((token = ctok_tokenizer_next(tokenizer)).type_index != -1) {
    printf("%s: '%s'\n", types[token.type_index].name, token.text);
    ctok_token_free(&token);
  }

  return 0;
}
