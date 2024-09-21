/**
 * \file            qbyte.h
 * \brief           Quarzum Bytecode include file.
 * \copyright       2024 Marcos González GPL3
 */
#ifndef QBYTE_H
#define QBYTE_H
#include "../core/vector.h"
#include "../core/error.h"

/**
 * \brief           Defines operation codes for each instruction.
 */
typedef enum {
    OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD,     // Arithmetics
    OP_LOAD, OP_STORE,                          // Memory load and store
    OP_PUSH, OP_POP,                            // Stack
    OP_CALL, OP_RET,                            // Functions
    OP_AND, OP_OR, OP_XOR, OP_NOT,              // Logical operations
    OP_CMP, OP_JMP, OP_JE, OP_JNE, OP_JGE, 
    OP_JG, OP_JLE, OP_JL, OP_JZ, OP_JNZ         // Flux control
} opcode_t;

/**
 * \brief           Defines a value in the Intermediate Representation.
 * A value can be a constant, a variable name, a register or a function
 * argument. 
 */
typedef struct {
    enum {VALUE_CONSTANT, VALUE_STRING_CONSTANT, VALUE_VARIABLE, VALUE_REGISTER, VALUE_ARGUMENT} kind;
    union {
        uint8_t reg;
        uint8_t arg;
        char* var;
        char* str_const;
        int64_t int_const;
    } value;
} ir_value_t;

/**
 * \brief           Defines an instruction in the Intermediate Representation.
 * Each instruction has its own opcode and a maximum of 3 operands.
 */
typedef struct {
    opcode_t opcode;
    uint8_t operand_num;
    ir_value_t operands[3];
} ir_instruction_t;

/**
 * \brief           Defines a function segment in the Intermediate Representation.
 * Each function has its own arguments and internal instructions.
 */
typedef struct {
    char name[64];
    uint32_t align;
    uint8_t arguments;
    vector_t* instructions;
} ir_function_t;

typedef enum {
    INT = 1,
    CHAR,
    STRING,
    POINTER,
    NUMBER
} data_type_t;

/**
 * \brief           Defines a variable on the data segment in the Intermediate Representation.
 * Each variable has its own type and value, `null` in case of uninitialized variables.
 */
typedef struct {
    char name[64];
    data_type_t type;
    uint32_t size;
    char* value;
} ir_data_t;

/**
 * \brief           Defines the Intermediate Representation data structure.
 * This structure can be serialized and deserialized in case the file needs to be
 * precompiled or if `foreign` functions, deserialize them.
 */
typedef struct {
    vector_t *data, *functions;
} ir_code_t;

/**
 * \brief           Serializes an `ir_code_t` into a .qbyte file.
 */ 
void serialize(ir_code_t* ir, char* file);

/**
 * \brief           Deserializes a .qbyte file and adds its content 
 *                  into an existing `ir_code_t`.
 */
void deserialize(char* file, ir_code_t* target);

#endif /* QBYTE_H */