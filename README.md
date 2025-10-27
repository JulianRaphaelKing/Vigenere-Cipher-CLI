# Vigenère Cipher (C, CLI) — by Raphael King

A small, menu-driven C program that encrypts and decrypts text using the classic **Vigenère cipher**.  
Supports both **interactive text** and **file I/O** (encrypt/decrypt a whole text file, line by line).

> Educational use only. Vigenère is not cryptographically secure.

---

## Features
- Menu-driven CLI:
  1. Encrypt a line of text (preserves case & punctuation)
  2. Decrypt a line of text
  3. Encrypt/Decrypt a text file (line-by-line)
  4. Exit
- Preserves punctuation and whitespace; shifts **A–Z/a–z** only.
- Simple, readable **file I/O** using `fopen` / `fgets` / `fprintf`.
- Makefile with convenience targets (`make`, `make run`, `make sample`, `make compare`, etc.).

---

## Build & Run

### Prerequisites
- **macOS:** `xcode-select --install` (for `cc`/`make`)
- **Linux:** `sudo apt install build-essential` (or distro equivalent)
- **Windows:** MSYS2/MinGW, Git Bash, or WSL with GCC & make

### Build
```bash
make
```

### Run (interactive menu)
```bash
make run
```

---

## File I/O Demo

Create a sample plaintext:
```bash
make sample
```

Run the app, then choose in the menu:
- **3** → **1** to **Encrypt file**  
  - Input: `test_plain.txt`  
  - Output: `test_encrypted.txt`  
  - Key: `lemon`
- **3** → **2** to **Decrypt file**  
  - Input: `test_encrypted.txt`  
  - Output: `test_decrypted.txt`  
  - Key: `lemon`

Verify decrypted = original:
```bash
make compare
# Defaults: PLAIN=test_plain.txt, DEC=test_decrypted.txt
# Or override:
# make compare PLAIN=original.txt DEC=after_decrypt.txt
```

---

## Make Targets
```bash
make         # build the program (vigenere or vigenere.exe)
make run     # run the interactive menu
make debug   # debug build with symbols
make clean   # remove the binary
make sample  # create test_plain.txt with sample lines
make compare # cmp PLAIN vs DEC (override with PLAIN=... DEC=...)
make rebuild # clean + build
```

*Windows note:* `compare` uses `cmp` (POSIX). In MSYS2/Git Bash/WSL you’re fine. Otherwise, open the files or use `fc`.

---

## Usage Notes & Limitations
- **Alphabetic only:** Only letters are shifted; everything else is copied through.
- **Case-preserving:** Uppercase stays uppercase; lowercase stays lowercase.
- **File mode is line-by-line:** The key resets at the start of each new line.
- **ASCII text expected:** This program targets simple text files.
- **Overwriting:** If the output file already exists, it will be **overwritten**.
- **Do not use the same path** for input and output in file mode (use different files).

---

## Example
Plain:
```
Attack at dawn!
```
Key: `lemon`  
Cipher:
```
Lxfopv ef rnhr!
```

---

## Project Structure
```
.
├─ vigenere.c      # main program (menu + cipher + file I/O helpers)
├─ Makefile        # build/run/clean/sample/compare targets
├─ README.md       # this file
└─ (generated at runtime)
   ├─ test_plain.txt
   ├─ test_encrypted.txt
   └─ test_decrypted.txt
```

---

## Future Ideas
- Streaming mode that keeps the key index across the entire file (not line-by-line)
- Key validation + clearer error messages in the menu
- Automated comparison within program

---

## License
MIT 
