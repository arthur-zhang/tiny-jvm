#include <iostream>
#include <ujvm/runtime/instance_class.h>
#include "bytecode_interpreter.h"
#include "opcode_executor.h"

void BytecodeInterpreter::run(Method *method, JavaThread *javaThread) {
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
            case 0x10: { // bipush
                OpcodeExecutor::Op_bipush(codeIdx, codeAttr->code, javaThread, method->getCp());
                break;
            }
            case 0x1A : { // iload_0
                OpcodeExecutor::Op_iload0(codeIdx, codeAttr->code, javaThread, method->getCp());
                break;
            }
            case 0xBB: { //new
                OpcodeExecutor::Op_new(codeIdx, codeAttr->code, javaThread, method->getCp());
                break;
            }
            case 0x12: { // ldc
                OpcodeExecutor::Op_ldc(codeIdx, codeAttr->code, javaThread, method->getCp());
                break;
            }
            case 0x2A: { // aload_0

                OpcodeExecutor::Op_aload0(codeIdx, codeAttr->code, javaThread, method->getCp());
                break;
            }
            case 0x2B: { // aload_1

                OpcodeExecutor::Op_aload1(codeIdx, codeAttr->code, javaThread, method->getCp());
                break;
            }

            case 0xB1: { // return
                OpcodeExecutor::Op_return(codeIdx, codeAttr->code, javaThread, method->getCp());

                break;
            }
            case 0xB3: {// putstatic
                OpcodeExecutor::Op_putstatic(codeIdx, codeAttr->code, javaThread, method->getCp());
                break;
            }
            case 0xB6: { // invokevirtual
                OpcodeExecutor::Op_invokeVirtual(codeIdx, codeAttr->code, javaThread, method->getCp());
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
                PANIC("bytecode not implemented....%d", bc);
        }
    }
}
