#ifndef UTGB_VARIABLES_H
#define UTGB_VARIABLES_H


//Get value of global Int16 variable
#define VAR_VAL(ID) (*(INT16 *)VM_REF_TO_PTR(ID))

//Get pointer to global variable (int8/16)
#define VAR_PTR_8(ID) ((INT8 *)VM_REF_TO_PTR(ID))
#define VAR_PTR_16(ID) ((INT16 *)VM_REF_TO_PTR(ID))



#endif