#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>

#include "classfile.h"

void print_class(jclass *c){
    printf("--------\n\n");
    printf("Magic                  %4x\n", c->magic);
    printf("Minor Version:         %d\n", c->minor_version);
    printf("Major Version:         %d\n", c->major_version);
    printf("Constant Pool Count:   %d\n", c->constant_pool_count);

    for (int i = 0; i < c->constant_pool_count - 1; i++){
        switch (c->constant_pool[i].tag)
        {
        case  CONSTANT_Class:
            printf("CONSTANT_Class\n");
            break;
        case  CONSTANT_Methodref:
            printf("CONSTANT_Methodref\n");
            break;
        case  CONSTANT_NameAndType:
            printf("CONSTANT_NameAndType\n");
            break;
        case  CONSTANT_Utf8:
            printf("CONSTANT_Utf8");
            printf("  %s\n", c->constant_pool[i].CONSTANT_Utf8_info.bytes);
            break;                                
        default:
            printf("Someting other\n");
            break;
        }
    }

    printf("Access Flags:       %d\n", c->access_flags);
    printf("This Class:         %d\n", c->this_class);
    printf("Super Class:        %d\n", c->super_class);
    printf("Interfaces Count:   %d\n", c->interfaces_count);

    for (int i = 0; i < c->interfaces_count; i++){
        printf("Interface Ref %d", c->interfaces[i]);
    }

    printf("Fields Count:   %d\n", c->fields_count);

    for (int i = 0; i < c->fields_count; i++){
        printf("Field Name Index %d", c->fields[i].name_index);
    }

    printf("Methods Count:   %d\n", c->methods_count);

    for (int i = 0; i < c->methods_count; i++){
        printf("---\n");
        printf("Method Acc Flags  %d\n", c->methods[i].access_flags);
        printf("Method Name Index %d : %s\n", c->methods[i].name_index, c->constant_pool[c->methods[i].name_index - 1].CONSTANT_Utf8_info.bytes);
        printf("Method Des Index %d\n", c->methods[i].descriptor_index);
        printf("Method At. Count  %d\n", c->methods[i].attributes_count);

        for (int j = 0; j < c->methods[i].attributes_count; j++){
            printf("    Attribute: %s\n", c->constant_pool[c->methods[i].attributes[j].attribute_name_index -1].CONSTANT_Utf8_info.bytes);
            
            // if attribute == code attribute
            if(strncmp(c->constant_pool[c->methods[i].attributes[j].attribute_name_index - 1].CONSTANT_Utf8_info.bytes,"Code",4) == 0){
                int codelength = c->methods[i].attributes[j].codeinfo->code_length;
                printf("    Code Length %d\n", codelength);
                printf("    Max Stack %d\n", c->methods[i].attributes[j].codeinfo->max_stack);
                printf("    Max Locals %d\n", c->methods[i].attributes[j].codeinfo->max_locals);
                


            }
        }

        printf("\n");
    }
}

// void exec_code(u1 * code, int len){
//     for(int i = 0; i < len; i++){
//         if( *code == 0x1a){
//             printf("1a");
//         }
//         if ( *code == 0x1b){
//             printf("1b");
//         }
//         if ( *code == 96){
//             printf("96");
//         }
//         code++;
//     }
// }


