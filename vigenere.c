/*
 * Vigenère Cipher Program - Raphael King
 *
 *
 * This program encrypts and decrypts text using the Vigenère Cipher.
 * It outputs a menu to the user with three options:
 *   1. Encrypt a message using a keyword
 *   2. Decrypt a message using a keyword
 *.  3. Encrypt/Decrypt a text file using a keyword
 *   4. Exit the program
 *
 * The cipher does not change the case of letters and ignores non-alphabetic 
 * characters, such as spaces and punctuation.
 *
 * Input is handled using fgets() for strings that may contain spaces, and the 
 * program clears the buffer between inputs.
 *
 * The encryption and decryption logic wraps around the alphabet, supports 
 * repeated keys, and shifts letters based on ASCII character codes.
 *
 */


#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

// Configuration
#ifndef FILEBUF
#define FILEBUF 4096
#endif

// Function prototype for encryption, decryption
char *encryptPassword(char *text, char *key, char *result);
char *decryptPassword(char *text, char *key, char *result);

// Function prototypes for file I/O 
static void trim_newline(char *s); // for internal use
static void clear_stdin_line(void);
int encryptFile(const char *inPath, const char *outPath, const char *key);
int decryptFile(const char *inPath, const char *outPath, const char *key);

int main()
{
    //Char array to hold the input text
	char text[100];
	//Char array to hold the key
	char key[100];
	//Char array to hold the result
	char result[100];
	//Integer value to hold the user's menu choice, set to sential value
	int menuChoice = -1;

    //While loop to continue until user exits program
	while (menuChoice != 4) {
	    //print out the menu
		puts("Welcome to Vigenere Cipher!");
		puts("  1 - Encrypt");
		puts("  2 - Decrypt");
		puts("  3 - File Encrypt/Decrypt");
		puts("  4 - Exit\n");

        //Get the user choice from menu
		printf("Please enter your choice from the menu -> ");
		scanf("%d", &menuChoice);

        //Input validation - make sure user selects a valid menu option
		while(menuChoice < 1 || menuChoice > 4) {
		    
		    //chear buffer
		    getchar();
		    
		    //get input again
			puts("\n- INVALID INPUT -");
			printf("Please enter your choice from the menu -> ");
			scanf("%d", &menuChoice);
		}

        //if user selects encryption
		if (menuChoice == 1) {
		    //clear the input buffer
			while ((getchar()) != '\n');

            //prompt user for the text they would like to encrypt
			printf("\nPlease enter the word you would like to encrypt: ");
			
			//use fgets so user can enter spaces
			fgets(text, sizeof(text), stdin);

			//remove new line from fgets
			size_t lenOfText = strlen(text) - 1;
			if(*text && text[lenOfText] == '\n') {
				text[lenOfText] = '\0';
			}

            //prompt user for key to shift by
			printf("Please enter the key word you would like to shift by: ");
			scanf("%99s", key);
			
			//print out the result
			printf("Encrypted Password: %s\n", encryptPassword(text, key, result));
			puts("");
		}

        //if user selects decryption
		if (menuChoice == 2) {
		    //clear the input buffer
			while ((getchar()) != '\n');
			
			//prompt user for the text they would like to decrypt
			printf("\nPlease enter the word you would like to decrypt: ");
			
			//use fgets so user can enter spaces
			fgets(text, sizeof(text), stdin);

			//remove new line from fgets
			size_t lenOfText = strlen(text) - 1;
			if(text[lenOfText] == '\n') {
				text[lenOfText] = '\0';
			}

             //prompt user for key to shift by
			printf("Please enter the key word you would like to reverse the shift by: ");
			scanf("%99s", key);
			
			//print out the result
			printf("Decrypted Password: %s\n", decryptPassword(text, key, result));
			puts("");
		}
		
		//if user selects file encryption/decryption
		if (menuChoice == 3) {
		    clear_stdin_line();
		    
		    //variable for file modetext
		    int option = 0;
		    
		    //path buffers
		    char inPath[300];
		    char outPath[300];
		    
		    //get input for file mode
		    puts("\nFile mode selected.");
		    puts("  1 - Encrypt file");
		    puts("  2 - Decrypt file");
		    printf("Choose -> ");
		    
            //validate user choice
            scanf("%d", &option);
            if (option != 1 && option != 2){
                puts("Invalid choice.\n"); 
                clear_stdin_line();
                continue;
        	} else {
        	    clear_stdin_line();
        	    
        	    //prompt for input and output paths
        	    printf("Input file path: ");
                if (!fgets(inPath, sizeof inPath, stdin)) {
                    puts("Read error for input path."); 
                } else {
                    //strip trailing newline(s)
                    inPath[strcspn(inPath, "\r\n")] = '\0';
                    
                    printf("Output file path: ");
                    if (!fgets(outPath, sizeof outPath, stdin)) {
                        puts("Read error for output path.");
                    } else {
                        outPath[strcspn(outPath, "\r\n")] = '\0';
        
                        // Read key
                        printf("Please enter the key word you would like to shift by: ");
                        if (scanf("%99s", key) != 1) {
                            puts("Key read error.");
                            clear_stdin_line();
                        } else {
                                int rc = 1;
                                if (option == 1)      rc = encryptFile(inPath, outPath, key);
                                else if (option == 2) rc = decryptFile(inPath, outPath, key);
                                else              puts("Invalid choice.");
        
                                if (rc == 0) puts("File operation completed successfully.\n");
                                else         puts("File operation failed.\n");
                            }
                        }
                    }
                }
        	}
        		
        
        }
        
        //exit the program
        if (menuChoice == 4) {
        	puts("Now exiting the program... Goodbye!");
        	exit(0);
        }
}

