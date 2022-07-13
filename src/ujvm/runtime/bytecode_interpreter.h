#pragma once

#include <iostream>
#include "instance_class.h"
#include "opcode_executor.h"

class BytecodeInterpreter {
public:
    static CodeAttribute *getCode(ConstantPool *constantPool, MethodInfo *methodInfo) {
        for (int i = 0; i < methodInfo->attributes_count; i++) {
            cout << methodInfo->attributes[i]->attribute_name_index << endl;
            auto idx = methodInfo->attributes[i]->attribute_name_index;
            u2 tag = AttributeInfo::attributeName2Tag(idx, constantPool);
            if (tag != ATTRIBUTE_Code) continue;
            return dynamic_cast<CodeAttribute *>(methodInfo->attributes[i]);
        }
        return nullptr;
    }

    static void
    run(InstanceClassStruct *instanceClassStruct, Method *method, JavaThread *javaThread) {
        auto codeAttr = method->getCode();
        auto codeLen = codeAttr->code_length;
        std::cout << ">>>>>>>>>>>>>>>>>" << std::endl;
        int codeIdx = 0;
        while (codeIdx < codeLen) {
            u1 bc = codeAttr->code[codeIdx];
            codeIdx++;
            switch (bc) {
                case 0x03: { // iconst_0
                    OpcodeExecutor::Op_iconst0(codeIdx, codeAttr->code, javaThread, method->getCp());
                    break;
                }
                case 0xBB: { //new
                    OpcodeExecutor::Op_new(codeIdx, codeAttr->code, javaThread, method->getCp());
                    break;
                }
                case 0x12: { // ldc
                    OpcodeExecutor::Op_dup(codeIdx, codeAttr->code, javaThread, method->getCp());
                    break;
                }
                case 0xB6: { // invokevirtual
                    break;
                }
                case 0xB7: { // invokespecial
                    OpcodeExecutor::Op_invokeSpecial(codeIdx, codeAttr->code, javaThread, method->getCp());
                    break;
                }
                case 0xB8: { // invokestatic
                    OpcodeExecutor::Op_invokeStatic(codeIdx, codeAttr->code, javaThread, method->getCp());
                    break;
                }

                case 0xB2: { // getstatic
                    OpcodeExecutor::Op_getStatic(codeIdx, codeAttr->code, javaThread, method->getCp());
                    break;
                }
                case 0x59:
                    OpcodeExecutor::Op_dup(codeIdx, codeAttr->code, javaThread, method->getCp());
                    break;
                default:
                    break;
            }
        }
    }
};