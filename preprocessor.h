#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#define MAX_MACROS 100
#define MAX_NAME 128
#define MAX_VALUE 1024
#define MAX_ARGS 10

typedef struct
{
    char name[MAX_NAME];
    char value[MAX_VALUE];

    int is_function;
    int arg_count;

    char args[MAX_ARGS][MAX_NAME];

} Macro;

extern Macro macros[MAX_MACROS];
extern int macro_count;

char *remove_comments(const char *source);

void define_macro(const char *line);

char *expand_macros(const char *line);

int process_file(const char *input_name,
                 const char *output_name,
                 int append);

#endif