static void trim_newline(char *s) {
    size_t n = strlen(s);
    if (n && s[n-1] == '\n') s[n-1] = '\0';
}

static void clear_stdin_line(void) {
    int c; while ((c = getchar()) != '\n' && c != EOF) {}
}

//Encrypts text using the given key
char *encryptPassword(char *text, char *key, char *result) {
	//index for key
	int keyIndex = 0;
	for(int i = 0; i < strlen(text); i++) {

		//Encrypt alphabetic characters
		if(isalpha(text[i])) {
			//Get the letter from the key, repeating if needed
			char k = tolower(key[keyIndex % strlen(key)]);
			//Get the shift value
			int shift = k - 'a';

			//Encrypt uppercase letters
			if (isupper(text[i])) {
				//convert uppercase letter to a number, add shift value,
				//wrap around alphabet with mod 26, then convert back to char
				result[i] = (char)((text[i] - 'A' + shift) % 26 + 'A');
			}

			//Encrypt lowercase letters
			else if (islower(text[i])) {
				//convert lowercase letter to a number, add shift value,
				//wrap around alphabet with mod 26, then convert back to char
				result[i] = (char)((text[i] - 'a' + shift) % 26 + 'a');
			}
			//Move to the next letter in the key
			keyIndex++;

		} else {
			//Don't change non-alphabet characters
			result[i] = text[i];
		}
	}
	
	//Add null character
	result[strlen(text)] = '\0';
	
	//Return result
	return result;

}

//Decrypts the text using the given key
char *decryptPassword(char *text, char *key, char *result) {
	//Index for key
	int keyIndex = 0;
	for(int i = 0; i < strlen(text); i++) {
		//Decrypt alphabetic characters
		if(isalpha(text[i])) {
			//Get the letter from the key, repeating if needed
			char k = tolower(key[keyIndex % strlen(key)]);
			//Get the shift value
			int shift = k - 'a';

			//Decrypt uppercase letters
			if (isupper(text[i])) {
				//convert uppercase letter to a number, subtract shift value,
				//prevent negatives and mod 26, then convert back to char
				result[i] = (char)((text[i] - 'A' - shift + 26) % 26 + 'A');
			}
			//Decrypt lowercase letters
			else if (islower(text[i])) {
				//convert lowercase letter to a number, subtract shift value,
				//prevent negatives and mod 26, then convert back to char
				result[i] = (char)((text[i] - 'a' - shift + 26) % 26 + 'a');
			}
			//Move to the next letter in the key
			keyIndex++;

		} else {
			//Don't change non-alphabet characters
			result[i] = text[i];
		}
	}
	
	//Add null character
    result[strlen(text)] = '\0';
    
	//Return result
	return result;

}

//Encrypt a text file line-by-line the given key
int encryptFile(const char *inPath, const char *outPath, const char *key) {
    //Open input for reading
    FILE *in = fopen(inPath, "r");
    if (!in) {
        perror("Error opening input file");
        return 1;
    }

    // Open output for writing (truncates existing file)
    FILE *out = fopen(outPath, "w");
    if (!out) {
        perror("Error opening output file");
        fclose(in);
        return 1;
    }

    //Reusable line buffers sized for typical text lines
    char line[FILEBUF];
    char outLine[FILEBUF];

    //Process file line-by-line
    while (fgets(line, sizeof line, in)) {
        trim_newline(line);

        //Start encrypting
        if (!encryptPassword(line, (char *)key, outLine)) {
            fprintf(stderr, "Encryption error on current line.\n");
            fclose(in);
            fclose(out);
            return 1;
        }

        //Write transformed line and newline to preserve file structure
        if (fprintf(out, "%s\n", outLine) < 0) {
            perror("Write error");
            fclose(in);
            fclose(out);
            return 1;
        }
    }

    // Check for read errors
    if (ferror(in)) {
        fprintf(stderr, "Read error.\n");
        fclose(in);
        fclose(out);
        return 1;
    }

    // Flush and close output
    if (fclose(out) != 0) {
        perror("Close error (output)");
        fclose(in); // close input even if output close failed
        return 1;
    }

    // Close input
    fclose(in);
    return 0;
}

//Decrypt a text file line-by-line the given key
int decryptFile(const char *inPath, const char *outPath, const char *key) {
    //Open input for reading
    FILE *in = fopen(inPath, "r");
    if (!in) {
        perror("Error opening input file");
        return 1;
    }

    // Open output for writing (truncates existing file)
    FILE *out = fopen(outPath, "w");
    if (!out) {
        perror("Error opening output file");
        fclose(in);
        return 1;
    }

    //Reusable line buffers
    char line[FILEBUF];
    char outLine[FILEBUF];

    //Process file line-by-line
    while (fgets(line, sizeof line, in)) {
        trim_newline(line);

        //Start decrypting
        if (!decryptPassword(line, (char *)key, outLine)) {
            fprintf(stderr, "Decryption error on current line.\n");
            fclose(in);
            fclose(out);
            return 1;
        }
        
        //Write transformed line and newline to preserve file structure
        if (fprintf(out, "%s\n", outLine) < 0) {
            perror("Write error");
            fclose(in);
            fclose(out);
            return 1;
        }
    }

    // Check for read errors
    if (ferror(in)) {
        fprintf(stderr, "Read error.\n");
        fclose(in);
        fclose(out);
        return 1;
    }

    // Flush and close output
    if (fclose(out) != 0) {
        perror("Close error (output)");
        fclose(in);
        return 1;
    }

    // Close input
    fclose(in);
    return 0;
}
