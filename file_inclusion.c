#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>

#include "preprocessor.h"

static int include_depth = 0;

/* Check whether a file exists */
static int try_include(const char *candidate,
                       char *path,
                       size_t size)
{
    FILE *fp;

    if (snprintf(path, size, "%s", candidate) >= (int)size)
        return 0;


fp = fopen(path, "r");

    if (fp)
    {
        fclose(fp);
        return 1;
    }

    return 0;
}

/* Get GCC multiarch directory */
static void get_multiarch(char *multiarch, size_t size)
{
    FILE *fp;
    char buffer[256];

    multiarch[0] = '\0';

    fp = popen("gcc -print-multiarch 2>/dev/null", "r");

    if (fp)
    {
        if (fgets(buffer, sizeof(buffer), fp))
        {
            buffer[strcspn(buffer, "\r\n")] = '\0';
            snprintf(multiarch, size, "%s", buffer);
        }

        pclose(fp);
    }
}

/* Search for header files */
static int find_include(const char *name,
                    
 const char *current_file,
                        char *path,
                        size_t size)
{
    char candidate[2048];
    char multiarch[256];
    char gcc_include[1024];

    FILE *fp;

    /* 1. Search directly in current working directory */
    if (try_include(name, path, size))
        return 1;

    /* 2. Search relative to the including file */
    const char *slash = strrchr(current_file, '/');

    if (slash)
    {
        size_t dir_length = (size_t)(slash - current_file + 1);

        if (dir_length + strlen(name) < sizeof(candidate))
        {
            memcpy(candidate, current_file, dir_length);
            candidate[dir_length] = '\0';
            strcat(candidate, name);

            if (try_include(candidate, path, size))
                return 1;
        }
    }

    /* 3. Search standard Ubuntu directories */
    snprintf(candidate, sizeof(candidate),
             "/usr/include/%s", name);

if (try_include(candidate, path, size))
        return 1;

    snprintf(candidate, sizeof(candidate),
             "/usr/local/include/%s", name);

    if (try_include(candidate, path, size))
        return 1;

    /* 4. Search architecture-specific directory */
    get_multiarch(multiarch, sizeof(multiarch));

    if (multiarch[0] != '\0')
    {
        snprintf(candidate, sizeof(candidate),
                 "/usr/include/%s/%s", multiarch, name);

        if (try_include(candidate, path, size))
            return 1;
    }

    /* 5. Search GCC internal include directory */
    fp = popen("gcc -print-file-name=include 2>/dev/null", "r");

    if (fp)
    {
        if (fgets(gcc_include, sizeof(gcc_include), fp))
        {
            gcc_include[strcspn(gcc_include, "\r\n")] = '\0';

            if (gcc_include[0] != '\0')
            {
                snprintf(candidate, sizeof(candidate),
                         "%s/%s", gcc_include, name);
         
 if (try_include(candidate, path, size))
                {
                    pclose(fp);
                    return 1;
                }
            }
        }

        pclose(fp);
    }

    return 0;
}

/* Main file processing function */
int process_file(const char *input_name,
                 const char *output_name,
                 int append)
{
    FILE *in, *out;

    char *raw, *clean;
    char *line, *save;

    long file_size;

    if (include_depth >= 20)
    {
        fprintf(stderr,
                "Error: maximum include depth exceeded\n");
        return 0;
    }

    /* Open input file */
    in = fopen(input_name, "r");

    if (!in)
    {
        fprintf(stderr,
                "Error: cannot open %s\n",
                input_name);
        return 0;
    }
 /* Find file size */
    if (fseek(in, 0, SEEK_END) != 0)
    {
        fclose(in);
        return 0;
    }

    file_size = ftell(in);

    if (file_size < 0)
    {
        fclose(in);
        return 0;
    }

    rewind(in);

    raw = malloc((size_t)file_size + 1);

    if (!raw)
    {
        fclose(in);
        return 0;
    }

    if (fread(raw, 1, (size_t)file_size, in) !=
        (size_t)file_size)
    {
        free(raw);
        fclose(in);
        return 0;
    }

    raw[file_size] = '\0';

    fclose(in);

    /* Remove comments */
    clean = remove_comments(raw);

    free(raw);

    if (!clean)
        return 0;
   
 /* Open output file */
    out = fopen(output_name, append ? "a" : "w");

    if (!out)
    {
        free(clean);
        return 0;
    }

    include_depth++;

    line = strtok_r(clean, "\n", &save);

    while (line)
    {
        char *p = line;

        while (isspace((unsigned char)*p))
            p++;

        /* Process macro definition */
        if (strncmp(p, "#define", 7) == 0 &&
            isspace((unsigned char)p[7]))
        {
            define_macro(p);
        }

        /* Process include statement */
        else if (strncmp(p, "#include", 8) == 0 &&
                 (p[8] == '<' || p[8] == '"'))
        {
            char name[512] = "";
            char path[1024];

            char *start = strchr(p, '<');
            char *end;

            if (!start)
                start = strchr(p, '"');
         
   if (start)
            {
                char closing =
                    (*start == '<') ? '>' : '"';

                end = strchr(start + 1, closing);

                if (end &&
                    (size_t)(end - start - 1) < sizeof(name))
                {
                    size_t name_length =
                        (size_t)(end - start - 1);

                    memcpy(name, start + 1, name_length);
                    name[name_length] = '\0';

                    if (find_include(name,
                                     input_name,
                                     path,
                                     sizeof(path)))
                    {
                        fclose(out);

                        if (!process_file(path,
                                          output_name,
                                          1))
                        {
                            free(clean);
                            include_depth--;
                            return 0;
                        }

                        out = fopen(output_name, "a");

                        if (!out)
                        {
                            free(clean);
                            include_depth--;
              
  return 0;
                        }

                        out = fopen(output_name, "a");

                        if (!out)
                        {
                            free(clean);
                            include_depth--;
                            return 0;
                        }
                    }
                    else
                    {
                        fprintf(stderr,
                                "Warning: include file not found: %s\n",
                                name);
                    }
                }
            }
        }

        /* Expand macros and write output */
        else
        {
            char *expanded = expand_macros(line);

            if (expanded)
            {
                fprintf(out, "%s\n", expanded);
                free(expanded);
            }
        }

        line = strtok_r(NULL, "\n", &save);
    }

    fclose(out);

    free(clean);

    include_depth--;

    return 1;
}

