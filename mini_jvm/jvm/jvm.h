
#ifndef PUP_JVM_H
#define PUP_JVM_H
#define HAVE_STRUCT_TIMESPEC


//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jvm/global.h"
#include "../utils/hashtable.h"
#include "../utils/utf8_string.h"
#include "../utils/arraylist.h"
#include "../utils/pairlist.h"


//=======================  micro define  =============================
#define _JVM_DEBUG 0
#define _JVM_DEBUG_BYTECODE_DUMP 0
#define _JVM_DEBUG_GARBAGE_DUMP 0
#define _JVM_DEBUG_PROFILE 0

#ifndef LINUX
#define LINUX 1
#endif

// x86   x64 ...
#define __JVM_LITTLE_ENDIAN__ 1
// arm
#define __JVM_BIG_ENDIAN__ 0

#define __JVM_OS_LINUX__ 1
#define __JVM_OS_WIN__  0

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

typedef union _field_value FieldValue;
typedef struct _ClassType Class;
typedef struct _InstanceType Instance;
typedef struct _FieldInfo FieldInfo;
typedef struct _MethodInfo MethodInfo;
typedef struct _Instruction Instruction;
typedef struct _ConstantNameAndType ConstantNameAndType;
typedef struct _JavaThreadLock JavaThreadLock;
typedef struct _JavaThreadInfo JavaThreadInfo;
typedef struct _Runtime Runtime;

typedef s32 (*java_native_fun)(Runtime *runtime, Class *p);

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
    JVM_EXCEPTION_ARRAYINDEXOUTOFBOUNDS,
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
};

static char *STR_CLASS_JAVA_LANG_STRING = "java/lang/String";
static char *STR_CLASS_JAVA_LANG_OBJECT = "java/lang/Object";
static char *STR_CLASS_JAVA_LANG_THREAD = "java/lang/Thread";
static char *STR_FIELD_THREADQ = "threadQ";
static char *STR_FIELD_VALUE = "value";
static char *STR_FIELD_COUNT = "count";
static char *STR_FIELD_OFFSET = "offset";

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
    MEM_TYPE_NODEF,
    MEM_TYPE_CLASS,
    MEM_TYPE_RUNTIME,
    MEM_TYPE_INS,
    MEM_TYPE_ARR
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
    CLASS_STATUS_LINKED,
    CLASS_STATUS_CLINITING,
    CLASS_STATUS_CLINITED,
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

extern Class *JVM_CLASS;
/**
 * key =  package+classname
 * value =  class_ptr
 */
#define METHOD_MAX_PARA_LENGHT 256
extern Utf8String *classpath;
extern Hashtable *classes;
extern ArrayList *thread_list;
extern ArrayList *native_libs;
extern s32 STACK_LENGHT;
extern Hashtable *sys_prop;


extern Hashtable *son_2_father; //key=mem_ptr, value=我被别人引用的列表
extern Hashtable *father_2_son; //key=mem_ptr, value=别人被我引用的列表
extern s64 MAX_HEAP_SIZE;
extern s64 heap_size; //当前已经分配的内存总数
extern Instance *main_thread;


Instruction **instructionsIndexies;

#if _JVM_DEBUG_PROFILE
extern Hashtable *instruct_profile;
#endif

//======================= MEM_OBJ =============================

typedef struct _MemoryBlock {
    u8 type;//type of array or object runtime,class
    u8 garbage_mark;
    Class *obj_of_clazz;
    JavaThreadLock *thread_lock;
} MemoryBlock;
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
    Instance *jstr;
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

typedef struct _ConstantNameAndType {
    s32 index;
    u8 tag;
    s32 additional_byte_size;
    u16 nameIndex;
    u16 typeIndex;
} ConstantNameAndType;

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
    u8 *converted_code;
    u8 *converted_exception;
} AttributeInfo;


typedef struct _line_number {
    u16 start_pc;
    u16 line_number;
} line_number;
typedef struct _LineNumberTable {
    u16 attribute_name_index;
    u32 attribute_length;
    u16 line_number_table_length;
    u8 *table;
} LineNumberTable;
typedef struct _ExceptionTable {
    u16 start_pc;
    u16 end_pc;
    u16 handler_pc;
    u16 catch_type;
} ExceptionTable;
/* Code Attributes */
typedef struct _CodeAttribute {
    u16 attribute_name_index;
    s32 attribute_length;
    u16 max_stack;
    u16 max_locals;
    s32 code_length;
    u8 *code; // [code_length];
    u16 exception_table_length;
    ExceptionTable *exception_table; //[exception_table_length];
    ArrayList *line_num_table;

#if 0
    u16 attributes_count;
    attribute_info* attributes; //[attributes_count];
#endif
} CodeAttribute;

