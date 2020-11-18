#include <sys/types.h> // for u_int...



#pragma pack(1) 

typedef char u1;
typedef u_int16_t u2;
typedef u_int32_t u4;

#define CONSTANT_Class 	7
#define CONSTANT_Fieldref 	9
#define CONSTANT_Methodref 	10
#define CONSTANT_InterfaceMethodref 	11
#define CONSTANT_String 	8
#define CONSTANT_Integer 	3
#define CONSTANT_Float 	4
#define CONSTANT_Long 	5
#define CONSTANT_Double 	6
#define CONSTANT_NameAndType 	12
#define CONSTANT_Utf8 	1
#define CONSTANT_MethodHandle 	15
#define CONSTANT_MethodType 	16
#define CONSTANT_InvokeDynamic 	18


// typedef struct {
//     u1 tag;
//     u1 *info;
// } cp_info;


typedef struct {
    u1 tag;
    union
    {
        struct 
        {
            u2 class_index;
            u2 name_and_type_index;
        } CONSTANT_Fieldref_info;

        struct  {
            u2 class_index;
            u2 name_and_type_index;
        } CONSTANT_Methodref_info;
        
        struct  {
            u2 name_index;
        } CONSTANT_Class_info;
        
        struct {
            u2 length;
            //u1 bytes[length];
            u1 * bytes;
        } CONSTANT_Utf8_info;

        struct  {
            u2 name_index;
            u2 descriptor_index;
        } CONSTANT_NameAndType_info;     

    };
    

} cp_info;

// typedef struct  {
//     u1 tag;
//     u2 name_index;
// } CONSTANT_Class_info;

// typedef struct  {
//     u1 tag;
//     u2 class_index;
//     u2 name_and_type_index;
// } CONSTANT_Fieldref_info;

// typedef struct  {
//     u1 tag;
//     u2 class_index;
//     u2 name_and_type_index;
// } CONSTANT_Methodref_info;

// typedef struct  {
//     u1 tag;
//     u2 class_index;
//     u2 name_and_type_index;
// }CONSTANT_InterfaceMethodref_info;

// typedef struct  {
//     u1 tag;
//     u2 string_index;
// } CONSTANT_String_info;

// typedef struct  {
//     u1 tag;
//     u4 bytes;
// } CONSTANT_Integer_info;

// typedef struct  {
//     u1 tag;
//     u4 bytes;
// }CONSTANT_Float_info;

// typedef struct  {
//     u1 tag;
//     u4 high_bytes;
//     u4 low_bytes;
// }CONSTANT_Long_info;

// typedef struct  {
//     u1 tag;
//     u4 high_bytes;
//     u4 low_bytes;
// }CONSTANT_Double_info;

// typedef struct  {
//     u1 tag;
//     u2 name_index;
//     u2 descriptor_index;
// }CONSTANT_NameAndType_info;

// typedef struct {
//     u1 tag;
//     u2 length;
//     //u1 bytes[length];
//     u1 * bytes;
// } CONSTANT_Utf8_info;

// typedef struct  {
//     u1 tag;
//     u1 reference_kind;
//     u2 reference_index;
// }CONSTANT_MethodHandle_info;

// typedef struct  {
//     u1 tag;
//     u2 descriptor_index;
// }CONSTANT_MethodType_info;

typedef struct {
    u2 max_stack;
    u2 max_locals;
    u4 code_length;
    //u1 code[code_length];
    u1 *code;
} code_info;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    //u1 info[attribute_length];
    u1 *info;
    code_info * codeinfo;
} attribute_info;

typedef struct {
    u2             access_flags;
    u2             name_index;
    u2             descriptor_index;
    u2             attributes_count;
    //attribute_info attributes[attributes_count];
    attribute_info *attributes;
    
}field_info;

typedef struct {
    u2             access_flags;
    u2             name_index;
    u2             descriptor_index;
    u2             attributes_count;
    //attribute_info attributes[attributes_count];
    attribute_info *attributes;

} method_info;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 max_stack;
    u2 max_locals;
    u4 code_length;
    //u1 code[code_length];
    u1 *code;
    //u2 exception_table_length;
    // {   u2 start_pc;
    //     u2 end_pc;
    //     u2 handler_pc;
    //     u2 catch_type;
    // } exception_table[exception_table_length];
    // u2 attributes_count;
    // attribute_info attributes[attributes_count];
} code_attribute;



typedef struct {
    u4             magic;
    u2             minor_version;
    u2             major_version;
    u2             constant_pool_count;
    //cp_info        constant_pool[constant_pool_count-1];
    cp_info *      constant_pool;
    u2             access_flags;
    u2             this_class;
    u2             super_class;
    u2             interfaces_count;
    // //u2             interfaces[interfaces_count];
    u2             *interfaces;
    u2             fields_count;
    // //field_info     fields[fields_count];
    field_info     *fields;
    u2             methods_count;
    // //method_info    methods[methods_count];
    method_info    *methods;
    u2             attributes_count;
    // //attribute_info attributes[attributes_count];
    attribute_info *attributes;
} jclass;