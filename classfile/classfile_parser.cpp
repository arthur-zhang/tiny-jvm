//
// Created by ya on 2022/2/28.
//
#include <map>
#include <iostream>
#include <sstream>
#include "classfile_parser.h"
#include "field_info.h"
#include "String.h"

wstring decodeMUTF8(unsigned char *bytearr, int len) {
    int utflen = len;
    unsigned char chararr[len * 2];
    int c, char2, char3;
    int count = 0;
    int chararr_count = 0;
    while (count < utflen) {
        c = (int) bytearr[count] & 0xff;
        if (c > 127) break;
        count++;
        chararr[chararr_count++] = (unsigned char) c;
    }
    while (count < utflen) {
        c = (int) bytearr[count] & 0xff;
        switch (c >> 4) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
                /* 0xxxxxxx*/
                count++;
                chararr[chararr_count++] = (char) c;
                break;
            case 12:
            case 13:
                /* 110x xxxx   10xx xxxx*/
                count += 2;
                if (count > utflen)
                    assert("malformed input: partial character at end");
                char2 = (int) bytearr[count - 1];
                if ((char2 & 0xC0) != 0x80)
                    assert("malformed input around byte " + count);
                chararr[chararr_count++] = (char) (((c & 0x1F) << 6) |
                                                   (char2 & 0x3F));
                break;
            case 14:
                /* 1110 xxxx  10xx xxxx  10xx xxxx */
                count += 3;
                if (count > utflen)
                    assert("malformed input: partial character at end");
                char2 = (int) bytearr[count - 2];
                char3 = (int) bytearr[count - 1];
                if (((char2 & 0xC0) != 0x80) || ((char3 & 0xC0) != 0x80))
                    assert("malformed input around byte " + (count - 1));
                chararr[chararr_count++] = (char) (((c & 0x0F) << 12) |
                                                   ((char2 & 0x3F) << 6) |
                                                   ((char3 & 0x3F) << 0));
                break;
            default:
                /* 10xx xxxx,  1111 xxxx */
                assert("malformed input around byte " + count);
        }
    }

    return wstring(chararr, chararr + chararr_count);
}

std::string recursive_parse_annotation(AnnotationElementValue *target);

std::string parse_inner_element_value(ElementValue *inner_ev) {
    std::stringstream ss;
    switch ((char) inner_ev->tag) {
        case 'B':
        case 'C':
        case 'D':
        case 'F':
        case 'I':
        case 'J':
        case 'S':
        case 'Z':
        case 's': {
            ss << "s#"
               << ((SimpleElementValue *) inner_ev)->const_value_index;        // bug2: 指针强转不会有错误提示！！因此......这里原先写得是((const_value_t *)inner_ev)，但其实 inner_ev 内部的 inner_ev->value 才是真正应该被转换的......编译器不给报错！！
            break;
        }
        case 'e': {
            ss << "e#" << ((EnumElementValue *) inner_ev)->type_name_index << "."
               << ((EnumElementValue *) inner_ev)->const_name_index;
            break;
        }
        case 'c': {
            ss << "c#" << ((ClassElementValue *) inner_ev)->class_info_index;
            break;
        }
        case '@': {
            ss << "@"
               << recursive_parse_annotation((AnnotationElementValue *) inner_ev);    // recursive call outer function.
            break;
        }
        case '[': {
            ss << "[";
            int length = ((ArrayElementValue *) inner_ev)->num_values;
            for (int pos = 0; pos < length; pos++) {
                // bug 3 ！！调试时间最长的 bug！！在这里我不小心定义了和这个函数的参数一模一样的 inner_ev 变量！！在程序走到这里的时候，发生了如下错误！！重新定义一个和原先名字一模一样的变量 clang++ 竟然不报错吗 ???
                /**
                    就好像:

                    void haha(char **argv)
                    {
                        {
                            char **argv = argv;	// 不会有错误提示！唉（  g++ 和 clang++ 都没有错误提示...... 万一写错了咋办......看来只能自己小心了...
                        }
                    };

                    */
                // element_value *inner_ev = &((array_value_t *)inner_ev->value)->values[pos];	// error: Couldn't apply expression side effects : Couldn't dematerialize a result variable: couldn't read its memory
                ElementValue *inner_ev_2 = ((ArrayElementValue *) inner_ev)->values[pos];
                ss << parse_inner_element_value(inner_ev_2);        // recursive !
                if (pos != length - 1) ss << ",";
            }
            ss << "]";
            break;
        }
        default: {
            std::cerr << "can't get here. in element_value." << std::endl;
            assert(false);
        }
    }
    return ss.str();
};

std::string recursive_parse_annotation(AnnotationElementValue *target) {
    std::stringstream total_str;
    total_str << "#" << target->type_index << "(";    // print key: #12

    // bool is_first = false;
    for (int j = 0; j < target->num_element_value_pairs; j++) {
        total_str << "#" << target->element_value_pairs[j]->element_name_index << "=";    // inner value's key: #13
        ElementValue *inner_ev = target->element_value_pairs[j]->value;
        total_str << parse_inner_element_value(inner_ev);
        if (j != target->num_element_value_pairs - 1) total_str << ",";
    }

    total_str << ")";
    return total_str.str();
};

