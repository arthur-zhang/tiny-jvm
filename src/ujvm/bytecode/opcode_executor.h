#pragma once

#include <ffi.h>
#include <stack>
#include <list>
#include <ujvm/runtime/java_thread.h>
#include <ujvm/runtime/system_dictionary.h>
#include <ujvm/runtime/instance_oop.h>
#include <ujvm/runtime/instance_class.h>
#include <ujvm/runtime/oop_value_type.h>
#include "bytecode_interpreter.h"
#include <shared/ujvm.h>

class OpcodeExecutor {

public:
    static void Op_new(JavaThread *javaThread) {
        CodeReader &codeReader = javaThread->currentFrame()->getMethod()->getCodeReader();
        u2 index = codeReader.readU2(javaThread->pc_);
        const ConstantPool *cp = javaThread->currentFrame()->getMethod()->getCp();

        CONSTANT_Class_info *classInfo = dynamic_cast<CONSTANT_Class_info *>(cp->getConstantPool()[index]);

        CONSTANT_Utf8_info *classNameUtf8Info = dynamic_cast<CONSTANT_Utf8_info *>(cp->getConstantPool()[classInfo->index]);
        const auto className = classNameUtf8Info->getConstant();

        InstanceKlass *clz = BootstrapClassLoader::get()->loadClassByName(className);
        InstanceOop *instance = InstanceOop::allocateInstance(clz);
        javaThread->currentFrame()->getOperandStack().pushRef(instance);
    }

    static void Op_dup(JavaThread *javaThread) {
        auto top = javaThread->currentFrame()->getOperandStack().top();
        javaThread->currentFrame()->getOperandStack().pushRef(top);
    }

    static void Op_lstore_1(JavaThread *javaThread) {
        auto v = javaThread->currentFrame()->getOperandStack().popLong();
        javaThread->currentFrame()->getLocals().getSlotArray().setLong(1, v);
    }

    static void Op_ldc(JavaThread *javaThread) {

        CodeReader &codeReader = javaThread->currentFrame()->getMethod()->getCodeReader();
        int idx = codeReader.readU1(javaThread->pc_);
        const ConstantPool *cp = javaThread->currentFrame()->getMethod()->getCp();

        auto constantStringInfo = (CONSTANT_String_info *) cp->getConstantPool()[idx];
        const strings::String *stringValue = ((CONSTANT_Utf8_info *) cp->getConstantPool()[constantStringInfo->index])->getConstantInHeap();
        InstanceKlass *clz = BootstrapClassLoader::get()->loadClassByName(L"java/lang/String");

        javaThread->currentFrame()->getOperandStack().pushRef(
                new InstanceOopDesc(clz, const_cast<strings::String *> (stringValue)));

    }

    static void Op_LDC2_W(JavaThread *javaThread) {
        CodeReader &codeReader = javaThread->currentFrame()->getMethod()->getCodeReader();
        u2 idx = codeReader.readU2(javaThread->pc_);

        const ConstantPool *cp = javaThread->currentFrame()->getMethod()->getCp();

        CONSTANT_Long_info *longInfo = dynamic_cast<CONSTANT_Long_info *>(cp->getConstantPool()[idx]);
        jlong longValue = longInfo->getConstant();
        javaThread->currentFrame()->getOperandStack().pushLong(longValue);
    }


    static OOP Op_iconst0(JavaThread *javaThread) {
        javaThread->currentFrame()->getOperandStack().pushInt(0);
        return nullptr;
    }

    static void Op_aload0(JavaThread *javaThread) {
        auto ref = javaThread->currentFrame()->getLocals().getSlotArray().getRef(0);
        javaThread->currentFrame()->getOperandStack().pushRef(ref);
    }

    static void Op_aload1(JavaThread *javaThread) {
        auto ref = javaThread->currentFrame()->getLocals().getSlotArray().getRef(1);
        javaThread->currentFrame()->getOperandStack().pushRef(ref);
    }

    static void Op_bipush(JavaThread *javaThread) {
        Method *method = javaThread->currentFrame()->getMethod();
        u1 byteValue = method->getCodeReader().readU1(javaThread->pc_);
        javaThread->currentFrame()->getOperandStack().pushInt(byteValue);
    }