int main (int argc, char **argv){
    printf("--- JVM --- \n");
    char * mem;
    struct stat st;

    // open file 
    int class_file = open(argv[1], O_RDONLY);
    fstat(class_file, &st);

    mem = mmap(NULL, st.st_size, PROT_READ+PROT_WRITE, MAP_PRIVATE, class_file, 0);

    // sizeof(jclass) = 64;
    jclass *clazz = (jclass *)malloc(sizeof(jclass));

    clazz->magic = htonl(*(u4 *)mem);
    mem += 4;
    clazz->minor_version = htons(*(u2 *)mem);
    mem += 2;
    clazz->major_version = htons(*(u2 *)mem);
    mem += 2;
    clazz->constant_pool_count = htons(*(u2 *)mem);
    mem +=2;

    // parse constant pool
    cp_info *constant_pool = (cp_info *)malloc(sizeof(cp_info) * (clazz->constant_pool_count - 1));
    for(int i = 0; i < clazz->constant_pool_count -1; i++){
        constant_pool[i].tag = *(u1 *)mem;
        switch (constant_pool[i].tag)
        {
        case CONSTANT_Class :
            printf("Classref\n");
            mem++;
            constant_pool[i].CONSTANT_Class_info.name_index = htons(*(short *)mem);
            //mem += sizeof(CONSTANT_Class_info) + 4;
            mem += 2;
            break;
        case CONSTANT_Fieldref :
        break;
        case CONSTANT_Methodref:
            printf("Methodref\n");
            mem++;
            constant_pool[i].CONSTANT_Methodref_info.class_index = htons(*(short *)mem);
            mem +=2;
            constant_pool[i].CONSTANT_Methodref_info.name_and_type_index = htons(*(short *)mem);
            mem +=2;
            break;
        case CONSTANT_InterfaceMethodref :
        break;
        case CONSTANT_String :
        break;
        case CONSTANT_Integer:
        break;
        case CONSTANT_Float :
        break;
        case CONSTANT_Long :
        break;
        case CONSTANT_Double :
        break;
        case CONSTANT_NameAndType :
            printf("NameAndType\n");
            mem++;
            constant_pool[i].CONSTANT_NameAndType_info.name_index = htons(*(short *)mem);
            mem +=2;
            constant_pool[i].CONSTANT_NameAndType_info.descriptor_index = htons(*(short *)mem);
            mem +=2;
            break;
        case CONSTANT_Utf8:
            printf("Utf8:  ");
            mem++;
            constant_pool[i].CONSTANT_Utf8_info.length = htons(*(short *)mem);
            mem +=2;
            //constant_pool[i].CONSTANT_Utf8_info.bytes = (char *)mem;
            constant_pool[i].CONSTANT_Utf8_info.bytes = (char *)malloc(constant_pool[i].CONSTANT_Utf8_info.length);
            memcpy(constant_pool[i].CONSTANT_Utf8_info.bytes, (char *)mem, constant_pool[i].CONSTANT_Utf8_info.length);
            printf("%s\n", constant_pool[i].CONSTANT_Utf8_info.bytes);
            mem += constant_pool[i].CONSTANT_Utf8_info.length;
            break;
        case CONSTANT_MethodHandle:
        break;
        case CONSTANT_MethodType:
        break;
        case CONSTANT_InvokeDynamic:
        break;
        default:
            break;
        }
    }
    clazz->constant_pool = constant_pool;

    clazz->access_flags = htons(*(u2 *)mem);
    mem +=2;

    clazz->this_class = htons(*(u2 *)mem);
    mem +=2;

    clazz->super_class = htons(*(u2 *)mem);
    mem +=2;

    clazz->interfaces_count = htons(*(u2 *)mem);
    mem +=2;

    // parse interfaces array
    u2 *interfaces = (u2 *)malloc(sizeof(u2) * clazz->interfaces_count);
    for (int i = 0; i < clazz->interfaces_count; i++){
        interfaces[i] = htons(*(u2 *)mem);
        mem +=2;
    }
    clazz->interfaces = interfaces;

    clazz->fields_count = htons(*(u2 *)mem);
    mem +=2;

    // parse fields array
    field_info *fields = (field_info *)malloc(sizeof(field_info) * clazz->fields_count);
    for (int i = 0; i < clazz->fields_count; i++){
        fields[i].access_flags = htons(*(u2 *)mem);
        mem +=2;
        fields[i].name_index = htons(*(u2 *)mem);
        mem +=2;
        fields[i].descriptor_index = htons(*(u2 *)mem);
        mem +=2;
        fields[i].attributes_count = htons(*(u2 *)mem);
        mem +=2;
        attribute_info *attributes_fields = (attribute_info *)malloc(sizeof(attribute_info) * fields[i].attributes_count);
        for (int j = 0; j < fields[i].attributes_count; j++){
            attributes_fields[j].attribute_name_index = htons(*(u2 *)mem);
            mem +=2;
            attributes_fields[j].attribute_length = htonl(*(u4 *)mem);
            mem +=4;
            attributes_fields[j].info = (char *)mem;
            mem += attributes_fields[j].attribute_length;
        }
        fields[i].attributes = attributes_fields;        
    }
    clazz->fields = fields;

    clazz->methods_count = htons(*(u2 *)mem);
    mem +=2;

    // parse methods array
    method_info *methods = (method_info*)malloc (sizeof(method_info) * clazz->methods_count);
    for (int i = 0; i < clazz->methods_count; i++){
        methods[i].access_flags = htons(*(u2 *)mem);
        mem +=2;
        methods[i].name_index = htons(*(u2 *)mem);
        mem +=2;
        methods[i].descriptor_index = htons(*(u2 *)mem);
        mem +=2;
        methods[i].attributes_count = htons(*(u2 *)mem);
        mem +=2;
        attribute_info *attributes_methods = (attribute_info *)malloc(sizeof(attribute_info) * methods[i].attributes_count);
        for (int j = 0; j < methods[i].attributes_count; j++){
            attributes_methods[j].attribute_name_index = htons(*(u2 *)mem);
            mem +=2;
            attributes_methods[j].attribute_length = htonl(*(u4 *)mem);
            mem +=4;
            attributes_methods[j].info = (u1 *)malloc(attributes_methods[j].attribute_length);
            memcpy(attributes_methods[j].info, (char *)mem, attributes_methods[j].attribute_length);
            if (strncmp(clazz->constant_pool[attributes_methods[j].attribute_name_index - 1].CONSTANT_Utf8_info.bytes,"Code",4) == 0 ){
                code_info * codeinfo = (code_info *)malloc(sizeof(code_info));
                codeinfo->max_stack = htons(*(u2 *)mem);
                mem +=2;
                codeinfo->max_locals = htons(*(u2 *)mem);
                mem +=2;
                codeinfo->code_length = htonl(*(u4 *)mem);
                mem +=4;
                codeinfo->code = (u1 *)malloc(sizeof(u1) * codeinfo->code_length);
                memcpy(codeinfo->code, (char *)mem, codeinfo->code_length);

                attributes_methods[j].codeinfo = codeinfo;
                mem += codeinfo->code_length;
                mem += attributes_methods[j].attribute_length - codeinfo->code_length - 8;
            } 
            else
            {
                mem += attributes_methods[j].attribute_length;
            }
            


        }
        methods[i].attributes = attributes_methods;
    }
    clazz->methods = methods;

    clazz->attributes_count = htons(*(u2 *)mem);
    mem +=2;

    // parse attributes array
    attribute_info * attributes = (attribute_info *)malloc(sizeof(attribute_info) * clazz->attributes_count);
    for (int i = 0; i < clazz->attributes_count; i++){
        attributes[i].attribute_name_index = htons(*(u2 *)mem);
        mem +=2;
        attributes[i].attribute_length = htonl(*(u4 *)mem);
        mem +=4;
        //attributes[i].info = (char *)mem;
        attributes[i].info = (char *) malloc(sizeof(char) * attributes[i].attribute_length);
        memcpy(attributes[i].info, (char *)mem, attributes[i].attribute_length);
        mem += attributes[i].attribute_length;        
    }
    clazz->attributes = attributes;
    

    print_class(clazz);


}