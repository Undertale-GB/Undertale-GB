const id = "EVENT_SAVE_POINT_DIALOGUE";
const groups = ["Undertale GB"];
const name = "Save Point Dialogue";

const autoLabel = (fetchArg, args) => {
  if (([].concat(args.text) || []).join()) {
    return `Show Save Dialogue "${fetchArg("text")}"`;
  } else {
    return "Show Save Dialogue";
  }
};

const fields = [
  {
    label: "Area Name",
    key: "text",
    type: "text",
    placeholder: "Text...",
    defaultValue: "",
    flexBasis: "100%",
  },
];

const compile = (input, helpers) => {
  
  const {
    appendRaw,
    _addComment,
    _overlayMoveTo,
    _loadStructuredText,
    _overlayClear,
    _overlayWait,
    _choice,
    _menuItem,
    _stackPushConst,
    getNextLabel,
    _label,
    _displayText,
    getVariableAlias,
    _addNL,
    _ifConst,
  } = helpers;

  appendRaw(`
.SUBMAP_TEMPVAR = -6
VM_RESERVE 6
VM_SET_CONST    .SUBMAP_TEMPVAR,       0
VM_SET_CONST ^/(.SUBMAP_TEMPVAR + 1)/, 0
VM_SET_CONST ^/(.SUBMAP_TEMPVAR + 2)/, 20
VM_SET_CONST ^/(.SUBMAP_TEMPVAR + 3)/, 18

VM_GET_INT16 VAR_TEXT_X _scroll_x
VM_GET_INT16 VAR_TEXT_Y _scroll_y

VM_RPN
  .R_REF VAR_TEXT_X
  .R_INT16 8
  .R_OPERATOR .DIV
  .R_STOP
VM_SET ^/(.SUBMAP_TEMPVAR + 4 - 1)/, .ARG0
VM_POP 1

VM_RPN
  .R_REF VAR_TEXT_Y
  .R_INT16 8
  .R_OPERATOR .DIV
  .R_STOP
VM_SET ^/(.SUBMAP_TEMPVAR + 5 - 1)/, .ARG0
VM_POP 1

VM_OVERLAY_SET_SUBMAP_EX .SUBMAP_TEMPVAR
VM_RESERVE -6

VM_OVERLAY_CLEAR 2, 6, 16, 6, .UI_COLOR_BLACK, .UI_DRAW_FRAME

VM_LOAD_TEXT 6
.dw VAR_NAME1, VAR_NAME2, VAR_NAME3, VAR_NAME4, VAR_NAME5, VAR_NAME6
.asciz \"\\001\\001\\003\\004\\010%c%c%c%c%c%c\\n` + input.text + `\\003\\005\\013Save\\003\\014\\013Return\"
VM_DISPLAY_TEXT_EX .DISPLAY_DEFAULT, 0
VM_OVERLAY_WAIT .UI_MODAL, .UI_WAIT_TEXT


;VM_SET_CONST_UINT8 _show_actors_on_overlay, 1
VM_OVERLAY_SETPOS 0, 0`);

/*
VM_CHOICE               VAR_GLOBAL_TEMP_VAR, .UI_MENU_CANCEL_B, 2
.MENUITEM           3, 10, 0, 2, 0, 0
.MENUITEM           10, 10, 1, 0, 0, 0
*/
  const skipLabel = getNextLabel();
  _stackPushConst(0);
  _choice(".ARG0", [".UI_MENU_CANCEL_B"], 2);
  _menuItem(3,  10, 0, 2, 0, 0);
  _menuItem(10, 10, 1, 0, 0, 0);
  //_stackPop(1);
  _ifConst(".NE", ".ARG0", 1, skipLabel, 1);

  
  _stackPushConst(0);
  appendRaw(`
; Save Data to Slot 0
VM_RAISE                EXCEPTION_SAVE, 1
    .SAVE_SLOT 0
VM_POLL_LOADED          .ARG0
`);
  _ifConst(".EQ", ".ARG0", 1, skipLabel, 1);

  
  appendRaw(`
VM_OVERLAY_CLEAR 2, 6, 16, 6, .UI_COLOR_BLACK, .UI_DRAW_FRAME

VM_LOAD_TEXT 6
.dw VAR_NAME1, VAR_NAME2, VAR_NAME3, VAR_NAME4, VAR_NAME5, VAR_NAME6
.asciz "\\001\\1\\002\\003\\003\\04\\10%c%c%c%c%c%c\\n` + input.text + `\\003\\04\\13Game Saved"
VM_DISPLAY_TEXT_EX .DISPLAY_DEFAULT, 0
VM_OVERLAY_WAIT .UI_MODAL, ^/(.UI_WAIT_BTN_A | .UI_WAIT_TEXT)/`);


  _label(skipLabel);

  _overlayMoveTo(0, 18, ".OVERLAY_SPEED_INSTANT");

};

//const MAX_DIALOGUE_LINES = 5;

module.exports = {
  id,
  name,
  description: "Show the Undertale save menu",
  autoLabel,
  groups,
  fields,
  compile,
  waitUntilAfterInitFade: true,
};