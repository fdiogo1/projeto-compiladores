#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define TOKEN_LENGTH 100

typedef enum
{
    IDENTIFIER,
    NUMBER,
    KEYWORD,
    OPERATOR,
    COMPOUND_OPERATOR,
    DELIMITER,
    COMMENTS,
    END_OF_FILE,
    UNKNOWN
} Tokens;

const char *keywords[] = {"and", "array", "begin", "div", "do",
                          "else", "end", "function", "goto", "if", "label", "not", "of", "or",
                          "procedure", "program", "then", "id", "var", "while", "read", "write"};
const char *operators[] = {"+", "-", "*", "/", "=", "<", ">"};
const char *compoundOperators[] = {":=", "<>", "<=", ">="};
const char *delimiters[] = {";", ",", ".", "(", ")", ":"};

typedef struct
{
    Tokens id;
    char word[TOKEN_LENGTH];
} TokenL;

int isKeyword(const char *str)
{
    for (int i = 0; i < sizeof(keywords) / sizeof(char *); i++)
    {
        if (strcmp(str, keywords[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int isOperator(const char *str)
{
    for (int i = 0; i < sizeof(operators) / sizeof(char *); i++)
    {
        if (strcmp(str, operators[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int isCompoundOperator(const char *str)
{
    for (int i = 0; i < sizeof(compoundOperators) / sizeof(char *); i++)
    {
        if (strcmp(str, compoundOperators[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int isDelimiter(const char *str)
{
    for (int i = 0; i < sizeof(delimiters) / sizeof(char *); i++)
    {
        if (strcmp(str, delimiters[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

void getNextToken(FILE *source, TokenL *token)
{
    int ch;
    int i = 0;

    while (isspace(ch = fgetc(source)))
        ;

    if (ch == EOF)
    {
        token->id = END_OF_FILE;
        return;
    }

    if (isalpha(ch))
    {
        token->word[i++] = ch;
        while (isalnum(ch = fgetc(source)))
        {
            if (i < TOKEN_LENGTH - 1)
            {
                token->word[i++] = ch;
            }
            else
            {
                break;
            }
        }
        ungetc(ch, source);
        token->word[i] = '\0';

        if (isKeyword(token->word))
        {
            token->id = KEYWORD;
        }
        else
        {
            token->id = IDENTIFIER;
        }
        return;
    }

    // Números
    if (isdigit(ch))
    {
        token->word[i++] = ch;
        while (isdigit(ch = fgetc(source)))
        {
            if (i < TOKEN_LENGTH - 1)
            {
                token->word[i++] = ch;
            }
            else
            {
                break;
            }
        }
        ungetc(ch, source);
        token->word[i] = '\0';
        token->id = NUMBER;
        return;
    }

    if (ch == '{')
    {
        token->word[i++] = ch;
        while ((ch = fgetc(source)) != '}' && ch != EOF)
        {
            if (i < TOKEN_LENGTH - 1)
            {
                token->word[i++] = ch;
            }
            else
            {
                break;
            }
        }
        token->word[i++] = '}';
        token->word[i] = '\0';
        token->id = COMMENTS;
        return;
    }

    token->word[i++] = ch;
    token->word[i] = '\0';

    if (ch == ':' || ch == '<' || ch == '>')
    {
        int lookNext = fgetc(source);
        if ((ch == ':' && lookNext == '=') ||
            (ch == '<' && (lookNext == '=' || lookNext == '>')) ||
            (ch == '>' && lookNext == '='))
        {
            if (i < TOKEN_LENGTH - 1)
            {
                token->word[i++] = lookNext;
                token->word[i] = '\0';
                token->id = COMPOUND_OPERATOR;
                return;
            }
        }
        else
        {
            ungetc(lookNext, source);
        }
    }

    if (isOperator(token->word))
    {
        token->id = OPERATOR;
    }
    else if (isDelimiter(token->word))
    {
        token->id = DELIMITER;
    }
    else
    {
        token->id = UNKNOWN;
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
        return 1;

    FILE *arqv = fopen(argv[1], "r");
    if (!arqv)
        return 1;

    TokenL token;
    int identifiers = 0;
    int numbers = 0;
    int keywords = 0;
    int operators = 0;
    int compoundOperators = 0;
    int delimiters = 0;
    int comments = 0;
    int unknowns = 0;

    do
    {
        getNextToken(arqv, &token);

        switch (token.id)
        {
            case IDENTIFIER:
                identifiers++;
                break;
            case NUMBER:
                numbers++;
                break;
            case KEYWORD:
                keywords++;
                break;
            case OPERATOR:
                operators++;
                break;
            case COMPOUND_OPERATOR:
                compoundOperators++;
                break;
            case DELIMITER:
                delimiters++;
                break;
            case COMMENTS:
                comments++;
                break;
            case UNKNOWN:
                unknowns++;
                break;
            default:
                break;
        }
    } while (token.id != END_OF_FILE);

    printf("KEYWORD: %d\n", keywords);
    printf("IDENTIFIER: %d\n", identifiers);
    printf("NUMBER: %d\n", numbers);
    printf("OPERATOR: %d\n", operators);
    printf("COMPOUND OPERATOR: %d\n", compoundOperators);
    printf("DELIMITER: %d\n", delimiters);
    printf("COMMENTS: %d\n", comments);
    printf("UNKNOWN: %d\n", unknowns);

    fclose(arqv);
    return 0;
}

// versão do getnextToken para reconhecer melhor (ultima versao)
/*
void getNextToken(FILE *source, TokenL *token)
{
    int ch;
    int i = 0;

    while (isspace(ch = fgetc(source)))
        ;

    if (ch == EOF)
    {
        token->id = END_OF_FILE;
        return;
    }

    if (isalpha(ch))
    {
        token->word[i++] = ch;
        while (isalnum(ch = fgetc(source)))
        {
            if (i < TOKEN_LENGTH - 1)
            {
                token->word[i++] = ch;
            }
            else
            {
                break;
            }
        }
        ungetc(ch, source);
        token->word[i] = '\0';

        if (isKeyword(token->word))
        {
            token->id = KEYWORD;
        }
        else
        {
            token->id = IDENTIFIER;
        }
        return;
    }

    // Números
    if (isdigit(ch))
    {
        token->word[i++] = ch;
        while (isdigit(ch = fgetc(source)))
        {
            if (i < TOKEN_LENGTH - 1)
            {
                token->word[i++] = ch;
            }
            else
            {
                break;
            }
        }
        ungetc(ch, source);
        token->word[i] = '\0';
        token->id = NUMBER;
        return;
    }

    // Comentários delimitados por {}
    if (ch == '{')
    {
        token->word[i++] = ch;
        while ((ch = fgetc(source)) != '}' && ch != EOF)
        {
            if (i < TOKEN_LENGTH - 1)
            {
                token->word[i++] = ch;
            }
            else
            {
                break;
            }
        }
        token->word[i++] = '}';
        token->word[i] = '\0';
        token->id = COMMENTS;
        return;
    }

    // Comentários delimitados por (* *)
    if (ch == '(')
    {
        int nextCh = fgetc(source);
        if (nextCh == '*')
        {
            token->word[i++] = ch;
            token->word[i++] = nextCh;
            while (1)
            {
                ch = fgetc(source);
                if (ch == EOF)
                {
                    break;
                }
                if (i < TOKEN_LENGTH - 1)
                {
                    token->word[i++] = ch;
                }
                if (ch == '*')
                {
                    nextCh = fgetc(source);
                    if (nextCh == ')')
                    {
                        if (i < TOKEN_LENGTH - 1)
                        {
                            token->word[i++] = nextCh;
                        }
                        break;
                    }
                    else
                    {
                        ungetc(nextCh, source);
                    }
                }
            }
            token->word[i] = '\0';
            token->id = COMMENTS;
            return;
        }
        else
        {
            ungetc(nextCh, source);
        }
    }

    token->word[i++] = ch;
    token->word[i] = '\0';

    if (ch == ':' || ch == '<' || ch == '>')
    {
        int lookNext = fgetc(source);
        if ((ch == ':' && lookNext == '=') ||
            (ch == '<' && (lookNext == '=' || lookNext == '>')) ||
            (ch == '>' && lookNext == '='))
        {
            if (i < TOKEN_LENGTH - 1)
            {
                token->word[i++] = lookNext;
                token->word[i] = '\0';
                token->id = COMPOUND_OPERATOR;
                return;
            }
        }
        else
        {
            ungetc(lookNext, source);
        }
    }

    if (isOperator(token->word))
    {
        token->id = OPERATOR;
    }
    else if (isDelimiter(token->word))
    {
        token->id = DELIMITER;
    }
    else
    {
        token->id = UNKNOWN;
    }
}
*/