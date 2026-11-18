.module utgb_battle_gbvm

.include "vm.i"
.include "macro.i"
        
.area _CODE_255

___bank_utgb_battle_run_menu = 255
.globl ___bank_utgb_battle_run_menu

;.globl ___bank_script_engine_init, _script_engine_init

_utgb_battle_run_menu::
        ;VM_LOCK
        ;VM_CALL_FAR             ___bank_script_engine_init, _script_engine_init
        ;VM_MUSIC_STOP
        ;VM_FADE_OUT             .UI_MODAL
        ;VM_RAISE                EXCEPTION_RESET, 0
        VM_CALL_NATIVE b_utgb_battle_run_menu_native, _utgb_battle_run_menu_native
        VM_STOP