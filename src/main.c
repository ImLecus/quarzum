#include "quarzum.h"
#include "core/error.h"

int main(int argc, char** argv) {
    struct process task = start_process("Task");
    printf(DEBUG_MSG("Starting the compiler..."));

    throw_error(1, 1, "My custom error");
    if(error_count > 0){
        print_errors();
    }
    

    // Lexical & Syntax analisys
    struct process parsing = start_process("Parse phase");
    hashmap* type_map = init_type_map();
    parse_tree* ast = parse("code.qz");
    if(!ast || ast->has_errors){
        printf(ERROR_MSG("Process aborted with exit code -1"));
        free_parse_tree(ast);
        exit(-1);
    }
    end_process(&parsing);

    // Semantic analisys
    struct process checking = start_process("Check phase");
    bool has_errors = check_parse_tree(ast);
    if(has_errors){
        printf(ERROR_MSG("Process aborted with exit code -1"));
        free_parse_tree(ast);
        exit(-1);
    }
    vector* ir_list = generate_ir(ast->ast);
    end_process(&checking);


    // Code generation
    // struct process codegn = start_process("Code generation phase");
    // FILE* asm_file = fopen("./out.asm", "w");
    // asm_code* assembly = code_gen(ir_list);
    // write_file(asm_file, assembly->data_section->value);
    // write_file(asm_file, assembly->bss_section->value);
    // write_file(asm_file, assembly->text_section->value);
    // fclose(asm_file);
    // end_process(&codegn);

    // free memory
    free_vector(ir_list);
    free_parse_tree(ast);

    // free_string(assembly->bss_section);
    // free_string(assembly->data_section);
    // free_string(assembly->text_section);
    // free(assembly);
    end_process(&task);
    printf(GREEN BOLD"[SUCCESS]"RESET" Process finished with exit code 0\n");
    // system("read");
    // system("as out.asm -o out.o");
    // system("ld ./builtins/x86_64/__base__.o ./builtins/x86_64/console.o out.o -o code");
    // system("rm out.o");
    // system("rm out.asm");
    // //system("clear");
    // system("./code");
    // system("rm ./code");
    
    return 0;
}
