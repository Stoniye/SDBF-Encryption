#include <stdio.h>
#include <string.h>

int encrypt();
int decrypt();

char *action;
char *path;

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Missing required arguments, please read README at https://github.com/Stoniye/SDBF-Encryption");
        return 1;
    }

    action = strdup(argv[1]);
    if (!action) {
        perror("Failed to allocate memory");
        return 1;
    }

    path = strdup(argv[2]);
    if (!path) {
        perror("Failed to allocate memory");
        return 1;
    }

    if (strcmp(action, "en") == 0) {
        return encrypt();
    }
    if (strcmp(action, "de") == 0) {
        return decrypt();
    }

    printf("Unknown action: %s\n, please read README at https://github.com/Stoniye/SDBF-Encryption", action);
    return 1;
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
    char fullPath[512];

    snprintf(fullPath, sizeof(fullPath), "%s.txt", path);
    FILE *in = fopen(fullPath, "rb");
    if (!in) {
        perror("Error opening input file");
        return 1;
    }

    snprintf(fullPath, sizeof(fullPath), "%s.sdbf", path);
    FILE *out = fopen(fullPath, "wb");
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
    //Get password for decryption
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
    char fullPath[512];

    snprintf(fullPath, sizeof(fullPath), "%s.sdbf", path);
    FILE *in = fopen(fullPath, "rb");
    if (!in) {
        perror("Error opening input file");
        return 1;
    }

    snprintf(fullPath, sizeof(fullPath), "%s.txt", path);
    FILE *out = fopen(fullPath, "wb");
    if (!out) {
        perror("Error opening output file");
        fclose(in);
        return 1;
    }

    //Decrypt file
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