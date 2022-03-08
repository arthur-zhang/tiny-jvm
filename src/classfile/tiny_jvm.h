#pragma once
#define PANIC(fmt, ...) \
    do { \
       (void) fprintf(stderr, "\n\n\n*** (panic) *** [%s:%d]: " fmt "\n\n\n", \
                __FILE__, __LINE__, ##__VA_ARGS__); \
       exit(1); \
    } while (false)