    static void Op_iloadx(JavaThread *javaThread, size_t pos) {
        int value = javaThread->currentFrame()->getLocals().getSlotArray().getInt(pos);
        javaThread->currentFrame()->getOperandStack().pushInt(value);
    }

    static void Op_iload0(JavaThread *javaThread) {
        Op_iloadx(javaThread, 0);
    }

    static void Op_iload1(JavaThread *javaThread) {
        Op_iloadx(javaThread, 1);
    }

    static void Op_lloadx(JavaThread *javaThread, size_t pos) {
        jlong value = javaThread->currentFrame()->getLocals().getSlotArray().getLong(pos);
        javaThread->currentFrame()->getOperandStack().pushLong(value);
    }

    static void Op_lload1(JavaThread *javaThread) {
        Op_lloadx(javaThread, 1);
    }

    static void Op_lload2(JavaThread *javaThread) {
        Op_lloadx(javaThread, 2);
    }

    static void Op_i2l(JavaThread *javaThread) {
        jint v = javaThread->currentFrame()->getOperandStack().popInt();
        javaThread->currentFrame()->getOperandStack().pushLong((jlong) v);
    }

    static void Op_ladd(JavaThread *javaThread) {
        jlong v1 = javaThread->currentFrame()->getOperandStack().popLong();
        jlong v2 = javaThread->currentFrame()->getOperandStack().popLong();
        javaThread->currentFrame()->getOperandStack().pushLong(v1 + v2);
    }

    static OOP Op_lreturn(JavaThread *javaThread) {
        jlong v = javaThread->currentFrame()->getOperandStack().popLong();
        return new LongOopDesc(v);
    }


    static void Op_aload(JavaThread *javaThread) {
        CodeReader &codeReader = javaThread->currentFrame()->getMethod()->getCodeReader();
        u1 idx = codeReader.readU1(javaThread->pc_);

        jobject value = javaThread->currentFrame()->getLocals().getSlotArray().getRef(idx);
        javaThread->currentFrame()->getOperandStack().pushRef(value);
    }


