#include <stdio.h>
#include <string.h>
#include "preprocessor.h"

int main(int argc, char *argv[])
{
    char output_name[1024];
    int n, i;
    int status = 0;

    if (argc < 2)
    {
        printf("Usage: %s inputfile.c [inputfile2.c ...]\n",
               argv[0]);
        return 1;
    }

    /* Process all input files */
    for (i = 1; i < argc; i++)
    {
        n = strlen(argv[i]);

        if (n >= 1021)
        {
            printf("Filename is too long: %s\n", argv[i]);
            status = 1;
            continue;
        }

        strcpy(output_name, argv[i]);

        /* Change .c extension to .i */
        if (n >= 2 &&
            strcmp(output_name + n - 2, ".c") == 0)
        {
            strcpy(output_name + n - 2, ".i");
        }
        else
        {
            strcat(output_name, ".i");
        }

        printf("\nProcessing file: %s\n", argv[i]);

        /* Process input file */
if (!process_file(argv[i], output_name, 0))
        {
            printf("Preprocessing failed: %s\n", argv[i]);
            status = 1;
            continue;
        }

        printf("Preprocessing completed successfully\n");
        printf("Output file: %s\n", output_name);
    }

    return status;
}
