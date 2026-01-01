# SDBF Encryption

A free and open-source encryption tool.

---

## About

**SDBF Encryption** is a personal project to explore encryption methods. The name **SDBF** stands for **S**tring **D**ependent **B**it **F**lipping, a concept I've been thinking about and wanted to test its feasibility.

## Disclaimer

⚠️ **Warning: This is a proof-of-concept project and should NOT be used for sensitive or confidential data.** ⚠️

This tool was created for educational purposes and to experiment with a own encryption algorithm. There are many established and more robust encryption programs available that are suitable for securing important data. I am not liable for any data loss, corruption, or security breaches that may occur from using this software. Using this program could potentially expose unencrypted data chunks within the encrypted file.

## Features

* **Encrypts and Decrypts** files of any type using a password.

## Usage

SDBF has no graphical user interface; it's entirely terminal-based. To use it, open the terminal and navigate to the directory where you downloaded the file. To open the directory, run this command in your terminal:

<details>
<summary>Windows</summary>

Command:
```powershell
cd <pathToDirectory>
```

<br>

Example:
```powershell
cd C:\stonie\Downloads
```

</details>

<details>
<summary>Linux</summary>

Command:
```bash
cd <pathToDirectory>
```

<br>

Example:
```bash
cd /home/stonie/Downloads
```

<br>

On Linux, you may need to grant the file execution permission with this command:

```bash
chmod +x sdbf
```

</details>

To encrypt or decrypt a file, execute this command:

<details>
<summary>Windows</summary>

Encrypting a file:
```powershell
.\sdbf.exe en <filePath>
```

Decrypting a file:
```powershell
.\sdbf.exe de <filePath>
```

<br>

Example:
```powershell
.\sdbf.exe en C:\stonie\Documents\text.txt
```

</details>

<details>
<summary>Linux</summary>

Encrypting a file:
```bash
./sdbf en <filePath>
```

Decrypting a file:
```bash
./sdbf de <filePath>
```

<br>

Example:
```bash
./sdbf en /home/stonie/Documents/text.txt
```

</details>

After executing this command, you will be prompted to input a password. This will be used for encrypting or decrypting your file.

## Optional: Add SDBF to PATH

If you don’t want to navigate to the directory and type `./sdbf` or `.\sdbf.exe` every time, you can add the program to your system’s **PATH**. After doing this, you can simply run `sdbf ...` from any location in your terminal.

<details>
<summary>Windows</summary>

1. Move `sdbf.exe` to a permanent folder (for example: `C:\Program Files\SDBF`).
2. Press **Win + R**, type `sysdm.cpl`, and press Enter.
3. Go to the **Advanced** tab → click **Environment Variables**.
4. Under **System variables**, find and select `Path`, then click **Edit**.
5. Click **New** and add the folder where you placed `sdbf.exe` (e.g., `C:\Program Files\SDBF`).
6. Click **OK** to save and close all dialogs.
7. Open a new terminal (PowerShell or CMD) and test:

   ```powershell
   sdbf en C:/stonie/Documents/text.txt
   ```

</details>

<details>
<summary>Linux</summary>

1. Move `sdbf` to your local folder:

   ```bash
   sudo mv sdbf /usr/local/bin
   ```

   (`/usr/local/bin` is a standard place for custom executables).

2. Ensure it is executable:

   ```bash
   sudo chmod +x /usr/local/bin/sdbf
   ```

3. Now you can run it directly from anywhere:

   ```bash
   sdbf en /home/stonie/Documents/text.txt
   ```

> If you don’t want to move the file, you can instead add its current folder to your PATH by editing your shell config file (`~/.bashrc`, `~/.zshrc`, etc.):
>
> ```bash
> echo 'export PATH="$PATH:/home/stonie/Downloads"' >> ~/.bashrc
> source ~/.bashrc
> ```

</details>

## How It Works

This is a simple program, but I think it implements a reasonable level of security. The program hashes the password with the xxhash algorithm to ensure a consistent length no matter how long the password is, then cycles through the bits of the file. Based on the bits of the password, it flips certain bits in the file, producing a corrupted and encrypted version. If the bit of the file is flipped on a 1 or 0 of the password is decided by an XOR operation of the first and last bit of the hash to minimize patterns in the process.

```c
const int flipBit = hashBits[0] ^ hashBits[127];
```

```c
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
```

### Potential Issues

* The program cycles through every bit of the file, which can result in long processing times for larger files.
* Since only some bits are flipped while others remain unchanged, some patterns from the original file may still be exposed in the encrypted output.
* A pattern attack could be executed on the .sdbf file based on the header of the original file format (.txt, .png, ...) or other predictable things.

## .sdbf File

I defined my own file format for encrypted files, using the `.sdbf` extension.

### Header

The header consists of three parts:

1. **Magic number**: The first 4 bytes. I chose `"SDBF"` as the magic number for `.sdbf` files.
2. **Version**: A single byte indicating the version of the encryption algorithm used, ensuring compatibility with future changes.
3. **Original file extension**: 5 bytes reserved to store the original file extension (e.g., `.txt`, `.png`). If the extension is shorter than 5 bytes, the remaining bytes are padded with zeros.

<img width="3068" height="449" alt="sdbf_file_header" src="https://gitlab.com/Stoniye/sdbf-encryption/-/raw/main/header.webp" />

---

# Contributing

I welcome all contributions!

Feel free to open an issue, suggest a feature, or submit a pull request.

### Development Setup

To build the project from source, you’ll need a C compiler such as **gcc** or **clang**.

Example build command:

```bash
gcc main.c xxhash/xxhash.c -o sdbf
````

### Legal requirements

By submitting a pull request, you represent and warrant that:
- Your contribution is your original work or you have the necessary rights to submit it.
- It does not infringe any third-party copyrights, trademarks, or other intellectual property rights.
- You grant the project the right to use, modify, and distribute your contribution under the project's license.

---

# Releases

Stable builds are published regularly in the [Releases tab](https://gitlab.com/Stoniye/sdbf-encryption/-/releases).

Binaries are generated through GitLab Pipelines to ensure a transparent build process, so you can be confident that each release corresponds exactly to the source code in this repository.