/* Field Info */
typedef struct _FieldInfo {
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
} FieldInfo;

/*  Field Pool */
typedef struct _FieldPool {
    FieldInfo *field;
    s32 field_used;
} FieldPool;

/* Method Info */
typedef struct _MethodInfo {
    u16 access_flags;
    u16 name_index;
    u16 descriptor_index;
    u16 attributes_count;
    AttributeInfo *attributes;

    //link
    Utf8String *name;
    Utf8String *descriptor;
    Utf8String *paraType;
    Class *_this_class;
    java_native_fun native_func;

} MethodInfo;

/*  Method Pool */
typedef struct _MethodPool {
    MethodInfo *method;
    s32 method_used;
} MethodPool;
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
} StackFrame;

//解决引用类型可能为4字节或8字节的不同情况
typedef struct _LocalVarItem {
    s32 integer;
    __refer refer;
} LocalVarItem;


typedef struct _Runtime {
    u8 type;//type of array or object or runtime
    StackFrame *stack;
    LocalVarItem *localVariables;
    s32 localvar_count;
    MethodInfo *methodInfo;
    Class *clazz;
    CodeAttribute *codeAttr;
    u8 wideMode;
//    Instance *thread;
    JavaThreadInfo *threadInfo;
    Runtime *son;//sub method's runtime
} Runtime;
//======================= class =============================

/*
 Gust 20170719 add Class define
 */
typedef struct _ClassType {
    MemoryBlock mb;

    //
    Utf8String *name;

    ClassFileFormat cff;
    ConstantPool constantPool;
    InterfacePool interfacePool;
    FieldPool fieldPool;
    MethodPool methodPool;

    __refer *constant_item_ptr;//存放常量池项目地址
    c8 status;
    //类变量及实例变量的参数
    s32 field_instance_start;//实例变量模板起始起址，继承自父类的变量放在前面
    s32 field_instance_len; //非静态变量长度
    c8 *field_instance_template;//实例变量模板
    s32 field_static_len; //静态变量内存长度
    c8 *field_static; //静态变量内存地址


    //public:
    s32 (*_load_from_file)(struct _ClassType *_this, c8 *file);


} Class;

void _INIT_CLASS(Class *_this);

s32 _DESTORY_CLASS(Class *_this);

Class *class_create();

Class *getSuperClass(Class *clazz);

void constant_list_create(Class *clazz);

void constant_list_destory(Class *clazz);

s32 class_destory(Class *clazz);

void load_class(Utf8String *pClassPath, Utf8String *pClassName, hmap_t classes);

void load_related_class(Utf8String *classpath, Class *clazz, hmap_t classes);

s32 _LOAD_FROM_FILE(Class *_this, c8 *file);

s32 class_link(Class *clazz);

s32 convert_to_code_attribute(CodeAttribute *ca, AttributeInfo *attr, Class *clazz);

void class_optmize(Class *clazz);

void class_clinit(Class *clazz, Runtime *runtime);

c8 *getInstanceFieldPtr(Instance *ins, FieldInfo *fi);

c8 *getStaticFieldPtr(FieldInfo *fi);

void setFieldInt(c8 *ptr, s32 v);

void setFieldRefer(c8 *ptr, __refer v);

void setFieldLong(c8 *ptr, s64 v);

void setFieldShort(c8 *ptr, s16 v);

void setFieldByte(c8 *ptr, c8 v);

void setFieldDouble(c8 *ptr, f64 v);

void setFieldFloat(c8 *ptr, f32 v);

s64 getFieldLong(c8 *ptr);

c8 getFieldByte(c8 *ptr);

s16 getFieldShort(c8 *ptr);

s32 getFieldInt(c8 *ptr);

__refer getFieldRefer(c8 *ptr);

f32 getFieldDouble(c8 *ptr);

f32 getFieldFloat(c8 *ptr);

