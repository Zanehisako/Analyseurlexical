#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
    /* ------------------------------------------------ */
typedef struct cellule {
    //char valeur[10];
    int code;
    struct cellule* next;
} Element;

typedef struct pile {
    Element* sommet;
} Pile;

Pile* InitialiserPile(Pile* p) {
    p = malloc(sizeof(Pile));
    p->sommet = NULL;
    return p;
}

void AfficherPile(Pile* p) {
    Element* ep = p->sommet;
    while (ep != NULL) {
        printf("%d ", ep->code);
        ep = ep->next;
    }
    printf("\n");
}
void Inserer_a_la_fin(Element* p, int c) {
    // si la liste chainee est vdie
    if (p == NULL) {
        return;
    }
    Element* dernier_mot = p;
    while (dernier_mot->next != NULL) {
        dernier_mot = dernier_mot->next;
    }
    dernier_mot->next = malloc(sizeof(Element));
    // copier le mot
    dernier_mot->next->code = c;
    dernier_mot->next->next = NULL;
    return;
}
Pile* concatenerPile(Pile *p, Pile* p2) {
    if(p->sommet == NULL) {
        return p2;
    }
    if(p2->sommet == NULL) {
        return p;
    }
    Element* curr = p->sommet;

    while(curr && curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = p2->sommet;

    return p;
}
Pile* Empiler(Pile* p, int c) {
    Element* nouveauElement = malloc(sizeof(Element));
    nouveauElement->code = c;
    nouveauElement->next = p->sommet;
    p->sommet = nouveauElement;
    return p;
}
/*Pile* EmpilerSansChangerSommet(Pile* p, int c) {
    Pile* pClone = p;
    Element* nouveauElement = malloc(sizeof(Element));
    //strcpy(nouveauElement->valeur, v);
    nouveauElement->code = c;
    nouveauElement->next = NULL;
    while (pClone->sommet != NULL && pClone->sommet->next != NULL) {
        pClone->sommet = pClone->sommet->next;
    }
    if(pClone->sommet != NULL)
        pClone->sommet->next = nouveauElement;
    AfficherPile(p);
    return p;
}*/

Pile* Depiler(Pile* p) {
    if (p->sommet == NULL)
        return p;
    Element* e;
    e = p->sommet;
    p->sommet = p->sommet->next;
    free(e);
    return p;
}

Element* SommetPile(Pile* p) {
    if (p->sommet == NULL)
        return NULL; /** code pile vide */
    else
        return p->sommet;
}

Pile* ViderPile(Pile* p) {
    while (p->sommet != NULL) {
        Element* e = p->sommet;
        p->sommet = p->sommet->next;
        free(e);
    }
    return p;
}

    /* ------------------------------------------------ */
#define Q0 0
#define Q1 1
#define Q2 2
#define Q3 3
#define Q4 4
#define Q5 5
#define charSp   c=='+' || c=='(' || c==')' || c=='/' || c=='-' || c=='=' || c=='*' || c==';' || c==',' || isspace(c)

#define IDENTIFIANT_UL 260
#define NOMBRE_UL 261
#define OPERATEUR_ARITHMETIQUE 262
#define SEPARATEUR_VIRGULE 263
#define SEPARATEUR_POINT_VIRGULE 264
#define ACCOLADE_OUVRANTE 265
#define ACCOLADE_FERMANTE 266
#define OPERATEUR_RELATIONNEL 267
#define OPERATEUR_LOGIQUE 268
#define SELECT_KW 269
#define FROM_KW 270
#define WHERE_KW 271
#define COUNT_KW 272
#define DISTINCT_KW 273
#define EPSILON 111
#define FIN_SUITE_UL 999
#define ERREUR -99

#define SELECT 0
#define FROM 1
#define WHERE 2
#define DISTINCT 3
#define COUNT 4
#define IDENT 5
#define NOMBRE 6
#define OP_ARTH 7
#define SEP_VRG 8
#define SEP_PNTVRG 9
#define OP_REL 10
#define OP_LOG 11
#define PAR_FER 12
#define PAR_OUV 13
#define _D 14

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

int RPs[19][7] = {
    {SELECT_KW,VN_A,FROM_KW,VN_C,VN_D,SEPARATEUR_POINT_VIRGULE,-1}, //...........0
    {COUNT_KW,ACCOLADE_OUVRANTE,VN_B,ACCOLADE_FERMANTE,-1,-1,-1}, //...........1
    {VN_B,-1,-1,-1,-1,-1,-1}, //...........2
    {DISTINCT_KW,VN_C,-1,-1,-1,-1,-1}, //...........3
    {VN_C,-1,-1,-1,-1,-1,-1}, //...........4
    {IDENTIFIANT_UL,VN_C1,-1,-1,-1,-1,-1}, //...........5
    {SEPARATEUR_VIRGULE,VN_B,-1,-1,-1,-1,-1}, //...........6
    {EPSILON,-1,-1,-1,-1,-1,-1}, //...........7
    {WHERE_KW,VN_E,-1,-1,-1,-1,-1}, //...........8
    {EPSILON,-1,-1,-1,-1,-1,-1}, //...........9
    {VN_F,VN_E1,-1,-1,-1,-1,-1}, //...........10
    {OPERATEUR_LOGIQUE,VN_E,-1,-1,-1,-1,-1}, //...........11
    {EPSILON,-1,-1,-1,-1,-1,-1}, //...........12
    {VN_G,OPERATEUR_RELATIONNEL,VN_G,-1,-1,-1,-1}, //...........13
    {ACCOLADE_OUVRANTE,VN_G,ACCOLADE_FERMANTE,VN_G1,-1,-1,-1}, //...........14
    {IDENTIFIANT_UL,VN_G1,-1,-1,-1,-1,-1}, //...........15
    {NOMBRE_UL,VN_G1,-1,-1,-1,-1,-1}, //...........16
    {OPERATEUR_ARITHMETIQUE,VN_G,-1,-1,-1,-1,-1}, //...........17
    {EPSILON,-1,-1,-1,-1,-1,-1}, //...........18
};

int Table_DAnalyse[11][15] = {
    {0,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR}, //S
    {ERREUR,ERREUR,ERREUR,2,1,2,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR}, //A
    {ERREUR,ERREUR,ERREUR,3,ERREUR,4,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR}, // B
    {ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,5,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR}, //C
    {ERREUR,7,7,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,6,7,ERREUR,ERREUR,ERREUR,7,ERREUR}, // C1
    {ERREUR,ERREUR,8,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,9,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR}, // D
    {ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,10,10,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,10,ERREUR,ERREUR}, // E
    {ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,12,ERREUR,11,ERREUR,ERREUR,ERREUR},// E1
    {ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,13,13,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,13,ERREUR,ERREUR}, // F
    {ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,15,16,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,14,ERREUR,ERREUR}, // G
    {ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,ERREUR,17,ERREUR,18,18,18,ERREUR,18,ERREUR} // G1
};

typedef struct Unite_Lexicale
{
    char Lexeme[20];
    int Code;
    int CodeTerminal;
    int Ligne;
    struct Unite_Lexicale* Suivant;
} UL;


UL* createNode(char lexeme[], int code, int codeTerminal, int ligne) {
    UL* newNode = (UL*)malloc(sizeof(UL));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    strcpy_s(newNode->Lexeme, sizeof(newNode->Lexeme), lexeme);
    newNode->Code = code;
    newNode->CodeTerminal = codeTerminal;
    newNode->Ligne = ligne;
    newNode->Suivant = NULL;

    return newNode;
}

void insertNode(UL** head, char lexeme[], int code, int codeTerminal, int ligne) {
    UL* newNode = createNode(lexeme, code, codeTerminal, ligne);

    if (*head == NULL) {
        *head = newNode;
    }
    else {
        UL* current = *head;
        while (current->Suivant != NULL) {
            current = current->Suivant;
        }
        current->Suivant = newNode;
    }
}

char* convertToLowercase(const char* str) {
    char* result = (char*)malloc(strlen(str) + 1);

    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; str[i]; i++) {
        result[i] = tolower(str[i]);
    }

    result[strlen(str)] = '\0';

    return result;
}
int UlCodeTerminal(char* mot, int* etat_final)
{
    char select[] = "select", from[] = "from", where[] = "where", count[] = "count", distinct[] = "distinct";

    if (strcmp(mot, select) == 0)
    {
        return SELECT;
    }
    else if (strcmp(mot, from) == 0)
    {
        return FROM;
    }
    else if ((strcmp(mot, where)) == 0)
    {
        return WHERE;
    }
    else if ((strcmp(mot, count)) == 0)
    {
        return COUNT;
    }
    else if ((strcmp(mot, distinct)) == 0)
    {
        return DISTINCT;
    }
    else if ((*etat_final == Q2 || *etat_final == Q4))
    {
        return NOMBRE;
    }
    else if (strcmp(mot, "+") == 0 || strcmp(mot, "-") == 0 || strcmp(mot, "*") == 0 || strcmp(mot, "/") == 0)
    {
        return OP_ARTH;
    }
    else if (strcmp(mot, ",") == 0)
    {
        return SEP_VRG ;
    }
    else if (strcmp(mot, ";") == 0)
    {
        SEP_PNTVRG;
    }
    else if (strcmp(mot, "{") == 0)
    {
        return PAR_OUV;
    }
    else if (strcmp(mot, "}") == 0)
    {
        return PAR_FER;
    }
    else if (strcmp(mot, ">") == 0 || strcmp(mot, "<") == 0 || strcmp(mot, "=") == 0)
    {
        return OP_REL;
    }
    else if ((strcmp(mot, "or") == 0) || (strcmp(mot, "and") == 0))
    {
        return OP_LOG;
    }
    else if (*etat_final == Q1)
    {
        return IDENT;
    }
    else
    {
        return _D;
    }
}

