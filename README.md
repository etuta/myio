# Custom Buffered File I/O Library (`myio`)

This project implements a custom buffered file I/O abstraction in C.  
It re-creates core functionalities of `open`, `read`, `write`, `seek`, and `close` system calls while managing an internal buffer.

---

## Overview

**myio.c** demonstrates:

- Manual implementation of file descriptors encapsulated in a `struct`.
- Buffered read operations (`myread()`).
- File opening and closing (`myopen()`, `myclose()`).
- File seeking (`myseek()`).

This project is useful for:

- **IT professionals** seeking to understand how low-level I/O operations work.
- **Systems programmers** needing precise control over I/O buffering.
- **Educators** teaching the concepts of file system interfaces in POSIX environments.

---

## Features

- `myopen()`:
  - Opens a file descriptor.
  - Initializes a buffer and tracking fields.
- `myread()`:
  - Reads data in chunks into an internal buffer.
  - Copies data to the user buffer while handling partial reads and buffer boundaries.
- `myseek()`:
  - Supports repositioning within a file.
- `myclose()`:
  - Closes the file descriptor and frees associated memory.
- `BYTES_PER_ITERATION` constant to define chunked reads (adjustable).

> ⚠️ *Note:* `mywrite()` is included as commented code and can be implemented similarly to `myread()`.

---

## Building

Ensure you have a POSIX C compiler (e.g., `gcc`).

Use the provided Makefile:

```
make
```

This produces:

- `myio`: the executable that demonstrates reading with the custom library.

To clean up:

```
make clean
```

---

## Running

**Basic Read Test:**

```bash
./myio <filename>
```

Example:

```
./myio test
```

This reads chunks of data from `test` and prints debug output, including:

- Bytes read
- Buffer content
- Offsets

---

## Example Output

```
Open file test

***********************
my read output = 25
content in user buffer = Lorem ipsum dolor sit a
Bytes read = 25
my buf_offset = 25
Content in my buf = Lorem ipsum dolor sit amet,
***********************

***********************
my read output = 10
content in user buffer = et, consect
Bytes read = 10
my buf_offset = 35
Content in my buf = Lorem ipsum dolor sit amet,
***********************
```

---

## Technical Details

- **Buffering**:
  - Custom internal buffer of fixed size (`20` bytes by default).
  - Handles overflow and partial buffer reads.
- **Struct-based State**:
  - Tracks file descriptor, buffer offsets, bytes read/written.
- **Educational Focus**:
  - Demonstrates the mechanics behind stdio buffering (e.g., `fread()`).
  - Useful for understanding performance trade-offs in IT systems.

---

## Potential Extensions

- Fully implement `mywrite()` for buffered writing.
- Add `myflush()` to flush buffered writes explicitly.
- Support SEEK_END in `myseek()`.
- Expand error handling and edge cases.

---

## Authors

- Eliza-Teodora Tuta
- Tenzin Dolma Gyalpo
