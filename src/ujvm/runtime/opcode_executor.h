#pragma once

#include <stack>
#include <list>
#include "java_thread.h"
#include "ujvm/classpath/system_dictionary.h"
#include "instance_oop.h"
#include "oop_value_type.h"


class OpcodeExecutor {

public:
    static void Op_new(int &codeIdx, u1 *code, const JavaThread *javaThread, const ConstantPool* cp) {
        u2 index = (code[codeIdx] << 8) + code[codeIdx + 1];
        codeIdx += 2;

        CONSTANT_Class_info *classInfo = dynamic_cast<CONSTANT_Class_info *>(cp->getConstantPool()[index]);

        CONSTANT_Utf8_info *classNameUtf8Info = dynamic_cast<CONSTANT_Utf8_info *>(cp->getConstantPool()[classInfo->index]);
        const auto className = classNameUtf8Info->getConstant();

        InstanceClassStruct *clz = BootstrapClassLoader::get()->loadClassByName(className);
        InstanceOop *instance = InstanceOop::allocateInstance(clz);
        auto size = sizeof(InstanceOop);
        javaThread->stackFrame.top()->getOperandStack().pushRef(instance);
    }

    static void Op_dup(int &codeIdx, u1 *code, const JavaThread *javaThread, const ConstantPool* cp) {
        auto top = javaThread->stackFrame.top()->getOperandStack().top();
        javaThread->stackFrame.top()->getOperandStack().pushRef(top);
    }

    static void Op_iconst0(int &codeIdx, u1 *code, const JavaThread *javaThread, const ConstantPool* cp) {
        javaThread->stackFrame.top()->getOperandStack().pushInt(0);
    }

    static void
    Op_invokeSpecial(int &codeIdx, u1 *code, JavaThread *javaThread, const ConstantPool* cp) {
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

//
//        MethodInfo *targetMethod = nullptr;
//        for (int i = 0; i < clz->getClassFile()->methods_count; ++i) {
//            MethodInfo *methodItem = clz->getClassFile()->methods[i];
//            auto tmpMethodName = methodItem->getMethodName();
//            auto tmpMethodDesc = methodItem->getMethodDesc();
//            std::wcout << "methodName_: " << tmpMethodName << std::endl;
//            std::wcout << "methodDesc_: " << tmpMethodDesc << std::endl;
//            if (tmpMethodName == methodName && tmpMethodDesc == methodDesc) {
//                targetMethod = methodItem;
//                break;
//            }
//        }
//        if (!targetMethod) {
//            PANIC("target method is null");
//        }

        // push operand stack
        // new File("xx");
        // new #2
        // dup
        // ldc #3
        // invokespecial #4
//
//        CodeAttribute *targetMethodCodeAttr = targetMethod->getCode();
//        JavaFrame *frame = new JavaFrame(targetMethodCodeAttr->max_locals, targetMethodCodeAttr->max_stack);
//        javaThread->stackFrame.push(frame);

//        todo
//        vector<CommonValue *> args;
//        args[0] = javaThread->stackFrame.top()->getOperandStack().top(); // this
        int argsCount = 1;

//        for (int i = 0; i < argsCount; ++i) {
//            javaThread->stackFrame.top()->locals[i] = args[i];
//        }

    }

    static void Op_invokeStatic(int &codeIdx, u1 *code, JavaThread *javaThread, const ConstantPool* cp) {
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

        // push operand stack
        // new File("xx");
        // new #2
        // dup
        // ldc #3
        // invokespecial #4
        CodeAttribute *targetMethodCodeAttr = targetMethod->getCode();

        JavaFrame *callerFrame = javaThread->currentFrame();
        JavaFrame *frame = new JavaFrame(targetMethodCodeAttr->max_locals, targetMethodCodeAttr->max_stack);
        javaThread->stackFrame.push(frame);

        auto methodArgs = targetMethod->getMethodArgs();

        Locals &locals = frame->getLocals();

        std::list<OopDesc *> args;

        for (auto it = methodArgs.rbegin(); it != methodArgs.rend(); ++it) {
            const ValueType &valueType = *it;
            switch (valueType) {
                case ValueType::INT:
                    args.push_front(new IntOopDesc(callerFrame->getOperandStack().popInt()));
                    break;
                case ValueType::LONG:
                    args.push_front(new IntOopDesc(callerFrame->getOperandStack().popInt()));
                    break;
                case ValueType::FLOAT:
                    //todo
                    break;
                case ValueType::DOUBLE:
                    //todo
                    break;
                case ValueType::OBJECT:
                    args.push_front((OopDesc *) callerFrame->getOperandStack().popRef());
                    break;
                case ValueType::ARRAY:
                    //todo
                    break;
                default:
                    break;
            }
        }
        if (!targetMethod->isStatic()) {
            args.push_front((OopDesc *) callerFrame->getOperandStack().popRef());
        }

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
    }

    static void Op_getStatic(int &codeIdx, u1 *code, const JavaThread *javaThread, const ConstantPool* cp) {
        u2 index = (code[codeIdx] << 8) + code[codeIdx + 1];
        codeIdx += 2;

        std::cout << "Index is :" << +index << std::endl;
        ConstantFieldref_info *constantFieldRefInfo = dynamic_cast<ConstantFieldref_info *>(cp->getConstantPool()[index]);

        CONSTANT_Class_info *classInfo = (CONSTANT_Class_info *) cp->getConstantPool()[constantFieldRefInfo->class_index];
        CONSTANT_NameAndType_info *nameAndTypeInfo = (CONSTANT_NameAndType_info *) cp->getConstantPool()[constantFieldRefInfo->name_and_type_index];

        std::cout << "tag is " << +(constantFieldRefInfo->tag) << std::endl;
        std::cout << "class_index: " << constantFieldRefInfo->class_index << std::endl;
        std::cout << "name_and_type_index: " << constantFieldRefInfo->name_and_type_index << std::endl;

        CONSTANT_Utf8_info *classNameUtf8Info = dynamic_cast<CONSTANT_Utf8_info *>(cp->getConstantPool()[classInfo->index]);
        std::wcout << "className: " << classNameUtf8Info->getConstant() << std::endl;

        InstanceClassStruct *classInstance = SystemDictionary::get()->find(classNameUtf8Info->getConstant());

        CONSTANT_Utf8_info *fieldNameUtf8Info = dynamic_cast<CONSTANT_Utf8_info *>(cp->getConstantPool()[nameAndTypeInfo->name_index]);
        CONSTANT_Utf8_info *fieldDescUtf8Info = dynamic_cast<CONSTANT_Utf8_info *>(cp->getConstantPool()[nameAndTypeInfo->descriptor_index]);
        std::wcout << "fieldName:" << fieldNameUtf8Info->getConstant() << std::endl;
        std::wcout << "fieldDesc:" << fieldDescUtf8Info->getConstant() << std::endl;
        const strings::String fieldName = fieldNameUtf8Info->getConstant();
        OopDesc *fieldValue = classInstance->getStaticValueMap()[fieldName];
        //todo
//        switch (fieldValue) {
//
//        }
//        javaThread->stackFrame.top()->getOperandStack().push(fieldValue);
        std::cout << ">>>>>>>>>>>" << std::endl;
    }
};