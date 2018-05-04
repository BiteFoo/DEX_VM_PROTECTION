/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Dalvik opcode information.
 *
 * IMPORTANT NOTE: The contents of this file are mostly generated
 * automatically by the opcode-gen tool. Any edits to the generated
 * sections will get wiped out the next time the tool is run.
 *
 * See the file opcode-gen/README.txt for information about updating
 * opcodes and instruction formats.
 */

#ifndef LIBDEX_DEXOPCODES_H_
#define LIBDEX_DEXOPCODES_H_

#include "Common.h"
#include "avmp.h"
#include "log.h"

//////////////////////////////////////////////////////////////////////////


#define VMvision 3;

enum AccessFlags {
    /** public member / class */
    ACC_PUBLIC = 0x0001,

    /** private member */
    ACC_PRIVATE = 0x0002,

    /** protected member */
    ACC_PROTECTED = 0x0004,

    /** static member */
    ACC_STATIC = 0x0008,

    /** final member / class */
    ACC_FINAL = 0x0010,

    /**
     * synchronized method, only valid in dex files for {@code native}
     * methods
     */
    ACC_SYNCHRONIZED = 0x0020,

    /**
     * class with new-style {@code invokespecial} for superclass
     * method access
     */
    ACC_SUPER = 0x0020,

    /** volatile field */
    ACC_VOLATILE = 0x0040,

    /** bridge method (generated) */
    ACC_BRIDGE = 0x0040,

    /** transient field */
    ACC_TRANSIENT = 0x0080,

    /** varargs method */
    ACC_VARARGS = 0x0080,

    /** native method */
    ACC_NATIVE = 0x0100,

    /** "class" is in fact an public static final interface */
    ACC_INTERFACE = 0x0200,

    /** abstract method / class */
    ACC_ABSTRACT = 0x0400,

    /**
     * method with strict floating point ({@code strictfp})
     * behavior
     */
    ACC_STRICT = 0x0800,

    /** synthetic member */
    ACC_SYNTHETIC = 0x1000,

    /** class is an annotation type */
    ACC_ANNOTATION = 0x2000,

    /**
     * class is an enumerated type, field is an element of an enumerated
     * type
     */
    ACC_ENUM = 0x4000,

    /** method is a constructor */
    ACC_CONSTRUCTOR = 0x10000,

    /**
     * method was declared {@code synchronized}, has no effect on
     * execution (other than inspecting this flag, per se)
     */
    ACC_DECLARED_SYNCHRONIZED = 0x20000,
};

//////////////////////////////////////////////////////////////////////////

//#include "DexFile.h"

/*
 * kMaxOpcodeValue: the highest possible raw (unpacked) opcode value
 *
 * kNumPackedOpcodes: the highest possible packed opcode value of a
 * valid Dalvik opcode, plus one
 *
 * TODO: Change this once the rest of the code is prepared to deal with
 * extended opcodes.
 */
// BEGIN(libdex-maximum-values); GENERATED AUTOMATICALLY BY opcode-gen
#define kMaxOpcodeValue 0xffff
#define kNumPackedOpcodes 0x100
// END(libdex-maximum-values); GENERATED AUTOMATICALLY BY opcode-gen

/*
 * Switch table and array data signatures are a code unit consisting
 * of "NOP" (0x00) in the low-order byte and a non-zero identifying
 * code in the high-order byte. (A true NOP is 0x0000.)
 */
#define kPackedSwitchSignature  0x0100
#define kSparseSwitchSignature  0x0200
#define kArrayDataSignature     0x0300

/*
 * Enumeration of all Dalvik opcodes, where the enumeration value
 * associated with each is the corresponding packed opcode number.
 * This is different than the opcode value from the Dalvik bytecode
 * spec for opcode values >= 0xff; see dexOpcodeFromCodeUnit() below.
 *
 * A note about the "breakpoint" opcode. This instruction is special,
 * in that it should never be seen by anything but the debug
 * interpreter. During debugging it takes the place of an arbitrary
 * opcode, which means operations like "tell me the opcode width so I
 * can find the next instruction" aren't possible. (This is
 * correctable, but probably not useful.)
 */
//extern const int which;




//#define SDKVERSION 2
#if SDKVERSION ==2
enum Opcode {//根据指令得到相应的Label地址

