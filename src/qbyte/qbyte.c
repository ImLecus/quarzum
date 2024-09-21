/**
 * \file            qbyte.c
 * \brief           QByte serialization and deserialization.
 * \copyright       2024 Marcos González GPL3
 */
#include "qbyte.h"

static void serialize_data(ir_data_t* data, FILE* f){
    fwrite(data->name, strlen(data->name), 1, f);
    fputc('\0',f);
    fwrite(&data->size, sizeof(uint32_t), 1, f);
    fputc(data->type, f);
    fwrite(data->value, strlen(data->value), 1, f);
    fputc('\0',f);
}

static void serialize_instruction(ir_instruction_t* instruction, FILE* f){
    fwrite(&instruction->opcode, sizeof(uint8_t), 1 ,f);
    fwrite(&instruction->operand_num, sizeof(uint8_t), 1, f);
    for(uint8_t i = 0; i < instruction->operand_num; ++i){
        fwrite(&instruction->operands[i].kind, sizeof(uint8_t), 1, f);
        
        switch (instruction->operands[i].kind)
        {
        case VALUE_ARGUMENT:
            fwrite(&instruction->operands[i].value.arg, sizeof(uint8_t), 1, f);
            break;
        case VALUE_CONSTANT:
            fwrite(&instruction->operands[i].value.int_const, sizeof(int64_t), 1, f);
            break;
        case VALUE_STRING_CONSTANT:
            fwrite(instruction->operands[i].value.str_const, strlen(instruction->operands[i].value.str_const), 1, f);
            fputc('\0',f);
            break;
        case VALUE_REGISTER:
            fwrite(&instruction->operands[i].value.reg, sizeof(uint8_t), 1, f);
            break;
        case VALUE_VARIABLE:
            fwrite(instruction->operands[i].value.var, strlen(instruction->operands[i].value.str_const), 1, f);
            fputc('\0',f);
            break;
        default:
            break;
        }
    }
}

static void serialize_function(ir_function_t* func, FILE* f){
    fwrite(func->name, strlen(func->name), 1, f);
    fputc('\0',f);
    fwrite(&func->align, sizeof(uint32_t), 1, f);
    fwrite(&func->arguments, sizeof(uint8_t), 1, f);
    for(uint32_t i = 0; i < func->instructions->len; ++i){
        serialize_instruction(func->instructions->value[i], f);
    }
}

void serialize(ir_code_t* ir, char* file){
    FILE* f = fopen(file, "wb");
    if(f == NULL) return; // error

    for(size_t i = 0; i < ir->data->len; ++i){
        serialize_data(ir->data->value[i],f);
    }
    fwrite("//", 2, 1, f); // Section separator
    for(size_t i = 0; i < ir->functions->len; ++i){
        serialize_function(ir->functions->value[i], f);
    }
    fclose(f);
}

static ir_data_t* deserialize_data(FILE* f){
    ir_data_t* data = malloc(sizeof(ir_data_t));
    fread(data->name, sizeof(data->name),1,f);

    return data;
}

void deserialize(char* file, ir_code_t* target){
    FILE* f = fopen(file, "rb");
    if(f == NULL){ file_not_found_err(file); return;}

    vector_push(target->data, deserialize_data(f)); 

    fclose(f);
}