    static OOP callNativeMethod(JavaThread *javaThread, Method *targetMethod) {

        JavaFrame *callerFrame = javaThread->currentFrame();
        auto nativeMethod = targetMethod->getNativeMethod();
        void *fn = nativeMethod->getNativeSymbol();

        auto methodArgTypes = targetMethod->getMethodArgTypes();
        int nativeMethodArgCount = methodArgTypes.size() + 2;

        auto nativeRealArgTypes = vector<ValueType>(nativeMethodArgCount);
        nativeRealArgTypes[0] = ValueType::OBJECT;
        nativeRealArgTypes[1] = ValueType::OBJECT;
        for (int i = 0; i < methodArgTypes.size(); ++i) {
            nativeRealArgTypes[i + 2] = methodArgTypes[i];
        }

        vector<void *> args = fillNativeMethodArgs(callerFrame, targetMethod, methodArgTypes);

        void *ffiArgs[nativeMethodArgCount];
        ffi_type *ffiArgTypeArr[nativeMethodArgCount];

        for (int i = 0; i < nativeRealArgTypes.size(); ++i) {
            auto argValueTypeItem = nativeRealArgTypes[i];
            switch (argValueTypeItem) {
                case BYTE:
                case BOOLEAN:
                case CHAR:
                case SHORT:
                case INT: {
                    ffiArgTypeArr[i] = &ffi_type_sint;
                    int intValue = ((IntOopDesc *) args[i])->getValue();
                    int *ffiArgPtr = (int *) alloca(ffi_type_sint.size);
                    *ffiArgPtr = intValue;
                    ffiArgs[i] = ffiArgPtr;
                    break;
                }
                case ValueType::LONG: {
                    ffiArgTypeArr[i] = &ffi_type_slong;
                    jlong longValue = ((LongOopDesc *) args[i])->getValue();
                    long *ffiArgPtr = (long *) alloca(ffi_type_sint.size);
                    *ffiArgPtr = longValue;
                    ffiArgs[i] = ffiArgPtr;
                    break;
                }

                case ValueType::FLOAT:
                    //todo
                    break;
                case ValueType::DOUBLE:
                    //todo
                    break;
                case ValueType::OBJECT: {
                    ffiArgTypeArr[i] = &ffi_type_pointer;
                    ffiArgs[i] = &args[i];
                    break;
                }
                case ValueType::ARRAY:
                    //todo
                    break;
                default:
                    break;
            }
        }


        ffi_cif cif;
        ffi_type *returnFfiType = &ffi_type_void;
        ffi_status ffiPrepStatus = ffi_prep_cif(&cif, FFI_DEFAULT_ABI, (unsigned int) nativeMethodArgCount,
                                                returnFfiType,
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


    static OOP callNonNativeMethod(JavaThread *javaThread, Method *targetMethod) {
        JavaFrame *callerFrame = javaThread->currentFrame();
        JavaFrame frame = JavaFrame(targetMethod);
        frame.setReturnPc(javaThread->pc_);

        javaThread->pushFrame(&frame);

        Locals &calleeLocals = frame.getLocals();
        int slotCount = targetMethod->getArgsSlotCount();
        for (int i = slotCount - 1; i >= 0; --i) {
            auto slot = callerFrame->getOperandStack().popSlot();
            calleeLocals.getSlotArray().setSlot(i, slot);
        }

        javaThread->pc_ = 0;
        OOP ret = BytecodeInterpreter::interpret(javaThread);
        auto returnType = targetMethod->getReturnType();
        javaThread->popFrame();
        javaThread->pc_ = frame.getReturnPc();
        return ret;

    }

    static vector<void *>
    fillNativeMethodArgs(JavaFrame *callerFrame, Method *targetMethod, vector<ValueType> &methodArgs) {

        list<void *> args;
        for (auto it = methodArgs.rbegin(); it != methodArgs.rend(); ++it) {
            const ValueType &valueType = *it;
            switch (valueType) {
                case INT:
                case BYTE:
                case BOOLEAN:
                case CHAR:
                case SHORT: {
                    args.push_front(new IntOopDesc(callerFrame->getOperandStack().popInt()));
                    break;
                }
                case LONG: {
                    args.push_front(new LongOopDesc(callerFrame->getOperandStack().popLong()));
                    break;
                }
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
            args.push_front(callerFrame->getMethod()->getClazz()); // todo
        } else {
            args.push_front(callerFrame->getOperandStack().popRef());
        }

        args.push_front(Ujvm::getJNIENV());
        return vector(args.begin(), args.end());
    }

    static void invokeMethod(JavaThread *javaThread) {
        CodeReader &codeReader = javaThread->currentFrame()->getMethod()->getCodeReader();

        u2 index = codeReader.readU2(javaThread->pc_);
        const ConstantPool *cp = javaThread->currentFrame()->getMethod()->getCp();

        ConstantMethodref_info *method = dynamic_cast<ConstantMethodref_info *>(cp->getConstantPool()[index]);
        CONSTANT_Class_info *classInfo = dynamic_cast<CONSTANT_Class_info *>(cp->getConstantPool()[method->class_index]);

        auto className = ((CONSTANT_Utf8_info *) cp->getConstantPool()[classInfo->index])->getConstant();
        auto clz = SystemDictionary::get()->find(className);

        CONSTANT_NameAndType_info *nameAndType = dynamic_cast<CONSTANT_NameAndType_info *>(cp->getConstantPool()[method->name_and_type_index]);
        auto methodName = ((CONSTANT_Utf8_info *) cp->getConstantPool()[nameAndType->name_index])->getConstant();
        auto methodDesc = ((CONSTANT_Utf8_info *) cp->getConstantPool()[nameAndType->descriptor_index])->getConstant();

        Method *targetMethod = clz->findMethod(methodName, methodDesc);
        if (!targetMethod) {
            PANIC("target method is null");
        }
        OOP ret;

        if (targetMethod->isNative()) {
            ret = callNativeMethod(javaThread, targetMethod);
        } else {
            ret = callNonNativeMethod(javaThread, targetMethod);
        }
        ValueType returnType = targetMethod->getReturnType();
        switch (returnType) {
            case VOID:
            default:
                break;
            case BYTE:
            case BOOLEAN:
            case CHAR:
            case SHORT:
            case INT: {
                IntOopDesc *intOopDesc = (IntOopDesc *) ret;
                javaThread->currentFrame()->getOperandStack().pushInt(intOopDesc->getValue());
                break;
            }
            case FLOAT:
                break;
            case LONG: {
                LongOopDesc *longOopDesc = (LongOopDesc *) ret;
                javaThread->currentFrame()->getOperandStack().pushLong(longOopDesc->getValue());
                break;
            }
            case DOUBLE:
                break;
            case OBJECT:
                break;
            case ARRAY:
                break;
        }
    }

    static void Op_invokeVirtual(JavaThread *javaThread) {
        invokeMethod(javaThread);
    }

    static void Op_invokeSpecial(JavaThread *javaThread) {
        invokeMethod(javaThread);
    }

    static void Op_invokeStatic(JavaThread *javaThread) {
        invokeMethod(javaThread);
    }

    static void Op_return(JavaThread *javaThread) {
    }

    static void Op_putstatic(JavaThread *javaThread) {
        CodeReader &codeReader = javaThread->currentFrame()->getMethod()->getCodeReader();
        u2 index = codeReader.readU2(javaThread->pc_);
        const ConstantPool *cp = javaThread->currentFrame()->getMethod()->getCp();

        ConstantFieldref_info *constantFieldRefInfo = dynamic_cast<ConstantFieldref_info *>(cp->getConstantPool()[index]);
        CONSTANT_Class_info *classInfo = (CONSTANT_Class_info *) cp->getConstantPool()[constantFieldRefInfo->class_index];
        CONSTANT_NameAndType_info *nameAndTypeInfo = (CONSTANT_NameAndType_info *) cp->getConstantPool()[constantFieldRefInfo->name_and_type_index];
        CONSTANT_Utf8_info *classNameUtf8Info = dynamic_cast<CONSTANT_Utf8_info *>(cp->getConstantPool()[classInfo->index]);
        CONSTANT_Utf8_info *fieldNameUtf8Info = dynamic_cast<CONSTANT_Utf8_info *>(cp->getConstantPool()[nameAndTypeInfo->name_index]);
        InstanceKlass *classInstance = SystemDictionary::get()->find(classNameUtf8Info->getConstant());
        auto fieldName = fieldNameUtf8Info->getConstant();
        classInstance->setStaticFieldValue(fieldName, new InstanceOopDesc(classInstance,
                                                                          javaThread->currentFrame()->getOperandStack().popRef()));
    }

    static void Op_getStatic(JavaThread *javaThread) {
        CodeReader &codeReader = javaThread->currentFrame()->getMethod()->getCodeReader();
        u2 index = codeReader.readU2(javaThread->pc_);

        const ConstantPool *cp = javaThread->currentFrame()->getMethod()->getCp();

        std::cout << "Index is :" << +index << std::endl;
        ConstantFieldref_info *constantFieldRefInfo = dynamic_cast<ConstantFieldref_info *>(cp->getConstantPool()[index]);

        CONSTANT_Class_info *classInfo = (CONSTANT_Class_info *) cp->getConstantPool()[constantFieldRefInfo->class_index];
        CONSTANT_NameAndType_info *nameAndTypeInfo = (CONSTANT_NameAndType_info *) cp->getConstantPool()[constantFieldRefInfo->name_and_type_index];


        CONSTANT_Utf8_info *classNameUtf8Info = dynamic_cast<CONSTANT_Utf8_info *>(cp->getConstantPool()[classInfo->index]);
        std::wcout << "className: " << classNameUtf8Info->getConstant() << std::endl;

        InstanceKlass *classInstance = SystemDictionary::get()->find(classNameUtf8Info->getConstant());

        CONSTANT_Utf8_info *fieldNameUtf8Info = dynamic_cast<CONSTANT_Utf8_info *>(cp->getConstantPool()[nameAndTypeInfo->name_index]);
        CONSTANT_Utf8_info *fieldDescUtf8Info = dynamic_cast<CONSTANT_Utf8_info *>(cp->getConstantPool()[nameAndTypeInfo->descriptor_index]);
        std::wcout << "fieldName:" << fieldNameUtf8Info->getConstant() << std::endl;
        std::wcout << "fieldDesc:" << fieldDescUtf8Info->getConstant() << std::endl;
        const strings::String fieldName = fieldNameUtf8Info->getConstant();
        OopDesc *fieldValue = classInstance->getStaticFieldValue(fieldName);
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