    OP_NOP                          = 0x4c,
    OP_MOVE                         = 0x4b,
    OP_MOVE_FROM16                  = 0x4a,
    OP_MOVE_16                      = 0x49,
    OP_MOVE_WIDE                    = 0x48,
    OP_MOVE_WIDE_FROM16             = 0x47,
    OP_MOVE_WIDE_16                 = 0x46,
    OP_MOVE_OBJECT                  = 0x45,
    OP_MOVE_OBJECT_FROM16           = 0x44,
    OP_MOVE_OBJECT_16               = 0x43,
    OP_MOVE_RESULT                  = 0x42,
    OP_MOVE_RESULT_WIDE             = 0x41,
    OP_MOVE_RESULT_OBJECT           = 0x40,
    OP_MOVE_EXCEPTION               = 0x3f,
    OP_RETURN_VOID                  = 0x3e,
    OP_RETURN                       = 0x3d,
    OP_RETURN_WIDE                  = 0x3c,
    OP_RETURN_OBJECT                = 0x3b,
    OP_CONST_4                      = 0x3a,
    OP_CONST_16                     = 0x39,
    OP_CONST                        = 0x38,
    OP_CONST_HIGH16                 = 0x37,
    OP_CONST_WIDE_16                = 0x36,
    OP_CONST_WIDE_32                = 0x35,
    OP_CONST_WIDE                   = 0x34,
    OP_CONST_WIDE_HIGH16            = 0x33,
    OP_CONST_STRING                 = 0x32,
    OP_CONST_STRING_JUMBO           = 0x31,
    OP_CONST_CLASS                  = 0x30,
    OP_MONITOR_ENTER                = 0x2f,
    OP_MONITOR_EXIT                 = 0x2e,
    OP_CHECK_CAST                   = 0x2d,
    OP_INSTANCE_OF                  = 0x2c,
    OP_ARRAY_LENGTH                 = 0x2b,
    OP_NEW_INSTANCE                 = 0x2a,
    OP_NEW_ARRAY                    = 0x29,
    OP_FILLED_NEW_ARRAY             = 0x28,
    OP_FILLED_NEW_ARRAY_RANGE       = 0x27,
    OP_FILL_ARRAY_DATA              = 0x26,
    OP_THROW                        = 0x25,
    OP_GOTO                         = 0x24,
    OP_GOTO_16                      = 0x23,
    OP_GOTO_32                      = 0x22,
    OP_PACKED_SWITCH                = 0x21,
    OP_SPARSE_SWITCH                = 0x20,
    OP_CMPL_FLOAT                   = 0x1f,
    OP_CMPG_FLOAT                   = 0x1e,
    OP_CMPL_DOUBLE                  = 0x1d,
    OP_CMPG_DOUBLE                  = 0x1c,
    OP_CMP_LONG                     = 0x1b,
    OP_IF_EQ                        = 0x1a,
    OP_IF_NE                        = 0x19,
    OP_IF_LT                        = 0x18,
    OP_IF_GE                        = 0x17,
    OP_IF_GT                        = 0x16,
    OP_IF_LE                        = 0x15,
    OP_IF_EQZ                       = 0x14,
    OP_IF_NEZ                       = 0x13,
    OP_IF_LTZ                       = 0x12,
    OP_IF_GEZ                       = 0x11,
    OP_IF_GTZ                       = 0x10,
    OP_IF_LEZ                       = 0xf,
    OP_UNUSED_3E                    = 0xe,
    OP_UNUSED_3F                    = 0xd,
    OP_UNUSED_40                    = 0xc,
    OP_UNUSED_41                    = 0xb,
    OP_UNUSED_42                    = 0xa,
    OP_UNUSED_43                    = 0x9,
    OP_AGET                         = 0x8,
    OP_AGET_WIDE                    = 0x7,
    OP_AGET_OBJECT                  = 0x6,
    OP_AGET_BOOLEAN                 = 0x5,
    OP_AGET_BYTE                    = 0x4,
    OP_AGET_CHAR                    = 0x3,
    OP_AGET_SHORT                   = 0x2,
    OP_APUT                         = 0x1,
    OP_APUT_WIDE                    = 0x00,
    OP_APUT_OBJECT                  = 0xff,
    OP_APUT_BOOLEAN                 = 0xfe,
    OP_APUT_BYTE                    = 0xfd,
    OP_APUT_CHAR                    = 0xfc,
    OP_APUT_SHORT                   = 0xfb,
    OP_IGET                         = 0xfa,
    OP_IGET_WIDE                    = 0xf9,
    OP_IGET_OBJECT                  = 0xf8,
    OP_IGET_BOOLEAN                 = 0xf7,
    OP_IGET_BYTE                    = 0xf6,
    OP_IGET_CHAR                    = 0xf5,
    OP_IGET_SHORT                   = 0xf4,
    OP_IPUT                         = 0xf3,
    OP_IPUT_WIDE                    = 0xf2,
    OP_IPUT_OBJECT                  = 0xf1,
    OP_IPUT_BOOLEAN                 = 0xf0,
    OP_IPUT_BYTE                    = 0xef,
    OP_IPUT_CHAR                    = 0xee,
    OP_IPUT_SHORT                   = 0xed,
    OP_SGET                         = 0xec,
    OP_SGET_WIDE                    = 0xeb,
    OP_SGET_OBJECT                  = 0xea,
    OP_SGET_BOOLEAN                 = 0xe9,
    OP_SGET_BYTE                    = 0xe8,
    OP_SGET_CHAR                    = 0xe7,
    OP_SGET_SHORT                   = 0xe6,
    OP_SPUT                         = 0xe5,
    OP_SPUT_WIDE                    = 0xe4,
    OP_SPUT_OBJECT                  = 0xe3,
    OP_SPUT_BOOLEAN                 = 0xe2,
    OP_SPUT_BYTE                    = 0xe1,
    OP_SPUT_CHAR                    = 0xe0,
    OP_SPUT_SHORT                   = 0xdf,
    OP_INVOKE_VIRTUAL               = 0xde,
    OP_INVOKE_SUPER                 = 0xdd,
    OP_INVOKE_DIRECT                = 0xdc,
    OP_INVOKE_STATIC                = 0xdb,
    OP_INVOKE_INTERFACE             = 0xda,
    OP_UNUSED_73                    = 0xa9,
    OP_INVOKE_VIRTUAL_RANGE         = 0xd8,
    OP_INVOKE_SUPER_RANGE           = 0xd7,
    OP_INVOKE_DIRECT_RANGE          = 0xd6,
    OP_INVOKE_STATIC_RANGE          = 0xd5,
    OP_INVOKE_INTERFACE_RANGE       = 0xd4,
    OP_UNUSED_79                    = 0xd3,
    OP_UNUSED_7A                    = 0xd2,
    OP_NEG_INT                      = 0xd1,
    OP_NOT_INT                      = 0xd0,
    OP_NEG_LONG                     = 0xcf,
    OP_NOT_LONG                     = 0xce,
    OP_NEG_FLOAT                    = 0xcd,
    OP_NEG_DOUBLE                   = 0xcc,
    OP_INT_TO_LONG                  = 0xcb,
    OP_INT_TO_FLOAT                 = 0xca,
    OP_INT_TO_DOUBLE                = 0xc9,
    OP_LONG_TO_INT                  = 0xc8,
    OP_LONG_TO_FLOAT                = 0xc7,
    OP_LONG_TO_DOUBLE               = 0xc6,
    OP_FLOAT_TO_INT                 = 0xc5,
    OP_FLOAT_TO_LONG                = 0xc4,
    OP_FLOAT_TO_DOUBLE              = 0xc3,
    OP_DOUBLE_TO_INT                = 0xc2,
    OP_DOUBLE_TO_LONG               = 0xc1,
    OP_DOUBLE_TO_FLOAT              = 0xc0,
    OP_INT_TO_BYTE                  = 0xbf,
    OP_INT_TO_CHAR                  = 0xbe,
    OP_INT_TO_SHORT                 = 0xbd,
    OP_ADD_INT                      = 0xbc,
    OP_SUB_INT                      = 0xbb,
    OP_MUL_INT                      = 0xba,
    OP_DIV_INT                      = 0xb9,
    OP_REM_INT                      = 0xb8,
    OP_AND_INT                      = 0xb7,
    OP_OR_INT                       = 0xb6,
    OP_XOR_INT                      = 0xb5,
    OP_SHL_INT                      = 0xb4,
    OP_SHR_INT                      = 0xb3,
    OP_USHR_INT                     = 0xb2,
    OP_ADD_LONG                     = 0xb1,
    OP_SUB_LONG                     = 0xb0,
    OP_MUL_LONG                     = 0xaf,
    OP_DIV_LONG                     = 0xae,
    OP_REM_LONG                     = 0xad,
    OP_AND_LONG                     = 0xac,
    OP_OR_LONG                      = 0xab,
    OP_XOR_LONG                     = 0xaa,
    OP_SHL_LONG                     = 0xa9,
    OP_SHR_LONG                     = 0xa8,
    OP_USHR_LONG                    = 0xa7,
    OP_ADD_FLOAT                    = 0xa6,
    OP_SUB_FLOAT                    = 0xa5,
    OP_MUL_FLOAT                    = 0xa4,
    OP_DIV_FLOAT                    = 0xa3,
    OP_REM_FLOAT                    = 0xa2,
    OP_ADD_DOUBLE                   = 0xa1,
    OP_SUB_DOUBLE                   = 0xa0,
    OP_MUL_DOUBLE                   = 0x9f,
    OP_DIV_DOUBLE                   = 0x9e,
    OP_REM_DOUBLE                   = 0x9d,
    OP_ADD_INT_2ADDR                = 0x9c,
    OP_SUB_INT_2ADDR                = 0x9b,
    OP_MUL_INT_2ADDR                = 0x9a,
    OP_DIV_INT_2ADDR                = 0x99,
    OP_REM_INT_2ADDR                = 0x98,
    OP_AND_INT_2ADDR                = 0x97,
    OP_OR_INT_2ADDR                 = 0x96,
    OP_XOR_INT_2ADDR                = 0x95,
    OP_SHL_INT_2ADDR                = 0x94,
    OP_SHR_INT_2ADDR                = 0x93,
    OP_USHR_INT_2ADDR               = 0x92,
    OP_ADD_LONG_2ADDR               = 0x91,
    OP_SUB_LONG_2ADDR               = 0x90,
    OP_MUL_LONG_2ADDR               = 0x8f,
    OP_DIV_LONG_2ADDR               = 0x8e,
    OP_REM_LONG_2ADDR               = 0x8d,
    OP_AND_LONG_2ADDR               = 0x8c,
    OP_OR_LONG_2ADDR                = 0x8b,
    OP_XOR_LONG_2ADDR               = 0x8a,
    OP_SHL_LONG_2ADDR               = 0x89,
    OP_SHR_LONG_2ADDR               = 0x88,
    OP_USHR_LONG_2ADDR              = 0x87,
    OP_ADD_FLOAT_2ADDR              = 0x86,
    OP_SUB_FLOAT_2ADDR              = 0x85,
    OP_MUL_FLOAT_2ADDR              = 0x84,
    OP_DIV_FLOAT_2ADDR              = 0x83,
    OP_REM_FLOAT_2ADDR              = 0x82,
    OP_ADD_DOUBLE_2ADDR             = 0x81,
    OP_SUB_DOUBLE_2ADDR             = 0x80,
    OP_MUL_DOUBLE_2ADDR             = 0x7f,
    OP_DIV_DOUBLE_2ADDR             = 0x7e,
    OP_REM_DOUBLE_2ADDR             = 0x7d,
    OP_ADD_INT_LIT16                = 0x7c,
    OP_RSUB_INT                     = 0x7b,
    OP_MUL_INT_LIT16                = 0x7a,
    OP_DIV_INT_LIT16                = 0x79,
    OP_REM_INT_LIT16                = 0x78,
    OP_AND_INT_LIT16                = 0x77,
    OP_OR_INT_LIT16                 = 0x76,
    OP_XOR_INT_LIT16                = 0x75,
    OP_ADD_INT_LIT8                 = 0x74,
    OP_RSUB_INT_LIT8                = 0x73,
    OP_MUL_INT_LIT8                 = 0x72,
    OP_DIV_INT_LIT8                 = 0x71,
    OP_REM_INT_LIT8                 = 0x70,
    OP_AND_INT_LIT8                 = 0x6f,
    OP_OR_INT_LIT8                  = 0x6e,
    OP_XOR_INT_LIT8                 = 0x6d,
    OP_SHL_INT_LIT8                 = 0x6c,
    OP_SHR_INT_LIT8                 = 0x6b,
    OP_USHR_INT_LIT8                = 0x6a,
    OP_IGET_VOLATILE                = 0x69,
    OP_IPUT_VOLATILE                = 0x68,
    OP_SGET_VOLATILE                = 0x67,
    OP_SPUT_VOLATILE                = 0x66,
    OP_IGET_OBJECT_VOLATILE         = 0x65,
    OP_IGET_WIDE_VOLATILE           = 0x64,
    OP_IPUT_WIDE_VOLATILE           = 0x63,
    OP_SGET_WIDE_VOLATILE           = 0x62,
    OP_SPUT_WIDE_VOLATILE           = 0x61,
    OP_BREAKPOINT                   = 0x60,
    OP_THROW_VERIFICATION_ERROR     = 0x5f,
    OP_EXECUTE_INLINE               = 0x5e,
    OP_EXECUTE_INLINE_RANGE         = 0x5d,
    OP_INVOKE_OBJECT_INIT_RANGE     = 0x5c,
    OP_RETURN_VOID_BARRIER          = 0x5b,
    OP_IGET_QUICK                   = 0x5a,
    OP_IGET_WIDE_QUICK              = 0x59,
    OP_IGET_OBJECT_QUICK            = 0x58,
    OP_IPUT_QUICK                   = 0x57,
    OP_IPUT_WIDE_QUICK              = 0x56,
    OP_IPUT_OBJECT_QUICK            = 0x55,
    OP_INVOKE_VIRTUAL_QUICK         = 0x54,
    OP_INVOKE_VIRTUAL_QUICK_RANGE   = 0x53,
    OP_INVOKE_SUPER_QUICK           = 0x52,
    OP_INVOKE_SUPER_QUICK_RANGE     = 0x51,
    OP_IPUT_OBJECT_VOLATILE         = 0x50,
    OP_SGET_OBJECT_VOLATILE         = 0x4f,
    OP_SPUT_OBJECT_VOLATILE         = 0x4e,
    OP_UNUSED_FF                    = 0x4d,
    // END(libdex-opcode-enum)
};
#define DEFINE_GOTO_TABLE(_name) \
    static const void* _name[kNumPackedOpcodes] = {                      \
        /* BEGIN(libdex-goto-table); GENERATED AUTOMATICALLY BY opcode-gen */ \
        H(OP_APUT_WIDE),                                                            \
        H(OP_APUT),                                                           \
        H(OP_AGET_SHORT),                                                    \
        H(OP_AGET_CHAR),                                                        \
        H(OP_AGET_BYTE),                                                      \
        H(OP_AGET_BOOLEAN),                                               \
        H(OP_AGET_OBJECT),                                                   \
        H(OP_AGET_WIDE),                                                    \
        H(OP_AGET),                                             \
        H(OP_UNUSED_43),                                                 \
        H(OP_UNUSED_42),                                                    \
        H(OP_UNUSED_41),                                               \
        H(OP_UNUSED_40),                                             \
        H(OP_UNUSED_3F),                                                 \
        H(OP_UNUSED_3E),                                                    \
        H(OP_IF_LEZ),                                                         \
        H(OP_IF_GTZ),                                                    \
        H(OP_IF_GEZ),                                                  \
        H(OP_IF_LTZ),                                                        \
        H(OP_IF_NEZ),                                                        \
        H(OP_IF_EQZ),                                                        \
        H(OP_IF_LE),                                                       \
        H(OP_IF_GT),                                                          \
        H(OP_IF_GE),                                                   \
        H(OP_IF_LT),                                                  \
        H(OP_IF_NE),                                                  \
        H(OP_IF_EQ),                                                     \
        H(OP_CMP_LONG),                                              \
        H(OP_CMPG_DOUBLE),                                                   \
        H(OP_CMPL_DOUBLE),                                             \
        H(OP_CMPG_FLOAT),                                                    \
        H(OP_CMPL_FLOAT),                                                  \
        H(OP_SPARSE_SWITCH),                                                   \
        H(OP_PACKED_SWITCH),                                                     \
        H(OP_GOTO_32),                                                    \
        H(OP_GOTO_16),                                                   \
        H(OP_GOTO),                                                   \
        H(OP_THROW),                                                      \
        H(OP_FILL_ARRAY_DATA),                                               \
        H(OP_FILLED_NEW_ARRAY_RANGE),                                         \
        H(OP_FILLED_NEW_ARRAY),                                                \
        H(OP_NEW_ARRAY),                                                          \
        H(OP_NEW_INSTANCE),                                                           \
        H(OP_ARRAY_LENGTH),                                                        \
        H(OP_INSTANCE_OF),                                                        \
        H(OP_CHECK_CAST),                                                  \
        H(OP_MONITOR_EXIT),                                                  \
        H(OP_MONITOR_ENTER),                                                     \
        H(OP_CONST_CLASS),                                                     \
        H(OP_CONST_STRING_JUMBO),                                                    \
        H(OP_CONST_STRING),                                                    \
        H(OP_CONST_WIDE_HIGH16),                                                       \
        H(OP_CONST_WIDE),                                                          \
        H(OP_CONST_WIDE_32),                                                          \
        H(OP_CONST_WIDE_16),                                                          \
        H(OP_CONST_HIGH16),                                                          \
        H(OP_CONST),                                                          \
        H(OP_CONST_16),                                                          \
        H(OP_CONST_4),                                                         \
        H(OP_RETURN_OBJECT),                                                         \
        H(OP_RETURN_WIDE),                                                         \
        H(OP_RETURN),                                                         \
        H(OP_RETURN_VOID),                                                         \
        H(OP_MOVE_EXCEPTION),                                                         \
        H(OP_MOVE_RESULT_OBJECT),                                                         \
        H(OP_MOVE_RESULT_WIDE),                                                      \
        H(OP_MOVE_RESULT),                                                      \
        H(OP_MOVE_OBJECT_16),                                                      \
        H(OP_MOVE_OBJECT_FROM16),                                                      \
        H(OP_MOVE_OBJECT),                                                      \
        H(OP_MOVE_WIDE_16),                                                      \
        H(OP_MOVE_WIDE_FROM16),                                                           \
        H(OP_MOVE_WIDE),                                                      \
        H(OP_MOVE_16),                                                    \
        H(OP_MOVE_FROM16),                                                   \
        H(OP_MOVE),                                                      \
        H(OP_NOP),                                                      \
        H(OP_UNUSED_FF),                                                     \
        H(OP_SPUT_OBJECT_VOLATILE),                                                           \
        H(OP_SGET_OBJECT_VOLATILE),                                                      \
        H(OP_IPUT_OBJECT_VOLATILE),                                                    \
        H(OP_INVOKE_SUPER_QUICK_RANGE),                                                   \
        H(OP_INVOKE_SUPER_QUICK),                                                      \
        H(OP_INVOKE_VIRTUAL_QUICK_RANGE),                                                      \
        H(OP_INVOKE_VIRTUAL_QUICK),                                                     \
        H(OP_IPUT_OBJECT_QUICK),                                                           \
        H(OP_IPUT_WIDE_QUICK),                                                      \
        H(OP_IPUT_QUICK),                                                    \
        H(OP_IGET_OBJECT_QUICK),                                                   \
        H(OP_IGET_WIDE_QUICK),                                                      \
        H(OP_IGET_QUICK),                                                      \
        H(OP_RETURN_VOID_BARRIER),                                                     \
        H(OP_INVOKE_OBJECT_INIT_RANGE),                                                           \
        H(OP_EXECUTE_INLINE_RANGE),                                                      \
        H(OP_EXECUTE_INLINE),                                                    \
        H(OP_THROW_VERIFICATION_ERROR),                                                   \
        H(OP_BREAKPOINT),                                                      \
        H(OP_SPUT_WIDE_VOLATILE),                                                      \
        H(OP_SGET_WIDE_VOLATILE),                                                     \
        H(OP_IPUT_WIDE_VOLATILE),                                                           \
        H(OP_IGET_WIDE_VOLATILE),                                                      \
        H(OP_IGET_OBJECT_VOLATILE),                                                    \
        H(OP_SPUT_VOLATILE),                                                   \
        H(OP_SGET_VOLATILE),                                                      \
        H(OP_IPUT_VOLATILE),                                                      \
        H(OP_IGET_VOLATILE),                                                     \
        H(OP_USHR_INT_LIT8),                                                           \
        H(OP_SHR_INT_LIT8),                                                      \
        H(OP_SHL_INT_LIT8),                                                    \
        H(OP_XOR_INT_LIT8),                                                   \
        H(OP_OR_INT_LIT8),                                                      \
        H(OP_AND_INT_LIT8),                                                      \
        H(OP_REM_INT_LIT8),                                                     \
        H(OP_DIV_INT_LIT8),                                                 \
        H(OP_MUL_INT_LIT8),                                                   \
        H(OP_RSUB_INT_LIT8),                                                  \
        H(OP_ADD_INT_LIT8),                                                  \
        H(OP_XOR_INT_LIT16),                                               \
        H(OP_OR_INT_LIT16),                                                      \
        H(OP_AND_INT_LIT16),                                           \
        H(OP_REM_INT_LIT16),                                             \
        H(OP_DIV_INT_LIT16),                                            \
        H(OP_MUL_INT_LIT16),                                            \
        H(OP_RSUB_INT),                                         \
        H(OP_ADD_INT_LIT16),                                                      \
        H(OP_REM_DOUBLE_2ADDR),                                                      \
        H(OP_DIV_DOUBLE_2ADDR),                                                        \
        H(OP_MUL_DOUBLE_2ADDR),                                                        \
        H(OP_SUB_DOUBLE_2ADDR),                                                       \
        H(OP_ADD_DOUBLE_2ADDR),                                                       \
        H(OP_REM_FLOAT_2ADDR),                                                      \
        H(OP_DIV_FLOAT_2ADDR),                                                     \
        H(OP_MUL_FLOAT_2ADDR),                                                    \
        H(OP_SUB_FLOAT_2ADDR),                                                   \
        H(OP_ADD_FLOAT_2ADDR),                                                  \
        H(OP_USHR_LONG_2ADDR),                                                    \
        H(OP_SHR_LONG_2ADDR),                                                  \
        H(OP_SHL_LONG_2ADDR),                                                 \
        H(OP_XOR_LONG_2ADDR),                                                   \
        H(OP_OR_LONG_2ADDR),                                                  \
        H(OP_AND_LONG_2ADDR),                                                 \
        H(OP_REM_LONG_2ADDR),                                                \
        H(OP_DIV_LONG_2ADDR),                                                    \
        H(OP_MUL_LONG_2ADDR),                                                    \
        H(OP_SUB_LONG_2ADDR),                                                   \
        H(OP_ADD_LONG_2ADDR),                                                        \
        H(OP_USHR_INT_2ADDR),                                                        \
        H(OP_SHR_INT_2ADDR),                                                        \
        H(OP_SHL_INT_2ADDR),                                                        \
        H(OP_XOR_INT_2ADDR),                                                        \
        H(OP_OR_INT_2ADDR),                                                        \
        H(OP_AND_INT_2ADDR),                                                         \
        H(OP_REM_INT_2ADDR),                                                        \
        H(OP_DIV_INT_2ADDR),                                                        \
        H(OP_MUL_INT_2ADDR),                                                        \
        H(OP_SUB_INT_2ADDR),                                                       \
        H(OP_ADD_INT_2ADDR),                                                       \
        H(OP_REM_DOUBLE),                                                       \
        H(OP_DIV_DOUBLE),                                                       \
        H(OP_MUL_DOUBLE),                                                       \
        H(OP_SUB_DOUBLE),                                                       \
        H(OP_ADD_DOUBLE),                                                       \
        H(OP_REM_FLOAT),                                                        \
        H(OP_DIV_FLOAT),                                                       \
        H(OP_MUL_FLOAT),                                                       \
        H(OP_SUB_FLOAT),                                                       \
        H(OP_ADD_FLOAT),                                                      \
        H(OP_USHR_LONG),                                                      \
        H(OP_SHR_LONG),                                                      \
        H(OP_SHL_LONG),                                                      \
        H(OP_XOR_LONG),                                                      \
        H(OP_OR_LONG),                                                      \
        H(OP_AND_LONG),                                                     \
        H(OP_REM_LONG),                                                     \
        H(OP_DIV_LONG),                                                     \
        H(OP_MUL_LONG),                                                     \
        H(OP_SUB_LONG),                                                     \
        H(OP_ADD_LONG),                                                  \
        H(OP_USHR_INT),                                                  \
        H(OP_SHR_INT),                                                  \
        H(OP_SHL_INT),                                                  \
        H(OP_XOR_INT),                                                  \
        H(OP_OR_INT),                                                  \
        H(OP_AND_INT),                                                   \
        H(OP_REM_INT),                                                  \
        H(OP_DIV_INT),                                                  \
        H(OP_MUL_INT),                                                  \
        H(OP_SUB_INT),                                                 \
        H(OP_ADD_INT),                                                 \
        H(OP_INT_TO_SHORT),                                                 \
        H(OP_INT_TO_CHAR),                                                 \
        H(OP_INT_TO_BYTE),                                                 \
        H(OP_DOUBLE_TO_FLOAT),                                                 \
        H(OP_DOUBLE_TO_LONG),                                                 \
        H(OP_DOUBLE_TO_INT),                                                  \
        H(OP_FLOAT_TO_DOUBLE),                                                 \
        H(OP_FLOAT_TO_LONG),                                                 \
        H(OP_FLOAT_TO_INT),                                                 \
        H(OP_LONG_TO_DOUBLE),                                                \
        H(OP_LONG_TO_FLOAT),                                                \
        H(OP_LONG_TO_INT),                                                \
        H(OP_INT_TO_DOUBLE),                                                \
        H(OP_INT_TO_FLOAT),                                                \
        H(OP_INT_TO_LONG),                                                \
        H(OP_NEG_DOUBLE),                                               \
        H(OP_NEG_FLOAT),                                               \
        H(OP_NOT_LONG),                                               \
        H(OP_NEG_LONG),                                               \
        H(OP_NOT_INT),                                               \
        H(OP_NEG_INT),                                                  \
        H(OP_UNUSED_7A),                                                       \
        H(OP_UNUSED_79),                                                  \
        H(OP_INVOKE_INTERFACE_RANGE),                                                  \
        H(OP_INVOKE_STATIC_RANGE),                                                  \
        H(OP_INVOKE_DIRECT_RANGE),                                                  \
        H(OP_INVOKE_SUPER_RANGE),                                                   \
        H(OP_INVOKE_VIRTUAL_RANGE),                                                  \
        H(OP_UNUSED_73),                                                   \
        H(OP_INVOKE_INTERFACE),                                                  \
        H(OP_INVOKE_STATIC),                                                   \
        H(OP_INVOKE_DIRECT),                                                   \
        H(OP_INVOKE_SUPER),                                                   \
        H(OP_INVOKE_VIRTUAL),                                                    \
        H(OP_SPUT_SHORT),                                                   \
        H(OP_SPUT_CHAR),                                                   \
        H(OP_SPUT_BYTE),                                                   \
        H(OP_SPUT_BOOLEAN),                                                  \
        H(OP_SPUT_OBJECT),                                                  \
        H(OP_SPUT_WIDE),                                                  \
        H(OP_SPUT),                                                  \
        H(OP_SGET_SHORT),                                                  \
        H(OP_SGET_CHAR),                                           \
        H(OP_SGET_BYTE),                                             \
        H(OP_SGET_BOOLEAN),                                             \
        H(OP_SGET_OBJECT),                                             \
        H(OP_SGET_WIDE),                                             \
        H(OP_SGET),                                                     \
        H(OP_IPUT_SHORT),                                       \
        H(OP_IPUT_CHAR),                                                 \
        H(OP_IPUT_BYTE),                                           \
        H(OP_IPUT_BOOLEAN),                                       \
        H(OP_IPUT_OBJECT),                                            \
        H(OP_IPUT_WIDE),                                                     \
        H(OP_IPUT),                                                \
        H(OP_IGET_SHORT),                                              \
        H(OP_IGET_CHAR),                                                     \
        H(OP_IGET_BYTE),                                                \
        H(OP_IGET_BOOLEAN),                                              \
        H(OP_IGET_OBJECT),                                           \
        H(OP_IGET_WIDE),                                     \
        H(OP_IGET),                                             \
        H(OP_APUT_SHORT),                                       \
        H(OP_APUT_CHAR),                                           \
        H(OP_APUT_BYTE),                                           \
        H(OP_APUT_BOOLEAN),                                           \
        H(OP_APUT_OBJECT),                                                      \
        /* END(libdex-goto-table) */                                          \
    };
