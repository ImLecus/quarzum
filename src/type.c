#include "quarzum.h"

type* ty_function = &(type){TY_FUNCTION, 1, 1};
type* ty_bool =     &(type){TY_BOOL, 1, 1, true};
type* ty_char =     &(type){TY_CHAR, 1, 1};

type* ty_int8 =     &(type){TY_INT, 1, 1};
type* ty_int16 =    &(type){TY_INT, 2, 2};
type* ty_int32 =    &(type){TY_INT, 4, 4};
type* ty_int64 =    &(type){TY_INT, 8, 8};

type* ty_uint8 =     &(type){TY_UINT, 1, 1, true};
type* ty_uint16 =    &(type){TY_UINT, 2, 2, true};
type* ty_uint32 =    &(type){TY_UINT, 4, 4, true};
type* ty_uint64 =    &(type){TY_UINT, 8, 8, true};

type* ty_num16 =    &(type){TY_NUM, 2, 2};
type* ty_num32 =    &(type){TY_NUM, 4, 4};
type* ty_num64 =    &(type){TY_NUM, 8, 8};

type* ty_decimal =  &(type){TY_DECIMAL, 8, 8};

type* ty_null =    &(type){TY_NULL, 1, 1, true};