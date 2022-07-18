#pragma once

#include <ffi.h>
#include <stack>
#include <list>
#include "java_thread.h"
#include "classpath/system_dictionary.h"
#include "instance_oop.h"
#include "oop_value_type.h"
#include "bytecode/bytecode_interpreter.h"
#include "shared/ujvm.h"

class OpcodeExecutor {

public:
    static void Op_new(int &codeIdx, u1 *code, JavaThread *javaThread, const ConstantPool *cp) {
        u2 index = (code[codeIdx] << 8) + code[codeIdx + 1];
        codeIdx += 2;

        CONSTANT_Class_info *classInfo = dynamic_cast<CONSTANT_Class_info *>(cp->getConstantPool()[index]);

        CONSTANT_Utf8_info *classNameUtf8Info = dynamic_cast<CONSTANT_Utf8_info *>(cp->getConstantPool()[classInfo->index]);
        const auto className = classNameUtf8Info->getConstant();

        InstanceClassStruct *clz = BootstrapClassLoader::get()->loadClassByName(className);
        InstanceOop *instance = InstanceOop::allocateInstance(clz);
        javaThread->currentFrame()->getOperandStack().pushRef(instance);
    }

    static void Op_dup(int &codeIdx, u1 *code, JavaThread *javaThread, const ConstantPool *cp) {
        auto top = javaThread->currentFrame()->getOperandStack().top();
        javaThread->currentFrame()->getOperandStack().pushRef(top);
    }

    static void Op_ldc(int &codeIdx, u1 *code, JavaThread *javaThread, const ConstantPool *cp) {

        int idx = code[codeIdx];
        auto constantStringInfo = (CONSTANT_String_info *) cp->getConstantPool()[idx];
        const strings::String *stringValue = ((CONSTANT_Utf8_info *) cp->getConstantPool()[constantStringInfo->index])->getConstantInHeap();
        InstanceClassStruct *clz = BootstrapClassLoader::get()->loadClassByName(L"java/lang/String");

        javaThread->currentFrame()->getOperandStack().pushRef(
                new InstanceOopDesc(clz, const_cast<strings::String *> (stringValue)));

        codeIdx++;
    }

    static void Op_iconst0(int &codeIdx, u1 *code, JavaThread *javaThread, const ConstantPool *cp) {
        javaThread->currentFrame()->getOperandStack().pushInt(0);
    }

    static void Op_aload0(int &codeIdx, u1 *code, JavaThread *javaThread, const ConstantPool *cp) {
        auto ref = javaThread->currentFrame()->getLocals().getSlotArray().getRef(0);
        javaThread->currentFrame()->getOperandStack().pushRef(ref);
    }

    static void Op_aload1(int &codeIdx, u1 *code, JavaThread *javaThread, const ConstantPool *cp) {
        auto ref = javaThread->currentFrame()->getLocals().getSlotArray().getRef(1);
        javaThread->currentFrame()->getOperandStack().pushRef(ref);
    }

    static void Op_bipush(int &codeIdx, u1 *code, JavaThread *javaThread, const ConstantPool *cp) {
        u1 byteValue = code[codeIdx];
        javaThread->currentFrame()->getOperandStack().pushInt(byteValue);
        codeIdx++;
    }

    static void Op_iload0(int &codeIdx, u1 *code, JavaThread *javaThread, const ConstantPool *cp) {

        int value = javaThread->currentFrame()->getLocals().getSlotArray().getInt(0);
        javaThread->currentFrame()->getOperandStack().pushInt(value);
    }

    static void Op_invokeVirtual(int &codeIdx, u1 *code, JavaThread *javaThread, const ConstantPool *cp) {
        //todo
        Op_invokeSpecial(codeIdx, code, javaThread, cp);
    }