void printClassFileFormat(ClassFileFormat *cff);

/* Method Pool Parser */
s32 _parse_method_pool(Class *_this, FILE *fp, s32 count);

s32 _class_method_info_destory(Class *clazz);


void printMethodPool(Class *p, MethodPool *fp);

void printMethodAttributes(Class *p, MethodInfo *method);

/* Interface Pool Parser */
s32 _parse_interface_pool(Class *_this, FILE *fp, s32 count);

s32 _class_interface_pool_destory(Class *clazz);

void printInterfacePool(Class *clazz, InterfacePool *ip);

/* constant pool parser */
s32 _parse_constant_pool(Class *_this, FILE *fp, s32 count);

s32 _class_constant_pool_destory(Class *clazz);

void printConstantPool(Class *clazz);

/* Field Pool Parser */
s32 _parse_field_pool(Class *_this, FILE *fp, s32 count);

s32 _class_field_info_destory(Class *clazz);

void printFieldPool(Class *clazz, FieldPool *fp);

u8 instance_of(Class *clazz, Instance *ins);

u8 isSonOfInterface(Class *clazz, Class *son);

u8 assignable_from(Class *clazzSon, Class *clazzSuper);

Instance *jarray_create(s32 count, s32 typeIdx);

s32 jarray_destory(Instance *arr);

Instance *jarray_multi_create(ArrayList *dim, Utf8String *desc, s32 deep);

void jarray_set_field(Instance *arr, s32 index, Long2Double *l2d, s32 bytes);

void jarray_get_field(Instance *arr, s32 index, Long2Double *l2d, s32 bytes);

Instance *jstring_create(Utf8String *src, Runtime *runtime);

Instance *exception_create(s32 exception_type, Runtime *runtime);

c8 *getFieldPtr_byName(Instance *instance, c8 *pclassName, c8 *pfieldName, c8 *pfieldType);


Class *classes_get(Utf8String *clsName);

Class *classes_load_get_c(c8 *pclassName, Runtime *runtime);

s32 classes_put(Class *clazz);

Class *classes_load_get(Utf8String *pclassName, Runtime *runtime);

//======================= instance =============================


typedef struct _InstanceType {
    MemoryBlock mb;
    //
    union {
        c8 *obj_fields; //object fieldRef body
        c8 *arr_body;//array body
    };
    u8 arr_data_type;
//    Utf8String *arr_type;
    s32 arr_length;


} Instance;


Instance *instance_create(Class *clazz);

void instance_init(Instance *ins, Runtime *runtime);

s32 instance_destory(Instance *instance);


//======================= bytecode =============================


typedef s32 (*InstructFunc)(u8 **opCode, Runtime *runtime);

typedef struct _Instruction {
    c8 *name;
    u8 opCode;
    s32 offset;
    InstructFunc func;
} Instruction;

static c8 *find_instruct_name(u8 op);

static InstructFunc find_instruct_func(u8 op);

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

Instruction **createInstructIndexies();

//======================= stack =============================

StackFrame *stack_init(s32 entry_size);

void stack_destory(StackFrame *stack);

void push_entry(StackFrame *stack, StackEntry *entry);

void push_int(StackFrame *stack, s32 value);

void push_long(StackFrame *stack, s64 value);

void push_double(StackFrame *stack, f64 value);

void push_float(StackFrame *stack, f32 value);

void push_ref(StackFrame *stack, __refer value);

__refer pop_ref(StackFrame *stack);

s32 pop_int(StackFrame *stack);

s64 pop_long(StackFrame *stack);

f64 pop_double(StackFrame *stack);

f32 pop_float(StackFrame *stack);

void pop_entry(StackFrame *stack, StackEntry *entry);

s32 entry_2_int(StackEntry *entry);

s64 entry_2_long(StackEntry *entry);

__refer entry_2_refer(StackEntry *entry);

s32 is_cat2(StackEntry *entry);

s32 is_cat1(StackEntry *entry);

s32 is_ref(StackEntry *entry);

void stack2localvar(MethodInfo *method, Runtime *father, Runtime *son);

static s32 op_notsupport(u8 **opCode, Runtime *runtime);

void peek_entry(StackFrame *stack, StackEntry *entry, int index);

//======================= localvar =============================
s32 localvar_init(Runtime *runtime, s32 count);


#endif