void print_attributes(AttributeInfo *ptr, ConstantPool *cp) {
    auto constant_pool = cp->getConstantPool();
    int attribute_tag = AttributeInfo::attributeName2Tag(ptr->attribute_name_index, cp);
    switch (attribute_tag) {
        case 0: {        // ConstantValue
            std::cout << "(DEBUG)   ConstantValue: ";
            u2 index = ((ConstantValueAttribute *) ptr)->constant_value_index;
            switch (constant_pool[index - 1]->tag) {
                case CONSTANT_Long: {
                    std::cout << "long " << ((CONSTANT_Long_info *) constant_pool[index - 1])->get_value() << "l"
                              << std::endl;
                    break;
                }
                case CONSTANT_Float: {
                    float result = ((CONSTANT_Float_info *) constant_pool[index - 1])->get_value();
                    std::cout << "float ";
                    if (result == FLOAT_INFINITY)
                        std::cout << "Infinityf" << std::endl;
                    else if (result == FLOAT_NEGATIVE_INFINITY)
                        std::cout << "-Infinityf" << std::endl;
                    else if (result == FLOAT_NAN)
                        std::cout << "NaNf" << std::endl;
                    else
                        std::cout << result << "f" << std::endl;
                    break;
                }
                case CONSTANT_Integer: {
                    std::cout << "int " << ((CONSTANT_Integer_info *) constant_pool[index - 1])->get_value()
                              << std::endl;
                    break;
                }
                case CONSTANT_Double: {
                    double result = ((CONSTANT_Double_info *) constant_pool[index - 1])->get_value();
                    std::cout << "double ";
                    if (result == DOUBLE_INFINITY)
                        std::cout << "Infinityd" << std::endl;
                    else if (result == DOUBLE_NEGATIVE_INFINITY)
                        std::cout << "-Infinityd" << std::endl;
                    else if (result == DOUBLE_NAN)
                        std::cout << "NaNd" << std::endl;
                    else
                        std::cout << result << "d" << std::endl;
                    break;
                }
                case CONSTANT_String: {
                    // search for two times.
                    u2 string_index = ((CONSTANT_String_info *) constant_pool[index - 1])->index;
                    assert (constant_pool[string_index - 1]->tag == CONSTANT_Utf8);
                    std::cout << "String ";
                    // std::wcout.imbue(std::locale(""));
                    std::wcout << ((CONSTANT_Utf8_info *) constant_pool[string_index - 1])->getConstant()
                               << std::endl;
                    wcout.clear();        // 防止 fuckking 拉丁文。
                    break;
                }
                default: {
                    std::cerr << "can't get here! in print_attributes()." << std::endl;
                    assert (false);
                }
            }
            break;
        }
        case 1: {    // Code
            std::cout << "(DEBUG)   Code: " << std::endl;
            auto *code_ptr = (CodeAttribute *) ptr;
            std::cout << "(DEBUG)     stack=" << code_ptr->max_stack << ", locals="
                      << code_ptr->max_locals /*<< ", args_size=" << args_size*/
                      << std::endl;    // output arg_size need parse descriptor. not important.
            // output bccode
            for (int bc_num = 0; bc_num < code_ptr->code_length; bc_num++) {
                auto *code = code_ptr->code;
                std::cout << "(DEBUG)     ";
                // print
                switch (code[bc_num]) {
                    case 0xb2: {        // getstatic
                        printf("%3d: %-15s #%d", bc_num, bccode_map[code[bc_num]].first.c_str(),
                               ((code[bc_num + 1] << 8) | code[bc_num + 2]));
                        break;
                    }

                    case 0xb6:        // invokevirtual
                    case 0xb7:        // invokespecial
                    case 0xb8:        // invokestatic
                    case 0xb9: {        // invokeinterface
                        printf("%3d: %-15s #%d", bc_num, bccode_map[code[bc_num]].first.c_str(),
                               ((code[bc_num + 1] << 8) | code[bc_num + 2]));
                        break;
                    }

                    case 0xbb: {        // new
                        printf("%3d: %-15s #%d", bc_num, bccode_map[code[bc_num]].first.c_str(),
                               ((code[bc_num + 1] << 8) | code[bc_num + 2]));
                        break;
                    }

                    case 0xc7: {        // ifnonnull
                        printf("%3d: %-15s #%d", bc_num, bccode_map[code[bc_num]].first.c_str(),
                               (bc_num + ((code[bc_num + 1] << 8) | code[bc_num + 2])));
                        break;
                    }

                    default: {
                        if (bccode_map[code[bc_num]].second != -3) {        // wide 指令集由我在后边自行输出。
                            printf("%3d: %-15s", bc_num,
                                   bccode_map[code[bc_num]].first.c_str());        // other message is to big, I dont want to save them.
                        }
                    }
                }

                if (bccode_map[code[bc_num]].second >= 0) {
                    bc_num += bccode_map[code[bc_num]].second;
                } else {        // 变长参数 以及 扩展局部变量索引 分别被定义为 -1 -2。 这里需要改进 !!!!!
                    if (bccode_map[code[bc_num]].second == -1) {    // tableswitch: switch case 中连续的跳转表。比如连续的 1～5 跳转
                        int origin_bc_num = bc_num;    // tableswitch 指令的所在位置
                        // switch 指令的特性，即保证 defaultbytes1 在 4 字节边界对齐的特性使得 nops 必然存在。如果 tableswitch 自身正好占在 4 边界对齐的位置的话，那么很不幸 nops 会增大到 3 个以保证 defaultbytes 的对齐。
                        if (bc_num % 4 != 0) {
//							for (int temp = bc_num + 1; temp < bc_num + (4 - bc_num % 4) + 50; temp ++) {		// delete
//								std::cout << temp << ": " << (int)code[temp] << " (" << bccode_map[code[temp]].first << ")" << std::endl;
//							}
                            bc_num += (4 - bc_num % 4);        // jump off nops: [lookup/tableswitch] align to 4
                        } else {
                            bc_num += 4;    // 跳过 tableswitch 指令自身外加 3 个 nops。
                        }
                        int ptr = bc_num;
                        // calculate basic values
                        int defaultbyte = ((code[ptr] << 24) | (code[ptr + 1] << 16) | (code[ptr + 2] << 8) |
                                           (code[ptr + 3]));
                        int lowbyte = ((code[ptr + 4] << 24) | (code[ptr + 5] << 16) | (code[ptr + 6] << 8) |
                                       (code[ptr + 7]));
                        int highbyte = ((code[ptr + 8] << 24) | (code[ptr + 9] << 16) | (code[ptr + 10] << 8) |
                                        (code[ptr + 11]));
//						std::cout << defaultbyte << " " << lowbyte << " " << highbyte << std::endl;	// delete
                        int num = highbyte - lowbyte + 1;
                        ptr += 12;
                        // create jump_table
                        vector<int> jump_tbl;
                        for (int pos = 0; pos < num; pos++) {
                            int jump_pos = ((code[ptr] << 24) | (code[ptr + 1] << 16) | (code[ptr + 2] << 8) |
                                            (code[ptr + 3])) + origin_bc_num;
                            ptr += 4;
                            jump_tbl.push_back(jump_pos);
                        }
                        jump_tbl.push_back(defaultbyte + origin_bc_num);        // 额外多 push 一个 default 跳转址
//						for_each(jump_tbl.begin(), jump_tbl.end(), [](int n) { cout << n << " ";});	// delete
                        // print
                        std::cout << " {" << std::endl;
                        for (int pos = 0; pos < jump_tbl.size(); pos++) {
                            if (pos != jump_tbl.size() - 1) {
                                printf("(DEBUG)%20d", pos +
                                                      lowbyte);        // fix the bug: if switch...case is 4~7, I will print 1~4...
                            } else {
                                printf("(DEBUG)%20s", "default");
                            }
                            std::cout << ": " << jump_tbl[pos] << std::endl;
                        }
                        printf("(DEBUG)%6s}", " ");
                        // end
                        bc_num = ptr - 1;    // 别忘了循环结束之后 bc_num 默认 +1.
                    } else if (bccode_map[code[bc_num]].second ==
                               -2) {        // lookupswitch：switch case 中非连续的跳转表。比如非连续的 1~5, 50 的跳转。
                        int origin_bc_num = bc_num;
                        if (bc_num % 4 != 0) {
                            bc_num += (4 - bc_num % 4);
                        } else {
                            bc_num += 4;
                        }
                        int ptr = bc_num;
                        // calculate basic values
                        int defaultbyte = ((code[ptr] << 24) | (code[ptr + 1] << 16) | (code[ptr + 2] << 8) |
                                           (code[ptr + 3]));
                        int npairs = ((code[ptr + 4] << 24) | (code[ptr + 5] << 16) | (code[ptr + 6] << 8) |
                                      (code[ptr + 7]));
                        ptr += 8;
                        // create jump_table
                        map<int, int> jump_tbl;
                        for (int pos = 0; pos < npairs; pos++) {
                            int match_value = ((code[ptr] << 24) | (code[ptr + 1] << 16) | (code[ptr + 2] << 8) |
                                               (code[ptr + 3]));
                            int jump_pos = ((code[ptr + 4] << 24) | (code[ptr + 5] << 16) | (code[ptr + 6] << 8) |
                                            (code[ptr + 7])) + origin_bc_num;
                            ptr += 8;
                            jump_tbl.insert(make_pair(match_value, jump_pos));
                        }
                        jump_tbl.insert(
                                make_pair(INT_MAX, defaultbyte + origin_bc_num));        // 额外多 insert 一个 default 跳转址
                        // print
                        std::cout << " {" << std::endl;
                        for (auto iter: jump_tbl) {
                            if (iter.first != INT_MAX) {
                                printf("(DEBUG)%20d", iter.first);
                            } else {
                                printf("(DEBUG)%20s", "default");
                            }
                            std::cout << ": " << iter.second << std::endl;
                        }
                        printf("(DEBUG)%6s}", " ");
                        // end
                        bc_num = ptr - 1;    // 别忘了循环结束之后 bc_num 默认 +1.
                    } else {        // [wide] bytecode instruction
                        // 1. iinc_w
                        if (bccode_map[code[bc_num + 1]].first == "iinc") {    // 如果是扩展的 iinc 指令，那么一共占据 6 个 bytecode 位。
                            int indexbyte = ((code[bc_num + 2] << 8) | (code[bc_num + 3]));
                            int constbyte = ((code[bc_num + 4] << 8) | (code[bc_num + 5]));
                            printf("%3d: %-15s%d, %d", bc_num, (bccode_map[code[bc_num]].first + "--iinc_w").c_str(),
                                   indexbyte, constbyte);
                        } else {
                            std::cerr << "didn't support -3!" << std::endl;
                            assert(false);
                        }
                    }
                }
                std::cout << std::endl;        // ....... 还有其他的参数没有输出......????
            }
            // output code exception table
            if (code_ptr->exception_table_length != 0) {
                std::cout << "(DEBUG)     " << "Exception Table:" << std::endl;
                printf("(DEBUG)%8sfrom%6sto%2starget%4stype\n", "", "", "", "");
            }
            for (int pos = 0; pos < code_ptr->exception_table_length; pos++) {
                ExceptionTable *table = code_ptr->exception_table[pos];
                printf("(DEBUG)%8s%4d%4s%4d%4s%4d%4s%4d\n", "", table->start_pc, "", table->end_pc, "",
                       table->handler_pc, "", table->catch_type);
            }
            // output LineNumberTable、StackMapTable etc. (attributes inner Code Area)
            for (int pos = 0; pos < code_ptr->attributes_count; pos++) {
                print_attributes(code_ptr->attributes[pos], cp);    // print other attributes.
            }
            break;
        }
        case 2: {
        }
//        case 2: {    // StackMapTable (Inner Code Attribute)
//            auto *smt_ptr = (StackMapTable_attribute *) ptr;
//            std::cout << "(DEBUG)     StackMapTable: number_of_entries = " << smt_ptr->number_of_entries << std::endl;
//            for (int pos = 0; pos < smt_ptr->number_of_entries; pos++) {
//                auto *entry = smt_ptr->entries[pos];
//                if (entry->frame_type >= 0 && entry->frame_type <= 63) {
//                    auto *frame = (StackMapTable_attribute::same_frame *) entry;
//                    std::cout << "(DEBUG)       frame_type = " << dec << (int) frame->frame_type << "  /* same */"
//                              << std::endl;
//                } else if (entry->frame_type >= 64 && entry->frame_type <= 127) {
//                    auto *frame = (StackMapTable_attribute::same_locals_1_stack_item_frame *) entry;
//                    std::cout << "(DEBUG)       frame_type = " << dec << (int) frame->frame_type << "  /* stack_item */"
//                              << std::endl;
//                    std::wcout << "(DEBUG)         " << print_verfication(&frame->stack[0], 1, constant_pool)
//                               << std::endl;
//                } else if (entry->frame_type == 247) {
//                    auto *frame = (StackMapTable_attribute::same_locals_1_stack_item_frame_extended *) entry;
//                    std::cout << "(DEBUG)       frame_type = " << dec << (int) frame->frame_type
//                              << "  /* stack_item_extended */" << std::endl;
//                    std::cout << "(DEBUG)         offset_delta = " << frame->offset_delta << std::endl;
//                    std::wcout << "(DEBUG)         " << print_verfication(&frame->stack[0], 1, constant_pool)
//                               << std::endl;
//                } else if (entry->frame_type >= 248 && entry->frame_type <= 250) {
//                    auto *frame = (StackMapTable_attribute::chop_frame *) entry;
//                    std::cout << "(DEBUG)       frame_type = " << dec << (int) frame->frame_type << "  /* chop */"
//                              << std::endl;
//                    std::cout << "(DEBUG)         offset_delta = " << frame->offset_delta << std::endl;
//                } else if (entry->frame_type == 251) {
//                    auto *frame = (StackMapTable_attribute::same_frame_extended *) entry;
//                    std::cout << "(DEBUG)       frame_type = " << dec << (int) frame->frame_type
//                              << "  /* same_extended */" << std::endl;
//                    std::cout << "(DEBUG)         offset_delta = " << frame->offset_delta << std::endl;
//                } else if (entry->frame_type >= 252 && entry->frame_type <= 254) {
//                    auto *frame = (StackMapTable_attribute::append_frame *) entry;
//                    std::cout << "(DEBUG)       frame_type = " << dec << (int) frame->frame_type << "  /* append */"
//                              << std::endl;
//                    std::cout << "(DEBUG)         offset_delta = " << frame->offset_delta << std::endl;
//                    std::wcout << "(DEBUG)         "
//                               << print_verfication(frame->locals, frame->frame_type - 251, constant_pool) << std::endl;
//                } else if (entry->frame_type == 255) {
//                    auto *frame = (StackMapTable_attribute::full_frame *) entry;
//                    std::cout << "(DEBUG)       frame_type = " << dec << (int) frame->frame_type << "  /* full */"
//                              << std::endl;
//                    std::cout << "(DEBUG)         offset_delta = " << frame->offset_delta << std::endl;
//                    std::wcout << "(DEBUG)         "
//                               << print_verfication(frame->locals, frame->number_of_locals, constant_pool, 0)
//                               << std::endl;
//                    std::wcout << "(DEBUG)         "
//                               << print_verfication(frame->stack, frame->number_of_stack_items, constant_pool, 1)
//                               << std::endl;
//                } else {
//                    std::cerr << "can't get here! in peek_stackmaptable_frame()" << std::endl;
//                    assert(false);
//                }
//            }
//            break;
//        }
        case 3: {    // Exceptions
            std::cout << "(DEBUG)   Exceptions: " << std::endl;
            auto *throws_ptr = (ExceptionTableAttribute *) ptr;
            std::cout << "(DEBUG)     throws ";
            for (int k = 0; k < throws_ptr->number_of_exceptions; k++) {        // same as print_method().
                assert (constant_pool[throws_ptr->exception_index_table[k] - 1]->tag ==
                        CONSTANT_Class);    // throw a Exception class
                assert (constant_pool[
                                ((CONSTANT_Class_info *) constant_pool[throws_ptr->exception_index_table[k] -
                                                                       1])->index -
                                1]->tag == CONSTANT_Utf8);
                std::wcout << ((CONSTANT_Utf8_info *) constant_pool[
                        ((CONSTANT_Class_info *) constant_pool[throws_ptr->exception_index_table[k] - 1])->index -
                        1])->getConstant() << " ";
                // Exceptions_attribute -> CONSTANT_class_info -> CONSTANT_Utf8_info
            }
            std::cout << std::endl << "(DEBUG)" << std::endl;
            break;
        }
        case 4: {    // InnerClass
            std::cout << "(DEBUG) InnerClasses:" << std::endl;
            auto *inner_ptr = (InnerClassesAttribute *) ptr;
            for (int i = 0; i < inner_ptr->number_of_classes; i++) {
                // get all indexes
                int inner_class_info_index = inner_ptr->classes[i]->inner_class_info_index;
                int outer_class_info_index = inner_ptr->classes[i]->outer_class_info_index;
                int inner_name_index = inner_ptr->classes[i]->inner_name_index;
//				cout << "..." <<	inner_class_info_index << " "<< outer_class_info_index << " " << inner_name_index << endl;	// delete
                wstring inner_class_info = ((CONSTANT_Utf8_info *) constant_pool[
                        ((CONSTANT_Class_info *) constant_pool[inner_class_info_index - 1])->index -
                        1])->getConstant();
                // parse inner class access flags
                u2 access_flags = inner_ptr->classes[i]->inner_class_access_flags;
//				cout << "..." << access_flags << endl;	// delete
                stringstream ss;
                int flags_num = 0;
                if ((access_flags & ACC_PUBLIC) == ACC_PUBLIC) {
                    if (flags_num != 0) ss << " ";
                    ss << "public";
                    flags_num++;
                } else if ((access_flags & ACC_PRIVATE) ==
                           ACC_PRIVATE) {        // in fact, private and protected member shouldn't be output. haha.
                    if (flags_num != 0) ss << " ";
                    ss << "private";
                    flags_num++;
                } else if ((access_flags & ACC_PROTECTED) == ACC_PROTECTED) {
                    if (flags_num != 0) ss << " ";
                    ss << "protected";
                    flags_num++;
                }
                if ((access_flags & ACC_STATIC) == ACC_STATIC) {
                    if (flags_num != 0) ss << " ";
                    ss << "static";
                    flags_num++;
                }
                if ((access_flags & ACC_FINAL) == ACC_FINAL) {
                    if (flags_num != 0) ss << " ";
                    ss << "final";
                    flags_num++;
                }
                if ((access_flags & ACC_INTERFACE) == ACC_INTERFACE) {
                    if (flags_num != 0) ss << " ";
                    ss << "interface";
                    flags_num++;
                }
                if ((access_flags & ACC_ABSTRACT) == ACC_ABSTRACT) {
                    if (flags_num != 0) ss << " ";
                    ss << "abstract";
                    flags_num++;
                }
                if ((access_flags & ACC_SYNTHETIC) == ACC_SYNTHETIC) {
                    if (flags_num != 0) ss << " ";
                    ss << "synthetic";
                    flags_num++;
                }
                if ((access_flags & ACC_ANNOTATION) == ACC_ANNOTATION) {
                    if (flags_num != 0) ss << " ";
                    ss << "annotation";
                    flags_num++;
                }
                if ((access_flags & ACC_ENUM) == ACC_ENUM) {
                    if (flags_num != 0) ss << " ";
                    ss << "enum";
                    flags_num++;
                }
                // print
                // verify:	inner_name_index 和 outer_class_info_index 要为 0 必须全都为 0
                if (inner_name_index == 0) {
//					assert(outer_class_info_index == 0);		// *** 这里去掉了，并且加上了下方的 else if！！因为 openjdk 的 java/lang/String 特别诡异，parse 不了！！这个文件不符合 Spec 规范！！！但是 Oracle 的 Java SE 8 的能够 parse！！！
                }
                if (inner_name_index == 0 && outer_class_info_index == 0) {
                    std::wcout << "(DEBUG)    " << ss.str().c_str() << " #" << inner_class_info_index << "  //<class:> "
                               << inner_class_info << endl;    // wcout 只能输出 char * 和 wstring，但是不能输出 string！！
                } else if (inner_name_index == 0 && outer_class_info_index != 0) {
                    wstring outer_class_info = (outer_class_info_index != 0) ? ((CONSTANT_Utf8_info *) constant_pool[
                            ((CONSTANT_Class_info *) constant_pool[outer_class_info_index - 1])->index -
                            1])->getConstant()
                                                                             : L"";        // *** 防止 openjdk 8 java/lang/String parse 不了，会被 assert 给停止！
                    std::wcout << "(DEBUG)    " << ss.str().c_str() << " #" << outer_class_info_index << "  //<class:> "
                               << outer_class_info << endl;
                } else {
                    // extra: 但是这时，虽然能够保证 inner_name_index 不为 0，但是却不能保证 outer_class_info_index 不为 0！
                    std::wcout << "(DEBUG)    " << ss.str().c_str() << " #" << inner_name_index << "= #"
                               << inner_class_info_index << " of #" << outer_class_info_index;
                    wstring outer_class_info = (outer_class_info_index != 0) ? ((CONSTANT_Utf8_info *) constant_pool[
                            ((CONSTANT_Class_info *) constant_pool[outer_class_info_index - 1])->index -
                            1])->getConstant()
                                                                             : L"";
                    wstring inner_name = ((CONSTANT_Utf8_info *) constant_pool[inner_name_index -
                                                                               1])->getConstant();
                    std::wcout << "  //" << inner_name << "=<class:> " << inner_class_info << " of <class:> "
                               << ((outer_class_info_index != 0) ? outer_class_info : L"ANONYMOUS") << endl;
                }
            }
            break;
        }
        case 5: {    // Enclosing Method
            auto *enclosing_ptr = (EnclosingMethodAttribute *) ptr;
            std::cout << "(DEBUG) EnclosingMethod: #" << enclosing_ptr->class_index << ".#"
                      << enclosing_ptr->method_index;
            wstring class_info = ((CONSTANT_Utf8_info *) constant_pool[
                    ((CONSTANT_Class_info *) constant_pool[enclosing_ptr->class_index - 1])->index -
                    1])->getConstant();
            std::wcout << "   // " << class_info;
            if (enclosing_ptr->method_index != 0) {
                wstring method_info = ((CONSTANT_Utf8_info *) constant_pool[
                        ((CONSTANT_Class_info *) constant_pool[enclosing_ptr->method_index - 1])->index -
                        1])->getConstant();
                std::wcout << "." << method_info;
            }
            std::wcout << std::endl;
            break;
        }
        case 6: {    // Synthetic
            std::cout << "(DEBUG)   Synthetic: (length) " << ptr->attribute_length << std::endl;
            break;
        }
        case 7: {    // Signature
            auto *signature_ptr = (SignatureAttribute *) ptr;
            assert (constant_pool[signature_ptr->signature_index - 1]->tag == CONSTANT_Utf8);
            std::wcout << "(DEBUG)   Signature: #" << signature_ptr->signature_index << " "
                       << ((CONSTANT_Utf8_info *) constant_pool[signature_ptr->signature_index -
                                                                1])->getConstant() << std::endl;
            break;
        }
        case 8: {    // SourceFile
            auto *sourcefile_ptr = (SourceFileAttribute *) ptr;
            std::wcout << "(DEBUG) SourceFile: \""
                       << ((CONSTANT_Utf8_info *) constant_pool[sourcefile_ptr->source_file_index -
                                                                1])->getConstant() << "\"" << std::endl;
            break;
        }
        case 9: {    // SourceDebug (no use for jvm)
            break;
        }
        case 10: {    // LineNumberTable (Inside the Code Attribution)
            std::cout << "(DEBUG)     LineNumberTable:" << std::endl;
            auto *line_table = (LineNumberTableAttribute *) ptr;
            for (int pos = 0; pos < line_table->line_number_table_length; pos++) {
                auto *table = line_table->line_number_table[pos];
                printf("(DEBUG)%7sline: %4d, start_pc: %-4d\n", "", table->line_number, table->start_pc);
            }
            break;
        }
        case 11: {    // LocalVariableTable (no use for jvm, use for Debugger.)
            break;
        }
        case 12: {    // LocalVariableTypeTable (no use for jvm, use for Debugger.)
            break;
        }
        case 13: {    // Deprecated
            std::wcout << "(DEBUG)   Deprecated: true" << std::endl;
            break;
        }
        case 14:
        case 15: {    // Runtime(In)VisibleAnnotations
            if (attribute_tag == 14)
                std::cout << "(DEBUG)   RuntimeVisibleAnnotations:" << std::endl;
            else
                std::cout << "(DEBUG)   RuntimeInisibleAnnotations:" << std::endl;
            auto *annotations_ptr = (RuntimeVisibleAnnotations_attribute *) ptr;
            for (int i = 0; i < annotations_ptr->parameter_annotations.num_annotations; i++) {
                AnnotationElementValue *target = annotations_ptr->parameter_annotations.annotations[i];
                std::cout << "(DEBUG)     " << i << ": ";
                std::cout << recursive_parse_annotation(target) << std::endl;
            }
            break;
        }
        case 16:
        case 17: {    // Runtime(In)VisibleParameterAnnotations
            if (attribute_tag == 16)
                std::cout << "(DEBUG)   RuntimeVisibleParameterAnnotations:" << std::endl;
            else
                std::cout << "(DEBUG)   RuntimeInisibleParameterAnnotations:" << std::endl;
            auto *annotations_ptr = (RuntimeVisibleParameterAnnotations_attribute *) ptr;
            for (int i = 0; i < annotations_ptr->num_parameters; i++) {
                for (int j = 0; j < annotations_ptr->parameter_annotations[i]->num_annotations; j++) {
                    AnnotationElementValue *target = annotations_ptr->parameter_annotations[i]->annotations[j];
                    std::cout << "(DEBUG)     " << j << ": ";
                    std::cout << recursive_parse_annotation(target) << std::endl;
                }
            }
            break;
        }
        case 18:
        case 19: {    // Runtime(In)VisibleTypeAnnotations
            if (attribute_tag == 18)
                std::cout << "(DEBUG)   RuntimeVisibleTypeAnnotations:" << std::endl;
            else
                std::cout << "(DEBUG)   RuntimeInisibleTypeAnnotations:" << std::endl;
            auto *annotations_ptr = (RuntimeVisibleTypeAnnotations_attribute *) ptr;
            for (int i = 0; i < annotations_ptr->num_annotations; i++) {
                type_annotation *ta = &annotations_ptr->annotations[i];
                // 1. print [annotation]
                AnnotationElementValue *target = ta->anno;
                std::cout << "(DEBUG)     ";
                std::cout << recursive_parse_annotation(target) << std::endl;
                // 2. print [target_info]
                std::cout << "(DEBUG)     ";
                std::cout << "tag == " << hex << (int) ta->target_type << " ";    // TODO: NEW / CLASS_EXTENSIONS 信息显示
                switch (ta->target_type) {
                    case 0x00:
                    case 0x01: {    // target_info is [type_parameter_target]
                        auto target = (type_annotation::type_parameter_target *) ta->target_info;
                        std::cout << i << ": [type_parameter_target] " << "#" << target->type_parameter_index
                                  << std::endl;
                        break;
                    }
                    case 0x10: {    // target_info is [supertype_target]
                        auto target = (type_annotation::supertype_target *) ta->target_info;
                        std::cout << i << ": [supertype_target] " << "#" << target->supertype_index << std::endl;
                        break;
                    }
                    case 0x11:
                    case 0x12: {    // target_info is [type_parameter_bound_target]
                        auto target = (type_annotation::type_parameter_bound_target *) ta->target_info;
                        std::cout << i << ": [type_parameter_bound_target] " << "#" << target->type_parameter_index
                                  << " #" << target->bound_index << std::endl;
                        break;
                    }
                    case 0x13:
                    case 0x14:
                    case 0x15: {    // target_info is [empty_target]
                        std::cout << i << ": [empty_target]" << std::endl;
                        break;
                    }
                    case 0x16: {    // target_info is [formal_parameter_target]
                        auto target = (type_annotation::formal_parameter_target *) ta->target_info;
                        std::cout << i << ": [formal_parameter_target] " << "#" << target->formal_parameter_index
                                  << std::endl;
                        break;
                    }
                    case 0x17: {    // target_info is [throws_target]
                        auto target = (type_annotation::throws_target *) ta->target_info;
                        std::cout << i << ": [throws_target] " << "#" << target->throws_type_index << std::endl;
                        break;
                    }
                    case 0x40:
                    case 0x41: {    // target_info is [localvar_target]
                        auto target = (type_annotation::localvar_target *) ta->target_info;
                        std::cout << i << ": [localvar_target] " << std::endl;
                        for (int pos = 0; pos < target->table_length; pos++) {
                            auto table = target->table[pos];
                            std::cout << "(DEBUG)       " << "index: " << table.index << ", start_pc: "
                                      << table.start_pc << ", length: " << table.length << std::endl;
                        }
                        break;
                    }
                    case 0x42: {    // target_info is [catch_target]
                        auto target = (type_annotation::catch_target *) ta->target_info;
                        std::cout << i << ": [catch_target] " << "#" << target->exception_table_index << std::endl;
                        break;
                    }
                    case 0x43:
                    case 0x44:
                    case 0x45:
                    case 0x46: {    // target_info is [offset_target]
                        auto target = (type_annotation::offset_target *) ta->target_info;
                        std::cout << i << ": [offset_target] " << "#" << target->offset << std::endl;
                        break;
                    }
                    case 0x47:
                    case 0x48:
                    case 0x49:
                    case 0x4A:
                    case 0x4B: {    // target_info is [type_argument_target]
                        auto target = (type_annotation::type_argument_target *) ta->target_info;
                        std::cout << i << ": [type_argument_target] " << "#" << target->offset << " #"
                                  << target->type_argument_index << std::endl;
                        break;
                    }
                    default: {
                        std::cerr << "can't get here!" << std::endl;
                        assert(false);
                    }
                }
                // 3. print [target_path]
                type_annotation::type_path *path_ptr = &ta->target_path;
                if (path_ptr->path_length != 0)
                    std::cout << "(DEBUG)     ";
                for (int pos = 0; pos < path_ptr->path_length; pos++) {
                    std::cout << "type_path_kind: " << (int) path_ptr->path[pos].type_path_kind
                              << "; type_argument_index: " << (int) path_ptr->path[pos].type_argument_index
                              << std::endl;
                }
            }
            break;
        }
        case 20: {    // Annotation Default
            std::cout << "(DEBUG)    AnnotationDefault:" << std::endl;
            std::cout << "(DEBUG)      default_value: ";
            auto *default_ptr = (AnnotationDefaultAttribute *) ptr;
            ElementValue *element_value = default_ptr->default_value;
            // 1. print [type]
            std::cout << (char) element_value->tag;
            // 2. print [annotation default value pos in constant_pool]
            std::cout << parse_inner_element_value(element_value) << std::endl;    // 我都忘了已经写过现成的递归函数了......
            break;
        }
        case 21: {    // BootstrapMethods	// 注：此表全局唯一。是所有的 lambda 动态调用的方法的集合表。invokeDynamic 通过此表查找想要 invoke 的方法。
            std::cout << "(DEBUG)  BootstrapMethods:" << std::endl;
            auto lambda_tbl = (BootstrapMethodsAttribute *) ptr;
            for (int pos = 0; pos < lambda_tbl->num_bootstrap_methods; pos++) {
                BootstrapMethodsAttribute::bootstrap_methods_t *method_handle_msg = lambda_tbl->bootstrap_methods[pos];
                // 1. parse MethodHandle pos in constant_pool
                std::cout << "(DEBUG)    " << pos << ": [CONSTANT_MethodHandle pos]#"
                          << method_handle_msg->bootstrap_method_ref << std::endl;
                // 2. parse MethodHandle arguments
                std::cout << "(DEBUG)      Method arguments:" << std::endl;
                for (int k = 0; k < method_handle_msg->num_bootstrap_arguments; k++) {
                    int rtpool_ref = method_handle_msg->bootstrap_arguments[k];
                    std::cout << "(DEBUG)        #" << rtpool_ref << " " << std::endl;
                }
            }
            break;
        }
        case 22: {    // MethodParameters	// 注：这个属性只有在：用 java8 编译 + 使用 java.lang.reflect.Parameter 类库 + 用 `javac -parameters` 来进行编译的时候才会生效！ MethodParameters 属性会被注入到 .class 文件中去！
            std::cout << "(DEBUG)  MethodParameters:" << std::endl;
            auto parameters = (MethodParametersAttribute *) ptr;
            for (int pos = 0; pos < parameters->parameters_count; pos++) {
                int name_index = parameters->parameters[pos]->name_index;
                int access_flags = parameters->parameters[pos]->access_flags;
                if (name_index == 0) {
                    std::cout << "(DEBUG)    NONAME"
                              << std::endl;        // TODO: Java8 Specification $4.7.24 指出这里有可能是 0。不过还没有试验出来...
                } else {
                    assert(constant_pool[name_index - 1]->tag == CONSTANT_Utf8);
                    wstring name = ((CONSTANT_Utf8_info *) constant_pool[name_index - 1])->getConstant();
                    // 1. print [name]
                    std::wcout << "(DEBUG)    " << name << " ";
                    // 2. print [access_flags]
                    wstringstream ss;
                    if ((access_flags & ACC_FINAL) == ACC_FINAL) {
                        ss << L"final ";
                    }
                    if ((access_flags & ACC_SYNTHETIC) == ACC_SYNTHETIC) {
                        ss << L"synthetic ";
                    }
                    if ((access_flags & ACC_MANDATED) == ACC_MANDATED) {
                        ss << L"mandated ";
                    }
                    std::wcout << ss.str() << std::endl;
                }
            }
            break;
        }
        default: {
            std::cerr << "can't get here!" << std::endl;
            assert(false);
        }
    }
}