int UlCode(char* mot, int* etat_final)
{
    char select[] = "select", from[] = "from", where[] = "where", count[] = "count", distinct[] = "distinct";

    if (strcmp(mot, select) == 0)
    {
        return SELECT_KW;
    }
    else if (strcmp(mot, from) == 0)
    {
        return FROM_KW;
    }
    else if ((strcmp(mot, where)) == 0)
    {
        return WHERE_KW;
    }
    else if ((strcmp(mot, count)) == 0)
    {
        return COUNT_KW;
    }
    else if ((strcmp(mot, distinct)) == 0)
    {
        return DISTINCT_KW;
    }
    else if ((*etat_final == Q2 || *etat_final == Q4))
    {
        return NOMBRE_UL;
    }
    else if (strcmp(mot, "+") == 0 || strcmp(mot, "-") == 0 || strcmp(mot, "*") == 0 || strcmp(mot, "/") == 0)
    {
        return OPERATEUR_ARITHMETIQUE;
    }
    else if (strcmp(mot, ",") == 0)
    {
        return SEPARATEUR_VIRGULE;
    }
    else if (strcmp(mot, ";") == 0)
    {
        SEPARATEUR_POINT_VIRGULE;
    }
    else if (strcmp(mot, "{") == 0)
    {
        return ACCOLADE_OUVRANTE;
    }
    else if (strcmp(mot, "}") == 0)
    {
        return ACCOLADE_FERMANTE;
    }
    else if (strcmp(mot, ">") == 0 || strcmp(mot, "<") == 0 || strcmp(mot, "=") == 0)
    {
        return OPERATEUR_RELATIONNEL;
    }
    else if ((strcmp(mot, "or") == 0) || (strcmp(mot, "and") == 0))
    {
        return OPERATEUR_LOGIQUE;
    }
    else if (*etat_final == Q1)
    {
        return IDENTIFIANT_UL;
    }
    else
    {
        return 0;
    }




}

