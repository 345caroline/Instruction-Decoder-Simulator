# Instruction Decoder Simulator Detailed Guide

## Introduction
The Instruction Decoder Simulator is a software tool designed to simulate the decoding and execution of machine instructions in a simplified computer architecture environment. It provides insights into the operational principles of CPUs, including instruction cycle phases such as fetching, decoding, execution, and memory management. This simulator is particularly useful for educational purposes, offering a hands-on approach to understanding the inner workings of computer architecture.

## Architecture Overview
The simulation models a minimalistic CPU architecture with a focus on instruction processing and memory management. Key components of this architecture include:

- **Memory:** A simulated 64K (65536 bytes) memory space used for storing instructions and data. This memory space is addressed byte-wise, allowing for fine-grained control and manipulation of data.
- **Registers:**
  - **Accumulator (ACC):** A general-purpose register used for arithmetic and logic operations.
  - **Instruction Register (IR):** Holds the currently executing instruction.
  - **Memory Address Register (MAR):** Holds the address of the memory location being accessed.
  - **Program Counter (PC):** Points to the next instruction to be fetched from memory.
  - **Memory Buffer Register (MBR):** Temporarily holds data being transferred between memory and the CPU.

## Simulation Workflow
The simulation follows a structured workflow to process instructions from the input file (`mem_in.txt`), execute them, and then output the final memory state to a file (`mem_out.txt`). The main steps in this workflow include:

### 1. Initialization
Upon starting, the simulator reads the initial memory state from `mem_in.txt`. This file contains a series of hexadecimal values representing both instructions and initial data. The `initializeMemory()` function is responsible for loading these values into the simulated memory space.

### 2. Instruction Cycle
The core of the simulation is the instruction cycle, which continuously fetches, decodes, and executes instructions until a halt instruction (`0x19`) is encountered. This loop involves several key functions:

- **fetchNextInstruction():** Increments the PC and loads the next instruction into the IR. It also determines the size of the instruction for correct PC incrementation, handling various instruction formats and addressing modes.
- **executeInstruction():** Decodes the instruction in the IR and performs the specified operation. This includes arithmetic operations (e.g., add, subtract, bitwise operations), memory operations (load and store), and branching (conditional and unconditional jumps). The results of these operations may alter the contents of registers and memory.

### 3. Memory Output
After the halt instruction is executed, the simulation ends, and the `writeMemoryToFile()` function writes the final state of the simulated memory to `mem_out.txt`. This output file provides a hexadecimal representation of memory, which can be analyzed to verify the correctness and effects of the executed instructions.

## Detailed Implementation
The implementation details include handling various types of instructions, each with its own operational logic. For instance:

- **Arithmetic instructions** manipulate data in the accumulator or memory, supporting operations like addition, subtraction, and bitwise logic.
- **Memory instructions** involve loading data into registers or storing register contents back into memory.
- **Branching instructions** change the flow of execution based on conditions such as the zero flag or accumulator value.

### Example Instruction Handling
- An ADD operation might read a value from a specified memory location, add it to the accumulator, and store the result back in the accumulator.
- A STORE operation takes the value in the accumulator and writes it to the specified memory address.
- A BRANCH operation checks conditions and, if met, changes the PC to the specified memory address, altering the execution flow.

## Conclusion
This Instruction Decoder Simulator offers a practical exploration into the decoding and execution processes of a computer's CPU. By simulating instruction processing and memory management, users gain a deeper understanding of fundamental computer architecture concepts. The project's simplicity makes it an excellent educational tool, while also providing a foundation for further exploration and enhancement in more complex simulation environments.
