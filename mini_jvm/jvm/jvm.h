
#ifndef PUP_JVM_H
#define PUP_JVM_H

#define HAVE_STRUCT_TIMESPEC
#define _POSIX_C_SOURCE 200809L


//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/d_type.h"
#include "../jvm/global.h"
#include "../utils/hashtable.h"
#include "../utils/utf8_string.h"
#include "../utils/arraylist.h"
#include "../utils/pairlist.h"

#ifdef __cplusplus
extern "C" {
#endif


//=======================  micro define  =============================
//_JVM_DEBUG   06 print all bytecode
#define _JVM_DEBUG 0
#define _JVM_DEBUG_PRINT_FILE 0
#define _JVM_DEBUG_BYTECODE_DUMP 0
#define _JVM_DEBUG_GARBAGE_DUMP 0
#define _JVM_DEBUG_PROFILE 0


// x86   x64 ...
#define __JVM_LITTLE_ENDIAN__ 1
// arm
#define __JVM_BIG_ENDIAN__ 0


#ifdef __MINGW_H
#define __JVM_OS_MINGW__ 1
#endif
#ifdef _CYGWIN_CONFIG_H
#define __JVM_OS_CYGWIN__ 1
#endif
#ifdef __DARWIN_C_ANSI
#define __JVM_OS_MAC__ 1
#endif
#ifdef __ANDROID
#define __JVM_OS_ANDROID__ 1
#define  __ANDROID__
#endif


//compile
#define __C99
//libary option : -lpthread -lws2_32
/*
 *  TAG
 *  1 UTF-8 String
 *  3 Integer
 *  4 Float
 *  5 Long
 *  6 Double
 *  7 Class reference
 *  8 String reference
 *  9 Field reference
 *  10 Method reference
 *  11 Interface methodRef garbage_refer
 *  12 Name and type descriptor
 * */
#define CONSTANT_UTF8           1
#define CONSTANT_INTEGER        3
#define CONSTANT_FLOAT          4
#define CONSTANT_LONG           5
#define CONSTANT_DOUBLE         6
#define CONSTANT_CLASS          7
#define CONSTANT_STRING_REF     8
#define CONSTANT_FIELD_REF      9
#define CONSTANT_METHOD_REF     10
#define CONSTANT_INTERFACE_REF  11
#define CONSTANT_NAME_AND_TYPE  12
//=======================  typedef  =============================

#if __JVM_LITTLE_ENDIAN__
typedef union _Short2Char {
    s16 s;
    struct {
        c8 c0;
        c8 c1;
    };
} Short2Char;

typedef union _Int2Float {
    s32 i;
    f32 f;
    struct {
        c8 c0;
        c8 c1;
        c8 c2;
        c8 c3;
    };
} Int2Float;

typedef union _Long2Double {
    f64 d;
    __refer r;
    union {
        s64 l;
        struct {
            s32 i0;
            s32 i1;
        } i2l;
    };
    struct {
        c8 c0;
        c8 c1;
        c8 c2;
        c8 c3;
        c8 c4;
        c8 c5;
        c8 c6;
        c8 c7;
    };
} Long2Double;
#elif __JVM_BIG_ENDIAN__
typedef union _Short2Char {
    s16 s;
    struct {
        c8 c1;
        c8 c0;
    };
} Short2Char;

typedef union _Int2Float {
    s32 i;
    f32 f;
    struct {
        c8 c3;
        c8 c2;
        c8 c1;
        c8 c0;
    };
} Int2Float;

typedef union _Long2Double {
    f64 d;
    __refer r;
    union {
        s64 l;
        struct {
            s32 i1;
            s32 i0;
        } i2l;
    };
    struct {
        c8 c7;
        c8 c6;
        c8 c5;
        c8 c4;
        c8 c3;
        c8 c2;
        c8 c1;
        c8 c0;
    };
} Long2Double;
#endif

typedef struct _ClassLoader ClassLoader;
typedef struct _ClassType Class;
typedef struct _InstanceType Instance;
typedef struct _FieldInfo FieldInfo;
typedef struct _MethodInfo MethodInfo;
typedef struct _Instruction Instruction;
typedef struct _ConstantNameAndType ConstantNameAndType;
typedef struct _ThreadLock ThreadLock;
typedef struct _JavaThreadInfo JavaThreadInfo;
typedef struct _Runtime Runtime;
typedef struct _CodeAttribute CodeAttribute;

typedef s32 (*java_native_fun)(Runtime *runtime, Class *p);

typedef struct _Collector Collector;

enum {
    JVM_ERROR_OUTOFMEMORY,
    JVM_ERROR_VIRTUALMACHINE,
    JVM_ERROR_NOCLASSDEFFOUND,
    JVM_EXCEPTION_EOF,
    JVM_EXCEPTION_FILENOTFOUND,
    JVM_EXCEPTION_ARRITHMETIC,
    JVM_EXCEPTION_CLASSNOTFOUND,
    JVM_EXCEPTION_NULLPOINTER,
    JVM_EXCEPTION_ILLEGALARGUMENT,
    JVM_EXCEPTION_CLASSCASTEXCEPTION,
    JVM_EXCEPTION_INSTANTIATIONEXCEPTION,
};

static char *exception_class_name[] = {
        "java.io.OutOfMemoryError",
        "java.io.VirtualMachineError",
        "java.io.NoClassDefFoundError",
        "java.io.EOFException",
        "java.lang.FileNotFoundException",
        "java.lang.ArithmeticException",
        "java.lang.ClassNotFoundException",
        "java.lang.NullPointerException",
        "java.lang.IllegalArgumentException",
        "java.lang.ClassCastException",
        "java.lang.ArrayIndexOutOfBoundsException",
        "java.lang.InstantiationException",
};

static c8 *STR_CLASS_JAVA_LANG_STRING = "java/lang/String";
static c8 *STR_CLASS_JAVA_LANG_OBJECT = "java/lang/Object";
static c8 *STR_CLASS_JAVA_LANG_THREAD = "java/lang/Thread";
static c8 *STR_CLASS_JAVA_LANG_CLASS = "java/lang/Class";
static c8 *STR_FIELD_THREADQ = "threadQ";
static c8 *STR_FIELD_NAME = "name";
static c8 *STR_FIELD_VALUE = "value";
static c8 *STR_FIELD_COUNT = "count";
static c8 *STR_FIELD_OFFSET = "offset";

static c8 *STR_INS_JAVA_LANG_STRING = "Ljava/lang/String;";
static c8 *STR_INS_JAVA_LANG_THREAD = "Ljava/lang/Thread;";
static c8 *STR_INS_JAVA_LANG_CLASS = "Ljava/lang/Class;";
static c8 *STR_INS_JAVA_LANG_OBJECT = "Ljava/lang/Object;";

enum {
    METHOD_INVOKE_DYNAMIC,
    METHOD_INVOKE_INTERFACE,
    METHOD_INVOKE_STATIC,
    METHOD_INVOKE_VIRTUAL,
    METHOD_INVOKE_SPECIAL
};
/**
 * 内存中几个主要对象的类型，他们是不同的数据结构，但是每种类型的第一个字节都是用来标识此内存对象的类型
 */
enum {
    MEM_TYPE_NODEF, //0
    MEM_TYPE_CLASS, //1
    MEM_TYPE_INS,   //2
    MEM_TYPE_ARR    //3
};


/*
boolean   4
char  5
float  6
double 7
unsigned char 8
short   9
int  10
long  11
  reference 12
 */
extern c8 *data_type_str;
static const s32 data_type_bytes[14] = {0, 0, 0, 0,
                                        sizeof(c8),
                                        sizeof(u16),
                                        sizeof(f32),
                                        sizeof(f64),
                                        sizeof(c8),
                                        sizeof(s16),
                                        sizeof(s32),
                                        sizeof(s64),
                                        sizeof(__refer),
                                        sizeof(__refer),
};
static const s32 DATATYPE_JCHAR = 5;
static const s32 DATATYPE_REFERENCE = 12;
static const s32 DATATYPE_ARRAY = 13;
static const s32 DATATYPE_SHORT = 9;
static const s32 DATATYPE_LONG = 11;

static const c8 tag_additional_byte_size[13] = {
        0, 2, 0, 4, 4,
        8, 8, 2, 2, 4,
        4, 4, 4
};

//访问标志
static const u16 ACC_PUBLIC = 0x0001;
static const u16 ACC_PRIVATE = 0x0002;
static const u16 ACC_PROTECTED = 0x0004;
static const u16 ACC_STATIC = 0x0008;
static const u16 ACC_FINAL = 0x0010;
static const u16 ACC_SYNCHRONIZED = 0x0020;
static const u16 ACC_VOLATILE = 0x0040;
static const u16 ACC_TRANSIENT = 0x0080;
static const u16 ACC_NATIVE = 0x0100;
static const u16 ACC_INTERFACE = 0x0200;
static const u16 ACC_ABSTRACT = 0x0400;
static const u16 ACC_STRICT = 0x0800;

//类状态
enum {
    CLASS_STATUS_RAW,
    CLASS_STATUS_LOADED,
    CLASS_STATUS_PREPARING,
    CLASS_STATUS_PREPARED,
    CLASS_STATUS_CLINITING,
    CLASS_STATUS_CLINITED,
};
//线程
enum {
    THREAD_STATUS_ZOMBIE,
    THREAD_STATUS_RUNNING,
    THREAD_STATUS_SLEEPING,
    THREAD_STATUS_MONITOR,
    THREAD_STATUS_WAIT,
};

//指令指行返回状态
static const c8 RUNTIME_STATUS_NORMAL = 0;
static const c8 RUNTIME_STATUS_RETURN = -1;
static const c8 RUNTIME_STATUS_EXCEPTION = -2;
static const c8 RUNTIME_STATUS_ERROR = -3;

//指令if类型
enum {
    TYPE_IF_ACMPEQ,
    TYPE_IF_ACMPNE,
    TYPE_IF_ICMPEQ,
    TYPE_IF_ICMPGE,
    TYPE_IF_ICMPGT,
    TYPE_IF_ICMPLE,
    TYPE_IF_ICMPLT,
    TYPE_IF_ICMPNE,
    TYPE_IFEQ,
    TYPE_IFGE,
    TYPE_IFGT,
    TYPE_IFLE,
    TYPE_IFLT,
    TYPE_IFNE,
    TYPE_IFNONNULL,
    TYPE_IFNULL
};
enum {
    GARBAGE_MARK_UNDEF,
    GARBAGE_MARK_NO_REFERED,
    GARBAGE_MARK_REFERED,
};


//======================= global var =============================
extern Instance *main_thread;
extern Runtime *main_runtime;
extern ThreadLock sys_lock;

extern ClassLoader *sys_classloader;
extern ClassLoader *array_classloader;

extern Instruction **instructionsIndexies;

extern ArrayList *thread_list;
extern ArrayList *native_libs;
extern Hashtable *sys_prop;

extern u8 volatile java_debug;


extern s32 STACK_LENGHT;

#if _JVM_DEBUG_PROFILE
extern Hashtable *instruct_profile;
#endif

//======================= MEM_OBJ =============================

typedef struct _MemoryBlock {
    u8 type;//type of array or object runtime,class
    u8 garbage_mark;
    u8 arr_type_index;
    Class *clazz;
    ThreadLock *volatile thread_lock;
} MemoryBlock;

struct _ClassLoader {
    Utf8String *g_classpath;
    Hashtable *classes;
    Class *JVM_CLASS;
};
//======================= class file =============================


/* Java Class File */
typedef struct _ClassFileFormat {
    u8 magic_number[4];
    u16 minor_version;
    u16 major_version;
    u16 constant_pool_count;
    /* constant pool */
    u16 access_flags;
    u16 this_class;
    u16 super_class;

    u16 interface_count;
    /* interfaceRef pool */
    u16 fields_count;
    /* obj_fields pool */
    u16 methods_count;
    /* methodRef pool */
    u16 attributes_count;
    /* attributes pool */
} ClassFileFormat;

typedef struct _ConstantType {
    s32 index;
    u8 tag;
} ConstantItem;

typedef struct _ConstantUTF8 {
    s32 index;
    u8 tag;
    s32 additional_byte_size;
    u16 string_size;
    //
    Utf8String *utfstr;
} ConstantUTF8;

typedef struct _ConstantInteger {
    s32 index;
    u8 tag;
    s32 additional_byte_size;
    s32 value;
} ConstantInteger;

typedef struct _ConstantFloat {
    s32 index;
    u8 tag;
    s32 additional_byte_size;
    f32 value;

} ConstantFloat;

typedef struct _ConstantLong {
    s32 index;
    u8 tag;
    s32 additional_byte_size;
    s64 value;
} ConstantLong;

typedef struct _ConstantDouble {
    s32 index;
    u8 tag;
    s32 additional_byte_size;
    f64 value;
} ConstantDouble;

typedef struct _ConstantClassRef {
    s32 index;
    u8 tag;
    s32 additional_byte_size;
    u16 stringIndex;

    //
    Utf8String *name;
} ConstantClassRef;

typedef struct _ConstantStringRef {
    s32 index;
    u8 tag;
    s32 additional_byte_size;
    u16 stringIndex;
} ConstantStringRef;

typedef struct _ConstantFieldRef {
    s32 index;
    u8 tag;
    s32 additional_byte_size;
    u16 classIndex;
    u16 nameAndTypeIndex;
    //
    FieldInfo *fieldInfo;
} ConstantFieldRef;

typedef struct _ConstantMethodRef {
    s32 index;
    u8 tag;
    s32 additional_byte_size;
    u16 classIndex;
    u16 nameAndTypeIndex;
    //
    MethodInfo *methodInfo;
    s32 methodParaCount;
    ConstantNameAndType *nameAndType;
    Utf8String *name;
    Utf8String *descriptor;
    Utf8String *clsName;
    Pairlist *virtual_methods;
} ConstantMethodRef;

typedef struct _ConstantInterfaceMethodRef {
    s32 index;
    u8 tag;
    s32 additional_byte_size;
    u16 classIndex;
    u16 nameAndTypeIndex;

    //
    Utf8String *name;

} ConstantInterfaceMethodRef;

struct _ConstantNameAndType {
    s32 index;
    u8 tag;
    s32 additional_byte_size;
    u16 nameIndex;
    u16 typeIndex;
};

typedef struct _ConstantPool {

    /* UTF-8 String */
//    ConstantUTF8 *utf8CP;
    ArrayList *utf8CP;

    /* Integer */
//    ConstantInteger *integerCP;
    ArrayList *integerCP;

    /* Float */
//    ConstantFloat *floatCP;
    ArrayList *floatCP;

    /* Long */
//    ConstantLong *longCP;
    ArrayList *longCP;

    /* Double */
//    ConstantDouble *doubleCP;
    ArrayList *doubleCP;

    /* Class Reference */
//    ConstantClassRef *classRef;
    ArrayList *classRef;

    /* String Reference */
//    ConstantStringRef *stringRef;
    ArrayList *stringRef;

    /* Field Reference */
//    ConstantFieldRef *fieldRef;
    ArrayList *fieldRef;

    /* Method Reference */
//    ConstantMethodRef *methodRef;
    ArrayList *methodRef;

    /* Interface Reference */
//    ConstantInterfaceRef *interfaceRef;
    ArrayList *interfaceRef;

    /* Name And Type Reference */
//    ConstantNameAndType *name_and_type;
    ArrayList *name_and_type;

} ConstantPool;

typedef struct _InterfacePool {
    /* Class Reference */
    ConstantClassRef *clasz;
    s32 clasz_used;
} InterfacePool;

/* Attribute Info */
typedef struct _AttributeInfo {
    u16 attribute_name_index;
    s32 attribute_length;
    u8 *info;
    //
    CodeAttribute *converted_code;
} AttributeInfo;


typedef struct _line_number {
    u16 start_pc;
    u16 line_number;
} LineNumberTable;

typedef struct _ExceptionTable {
    u16 start_pc;
    u16 end_pc;
    u16 handler_pc;
    u16 catch_type;
} ExceptionTable;

typedef struct _LocalVarTable {
    u16 start_pc;
    u16 length;
    u16 name_index;
    u16 descriptor_index;
    u16 index;
} LocalVarTable;

struct _CodeAttribute {
    u16 attribute_name_index;
    s32 attribute_length;
    u16 max_stack;
    u16 max_locals;
    s32 code_length;
    u8 *code; // [code_length];
    u16 exception_table_length;
    ExceptionTable *exception_table; //[exception_table_length];
    u16 line_number_table_length;
    LineNumberTable *line_number_table;
    u16 local_var_table_length;
    LocalVarTable *local_var_table;

};

/* Field Info */
struct _FieldInfo {
    u16 access_flags;
    u16 name_index;
    u16 descriptor_index;
    u16 attributes_count;
    AttributeInfo *attributes;
    //link
    Utf8String *name;
    Utf8String *descriptor;
    u16 offset;//字段的偏移地址，静态字段存放在class中
    Class *_this_class;
};

/*  Field Pool */
typedef struct _FieldPool {
    FieldInfo *field;
    s32 field_used;
} FieldPool;

/* Method Info */
struct _MethodInfo {
    u16 access_flags;
    u16 name_index;
    u16 descriptor_index;
    u16 attributes_count;
    AttributeInfo *attributes;

    //link
    Utf8String *name;
    Utf8String *descriptor;
    Utf8String *paraType;
    Utf8String *returnType;
    s32 para_count;
    Class *_this_class;
    java_native_fun native_func;
    Pairlist *breakpoint;

};

/*  Method Pool */
typedef struct _MethodPool {
    MethodInfo *method;
    s32 method_used;
} MethodPool;

typedef struct _AttributePool {
    AttributeInfo *attribute;
    s32 attribute_used;
} AttributePool;

//======================= runtime =============================

/* Stack Frame */
#define STACK_ENTRY_NONE        0
#define STACK_ENTRY_INT         1
#define STACK_ENTRY_FLOAT       2
#define STACK_ENTRY_LONG        4
#define STACK_ENTRY_DOUBLE      8
#define STACK_ENTRY_REF         16

typedef struct _StackEntry {
    u8 entry[8];
    s32 type;
} StackEntry;

typedef struct _StackFrame {
    s32 max_size;
    s32 size;
    StackEntry *store;
} RuntimeStack;

//解决引用类型可能为4字节或8字节的不同情况
typedef struct _LocalVarItem {
    s32 integer;
    __refer refer;
} LocalVarItem;


struct _Runtime {
    MethodInfo *methodInfo;
    Class *clazz;
    u8 *pc;
    CodeAttribute *ca;//method bytecode
    JavaThreadInfo *threadInfo;
    Runtime *son;//sub method's runtime
    Runtime *parent;//father method's runtime
    RuntimeStack *stack;
    LocalVarItem *localVariables;
    s32 localvar_count;
    u8 wideMode;
};
//======================= class =============================

/*
 Gust 20170719 add Class define
 */
struct _ClassType {
    MemoryBlock mb;

    Utf8String *name;
    ClassFileFormat cff;
    ConstantPool constantPool;
    InterfacePool interfacePool;
    FieldPool fieldPool;
    MethodPool methodPool;
    AttributePool attributePool;

    __refer *constant_item_ptr;//存放常量池项目地址
    c8 status;
    //类变量及实例变量的参数
    s32 field_instance_start;//实例变量模板起始起址，继承自父类的变量放在前面
    s32 field_instance_len; //非静态变量长度
    s32 field_static_len; //静态变量内存长度
    c8 *field_static; //静态变量内存地址

    //public:
    s32 (*_load_from_file)(struct _ClassType *_this, c8 *file);

    Utf8String *source;

    //for array class
    s32 arr_type_index;
};

void _INIT_CLASS(Class *_this);

s32 _DESTORY_CLASS(Class *_this);

Class *class_create(void);

Class *getSuperClass(Class *clazz);

void constant_list_create(Class *clazz);

void constant_list_destory(Class *clazz);

s32 class_destory(Class *clazz);

s32 load_class(Utf8String *pClassPath, Utf8String *pClassName, hmap_t classes);

s32 load_related_class(Utf8String *classpath, Class *clazz, hmap_t classes);

s32 _LOAD_FROM_FILE(Class *_this, c8 *file);

s32 class_prepar(Class *clazz);

s32 convert_to_code_attribute(CodeAttribute *ca, AttributeInfo *attr, Class *clazz);

void class_link(Class *clazz);

void class_clinit(Class *clazz, Runtime *runtime);

void printClassFileFormat(ClassFileFormat *cff);

/* Method Pool Parser */
s32 _parse_method_pool(Class *_this, FILE *fp, s32 count);

s32 _class_method_info_destory(Class *clazz);

s32 _class_attribute_info_destory(Class *clazz);

void printMethodPool(Class *p, MethodPool *fp);

void printMethodAttributes(Class *p, MethodInfo *method);

/* Interface Pool Parser */
s32 _parse_interface_pool(Class *_this, FILE *fp, s32 count);

s32 _class_interface_pool_destory(Class *clazz);

void printInterfacePool(Class *clazz, InterfacePool *ip);

/* constant pool parser */
s32 _parse_constant_pool(Class *_this, FILE *fp, s32 count);

s32 _class_constant_pool_destory(Class *clazz);

/* Field Pool Parser */
s32 _parse_field_pool(Class *_this, FILE *fp, s32 count);

s32 _class_field_info_destory(Class *clazz);

void printFieldPool(Class *clazz, FieldPool *fp);

u8 instance_of(Class *clazz, Instance *ins);

u8 isSonOfInterface(Class *clazz, Class *son);

u8 assignable_from(Class *clazzSon, Class *clazzSuper);

//======================= instance =============================


struct _InstanceType {
    MemoryBlock mb;
    //
    union {
        c8 *obj_fields; //object fieldRef body
        c8 *arr_body;//array body
    };
    s32 arr_length;
};


Instance *instance_create(Class *clazz);

void instance_init(Instance *ins, Runtime *runtime);

s32 instance_destory(Instance *instance);


//======================= bytecode =============================


typedef s32 (*InstructFunc)(u8 **opCode, Runtime *runtime);

struct _Instruction {
    c8 *name;
    u8 opCode;
    s32 offset;
    InstructFunc func;
};

c8 *find_instruct_name(u8 op);

InstructFunc find_instruct_func(u8 op);

ConstantUTF8 *find_constant_utf8(Class *clazz, s32 index);

ConstantStringRef *find_constant_stringref(Class *clazz, s32 index);

ConstantClassRef *find_constant_classref(Class *clazz, s32 index);

ConstantMethodRef *find_constant_method_ref(Class *clazz, s32 index);

ConstantInterfaceMethodRef *find_constant_interface_method_ref(Class *clazz, s32 index);

ConstantNameAndType *find_constant_name_and_type(Class *clazz, s32 index);

ConstantItem *find_constant_item(Class *clazz, s32 index);

MethodInfo *find_instance_methodInfo_by_name(Instance *ins, Utf8String *methodName, Utf8String *methodType);

MethodInfo *find_methodInfo_by_methodref(Class *clazz, s32 method_ref);

MethodInfo *find_methodInfo_by_name(Utf8String *clsName, Utf8String *methodName, Utf8String *methodType);

ConstantFieldRef *find_constant_fieldref(Class *clazz, s32 field_ref);

FieldInfo *find_fieldInfo_by_fieldref(Class *clazz, s32 field_ref);

FieldInfo *find_fieldInfo_by_name(Utf8String *clsName, Utf8String *fieldName, Utf8String *fieldType);

s32 find_constant_fieldref_index(Class *clazz, Utf8String *fieldName, Utf8String *type);

//

s32 get_constant_integer(Class *clazz, s32 index);

s64 get_constant_long(Class *clazz, s32 index);

Utf8String *get_utf8_string(Class *clazz, s32 index);

f32 get_constant_float(Class *clazz, s32 index);

f64 get_double_from_constant_pool(Class *clazz, s32 index);

Class *getClassByConstantClassRef(Class *clazz, s32 index);

//======================= execute =============================

s32 execute(c8 *p_classpath, c8 *mainclass, s32 argc, c8 **argv);

s32 execute_method(MethodInfo *method, Runtime *runtime, Class *clazz);

Instruction **instruct_indexies_create(void);

void instruct_indexies_destory(Instruction **instcts);
//======================= stack =============================

void push_entry(RuntimeStack *stack, StackEntry *entry);

void push_int(RuntimeStack *stack, s32 value);

void push_long(RuntimeStack *stack, s64 value);

void push_double(RuntimeStack *stack, f64 value);

void push_float(RuntimeStack *stack, f32 value);

void push_ref(RuntimeStack *stack, __refer value);

__refer pop_ref(RuntimeStack *stack);

s32 pop_int(RuntimeStack *stack);

s64 pop_long(RuntimeStack *stack);

f64 pop_double(RuntimeStack *stack);

f32 pop_float(RuntimeStack *stack);

void pop_entry(RuntimeStack *stack, StackEntry *entry);

s32 entry_2_int(StackEntry *entry);

s64 entry_2_long(StackEntry *entry);

__refer entry_2_refer(StackEntry *entry);

s32 is_cat2(StackEntry *entry);

s32 is_cat1(StackEntry *entry);

s32 is_ref(StackEntry *entry);

void stack2localvar(MethodInfo *method, Runtime *father, Runtime *son);

void peek_entry(RuntimeStack *stack, StackEntry *entry, int index);

//======================= localvar =============================
s32 localvar_init(Runtime *runtime, s32 count);

s32 localvar_dispose(Runtime *runtime);

//======================= other =============================
void open_log(void);

void close_log(void);

c8 *getMajorVersionString(u16 major_number);

#ifdef __cplusplus
}
#endif


#endif