    typedef union jvalue {
        jboolean z;
        jbyte b;
        jchar c;
        jshort s;
        jint i;
        jlong j;
        jfloat f;
        jdouble d;
        jobject l;
    } jvalue;
    static void invokeNative(Method *targetMethod, std::list<void *> &args_) {

        vector<void *> args(args_.begin(), args_.end());


        auto methodArgs = targetMethod->getMethodArgs();

        auto nativeMethod = targetMethod->getNativeMethod();
        int argCount = targetMethod->getMethodArgs().size() + 2;
        void *fn = nativeMethod->getNativeSymbol();

        // prepare args type
        ffi_type *ffiArgTypeArr[argCount];
        ffiArgTypeArr[0] = &ffi_type_pointer;
        ffiArgTypeArr[1] = &ffi_type_pointer;

        for (int i = 0; i < targetMethod->getMethodArgs().size(); ++i) {
            auto argValueTypeItem = targetMethod->getMethodArgs()[i];
            switch (argValueTypeItem) {
                case ValueType::INT:
                    ffiArgTypeArr[i + 2] = &ffi_type_sint;
                    break;
                case ValueType::LONG:
                    break;
                case ValueType::FLOAT:
                    //todo
                    break;
                case ValueType::DOUBLE:
                    //todo
                    break;
                case ValueType::OBJECT:
                    ffiArgTypeArr[i + 2] = &ffi_type_pointer;
                    break;
                case ValueType::ARRAY:
                    //todo
                    break;
                default:
                    break;
            }
        }
        jvalue argsHolder[argCount];

        // prepare args value
        void *ffiArgs[argCount];

//            void *ffiArgPtr0 = alloca(ffiArgTypeArr[0]->size);
//            ffiArgPtr0 = Ujvm::getJNIENV();
        ffiArgs[0] = &args[0];

//            void *ffiArgPtr1 = alloca(ffiArgTypeArr[1]->size);
//            ffiArgPtr1;
//        ffiArgs[1] = alloca(ffiArgTypeArr[1]->size);

        int localVariableIndex = 0;
        for (int i = 1; i < argCount; ++i) {
            auto item = (OopDesc *) args[i];
            switch (item->getOopType()) {

                case INSTANCE_OOP: {
                    ffiArgs[i] = &item;
                    break;
                }
                case OBJECT_ARRAY_OOP:
                case TYPE_ARRAY_OOP:
                    PANIC("not implemented>>>>>..");
                case PRIMITIVE_OOP: {
                    auto valueType = methodArgs[localVariableIndex];
                    switch (valueType) {
                        case BYTE:
                        case BOOLEAN:
                        case CHAR:
                        case SHORT:
                        case ValueType::INT: {
                            int intValue = ((IntOopDesc *) item)->getValue();

                            int *ffiArgPtr = (int *) alloca(ffi_type_sint.size);
                            *ffiArgPtr = intValue;
                            ffiArgs[localVariableIndex + 2] = ffiArgPtr;
                            break;
                        }
                        case ValueType::LONG: {
                            PANIC("not implemented");
                        }
                        case FLOAT:
                            //todo
                            break;
                        case DOUBLE:
                            //todo
                            break;
                        default:
                            break;
                    }
                }
                default:
                    break;

            }
            localVariableIndex++;
        }

        ffi_cif cif;
        ffi_type *returnFfiType = &ffi_type_void;
        ffi_status ffiPrepStatus = ffi_prep_cif(&cif, FFI_DEFAULT_ABI, (unsigned int) argCount, returnFfiType,
                                                ffiArgTypeArr);
        if (ffiPrepStatus != FFI_OK) {
            PANIC("prepare stauts not ok");
        }

        void *returnPtr = nullptr;
//            if (returnFfiType->size > 0) {
//                returnPtr = alloca(returnFfiType->size);
//            }
        ffi_call(&cif, FFI_FN(fn), returnPtr, ffiArgs);
    }

    static void Op_invokeSpecial(int &codeIdx, u1 *code, JavaThread *javaThread, const ConstantPool *cp) {
        u2 index = (code[codeIdx] << 8) + code[codeIdx + 1];
        codeIdx += 2;

        ConstantMethodref_info *method = dynamic_cast<ConstantMethodref_info *>(cp->getConstantPool()[index]);
        CONSTANT_Class_info *classInfo = dynamic_cast<CONSTANT_Class_info *>(cp->getConstantPool()[method->class_index]);

        auto className = ((CONSTANT_Utf8_info *) cp->getConstantPool()[classInfo->index])->getConstant();
        auto clz = SystemDictionary::get()->find(className);

        CONSTANT_NameAndType_info *nameAndType = dynamic_cast<CONSTANT_NameAndType_info *>(cp->getConstantPool()[method->name_and_type_index]);
        auto methodName = ((CONSTANT_Utf8_info *) cp->getConstantPool()[nameAndType->name_index])->getConstant();
        auto methodDesc = ((CONSTANT_Utf8_info *) cp->getConstantPool()[nameAndType->descriptor_index])->getConstant();
        std::wcout << methodName << ":" << methodDesc << std::endl;

        Method *targetMethod = clz->findMethod(methodName, methodDesc);
        if (!targetMethod) {
            PANIC("target method is null");
        }
        JavaFrame *callerFrame = javaThread->currentFrame();

        if (!targetMethod->isNative()) {
            JavaFrame *frame = new JavaFrame(targetMethod->getCode()->max_locals, targetMethod->getCode()->max_stack);
            javaThread->pushFrame(frame);

//            vector<ValueType> &argTypes = targetMethod->getMethodArgs();
            Locals &calleeLocals = frame->getLocals();
            int slotCount = targetMethod->getArgsSlotCount();
            for (int i = slotCount - 1; i >= 0; --i) {
                auto ref = callerFrame->getOperandStack().popRef();
                calleeLocals.getSlotArray().setRef(i, ref); //todo
            }

            BytecodeInterpreter::run(targetMethod, javaThread);
            return;
        }
        // process native
        std::list<void *> args;

        auto methodArgs = targetMethod->getMethodArgs();
        fillArgsValue(callerFrame, targetMethod, methodArgs, args);
        invokeNative(targetMethod, args);
    }

