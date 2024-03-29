#pragma  once

#include "shared/types.h"
#include "class_reader.h"
#include "shared/jstring.h"
#include "constant_pool.h"
#include "element_value.h"

class Annotations {
public:
    Annotations(ClassReader &reader) {
        num_annotations = reader.readUInt16();
        if (num_annotations != 0)
            annotations = new AnnotationEntry *[num_annotations];
        for (int pos = 0; pos < num_annotations; pos++) {
            annotations[pos] = new AnnotationEntry(reader); // todo
        }
    }

    u2 num_annotations;
    AnnotationEntry **annotations = nullptr;    // [num_annotations]

    void dump(DataOutputStream &os) {
        os.writeUInt16(num_annotations);
        for (int pos = 0; pos < num_annotations; pos++) {
            annotations[pos]->dump(os);
        }
    }
};