void print_methods(MethodInfo **bufs, int length, ConstantPool *constant_pool) {
    std::cout << "(DEBUG)" << std::endl;
    for (int i = 0; i < length; i++) {
        std::stringstream ss;
        // parse access_flags
        int flags_num = 0;
        if ((bufs[i]->access_flags & ACC_PUBLIC) == ACC_PUBLIC) {
            std::cout << "(DEBUG) public ";
            if (flags_num != 0) ss << ", ";
            ss << "ACC_PUBLIC";
            flags_num++;
        } else if ((bufs[i]->access_flags & ACC_PRIVATE) ==
                   ACC_PRIVATE) {        // in fact, private and protected member shouldn't be output. haha.
            std::cout << "(DEBUG) private ";
            if (flags_num != 0) ss << ", ";
            ss << "ACC_PRIVATE";
            flags_num++;
        } else if ((bufs[i]->access_flags & ACC_PROTECTED) == ACC_PROTECTED) {
            std::cout << "(DEBUG) protected ";
            if (flags_num != 0) ss << ", ";
            ss << "ACC_PROTECTED";
            flags_num++;
        } else {
            std::cout
                    << "(DEBUG) ";        // using for <clinit> : because <clinit> doesn't have a public/private/protected.
        }
        if ((bufs[i]->access_flags & ACC_STATIC) == ACC_STATIC) {
            std::cout << "static ";
            if (flags_num != 0) ss << ", ";
            ss << "ACC_STATIC";
            flags_num++;
        }
        if ((bufs[i]->access_flags & ACC_FINAL) == ACC_FINAL) {
            std::cout << "final ";
            if (flags_num != 0) ss << ", ";
            ss << "ACC_FINAL";
            flags_num++;
        }
        if ((bufs[i]->access_flags & ACC_SYNCHRONIZED) == ACC_SYNCHRONIZED) {
            std::cout << "synchronized ";
            if (flags_num != 0) ss << ", ";
            ss << "ACC_SYNCHRONIZED";
            flags_num++;
        }
        if ((bufs[i]->access_flags & ACC_BRIDGE) == ACC_BRIDGE) {
            std::cout << "bridge ";
            if (flags_num != 0) ss << ", ";
            ss << "ACC_BRIDGE";
            flags_num++;
        }
        if ((bufs[i]->access_flags & ACC_VARARGS) == ACC_VARARGS) {
            std::cout << "va_args ";
            if (flags_num != 0) ss << ", ";
            ss << "ACC_VARARGS";
            flags_num++;
        }
        if ((bufs[i]->access_flags & ACC_NATIVE) == ACC_NATIVE) {
            std::cout << "native ";
            if (flags_num != 0) ss << ", ";
            ss << "ACC_NATIVE";
            flags_num++;
        }
        if ((bufs[i]->access_flags & ACC_ABSTRACT) == ACC_ABSTRACT) {
            std::cout << "abstract ";
            if (flags_num != 0) ss << ", ";
            ss << "ACC_ABSTRACT";
            flags_num++;
        }
        if ((bufs[i]->access_flags & ACC_STRICT) == ACC_STRICT) {
            std::cout << "strict ";
            if (flags_num != 0) ss << ", ";
            ss << "ACC_STRICT";
            flags_num++;
        }
        if ((bufs[i]->access_flags & ACC_SYNTHETIC) == ACC_SYNTHETIC) {
            std::cout << "synthetic ";
            if (flags_num != 0) ss << ", ";
            ss << "ACC_SYNTHETIC";
            flags_num++;
        }
        // parse name_index
        assert (constant_pool->getConstantPool()[bufs[i]->name_index - 1]->tag == CONSTANT_Utf8);
        std::wstring method_name = ((CONSTANT_Utf8_info *) constant_pool->getConstantPool()[bufs[i]->name_index -
                                                                                            1])->getConstant();    // get function_name
        // first parse Exception_attribute to output name!! because there should be output `throws messages`! in fact this should be written in print_attributes() function...
        bool has_exception = false;
        for (int pos = 0; pos < bufs[i]->attributes_count; pos++) {
            int attribute_tag = AttributeInfo::attributeName2Tag(bufs[i]->attributes[pos]->attribute_name_index,
                                                                 constant_pool);
            if (attribute_tag == 3) {
                if (!has_exception) {
                    has_exception = true;
                    std::cout << " throws ";
                }
                auto *throws_ptr = (ExceptionTableAttribute *) bufs[i]->attributes[pos];
                for (int k = 0; k < throws_ptr->number_of_exceptions; k++) {
                    assert (constant_pool->getConstantPool()[throws_ptr->exception_index_table[k] - 1]->tag ==
                            CONSTANT_Class);    // throw a Exception class
                    assert (constant_pool->getConstantPool()[((CONSTANT_Class_info *) constant_pool->getConstantPool()[
                            throws_ptr->exception_index_table[k] -
                            1])->index - 1]->tag == CONSTANT_Utf8);
                    std::wcout << ((CONSTANT_Utf8_info *) constant_pool->getConstantPool()[
                            (
                                    (CONSTANT_Class_info *)
                                            constant_pool->getConstantPool()[throws_ptr->exception_index_table[k] - 1]
                            )->index - 1
                    ])->getConstant() << " ";
                    // Exceptions_attribute -> CONSTANT_class_info -> CONSTANT_Utf8_info
                }
            }
        }
        std::wcout << method_name << L";\n";
        // parse descriptor_index
        assert (constant_pool->getConstantPool()[bufs[i]->descriptor_index - 1]->tag == CONSTANT_Utf8);
        std::wcout << "(DEBUG)   descriptor: "
                   << ((CONSTANT_Utf8_info *) constant_pool->getConstantPool()[bufs[i]->descriptor_index -
                                                                               1])->getConstant()
                   << std::endl;
        // output flags
        std::cout << "(DEBUG)   flags: " << ss.str() << std::endl;
        // parse ConstantValue / Signature / RuntimeVisibleAnnotations / RuntimeInvisibleAnnotations
        for (int pos = 0; pos < bufs[i]->attributes_count; pos++) {
            int attribute_tag = AttributeInfo::attributeName2Tag(bufs[i]->attributes[pos]->attribute_name_index,
                                                                 constant_pool);
            if (!(attribute_tag == 1 || attribute_tag == 3 || attribute_tag == 6 ||
                  attribute_tag == 7 || attribute_tag == 13 || attribute_tag == 14 ||
                  attribute_tag == 15 || attribute_tag == 16 || attribute_tag == 17 ||
                  attribute_tag == 18 || attribute_tag == 19 || attribute_tag == 20 || attribute_tag == 22)) {
                // ignore (not 1, 3, 6, 7, 13, 14, 15, 16, 17, 18, 19, 20, 22). in Java SE 8 Specification $ 4.5
                continue;
            }
            print_attributes(bufs[i]->attributes[pos],
                             constant_pool);        // I only implemented the '0' --> ConstantValue output. ?????????
            // note: the args_size only used for Code_attribute.
        }
        std::cout << "(DEBUG)" << std::endl;
    }
}