    static void
    fillArgsValue(JavaFrame *callerFrame, Method *targetMethod, vector<ValueType> &methodArgs, list<void *> &args) {

        for (auto it = methodArgs.rbegin(); it != methodArgs.rend(); ++it) {
            const ValueType &valueType = *it;
            switch (valueType) {
                case INT:
                    args.push_front(new IntOopDesc(callerFrame->getOperandStack().popInt()));
                    break;
                case LONG:
                    args.push_front(new IntOopDesc(callerFrame->getOperandStack().popInt()));
                    break;
                case FLOAT:
                    //todo
                    break;
                case DOUBLE:
                    //todo
                    break;
                case OBJECT: {

                    auto ref = (InstanceOopDesc *) callerFrame->getOperandStack().popRef();
                    args.push_front(ref);
                    break;
                }
                case ARRAY:
                    //todo
                    break;
                default:
                    break;
            }
        }

        if (targetMethod->isStatic()) {
            args.push_front(nullptr); // todo
        } else {
            args.push_front(callerFrame->getOperandStack().popRef());
        }

        args.push_front(Ujvm::getJNIENV());
    }

    static void Op_invokeStatic(int &codeIdx, u1 *code, JavaThread *javaThread, const ConstantPool *cp) {
        u2 index = (code[codeIdx] << 8) + code[codeIdx + 1];
        codeIdx += 2;

        ConstantMethodref_info *method = dynamic_cast<ConstantMethodref_info *>(cp->getConstantPool()[index]);
        CONSTANT_Class_info *classInfo = dynamic_cast<CONSTANT_Class_info *>(cp->getConstantPool()[method->class_index]);

        auto className = ((CONSTANT_Utf8_info *) cp->getConstantPool()[classInfo->index])->getConstant();
        auto clz = SystemDictionary::get()->find(className);

        CONSTANT_NameAndType_info *nameAndType = dynamic_cast<CONSTANT_NameAndType_info *>(cp->getConstantPool()[method->name_and_type_index]);
        auto methodName = ((CONSTANT_Utf8_info *) cp->getConstantPool()[nameAndType->name_index])->getConstant();
        auto methodDesc = ((CONSTANT_Utf8_info *) cp->getConstantPool()[nameAndType->descriptor_index])->getConstant();
        std::wcout << methodName << ":" << methodDesc << std::endl;

        JavaFrame *callerFrame = javaThread->currentFrame();

        Method *targetMethod = clz->findMethod(methodName, methodDesc);
        if (!targetMethod) {
            PANIC("target method is null");
        }

        auto methodArgs = targetMethod->getMethodArgs();


        std::list<void *> nativeArgs;

        fillArgsValue(callerFrame, targetMethod, methodArgs, nativeArgs);


        if (targetMethod->isNative()) {
            invokeNative(targetMethod, nativeArgs);
            return;
        }

        std::list<OopDesc *> args;
        CodeAttribute *targetMethodCodeAttr = targetMethod->getCode();

        JavaFrame *frame = new JavaFrame(targetMethodCodeAttr->max_locals, targetMethodCodeAttr->max_stack);
        javaThread->pushFrame(frame);


        Locals &locals = frame->getLocals();
        // fill callee frame local variable table
        int slot = 0;
        int localVariableIndex = 0;
        for (auto ite = args.begin(); ite != args.end(); ++ite) {

            const auto &item = *ite;
            switch (item->getOopType()) {

                case INSTANCE_OOP:
                case OBJECT_ARRAY_OOP:
                case TYPE_ARRAY_OOP:
                    locals.getSlotArray().setRef(slot++, item);
                    break;
                case PRIMITIVE_OOP: {
                    auto valueType = methodArgs[localVariableIndex];
                    switch (valueType) {
                        case BYTE:
                        case BOOLEAN:
                        case CHAR:
                        case SHORT:
                        case ValueType::INT: {
                            auto intValue = ((IntOopDesc *) item)->getValue();
                            locals.getSlotArray().setInt(slot++, intValue);
                            break;
                        }
                        case ValueType::LONG: {
                            jlong value = ((LongOopDesc *) item)->getValue();
                            locals.getSlotArray().setLong(slot, value);
                            slot += 2;
                            break;
                        }
                        case FLOAT:
                            //todo
                            break;
                        case DOUBLE:
                            //todo
                            break;
                        default:
                            break;
                    }
                }
                default:
                    break;

            }
            localVariableIndex++;
        }
        BytecodeInterpreter::run(targetMethod, javaThread);
//        javaThread->popFrame();
//        delete frame;
    }

