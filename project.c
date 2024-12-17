#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to return each single character Roman numeral to their corresponding decimal number
int RomanNumeral(char ch) { //takes character as an input
    switch (ch) { //switch to return integer
        case 'I': return 1; // I = 1
        case 'V': return 5; // V = 5
        case 'X': return 10;// X = 10
        case 'L': return 50;// L = 50
        case 'C': return 100;//C = 100
        case 'D': return 500;//D = 500
        case 'M': return 1000;//M = 1000
        default: return 0;
    }
}

// Function to convert Roman numeral to its decimal value
int RomanToDecimal(char *roman) { // parameter string pointer roman
    int total = 0;//initializes total sum of the final decimal value
    int length = strlen(roman); // gets the length of the roman numeral
    int currentVal, nextVal; // variables to be used in loop

    int i;// for loop count
    for (i = 0; i < length; i++) {
        currentVal = RomanNumeral(roman[i]); // calls RomanNumeral function current character
        nextVal = (i + 1 < length) ? RomanNumeral(roman[i + 1]) : 0; // checks the next value

        if (currentVal < nextVal) { //If the current value is less than the next value, subtract the current value if not then add
            total -= currentVal;
        } else {
            total += currentVal; // if condition doesn't satisfy then add them
        }
    }

    return total; // return final value
}

// Function to convert decimal number to words
void NumberToWords(int val, char *result) {// val for the final value to be converted, and will store the words in *result
    char *ones[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"}; //for single digits 1-9
    char *teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen",
                     "Sixteen", "Seventeen", "Eighteen", "Nineteen"}; // for teens number 10-19
    char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};// tens numbers
    char *thousands[] = {"", "Thousand", "Million", "Billion"}; // for place values of thousands, millions and billion

    char temp[1024] = "";  // Temporary buffer to hold intermediate words
    int place = 0; // tracks the current place of the number
    int chunk;   //Will store the current group of 3 digits being processed

    if (val == 0) { //when the input number is 0, lines copy string zero
        strcpy(result, "Zero");
        return;
    }

    while (val > 0) { // the number being converted is greater than 0.
        chunk = val % 1000; // extract last 3 digits
        val /= 1000; // Remove the last three digits that have been processed

        if (chunk > 0) { //checks chunk greater to zero
            char chunkStr[256] = ""; //temporarily stores the words of 3 digit

            // condition to process hundreds place
            if (chunk / 100 > 0) { // isolates the hundreds place of the chunk
                sprintf(chunkStr + strlen(chunkStr), "%s Hundred ", ones[chunk / 100]); //append hundred
            }

            chunk %= 100; //removes the hundreds place

            // condition to process tens and ones place
            if (chunk >= 10 && chunk <= 19) {// if chunk is between 10-19
                sprintf(chunkStr + strlen(chunkStr), "%s ", teens[chunk - 10]); //extract teens place and word added to chunkStr
            } else {
                if (chunk / 10 > 0) {//  isolates tens value
                    sprintf(chunkStr + strlen(chunkStr), "%s ", tens[chunk / 10]); // tens added to chunkStr
                }
                if (chunk % 10 > 0) { // gets the ones place
                    sprintf(chunkStr + strlen(chunkStr), "%s ", ones[chunk % 10]);// ones added to chunkStr
                }
            }

            // Add place value (thousands, millions, billion)
            if (place > 0) { //ensures place value is added when greater than 0
                sprintf(chunkStr + strlen(chunkStr), "%s ", thousands[place]); //appends the corresponding place value
            }

            // Append this chunk to the final result
            strcat(chunkStr, temp); // Add previously processed words in temp to chunkStr
            strcpy(temp, chunkStr); // Update temp
        }

        place++; //increments place value
    }

    strcpy(result, temp); // store final result
}

int main() {//main function
    char inputFileName[100], outputFileName[100]; // stores input and output file names

    printf("Enter the input file name: "); //Enter input.txt filename
    scanf("%s", inputFileName); //stores the filename

    printf("Enter the output file name: "); //Enter the output.txt filename
    scanf("%s", outputFileName); //stores the filename

    FILE *inputFile = fopen(inputFileName, "r"); //opens and reads the input file
    FILE *outputFile = fopen(outputFileName, "w");// opens an write the final result of the conversion in output.txt

    if (!inputFile || !outputFile) { //if filename can't be seen display error opening
        printf("Error opening files.\n");
        return 1;
    }

    char line[256];// buffer line with max 256 to hold each line from input file
    while (fgets(line, sizeof(line), inputFile)) { // reads line one at a time
        char roman1[50], roman2[50], operation; // variables for roman numerals and operation used
        int decimal1, decimal2, result;

        // Parse the input line
        if (sscanf(line, "%s %c %s", roman1, &operation, roman2) != 3) { // 3 components= roman numeral, operation, roman numeral
            fprintf(outputFile, "Invalid input format: %s\n", line); // if components is not 3 display error
            continue;
        }

        // Convert Roman numerals to decimal
        decimal1 = RomanToDecimal(roman1); //calls function to convert roman to decimal for decimal1
        decimal2 = RomanToDecimal(roman2); //calls function to convert roman to decimal for decimal2

        // Perform the specified operation
        if (operation == '+') { //performing the arithmetic operation
            result = decimal1 + decimal2; //adding decimal
        } else if (operation == '-') {
            result = decimal1 - decimal2; // subtracting decimal
        } else if (operation == '*') {
            result = decimal1 * decimal2; // multiplying decimal
        } else if (operation == '/') {
            result = (decimal2 != 0) ? decimal1 / decimal2 : 0; // dividing decimal, checks decimal2 is not zero
        } else {
            fprintf(outputFile, "Invalid operation: %c\n", operation); //if none of the operation above is used
            continue;
        }

        // Convert the result to words
        char words[1024];
        NumberToWords(result, words);// calls function number to words

        // Writing the output to the output file
        fprintf(outputFile, "%d %c %d = %d\n%s\n\n", decimal1, operation, decimal2, result, words);
    }

    fclose(inputFile); //closes the input file
    fclose(outputFile); //closes the output file

    printf("Roman Numerals converted to decimal. Check the output file for the result.\n");// prints if all the process is successful and output is ready

    return 0;
}
