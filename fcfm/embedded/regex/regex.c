#include <stdio.h>
#include <regex.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#define EXP "[ |\n|\t]*[+|-]?[[:digit:]]+"

bool validarCadena(char*);
int str_a_entero(char*);
int main(int argc, char** argv)
{
    if(argc <= 1)
    {
        printf("Error\n");
        return 2;
    }

    int i = 0;

    int total = 0;

    for(i = 1; i < argc; i++)
    {

        if(validarCadena(argv[i]))
        {
             total += str_a_entero(argv[i]);
        }
    }

    printf("%d\n", total);
    
    return 0;
}


bool validarCadena(char* pCadena)
{
    regex_t expresion;

    if( regcomp(&expresion, EXP, REG_EXTENDED|REG_NOSUB) != 0 )
    {
        printf("Error\n");
        exit(1);
    }

    if(regexec(&expresion, pCadena, 0, NULL, 0) == 0)
    {
        regfree(&expresion);
        return true;
    }
    else
    {
        regfree(&expresion);
        return false;
    }
}

int str_a_entero(char* pCadena)
{
    int i = 0;
    int entero = 0;
    int signo = 1;
    int longitud = strlen(pCadena);

    for(i = 0; i < longitud; i++)
    {
        if(pCadena[i] == ' ' || pCadena[i] == '\n' || pCadena[i] == '\t' ||
            pCadena[i] == '+')
        {
            continue;
        }

        if(pCadena[i] == '-')
        {
            signo = -1;
            continue;
        }

        entero += (pCadena[i] - 48) * pow(10, longitud - i - 1);
    }

    return (entero * signo);
}