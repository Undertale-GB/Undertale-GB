const id = "UTGB_EVENT_BATTLE_ENEMY_SELECT";
const groups = ["Undertale GB", "EVENT_GROUP_DIALOGUE"];
const name = "Display Battle Menu(Enemy Select)";

const MAX_OPTIONS = 4;

const wrap8Bit = (val) => (256 + (val % 256)) % 256;

const decOct = (dec) => wrap8Bit(dec).toString(8).padStart(3, "0");

const fields = [].concat(
  [
    {
      key: "variable",
      label: "Set Variable",
      type: "variable",
      width: "50%",
      defaultValue: "LAST_VARIABLE",
    },
    {
      key: "items",
      label: "Number of options",
      type: "number",
      min: 1,
      max: MAX_OPTIONS,
      width: "50%",
      defaultValue: 1,
    },
    {
      type: "break",
    },
    ...Array(MAX_OPTIONS)
      .fill()
      .reduce((arr, _, i) => {
        const idx = i + 1;
        arr.push(
          {
            type: "break",
            conditions: [
              {
                key: "items",
                gte: idx,
              },
              
            ],
          },
          {
            key: `option_${idx}_text`,
            type: "text",
            label: `Set to '${idx}' if`,
            placeholder: `Item ${idx}`,
            defaultValue: "",
            flexBasis: "100%",
            conditions: [
              {
                key: "items",
                gte: idx,
              },
            ],
          },
          {
            key: `option_${idx}_math`,
            type: "matharea",
            label: `Draw in yellow if:`,
            placeholder: `e.g. $ActState == 1...`,
            defaultValue: "",
            flexBasis: "100%",
            conditions: [
              {
                key: "items",
                gte: idx,
              },
            ],
          },
        );
        return arr;
      }, []),
  ]
);

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
    _displayText,
    getVariableAlias,
    getNextLabel,
    _expressionToHumanReadable,
    _stackPushEvaluatedExpression,
    _ifConst,
    _jump,
    _label,
    _addNL,
  } = helpers;

  const variableAlias = getVariableAlias(input.variable);

  const instantTextSpeedCode = `\\001\\1`;

  const yellowFontCode = `\\002\\2`//change to Font index in Main

  _addComment("Advanced Menu");

  //_loadStructuredText(`${instantTextSpeedCode}${str}`);

  _overlayClear(3, 1, 14, 4, ".UI_COLOR_BLACK", false);

  Array(input.items)
    .fill()
    .map((_, i) => {
      const idx = i + 1;
      const itemText = input[`option_${idx}_text`];

      fieldX = 3;
      fieldY = i + 1;
      


      const x = decOct(2 + fieldX);
      const y = decOct(1 + fieldY);

      if (itemText) {

        const expression = input[`option_${idx}_math`];

        const trueLabel = getNextLabel();
        const endLabel = getNextLabel();
        _addComment(`If ${_expressionToHumanReadable(expression)}`);
        _stackPushEvaluatedExpression(expression);
        _ifConst(".GT", ".ARG0", 0, trueLabel, 1);
        //If not yellow
        _loadStructuredText(`${instantTextSpeedCode}\\003\\${x}\\${y}${itemText}`);

        _jump(endLabel);
        _label(trueLabel);
        //If Yellow
        _loadStructuredText(`${instantTextSpeedCode}${yellowFontCode}\\003\\${x}\\${y}${itemText}`);

        _label(endLabel);
        _addNL();

        if(i == 0){
          appendRaw("VM_DISPLAY_TEXT_EX .DISPLAY_DEFAULT, 5");
        }else{
          appendRaw("VM_DISPLAY_TEXT_EX .DISPLAY_DEFAULT, .TEXT_TILE_CONTINUE");
        }
        
        _overlayWait(true, [".UI_WAIT_WINDOW", ".UI_WAIT_TEXT"]);
      }
    });

  //appendRaw("VM_DISPLAY_TEXT_EX .DISPLAY_DEFAULT, 5");

  _overlayWait(true, [".UI_WAIT_WINDOW", ".UI_WAIT_TEXT"]);

  _choice(variableAlias, [".UI_MENU_CANCEL_B"], input.items);

  Array(input.items)
    .fill()
    .map((_, i) => {
      const clampItem = (i) => Math.min(i || 0, input.items);

      const idx = i + 1;
      

      fieldX = 3;
      fieldY = i + 1;
      
      
      left = 0;
      right = 0;
      up = 0;
      down = 0;

      up = idx - 1;
      if (idx != input.items) down = idx + 1;
      


      _menuItem(fieldX, fieldY, left, right, up, down);
    });

  _overlayWait(true, [".UI_WAIT_WINDOW", ".UI_WAIT_TEXT"]);

  _overlayClear(3, 1, 14, 4, ".UI_COLOR_BLACK", false);

  _addNL();
};

module.exports = {
  id,
  name,
  groups,
  fields,
  compile,
  waitUntilAfterInitFade: true,
};
