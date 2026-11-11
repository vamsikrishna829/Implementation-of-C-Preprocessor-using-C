#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "preprocessor.h"

char *remove_comments(const char *source)
{
    int i = 0, j = 0;
    int state = 0;

    int n = strlen(source);

    char *result = malloc(n + 1);

if (result == NULL)
        return NULL;

    while (i < n)
    {
        char c = source[i];

        if (state == 0)
        {
            if (c == '/' && source[i + 1] == '/')
            {
                state = 1;
                i += 2;
            }
            else if (c == '/' && source[i + 1] == '*')
            {
                state = 2;
                i += 2;
            }
            else
            {
                result[j++] = c;

                if (c == '"')
                    state = 3;
                else if (c == '\'')
                    state = 4;

                i++;
            }
        }

        else if (state == 1)
        {
            if (c == '\n')
            {
                result[j++] = '\n';
                state = 0;
            }         
  i++;
        }

        else if (state == 2)
        {
            if (c == '*' && source[i + 1] == '/')
            {
                state = 0;
                i += 2;
            }
            else
            {
                if (c == '\n')
                    result[j++] = '\n';

                i++;
            }
        }

        else
        {
            result[j++] = c;

            if (c == '\\' && i + 1 < n)
            {
                result[j++] = source[i + 1];
                i += 2;
                continue;
            }

            if ((state == 3 && c == '"') ||
                (state == 4 && c == '\''))
            {
                state = 0;
            }

            i++;
        }
    }

    result[j] = '\0';

    return result;
}