#endif

#if SDKVERSION ==3
enum Opcode {//根据指令得到相应的Label地址

    OP_NOP                          = 0x00,
    OP_MOVE                         = 0x01,
    OP_MOVE_FROM16                  = 0x02,
    OP_MOVE_16                      = 0x03,
    OP_MOVE_WIDE                    = 0x04,
    OP_MOVE_WIDE_FROM16             = 0x05,
    OP_MOVE_WIDE_16                 = 0x06,
    OP_MOVE_OBJECT                  = 0x07,
    OP_MOVE_OBJECT_FROM16           = 0x08,
    OP_MOVE_OBJECT_16               = 0x09,
    OP_MOVE_RESULT                  = 0x0a,
    OP_MOVE_RESULT_WIDE             = 0x0b,
    OP_MOVE_RESULT_OBJECT           = 0x0c,
    OP_MOVE_EXCEPTION               = 0x0d,
    OP_RETURN_VOID                  = 0x0e,
    OP_RETURN                       = 0x0f,
    OP_RETURN_WIDE                  = 0x10,
    OP_RETURN_OBJECT                = 0x11,
    OP_CONST_4                      = 0x12,
    OP_CONST_16                     = 0x13,
    OP_CONST                        = 0x14,
    OP_CONST_HIGH16                 = 0x15,
    OP_CONST_WIDE_16                = 0x16,
    OP_CONST_WIDE_32                = 0x17,
    OP_CONST_WIDE                   = 0x18,
    OP_CONST_WIDE_HIGH16            = 0x19,
    OP_CONST_STRING                 = 0x1a,
    OP_CONST_STRING_JUMBO           = 0x1b,
    OP_CONST_CLASS                  = 0x1c,
    OP_MONITOR_ENTER                = 0x1d,
    OP_MONITOR_EXIT                 = 0x1e,
    OP_CHECK_CAST                   = 0x1f,
    OP_INSTANCE_OF                  = 0x20,
    OP_ARRAY_LENGTH                 = 0x21,
    OP_NEW_INSTANCE                 = 0x22,
    OP_NEW_ARRAY                    = 0x23,
    OP_FILLED_NEW_ARRAY             = 0x24,
    OP_FILLED_NEW_ARRAY_RANGE       = 0x25,
    OP_FILL_ARRAY_DATA              = 0x26,
    OP_THROW                        = 0x27,
    OP_GOTO                         = 0x28,
    OP_GOTO_16                      = 0x29,
    OP_GOTO_32                      = 0x2a,
    OP_PACKED_SWITCH                = 0x2b,
    OP_SPARSE_SWITCH                = 0x2c,
    OP_CMPL_FLOAT                   = 0x2d,
    OP_CMPG_FLOAT                   = 0x2e,
    OP_CMPL_DOUBLE                  = 0x2f,
    OP_CMPG_DOUBLE                  = 0x30,
    OP_CMP_LONG                     = 0x31,
    OP_IF_EQ                        = 0x32,
    OP_IF_NE                        = 0x33,
    OP_IF_LT                        = 0x34,
    OP_IF_GE                        = 0x35,
    OP_IF_GT                        = 0x36,
    OP_IF_LE                        = 0x37,
    OP_IF_EQZ                       = 0x38,
    OP_IF_NEZ                       = 0x39,
    OP_IF_LTZ                       = 0x3a,
    OP_IF_GEZ                       = 0x3b,
    OP_IF_GTZ                       = 0x3c,
    OP_IF_LEZ                       = 0x3d,
    OP_UNUSED_3E                    = 0x3e,
    OP_UNUSED_3F                    = 0x3f,
    OP_UNUSED_40                    = 0x40,
    OP_UNUSED_41                    = 0x41,
    OP_UNUSED_42                    = 0x42,
    OP_UNUSED_43                    = 0x43,
    OP_AGET                         = 0x44,
    OP_AGET_WIDE                    = 0x45,
    OP_AGET_OBJECT                  = 0x46,
    OP_AGET_BOOLEAN                 = 0x47,
    OP_AGET_BYTE                    = 0x48,
    OP_AGET_CHAR                    = 0x49,
    OP_AGET_SHORT                   = 0x4a,
    OP_APUT                         = 0x4b,
    OP_APUT_WIDE                    = 0x4c,
    OP_APUT_OBJECT                  = 0x4d,
    OP_APUT_BOOLEAN                 = 0x4e,
    OP_APUT_BYTE                    = 0x4f,
    OP_APUT_CHAR                    = 0x50,
    OP_APUT_SHORT                   = 0x51,
    OP_IGET                         = 0x52,
    OP_IGET_WIDE                    = 0x53,
    OP_IGET_OBJECT                  = 0x54,
    OP_IGET_BOOLEAN                 = 0x55,
    OP_IGET_BYTE                    = 0x56,
    OP_IGET_CHAR                    = 0x57,
    OP_IGET_SHORT                   = 0x58,
    OP_IPUT                         = 0x59,
    OP_IPUT_WIDE                    = 0x5a,
    OP_IPUT_OBJECT                  = 0x5b,
    OP_IPUT_BOOLEAN                 = 0x5c,
    OP_IPUT_BYTE                    = 0x5d,
    OP_IPUT_CHAR                    = 0x5e,
    OP_IPUT_SHORT                   = 0x5f,
    OP_SGET                         = 0x60,
    OP_SGET_WIDE                    = 0x61,
    OP_SGET_OBJECT                  = 0x62,
    OP_SGET_BOOLEAN                 = 0x63,
    OP_SGET_BYTE                    = 0x64,
    OP_SGET_CHAR                    = 0x65,
    OP_SGET_SHORT                   = 0x66,
    OP_SPUT                         = 0x67,
    OP_SPUT_WIDE                    = 0x68,
    OP_SPUT_OBJECT                  = 0x69,
    OP_SPUT_BOOLEAN                 = 0x6a,
    OP_SPUT_BYTE                    = 0x6b,
    OP_SPUT_CHAR                    = 0x6c,
    OP_SPUT_SHORT                   = 0x6d,
    OP_INVOKE_VIRTUAL               = 0x6e,
    OP_INVOKE_SUPER                 = 0x6f,
    OP_INVOKE_DIRECT                = 0x70,
    OP_INVOKE_STATIC                = 0x71,
    OP_INVOKE_INTERFACE             = 0x72,
    OP_UNUSED_73                    = 0x73,
    OP_INVOKE_VIRTUAL_RANGE         = 0x74,
    OP_INVOKE_SUPER_RANGE           = 0x75,
    OP_INVOKE_DIRECT_RANGE          = 0x76,
    OP_INVOKE_STATIC_RANGE          = 0x77,
    OP_INVOKE_INTERFACE_RANGE       = 0x78,
    OP_UNUSED_79                    = 0x79,
    OP_UNUSED_7A                    = 0x7a,
    OP_NEG_INT                      = 0x7b,
    OP_NOT_INT                      = 0x7c,
    OP_NEG_LONG                     = 0x7d,
    OP_NOT_LONG                     = 0x7e,
    OP_NEG_FLOAT                    = 0x7f,
    OP_NEG_DOUBLE                   = 0x80,
    OP_INT_TO_LONG                  = 0x81,
    OP_INT_TO_FLOAT                 = 0x82,
    OP_INT_TO_DOUBLE                = 0x83,
    OP_LONG_TO_INT                  = 0x84,
    OP_LONG_TO_FLOAT                = 0x85,
    OP_LONG_TO_DOUBLE               = 0x86,
    OP_FLOAT_TO_INT                 = 0x87,
    OP_FLOAT_TO_LONG                = 0x88,
    OP_FLOAT_TO_DOUBLE              = 0x89,
    OP_DOUBLE_TO_INT                = 0x8a,
    OP_DOUBLE_TO_LONG               = 0x8b,
    OP_DOUBLE_TO_FLOAT              = 0x8c,
    OP_INT_TO_BYTE                  = 0x8d,
    OP_INT_TO_CHAR                  = 0x8e,
    OP_INT_TO_SHORT                 = 0x8f,
    OP_ADD_INT                      = 0x90,
    OP_SUB_INT                      = 0x91,
    OP_MUL_INT                      = 0x92,
    OP_DIV_INT                      = 0x93,
    OP_REM_INT                      = 0x94,
    OP_AND_INT                      = 0x95,
    OP_OR_INT                       = 0x96,
    OP_XOR_INT                      = 0x97,
    OP_SHL_INT                      = 0x98,
    OP_SHR_INT                      = 0x99,
    OP_USHR_INT                     = 0x9a,
    OP_ADD_LONG                     = 0x9b,
    OP_SUB_LONG                     = 0x9c,
    OP_MUL_LONG                     = 0x9d,
    OP_DIV_LONG                     = 0x9e,
    OP_REM_LONG                     = 0x9f,
    OP_AND_LONG                     = 0xa0,
    OP_OR_LONG                      = 0xa1,
    OP_XOR_LONG                     = 0xa2,
    OP_SHL_LONG                     = 0xa3,
    OP_SHR_LONG                     = 0xa4,
    OP_USHR_LONG                    = 0xa5,
    OP_ADD_FLOAT                    = 0xa6,
    OP_SUB_FLOAT                    = 0xa7,
    OP_MUL_FLOAT                    = 0xa8,
    OP_DIV_FLOAT                    = 0xa9,
    OP_REM_FLOAT                    = 0xaa,
    OP_ADD_DOUBLE                   = 0xab,
    OP_SUB_DOUBLE                   = 0xac,
    OP_MUL_DOUBLE                   = 0xad,
    OP_DIV_DOUBLE                   = 0xae,
    OP_REM_DOUBLE                   = 0xaf,
    OP_ADD_INT_2ADDR                = 0xb0,
    OP_SUB_INT_2ADDR                = 0xb1,
    OP_MUL_INT_2ADDR                = 0xb2,
    OP_DIV_INT_2ADDR                = 0xb3,
    OP_REM_INT_2ADDR                = 0xb4,
    OP_AND_INT_2ADDR                = 0xb5,
    OP_OR_INT_2ADDR                 = 0xb6,
    OP_XOR_INT_2ADDR                = 0xb7,
    OP_SHL_INT_2ADDR                = 0xb8,
    OP_SHR_INT_2ADDR                = 0xb9,
    OP_USHR_INT_2ADDR               = 0xba,
    OP_ADD_LONG_2ADDR               = 0xbb,
    OP_SUB_LONG_2ADDR               = 0xbc,
    OP_MUL_LONG_2ADDR               = 0xbd,
    OP_DIV_LONG_2ADDR               = 0xbe,
    OP_REM_LONG_2ADDR               = 0xbf,
    OP_AND_LONG_2ADDR               = 0xc0,
    OP_OR_LONG_2ADDR                = 0xc1,
    OP_XOR_LONG_2ADDR               = 0xc2,
    OP_SHL_LONG_2ADDR               = 0xc3,
    OP_SHR_LONG_2ADDR               = 0xc4,
    OP_USHR_LONG_2ADDR              = 0xc5,
    OP_ADD_FLOAT_2ADDR              = 0xc6,
    OP_SUB_FLOAT_2ADDR              = 0xc7,
    OP_MUL_FLOAT_2ADDR              = 0xc8,
    OP_DIV_FLOAT_2ADDR              = 0xc9,
    OP_REM_FLOAT_2ADDR              = 0xca,
    OP_ADD_DOUBLE_2ADDR             = 0xcb,
    OP_SUB_DOUBLE_2ADDR             = 0xcc,
    OP_MUL_DOUBLE_2ADDR             = 0xcd,
    OP_DIV_DOUBLE_2ADDR             = 0xce,
    OP_REM_DOUBLE_2ADDR             = 0xcf,
    OP_ADD_INT_LIT16                = 0xd0,
    OP_RSUB_INT                     = 0xd1,
    OP_MUL_INT_LIT16                = 0xd2,
    OP_DIV_INT_LIT16                = 0xd3,
    OP_REM_INT_LIT16                = 0xd4,
    OP_AND_INT_LIT16                = 0xd5,
    OP_OR_INT_LIT16                 = 0xd6,
    OP_XOR_INT_LIT16                = 0xd7,
    OP_ADD_INT_LIT8                 = 0xd8,
    OP_RSUB_INT_LIT8                = 0xd9,
    OP_MUL_INT_LIT8                 = 0xda,
    OP_DIV_INT_LIT8                 = 0xdb,
    OP_REM_INT_LIT8                 = 0xdc,
    OP_AND_INT_LIT8                 = 0xdd,
    OP_OR_INT_LIT8                  = 0xde,
    OP_XOR_INT_LIT8                 = 0xdf,
    OP_SHL_INT_LIT8                 = 0xe0,
    OP_SHR_INT_LIT8                 = 0xe1,
    OP_USHR_INT_LIT8                = 0xe2,
    OP_IGET_VOLATILE                = 0xe3,
    OP_IPUT_VOLATILE                = 0xe4,
    OP_SGET_VOLATILE                = 0xe5,
    OP_SPUT_VOLATILE                = 0xe6,
    OP_IGET_OBJECT_VOLATILE         = 0xe7,
    OP_IGET_WIDE_VOLATILE           = 0xe8,
    OP_IPUT_WIDE_VOLATILE           = 0xe9,
    OP_SGET_WIDE_VOLATILE           = 0xea,
    OP_SPUT_WIDE_VOLATILE           = 0xeb,
    OP_BREAKPOINT                   = 0xec,
    OP_THROW_VERIFICATION_ERROR     = 0xed,
    OP_EXECUTE_INLINE               = 0xee,
    OP_EXECUTE_INLINE_RANGE         = 0xef,
    OP_INVOKE_OBJECT_INIT_RANGE     = 0xf0,
    OP_RETURN_VOID_BARRIER          = 0xf1,
    OP_IGET_QUICK                   = 0xf2,
    OP_IGET_WIDE_QUICK              = 0xf3,
    OP_IGET_OBJECT_QUICK            = 0xf4,
    OP_IPUT_QUICK                   = 0xf5,
    OP_IPUT_WIDE_QUICK              = 0xf6,
    OP_IPUT_OBJECT_QUICK            = 0xf7,
    OP_INVOKE_VIRTUAL_QUICK         = 0xf8,
    OP_INVOKE_VIRTUAL_QUICK_RANGE   = 0xf9,
    OP_INVOKE_SUPER_QUICK           = 0xfa,
    OP_INVOKE_SUPER_QUICK_RANGE     = 0xfb,
    OP_IPUT_OBJECT_VOLATILE         = 0xfc,
    OP_SGET_OBJECT_VOLATILE         = 0xfd,
    OP_SPUT_OBJECT_VOLATILE         = 0xfe,
    OP_UNUSED_FF                    = 0xff,
    // END(libdex-opcode-enum)
};

