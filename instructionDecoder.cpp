#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

#define HALT_OPCODE 0x19

void fetchNextInstruction(void);
void executeInstruction(void);
void initializeMemory(void);
void writeMemoryToFile(void);

unsigned char memory[65536];

unsigned char ACC=0;
unsigned char IR=0;
unsigned int MAR=0;
unsigned int PC=0;

//additional variable used to store data temporarily
//The way its used, I think of it like the MBR, I think that's an appropriate name for it
unsigned int MBR=0;

int main(int argc, char *argv[])
{
    initializeMemory();
    //execution loop. Continue fetching and executing until PC points to a halt instruction.
    while(memory[PC] != HALT_OPCODE)
    {
        fetchNextInstruction();
        executeInstruction();
    }
    writeMemoryToFile();
    return 0;
}

void fetchNextInstruction(){
    //will perform the following steps
    //use the PC as an index to retrieve an instruction from the memory array
    //store the retrieved instruction to the IR 
    //determine from the instruction how much to increment the PC so that it points to the next instruction in memory and increment

    //also need to load the data from memory into registers when appropriate
    
    IR = memory[PC];
    int amountToIncrement = 0; //depends on both the source and the destination register
    amountToIncrement += 1;//Need to increment by at least one byte always
    
    MBR = 0;//clear the MBR
    MBR &= 0;//clear the MBR
    
    //amout to increment the PC also depends on what kind of instruction is in the IR
    
    //check for arithmetic instruction
    if((IR >> 7) == 1){
        //OP [MAR] CONST(16 bits)
        if((IR & 15) == 2)
        {
            MBR = (memory[PC+1]<<8) + memory[PC+2];
            amountToIncrement += 2;
        }
        //OP [MAR] [16 bit CONST]
        else if((IR & 15) == 3)
        {
            MBR = (memory[PC+1]<<8) + memory[PC+2];
            amountToIncrement += 2;
        }
        //OP ACC CONST(8 bits)
        else if((IR & 15) == 6)
        {
            MBR = memory[PC+1];
            amountToIncrement += 1;
        }
        //OP ACC [CONST 16 bits]
        else if((IR & 15) == 7)
        {
            MBR = (memory[PC+1]<<8) + memory[PC+2];
            amountToIncrement += 2;
        }
        //OP MAR CONST(16 bits)
        else if((IR & 15) == 10)
        {
            MBR = (memory[PC+1]<<8) + memory[PC+2];
            amountToIncrement += 2;
        }
        //OP MAR [CONST 16 bits]
        else if((IR & 15) == 11)
        {
            MBR = (memory[PC+1]<<8) + memory[PC+2];
            amountToIncrement += 2;
        }
        //OP [16 bit const] [MAR]
        else if((IR & 15) == 12)
        {
             MBR = (memory[PC+1]<<8) + memory[PC+2];
            amountToIncrement += 2;
        }
        //OP [16 bit const] ACC
        else if((IR & 15) == 13)
        {
            MBR = (memory[PC+1]<<8) + memory[PC+2];
            amountToIncrement += 2;
        }
    }
    //check for memory operations
    else if((IR >> 4) == 0)
    {
        //increment by the size of an address (16bits)
        if((IR & 7) == 0)
        {
            MBR = (memory[PC+1] << 8) + memory[PC+2];
            amountToIncrement += 2;
        }
        //if the register is MAR and the operand is used as a constant, increment by size of MAR (16 bits)
        else if((IR & 7) == 1)
        {
            MBR = memory[PC+1];
            amountToIncrement += 1;
        }
        else if((IR & 7) == 4)
        {
            MBR = (memory[PC+1] << 8) + memory[PC+2];
            amountToIncrement += 2;   
        }
        else if((IR & 7) == 5)
        {
            MBR = (memory[PC+1] << 8) + memory[PC+2];
            amountToIncrement += 2;
        }
    }
    //check for branch instructions
    else if((IR >> 3) == 2)
    {
        //The opcode for a branch instruction is always followed by a 16 bit operand that serves as the memory address
        MBR = (memory[PC+1]<<8) + memory[PC+2];
        amountToIncrement += 2;
    }
    //MBR &= 0xffff;//only use the lower 16 bits of MBR
    //lastly, increment the PC
    PC += amountToIncrement;
}

