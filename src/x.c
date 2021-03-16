
typedef struct list {
    float *valPtr;
    struct list *nextPtr;
} list;



float value = 0.5;
list *p = New list;

float value = 0.5;
p->valPtr = &value;