#define DEFINE_GOTO_TABLE(_name) \
    static const void* _name[kNumPackedOpcodes] = {                      \
        /* BEGIN(libdex-goto-table); GENERATED AUTOMATICALLY BY opcode-gen */ \
        H(OP_NOP),                                                            \
        H(OP_MOVE),                                                           \
        H(OP_MOVE_FROM16),                                                    \
        H(OP_MOVE_16),                                                        \
        H(OP_MOVE_WIDE),                                                      \
        H(OP_MOVE_WIDE_FROM16),                                               \
        H(OP_MOVE_WIDE_16),                                                   \
        H(OP_MOVE_OBJECT),                                                    \
        H(OP_MOVE_OBJECT_FROM16),                                             \
        H(OP_MOVE_OBJECT_16),                                                 \
        H(OP_MOVE_RESULT),                                                    \
        H(OP_MOVE_RESULT_WIDE),                                               \
        H(OP_MOVE_RESULT_OBJECT),                                             \
        H(OP_MOVE_EXCEPTION),                                                 \
        H(OP_RETURN_VOID),                                                    \
        H(OP_RETURN),                                                         \
        H(OP_RETURN_WIDE),                                                    \
        H(OP_RETURN_OBJECT),                                                  \
        H(OP_CONST_4),                                                        \
        H(OP_CONST_16),                                                       \
        H(OP_CONST),                                                          \
        H(OP_CONST_HIGH16),                                                   \
        H(OP_CONST_WIDE_16),                                                  \
        H(OP_CONST_WIDE_32),                                                  \
        H(OP_CONST_WIDE),                                                     \
        H(OP_CONST_WIDE_HIGH16),                                              \
        H(OP_CONST_STRING),                                                   \
        H(OP_CONST_STRING_JUMBO),                                             \
        H(OP_CONST_CLASS),                                                    \
        H(OP_MONITOR_ENTER),                                                  \
        H(OP_MONITOR_EXIT),                                                   \
        H(OP_CHECK_CAST),                                                     \
        H(OP_INSTANCE_OF),                                                    \
        H(OP_ARRAY_LENGTH),                                                   \
        H(OP_NEW_INSTANCE),                                                   \
        H(OP_NEW_ARRAY),                                                      \
        H(OP_FILLED_NEW_ARRAY),                                               \
        H(OP_FILLED_NEW_ARRAY_RANGE),                                         \
        H(OP_FILL_ARRAY_DATA),                                                \
        H(OP_THROW),                                                          \
        H(OP_GOTO),                                                           \
        H(OP_GOTO_16),                                                        \
        H(OP_GOTO_32),                                                        \
        H(OP_PACKED_SWITCH),                                                  \
        H(OP_SPARSE_SWITCH),                                                  \
        H(OP_CMPL_FLOAT),                                                     \
        H(OP_CMPG_FLOAT),                                                     \
        H(OP_CMPL_DOUBLE),                                                    \
        H(OP_CMPG_DOUBLE),                                                    \
        H(OP_CMP_LONG),                                                       \
        H(OP_IF_EQ),                                                          \
        H(OP_IF_NE),                                                          \
        H(OP_IF_LT),                                                          \
        H(OP_IF_GE),                                                          \
        H(OP_IF_GT),                                                          \
        H(OP_IF_LE),                                                          \
        H(OP_IF_EQZ),                                                         \
        H(OP_IF_NEZ),                                                         \
        H(OP_IF_LTZ),                                                         \
        H(OP_IF_GEZ),                                                         \
        H(OP_IF_GTZ),                                                         \
        H(OP_IF_LEZ),                                                         \
        H(OP_UNUSED_3E),                                                      \
        H(OP_UNUSED_3F),                                                      \
        H(OP_UNUSED_40),                                                      \
        H(OP_UNUSED_41),                                                      \
        H(OP_UNUSED_42),                                                      \
        H(OP_UNUSED_43),                                                      \
        H(OP_AGET),                                                           \
        H(OP_AGET_WIDE),                                                      \
        H(OP_AGET_OBJECT),                                                    \
        H(OP_AGET_BOOLEAN),                                                   \
        H(OP_AGET_BYTE),                                                      \
        H(OP_AGET_CHAR),                                                      \
        H(OP_AGET_SHORT),                                                     \
        H(OP_APUT),                                                           \
        H(OP_APUT_WIDE),                                                      \
        H(OP_APUT_OBJECT),                                                    \
        H(OP_APUT_BOOLEAN),                                                   \
        H(OP_APUT_BYTE),                                                      \
        H(OP_APUT_CHAR),                                                      \
        H(OP_APUT_SHORT),                                                     \
        H(OP_IGET),                                                           \
        H(OP_IGET_WIDE),                                                      \
        H(OP_IGET_OBJECT),                                                    \
        H(OP_IGET_BOOLEAN),                                                   \
        H(OP_IGET_BYTE),                                                      \
        H(OP_IGET_CHAR),                                                      \
        H(OP_IGET_SHORT),                                                     \
        H(OP_IPUT),                                                           \
        H(OP_IPUT_WIDE),                                                      \
        H(OP_IPUT_OBJECT),                                                    \
        H(OP_IPUT_BOOLEAN),                                                   \
        H(OP_IPUT_BYTE),                                                      \
        H(OP_IPUT_CHAR),                                                      \
        H(OP_IPUT_SHORT),                                                     \
        H(OP_SGET),                                                           \
        H(OP_SGET_WIDE),                                                      \
        H(OP_SGET_OBJECT),                                                    \
        H(OP_SGET_BOOLEAN),                                                   \
        H(OP_SGET_BYTE),                                                      \
        H(OP_SGET_CHAR),                                                      \
        H(OP_SGET_SHORT),                                                     \
        H(OP_SPUT),                                                           \
        H(OP_SPUT_WIDE),                                                      \
        H(OP_SPUT_OBJECT),                                                    \
        H(OP_SPUT_BOOLEAN),                                                   \
        H(OP_SPUT_BYTE),                                                      \
        H(OP_SPUT_CHAR),                                                      \
        H(OP_SPUT_SHORT),                                                     \
        H(OP_INVOKE_VIRTUAL),                                                 \
        H(OP_INVOKE_SUPER),                                                   \
        H(OP_INVOKE_DIRECT),                                                  \
        H(OP_INVOKE_STATIC),                                                  \
        H(OP_INVOKE_INTERFACE),                                               \
        H(OP_UNUSED_73),                                                      \
        H(OP_INVOKE_VIRTUAL_RANGE),                                           \
        H(OP_INVOKE_SUPER_RANGE),                                             \
        H(OP_INVOKE_DIRECT_RANGE),                                            \
        H(OP_INVOKE_STATIC_RANGE),                                            \
        H(OP_INVOKE_INTERFACE_RANGE),                                         \
        H(OP_UNUSED_79),                                                      \
        H(OP_UNUSED_7A),                                                      \
        H(OP_NEG_INT),                                                        \
        H(OP_NOT_INT),                                                        \
        H(OP_NEG_LONG),                                                       \
        H(OP_NOT_LONG),                                                       \
        H(OP_NEG_FLOAT),                                                      \
        H(OP_NEG_DOUBLE),                                                     \
        H(OP_INT_TO_LONG),                                                    \
        H(OP_INT_TO_FLOAT),                                                   \
        H(OP_INT_TO_DOUBLE),                                                  \
        H(OP_LONG_TO_INT),                                                    \
        H(OP_LONG_TO_FLOAT),                                                  \
        H(OP_LONG_TO_DOUBLE),                                                 \
        H(OP_FLOAT_TO_INT),                                                   \
        H(OP_FLOAT_TO_LONG),                                                  \
        H(OP_FLOAT_TO_DOUBLE),                                                \
        H(OP_DOUBLE_TO_INT),                                                  \
        H(OP_DOUBLE_TO_LONG),                                                 \
        H(OP_DOUBLE_TO_FLOAT),                                                \
        H(OP_INT_TO_BYTE),                                                    \
        H(OP_INT_TO_CHAR),                                                    \
        H(OP_INT_TO_SHORT),                                                   \
        H(OP_ADD_INT),                                                        \
        H(OP_SUB_INT),                                                        \
        H(OP_MUL_INT),                                                        \
        H(OP_DIV_INT),                                                        \
        H(OP_REM_INT),                                                        \
        H(OP_AND_INT),                                                        \
        H(OP_OR_INT),                                                         \
        H(OP_XOR_INT),                                                        \
        H(OP_SHL_INT),                                                        \
        H(OP_SHR_INT),                                                        \
        H(OP_USHR_INT),                                                       \
        H(OP_ADD_LONG),                                                       \
        H(OP_SUB_LONG),                                                       \
        H(OP_MUL_LONG),                                                       \
        H(OP_DIV_LONG),                                                       \
        H(OP_REM_LONG),                                                       \
        H(OP_AND_LONG),                                                       \
        H(OP_OR_LONG),                                                        \
        H(OP_XOR_LONG),                                                       \
        H(OP_SHL_LONG),                                                       \
        H(OP_SHR_LONG),                                                       \
        H(OP_USHR_LONG),                                                      \
        H(OP_ADD_FLOAT),                                                      \
        H(OP_SUB_FLOAT),                                                      \
        H(OP_MUL_FLOAT),                                                      \
        H(OP_DIV_FLOAT),                                                      \
        H(OP_REM_FLOAT),                                                      \
        H(OP_ADD_DOUBLE),                                                     \
        H(OP_SUB_DOUBLE),                                                     \
        H(OP_MUL_DOUBLE),                                                     \
        H(OP_DIV_DOUBLE),                                                     \
        H(OP_REM_DOUBLE),                                                     \
        H(OP_ADD_INT_2ADDR),                                                  \
        H(OP_SUB_INT_2ADDR),                                                  \
        H(OP_MUL_INT_2ADDR),                                                  \
        H(OP_DIV_INT_2ADDR),                                                  \
        H(OP_REM_INT_2ADDR),                                                  \
        H(OP_AND_INT_2ADDR),                                                  \
        H(OP_OR_INT_2ADDR),                                                   \
        H(OP_XOR_INT_2ADDR),                                                  \
        H(OP_SHL_INT_2ADDR),                                                  \
        H(OP_SHR_INT_2ADDR),                                                  \
        H(OP_USHR_INT_2ADDR),                                                 \
        H(OP_ADD_LONG_2ADDR),                                                 \
        H(OP_SUB_LONG_2ADDR),                                                 \
        H(OP_MUL_LONG_2ADDR),                                                 \
        H(OP_DIV_LONG_2ADDR),                                                 \
        H(OP_REM_LONG_2ADDR),                                                 \
        H(OP_AND_LONG_2ADDR),                                                 \
        H(OP_OR_LONG_2ADDR),                                                  \
        H(OP_XOR_LONG_2ADDR),                                                 \
        H(OP_SHL_LONG_2ADDR),                                                 \
        H(OP_SHR_LONG_2ADDR),                                                 \
        H(OP_USHR_LONG_2ADDR),                                                \
        H(OP_ADD_FLOAT_2ADDR),                                                \
        H(OP_SUB_FLOAT_2ADDR),                                                \
        H(OP_MUL_FLOAT_2ADDR),                                                \
        H(OP_DIV_FLOAT_2ADDR),                                                \
        H(OP_REM_FLOAT_2ADDR),                                                \
        H(OP_ADD_DOUBLE_2ADDR),                                               \
        H(OP_SUB_DOUBLE_2ADDR),                                               \
        H(OP_MUL_DOUBLE_2ADDR),                                               \
        H(OP_DIV_DOUBLE_2ADDR),                                               \
        H(OP_REM_DOUBLE_2ADDR),                                               \
        H(OP_ADD_INT_LIT16),                                                  \
        H(OP_RSUB_INT),                                                       \
        H(OP_MUL_INT_LIT16),                                                  \
        H(OP_DIV_INT_LIT16),                                                  \
        H(OP_REM_INT_LIT16),                                                  \
        H(OP_AND_INT_LIT16),                                                  \
        H(OP_OR_INT_LIT16),                                                   \
        H(OP_XOR_INT_LIT16),                                                  \
        H(OP_ADD_INT_LIT8),                                                   \
        H(OP_RSUB_INT_LIT8),                                                  \
        H(OP_MUL_INT_LIT8),                                                   \
        H(OP_DIV_INT_LIT8),                                                   \
        H(OP_REM_INT_LIT8),                                                   \
        H(OP_AND_INT_LIT8),                                                   \
        H(OP_OR_INT_LIT8),                                                    \
        H(OP_XOR_INT_LIT8),                                                   \
        H(OP_SHL_INT_LIT8),                                                   \
        H(OP_SHR_INT_LIT8),                                                   \
        H(OP_USHR_INT_LIT8),                                                  \
        H(OP_IGET_VOLATILE),                                                  \
        H(OP_IPUT_VOLATILE),                                                  \
        H(OP_SGET_VOLATILE),                                                  \
        H(OP_SPUT_VOLATILE),                                                  \
        H(OP_IGET_OBJECT_VOLATILE),                                           \
        H(OP_IGET_WIDE_VOLATILE),                                             \
        H(OP_IPUT_WIDE_VOLATILE),                                             \
        H(OP_SGET_WIDE_VOLATILE),                                             \
        H(OP_SPUT_WIDE_VOLATILE),                                             \
        H(OP_BREAKPOINT),                                                     \
        H(OP_THROW_VERIFICATION_ERROR),                                       \
        H(OP_EXECUTE_INLINE),                                                 \
        H(OP_EXECUTE_INLINE_RANGE),                                           \
        H(OP_INVOKE_OBJECT_INIT_RANGE),                                       \
        H(OP_RETURN_VOID_BARRIER),                                            \
        H(OP_IGET_QUICK),                                                     \
        H(OP_IGET_WIDE_QUICK),                                                \
        H(OP_IGET_OBJECT_QUICK),                                              \
        H(OP_IPUT_QUICK),                                                     \
        H(OP_IPUT_WIDE_QUICK),                                                \
        H(OP_IPUT_OBJECT_QUICK),                                              \
        H(OP_INVOKE_VIRTUAL_QUICK),                                           \
        H(OP_INVOKE_VIRTUAL_QUICK_RANGE),                                     \
        H(OP_INVOKE_SUPER_QUICK),                                             \
        H(OP_INVOKE_SUPER_QUICK_RANGE),                                       \
        H(OP_IPUT_OBJECT_VOLATILE),                                           \
        H(OP_SGET_OBJECT_VOLATILE),                                           \
        H(OP_SPUT_OBJECT_VOLATILE),                                           \
        H(OP_UNUSED_FF),                                                      \
        /* END(libdex-goto-table) */                                          \
    };
