
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>  // Consider adding string.h for strcmp
#include <cctype>
#include<string>

//make a lookup table
#define IDENT 0  //identifier
#define A_OP 1 //assign op
#define SEMI_COLON 91 //; ascii
#define ADD_OP 2
#define MUL_OP 3
#define CONST 4 //constant
#define L_PAR 72  //( ascii
#define R_PAR 73  //) ascii
#define UNKOWN 1000
#define LAMBDA 99
#define LINE_CHANGE 100
int current_index;
bool semi_back = true;
typedef struct {
    char name[50];
    char value[100];
}Ident;


char line[100];
FILE* file;
Ident idArray[50];
int next_token;   //save the token type of the lexeme
char token_string[100]; //save that lexeme
bool initial_lex = true;
void printResultByLine(char* line, int ID, int CON, int OP);
void printIdent(int num_ident);

void parse();
void parse_V();
void printOK();
void printToken(char* token);
void printOPWarning(int code);
void printIDError(char* name);
void assign_Value(char* name, char* value);
char* get_Value(char* name);

//lexical
void lexical();
//recursive functions 
void program();
void statement_s(); //statements and statement are different!!!! last updated 11.19 
void statement();
char* expression();
char* term_tail(char* term_result);
char* term();
char* factor_tail(char* factor_result);
char* factor();
bool is_Identfound(char* input);

char calname[50];
char calvalue[50];
char leftname[50];
char rhsval[50];
int id_index = 0;

bool okay_flag = true;
int id_num = 0;
int total_idnum = 0;
int op_num = 0;
int const_num = 0;
char line4print[100];
char NotSetVal[8];
char expression_op;

char undefined_idents[50][50];  //to print errors of undefined ints
int num_of_undefined=0;

int warning_code[50]; //to print warning codes;
int num_of_warcode = 0;

bool is_v = false;

int main() {

    char* filepath = NULL;

   

    file = fopen("test.txt" ,"r");

    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", filepath);
        return 1;
    }

    // Gets input line for the first time
    fgets(line, sizeof(line), file);

    parse_V();

    fclose(file);
    return 0;
}

