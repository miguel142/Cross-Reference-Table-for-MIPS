#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Our Structs
struct node {
    char *label;
    int num;
    struct node *next;
};

//An Array of Structs
struct node row[100];


//Adds the words to the linked list
int addRow(int rowLine, int lineCount){
    
    if(row[rowLine].num == lineCount){
        return 0;
    }
    
    
    if(row[rowLine].next == NULL){
        row[rowLine].next = (struct node*)malloc(sizeof(struct node));
        struct node *temp = row[rowLine].next;
        temp->num = lineCount;
        temp->next = NULL;
        return 0;
    }
    
    struct node *temp = row[rowLine].next;
    while(temp->next != NULL){
        temp = temp->next;
    }
    
    temp->next = (struct node*)malloc(sizeof(struct node));
    temp->next->num = lineCount;
    temp->next->next = NULL;
    
    return 0;
}


//Checks for the use of the identifiers throught the whole file
int rowDivider(char* in, char* out, char fileLine[], int idLine, int tableLine){
    
    FILE *f1, *f2;
    f1 = fopen(in, "r");
    f2 = fopen(out, "w");
    int line = tableLine - 1;
    row[line];
    int i;
    int length = strlen(fileLine);
    char* identifier;
    identifier = malloc(50);
    int lineCount = 1;
    char* tempTok;
    
    if(fileLine[0] == '#'){
        return 0;
    }
    
    tempTok = strtok(fileLine, "#");
    
    identifier = strtok(tempTok, ":");

    int k = 0;
    int len = strlen(identifier);
    identifier[len+1] = '\0';
    
    
    row[line].label = malloc(100);
    strcpy(row[line].label, identifier);
    row[line].num = idLine;
    

    
    while (!feof(f1)) {
        
        char *compare;
        char *token;
        char *token2;
        char *tokenTemp2;
        char fileLine2[255];
        fgets(fileLine2, 255, f1);
        
        int len = strlen(fileLine2);
        char *tokenTemp = malloc(len + 2);
        strcpy(tokenTemp, fileLine2);
        int i = 0;
        
        while (fileLine2[i] != '\0') {
            i++;
        }
        
        int count = len - 3;
        tokenTemp[i - 2] = ' ';
        tokenTemp[i - 1] = ' ';
        tokenTemp[i] = '\0';
        
        token = strtok(tokenTemp, "#");
        
        
        if (strncmp(fileLine2, "#", 1) == 0) {
            lineCount++;
        } else {
            token2 = strtok(token, " ");
            
            while ((token2 = strtok(NULL, " ")) != NULL) {
                
                if (strcmp(identifier, token2) == 0) {
                    addRow(line, lineCount);
                }
            }
            lineCount++;
        }
        
    }
    
    fclose(f1);
    fclose(f2);
    
    return 0;
}



//Copies the file into another file
int copyCode(char* in, char* out){
    char fileLine[255];
    int line = 0;
    FILE *f1, *f2;
    f1 = fopen(in, "r");
    f2 = fopen(out, "w");
    
    while(!feof(f1)){
        fgets(fileLine, 255, f1);
        fputs(fileLine, f2);
        line++;
    }
    
    fclose(f1);
    fclose(f2);
    return 0;
}


//This creates a the cross refrence table
int table(char* in, char* out){
    int idLine = 1;
    int tableLine = 0;
    char fileLine[255];
    FILE *f1, *f2;
    f1 = fopen(in, "r");
    f2 = fopen(out, "w");
    
    while(!feof(f1)){
        fgets(fileLine, 255, f1);
        char* compare;
        compare = malloc(50);
        char* tempTok;
        tempTok = strtok(fileLine, "#");
        
        if(fileLine[0] == '#'){
            idLine++;
        }
        else{
        
        compare = strstr(tempTok, ": ");
        
        if(compare != NULL){
            tableLine++;
            rowDivider(in, out, fileLine, idLine, tableLine);
        }
        idLine++;
       }
        
    }
    
    fclose(f1);
    fclose(f2);
    return 0;
}

//This creates the cross table on the file
int crossTable(char* out){
    FILE *f1;
    f1 = fopen(out, "w");
    
    fprintf(f1, "Cross Reference table\n\n");
    fprintf(f1, "Identifier      Definition     Use\n\n");
    int i = 0;
    
    while(row[i].label != NULL) {
        
        if(row[i].next == NULL){
        
            fprintf(f1, "%7s %13d", row[i].label, row[i].num );
        }
        
        else {
            struct node *temp = row[i].next->next;
            fprintf(f1, "%7s %13d %12d", row[i].label, row[i].num, row[i].next->num);
            
            while (temp != NULL) {
                fprintf(f1, ", %d", temp->num);
                temp = temp->next;
            }
        }
        fprintf(f1, "\n");
        i++;
    }
    fclose(f1);
    return 0;
}



//Creates the cross table and the copy code
int crossTableAndCopyCode(char* in, char* out){
    
    char fileLine[255];
    int line = 0;
    FILE *f1, *f2;
    f1 = fopen(in, "r");
    f2 = fopen(out, "w");
    
    while(!feof(f1)){
        fgets(fileLine, 255, f1);
        fputs(fileLine, f2);
        line++;
    }
    
    fprintf(f2, "\n\n\n\n");
    fprintf(f2, "Cross Reference table\n\n");
    fprintf(f2, "Identifier      Definition     Use\n\n");
    
    int i = 0;
    table(in, out);
    
    while(row[i].label != NULL) {
        
        if(row[i].next == NULL){
            fprintf(f2, "%7s %13d\n", row[i].label, row[i].num );
        }
        
        else {
            struct node *temp = row[i].next->next;
            
            fprintf(f2, "%7s %13d %12d", row[i].label, row[i].num, row[i].next->num);
            
            while (temp != NULL) {
                fprintf(f2, ", %d", temp->num);
                temp = temp->next;
            }
            fprintf(f2, "\n");
        }
        i++;
    }
    
    fclose(f1);
    fclose(f2);
    return 0;
}


//The main function the calls the other functions
int main(int argc, char *argv[]){
    
    char* flag = argv[1];
    char* input = argv[2];
    char* output = argv[3];
    
    //Copies the code
    if(strcmp(flag, "-l")==0){
        copyCode(input, output);
    }
    
    //Calls the function cross table
    if(strcmp(flag, "-b")==0){
        crossTableAndCopyCode(input, output);
    }
    
    
    //
    if(strcmp(flag, "-c")==0){
        table(input, output);
        crossTable(output);
    }
    
    return 0;
}