#endif



#if SDKVERSION <2
enum Opcode {//根据指令得到相应的Label地址

    OP_NOP                          = 0x5,
    OP_MOVE                         = 0x6,
    OP_MOVE_FROM16                  = 0x7,
    OP_MOVE_16                      = 0x8,
    OP_MOVE_WIDE                    = 0x9,
    OP_MOVE_WIDE_FROM16             = 0xa,
    OP_MOVE_WIDE_16                 = 0xb,
    OP_MOVE_OBJECT                  = 0xc,
    OP_MOVE_OBJECT_FROM16           = 0xd,
    OP_MOVE_OBJECT_16               = 0xe,
    OP_MOVE_RESULT                  = 0xf,
    OP_MOVE_RESULT_WIDE             = 0x10,
    OP_MOVE_RESULT_OBJECT           = 0x11,
    OP_MOVE_EXCEPTION               = 0x12,
    OP_RETURN_VOID                  = 0x13,
    OP_RETURN                       = 0x14,
    OP_RETURN_WIDE                  = 0x15,
    OP_RETURN_OBJECT                = 0x16,
    OP_CONST_4                      = 0x17,
    OP_CONST_16                     = 0x18,
    OP_CONST                        = 0x19,
    OP_CONST_HIGH16                 = 0x1a,
    OP_CONST_WIDE_16                = 0x1b,
    OP_CONST_WIDE_32                = 0x1c,
    OP_CONST_WIDE                   = 0x1d,
    OP_CONST_WIDE_HIGH16            = 0x1e,
    OP_CONST_STRING                 = 0x1f,
    OP_CONST_STRING_JUMBO           = 0x20,
    OP_CONST_CLASS                  = 0x21,
    OP_MONITOR_ENTER                = 0x22,
    OP_MONITOR_EXIT                 = 0x23,
    OP_CHECK_CAST                   = 0x24,
    OP_INSTANCE_OF                  = 0x25,
    OP_ARRAY_LENGTH                 = 0x26,
    OP_NEW_INSTANCE                 = 0x27,
    OP_NEW_ARRAY                    = 0x28,
    OP_FILLED_NEW_ARRAY             = 0x29,
    OP_FILLED_NEW_ARRAY_RANGE       = 0x2a,
    OP_FILL_ARRAY_DATA              = 0x2b,
    OP_THROW                        = 0x2c,
    OP_GOTO                         = 0x2d,
    OP_GOTO_16                      = 0x2e,
    OP_GOTO_32                      = 0x2f,
    OP_PACKED_SWITCH                = 0x30,
    OP_SPARSE_SWITCH                = 0x31,
    OP_CMPL_FLOAT                   = 0x32,
    OP_CMPG_FLOAT                   = 0x33,
    OP_CMPL_DOUBLE                  = 0x34,
    OP_CMPG_DOUBLE                  = 0x35,
    OP_CMP_LONG                     = 0x36,
    OP_IF_EQ                        = 0x37,
    OP_IF_NE                        = 0x38,
    OP_IF_LT                        = 0x39,
    OP_IF_GE                        = 0x3a,
    OP_IF_GT                        = 0x3b,
    OP_IF_LE                        = 0x3c,
    OP_IF_EQZ                       = 0x3d,
    OP_IF_NEZ                       = 0x3e,
    OP_IF_LTZ                       = 0x3f,
    OP_IF_GEZ                       = 0x40,
    OP_IF_GTZ                       = 0x41,
    OP_IF_LEZ                       = 0x42,
    OP_UNUSED_3E                    = 0x43,
    OP_UNUSED_3F                    = 0x44,
    OP_UNUSED_40                    = 0x45,
    OP_UNUSED_41                    = 0x46,
    OP_UNUSED_42                    = 0x47,
    OP_UNUSED_43                    = 0x48,
    OP_AGET                         = 0x49,
    OP_AGET_WIDE                    = 0x4a,
    OP_AGET_OBJECT                  = 0x4b,
    OP_AGET_BOOLEAN                 = 0x4c,
    OP_AGET_BYTE                    = 0x4d,
    OP_AGET_CHAR                    = 0x4e,
    OP_AGET_SHORT                   = 0x4f,
    OP_APUT                         = 0x50,
    OP_APUT_WIDE                    = 0x51,
    OP_APUT_OBJECT                  = 0x52,
    OP_APUT_BOOLEAN                 = 0x53,
    OP_APUT_BYTE                    = 0x54,
    OP_APUT_CHAR                    = 0x55,
    OP_APUT_SHORT                   = 0x56,
    OP_IGET                         = 0x57,
    OP_IGET_WIDE                    = 0x58,
    OP_IGET_OBJECT                  = 0x59,
    OP_IGET_BOOLEAN                 = 0x5a,
    OP_IGET_BYTE                    = 0x5b,
    OP_IGET_CHAR                    = 0x5c,
    OP_IGET_SHORT                   = 0x5d,
    OP_IPUT                         = 0x5e,
    OP_IPUT_WIDE                    = 0x5f,
    OP_IPUT_OBJECT                  = 0x60,
    OP_IPUT_BOOLEAN                 = 0x61,
    OP_IPUT_BYTE                    = 0x62,
    OP_IPUT_CHAR                    = 0x63,
    OP_IPUT_SHORT                   = 0x64,
    OP_SGET                         = 0x65,
    OP_SGET_WIDE                    = 0x66,
    OP_SGET_OBJECT                  = 0x67,
    OP_SGET_BOOLEAN                 = 0x68,
    OP_SGET_BYTE                    = 0x69,
    OP_SGET_CHAR                    = 0x6a,
    OP_SGET_SHORT                   = 0x6b,
    OP_SPUT                         = 0x6c,
    OP_SPUT_WIDE                    = 0x6d,
    OP_SPUT_OBJECT                  = 0x6e,
    OP_SPUT_BOOLEAN                 = 0x6f,
    OP_SPUT_BYTE                    = 0x70,
    OP_SPUT_CHAR                    = 0x71,
    OP_SPUT_SHORT                   = 0x72,
    OP_INVOKE_VIRTUAL               = 0x73,
    OP_INVOKE_SUPER                 = 0x74,
    OP_INVOKE_DIRECT                = 0x75,
    OP_INVOKE_STATIC                = 0x76,
    OP_INVOKE_INTERFACE             = 0x77,
    OP_UNUSED_73                    = 0x78,
    OP_INVOKE_VIRTUAL_RANGE         = 0x79,
    OP_INVOKE_SUPER_RANGE           = 0x7a,
    OP_INVOKE_DIRECT_RANGE          = 0x7b,
    OP_INVOKE_STATIC_RANGE          = 0x7c,
    OP_INVOKE_INTERFACE_RANGE       = 0x7d,
    OP_UNUSED_79                    = 0x7e,
    OP_UNUSED_7A                    = 0x7f,
    OP_NEG_INT                      = 0x80,
    OP_NOT_INT                      = 0x81,
    OP_NEG_LONG                     = 0x82,
    OP_NOT_LONG                     = 0x83,
    OP_NEG_FLOAT                    = 0x84,
    OP_NEG_DOUBLE                   = 0x85,
    OP_INT_TO_LONG                  = 0x86,
    OP_INT_TO_FLOAT                 = 0x87,
    OP_INT_TO_DOUBLE                = 0x88,
    OP_LONG_TO_INT                  = 0x89,
    OP_LONG_TO_FLOAT                = 0x8a,
    OP_LONG_TO_DOUBLE               = 0x8b,
    OP_FLOAT_TO_INT                 = 0x8c,
    OP_FLOAT_TO_LONG                = 0x8d,
    OP_FLOAT_TO_DOUBLE              = 0x8e,
    OP_DOUBLE_TO_INT                = 0x8f,
    OP_DOUBLE_TO_LONG               = 0x90,
    OP_DOUBLE_TO_FLOAT              = 0x91,
    OP_INT_TO_BYTE                  = 0x92,
    OP_INT_TO_CHAR                  = 0x93,
    OP_INT_TO_SHORT                 = 0x94,
    OP_ADD_INT                      = 0x95,
    OP_SUB_INT                      = 0x96,
    OP_MUL_INT                      = 0x97,
    OP_DIV_INT                      = 0x98,
    OP_REM_INT                      = 0x99,
    OP_AND_INT                      = 0x9a,
    OP_OR_INT                       = 0x9b,
    OP_XOR_INT                      = 0x9c,
    OP_SHL_INT                      = 0x9d,
    OP_SHR_INT                      = 0x9e,
    OP_USHR_INT                     = 0x9f,
    OP_ADD_LONG                     = 0xa0,
    OP_SUB_LONG                     = 0xa1,
    OP_MUL_LONG                     = 0xa2,
    OP_DIV_LONG                     = 0xa3,
    OP_REM_LONG                     = 0xa4,
    OP_AND_LONG                     = 0xa5,
    OP_OR_LONG                      = 0xa6,
    OP_XOR_LONG                     = 0xa7,
    OP_SHL_LONG                     = 0xa8,
    OP_SHR_LONG                     = 0xa9,
    OP_USHR_LONG                    = 0xaa,
    OP_ADD_FLOAT                    = 0xab,
    OP_SUB_FLOAT                    = 0xac,
    OP_MUL_FLOAT                    = 0xad,
    OP_DIV_FLOAT                    = 0xae,
    OP_REM_FLOAT                    = 0xaf,
    OP_ADD_DOUBLE                   = 0xb0,
    OP_SUB_DOUBLE                   = 0xb1,
    OP_MUL_DOUBLE                   = 0xb2,
    OP_DIV_DOUBLE                   = 0xb3,
    OP_REM_DOUBLE                   = 0xb4,
    OP_ADD_INT_2ADDR                = 0xb5,
    OP_SUB_INT_2ADDR                = 0xb6,
    OP_MUL_INT_2ADDR                = 0xb7,
    OP_DIV_INT_2ADDR                = 0xb8,
    OP_REM_INT_2ADDR                = 0xb9,
    OP_AND_INT_2ADDR                = 0xba,
    OP_OR_INT_2ADDR                 = 0xbb,
    OP_XOR_INT_2ADDR                = 0xbc,
    OP_SHL_INT_2ADDR                = 0xbd,
    OP_SHR_INT_2ADDR                = 0xbe,
    OP_USHR_INT_2ADDR               = 0xbf,
    OP_ADD_LONG_2ADDR               = 0xc0,
    OP_SUB_LONG_2ADDR               = 0xc1,
    OP_MUL_LONG_2ADDR               = 0xc2,
    OP_DIV_LONG_2ADDR               = 0xc3,
    OP_REM_LONG_2ADDR               = 0xc4,
    OP_AND_LONG_2ADDR               = 0xc5,
    OP_OR_LONG_2ADDR                = 0xc6,
    OP_XOR_LONG_2ADDR               = 0xc7,
    OP_SHL_LONG_2ADDR               = 0xc8,
    OP_SHR_LONG_2ADDR               = 0xc9,
    OP_USHR_LONG_2ADDR              = 0xca,
    OP_ADD_FLOAT_2ADDR              = 0xcb,
    OP_SUB_FLOAT_2ADDR              = 0xcc,
    OP_MUL_FLOAT_2ADDR              = 0xcd,
    OP_DIV_FLOAT_2ADDR              = 0xce,
    OP_REM_FLOAT_2ADDR              = 0xcf,
    OP_ADD_DOUBLE_2ADDR             = 0xd0,
    OP_SUB_DOUBLE_2ADDR             = 0xd1,
    OP_MUL_DOUBLE_2ADDR             = 0xd2,
    OP_DIV_DOUBLE_2ADDR             = 0xd3,
    OP_REM_DOUBLE_2ADDR             = 0xd4,
    OP_ADD_INT_LIT16                = 0xd5,
    OP_RSUB_INT                     = 0xd6,
    OP_MUL_INT_LIT16                = 0xd7,
    OP_DIV_INT_LIT16                = 0xd8,
    OP_REM_INT_LIT16                = 0xd9,
    OP_AND_INT_LIT16                = 0xda,
    OP_OR_INT_LIT16                 = 0xdb,
    OP_XOR_INT_LIT16                = 0xdc,
    OP_ADD_INT_LIT8                 = 0xdd,
    OP_RSUB_INT_LIT8                = 0xde,
    OP_MUL_INT_LIT8                 = 0xdf,
    OP_DIV_INT_LIT8                 = 0xe0,
    OP_REM_INT_LIT8                 = 0xe1,
    OP_AND_INT_LIT8                 = 0xe2,
    OP_OR_INT_LIT8                  = 0xe3,
    OP_XOR_INT_LIT8                 = 0xe4,
    OP_SHL_INT_LIT8                 = 0xe5,
    OP_SHR_INT_LIT8                 = 0xe6,
    OP_USHR_INT_LIT8                = 0xe7,
    OP_IGET_VOLATILE                = 0xe8,
    OP_IPUT_VOLATILE                = 0xe9,
    OP_SGET_VOLATILE                = 0xea,
    OP_SPUT_VOLATILE                = 0xeb,
    OP_IGET_OBJECT_VOLATILE         = 0xec,
    OP_IGET_WIDE_VOLATILE           = 0xed,
    OP_IPUT_WIDE_VOLATILE           = 0xee,
    OP_SGET_WIDE_VOLATILE           = 0xef,
    OP_SPUT_WIDE_VOLATILE           = 0xf0,
    OP_BREAKPOINT                   = 0xf1,
    OP_THROW_VERIFICATION_ERROR     = 0xf2,
    OP_EXECUTE_INLINE               = 0xf3,
    OP_EXECUTE_INLINE_RANGE         = 0xf4,
    OP_INVOKE_OBJECT_INIT_RANGE     = 0xf5,
    OP_RETURN_VOID_BARRIER          = 0xf6,
    OP_IGET_QUICK                   = 0xf7,
    OP_IGET_WIDE_QUICK              = 0xf8,
    OP_IGET_OBJECT_QUICK            = 0xf9,
    OP_IPUT_QUICK                   = 0xfa,
    OP_IPUT_WIDE_QUICK              = 0xfb,
    OP_IPUT_OBJECT_QUICK            = 0xfc,
    OP_INVOKE_VIRTUAL_QUICK         = 0xfd,
    OP_INVOKE_VIRTUAL_QUICK_RANGE   = 0xfe,
    OP_INVOKE_SUPER_QUICK           = 0xff,
    OP_INVOKE_SUPER_QUICK_RANGE     = 0x0,
    OP_IPUT_OBJECT_VOLATILE         = 0x1,
    OP_SGET_OBJECT_VOLATILE         = 0x2,
    OP_SPUT_OBJECT_VOLATILE         = 0x3,
    OP_UNUSED_FF                    = 0x4,
    // END(libdex-opcode-enum)
};