void parse()
{
    strcpy(NotSetVal, "UNKOWN");
    memset(calname, 0, 50);
    memset(calvalue, 0, 50);
    // TODO: Implement the parsing logic here
  /*  for (int i = 0; i < sizeof(line); i++)
    {
        if (line[i] <= 32)
        {
            line[i] = ' ';
        }
    }  */
    char copied_line[100]; //saves the copy of "line"
    char original_line[100];
    int semi_index=0;
    memset(original_line, 0, 100);
    strcpy(original_line, line);
    strcpy(copied_line, line);
    bool semi_exist = false;

    char* token1 = strtok(copied_line, ";");
    for (int i = 0; i <= strlen(token1); i++)//
    {
        if (original_line[semi_index+i] == ';')
        {
            semi_index = semi_index + i;
            semi_exist = true;
            break;
        }
        else if (semi_index + i == strlen(original_line))
        {
            break;
        }
    }
    strcpy(line, token1);
    if (semi_exist == true)
    {
        strcat(line, ";");
    }
    semi_exist = false;
   
    memset(line4print, 0, 100);
    for (int i = 0; i < 50; i++)
    {
        memset(undefined_idents[i],0,50);
    }
    for (int i = 0; i < 50; i++)
    {
        warning_code[i]=0;
    }

    current_index = 0;
    //printf("line: %s\n", line);
    initial_lex = true;
    lexical();
    //printf("first: %s\n",token_string);
    initial_lex = false;
    program();
    printResultByLine(line4print, id_num, const_num, op_num);

    if (okay_flag == true)
    {
        printOK();
    }
    //printIdent(total_idnum); //total id_num
    for (int i = 0; i < num_of_undefined; i++)
    {
        printIDError(undefined_idents[i]);
    }

    for (int i = 0; i < num_of_warcode; i++)
    {
        printOPWarning(warning_code[i]);
    }

    id_num = 0;
    const_num = 0;
    op_num = 0;
    okay_flag = true;

    num_of_undefined = 0;
    num_of_warcode = 0;

   

    while (token1 != NULL)
    {
        token1 = strtok(NULL, ";");
        if (token1 == NULL || strcmp(token1 , "\n")==0)
        {
            break;
        }
      
        else if (original_line[strlen(original_line)] = '\n' && original_line[strlen(original_line) - 1] == ';')//
        {
            break;
        }
        else if (strcmp(token1, " ") == 0)
        {
            break;
        }

        
        for (int i = 0; i <= strlen(token1); i++)//
        {
            if (original_line[semi_index + i+1] == ';')
            {
                semi_index = semi_index + i+1;
                semi_exist = true;
                break;
            }
            else if (original_line[semi_index + i]=='\0' || original_line[semi_index + i] == '\n')
            {
                semi_exist= false;
                break;
            }
        }
        bool left_space = true;
        char token2[100];
        int index2 = 0;

        for (int i = 0; i <= strlen(token1); i++)
        {
            if (token1[i] == 32 && left_space == true)
            {
                //left_space = true;
            }
            else
            {
                token2[index2] = token1[i];
                index2++;
                left_space = false;
            }
        }
        strcpy(line, token2);
        if (semi_exist == true)
        {
            strcat(line, ";");
        }
        
        semi_exist = false;


        memset(line4print, 0, 100);
        for (int i = 0; i < 50; i++)
        {
            memset(undefined_idents[i], 0, 50);
        }
        for (int i = 0; i < 50; i++)
        {
            warning_code[i] = 0;
        }

        current_index = 0;
        //printf("line: %s\n", line);
        initial_lex = true;
        lexical();
        //printf("first: %s\n",token_string);
        initial_lex = false;
        program();
        printResultByLine(line4print, id_num, const_num, op_num);

        if (okay_flag == true)
        {
            printOK();
        }
        //printIdent(total_idnum); //total id_num
        for (int i = 0; i < num_of_undefined; i++)
        {
            printIDError(undefined_idents[i]);
        }

        for (int i = 0; i < num_of_warcode; i++)
        {
            printOPWarning(warning_code[i]);
        }

        id_num = 0;
        const_num = 0;
        op_num = 0;
        okay_flag = true;

    num_of_undefined = 0;
    num_of_warcode = 0;
    }
    memset(line, 0, 100);
    // starting from the second line
    //feed another line.

     while (fgets(line, sizeof(line), file) != NULL)
    {
        memset(original_line, 0, 100);
        memset(copied_line, 0, 100);

        semi_index = 0;
        for (int i = 0; i < strlen(line); i++)
        {
            if (line[i] <= 32)
            {
                line[i] = ' ';
            }
        }

        strcpy(original_line, line);
        strcpy(copied_line, line);
        bool semi_exist = false;

        char* token1 = strtok(copied_line, ";");

        for (int i = 0; i <= strlen(token1); i++)//
        {
            if (original_line[semi_index + i] == ';')
            {
                semi_index = semi_index + i;
                semi_exist = true;
                break;
            }
            else if (semi_index + i == strlen(original_line))
            {
                break;
            }
        }
        strcpy(line, token1);
        if (semi_exist == true)
        {
            strcat(line, ";");
        }
        semi_exist = false;

        memset(line4print, 0, 100);
        for (int i = 0; i < 50; i++)
        {
            memset(undefined_idents[i], 0, 50);
        }
        for (int i = 0; i < 50; i++)
        {
            warning_code[i] = 0;
        }

        current_index = 0;
        //printf("line: %s\n", line);
        initial_lex = true;
        lexical();
        //printf("first: %s\n",token_string);
        initial_lex = false;
        program();
        printResultByLine(line4print, id_num, const_num, op_num);

        if (okay_flag == true)
        {
            printOK();
        }
        //printIdent(total_idnum); //total id_num
        for (int i = 0; i < num_of_undefined; i++)
        {
            printIDError(undefined_idents[i]);
        }

        for (int i = 0; i < num_of_warcode; i++)
        {
            printOPWarning(warning_code[i]);
        }

        id_num = 0;
        const_num = 0;
        op_num = 0;
        okay_flag = true;

        num_of_undefined = 0;
        num_of_warcode = 0;

        while (token1 != NULL)
        {
            token1 = strtok(NULL, ";");

            if (token1 == NULL || strcmp(token1, "\n") == 0 )
            {
                break;
            }
            else if (original_line[strlen(original_line)] = '\n' && original_line[strlen(original_line)-1]==';')//
            {
                break;
            }
            else if (strcmp(token1, " ") == 0)
            {
                break;
            }
           


            for (int i = 0; i <= strlen(token1); i++)
            {
                if (original_line[semi_index + i+1] == ';')
                {
                    semi_index = semi_index + i+1;
                    semi_exist = true;
                    break;
                }
                else if (semi_index + i == strlen(original_line))
                {
                    break;
                }
            }
            bool left_space = true;
            char token2[100];
            int index2 = 0;

            for (int i = 0; i <= strlen(token1); i++)
            {
                if (token1[i] == 32 && left_space == true)
                {
                    //left_space = true;
                }
                else
                {
                    token2[index2] = token1[i];
                    index2++;
                    left_space = false;
                }
            }
            strcpy(line, token2);
            if (semi_exist == true)
            {
                strcat(line, ";");
            }

            semi_exist = false;


            memset(line4print, 0, 100);
            for (int i = 0; i < 50; i++)
            {
                memset(undefined_idents[i], 0, 50);
            }
            for (int i = 0; i < 50; i++)
            {
                warning_code[i] = 0;
            }

            current_index = 0;
            //printf("line: %s\n", line);
            initial_lex = true;
            lexical();
            //printf("first: %s\n",token_string);
            initial_lex = false;
            program();
            printResultByLine(line4print, id_num, const_num, op_num);

            if (okay_flag == true)
            {
                printOK();
            }
            //printIdent(total_idnum); //total id_num
            for (int i = 0; i < num_of_undefined; i++)
            {
                printIDError(undefined_idents[i]);
            }

            for (int i = 0; i < num_of_warcode; i++)
            {
                printOPWarning(warning_code[i]);
            }

            id_num = 0;
            const_num = 0;
            op_num = 0;
            okay_flag = true;

            num_of_undefined = 0;
            num_of_warcode = 0;
        }
 
    }
  
    printIdent(total_idnum); //total id_num
    
}


