#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define Q0 0
#define Q1 1
#define Q2 2
#define Q3 3
#define Q4 4
#define Q5 5
#define charSp c == '+' || c == '(' || c == ')' || c == '/' || c == '-'  || c == '*' || c == ';' || c == ',' || isspace(c)

typedef struct Unite_Lexicale
{
    char Lexeme[20];
    int Code;
    int Ligne;
    struct Unite_Lexicale *Suivant;
} UL;

UL *createNode(char lexeme[], int code, int ligne)
{
    UL *newNode = (UL *)malloc(sizeof(UL));
    if (newNode == NULL)
    {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    strcpy(newNode->Lexeme, lexeme);
    newNode->Code = code;
    newNode->Ligne = ligne;
    newNode->Suivant = NULL;

    return newNode;
}

void insertNode(UL **head, char lexeme[], int code, int ligne)
{
    UL *newNode = createNode(lexeme, code, ligne);

    if (*head == NULL)
    {
        *head = newNode;
    }
    else
    {
        UL *current = *head;
        while (current->Suivant != NULL)
        {
            current = current->Suivant;
        }
        current->Suivant = newNode;
    }
}


int ULCodeSep(char *mot)
{
    if (strcmp(mot, ",") == 0)
    {
        return 263;
    }
    else if (strcmp(mot, ";") == 0)
    {
        return 264;
    }
    else if (strcmp(mot, "(") == 0)
    {
        return 265;
    }
    else if (strcmp(mot, ")") == 0)
    {
        return 266;
    }
}


int UlCodeTerminal(char*mot)
{
char select[] = "select", from[] = "from", where[] = "where", count[] = "count", distinct[] = "distinct";

if (strcmp(mot, select) == 0)
    {
        return 269;
    }
    else if (strcmp(mot, from) == 0)
    {
        return 270;
    }
    else if ((strcmp(mot, where)) == 0)
    {
        return 271;
    }
    else if ((strcmp(mot, count)) == 0)
    {
        return 272;
    }
    else if ((strcmp(mot, distinct)) == 0)
    {
        return 273;
    }
    else if ((strcmp(mot, "or") == 0) || (strcmp(mot, "and") == 0))
    {
        return 268;
    }
    else 
    {
        return 0;
    }
}



int ULcodeRel(char *mot)
{
    int i=0;
    int isDig=0;
    int isAlpha=0;

    while (mot[i]!='\0')
    {
     if (isdigit(mot[i]))
     {
        isDig=1;
     }
     else if (isalpha(mot[i]))
     {
        isAlpha=1;
     }
     i++;
    }
    if (isAlpha==1)
    {
        return 260;
    }
    else if (isDig==1)
    {
    return 261;
    }
    else
    {
        return 0;
    }
    
    
}

int UlCode(char *mot, int *etat_final)
{
    

    if (strcmp(mot, ">") == 0)
    {
        return 267;
    }
    else if (*etat_final == Q2 || *etat_final == Q4)
    {
        return 261;
    }
    else if (strcmp(mot, "+") == 0 || strcmp(mot, "-") == 0 || strcmp(mot, "*") == 0 || strcmp(mot, "/") == 0)
    {
        return 262;
    }
    
    else if (strcmp(mot, "{") == 0)
    {
        return 265;
    }
    else if (strcmp(mot, "}") == 0)
    {
        return 266;
    }
    else if (strcmp(mot, ">") == 0 || strcmp(mot, "<") == 0 || strcmp(mot, ">=") == 0|| strcmp(mot, "<=") == 0)
    {
        return 267;
    }
    else if ((strcmp(mot, "or") == 0) || (strcmp(mot, "and") == 0))
    {
        return 268;
    }
    else if (*etat_final == Q1)
    {
        return 260;
    }
    else
    {
        return 0;
    }
}

void printList(UL *head)
{
    UL *current = head;
    while (current != NULL)
    {
        printf("\tLexeme: [%s]      Code: [%d]        Ligne: [%d]\n", current->Lexeme, current->Code, current->Ligne);
        current = current->Suivant;
    }
}

void freeList(UL *head)
{
    UL *current = head;
    UL *next;
    while (current != NULL)
    {
        next = current->Suivant;
        free(current);
        current = next;
    }
}

UL *AnalyseurLexical(FILE *file, UL *head)
{

    int etat_automate = 0, etat_final = 0, error = 0;
    int c, i = 0;
    char lexeme[20];
    int errordigit=0,errorletter=0;
    int code = 0;
    int ligne = 1;
    int index = 0;
    int lastCharWasSeparator = 0;
    int lastCharWasSpace = 0;
    int lastCharWasOp=0;
    
    while (!(feof(file)))
    {

        c = fgetc(file);

        /*if (error == 1 && c == '\n')
        {
            printf("I broke it \n");
            break;
        }

        else
        {
            //printf("%c", c);
        }*/

        if (isspace(c) && lastCharWasSpace == 0) // To Separate
        {

            if (lastCharWasSeparator == 0)
            {
                lexeme[index] = '\0';
                if (lastCharWasOp==1)
                {
                printf("if (lastCharWasOp==1)\t %s\n",lexeme);    
                insertNode(&head, lexeme,267, ligne);    
                }
                else
                {
                printf("%s Etat automate %d\n",lexeme,etat_automate);
                insertNode(&head, lexeme, (UlCodeTerminal(lexeme)) ? UlCodeTerminal(lexeme) : ULcodeRel(lexeme), ligne);
                }
                lastCharWasSpace=0;
                lastCharWasOp=0;
                lastCharWasSeparator=0;
                index = 0;
            }
            lastCharWasSpace = 1;
        }
        else if (c == ',' || c == ';'  || c == '(' || c == ')' )
        {   
            if (lastCharWasSpace == 0)
            {
                lexeme[index] = '\0'; 
                printf("------2%s-------\n",lexeme);
                insertNode(&head, lexeme,ULcodeRel(lexeme)? ULcodeRel(lexeme) : 267, ligne);
                index = 0;
            }
            lexeme[0] = c;
            lexeme[1] = '\0';
            printf("-----%s----\n",lexeme);
            insertNode(&head, lexeme, ULCodeSep(lexeme), ligne);
            index = 0;
            lastCharWasSeparator = 1;
        }
        else if (c=='>' || c=='<' || c=='=')
        {
            lexeme[index] = c;
            index++;
            lastCharWasSeparator = 0;

            lastCharWasSpace = 0;
            lastCharWasOp = 1;
        }

        else if (isalnum(c) && lastCharWasOp !=1)
        {
            lexeme[index] = c;
            index++;
            lastCharWasSeparator = 0;

            lastCharWasSpace = 0;
        }
        if (c == '\n')
        {
            ligne++;
        }
        switch (etat_automate)
        {
            
        case Q0:
        
            //printf("Q0\n");
            if (isalpha(c))
            {
                etat_automate = Q1;
                etat_final = Q1;
            }
            else if (isdigit(c))
            {
                etat_automate = Q2;
                etat_final = Q2;
            }
            else if (c == '.')
            {
                etat_automate = Q3;
            }
            else if (c == '<' || c == '>')
            {
                etat_automate = Q5;
                etat_final = Q5;
            }
            else if (charSp)
            {
                etat_automate = Q0;
                etat_final = Q0;
            }
            else
            {
                errorletter = c;


                error = 1;
            }






            break;
        case Q1:
            //printf("Q1\n");
            if (isalnum(c))
            {
                etat_automate = Q1;
                etat_final = Q1;
            }
            else if ((c == '>') || (c == '<'))
            {
                etat_automate = Q5;
            }
            else if (charSp)
            {
                etat_automate = Q0;
            }
            else
            {
                errorletter = c;

                error = 1;
            }

            break;
        case Q2:
            //printf("Q2\n");
            if (isdigit(c))
            {
                etat_automate = Q2;
                etat_final = Q2;
            }
            else if (c == '.')
            {
                etat_automate = Q3;
            }
            else if ((c == '>') || (c == '<'))
            {
                etat_automate = Q5;
            }
            else if (charSp)
            {
                etat_automate = Q0;
            }
            else
            {
                errorletter = c;

                error = 1;
            }

            break;
        case Q3:
            //printf("Q3\n");
            etat_automate = Q4;
            etat_final = Q4;

            break;
        case Q4:
            //printf("Q4\n");
            if (isdigit(c))
            {
                etat_automate = Q4;
                etat_final = Q4;
            }
            else if (c == '<' || c == '>')
            {
                etat_automate = Q5;
            }
            else if (charSp)
            {
                etat_automate = Q0;
            }
            else
            {
                errorletter = c;

                error = 1;
            }

            break;
        case Q5:
            //printf("Q5\n");
            if (isalpha(c))
            {
                etat_automate = Q1;
            }
            else if (isdigit(c))
            {
                etat_automate = Q2;
            }
            else if (c == '<' || c == '>' || c=='=')
            {
                etat_automate=Q0;
                etat_final = Q5;
            }
            else if (charSp)
            {
                etat_automate = Q0;
            }
            else
            {
                errorletter = c;
                etat_automate=Q5;

                error = 1;
            }
            
            break;
        }
        
    }

    lexeme[0] = '#';
    lexeme[1] = '\0';
    insertNode(&head, lexeme, 333, ligne);

    return head;
}



int main()
{
   FILE* file = fopen("requete SQL.txt", "r");
    
    UL* head = NULL;
    head = AnalyseurLexical(file,head);
    printList(head);
    
    
    return 0;
}
