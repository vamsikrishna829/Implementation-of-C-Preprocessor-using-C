#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "preprocessor.h"

Macro macros[MAX_MACROS];
int macro_count = 0;

static void trim(char *s)
{
    size_t n;

    while (isspace((unsigned char)*s))
        memmove(s, s + 1, strlen(s));

    n = strlen(s);

    while (n && isspace((unsigned char)s[n - 1]))
        s[--n] = '\0';
}

void define_macro(const char *line)
{
    char name[MAX_NAME] = "";
    char value[MAX_VALUE] = "";

    const char *p = line + 7;
    int i = 0, k;

    while (isspace((unsigned char)*p))
        p++;

    while ((isalnum((unsigned char)*p) || *p == '_')
           && i < MAX_NAME - 1)
    {
        name[i++] = *p++;
    }

    name[i] = '\0';

    if (!name[0] || macro_count >= MAX_MACROS)
        return;

    Macro m = {0};
 strcpy(m.name, name);

    /* Function-like macro */

    if (*p == '(')
    {
        m.is_function = 1;
        p++;

        while (*p && *p != ')' &&
               m.arg_count < MAX_ARGS)
        {
            char arg[MAX_NAME] = "";
            int a = 0;

            while (isspace((unsigned char)*p))
                p++;

            while ((isalnum((unsigned char)*p) ||
                    *p == '_') && a < MAX_NAME - 1)
            {
                arg[a++] = *p++;
            }

            arg[a] = '\0';

            if (arg[0])
                strcpy(m.args[m.arg_count++], arg);

            while (isspace((unsigned char)*p))
                p++;

            if (*p == ',')
                p++;
            else
                break;
        }

        if (*p == ')')
            p++;
    }

    while (isspace((unsigned char)*p))
        p++;
 snprintf(value, sizeof(value), "%s", p);

    trim(value);

    strcpy(m.value, value);

    /* Update existing macro */

    for (k = 0; k < macro_count; k++)
    {
        if (strcmp(macros[k].name, name) == 0)
        {
            macros[k] = m;
            return;
        }
    }

    macros[macro_count++] = m;
}

static int is_id_char(char c)
{
    return isalnum((unsigned char)c) || c == '_';
}

/* Replace macro text */

static void replace_text(char *text,
                         const char *from,
                         const char *to)
{
    char result[8192] = "";

    size_t used = 0;
    size_t flen = strlen(from);

    const char *p = text;

    if (!flen)
        return;

    while (*p && used < sizeof(result) - 1)
    {
        if (strncmp(p, from, flen) == 0 &&
            (p == text || !is_id_char(p[-1])) &&
                               !is_id_char(p[flen]))
        {
            size_t tlen = strlen(to);

            if (used + tlen >= sizeof(result) - 1)
                break;

            memcpy(result + used, to, tlen);

            used += tlen;
            p += flen;
        }
        else
        {
            result[used++] = *p++;
        }
    }

    result[used] = '\0';

    strcpy(text, result);
}

/* Expand macros */

char *expand_macros(const char *line)
{
    char *out = malloc(8192);

    int i, pass;

    if (!out)
        return NULL;

    snprintf(out, 8192, "%s", line);

    for (pass = 0; pass < 5; pass++)
    {
        for (i = 0; i < macro_count; i++)
        {
            Macro *m = &macros[i];

            if (!m->is_function)
            {
                replace_text(out, m->name, m->value);
                                                           }
            else
            {
                char *p = out;

                while ((p = strstr(p, m->name)) != NULL)
                {
                    char *open = p + strlen(m->name);
                    char *close;

                    char call[2048];
                    char args[MAX_ARGS][512] = {{0}};

                    int ac = 0;
                    int depth = 0;
                    int a = 0;

                    size_t len;

                    if ((p > out && is_id_char(p[-1])) ||
                        is_id_char(*open))
                    {
                        p = open;
                        continue;
                    }

                    if (*open != '(')
                    {
                        p = open;
                        continue;
                    }

                    close = open + 1;

                    while (*close)
                    {
                        if (*close == '(')
                            depth++;

                        else if (*close == ')')
                        {
                            if (depth == 0)
                                break;

                            depth--;
                        }
                           else if (*close == ',' && depth == 0)
                        {
                            if (ac < MAX_ARGS)
                                args[ac][a] = '\0';

                            ac++;
                            a = 0;
                            close++;
                            continue;
                        }

                        if (ac < MAX_ARGS && a < 510)
                            args[ac][a++] = *close;

                        close++;
                    }

                    if (*close != ')' ||
                        (ac + 1 != m->arg_count &&
                         m->arg_count != 0))
                    {
                        p = open + 1;
                        continue;
                    }

                    if (m->arg_count == 0)
                        ac = 0;
                    else
                    {
                        args[ac][a] = '\0';
                        ac++;
                    }

                    snprintf(call, sizeof(call),
                             "%s", m->value);

                    for (int x = 0; x < m->arg_count; x++)
                    {
                        replace_text(call,
                                     m->args[x],
                                     args[x]);
                    }

                    len = strlen(close + 1);
                                                                                                                                                                                           if (strlen(out) -
                        (size_t)(close + 1 - out) +
                        strlen(call) >= 8190)
                        break;

                    memmove(p + strlen(call),
                            close + 1, len + 1);

                    memcpy(p, call, strlen(call));

                    p += strlen(call);
                }
            }
        }
    }

    return out;
}