void parse_V()
{
    // TODO: Implement the parsing logic here for verbose output

    is_v = true;

    strcpy(NotSetVal, "UNKOWN");
    memset(calname, 0, 50);
    memset(calvalue, 0, 50);
    // TODO: Implement the parsing logic here
  /*  for (int i = 0; i < sizeof(line); i++)
    {
        if (line[i] <= 32)
        {
            line[i] = ' ';
        }
    }  */
    char copied_line[100]; //saves the copy of "line"
    char original_line[100];
    int semi_index = 0;
    memset(original_line, 0, 100);
    strcpy(original_line, line);
    strcpy(copied_line, line);
    bool semi_exist = false;

    char* token1 = strtok(copied_line, ";");
    for (int i = 0; i <= strlen(token1); i++)//
    {
        if (original_line[semi_index + i] == ';')
        {
            semi_index = semi_index + i;
            semi_exist = true;
            break;
        }
        else if (semi_index + i == strlen(original_line))
        {
            break;
        }
    }
    strcpy(line, token1);
    if (semi_exist == true)
    {
        strcat(line, ";");
    }
    semi_exist = false;

    memset(line4print, 0, 100);
    for (int i = 0; i < 50; i++)
    {
        memset(undefined_idents[i], 0, 50);
    }
    for (int i = 0; i < 50; i++)
    {
        warning_code[i] = 0;
    }

    current_index = 0;
    //printf("line: %s\n", line);
    initial_lex = true;
    lexical();
    //printf("first: %s\n",token_string);
    initial_lex = false;
    program();

    id_num = 0;
    const_num = 0;
    op_num = 0;
    okay_flag = true;

    num_of_undefined = 0;
    num_of_warcode = 0;



    while (token1 != NULL)
    {
        token1 = strtok(NULL, ";");
        if (token1 == NULL || strcmp(token1, "\n") == 0)
        {
            break;
        }

        else if (original_line[strlen(original_line)] = '\n' && original_line[strlen(original_line) - 1] == ';')//
        {
            break;
        }
        else if (strcmp(token1, " ") == 0)
        {
            break;
        }


        for (int i = 0; i <= strlen(token1); i++)//
        {
            if (original_line[semi_index + i + 1] == ';')
            {
                semi_index = semi_index + i + 1;
                semi_exist = true;
                break;
            }
            else if (original_line[semi_index + i] == '\0' || original_line[semi_index + i] == '\n')
            {
                semi_exist = false;
                break;
            }
        }
        bool left_space = true;
        char token2[100];
        int index2 = 0;

        for (int i = 0; i <= strlen(token1); i++)
        {
            if (token1[i] == 32 && left_space == true)
            {
                //left_space = true;
            }
            else
            {
                token2[index2] = token1[i];
                index2++;
                left_space = false;
            }
        }
        strcpy(line, token2);
        if (semi_exist == true)
        {
            strcat(line, ";");
        }

        semi_exist = false;


        memset(line4print, 0, 100);
        for (int i = 0; i < 50; i++)
        {
            memset(undefined_idents[i], 0, 50);
        }
        for (int i = 0; i < 50; i++)
        {
            warning_code[i] = 0;
        }

        current_index = 0;
        //printf("line: %s\n", line);
        initial_lex = true;
        lexical();
        //printf("first: %s\n",token_string);
        initial_lex = false;
        program();
      

        id_num = 0;
        const_num = 0;
        op_num = 0;
        okay_flag = true;

        num_of_undefined = 0;
        num_of_warcode = 0;
    }
    memset(line, 0, 100);
    // starting from the second line
    //feed another line.

    while (fgets(line, sizeof(line), file) != NULL)
    {
        memset(original_line, 0, 100);
        memset(copied_line, 0, 100);

        semi_index = 0;
        for (int i = 0; i < strlen(line); i++)
        {
            if (line[i] <= 32)
            {
                line[i] = ' ';
            }
        }

        strcpy(original_line, line);
        strcpy(copied_line, line);
        bool semi_exist = false;

        char* token1 = strtok(copied_line, ";");

        for (int i = 0; i <= strlen(token1); i++)//
        {
            if (original_line[semi_index + i] == ';')
            {
                semi_index = semi_index + i;
                semi_exist = true;
                break;
            }
            else if (semi_index + i == strlen(original_line))
            {
                break;
            }
        }
        strcpy(line, token1);
        if (semi_exist == true)
        {
            strcat(line, ";");
        }
        semi_exist = false;

        memset(line4print, 0, 100);
        for (int i = 0; i < 50; i++)
        {
            memset(undefined_idents[i], 0, 50);
        }
        for (int i = 0; i < 50; i++)
        {
            warning_code[i] = 0;
        }

        current_index = 0;
        //printf("line: %s\n", line);
        initial_lex = true;
        lexical();
        //printf("first: %s\n",token_string);
        initial_lex = false;
        program();
       
        id_num = 0;
        const_num = 0;
        op_num = 0;
        okay_flag = true;

        num_of_undefined = 0;
        num_of_warcode = 0;

        while (token1 != NULL)
        {
            token1 = strtok(NULL, ";");

            if (token1 == NULL || strcmp(token1, "\n") == 0)
            {
                break;
            }
            else if (original_line[strlen(original_line)] = '\n' && original_line[strlen(original_line) - 1] == ';')//
            {
                break;
            }
            else if (strcmp(token1, " ") == 0)
            {
                break;
            }



            for (int i = 0; i <= strlen(token1); i++)
            {
                if (original_line[semi_index + i + 1] == ';')
                {
                    semi_index = semi_index + i + 1;
                    semi_exist = true;
                    break;
                }
                else if (semi_index + i == strlen(original_line))
                {
                    break;
                }
            }
            bool left_space = true;
            char token2[100];
            int index2 = 0;

            for (int i = 0; i <= strlen(token1); i++)
            {
                if (token1[i] == 32 && left_space == true)
                {
                    //left_space = true;
                }
                else
                {
                    token2[index2] = token1[i];
                    index2++;
                    left_space = false;
                }
            }
            strcpy(line, token2);
            if (semi_exist == true)
            {
                strcat(line, ";");
            }

            semi_exist = false;


            memset(line4print, 0, 100);
            for (int i = 0; i < 50; i++)
            {
                memset(undefined_idents[i], 0, 50);
            }
            for (int i = 0; i < 50; i++)
            {
                warning_code[i] = 0;
            }

            current_index = 0;
            //printf("line: %s\n", line);
            initial_lex = true;
            lexical();
            //printf("first: %s\n",token_string);
            initial_lex = false;
            program();
          

            id_num = 0;
            const_num = 0;
            op_num = 0;
            okay_flag = true;

            num_of_undefined = 0;
            num_of_warcode = 0;
        }

    }


}

