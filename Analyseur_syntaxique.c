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
#define charSp c == '+' || c == '(' || c == ')' || c == '/' || c == '-' || c == '=' || c == '*' || c == ';' || c == ',' || isspace(c)

#define VN_S 0
#define VN_A 1
#define VN_B 2
#define VN_C 3
#define VN_C1 4
#define VN_D 5
#define VN_E 6
#define VN_E1 7
#define VN_F 8
#define VN_G 9
#define VN_G1 10
#define ERREUR -1

#define Code_MC_SELECT 269
#define Code_MC_FROM 270
#define Code_SEP_PTVIRG 264
#define Code_ACCOLADE_OVR 265
#define Code_ACCOLADE_FER 266
#define Code_MC_COUNT 272
#define Code_IDENT 260
#define Code_MC_DISTINCT 273
#define Code_NOMBRE 261
#define Code_SEP_VIRG 263
#define Code_OP_REL 267
#define Code_OP_LOG 268
#define Code_MC_WHERE 271
#define Code_OP_ARTH 262
#define EPSILONE 111
#define Fin_Suite_UL 333

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

typedef struct pile
{
    UL *sommet;
} Pile;

Pile *InitialiserPile(Pile *p)
{
    p = malloc(sizeof(Pile));
    p->sommet = NULL;
    return p;
}

Pile *Empiler(Pile *p, char v[], int code)
{
    UL *nouveauElement = malloc(sizeof(UL));
    strcpy(nouveauElement->Lexeme, v);
    nouveauElement->Code = code;
    nouveauElement->Suivant = p->sommet;
    p->sommet = nouveauElement;
    return p;
}

Pile *Depiler(Pile *p)
{
    if (p->sommet == NULL)
        return p;
    UL *e;
    e = p->sommet;
    p->sommet = p->sommet->Suivant;
    free(e);
    return p;
}

UL *DepilerUL(Pile *p)
{
    if (p->sommet == NULL)
        return NULL;

    UL *e = p->sommet;
    p->sommet = p->sommet->Suivant;
    e->Suivant = NULL;
    return e;
}

Pile *ViderPile(Pile *p)
{
    while (p->sommet != NULL)
    {
        UL *e = p->sommet;
        p->sommet = p->sommet->Suivant;
        free(e);
    }
    return p;
}

void AfficherPile(Pile *p)
{
    UL *ep = p->sommet;
    while (ep != NULL)
    {
        printf("|%s| Code|%d|\n", ep->Lexeme, ep->Code);
        ep = ep->Suivant;
    }
    printf("\n");
}

char *SommetPile(Pile *p)
{
    return p->sommet->Lexeme;
}

void concatenerPile(Pile *p, Pile *p2)
{

    while (p2->sommet != NULL)
    {
        Empiler(p, p2->sommet->Lexeme, p2->sommet->Code);
        p2->sommet = p2->sommet->Suivant;
    }
    ViderPile(p2);
    p2 = InitialiserPile(p2);
}
int CodeTerminal(int v)
{
    switch (v)
    {

    case Code_MC_SELECT:
        return 0;
        break;

    case Code_MC_FROM:
        return 1;
        break;

    case Code_MC_WHERE:
        return 2;
        break;

    case Code_MC_DISTINCT:
        return 3;
        break;

    case Code_MC_COUNT:
        return 4;
        break;

    case Code_IDENT:
        return 5;
        break;
    case Code_NOMBRE:
        return 6;
        break;
    case Code_OP_ARTH:
        return 7;
        break;
    case Code_SEP_VIRG:
        return 8;
        break;
    case Code_SEP_PTVIRG:
        return 9;
        break;
    case Code_OP_REL:
        return 10;
        break;
    case Code_OP_LOG:
        return 11;
        break;
    case Code_ACCOLADE_OVR:
        return 12;
        break;
    case Code_ACCOLADE_FER:
        return 13;
        break;
    case Fin_Suite_UL:
        return 14;
        break;
    }
}

