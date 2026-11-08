# ForkOff

**ForkOff** is a lightweight, custom UNIX shell written in C, focused on process isolation, efficient memory usage, and zero memory leaks. 

It was built from scratch as a hands-on systems programming project, applying concepts from *Beej's Guide to C Programming*, Linux `man` pages, and direct observation of POSIX shell behavior.

---

## 🚀 Features & Implementation

* **Process Management:** Executes system binaries using `fork()`, `execvp()`, and `waitpid()`.
* **Built-in Commands:** Parent-process execution for core shell routines like `cd` and `exit`.
* **Dynamic Memory Handling:** Custom string parsing and tokenization built with raw C dynamic allocations (`malloc`, `realloc`, `free`).
* **Zero Memory Leaks:** Verified runtime stability and heap management using Valgrind.

---

## 🚧 Project Status & Roadmap

> *"Busy, I am. Slowly working on this, I progress."* 🧙‍♂️

- [x] Single-file REPL engine and POSIX process execution loop.
- [x] Modularizing the codebase into a clean multi-file structure (`parser.c`, `builtins.c`, `exec.c`).
- [/] **In Progress:** Implementing I/O redirection (`>`, `<`) via `dup2()`.
- [ ] **Next:** Multi-stage command pipelines (`|`) via `pipe()`.
- [ ] **Next:** Robust signal handling (`SIGINT`, `SIGTSTP`).

---

## 🛠️ Build & Run

### Prerequisites
* `gcc` compiler
* `make` build tool
* Linux / POSIX-compliant environment

### Compilation
Build the executable using the included Makefile:
```bash
make
```

### Execution
Launch the shell:
```bash
./ForkOff
```

---

## 🤝 Seeking Feedback & Code Reviews

I am actively looking for **code reviews** and community feedback regarding:
* Memory safety and pointer boundary edge cases.
* Idiomatic C design and readability.
* Architectural patterns for IPC/Piping.

If you find this project interesting or educational, feel free to leave a ⭐ on the repository!

---

## 📜 License & Attribution

This project is open-source. You are free to use, modify, and distribute it, provided you include attribution by linking back to this repository and crediting **Anchit Jain**.

---

> *"Do. Or do not. There is no try."*  
> *May the Force be with you.*