void program()  //<program>-><statement_s>
{
    statement_s();

    if (next_token != SEMI_COLON)
    {
        //printIDError(token_string); //if the sentence doesn't end with semicolon, print error
    }

}

void statement_s() // statement_s -> <statement> | <statement> <semicolon> <statements>
{
    statement();

    if (next_token == SEMI_COLON)
    {
       // strcat(line4print, "\n");
        lexical();
        
            
        statement_s();
        
    }
    else if(next_token == LINE_CHANGE || next_token == EOF) //이 줄은 끝
    {

    }
    else
    {

    }


}

void statement()
 {
    memset(leftname, 0, 50);

    if (next_token == IDENT) 
    {  // If it's an identifier
        strcpy(calname, token_string);
        if (is_Identfound(calname) == false)
        {
            strcpy(rhsval, "Unknown");//
            
            strcpy(idArray[total_idnum].name, calname); //total_idnum 1121
            assign_Value(idArray[total_idnum].name, rhsval);//initially set to unkown 
            total_idnum++;
        }
        strcpy(leftname, calname);
        lexical();  // Move to the next token
        //id_num++
        
        if (next_token == A_OP)
        { 
            lexical();  // Move to the next token after ":="
            
            assign_Value(leftname, expression()); //assign the result of expreeson()to LHS
        }
        else 
        {
           // printIDError(token_string);  // Handle error if assignment operator is missing
        }
    }
    else {
        //printIDError(token_string);  // Handle error if it's not an identifier
    }
}

