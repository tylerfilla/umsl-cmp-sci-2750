#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define DEF_ARG_LENGTH "64"
#define DEF_ARG_STARTPOS "0"

int main(int argc, char** argv) {
    // Default arguments
    char* defArgLength = DEF_ARG_LENGTH;
    char* defArgStartpos = DEF_ARG_STARTPOS;

    // Extracted arguments
    char* argInfile;
    char* argLength = defArgLength;
    char* argStartpos = defArgStartpos;

    // Extract arguments based on their number and set defaults
    switch (argc) {
    case 2:
        argInfile = argv[1];
        break;
    case 3:
        argInfile = argv[1];
        argLength = argv[2];
        break;
    case 4:
        argInfile = argv[1];
        argLength = argv[2];
        argStartpos = argv[3];
        break;
    default:
        printf("Usage: proj3 FILE [LENGTH [SEEK]]\n");
        printf("       length and startpos are in decimal bytes\n");
        return 1;
    }

    // Parse length and start position arguments
    int length = atoi(argLength);
    int startpos = atoi(argStartpos);

    // Open input file
    int infile = open(argInfile, O_RDONLY);

    // If open failed
    if (infile < 0) {
        int errsv = errno;
        printf("Unable to open file (error %d)\n", errsv);
        printf("\"%s\"\n", strerror(errsv));
        return 2;
    }

    // Seek to desired starting position
    lseek(infile, startpos, SEEK_SET);

    // Print formatted output
    int bufferCount = 0;
    int fileCount = 0;
    unsigned char buffer[1024];
    unsigned char last16[16];
    int last = -1;
    int stop = 0;
    while (fileCount < length && (bufferCount = read(infile, buffer, sizeof(buffer))) > 0) {
        int i;
        for (i = 0; i < bufferCount; i++) {
            if (fileCount % 16 == 0) {
                if (fileCount > 0) {
                    // Print ASCII representation
                    int j;
                    for (j = 0; j < 16; j++) {
                        // A quick hack, as I apparently did not read the instructions and I don't want to redo this project
                        // I didn't design this program originally to produce the ASCII section, so I had to add the last16
                        // lookbehind thing (this check here is to cut off the ASCII on an incomplete row)
                        // I don't even think I use the original loop conditions anymore
                        if (last > -1 && j == last) {
                            stop = 1;
                            break;
                        }

                        if (last16[j] >= ' ' && last16[j] <= '~') {
                            printf("%c", last16[j]);
                        } else {
                            printf(".");
                        }
                    }
                }

                // Hack
                if (stop) {
                    break;
                }

                if (fileCount < length) {
                    // Begin new row
                    if (fileCount > 0) {
                        printf("\n");
                    }

                    // Print first index in row
                    printf("%x\t", fileCount + startpos);
                }

                last = -1;
            }

            // See my comment above about this terrible kludge
            if (fileCount == length) {
                last = fileCount % 16;
            }

            if (fileCount < length) {
                // Print byte as hex
                printf("%02x ", buffer[i]);
            } else {
                // Print padding (KLUDGE)
                printf("   ");
            }

            // Shift first element out of last 16 array and add this byte
            int k;
            for (k = 1; k < 16; k++) {
                last16[k - 1] = last16[k];
            }
            last16[15] = buffer[i];

            // Increment file counter
            fileCount++;
        }

        // Hack (I wish C had nested loop breaks)
        if (stop) {
            break;
        }
    }
    printf("\n");

    // Close input file
    close(infile);

    return 0;
}
