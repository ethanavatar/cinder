enum IOResult {
    IO_OK,
    IO_ERROR
};

#define IOResult(T, E) struct {\
    enum IOResult type;\
    T ok_value;\
    E err_value;}

#define IOResultOk(T, V) (IOResult(T, void)) { .type = IO_OK, .ok_value = V }
#define IOResultErr(T, V) (IOResult(T, void)) { .type = IO_ERROR, .err_value = V }

#define IOResultIsOk(T, V) (V.type == IO_OK)
#define IOResultIsErr(T, V) (V.type == IO_ERROR)