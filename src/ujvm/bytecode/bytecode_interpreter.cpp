#include <iostream>
#include <ujvm/runtime/instance_class.h>
#include "bytecode_interpreter.h"
#include "opcode_executor.h"

void BytecodeInterpreter::interpret(Method *method, JavaThread *javaThread) {
    CodeReader &codeReader = method->getCodeReader();
    std::cout << ">>>>>>>>>>>>>>>>>" << std::endl;
    u4 &pc = javaThread->pc_;

    while (!codeReader.ended(pc)) {
        u1 bc = codeReader.readU1(pc);
        switch (bc) {
            case 0x03: { // iconst_0
                OpcodeExecutor::Op_iconst0(javaThread, method->getCp());
                break;
            }
            case 0x10: { // bipush
                OpcodeExecutor::Op_bipush(javaThread, method->getCp());
                break;
            }
            case 0x1A : { // iload_0
                OpcodeExecutor::Op_iload0(javaThread, method->getCp());
                break;
            }
            case 0xBB: { //new
                OpcodeExecutor::Op_new(javaThread, method->getCp());
                break;
            }
            case 0x12: { // ldc
                OpcodeExecutor::Op_ldc(javaThread, method->getCp());
                break;
            }
            case 0x2A: { // aload_0

                OpcodeExecutor::Op_aload0(javaThread, method->getCp());
                break;
            }
            case 0x2B: { // aload_1

                OpcodeExecutor::Op_aload1(javaThread, method->getCp());
                break;
            }

            case 0xB1: { // return
                OpcodeExecutor::Op_return(javaThread, method->getCp());

                break;
            }
            case 0xB3: {// putstatic
                OpcodeExecutor::Op_putstatic(javaThread, method->getCp());
                break;
            }
            case 0xB6: { // invokevirtual
                OpcodeExecutor::Op_invokeVirtual(javaThread, method->getCp());
                break;
            }
            case 0xB7: { // invokespecial
                OpcodeExecutor::Op_invokeSpecial(javaThread, method->getCp());
                break;
            }
            case 0xB8: { // invokestatic
                OpcodeExecutor::Op_invokeStatic(javaThread, method->getCp());
                break;
            }

            case 0xB2: { // getstatic
                OpcodeExecutor::Op_getStatic(javaThread, method->getCp());
                break;
            }
            case 0x59:
                OpcodeExecutor::Op_dup(javaThread, method->getCp());
                break;
            default:
                PANIC("bytecode not implemented....%d", bc);
        }
    }
}
