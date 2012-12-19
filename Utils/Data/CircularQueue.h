#ifndef CIRCULAR_QUEUE_H_
#define CIRCULAR_QUEUE_H_


#define DECLARE_CIR_QUEUE(struct_name, element_type, max) \
    typedef struct { \
        int limit; \
        int front; \
        int rear; \
        int count; \
        element_type records[(max)]; \
    } struct_name


#endif // CIRCULAR_QUEUE_H_
