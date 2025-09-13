#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int encrypt();
int decrypt();

int main() {
    return decrypt();
}

int encrypt() {
    //Get password for encryption
    char password[256];

    printf("Enter password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    //Store bits of password
    size_t len = strlen(password);
    int passwordBits[len * 8];
    int bitCount = 0;
    for (size_t i = 0; i < len; i++) {
        for (int j = 7; j >= 0; j--) {
            int bit = (password[i] >> j) & 1;
            passwordBits[bitCount] = bit;
            bitCount++;
        }
    }

    //Open files
    FILE *in = fopen("/home/elias/Coding/SDBF-Encryption/test.txt", "rb");
    if (!in) {
        perror("Error opening input file");
        return 1;
    }

    FILE *out = fopen("/home/elias/Coding/SDBF-Encryption/test.sdbf", "wb");
    if (!out) {
        perror("Error opening output file");
        fclose(in);
        return 1;
    }

    //Encrypt file
    int c;
    int i = 0;
    while ((c = fgetc(in)) != EOF) {
        unsigned char byte = (unsigned char)c;
        unsigned char newByte = 0;

        for (int bit = 7; bit >= 0; bit--) {
            int currentBit = (byte >> bit) & 1;

            //Flip bits based on the password
            int modifiedBit = 0;
            if (passwordBits[i % bitCount] == 1) {
                modifiedBit = currentBit ^ 1;
            }else {
                modifiedBit = currentBit;
            }

            newByte |= (modifiedBit << bit);

            i++;
        }

        fputc(newByte, out);
    }

    fclose(in);
    fclose(out);

    return 0;
}

int decrypt() {
    //Get password for encryption
    char password[256];

    printf("Enter password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    //Store bits of password
    size_t len = strlen(password);
    int passwordBits[len * 8];
    int bitCount = 0;
    for (size_t i = 0; i < len; i++) {
        for (int j = 7; j >= 0; j--) {
            int bit = (password[i] >> j) & 1;
            passwordBits[bitCount] = bit;
            bitCount++;
        }
    }

    //Open files
    FILE *in = fopen("/home/elias/Coding/SDBF-Encryption/test.sdbf", "rb");
    if (!in) {
        perror("Error opening input file");
        return 1;
    }

    FILE *out = fopen("/home/elias/Coding/SDBF-Encryption/test.txt", "wb");
    if (!out) {
        perror("Error opening output file");
        fclose(in);
        return 1;
    }

    //Encrypt file
    int c;
    int i = 0;
    while ((c = fgetc(in)) != EOF) {
        unsigned char byte = (unsigned char)c;
        unsigned char newByte = 0;

        for (int bit = 7; bit >= 0; bit--) {
            int currentBit = (byte >> bit) & 1;

            //Flip bits based on the password
            int modifiedBit = 0;
            if (passwordBits[i % bitCount] == 1) {
                modifiedBit = currentBit ^ 1;
            }else {
                modifiedBit = currentBit;
            }

            newByte |= (modifiedBit << bit);

            i++;
        }

        fputc(newByte, out);
    }

    fclose(in);
    fclose(out);

    return 0;
}