#define DEFINE_GOTO_TABLE(_name) \
    static const void* _name[kNumPackedOpcodes] = {                      \
        /* BEGIN(libdex-goto-table); GENERATED AUTOMATICALLY BY opcode-gen */ \
        H(OP_INVOKE_SUPER_QUICK_RANGE),                                       \
        H(OP_IPUT_OBJECT_VOLATILE),                                           \
        H(OP_SGET_OBJECT_VOLATILE),                                           \
        H(OP_SPUT_OBJECT_VOLATILE),                                           \
        H(OP_UNUSED_FF),                                                      \
        H(OP_NOP),                                                            \
        H(OP_MOVE),                                                           \
        H(OP_MOVE_FROM16),                                                    \
        H(OP_MOVE_16),                                                        \
        H(OP_MOVE_WIDE),                                                      \
        H(OP_MOVE_WIDE_FROM16),                                               \
        H(OP_MOVE_WIDE_16),                                                   \
        H(OP_MOVE_OBJECT),                                                    \
        H(OP_MOVE_OBJECT_FROM16),                                             \
        H(OP_MOVE_OBJECT_16),                                                 \
        H(OP_MOVE_RESULT),                                                    \
        H(OP_MOVE_RESULT_WIDE),                                               \
        H(OP_MOVE_RESULT_OBJECT),                                             \
        H(OP_MOVE_EXCEPTION),                                                 \
        H(OP_RETURN_VOID),                                                    \
        H(OP_RETURN),                                                         \
        H(OP_RETURN_WIDE),                                                    \
        H(OP_RETURN_OBJECT),                                                  \
        H(OP_CONST_4),                                                        \
        H(OP_CONST_16),                                                       \
        H(OP_CONST),                                                          \
        H(OP_CONST_HIGH16),                                                   \
        H(OP_CONST_WIDE_16),                                                  \
        H(OP_CONST_WIDE_32),                                                  \
        H(OP_CONST_WIDE),                                                     \
        H(OP_CONST_WIDE_HIGH16),                                              \
        H(OP_CONST_STRING),                                                   \
        H(OP_CONST_STRING_JUMBO),                                             \
        H(OP_CONST_CLASS),                                                    \
        H(OP_MONITOR_ENTER),                                                  \
        H(OP_MONITOR_EXIT),                                                   \
        H(OP_CHECK_CAST),                                                     \
        H(OP_INSTANCE_OF),                                                    \
        H(OP_ARRAY_LENGTH),                                                   \
        H(OP_NEW_INSTANCE),                                                   \
        H(OP_NEW_ARRAY),                                                      \
        H(OP_FILLED_NEW_ARRAY),                                               \
        H(OP_FILLED_NEW_ARRAY_RANGE),                                         \
        H(OP_FILL_ARRAY_DATA),                                                \
        H(OP_THROW),                                                          \
        H(OP_GOTO),                                                           \
        H(OP_GOTO_16),                                                        \
        H(OP_GOTO_32),                                                        \
        H(OP_PACKED_SWITCH),                                                  \
        H(OP_SPARSE_SWITCH),                                                  \
        H(OP_CMPL_FLOAT),                                                     \
        H(OP_CMPG_FLOAT),                                                     \
        H(OP_CMPL_DOUBLE),                                                    \
        H(OP_CMPG_DOUBLE),                                                    \
        H(OP_CMP_LONG),                                                       \
        H(OP_IF_EQ),                                                          \
        H(OP_IF_NE),                                                          \
        H(OP_IF_LT),                                                          \
        H(OP_IF_GE),                                                          \
        H(OP_IF_GT),                                                          \
        H(OP_IF_LE),                                                          \
        H(OP_IF_EQZ),                                                         \
        H(OP_IF_NEZ),                                                         \
        H(OP_IF_LTZ),                                                         \
        H(OP_IF_GEZ),                                                         \
        H(OP_IF_GTZ),                                                         \
        H(OP_IF_LEZ),                                                         \
        H(OP_UNUSED_3E),                                                      \
        H(OP_UNUSED_3F),                                                      \
        H(OP_UNUSED_40),                                                      \
        H(OP_UNUSED_41),                                                      \
        H(OP_UNUSED_42),                                                      \
        H(OP_UNUSED_43),                                                      \
        H(OP_AGET),                                                           \
        H(OP_AGET_WIDE),                                                      \
        H(OP_AGET_OBJECT),                                                    \
        H(OP_AGET_BOOLEAN),                                                   \
        H(OP_AGET_BYTE),                                                      \
        H(OP_AGET_CHAR),                                                      \
        H(OP_AGET_SHORT),                                                     \
        H(OP_APUT),                                                           \
        H(OP_APUT_WIDE),                                                      \
        H(OP_APUT_OBJECT),                                                    \
        H(OP_APUT_BOOLEAN),                                                   \
        H(OP_APUT_BYTE),                                                      \
        H(OP_APUT_CHAR),                                                      \
        H(OP_APUT_SHORT),                                                     \
        H(OP_IGET),                                                           \
        H(OP_IGET_WIDE),                                                      \
        H(OP_IGET_OBJECT),                                                    \
        H(OP_IGET_BOOLEAN),                                                   \
        H(OP_IGET_BYTE),                                                      \
        H(OP_IGET_CHAR),                                                      \
        H(OP_IGET_SHORT),                                                     \
        H(OP_IPUT),                                                           \
        H(OP_IPUT_WIDE),                                                      \
        H(OP_IPUT_OBJECT),                                                    \
        H(OP_IPUT_BOOLEAN),                                                   \
        H(OP_IPUT_BYTE),                                                      \
        H(OP_IPUT_CHAR),                                                      \
        H(OP_IPUT_SHORT),                                                     \
        H(OP_SGET),                                                           \
        H(OP_SGET_WIDE),                                                      \
        H(OP_SGET_OBJECT),                                                    \
        H(OP_SGET_BOOLEAN),                                                   \
        H(OP_SGET_BYTE),                                                      \
        H(OP_SGET_CHAR),                                                      \
        H(OP_SGET_SHORT),                                                     \
        H(OP_SPUT),                                                           \
        H(OP_SPUT_WIDE),                                                      \
        H(OP_SPUT_OBJECT),                                                    \
        H(OP_SPUT_BOOLEAN),                                                   \
        H(OP_SPUT_BYTE),                                                      \
        H(OP_SPUT_CHAR),                                                      \
        H(OP_SPUT_SHORT),                                                     \
        H(OP_INVOKE_VIRTUAL),                                                 \
        H(OP_INVOKE_SUPER),                                                   \
        H(OP_INVOKE_DIRECT),                                                  \
        H(OP_INVOKE_STATIC),                                                  \
        H(OP_INVOKE_INTERFACE),                                               \
        H(OP_UNUSED_73),                                                      \
        H(OP_INVOKE_VIRTUAL_RANGE),                                           \
        H(OP_INVOKE_SUPER_RANGE),                                             \
        H(OP_INVOKE_DIRECT_RANGE),                                            \
        H(OP_INVOKE_STATIC_RANGE),                                            \
        H(OP_INVOKE_INTERFACE_RANGE),                                         \
        H(OP_UNUSED_79),                                                      \
        H(OP_UNUSED_7A),                                                      \
        H(OP_NEG_INT),                                                        \
        H(OP_NOT_INT),                                                        \
        H(OP_NEG_LONG),                                                       \
        H(OP_NOT_LONG),                                                       \
        H(OP_NEG_FLOAT),                                                      \
        H(OP_NEG_DOUBLE),                                                     \
        H(OP_INT_TO_LONG),                                                    \
        H(OP_INT_TO_FLOAT),                                                   \
        H(OP_INT_TO_DOUBLE),                                                  \
        H(OP_LONG_TO_INT),                                                    \
        H(OP_LONG_TO_FLOAT),                                                  \
        H(OP_LONG_TO_DOUBLE),                                                 \
        H(OP_FLOAT_TO_INT),                                                   \
        H(OP_FLOAT_TO_LONG),                                                  \
        H(OP_FLOAT_TO_DOUBLE),                                                \
        H(OP_DOUBLE_TO_INT),                                                  \
        H(OP_DOUBLE_TO_LONG),                                                 \
        H(OP_DOUBLE_TO_FLOAT),                                                \
        H(OP_INT_TO_BYTE),                                                    \
        H(OP_INT_TO_CHAR),                                                    \
        H(OP_INT_TO_SHORT),                                                   \
        H(OP_ADD_INT),                                                        \
        H(OP_SUB_INT),                                                        \
        H(OP_MUL_INT),                                                        \
        H(OP_DIV_INT),                                                        \
        H(OP_REM_INT),                                                        \
        H(OP_AND_INT),                                                        \
        H(OP_OR_INT),                                                         \
        H(OP_XOR_INT),                                                        \
        H(OP_SHL_INT),                                                        \
        H(OP_SHR_INT),                                                        \
        H(OP_USHR_INT),                                                       \
        H(OP_ADD_LONG),                                                       \
        H(OP_SUB_LONG),                                                       \
        H(OP_MUL_LONG),                                                       \
        H(OP_DIV_LONG),                                                       \
        H(OP_REM_LONG),                                                       \
        H(OP_AND_LONG),                                                       \
        H(OP_OR_LONG),                                                        \
        H(OP_XOR_LONG),                                                       \
        H(OP_SHL_LONG),                                                       \
        H(OP_SHR_LONG),                                                       \
        H(OP_USHR_LONG),                                                      \
        H(OP_ADD_FLOAT),                                                      \
        H(OP_SUB_FLOAT),                                                      \
        H(OP_MUL_FLOAT),                                                      \
        H(OP_DIV_FLOAT),                                                      \
        H(OP_REM_FLOAT),                                                      \
        H(OP_ADD_DOUBLE),                                                     \
        H(OP_SUB_DOUBLE),                                                     \
        H(OP_MUL_DOUBLE),                                                     \
        H(OP_DIV_DOUBLE),                                                     \
        H(OP_REM_DOUBLE),                                                     \
        H(OP_ADD_INT_2ADDR),                                                  \
        H(OP_SUB_INT_2ADDR),                                                  \
        H(OP_MUL_INT_2ADDR),                                                  \
        H(OP_DIV_INT_2ADDR),                                                  \
        H(OP_REM_INT_2ADDR),                                                  \
        H(OP_AND_INT_2ADDR),                                                  \
        H(OP_OR_INT_2ADDR),                                                   \
        H(OP_XOR_INT_2ADDR),                                                  \
        H(OP_SHL_INT_2ADDR),                                                  \
        H(OP_SHR_INT_2ADDR),                                                  \
        H(OP_USHR_INT_2ADDR),                                                 \
        H(OP_ADD_LONG_2ADDR),                                                 \
        H(OP_SUB_LONG_2ADDR),                                                 \
        H(OP_MUL_LONG_2ADDR),                                                 \
        H(OP_DIV_LONG_2ADDR),                                                 \
        H(OP_REM_LONG_2ADDR),                                                 \
        H(OP_AND_LONG_2ADDR),                                                 \
        H(OP_OR_LONG_2ADDR),                                                  \
        H(OP_XOR_LONG_2ADDR),                                                 \
        H(OP_SHL_LONG_2ADDR),                                                 \
        H(OP_SHR_LONG_2ADDR),                                                 \
        H(OP_USHR_LONG_2ADDR),                                                \
        H(OP_ADD_FLOAT_2ADDR),                                                \
        H(OP_SUB_FLOAT_2ADDR),                                                \
        H(OP_MUL_FLOAT_2ADDR),                                                \
        H(OP_DIV_FLOAT_2ADDR),                                                \
        H(OP_REM_FLOAT_2ADDR),                                                \
        H(OP_ADD_DOUBLE_2ADDR),                                               \
        H(OP_SUB_DOUBLE_2ADDR),                                               \
        H(OP_MUL_DOUBLE_2ADDR),                                               \
        H(OP_DIV_DOUBLE_2ADDR),                                               \
        H(OP_REM_DOUBLE_2ADDR),                                               \
        H(OP_ADD_INT_LIT16),                                                  \
        H(OP_RSUB_INT),                                                       \
        H(OP_MUL_INT_LIT16),                                                  \
        H(OP_DIV_INT_LIT16),                                                  \
        H(OP_REM_INT_LIT16),                                                  \
        H(OP_AND_INT_LIT16),                                                  \
        H(OP_OR_INT_LIT16),                                                   \
        H(OP_XOR_INT_LIT16),                                                  \
        H(OP_ADD_INT_LIT8),                                                   \
        H(OP_RSUB_INT_LIT8),                                                  \
        H(OP_MUL_INT_LIT8),                                                   \
        H(OP_DIV_INT_LIT8),                                                   \
        H(OP_REM_INT_LIT8),                                                   \
        H(OP_AND_INT_LIT8),                                                   \
        H(OP_OR_INT_LIT8),                                                    \
        H(OP_XOR_INT_LIT8),                                                   \
        H(OP_SHL_INT_LIT8),                                                   \
        H(OP_SHR_INT_LIT8),                                                   \
        H(OP_USHR_INT_LIT8),                                                  \
        H(OP_IGET_VOLATILE),                                                  \
        H(OP_IPUT_VOLATILE),                                                  \
        H(OP_SGET_VOLATILE),                                                  \
        H(OP_SPUT_VOLATILE),                                                  \
        H(OP_IGET_OBJECT_VOLATILE),                                           \
        H(OP_IGET_WIDE_VOLATILE),                                             \
        H(OP_IPUT_WIDE_VOLATILE),                                             \
        H(OP_SGET_WIDE_VOLATILE),                                             \
        H(OP_SPUT_WIDE_VOLATILE),                                             \
        H(OP_BREAKPOINT),                                                     \
        H(OP_THROW_VERIFICATION_ERROR),                                       \
        H(OP_EXECUTE_INLINE),                                                 \
        H(OP_EXECUTE_INLINE_RANGE),                                           \
        H(OP_INVOKE_OBJECT_INIT_RANGE),                                       \
        H(OP_RETURN_VOID_BARRIER),                                            \
        H(OP_IGET_QUICK),                                                     \
        H(OP_IGET_WIDE_QUICK),                                                \
        H(OP_IGET_OBJECT_QUICK),                                              \
        H(OP_IPUT_QUICK),                                                     \
        H(OP_IPUT_WIDE_QUICK),                                                \
        H(OP_IPUT_OBJECT_QUICK),                                              \
        H(OP_INVOKE_VIRTUAL_QUICK),                                           \
        H(OP_INVOKE_VIRTUAL_QUICK_RANGE),                                     \
        H(OP_INVOKE_SUPER_QUICK),                                             \
        /* END(libdex-goto-table) */                                          \
    };

#endif
/*
 * Macro used to generate a computed goto table for use in implementing
 * an interpreter in C.
 */
//kNumPackedOpcodes是0x100

/*
 * Return the Opcode for a given raw opcode code unit (which may
 * include data payload). The packed index is a zero-based index which
 * can be used to point into various opcode-related tables. The Dalvik
 * opcode space is inherently sparse, in that the opcode unit is 16
 * bits wide, but for most opcodes, eight of those bits are for data.
 */
/* DEX_INLINE */ Opcode dexOpcodeFromCodeUnit(u2 codeUnit);

/*
 * Return the name of an opcode.
 */
const char* dexGetOpcodeName(Opcode op);

#endif  // LIBDEX_DEXOPCODES_H_