    static void Op_return(int &codeIdx, u1 *code, JavaThread *javaThread, const ConstantPool *cp) {
        auto frame = javaThread->popFrame();
        delete frame;
    }

    static void Op_putstatic(int &codeIdx, u1 *code, JavaThread *javaThread, const ConstantPool *cp) {
        u2 index = (code[codeIdx] << 8) + code[codeIdx + 1];
        codeIdx += 2;
        ConstantFieldref_info *constantFieldRefInfo = dynamic_cast<ConstantFieldref_info *>(cp->getConstantPool()[index]);
        CONSTANT_Class_info *classInfo = (CONSTANT_Class_info *) cp->getConstantPool()[constantFieldRefInfo->class_index];
        CONSTANT_NameAndType_info *nameAndTypeInfo = (CONSTANT_NameAndType_info *) cp->getConstantPool()[constantFieldRefInfo->name_and_type_index];
        CONSTANT_Utf8_info *classNameUtf8Info = dynamic_cast<CONSTANT_Utf8_info *>(cp->getConstantPool()[classInfo->index]);
        CONSTANT_Utf8_info *fieldNameUtf8Info = dynamic_cast<CONSTANT_Utf8_info *>(cp->getConstantPool()[nameAndTypeInfo->name_index]);
        InstanceClassStruct *classInstance = SystemDictionary::get()->find(classNameUtf8Info->getConstant());
        auto fieldName = fieldNameUtf8Info->getConstant();
        classInstance->getStaticValueMap()[fieldName] = new InstanceOopDesc(classInstance,
                                                                            javaThread->currentFrame()->getOperandStack().popRef());
    }

    static void Op_getStatic(int &codeIdx, u1 *code, JavaThread *javaThread, const ConstantPool *cp) {
        u2 index = (code[codeIdx] << 8) + code[codeIdx + 1];
        codeIdx += 2;
        std::cout << "Index is :" << +index << std::endl;
        ConstantFieldref_info *constantFieldRefInfo = dynamic_cast<ConstantFieldref_info *>(cp->getConstantPool()[index]);

        CONSTANT_Class_info *classInfo = (CONSTANT_Class_info *) cp->getConstantPool()[constantFieldRefInfo->class_index];
        CONSTANT_NameAndType_info *nameAndTypeInfo = (CONSTANT_NameAndType_info *) cp->getConstantPool()[constantFieldRefInfo->name_and_type_index];


        CONSTANT_Utf8_info *classNameUtf8Info = dynamic_cast<CONSTANT_Utf8_info *>(cp->getConstantPool()[classInfo->index]);
        std::wcout << "className: " << classNameUtf8Info->getConstant() << std::endl;

        InstanceClassStruct *classInstance = SystemDictionary::get()->find(classNameUtf8Info->getConstant());

        CONSTANT_Utf8_info *fieldNameUtf8Info = dynamic_cast<CONSTANT_Utf8_info *>(cp->getConstantPool()[nameAndTypeInfo->name_index]);
        CONSTANT_Utf8_info *fieldDescUtf8Info = dynamic_cast<CONSTANT_Utf8_info *>(cp->getConstantPool()[nameAndTypeInfo->descriptor_index]);
        std::wcout << "fieldName:" << fieldNameUtf8Info->getConstant() << std::endl;
        std::wcout << "fieldDesc:" << fieldDescUtf8Info->getConstant() << std::endl;
        const strings::String fieldName = fieldNameUtf8Info->getConstant();
        OopDesc *fieldValue = classInstance->getStaticValueMap()[fieldName];
        switch (fieldValue->getOopType()) {
            case INSTANCE_OOP:
            case OBJECT_ARRAY_OOP:
            case TYPE_ARRAY_OOP:
                javaThread->currentFrame()->getOperandStack().pushRef(fieldValue);
                break;
            case PRIMITIVE_OOP:
                PANIC("not implemented: getstatic ");
                //todo
                break;
            default:
                break;
        }
    }
};