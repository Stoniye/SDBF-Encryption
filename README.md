# SDBF Encryption

A free and open-source encryption tool.

---

## About

**SDBF Encryption** is a personal project to explore encryption methods. The name **SDBF** stands for **S**tring **D**ependent **B**it **F**lipping, a concept I've been thinking about and wanted to test its feasibility.

---

## Disclaimer

⚠️ **Warning: This is a proof-of-concept project and should NOT be used for sensitive or confidential data.** ⚠️

This tool was created for educational purposes and to experiment with a own encryption algorithm. There are many established and more robust encryption programs available that are suitable for securing important data. I am not liable for any data loss, corruption, or security breaches that may occur from using this software. Using this program could potentially expose unencrypted data chunks within the encrypted file.

---

## Features

* **Encrypts and Decrypts** files of any type using a password.

---

## Usage

Encrypting a file:
```bash
sdbf en <filePath>
```

Decrypting a file:
```bash
sdbf de <filePath>
```

After executing this command you will be prompted to input a password, this will be used to encrypting or decrypting your file

## How It Works

This is a simple program, but I think it implements a reasonable level of security. The program stores both the file and the password in binary form, then cycles through the bits of the file. Based on the bits of the password, it flips certain bits in the file, producing a corrupted and encrypted version.

```c
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
```

### Potential Issues

* The program cycles through every bit of the file, which can result in long processing times for larger files.
* Since only some bits are flipped while others remain unchanged, some patterns from the original file may still be exposed in the encrypted output.

## .sdbf File

I created my own file format for encrypted files, using the `.sdbf` extension.

### Header

The header consists of three parts:

1. **Magic number**: The first 4 bytes. I chose `"SDBF"` as the magic number for `.sdbf` files.
2. **Version**: A single byte indicating the version of the encryption algorithm used, ensuring compatibility with future changes.
3. **Original file extension**: 5 bytes reserved to store the original file extension (e.g., `.txt`, `.png`). If the extension is shorter than 5 bytes, the remaining bytes are padded with zeros.

# Contributing

I welcome all contributions!

Feel free to open an issue, suggest a feature, or submit a pull request.

# Releases

Stable builds will be published regularly in the [Releases tab](https://github.com/Stoniye/SDBF-Encryption/releases) for download.