char* expression() //<expression>-><term><term_tail>
{
    
    char term_result[50];
    static char exp_result[50];
    strcpy(term_result, term());
    if (strcmp(term_result, "Unknown") == 0)
    {
        strcpy(exp_result, "Unknown");
        term_tail(term_result);
    }
    else
    {
       strcpy(exp_result, term_tail(term_result));
    }
    //calvalue (exprop) term_tail_result
   
    return exp_result;
}
char* term_tail(char* term_result) //<term_tail> → <add_op><term><term_tail> | E  
//take care of +/-
{
    char term_tail_result[50];
    if (next_token == ADD_OP)
    {
        if (strcmp(token_string, "+") == 0 || strcmp(token_string, "++") == 0)
        {
            expression_op = '+';
        }
        else
        {
           
            expression_op = '-';
        }
        lexical();
        char termintermtail[50];
        char save_op = expression_op;
        strcpy(termintermtail,term());
        int temp_result = 0;
       static char temp[50];

        if (strcmp(termintermtail, "Unknown") == 0 || strcmp(term_result, "Unknown") == 0)
        {
            strcpy(term_tail_result, "Unknown");
        }
        else
        {
            if (save_op == '+')
            {
                temp_result = atoi(termintermtail) + atoi(term_result);
               strcpy(temp , _itoa(temp_result, term_tail_result, 10));
               
            }
            else
            {
                temp_result =   atoi(term_result) - atoi(termintermtail);
                strcpy(temp, _itoa(temp_result, term_tail_result, 10));
                
            }
        }
        term_tail(termintermtail);
        op_num++;
        return temp;
    }
    else if (next_token == LAMBDA)
    {
        return term_result;
    }
    else
    {
        return term_result;
       // printIDError(token_string);
    }
}