void print_constant_pool(ConstantPool *constantPool) {
    const vector<Constant *> &constantPoolVector = constantPool->getConstantPool();
    for (int i = 0; i < constantPool->constantPoolCount_; i++) {
        switch (constantPoolVector[i]->tag) {
            case CONSTANT_Class:
            case CONSTANT_String: {
                CONSTANT_String_info *target = (CONSTANT_String_info *) constantPoolVector[i];
                if (target->tag == CONSTANT_Class)
                    printf("(DEBUG) #%4d = Class %15s #%d\n", i + 1, "", target->index);
                else
                    printf("(DEBUG) #%4d = String %14s #%d\n", i + 1, "", target->index);
                break;
            }
            case CONSTANT_Fieldref:
            case CONSTANT_Methodref:
            case CONSTANT_InterfaceMethodref: {
                CONSTANT_FMI_info *target = (CONSTANT_FMI_info *) constantPoolVector[i];
                if (target->tag == CONSTANT_Fieldref)
                    printf("(DEBUG) #%4d = Fieldref %12s #%d.#%d\n", i + 1, "", target->class_index,
                           target->name_and_type_index);
                else if (target->tag == CONSTANT_Methodref)
                    printf("(DEBUG) #%4d = Methodref %11s #%d.#%d\n", i + 1, "", target->class_index,
                           target->name_and_type_index);
                else
                    printf("(DEBUG) #%4d = InterfaceMethodref %2s #%d.#%d\n", i + 1, "", target->class_index,
                           target->name_and_type_index);
                break;
            }
            case CONSTANT_Integer: {
                CONSTANT_Integer_info *target = (CONSTANT_Integer_info *) constantPoolVector[i];
                printf("(DEBUG) #%4d = Integer %13s %di\n", i + 1, "", target->get_value());
                break;
            }
            case CONSTANT_Float: {
                CONSTANT_Float_info *target = (CONSTANT_Float_info *) constantPoolVector[i];
                float result = target->get_value();
                if (result == FLOAT_INFINITY)
                    printf("(DEBUG) #%4d = Float %15s Infinityf\n", i + 1, "");
                else if (result == FLOAT_NEGATIVE_INFINITY)
                    printf("(DEBUG) #%4d = Float %15s -Infinityf\n", i + 1, "");
                else if (result == FLOAT_NAN)
                    printf("(DEBUG) #%4d = Float %15s NaNf\n", i + 1, "");
                else
                    printf("(DEBUG) #%4d = Float %15s %ff\n", i + 1, "", result);
                break;
            }
            case CONSTANT_Long: {
                CONSTANT_Long_info *target = (CONSTANT_Long_info *) constantPoolVector[i];
                printf("(DEBUG) #%4d = Long %16s %ldl\n", i + 1, "", target->get_value());
                i++;
                break;
            }
            case CONSTANT_Double: {
                CONSTANT_Double_info *target = (CONSTANT_Double_info *) constantPoolVector[i];
                double result = target->get_value();
                if (result == DOUBLE_INFINITY)
                    printf("(DEBUG) #%4d = Double %14s Infinityd\n", i + 1, "");
                else if (result == DOUBLE_NEGATIVE_INFINITY)
                    printf("(DEBUG) #%4d = Double %14s -Infinityd\n", i + 1, "");
                else if (result == DOUBLE_NAN)
                    printf("(DEBUG) #%4d = Double %14s NaNd\n", i + 1, "");
                else
                    printf("(DEBUG) #%4d = Double %14s %lfd\n", i + 1, "", result);
                i++;
                break;
            }
            case CONSTANT_NameAndType: {
                CONSTANT_NameAndType_info *target = (CONSTANT_NameAndType_info *) constantPoolVector[i];
                printf("(DEBUG) #%4d = NameAndType %9s #%d.#%d\n", i + 1, "", target->name_index,
                       target->descriptor_index);
                break;
            }
            case CONSTANT_Utf8: {
                CONSTANT_Utf8_info *target = (CONSTANT_Utf8_info *) constantPoolVector[i];
                printf("(DEBUG) #%4d = Utf8 %16s ", i + 1, "");
                String str = fromBytes(target->bytes, target->length);
                cout << toStdString(str) << endl;
                break;
            }
            case CONSTANT_MethodHandle: {
                CONSTANT_MethodHandle_info *target = (CONSTANT_MethodHandle_info *) constantPoolVector[i];
                const char *ref_kind;
                // 1. judge REF_kind
                switch (target->reference_kind) {
                    case REF_getField:
                        ref_kind = "[getField]";
                        break;
                    case REF_getStatic:
                        ref_kind = "[getStatic]";
                        break;
                    case REF_putField:
                        ref_kind = "[putField]";
                        break;
                    case REF_putStatic:
                        ref_kind = "[putStatic]";
                        break;
                    case REF_invokeVirtual:
                        ref_kind = "[invokeVirtual]";
                        break;
                    case REF_invokeStatic:
                        ref_kind = "[invokeStatic]";
                        break;
                    case REF_invokeSpecial:
                        ref_kind = "[invokeSpecial]";
                        break;
                    case REF_newInvokeSpecial:
                        ref_kind = "[newInvokeSpecial]";
                        break;
                    case REF_invokeInterface:
                        ref_kind = "[invokeInterface]";
                        break;
                    default:
                        std::cerr << "can't get here!" << std::endl;
                        assert(false);
                }
                printf("(DEBUG) #%4d = MethodHandle %8s %s:#%d\n", i + 1, "", ref_kind, target->reference_index);
                break;
            }
            case CONSTANT_MethodType: {
                CONSTANT_MethodType_info *target = (CONSTANT_MethodType_info *) constantPoolVector[i];
                printf("(DEBUG) #%4d = MethodType %10s #%d\n", i + 1, "", target->descriptor_index);
                break;
            }
            case CONSTANT_InvokeDynamic: {
                CONSTANT_InvokeDynamic_info *target = (CONSTANT_InvokeDynamic_info *) constantPoolVector[i];
                printf("(DEBUG) #%4d = InvokeDynamic %7s #%d:#%d\n", i + 1, "", target->bootstrap_method_attr_index,
                       target->name_and_type_index);
                break;
            }
            default: {
                std::cerr << constantPoolVector[i]->tag << ":" << i << " "
                          << "didn't finish MethodHandle, MethodType and InvokeDynamic!!" << std::endl;
                assert(false);
            }
        }
    }
}

