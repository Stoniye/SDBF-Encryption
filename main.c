// SPDX-FileCopyrightText: © 2025 Elias Steininger <elias.st4600@gmail.com>
// SPDX-License-Identifier: MIT License

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "xxhash/xxhash.h"

#define extLength 5
#define minPassLength 5
#define majorVersion 4

const char *header = "SDBF";
const char *version = "4.0.0";

char *action;
char *path;

int encrypt();
int decrypt();
void hash(const char* password, uint8_t* hash_output);

int main(int argc, char *argv[]) {
    if (argc < 3) {
        if (argc == 2) {
            action = strdup(argv[1]);

            if (strcmp(action, "help") == 0) {
                printf("To Encrypt a file use \"sdbf en <filePath>\"\nTo Decrypt a file use \"sdbf de <filePath>\"\nTo get your sdbf version use \"sdbf version\"\n\nFor more info read the usage documentation at https://gitlab.com/Stoniye/sdbf-encryption#usage\n");
                return 0;
            }
            if (strcmp(action, "version") == 0) {
                printf("You are running sdbf version %s\n\nYou can download other versions here https://gitlab.com/Stoniye/sdbf-encryption/-/releases\n", version);
                return 0;
            }
        }

        printf("Missing required arguments, use argument 'help' for help\n");
        return 1;
    }

    //Get arguments
    action = strdup(argv[1]);
    path = strdup(argv[2]);

    //Encrypt File
    if (strcmp(action, "en") == 0) {
        path = strtok(path, ".");
        return encrypt();
    }

    //Decrypt file
    if (strcmp(action, "de") == 0) {
        path = strtok(path, ".");
        return decrypt();
    }

    printf("Unknown action: %s, please read README at https://gitlab.com/Stoniye/sdbf-encryption#usage\n", action);
    return 1;
}

int encrypt() {
    //Get original extension
    char *extension = strtok(NULL, ".");

    //Get password for encryption
    char password[256];

    printf("Enter password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    if (strlen(password) == 0) {
        printf("No password entered!\n");
        return 1;
    }
    if (strlen(password) < minPassLength) {
        printf("Password to short, has to be at least %i character long, the longer the higher the security!\n", minPassLength);
        return 1;
    }

    //Hash password
    uint8_t password_hashed[16];
    hash(password, password_hashed);

    //Store bits of password
    size_t len = 16;
    int hashBits[len * 8];
    int bitCount = 0;
    for (size_t i = 0; i < len; i++) {
        for (int j = 7; j >= 0; j--) {
            int bit = (password_hashed[i] >> j) & 1;
            hashBits[bitCount] = bit;
            bitCount++;
        }
    }

    //Open files
    char fullPath[512];

    snprintf(fullPath, sizeof(fullPath), "%s.%s", path, extension);
    FILE *in = fopen(fullPath, "rb");
    if (!in) {
        perror("Error opening input file\n");
        return 1;
    }

    snprintf(fullPath, sizeof(fullPath), "%s.sdbf", path);
    FILE *out = fopen(fullPath, "wb");
    if (!out) {
        perror("Error opening output file\n");
        fclose(in);
        return 1;
    }

    //Check file extension length
    len = strlen(extension);
    if (len > extLength) {
        printf("File extension to long, only file extension with a max length of %d are supported\n", extLength);
        fclose(in);
        fclose(out);
        return 1;
    }

    //Store Data type in header
    len = strlen(header);
    for (size_t i = 0; i < len; i++) {
        fputc(header[i], out);
    }

    //Store Version in header
    fputc(majorVersion, out);

    //Store original extension in header
    size_t dif = extLength - len;
    for (size_t i = 0; i < len; i++) {
        fputc(extension[i], out);
    }

    //Fill up reserved space with zeros in header
    for (size_t i = 0; i < dif; i++) {
        fputc(0, out);
    }

    //Encrypt file
    int c;
    int i = 0;
    const int flipBit = hashBits[0] ^ hashBits[127];
    while ((c = fgetc(in)) != EOF) {
        unsigned char byte = (unsigned char)c;
        unsigned char newByte = 0;

        for (int bit = 7; bit >= 0; bit--) {
            int currentBit = (byte >> bit) & 1;

            //Flip bits based on the password hash
            int modifiedBit = 0;
            if (hashBits[i % bitCount] == flipBit) {
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

    printf("File encrypted successfully\n");

    return 0;
}

int decrypt() {
    //Get password for decryption
    char password[256];

    printf("Enter password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    if (strlen(password) == 0) {
        printf("No password entered!\n");
        return 1;
    }
    if (strlen(password) < minPassLength) {
        printf("Password to short, has to be at least %i character long, the longer the higher the security!\n", minPassLength);
        return 1;
    }

    //Hash password
    uint8_t password_hashed[16];
    hash(password, password_hashed);

    //Store bits of password
    size_t len = 16;
    int hashBits[len * 8];
    int bitCount = 0;
    for (size_t i = 0; i < len; i++) {
        for (int j = 7; j >= 0; j--) {
            int bit = (password_hashed[i] >> j) & 1;
            hashBits[bitCount] = bit;
            bitCount++;
        }
    }

    //Open files
    char fullPath[512];

    snprintf(fullPath, sizeof(fullPath), "%s.sdbf", path);
    FILE *in = fopen(fullPath, "rb");
    if (!in) {
        perror("Error opening input file\n");
        return 1;
    }

    //Check file type
    int i;
    for (i = 0; i < 4; ++i) {
        int byte = fgetc(in);
        if (byte != header[i]) {
            printf("Input file is not a .sdbf file: Header mismatch\n");
            fclose(in);
            return 1;
        }
    }

    //Get SDBF encryption version
    int encVersion = fgetc(in);
    if (encVersion != majorVersion) {
        printf("File got encrypted with sdbf major version %i, you are running major version %i. You can download other versions here https://gitlab.com/Stoniye/sdbf-encryption/-/releases\n", encVersion, majorVersion);
        return 1;
    }

    //Get original extension
    char extension[extLength + 1] = {0};
    for (i = 0; i < extLength; ++i) {
        int byte = fgetc(in);
        if (byte == EOF) {
            fclose(in);
            return 1;
        }
        if (byte != 0) {
            extension[i] = (char)byte;
        }
    }
    extension[i] = '\0';

    snprintf(fullPath, sizeof(fullPath), "%s.%s", path, extension);
    FILE *out = fopen(fullPath, "wb");
    if (!out) {
        perror("Error opening output file\n");
        fclose(in);
        return 1;
    }

    //Decrypt file
    int c;
    i = 0;
    const int flipBit = hashBits[0] ^ hashBits[127];
    while ((c = fgetc(in)) != EOF) {
        unsigned char byte = (unsigned char)c;
        unsigned char newByte = 0;

        for (int bit = 7; bit >= 0; bit--) {
            int currentBit = (byte >> bit) & 1;

            //Flip bits based on the password hash
            int modifiedBit = 0;
            if (hashBits[i % bitCount] == flipBit) {
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

    printf("File decrypted successfully\n");

    return 0;
}

void hash(const char* password, uint8_t* hash_output) {
    const XXH128_hash_t hash = XXH3_128bits_withSeed(password, strlen(password), 0);
    memcpy(hash_output, &hash, 16);
}