char* term() // <term>-><factor><factor_tail>
{
    char fac_result[50];
    static char term_result[50];
    strcpy(fac_result,factor());
    if (strcmp(fac_result, "Unknown") == 0)
    {
        strcpy(term_result, "Unknown");
        factor_tail(fac_result);
    }
    else
    {
        strcpy(term_result, factor_tail(fac_result));
    }
    return term_result;
}

char* factor_tail(char* factor_result) //<mult_op> -> <factor><factor_tail> | E
//take care of */-
{

    if (next_token == MUL_OP)
    {
        if (strcmp(token_string, "*") == 0 || strcmp(token_string, "*") == 0)
        {
            expression_op = '*';
        }
        else
        {

            expression_op = '/';
        }
        lexical();
        char falinfaltail[50];
        char save_op2 = expression_op;
        strcpy(falinfaltail, factor());
        int temp_result = 0;
       static char temp2[50];

        if (strcmp(falinfaltail, "Unknown") == 0 || strcmp(factor_result, "Unknown") == 0)
        {
            strcpy(falinfaltail, "Unknown");
        }
        else
        {
            if (save_op2 == '*')
            {
                temp_result = atoi(falinfaltail) * atoi(factor_result);
                strcpy(temp2, _itoa(temp_result, falinfaltail, 10));

            }
            else
            {
                temp_result = atoi(factor_result) / atoi(falinfaltail);
                strcpy(temp2, _itoa(temp_result,falinfaltail, 10));
            }
        }
        factor_tail(falinfaltail);
        op_num++;
        return temp2;
    }
    else if (next_token == LAMBDA)
    {
        return factor_result;
    }
    else  //semicolon leads to here
    {
        return factor_result;
       // printIDError(token_string);
    }
}

char* factor()
{
    static char result[50];
    if (next_token == L_PAR)
    {
        lexical();
        strcpy(result,expression());
        if (next_token == R_PAR)
        {
            lexical();
        }
        else //parenthesis not closed properly
        {
           //printIDError(token_string);
        }
    }
    else if (next_token == IDENT) //ident
    {
        char* value = get_Value(token_string);
        bool existence = is_Identfound(token_string);

        if (existence == false)  // rhs operand that doesn't exist;
        {
            okay_flag = false;
            
            strcpy(undefined_idents[num_of_undefined], token_string);
            num_of_undefined++;   //0->1 but saved at index_0

            strcpy(idArray[total_idnum].name, token_string);
            assign_Value(token_string, rhsval);
            total_idnum++;
            strcpy(result, "Unknown");
        }
        else 
        {
           
            strcpy(result, get_Value(token_string));
        }
        lexical();
        id_num++;
    }
    else if (next_token == CONST) //const
    {
        strcpy(calvalue, token_string);
        strcpy(result, token_string);
        lexical();
        const_num++;
    }
    else
    {
        //printIDError(token_string);
    }
    return result;
}

