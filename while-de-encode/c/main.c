#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int countDigits(int num){
    int count = 1;
    while(num > 10){
        num = num/10;
        count++;
    }
    return count;
}

int int_pow(int a, int b){
    if (b == 0){
        return 1;
    }
    int result = 1;
    while(b != 0){
        result *= a;
        b--;
    }
    return result;
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
    struct String* string = malloc(sizeof(struct String));
    string->string = malloc((strlen(inputString)+1)*sizeof(char));
    strcpy(string->string, inputString);
    return string;
}

struct String* newStringFromInt(int index){
    struct String* head = malloc(sizeof(struct String));
    head->string = malloc((countDigits(index)+1)*sizeof(int));
    sprintf(head->string, "%d", index);
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

    int* mn = calloc(2, sizeof(int));
    mn[1] = ++index;
    while(index % 2 == 0){
        mn[0]++;
        mn[1]/=2;
    }
    mn[1] /= int_pow(2, mn[0]);
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
            return str;

        case 4:
            index -= 4;
            index /= 5;
            values = inverse_phi(index);

            str = inverse_phi_arithmetic(values[0]);
            end =  endOfString(str);
            end->next = newString(" \\times ");
            end = end->next;
            end->next = inverse_phi_arithmetic(values[1]);
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
            str = newString("\\lnot ");
            str->next = inverse_phi_boolean(index);
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
            return str;
    };
}

struct String* inverse_phi_statement(int index, int indentation){
    struct String* str;
    str = createIndentation(3 * indentation);
    if (index == 0) {
        str->next = newString("skip\n");
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
            end->next = newString("while");
            end = end->next;
            end->next = inverse_phi_boolean(values[0]);
            end =  endOfString(end);
            end->next = newString("do \n");
            end = end->next;
            end->next = inverse_phi_statement(values[1], indentation + 1);
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
            return str;

        case 2:
            index -= 2;
            index /= 4;
            index--;
            values = inverse_phi(index);

            end->next = inverse_phi_statement(values[0], 0) ;
            end =  endOfString(end);
            end->next =  newString(";"); 
            end = end->next;
            end->next = inverse_phi_statement(values[1], 0);
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
            end->next = inverse_phi_statement(statements[0], indentation + 1);

            return str;
    }
}

void printString(struct String* string){
    while(string->next != NULL){
        printf("%s", string->string);
        string = string->next;
    }
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