void executeInstruction(){
    
    //first check for math op
    if ((IR & 0x80) == 0x80)
    {
        //easiest solution is to check source and destination at the same time
        //This process needs to be done twice for math ops - once when loading the operands and again when storing them
        unsigned int source;
        unsigned int destination;

        //OP [MAR] [MAR]
        if((IR & 15) == 0)
        {
            destination = memory[MAR]&0xff;
            source = memory[MAR]&0xff;
        }
        //OP [MAR] ACC
        else if((IR & 15) == 1)
        {
            destination = memory[MAR];
            source = ACC;
        }
        //OP [MAR] CONST(16 bits)
        else if((IR & 15) == 2)
        {
            destination = memory[MAR];
            source = MBR&0xffff;
        }
        //OP [MAR] [16 bit CONST]
        else if((IR & 15) == 3)
        {
            destination = memory[MAR]&0xff;
            source = memory[MBR]&0xff;
        }
        //OP ACC [MAR]
        else if((IR & 15) == 4)
        {
            destination = ACC&0xff;
            source = memory[MAR]&0xff;
        }
        //OP ACC ACC
        else if((IR & 15) == 5)
        {
            destination = ACC&0xff;
            source = ACC&0xff;
        }
        //OP ACC CONST(8 bits)
        else if((IR & 15) == 6)
        {
            destination = ACC&0xff;
            source = MBR&0xffff;
        }
        //OP ACC [CONST 16 bits]
        else if((IR & 15) == 7)
        {
            destination = ACC&0xff;
            source = memory[MBR]&0xff;
        }
        //OP MAR [MAR]
        else if((IR & 15) == 8)
        {
            destination = MAR&0xffff;
            source = memory[MAR]&0xff;
        }
        //OP MAR ACC
        else if((IR & 15) == 9)
        {
            destination = MAR&0xffff;
            source = ACC&0xff;
        }
        //OP MAR CONST(16 bits)
        else if((IR & 15) == 10)
        {
            destination = MAR&0xffff;
            source = MBR&0xffff;
        }
        //OP MAR [CONST 16 bits]
        else if((IR & 15) == 11)
        {
            destination = MAR&0xffff;
            source = memory[MBR]&0xff;
        }
        //OP [16 bit const] [MAR]
        else if((IR & 15) == 12)
        {
            destination = memory[MBR]&0xff;
            source = memory[MAR]&0xff;
        }
        //OP [16 bit const] ACC
        else if((IR & 15) == 13)
        {
            destination = memory[MBR]&0xff;
            source = ACC&0xff;
        }
        //op [16bit const] CONST(8 bits)
        else if((IR & 15) == 14)
        {
            //This does not make sense - the opcode would be larger than 3 bytes anyway
            destination = memory[MBR];
            source = MBR;
        }
        //OP [16 bit const] [16 bit const]
        //not implemented as explained in assignment

        //Do the operation
        if((IR & 112) == 0)
        {
            //AND
            destination &= source;
        }
        else if((IR & 112) == 16)
        {
            //OR
            destination |= source;
        }
        else if((IR & 112) == 32)
        {
            //XOR TODO
            destination ^= source;
        }
        else if((IR & 112) == 48)
        {
            //ADD TODO
            destination += source;
        }
        else if((IR & 112) == 64)
        {
            //SUB
            destination -= source;
        }
        else if((IR & 112) == 80)
        {
            //INC TODO
            destination += 1;
        }
        else if((IR & 112) == 96)
        {
            //DEC
            destination -= 1;
        }
        else if((IR & 112) == 112)
        {
            //NOT
            destination = ~destination;
        }

        //store the result back in the destination
        //OP [MAR] [MAR]
        if((IR & 15) == 0)
        {
            memory[MAR] = destination&0xff;
        }
        //OP [MAR] ACC
        else if((IR & 15) == 1)
        {
            memory[MAR] = destination&0xff;
        }
        //OP [MAR] CONST(16 bits)
        else if((IR & 15) == 2)
        {
            memory[MAR] = destination&0xff;
        }
        //OP [MAR] [16 bit CONST]
        else if((IR & 15) == 3)
        {
            memory[MAR] = destination&0xff;
        }
        //OP ACC [MAR]
        else if((IR & 15) == 4)
        {
           ACC = destination&0xff;
        }
        //OP ACC ACC
        else if((IR & 15) == 5)
        {
            ACC = destination&0xff;
        }
        //OP ACC CONST(8 bits)
        else if((IR & 15) == 6)
        {
             ACC = destination&0xff;
        }
        //OP ACC [CONST 16 bits]
        else if((IR & 15) == 7)
        {
             ACC = destination&0xff;
        }
        //OP MAR [MAR]
        else if((IR & 15) == 8)
        {
            MAR = destination&0xffff;
        }
        //OP MAR ACC
        else if((IR & 15) == 9)
        {
            MAR = destination&0xffff;
        }
        //OP MAR CONST(16 bits)
        else if((IR & 15) == 10)
        {
            MAR = destination&0xffff;
        }
        //OP MAR [CONST 16 bits]
        else if((IR & 15) == 11)
        {
            MAR = destination&0xffff;
        }
        //OP [16 bit const] [MAR]
        else if((IR & 15) == 12)
        {
            memory[MBR] = destination&0xff;
        }
        //OP [16 bit const] ACC
        else if((IR & 15) == 13)
        {
           memory[MBR] = destination&0xff;
        }
        //op [16bit const] CONST(8 bits)
        else if((IR & 15) == 14)
        {
            //This does not make sense - the opcode would be larger than 3 bytes anyway
            destination = memory[MBR];
            source = MBR;
        }
    }
    //it is a memory OP
    else if((IR >> 4)==0)
    {
        //simplest solution is to check every possible combination
        
        //may need to check when storing a 16 bit register back into memory - most likely causing issues
        //STOR ACC [OPERAND]
        if((IR & 15) == 0)
        {
            memory[MBR] = ACC;
        }
        //STOR ACC [MAR]
        else if((IR & 15) == 2)
        {
            memory[MAR] = ACC;
        }
        //STOR MAR [OPERAND]
        //THIS IS IMPORTANT - 16BIT TO 8 BIT STORAGE NEEDS TO BE DONE LIKE THIS   
        else if((IR & 15) == 4)
        {
            memory[MBR] = MAR>>8;
            memory[MBR+1] = MAR&0xff;
            //memory[MBR] = MAR&0xffff;
        }
        //ABOVE IS EXTREMELY IMPORTANT IT IS THE ONLY THING THAT MATTERS
        //STOR MAR [MAR]
        else if((IR & 15) == 6)
        {
            //memory[MAR] = MAR&0xffff;
            memory[MAR] = MAR>>8;
            memory[MAR+1] = MAR&0xff;
        }
        //LOAD ACC [OPERAND]
        else if((IR & 15) == 8)
        {
            ACC = memory[MBR];
        }
        //LOAD ACC OPERAND
        else if((IR & 15) == 9)
        {
            ACC = MBR&0xff;
        }
        //LOAD ACC [MAR]
        else if((IR & 15) == 10)
        {
            ACC = memory[MAR];
        }
        //LOAD MAR [OPERAND]
        else if((IR & 15) == 12)
        {
            MAR = (memory[MBR] << 8) + memory[MBR+1];
        }
        //LOAD MAR OPERAND
        else if((IR & 15) == 13)
        {
            MAR = MBR&0xffff;
        }
        //LOAD MAR [MAR]
        else if((IR & 15) == 14)
        {
            MAR = (memory[MAR]<<8) + memory[MAR+1];
        }
    }
    //branching instruction
    else if((IR >> 3) == 2)
    {
        //simplest solution is to check all combinations.
        //A 16 bit memory address always follows a branch instruction
        //PC needs to change to be in the same position as the new instruction
        //BRA unconditional
        if((IR & 7) == 0)
        {
            //IR = memory[MBR];
            PC = MBR;
        }
        //BRZ
        else if((IR & 7) == 1)
        {
            if(ACC == 0)
            {
                //IR = memory[MBR];
                PC = MBR;
            }
        }
        //BNE ACC != 0
        else if((IR & 7) == 2)
        {
            if(ACC != 0){
                //IR = memory[MBR];
                PC = MBR;
            }
        }
        //B ACC < 0
        else if((IR & 7) == 3)
        {
            if(ACC < 0)
            {
                //IR = memory[MBR];
                PC = MBR;
            }
        }
        //B ACC <= 0
        else if((IR & 7) == 4)
        {
            if(ACC <= 0)
            {
                //IR = memory[MBR];
                PC = MBR;
            }
        }
        //B ACC >0
        else if((IR & 7) == 5)
        {
            if(ACC > 0){
                //IR = memory[MBR];
                PC = MBR;
            }
        }
        //B ACC >= 0
        else if((IR & 7) == 6)
        {
            if(ACC >= 0){
                //IR = memory[MBR];
                PC = MBR;
            }
        }
    }
}

