#define PCRE2_CODE_UNIT_WIDTH 8

#include <ctok/tokenizer.h>
#include <pcre2.h>

#include <string.h>

int ctok_token_type_equals(const ctok_TokenType *a, const ctok_TokenType *b) {
  return strcmp(a->name, b->name) == 0;
}

ctok_Tokenizer *ctok_create_tokenizer(const char *src,
                                      const ctok_TokenType *types,
                                      size_t type_count) {
  ctok_Tokenizer *tokenizer = malloc(sizeof(ctok_Tokenizer));
  tokenizer->src = src;
  tokenizer->pos = 0;
  tokenizer->types = types;
  tokenizer->type_count = type_count;
  return tokenizer;
}

ctok_Token ctok_tokenizer_next(ctok_Tokenizer *tokenizer) {
  ctok_Token token = {.type_index = -1, .text = NULL};
  if (!tokenizer->src[tokenizer->pos])
    return token;

  size_t best_match_len = 0;
  int best_type = -1;

  for (size_t i = 0; i < tokenizer->type_count; ++i) {
    int errornumber;
    PCRE2_SIZE erroroffset;
    pcre2_code *re = pcre2_compile((PCRE2_SPTR)tokenizer->types[i].regex,
                                   PCRE2_ZERO_TERMINATED, 0, &errornumber,
                                   &erroroffset, NULL);

    if (!re)
      continue;

    pcre2_match_data *match_data =
        pcre2_match_data_create_from_pattern(re, NULL);
    int rc = pcre2_match(re, (PCRE2_SPTR)(tokenizer->src + tokenizer->pos),
                         PCRE2_ZERO_TERMINATED, 0, 0, match_data, NULL);

    if (rc >= 0) {
      PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(match_data);
      size_t match_len = ovector[1] - ovector[0];

      if (match_len > best_match_len) {
        best_match_len = match_len;
        best_type = (int)i;
      }
    }

    pcre2_match_data_free(match_data);
    pcre2_code_free(re);
  }

  if (best_type != -1 && best_match_len > 0) {
    token.type_index = best_type;
    token.text = (char *)malloc(best_match_len + 1);
#if defined(_MSC_VER)
    strncpy_s(token.text, best_match_len + 1, tokenizer->src + tokenizer->pos,
              best_match_len);
#else
    memcpy(token.text, tokenizer->src + tokenizer->pos, best_match_len);
#endif
    token.text[best_match_len] = '\0';
    tokenizer->pos += best_match_len;
  } else {
    tokenizer->pos++; // skip one char if no match
  }

  return token;
}

void ctok_token_free(ctok_Token *token) {
  if (token->text)
    free(token->text);
  token->text = NULL;
}
