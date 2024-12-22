#include <stdio.h>
#include <string.h>
#include <ctype.h>

void evaluateExpression(char *line);
void getinput(char letters[50], char values[100], int *letterIndex);
void numberToWords(int num, char *result);
void writeResultToFile(char *result);

// Function to read input from file and process expressions
void getinput(char letters[50], char values[100], int *letterIndex) {
    FILE *fptr;
    // Open the input file for reading
    fptr = fopen("input.txt", "r");
    if (fptr == NULL) {
        printf("Error: Could not open file.\n");
        return;
    }

    // Create or clear the output file
    FILE *ffptr = fopen("output.txt", "w");
    if (ffptr == NULL) {
        printf("Error: Could not open output file.\n");
        return;
    }
    fclose(ffptr);  // Close the output file after clearing

    // Process each line from the input file
    while (fgets(values, 100, fptr)) {  // Read a line into `values`

        // Extract valid characters (letters or operators)
        for (int i = 0; values[i] != '\0'; i++) {
            if (isalpha(values[i]) || values[i] == '+' || values[i] == '-' || values[i] == '*') {
                letters[(*letterIndex)++] = values[i];
            }
        }

        // Evaluate the expression on the current line
        evaluateExpression(values);
    }

    // Close the input file
    fclose(fptr);
}

// Convert a Roman numeral character to its integer value
int romanCharToInt(char c) {
    switch (c) {
        case 'I': return 1;
        case 'V': return 5;
        case 'X': return 10;
        case 'L': return 50;
        case 'C': return 100;
        case 'D': return 500;
        case 'M': return 1000;
        default: return 0; // Return 0 for invalid characters
    }
}

// Convert a Roman numeral string to an integer
int romanToInteger(char *roman) {
    int total = 0, i = 0;
    while (roman[i] != '\0') {
        int current = romanCharToInt(roman[i]);
        int next = romanCharToInt(roman[i + 1]);

        // Handle subtraction cases (e.g., IV = 4)
        if (current < next) {
            total -= current;
        } else {
            total += current;
        }
        i++;
    }
    return total;
}

// Evaluate a mathematical expression with Roman numerals
void evaluateExpression(char *line) {
    char operand1[50] = {0}, operand2[50] = {0}, operator;
    int i = 0, j = 0;

    // Extract the first Roman numeral operand
    while (isalpha(line[i])) {
        operand1[j++] = line[i++];
    }
    operand1[j] = '\0'; // Null-terminate the string

    // Skip whitespace and find the operator
    while (line[i] == ' ') {
        i++;
    }
    if (line[i] == '+' || line[i] == '-' || line[i] == '*') {
        operator = line[i++];
    }

    // Skip whitespace and extract the second Roman numeral operand
    j = 0;
    while (line[i] == ' ') {
        i++;
    }
    while (isalpha(line[i])) {
        operand2[j++] = line[i++];
    }
    operand2[j] = '\0'; // Null-terminate the string

    // Convert Roman numerals to integers
    int num1 = romanToInteger(operand1);
    int num2 = romanToInteger(operand2);

    // Perform the operation
    int result = 0;
    switch (operator) {
        case '+': result = num1 + num2; break;
        case '-': result = num1 - num2; break;
        case '*': result = num1 * num2; break;
        default:
            printf("Invalid operator!\n");
            return;
    }

    // Print the result of the operation
    printf("%d %c %d = %d\n", num1, operator, num2, result);

    // Convert the result to words and write it to the output file
    char resultWords[200];
    numberToWords(result, resultWords);
    writeResultToFile(resultWords);
}

void numberToWords(int num, char *result) {
    const char *ones[] = { "", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine" };
    const char *tens[] = { "", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety" };
    const char *teen[] = { "Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen" };

    if (num == 0) {
        strcpy(result, "Zero");
        return;
    }

    char buffer[200] = "";

    // Handle negative numbers
    if (num < 0) {
        strcat(buffer, "Negative ");
        num = -num; // Convert to positive for processing
    }

    // Millions
    if (num >= 1000000) {
        sprintf(buffer + strlen(buffer), "%s Million ", ones[num / 1000000]);
        num %= 1000000;
    }

    // Hundreds of Thousands
    if (num >= 100000) {
        sprintf(buffer + strlen(buffer), "%s Hundred ", ones[num / 100000]);
        num %= 100000;
    }

    // Tens of Thousands
    if (num >= 20000) {
        sprintf(buffer + strlen(buffer), "%s Thousand ", tens[num / 10000]);
        num %= 10000;
    }

    // Handle teens in the thousands place
    if (num >= 10000) {
        sprintf(buffer + strlen(buffer), "%s Thousand ", teen[num / 1000 - 10]);
        num %= 1000;
    } else if (num >= 1000) {
        sprintf(buffer + strlen(buffer), "%s Thousand ", ones[num / 1000]);
        num %= 1000;
    }

    // Hundreds
    if (num >= 100) {
        sprintf(buffer + strlen(buffer), "%s Hundred ", ones[num / 100]);
        num %= 100;
    }

    // Tens
    if (num >= 20) {
        sprintf(buffer + strlen(buffer), "%s ", tens[num / 10]);
        num %= 10;
    }

    // Teens
    if (num >= 10) {
        sprintf(buffer + strlen(buffer), "%s ", teen[num - 10]);
        num = 0;
    }

    // Ones
    if (num > 0) {
        sprintf(buffer + strlen(buffer), "%s", ones[num]);
    }

    strcpy(result, buffer);
}

void writeResultToFile(char *result) {

    FILE *fptr = fopen("output.txt", "a"); // Open the file in append mode
    if (fptr == NULL) {
        printf("Error: Could not open output file.\n");
        return;
    }

    fprintf(fptr, "%s\n", result);  // Write the result to the file
    fclose(fptr);  // Close the file
}

int main() {
    char letters[500];  // Array to hold extracted letters/operators
    char values[1000];  // To read each line from the file
    int letterIndex = 0;

    // Process each line in the file
    getinput(letters, values, &letterIndex);

    return 0;
}