void initializeMemory()
{
    // https://stackoverflow.com/questions/11608878/c-convert-string-to-hex
    // https://cplusplus.com/doc/tutorial/files/
    // read the memory file and initialize the memory array
    ifstream memoryFile;

    memoryFile.open("mem_in.txt");
    char memblock[3];
    char garbage[1];
    int x = 0;
    int test;

    if (memoryFile.is_open())
    {

        while (!memoryFile.eof())
        {
            if (garbage[0] == '\r')
            {
                // the end of the line has been reached, need to read an additional \n into the garbage buffer
                memoryFile.read(garbage, 1);
            }
            memoryFile.read(memblock, 2);
            memoryFile.read(garbage, 1); // read in the "space"
            memblock[3] = '\0';
            // once the data has been read from the file, load it into memory
            memory[x] = stoi(memblock, 0, 16);

            x++;
        }
        memoryFile.close();
    }
}

void writeMemoryToFile(){
    //https://stackoverflow.com/questions/5100718/integer-to-hex-string-in-c

    //I altered the input file to be formatted consistently so this function may not work with a different input file

    ofstream outputFile;
    outputFile.open("mem_result.txt");
    
    if(outputFile.is_open())
    {
        int charCount = 0;
        for(unsigned int x : memory){
            
            if(charCount == 15){
                //write to file
                //write newline
                outputFile << setw(2) << setfill('0') << hex << x << " " << endl;
                charCount = 0;
            }
            else{
                //write to file
                outputFile << setw(2) << setfill('0') << hex << x << " ";
                charCount++;
            }
        }
        
        outputFile.close();
    }
}