void ClassFile::readMagic() {
    u1 c1 = reader.readUint8();
    u1 c2 = reader.readUint8();
    u1 c3 = reader.readUint8();
    u1 c4 = reader.readUint8();
    magic = c1 << 24 | c2 << 16 | c3 << 8 | c4;
    if (magic != 0xCAFEBABE) {
        cout << "Bad magic value" << endl;
        exit(1);
    } else {
        cout << "ok magic value" << endl;
    }
}

void ClassFile::parse() {
    readMagic();
    readVersion();
    readConstantPool();
    readClassInfo();
    readInterfaces();
    readFields();
    readMethods();
    print_methods(methods, methods_count, constantPool);
    readAttributes();
}

void ClassFile::readVersion() {
    minor_version = reader.readUint16();
    major_version = reader.readUint16();
    cout << "version: " << minor_version << ":" << major_version << endl;
}

void ClassFile::readConstantPool() {
    constantPool = new ConstantPool(reader);
}

void ClassFile::readClassInfo() {
    access_flags = reader.readUint16();
    if ((access_flags & ACC_INTERFACE) != 0) {
        access_flags |= ACC_ABSTRACT;
    }
    if ((access_flags & ACC_ABSTRACT) != 0
        && (access_flags & ACC_FINAL) != 0) {
        assert("Class can't be both final and abstract");
    }
    this_class = reader.readUint16();
    super_class = reader.readUint16();
}

