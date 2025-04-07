# 💻 Machine Language Simulator in C++

## 📌 Overview
This project simulates the basic architecture of a computer processor with components such as:
- **Memory**
- **Registers**
- **ALU (Arithmetic Logic Unit)**
- **CU (Control Unit)**
- **CPU (Central Processing Unit)**
- **Main UI Interface**

The simulator can load instructions from a file, decode and execute them while displaying internal state changes.

---

## 📦 Components

- **Memory**: Simulates RAM, stores string values at different addresses.
- **Register**: Holds temporary values for CPU operations.
- **ALU**: Handles arithmetic and logic operations (add, AND, OR, XOR, rotate).
- **CU**: Handles instruction decoding and issuing commands to the ALU and memory.
- **CPU**: Coordinates all components and maintains program counter and instruction register.
- **Machine**: Ties the CPU and Memory together.
- **MainUI**: Provides interaction functions for file input, instruction loading, and menu display.

---