void lexical()//read line, analyze it, find one lexeme
{
    int token_index = 0;
    
    memset(token_string, 0, sizeof(token_string));
    
    char now = line[current_index];
  
    
    while (line[current_index] == ' ')
    {
        current_index++;
        if (current_index == sizeof(line)) //end of line
        {
            return;
        }
        strcat(line4print, " ");
         now = line[current_index];
    }

    if (now=='_'||isalpha(now) || (isdigit(now) && isalpha(line[current_index - 1])) || isdigit(now) && isalpha(line[current_index + 1]))
    {
        while (isalpha(now) || (isdigit(now) && isalpha(line[current_index - 1])) || isdigit(now) && isalpha(line[current_index + 1]))
        {
            token_string[token_index] = now;
            token_index++;
            current_index++;
            if (current_index == sizeof(line)) //end of line
            {
                break;
            }
            now =line[current_index] ;
        }
       // token_string[current_index] = '\0';
        next_token = IDENT;
        if (is_v == true)
        {
            char identprinter[6] = "ident";
            printToken(identprinter);
        }
        if (initial_lex == true)
        {
            id_num++;
        }
        //id_num++;
    }

    else if (isdigit(now) )
    {
        while (isdigit(now))
        {
            token_string[token_index] = now;
            token_index++;
            current_index++;
            if (current_index == sizeof(line)) //end of line
            {
                break;
            }
            now = line[current_index];
        }
       // token_string[current_index] = '\0';
        next_token = CONST;
        if (is_v == true)
        {
            char constprint[6] = "const";
            printToken(constprint);
        }
        if (initial_lex == true)
        {
            const_num++;
        }
        //const_num++;
    }  //\0으로 다 대체됨??
    else if (now == ':')
    {
        token_string[token_index] = ':';
        token_index++;
        current_index++;
        now = line[current_index];

        if (now == '=')
        {

            token_string[token_index] = '=';
            token_index++;
            current_index++;
        }
        now = line[current_index];
        next_token = A_OP;
        if (is_v == true)
        {
            char aop[20] = "assignment operator";
            printToken(aop);
        }
    }

    else if (now == '+')
    {
        token_string[token_index] = '+';
        token_index++;
        current_index++;
        now = line[current_index];

        if (now == '+')
        {
            current_index++;
            //printOPWarning(1);
            warning_code[num_of_warcode] = 1;
            num_of_warcode++;
            okay_flag = false;
        }
       
        now = line[current_index];
        next_token = ADD_OP;
        if (is_v == true)
        {
            char addprinter[13] = "add operator";
            printToken(addprinter);
        }
        if (initial_lex == true)
        {
            op_num++;
        }
       // op_num++;
    }

    else if (now == '-')
    {
        token_string[token_index] = '-';
        token_index++;
        current_index++;
        now = line[current_index];

        if (now == '-')
        {
            current_index++;
            //printOPWarning(2);
            warning_code[num_of_warcode] = 2;
            num_of_warcode++;
            okay_flag = false;
        }
        now = line[current_index];
        next_token = ADD_OP;
        if (is_v == true)
        {
            char addprinter[13] = "add operator";
            printToken(addprinter);
        }
        op_num++;
        if (initial_lex == true)
        {
            op_num++;
        }
    }

    else if (now == '*')
    {
        token_string[token_index] = '*';
        token_index++;
        current_index++;
        now = line[current_index];

        if (now == '*')
        {
            current_index++;
            //printOPWarning(3);
            warning_code[num_of_warcode] = 3;
            num_of_warcode++;
            okay_flag = false;
        }
        now = line[current_index];
        next_token = MUL_OP;
        if (is_v == true)
        {
            char multprinter[14] = "mult operator";
            printToken(multprinter);
        }
        //op_num++;
        if (initial_lex == true)
        {
            op_num++;
        }
    }
    else if (now == '/')
    {
        token_string[token_index] = '/';
        token_index++;
        current_index++;
        now = line[current_index];

        if (now == '/')
        {
            current_index++;
            //printOPWarning(4);
            warning_code[num_of_warcode] = 4;
            num_of_warcode++;
            okay_flag = false;
        }
        now = line[current_index];
        next_token = MUL_OP;
        if (is_v == true)
        {
            char multprinter[14] = "mult operator";
            printToken(multprinter);
        }
        //op_num++;
        if (initial_lex == true)
        {
            op_num++;
        }
    }
    else if (now == '=')
    {
        token_string[token_index] = ':';
        token_string[token_index+1] = '=';
        token_index+=2;
        current_index++;
        now = line[current_index];
    
      //printOPWarning(5);
        warning_code[num_of_warcode] = 5;
        num_of_warcode++;
        okay_flag = false;

        now = line[current_index];
        next_token = A_OP;
        if (is_v == true)
        {
            char aop[20] = "assignment operator";
            printToken(aop);
        }
    }

    else if (now == ';')
    {
        token_string[token_index] = ';';
        
        current_index++;
        
       // line[current_index] = '\n';
        next_token = SEMI_COLON;
        if (is_v == true)
        {
            char semiprint[11] = "semi colon";
            printToken(semiprint);
        }
        
    }
    else if (now == '(')
    { 
        token_string[token_index] = '(';
        token_index++;
        current_index++;
        next_token = L_PAR;
        if (is_v == true)
        {
            char lpprint[11] = "left paren";
            printToken(lpprint);
        }
      }
    else if (now == ')')
    {
        token_string[token_index] = ')';
        token_index++;
        current_index++;
        next_token = R_PAR;
        if (is_v == true)
        {
            char rpprint[12] = "right paren";
            printToken(rpprint);
        }
     }
    else if (now == '\0')
    {
        next_token = LAMBDA;
        memset(token_string, 0, 100);
        return;
    }
    else   if (line[current_index] == '\n')
    {
        next_token = LINE_CHANGE;
       // strcpy( line, line + current_index);
    }

    else
    {
        token_string[token_index] = now;
        token_string[token_index + 1] = '\0';
        current_index++;
        now = line[current_index];
        printIDError(token_string);
        next_token = UNKOWN;
    }
   // printf("token: strings = %s\n", token_string);
   strcat(line4print, token_string);
}

