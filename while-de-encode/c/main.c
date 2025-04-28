#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

bool DEBUG = false;

int countDigits(int num){
    if (num == 0) return 1; // 0 has 1 digit
    int count = 0;
    while(num != 0){
        num /= 10;
        count++;
    }
    return count;count;
}


struct String {
    char* string;
    struct String* next;
};

void freeString(struct String* head){
    struct String* tmp;
    while(head != NULL){
        tmp = head;
        head = head->next;
        free(tmp->string);
        free(tmp);
    }
}

struct String* endOfString(struct String* head){
    while(head->next != NULL){
        head = head->next;
    }
    return head;
}

struct String* newString(char* inputString){
    if (DEBUG) printf("String to allocate %s\n", inputString);
    struct String* string = malloc(sizeof(struct String));
    string->string = malloc((strlen(inputString)+1)*sizeof(char));
    strcpy(string->string, inputString);
    string->next = NULL;
    return string;
}

struct String* newStringFromInt(int index){
    struct String* head = malloc(sizeof(struct String));
    head->string = malloc((countDigits(index)+1)*sizeof(char));
    sprintf(head->string, "%d", index);
    head->next = NULL;
    return head;
}

struct String* createIndentation(int spaceCount){
    struct String* string = malloc(sizeof(struct String));
    string->string = malloc((spaceCount+1)*sizeof(char));
    
    string->string[0] = '\0'; // <-- Initialize as empty string

    for (int i = 0; i < spaceCount; i++) {
        strcat(string->string, " ");
    }

    return string;
}



int* inverse_phi(int index){
    if(DEBUG)
    printf("ENTER INVERSE PHI: %d\n", index);
    
    int* mn = calloc(2, sizeof(int));
    mn[1] = ++index;
    
    if(DEBUG){
        printf("VALUE M: %d\n", mn[0]);
        printf("VALUE N: %d\n\n", mn[1]);
    }

    while(mn[1] % 2 == 0){

        mn[0]++;
        mn[1]/=2;
        
        if (DEBUG){
        printf("VALUE M: %d\n", mn[0]);
        printf("VALUE N: %d\n\n", mn[1]);
    
        }
    }

    mn[1]--;
    mn[1]/=2;
    
    return mn;

}

struct String* inverse_phi_variable(int index) {
    struct String* head = newString("x_{");
    struct String* tmp = head;

    head->next = newStringFromInt(index); 
    head=head->next;
    head->next = newString("}"); 
    
    return tmp;
}

struct String* inverse_phi_arithmetic(int index){
    if (DEBUG){
    printf("INVERSE ARITHMETIC: %d\n", index);
    printf("CASE: %d\n", index%5);
    }
    int* values;
    struct String* str;
    struct String* end;
    switch (index % 5) {
        case 0:
            return newStringFromInt(index/ 5);

        case 1:
            index--;
            index /= 5;

            return inverse_phi_variable(index);

        case 2:
            index -= 2;
            index /= 5;
            values = inverse_phi(index);

            str = inverse_phi_arithmetic(values[0]);
            end =  endOfString(str);
            end->next = newString(" + ");
            end = end->next;
            end->next = inverse_phi_arithmetic(values[1]);
            free(values);
            return str;
        
        case 3:
            index -= 3;
            index /= 5;
            values = inverse_phi(index);

            str = inverse_phi_arithmetic(values[0]);
            end =  endOfString(str);
            end->next = newString(" - ");
            end = end->next;
            end->next = inverse_phi_arithmetic(values[1]);
            free(values);
            return str;

        case 4:
            index -= 4;
            index /= 5;
            
            values = inverse_phi(index);
            if (DEBUG){
                printf("VALUE M: %d\n", values[0]);

                printf("VALUE N %d\n", values[1]);
            }
            str = inverse_phi_arithmetic(values[0]);
            end =  endOfString(str);
            end->next = newString(" \\times ");
            end = end->next;
            end->next = inverse_phi_arithmetic(values[1]);
            free(values);
            return str;
    }
}

struct String* inverse_phi_boolean(int index){
        
    if (index == 0) {
        return newString("\\false");
    } else if (index == 1) {
        return newString("\\true");
    }

    int* values;
    struct String* str;
    struct String* end;

     switch (index % 4) {
        case 0:
            index -= 4;
            index /= 4;
            str = newString(" \\lnot ");
            str->next = inverse_phi_boolean(index);
            free(values);
            return str;
        
        case 1:
            index -= 5;
            index /= 4;
            values = inverse_phi(index);

            str = inverse_phi_boolean(values[0]);
            end =  endOfString(str);
            end->next = newString(" \\land ");
            end = end->next;
            end->next = inverse_phi_boolean(values[1]);
            free(values);
            return str;

        case 2:
            index -= 2;
            index /= 4;
            values = inverse_phi(index);

            str = inverse_phi_arithmetic(values[0]);
             end =  endOfString(str);
            end->next = newString(" = ");
            end = end->next;
            end->next = inverse_phi_arithmetic(values[1]);
            free(values);
            return str;
        
        case 3:
            index -= 3;
            index /= 4;
            values = inverse_phi(index);

            str = inverse_phi_arithmetic(values[0]);
            end =  endOfString(str);
            end->next = newString(" \\leq ");
            end = end->next;
            end->next = inverse_phi_arithmetic(values[1]);
            free(values);
            return str;
    };
}

struct String* inverse_phi_statement(int index, int indentation){
    struct String* str;
    str = createIndentation(3 * indentation);
    if (index == 0) {
        str->next = newString("skip");
        return str;
    }
    
    int* values;
    int* statements;
    struct String* end = str;

    switch (index % 4) {
        case 0:
            index /= 4;
            index--;
            values = inverse_phi(index);
            end->next = newString("while ");
            end = end->next;
            end->next = inverse_phi_boolean(values[0]);
            end =  endOfString(end);
            end->next = newString(" do \n");
            end = end->next;
            end->next = inverse_phi_statement(values[1], indentation + 1);
            free(values);
            return str;

        case 1:
            index -= 1;
            index /= 4;
            values = inverse_phi(index);

            end->next = inverse_phi_variable(values[0]);
            end = endOfString(end); 
            end->next = newString(" := ");
            end = end->next;
            end->next = inverse_phi_arithmetic(values[1]);
            end = endOfString(end); 
            end->next = newString("\n");
            free(values);
            return str;

        case 2:
            index -= 2;
            index /= 4;
            values = inverse_phi(index);

            end->next = inverse_phi_statement(values[0], 0) ;
            end =  endOfString(end);
            end->next =  newString(";\n"); 
            end = end->next;
            end->next = inverse_phi_statement(values[1], 0);
            free(values);
            return str;

        case 3:
            index -= 3;
            index /= 4;
            values = inverse_phi(index);
            statements = inverse_phi(values[1]);

            end->next = newString("if "); 
            end = end->next;
            end->next =  inverse_phi_boolean(values[0]);
            end = endOfString(end);
            end->next =  newString(" then \n");
            end = end->next;
            end->next = inverse_phi_statement(statements[0], indentation + 1);
            end = endOfString(end);
            end->next = newString(" else\n");
            end = end->next;
            end->next = inverse_phi_statement(statements[1], indentation + 1);

            free(values);
            free(statements);
            return str;
    }
}

void printString(struct String* string){
    while(string != NULL){
        printf("%s", string->string);
        string = string->next;
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    int input;
    do{
        printf("Enter a number:");
        scanf("%d", &input);
        struct String* code = inverse_phi_statement(input, 0);
        printString(code);
        freeString(code);
    }while(input >= 0);
    return EXIT_SUCCESS;
}
