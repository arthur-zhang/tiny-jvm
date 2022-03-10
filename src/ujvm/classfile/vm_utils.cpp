
#include "vm_utils.h"

strings::String decodeMUTF8(const u1 *inBytes, size_t len) {
    u2 out[len];
    u2 a, b, c;
    int count = 0;
    int pos = 0;

    while (count < len) {
        a = (u2) inBytes[count] & 0xff;
        if (a <= 127) {  /* 0xxxxxxx*/
            count++;
            out[pos++] = a;
        } else if ((a & 0XE0) == 0XC0) { /* 110x xxxx   10xx xxxx*/
            count += 2;
            if (count > len) PANIC("malformed input: partial character at end");
            b = (u2) inBytes[count - 1];
            if ((b & 0xC0) != 0x80) PANIC("bad second byte");
            out[pos++] = (u2) (((a & 0x1F) << 6) | (b & 0x3F));
        } else if ((a & 0XF0) == 0XE0) {  /* 1110 xxxx  10xx xxxx  10xx xxxx */
            count += 3;
            if (count > len) PANIC("malformed input: partial character at end");
            b = (u2) inBytes[count - 2];
            c = (u2) inBytes[count - 1];
            if (((b & 0xC0) != 0x80) || ((c & 0xC0) != 0x80)) PANIC("bad second or third byte");
            out[pos++] = (u2) (((a & 0x0F) << 12) | ((b & 0x3F) << 6) | ((c & 0x3F) << 0));
        } else {
            PANIC("bad byte");
        }
    }
    return strings::String(out, out + pos);
}