bool is_Identfound(char* input)
{
    for (int i = 0; i <= total_idnum; i++)
    {
        if (strcmp(idArray[i].name, input) == 0)
        {
            return true;
        }
    }
    return false;
}
void assign_Value(char* name, char* value)
{
    for (int i = 0; i <= total_idnum; i++)
    {
      
        if (strcmp(idArray[i].name, name) == 0)
        {
            strcpy(idArray[i].value, value); //assgin new value to existing ident
            return;
        }
    }
    // if not found
    strcpy(idArray[total_idnum].name, name);
    strcpy(idArray[total_idnum].value, value);
    total_idnum++;

 }

char* get_Value(char* name)
{
    for (int i = 0; i <= total_idnum; i++)
    {
        if (strcmp(idArray[i].name, name) == 0)
        {
             
            return idArray[i].value;
        }
    }
   
    return calvalue;
    // if not found
}


void printResultByLine(char* line, int ID, int CON, int OP) {
    printf("%s\n", line);
    printf("ID: %d; CONST: %d; OP: %d;\n", ID, CON, OP);
}


void printOPWarning(int code) {
    switch (code) {

    case 1:
        printf("(Warning) \"Eliminating duplicate operator (+)\"\n");
        break;
    case 2:
        printf("(Warning) \"Eliminating duplicate operator (-)\"\n");
        break;
    case 3:
        printf("(Warning) \"Eliminating duplicate operator (*)\"\n");
        break;
    case 4:
        printf("(Warning) \"Eliminating duplicate operator (/)\"\n");
        break;
    case 5:
        printf("(Warning) \"Substituting assignment operator (:=)\"\n");
        break;
    }
}
/**
 * @brief Function that prints OK sign
 *
 */
void printOK() {
    printf("(OK)\n");
}


void printIDError(char* name) {
    printf("(Error) \"referring to undefined identifiers(%s)\"\n", name);
}


void printIdent(int num_ident) {
    int i;
    printf("Result ==>");
    for (i = 0; i < num_ident; i++) {
        printf(" %s: %s;", idArray[i].name, idArray[i].value);
    }
}

void printToken(char* token) {
    printf("%s\n", token);
}