int CodeNonTerminal(int v)
{
    switch (v)
    {

    case VN_S:
        return 0;
        break;

    case VN_A:
        return 1;
        break;

    case VN_B:
        return 2;
        break;

    case VN_C:
        return 3;
        break;

    case VN_C1:
        return 4;
        break;

    case VN_D:
        return 5;
        break;

    case VN_E:
        return 6;
        break;

    case VN_E1:
        return 7;
        break;

    case VN_F:
        return 8;
        break;

    case VN_G:
        return 9;
        break;

    case VN_G1:
        return 10;
        break;
    }
}

void removeHead(UL **head)
{
    if (*head == NULL)
    {
        printf("List is empty. Cannot remove head.\n");
        return;
    }

    UL *temp = *head;
    *head = (*head)->Suivant;
    free(temp);
}
char *getNomTerminal(int code)
{
    if (code == 269)
    {
        return "SELECT";
    }
    else if (code == 270)
    {
        return "FROM";
    }
    else if (code == 271)
    {
        return "WHERE";
    }
    else if (code == 272)
    {
        return "COUNT";
    }
    else if (code == 273)
    {
        return "DISTINCT";
    }
    else if (code == VN_A)
    {
        return "A";
    }

    else if (code == VN_B)
    {
        return "B";
    }
    else if (code == VN_C)
    {
        return "C";
    }
    else if (code == VN_C1)
    {
        return "C1";
    }
    else if (code == VN_D)
    {
        return "D";
    }
    else if (code == VN_E)
    {
        return "E";
    }
    else if (code == VN_E1)
    {
        return "E1";
    }
    else if (code == VN_F)
    {
        return "F";
    }
    else if (code == VN_G)
    {
        return "G";
    }
    else if (code == VN_G1)
    {
        return "G1";
    }

    else if (code == 261)
    {
        return "NOMBRE";
    }
    else if (code == Code_SEP_PTVIRG)
    {
        return "SEPARATEUR_POINT_VIRGULE";
    }
    else
    {
        return "IDENTIFIANT";
    }
}

