#include <iostream>
#include <ujvm/runtime/instance_class.h>
#include "bytecode_interpreter.h"
#include "opcode_executor.h"

OOP BytecodeInterpreter::interpret(JavaThread *javaThread) {
    Method *method = javaThread->currentFrame()->getMethod();
    CodeReader &codeReader = method->getCodeReader();
//    std::wcout << ">>>> " << method->getClazz()->getClassName() << " " << method->getMethodName() << ":"
//               << method->getMethodDesc() << std::endl;
    u4 &pc = javaThread->pc_;

    while (!codeReader.ended(pc)) {
        u1 byteCode = codeReader.readU1(pc);
//        std::cout << "\t" << byteCodeMap[byteCode].first << std::endl;
        switch (byteCode) {
            case OPC_ICONST_0: {
                OpcodeExecutor::Op_iconst0(javaThread);
                break;
            }
            case OPC_BIPUSH: {
                OpcodeExecutor::Op_bipush(javaThread);
                break;
            }
            case OPC_LDC: { // ldc
                OpcodeExecutor::Op_ldc(javaThread);
                break;
            }
            case OPC_LDC2_W: {
                OpcodeExecutor::Op_LDC2_W(javaThread);
                break;
            }
            case OPC_ALOAD : {
                OpcodeExecutor::Op_aload(javaThread);
                break;
            }
            case OPC_ILOAD_0 : {
                OpcodeExecutor::Op_iload0(javaThread);
                break;
            }
            case OPC_ILOAD_1 : {
                OpcodeExecutor::Op_iload1(javaThread);
                break;
            }
            case OPC_LLOAD_1 : {
                OpcodeExecutor::Op_lload1(javaThread);
                break;
            }
            case OPC_LLOAD_2 : {
                OpcodeExecutor::Op_lload2(javaThread);
                break;
            }
            case OPC_I2L: {
                OpcodeExecutor::Op_i2l(javaThread);
                break;
            }
            case OPC_LADD: {
                OpcodeExecutor::Op_ladd(javaThread);
                break;
            }
            case OPC_NEW: { //new
                OpcodeExecutor::Op_new(javaThread);
                break;
            }
            case OPC_ALOAD_0: { // aload_0
                OpcodeExecutor::Op_aload0(javaThread);
                break;
            }
            case OPC_ALOAD_1: { // aload_1
                OpcodeExecutor::Op_aload1(javaThread);
                break;
            }

            case OPC_RETURN: { // return
                OpcodeExecutor::Op_return(javaThread);
                break;
            }
            case OPC_LRETURN: {
                return OpcodeExecutor::Op_lreturn(javaThread);
            }
            case OPC_PUTSTATIC: {// putstatic
                OpcodeExecutor::Op_putstatic(javaThread);
                break;
            }
            case OPC_INVOKEVIRTUAL: { // invokevirtual
                OpcodeExecutor::Op_invokeVirtual(javaThread);
                break;
            }
            case OPC_INVOKESPECIAL: { // invokespecial
                OpcodeExecutor::Op_invokeSpecial(javaThread);
                break;
            }
            case OPC_INVOKESTATIC: { // invokestatic
                OpcodeExecutor::Op_invokeStatic(javaThread);
                break;
            }

            case OPC_GETSTATIC: { // getstatic
                OpcodeExecutor::Op_getStatic(javaThread);
                break;
            }
            case OPC_DUP:
                OpcodeExecutor::Op_dup(javaThread);
                break;
            case OPC_LSTORE_1:
                OpcodeExecutor::Op_lstore_1(javaThread);
                break;
            default:
                PANIC("bytecode not implemented....", byteCodeMap[byteCode].first.c_str());
        }
    }
    return nullptr;
}