void ClassFile::readInterfaces() {
    interfaces_count = reader.readUint16();
    interfaces = new u2[interfaces_count];
    for (int i = 0; i < interfaces_count; ++i) {
        interfaces[i] = reader.readUint16();
    }
}

void ClassFile::readFields() {
    fields_count = reader.readUint16();
    if (fields_count == 0) return;
    fields = new FieldInfo *[fields_count];
    for (int i = 0; i < fields_count; ++i) {
        fields[i] = new FieldInfo(reader, constantPool);
    }
}

void ClassFile::readMethods() {
    methods_count = reader.readUint16();
    if (methods_count != 0)
        methods = new MethodInfo *[methods_count];
    for (int pos = 0; pos < methods_count; pos++) {
        methods[pos] = new MethodInfo(reader, constantPool, 1);
    }
    if (methods_count != 0) {
        std::cout << "(DEBUG) method_number: " << methods_count << std::endl;
    } else {
        std::cout << "(DEBUG) no method functions." << std::endl;
    }
}

void ClassFile::readAttributes() {
    attributes_count = reader.readUint16();
    if (attributes_count != 0)
        attributes = new AttributeInfo *[attributes_count];
    for (int pos = 0; pos < attributes_count; pos++) {
        attributes[pos] = AttributeInfo::readAttribute(reader, constantPool);
    }
#ifdef POOL_DEBUG
    if (attributes_count != 0) {
        std::cout << "(DEBUG) attribute_number: " << attributes_count << std::endl;
        for (int pos = 0; pos < attributes_count; pos ++) {
            print_attributes(attributes[pos], constant_pool);
        }
    } else {
        std::cout << "(DEBUG) no class attributes." << std::endl;
    }
#endif
}