void printList(UL* head) {
    UL* current = head;
    while (current != NULL) {
        printf("\tLexeme: [%s]      Code: [%d]        Ligne: [%d]\n", current->Lexeme, current->Code, current->Ligne);
        current = current->Suivant;
    }
}
void AfficherListe(UL* head) {
    UL* currentUL = head;
    while (currentUL != NULL) {
        printf("%d ", currentUL->Code);
        currentUL = currentUL->Suivant;
    }
    return;
}
void printListSyntaxe(Pile* p, UL* head) {
    UL* currentSuiteUL = head;
    Pile* currentPile = p;
    printf("\n\n\t--------------------------------------------------\n");
    printf("\nPileNVT: ");
    AfficherPile(currentPile);
    printf("\nSuiteUL: ");
    AfficherListe(currentSuiteUL);
    printf("\n");
}
void freeList(UL* head) {
    UL* current = head;
    UL* next;
    while (current != NULL) {
        next = current->Suivant;
        free(current);
        current = next;
    }
}
void analyseurSyntaxique(Pile* pile, UL* suiteUL) {
    Pile* PileAnalyseurSyntaxique = pile;

    PileAnalyseurSyntaxique = Empiler(PileAnalyseurSyntaxique, FIN_SUITE_UL);
    PileAnalyseurSyntaxique = Empiler(PileAnalyseurSyntaxique, VN_S);

    UL* CopieSuiteUL = suiteUL; // Choisissez la taille en conséquence
    printListSyntaxe(PileAnalyseurSyntaxique, CopieSuiteUL);
    // Assumez que la fonction Tete renvoie le premier élément de la suite
    // et que SuiteUL est terminé par le caractère '#'
    // La fonction Tete et la structure TA (table d'analyse) doivent être définies

    // Boucle principale de l'analyse syntaxique
    while (CopieSuiteUL->Code != FIN_SUITE_UL && SommetPile(PileAnalyseurSyntaxique)->code != FIN_SUITE_UL) {
        if (CopieSuiteUL->Code == SommetPile(PileAnalyseurSyntaxique)->code) {
            // Action1
        printf("\ntestif: %d Pile: %d\n",CopieSuiteUL->Code, SommetPile(PileAnalyseurSyntaxique)->code);
            CopieSuiteUL = CopieSuiteUL->Suivant;
            // Dépiler l'élément tête de PileAnalyseurSyntaxique
            PileAnalyseurSyntaxique = Depiler(PileAnalyseurSyntaxique);
            //pop(&PileAnalyseurSyntaxique);

            printListSyntaxe(PileAnalyseurSyntaxique,CopieSuiteUL);

            // printf("\n\nACTION1:\n\n");
            // AfficherPile(PileAnalyseurSyntaxique);
            // AfficherListe(CopieSuiteUL);


            //return;
        }
        else {
             printf("\ntestelse: %d Pile: %d\n",CopieSuiteUL->Code, SommetPile(PileAnalyseurSyntaxique)->code);
            if (SommetPile(PileAnalyseurSyntaxique)) { //!EstTerminal(top(&PileAnalyseurSyntaxique))
                if (Table_DAnalyse[SommetPile(PileAnalyseurSyntaxique)->code][CopieSuiteUL->CodeTerminal] != ERREUR) {
                    // printf("\ntest tab: %d Pile: %d\n", SommetPile(PileAnalyseurSyntaxique)->code , CopieSuiteUL->CodeTerminal);


                   // Numéro de R.G define
                    int codeNonTerminal = Table_DAnalyse[SommetPile(PileAnalyseurSyntaxique)->code][CopieSuiteUL->CodeTerminal];
                    PileAnalyseurSyntaxique = Depiler(PileAnalyseurSyntaxique);
                //    printf("after depiler %d\n",SommetPile(PileAnalyseurSyntaxique)->code);
                    Pile* p2 = InitialiserPile(p2);
                    p2 = Empiler(p2,SommetPile(PileAnalyseurSyntaxique)->code);
                    Element* ptrPileAnalyseurSyntaxique = PileAnalyseurSyntaxique->sommet;

                    ptrPileAnalyseurSyntaxique = ptrPileAnalyseurSyntaxique->next;  //this ==NULL
                   // printf("%d\n",ptrPileAnalyseurSyntaxique != NULL);
                    while(ptrPileAnalyseurSyntaxique != NULL) {
                        Inserer_a_la_fin(p2->sommet,ptrPileAnalyseurSyntaxique->code);
                        ptrPileAnalyseurSyntaxique = ptrPileAnalyseurSyntaxique->next;
                    }
                    //printf("\nNEW PILE\n");
                    //AfficherPile(p2);
                    ViderPile(PileAnalyseurSyntaxique);
                    PileAnalyseurSyntaxique = InitialiserPile(PileAnalyseurSyntaxique);
                    // int lastCode = PileAnalyseurSyntaxique->sommet->code;
                    // PileAnalyseurSyntaxique = Depiler(PileAnalyseurSyntaxique);
                    for (int i = 0; RPs[codeNonTerminal][i] != -1 && i < sizeof(RPs[0]) / sizeof(RPs[0][0]); i++) {
                        if(RPs[codeNonTerminal][i] != 111){
                            if(i == 0){

                                PileAnalyseurSyntaxique = Empiler(PileAnalyseurSyntaxique, RPs[codeNonTerminal][i]);
                                 printf("%d\n",PileAnalyseurSyntaxique->sommet->code);
                            }
                            else
                                Inserer_a_la_fin(PileAnalyseurSyntaxique->sommet, RPs[codeNonTerminal][i]);
                        }

                        //AfficherPile(PileAnalyseurSyntaxique);
                        //printf("%s",CopieUL->Lexeme);
                    }
                    //Inserer_a_la_fin(PileAnalyseurSyntaxique->sommet, lastCode);
                    //free(last);
                    PileAnalyseurSyntaxique = concatenerPile(PileAnalyseurSyntaxique,p2);
                    //printf("%d",PileAnalyseurSyntaxique->sommet->code)  ;
                    printListSyntaxe(PileAnalyseurSyntaxique, CopieSuiteUL);

                    //return;
                    //return;
                    // Empiler la Règle de production TA[top(&PileAnalyseurSyntaxique).code][Tete(CopieSuiteUL).code]
                    //dans PileAnalyseurSyntaxique
                    //
                    //PileAnalyseurSyntaxique = Empiler(PileAnalyseurSyntaxique, CopieSuiteUL->Lexeme,RPs[SommetPile(PileAnalyseurSyntaxique)->code][CopieSuiteUL->CodeTerminal]);
                    //
                    //pop(&PileAnalyseurSyntaxique);
                    //push(&PileAnalyseurSyntaxique, TA[top(&PileAnalyseurSyntaxique).code][Tete(CopieSuiteUL).code]);
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

        // Afficher l'état actuel de PileAnalyseurSyntaxique et CopieSuiteUL
        //printf("PileAnalyseurSyntaxique: ");
        /*for (int i = 0; i <= PileAnalyseurSyntaxique.top; i++) {
            printf("%c ", PileAnalyseurSyntaxique.elements[i].code);
        }*/
        //printf("\nCopieSuiteUL: %s\n", CopieSuiteUL);
    }

    // Vérifier si l'analyse syntaxique a réussi
    if (CopieSuiteUL->Code == FIN_SUITE_UL && SommetPile(PileAnalyseurSyntaxique)->code == FIN_SUITE_UL) {
       printf("\nAnalyse Syntaxique reussie\n");
    }
    else {
       // Erreur Syntaxique
       printf("Erreur Syntaxique\n");
    }
}

int main()
{

    FILE* file = fopen("requete SQL.txt", "r");
    int c, etat_automate = 0, etat_final = 0, error = 0;

    Pile* p = NULL;
    p = InitialiserPile(p);
    //analyseurSyntaxique(p, head);
    /*p = Empiler(p, "#", EPSILON);
    p = Empiler(p, "S", VN_S);
    AfficherPile(p);*/

    UL* head = NULL;
    int k=0;
    char lexeme[20];
    char errorletter;
    int code = 0;
    int ligne = 1;
    int index = 0;
    int lastCharWasSeparator = 0;
    int lastCharWasSpace = 0;
    int firstCharIsSpace = 0;

    printf("\tLECTURE ET AFFICHAGE DU CODE SOURCE LIGNE PAR LIGNE\n\n");
    while ((c = fgetc(file)) != EOF)
    {

        if (error == 1 && c == '\n')
        {

            break;
        }

        else
        {
            printf("%c", c);
        }

         if (isspace(c))
        {

            if (index == 0)  firstCharIsSpace = 1;
            else firstCharIsSpace = 0;
            if (lastCharWasSeparator == 0 && lastCharWasSpace == 0 && firstCharIsSpace == 0) {
                lexeme[index] = '\0';
                insertNode(&head, lexeme, UlCode(lexeme, &etat_final), UlCodeTerminal(lexeme, &etat_final), ligne);
                index = 0;
            }
            lastCharWasSpace = 1;

        }
        else if (c == ',' || c == ';' || c == '=' )
        {

            //printf("%c\n",c);
            if (lastCharWasSpace == 0)
            {
                lexeme[index] = '\0';
                insertNode(&head, lexeme, UlCode(lexeme, &etat_final), UlCodeTerminal(lexeme, &etat_final), ligne);
                index = 0;
            }else{
             lexeme[0] = c;
            lexeme[1] = '\0';
            if (c == ';') {
                insertNode(&head, lexeme, SEPARATEUR_POINT_VIRGULE, SEP_PNTVRG, ligne);
            }
            else {
                insertNode(&head, lexeme, UlCode(lexeme, &etat_final), UlCodeTerminal(lexeme, &etat_final), ligne);
            }
            index = 0;
            lastCharWasSeparator = 1;
            }

        }else if (c == '<' || c == '>')
        {

            lexeme[index++] = c;
            lastCharWasSeparator = 0;
            lastCharWasSpace = 0;
            k++;
        }

        else
        {
            lexeme[index++] = c;
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
            }
            else if (isdigit(c))
            {
                etat_automate = Q2;
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
            else if (c == '<' || c == '>')
            {
                etat_automate = Q5;
                etat_final = Q5;
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
        }

    }

    fclose(file);

    if (error != 1) {
        lexeme[0] = '#';
        lexeme[1] = '\0';
        insertNode(&head, lexeme, FIN_SUITE_UL, _D,ligne);
        analyseurSyntaxique(p,head);
        //printf("\n\n\tAfficher la suite des Unites Lexicales:\n\n");
        //printList(head);
        //printf("\n");
        freeList(head);
    }
    else
    {
        printf("\nErreur Lexical,character %c non Reconnu ligne %d", errorletter, ligne);
    }

    return 0;
}
