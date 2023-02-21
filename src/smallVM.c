#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
const int FILESIZE = 1024;
char buffer[1024];

// I got this from chatGPT
int hexCharToInt(char c) {
    if (isdigit(c)) {
        return c - '0';
    }
    if (islower(c)) {
        return c - 'a' + 10;
    }
    if (isupper(c)) {
        return c - 'A' + 10;
    }
}

// I made this myself based on hexCharToInt()
char intToHexChar(int n) {
    if (n >= 0 && n <= 9) {
        return '0' + n;
    }
    if (n >= 10 && n <= 16) {
        return 'a' + (n - 10);
    }
    return -1;
}


// pass in the first index of a hexedecimal int in the buffer and the amount of digits, return int
int readHex(int index, int num_chars) {
    int mult = 1;
    int ret = 0;
    for (int i = index; i < index + num_chars; i++) {
        ret += (hexCharToInt(buffer[i]) * mult);
        if (i < index + num_chars - 1) {
            ret = ret << 4;
        }
    }
    return(ret);
}

// get the index of the first character of the next token in the buffer
int nextToken(int index) {
    while (buffer[index] != ' ') {
        index+=1;
    }
    return index + 1;
}

// get the first index of the next line in the buffer
int nextLine(int index) {
    while (buffer[index] != '\n') {
        index++;
    }
    return index + 1;
}

// pass in the first index of an idenifier and return the first index of the int stored in memory
int getIdentifierIndex(int index) {
    // assume index is pointing at '$'
    int token_length = 0;
    for (int i = index; buffer[i] != ' ' && buffer[i] != '\n'; i++) {
        token_length++;
    }
    int identifier = readHex(index + 1, token_length);
    return readHex(0, 4) + (identifier * 8);
}

// pass in the first index of an identifier or int literal, and return the value of the identifer or the value of the literal
int getValue(int index) {
    int token_length = 0;
    int startindex = index;
    if (buffer[index] == '$') { // if its an idenfifier
        return readHex(getIdentifierIndex(index), 8);
    }
    if (isdigit(buffer[index]) || isalpha(buffer[index])) {
        while(isdigit(buffer[index]) || isalpha(buffer[index])) {
            token_length++;
            index++;
        }
        return readHex(startindex, token_length);
    }
}

// pass in the first index of an identifier, and an int; set the identifier to store the value
void setValue(int index, int value) {
    // assume the index is pointing at a '$' 
    int idindex = getIdentifierIndex(index);
    for (int i = idindex + 7; i > idindex - 1; i--) {
        buffer[i] = intToHexChar(value % 16);
        value = value >> 4;
    }
}

// dump the buffer to output (cuurently not working fully)
int DUMP(int index) {
    printf(buffer);fflush(stdout);
}

// pass in the first index of an identifier or string literal, and print the value to STDOUT
int OUT(int index) {
    if (buffer[index + 1] == '$') {
        printf("%d\n", getValue(index + 1));fflush(stdout);
    }
    else if (buffer[index + 1] == '"') {
        for (int i = index + 2; buffer[i] != '"' && buffer[i] != '\n'; i++) {
            printf("%c", buffer[i]);
        }
        printf("\n");
        fflush(stdout);
    }
    return nextLine(index);
}

// takes the second token and stores its value in the first
int STO(int index) {
    setValue(index + 1, getValue(nextToken(index)));
    return nextLine(index);
}

// stop program execution
int HALT(int index) {
    exit(0);
    return(index);
}

// take value from STDIN and store it in an identifier
int IN(int index) {
    int num;
    scanf("%d", &num);fflush(stdin);
    setValue(index + 1, num);
    return nextLine(index);
}

// arithmetic functions
int ADD(int index) {
    setValue(index + 1, getValue(nextToken(index)) + getValue(nextToken(nextToken(index))));
    return nextLine(index);
}
int DIV(int index) {
    setValue(index + 1, getValue(nextToken(index)) / getValue(nextToken(nextToken(index))));
    return nextLine(index);
}
int SUB(int index) {
    setValue(index + 1, getValue(nextToken(index)) - getValue(nextToken(nextToken(index))));
    return nextLine(index);
}
int MUL(int index) {
    setValue(index + 1, getValue(nextToken(index)) * getValue(nextToken(nextToken(index))));
    return nextLine(index);
}

int execute(int index) {
    switch (buffer[index]) {
        case '+':
            return ADD(index);
            break;
        case '/':
            return DIV(index);
            break;
        case '-':
            return SUB(index);
            break;
        case '*':
            return MUL(index);
            break;
        case 'h':
            return HALT(index);
            break;
        case 'i':
            return IN(index);
            break;
        case 'o':
            return OUT(index);
            break;
        case 's':
            return STO(index);
            break;
        case 'd':
            return DUMP(index);
            break;
    }
}

int main(int argc, char *argv[]) {
    FILE *fileptr;
    char file[50];
    if (argc >= 2) {
        fileptr = fopen(argv[1], "r");
    } else {
        fileptr = fopen("smallvm.out", "r");
    }


    if (fileptr == NULL) {
        printf("File could not be found");fflush(stdout);
        return 1;
    }

    fread(buffer, sizeof(buffer), 1, fileptr); // Read the contents of the file into buffer
    fclose(fileptr); // close the file


    int instruction_index = 9; // 9 is always index of first instruction
    while (true) { // this is the fetch execute cycle
        instruction_index = execute(instruction_index); // execute() executes the line and returns the index of the next line
        if (instruction_index >= readHex(0,4)) { // just in case HALT doesnt exist or is skipped
            exit(0);
        }
    }
    return 0;
}