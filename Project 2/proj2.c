
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAX_WORD_SIZE 128
#define MAX_WORD_LIST_SIZE 512

int main(int argc, char** argv) {
    // Validate number of arguments and help the user
    if (argc != 3) {
        printf("Word Counter\n");
        printf("Usage: proj2 <infile> <outfile>\n");

        return 1;
    }

    // Get input and output filenames
    const char* fileNameIn = argv[1];
    const char* fileNameOut = argv[2];

    // Access input file
    FILE* fileIn = fopen(fileNameIn, "r");

    // Fail if input file cannot be read
    if (!fileIn) {
        printf("Unable to open file \"%s\" for reading\n", fileNameIn);
        return 2;
    }

    // In parallel, the list of all words and list of their counts
    char wordList[MAX_WORD_LIST_SIZE][MAX_WORD_SIZE];
    int wordListCounts[MAX_WORD_LIST_SIZE];
    int wordListIndex = 0;

    // Word accumulator
    char word[MAX_WORD_SIZE];
    int wordIndex = 0;

    // Clear stuff
    memset(wordList, 0, MAX_WORD_LIST_SIZE * MAX_WORD_SIZE);
    memset(wordListCounts, 0, MAX_WORD_LIST_SIZE * sizeof(int));
    memset(word, 0, MAX_WORD_SIZE);

    // Trudge through file stream character-by-character
    char c;
    while ((c = fgetc(fileIn)) != EOF) {
        // If a whitespace character was read and the word accumulator is not empty
        if (isspace(c) && strlen(word) > 0) {
            // Trim non-alphanumeric characters from end
            for (int i = strlen(word) - 1; i >= 0; i--) {
                if (!isalnum(word[i])) {
                    word[i] = 0;
                }
            }

            // Whether word already exists
            int exists = 0;

            // If word already exists, increment its count
            for (int i = 0; i < MAX_WORD_LIST_SIZE; i++) {
                if (strcmp(word, wordList[i]) == 0) {
                    wordListCounts[i]++;
                    exists = 1;
                }
            }

            // If word does not already exist, add it to the list
            if (!exists) {
                memcpy(wordList[wordListIndex], word, strlen(word));
                wordListCounts[wordListIndex]++;
                wordListIndex++;
            }

            // Reset word accumulator
            memset(word, 0, MAX_WORD_SIZE);
            wordIndex = 0;
        } else {
            // Accept only alphanumeric characters unless buffer has content (this excludes leading specials)
            if (isalnum(c) || strlen(word) > 0) {
                // Add lowercase version of character to word accumulator
                word[wordIndex++] = tolower(c);
            }
        }
    }

    // Close input file
    fclose(fileIn);

    // Reinterpret the last list index as the unique word count
    int uniqueWordCount = wordListIndex;

    // Create output file
    FILE* fileOut = fopen(fileNameOut, "w");

    // Fail if output file cannot be opened
    if (!fileOut) {
        printf("Unable to open file \"%s\" for writing\n", fileNameOut);
        return 3;
    }

    // Each iteration's last iteration's lowest word
    char* lastLowestWord = NULL;

    // Iterate for number of unique words
    for (int i = 0; i < uniqueWordCount; i++) {
        // This iteration's lowest word above the last iteration's lowest word
        char* lowestWord = NULL;
        int lowestWordIndex = -1;

        // Iterate over unique words
        for (int j = 0; j < uniqueWordCount; j++) {
            char* word = wordList[j];

            // Skip any character lower than or equal to the last lowest word
            if (lastLowestWord != NULL && strcmp(word, lastLowestWord) <= 0) {
                continue;
            }

            // If word can qualify as this iteration's lowest, use it
            if (lowestWord == NULL || strcmp(word, lowestWord) < 0) {
                lowestWord = word;
                lowestWordIndex = j;
            }
        }

        // Write word count to output file
        fprintf(fileOut, "%-17s%d\n", lowestWord, wordListCounts[lowestWordIndex]);

        // Store lowest word for next iteration
        lastLowestWord = lowestWord;
    }

    // Close output file
    fclose(fileOut);

    return 0;
}