int main()
{
   FILE* file = fopen("requete SQL.txt", "r");
    
    UL* head = NULL;
    head = AnalyseurLexical(file,head);
    printList(head);

int regle[19][7] = {
        {Code_MC_SELECT, VN_A, Code_MC_FROM, VN_C, VN_D, Code_SEP_PTVIRG, -1},   //...........0
        {Code_MC_COUNT, Code_ACCOLADE_OVR, VN_B, Code_ACCOLADE_FER, -1, -1, -1}, //...........1
        {VN_B, -1, -1, -1, -1, -1, -1},                                          //...........2
        {Code_MC_DISTINCT, VN_C, -1, -1, -1, -1, -1},                            //...........3
        {VN_C, -1, -1, -1, -1, -1, -1},                                          //...........4
        {Code_IDENT, VN_C1, -1, -1, -1, -1, -1},                                 //...........5
        {Code_SEP_VIRG, VN_B, -1, -1, -1, -1, -1},                               //...........6
        {EPSILONE, -1, -1, -1, -1, -1, -1},                                      //...........7
        {Code_MC_WHERE, VN_E, -1, -1, -1, -1, -1},                               //...........8
        {EPSILONE, -1, -1, -1, -1, -1, -1},                                      //...........9
        {VN_F, VN_E1, -1, -1, -1, -1, -1},                                       //...........10
        {Code_OP_LOG, VN_E, -1, -1, -1, -1, -1},                                 //...........11
        {EPSILONE, -1, -1, -1, -1, -1, -1},                                      //...........12
        {VN_G, Code_OP_REL, VN_G, -1, -1, -1, -1},                               //...........13
        {Code_ACCOLADE_OVR, VN_G, Code_ACCOLADE_FER, VN_G1, -1, -1, -1},         //...........14
        {Code_IDENT, VN_G1, -1, -1, -1, -1, -1},                                 //...........15
        {Code_NOMBRE, VN_G1, -1, -1, -1, -1, -1},                                //...........16
        {Code_OP_ARTH, VN_G, -1, -1, -1, -1, -1},                                //...........17
        {EPSILONE, -1, -1, -1, -1, -1, -1},                                      //...........18
    };

    int TA[11][15] = {
        {0, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR}, // S
        {ERREUR, ERREUR, ERREUR, 2, 1, 2, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR},           // A
        {ERREUR, ERREUR, ERREUR, 3, ERREUR, 4, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR},      // B
        {ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, 5, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR}, // C
        {ERREUR, 7, 7, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, 6, 7, ERREUR, ERREUR, ERREUR, 7, ERREUR},                     // C1
        {ERREUR, ERREUR, 8, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, 9, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR},      // D
        {ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, 10, 10, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, 10, ERREUR, ERREUR},        // E
        {ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, 12, ERREUR, 11, ERREUR, ERREUR, ERREUR},    // E1
        {ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, 13, 13, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, 13, ERREUR, ERREUR},        // F
        {ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, 15, 16, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, 14, ERREUR, ERREUR},        // G
        {ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, ERREUR, 17, ERREUR, 18, 18, 18, ERREUR, 18, ERREUR}                 // G1
    };

    

   

    int NextCode = 0, RegleCode = 0;

    int i = 0;
    int TALigne = 0, TACol = 0;



    Pile* PileAnalyseurSyntaxique = pile;

    PileAnalyseurSyntaxique = Empiler(PileAnalyseurSyntaxique, FIN_SUITE_UL);
    PileAnalyseurSyntaxique = Empiler(PileAnalyseurSyntaxique, VN_S);

    UL* CopieSuiteUL = suiteUL; // Choisissez la taille en conséquence
    printListSyntaxe(PileAnalyseurSyntaxique, CopieSuiteUL);
  
    while (CopieSuiteUL->Code != FIN_SUITE_UL && PileAnalyseurSyntaxique->sommet->code != FIN_SUITE_UL) {
        if (CopieSuiteUL->Code == PileAnalyseurSyntaxique->sommet->code) {
            // Action1
       
            CopieSuiteUL = CopieSuiteUL->Suivant;
            // Dépiler l'élément tête de PileAnalyseurSyntaxique
            PileAnalyseurSyntaxique = Depiler(PileAnalyseurSyntaxique);
            //pop(&PileAnalyseurSyntaxique);

            printListSyntaxe(PileAnalyseurSyntaxique,CopieSuiteUL);

    
        }
        else {
         
            if (PileAnalyseurSyntaxique->sommet) { //!EstTerminal(top(&PileAnalyseurSyntaxique))
                if (Table_DAnalyse[PileAnalyseurSyntaxique->sommet->code][CopieSuiteUL->CodeTerminal] != ERREUR) {
                  
                    int codeNonTerminal = Table_DAnalyse[PileAnalyseurSyntaxique->sommet->code][CopieSuiteUL->CodeTerminal];
                    PileAnalyseurSyntaxique = Depiler(PileAnalyseurSyntaxique);
               
                    for (int i = 6; i>=0 ; i--) {
                    if(RPs[codeNonTerminal][i] != -1){
          if(RPs[codeNonTerminal][i] != 111){
                     // printf("%d ",RPs[codeNonTerminal][i]);
                        PileAnalyseurSyntaxique = Empiler(PileAnalyseurSyntaxique,RPs[codeNonTerminal][i] );
                                  
                        }
                    }
                    }
           
                    printListSyntaxe(PileAnalyseurSyntaxique, CopieSuiteUL);
                }
                else {
                    // Erreur Syntaxique
                    printf("Erreur Syntaxique\n");
                    break;
                }
            }
            else {
                // Erreur Syntaxique
                printf("Erreur Syntaxique\n");
                break;
            }
        }

    
    }

    // Vérifier si l'analyse syntaxique a réussi
    if (CopieSuiteUL->Code == FIN_SUITE_UL && PileAnalyseurSyntaxique->sommet->code == FIN_SUITE_UL) {
       printf("\nAnalyse Syntaxique reussie\n");
    }
    else {
       // Erreur Syntaxique
       printf("Erreur Syntaxique\n");
    }
    return 0;
}
