#ifndef CTOK_TOKENIZER_H
#define CTOK_TOKENIZER_H

/**
 * @file tokenizer.h
 * @brief Tokenizer interface for parsing text using regular expressions.
 *
 * The tokenizer is designed to scan an input string and produce a stream of
 * `ctok_Token` objects.  It uses a user-provided array of
 * `ctok_TokenType` structures that describe each token type by name and a
 * regular expression pattern.
 *
 * All public functions are documented with Doxygen-style comments below.
 */

/**
 * @brief Describes a single token type.
 *
 * Each token type consists of a human-readable name and a regex pattern.
 *
 * @todo Add a unique integer ID or precomputed string hash field to enable
 *       faster comparisons between token types (instead of comparing strings
 *       at runtime). This will improve lookup performance in parsers and
 *       ignore-lists.
 */
typedef struct {

  /**
   * @brief The human-readable name of the token type.
   *
   * This string is used for debugging or error reporting.  The caller
   * owns the memory and must ensure it remains valid for the lifetime of
   * the tokenizer.
   */
  char *name;

  /**
   * @brief The regular expression that matches this token type.
   *
   * The regex must be null-terminated and follow the syntax understood by
   * the tokenizer implementation.  The caller owns the memory and must
   * keep it valid for the tokenizer's lifetime.
   */
  char *regex;
} ctok_TokenType;

/**
 * @brief A single token produced by the tokenizer.
 *
 * The tokenizer returns tokens sequentially; each token contains the
 * index of the matched type in the original `ctok_TokenType` array
 * and a pointer to the matched text slice.  The caller is responsible
 * for freeing the `text` field with `ctok_token_free`.
 */
typedef struct {
  int type_index; /**< Index into the token type array. */
  char *text;     /**< Pointer to the matched substring. */
} ctok_Token;

/**
 * @brief The state of a tokenizer instance.
 *
 * The tokenizer keeps a reference to the array of token types,
 * the source string, and the current parsing position.
 */
typedef struct {
  const ctok_TokenType *types; /**< Array of token types. */
  size_t type_count;           /**< Number of token types. */
  const char *src;             /**< Source string to tokenize. */
  size_t pos;                  /**< Current offset in src. */
} ctok_Tokenizer;

/**
 * @brief Create a new tokenizer instance.
 *
 * @param src         The input string to be tokenized.
 * @param types       Array of token type definitions.
 * @param type_count  Number of elements in @p types.
 * @return A pointer to a newly allocated `ctok_Tokenizer`, or `NULL` on
 * failure.
 *
 * The returned tokenizer owns no resources beyond a pointer to @p src and
 * @p types; it does not copy the strings.  The caller must ensure that
 * @p src and @p types remain valid for the duration of the tokenizer's
 * lifetime.  The tokenizer must be destroyed by freeing the returned
 * pointer when no longer needed.
 */
ctok_Tokenizer *ctok_create_tokenizer(const char *src,
                                      const ctok_TokenType *types,
                                      size_t type_count);

/**
 * @brief Retrieve the next token from the tokenizer.
 *
 * @param tokenizer  Pointer to a valid tokenizer instance.
 * @return A `ctok_Token` struct representing the next token.
 *
 * The returned token must be freed with `ctok_token_free` when the
 * caller has finished using it.  If the end of the input is reached,
 * the token's `text` field will be `NULL` and `type_index` will be
 * undefined.  This behavior can be used to detect EOF.
 */
ctok_Token ctok_tokenizer_next(ctok_Tokenizer *tokenizer);

/**
 * @brief Free the resources associated with a token.
 *
 * @param token Pointer to a token obtained from `ctok_tokenizer_next`.
 *
 * This function frees any memory allocated by the tokenizer for the
 * token's `text` field.  The `token` pointer itself should not be freed
 * by the caller; only the internal resources are released.
 */
void ctok_token_free(ctok_Token *token);

/**
 * @brief Compare two token types for equality.
 *
 * @param a Pointer to the first token type.
 * @param b Pointer to the second token type.
 * @return Non-zero if the token types are equal, zero otherwise.
 *
 * This function compares the names of the token types using `strcmp`.
 */
int ctok_token_type_equals(const ctok_TokenType *a, const ctok_TokenType *b);

#endif
