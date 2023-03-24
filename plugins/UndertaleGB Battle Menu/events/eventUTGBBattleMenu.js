const id = "UTGB_EVENT_BATTLE_MENU";
const groups = ["Undertale GB", "EVENT_GROUP_DIALOGUE"];
const name = "Display Battle Menu";

const MAX_OPTIONS = 8;

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
      defaultValue: 2,
    },
    {
      key: "twoRows",
      label: "Split Menu into 2 Rows?",
      type: "checkbox",
      width: "100%",
      defaultValue: true,
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
        );
        return arr;
      }, []),
  ]
);

const compile = (input, helpers) => {
  const {
    _addComment,
    _overlayMoveTo,
    _loadStructuredText,
    _overlayClear,
    _overlayWait,
    _choice,
    _menuItem,
    _displayText,
    getVariableAlias,
    _addNL,
  } = helpers;

  let str = "";
  Array(input.items)
    .fill()
    .map((_, i) => {
      const idx = i + 1;
      const itemText = input[`option_${idx}_text`];

      fieldX = 0;
      fieldY = 0;

      if (input.twoRows) {

        fieldX = ((i % 2) * 7) + 3;
        fieldY = (i >> 1) + 1;
      } else {

        fieldX = 3;
        fieldY = i + 1;
      }


      const x = decOct(2 + fieldX);
      const y = decOct(1 + fieldY);

      if (itemText) {
        str += `\\003\\${x}\\${y}${itemText}`;
      }
    });

  const variableAlias = getVariableAlias(input.variable);

  const instantTextSpeedCode = `\\001\\1`;

  _addComment("Advanced Menu");

  _loadStructuredText(`${instantTextSpeedCode}${str}`);

  _overlayClear(3, 1, 14, 4, ".UI_COLOR_BLACK", false);

  _displayText();

  _overlayWait(true, [".UI_WAIT_WINDOW", ".UI_WAIT_TEXT"]);

  _choice(variableAlias, [".UI_MENU_CANCEL_B"], input.items);

  Array(input.items)
    .fill()
    .map((_, i) => {
      const clampItem = (i) => Math.min(i || 0, input.items);

      const idx = i + 1;
      
      fieldX = 0;
      fieldY = 0;

      if (input.twoRows) {

        fieldX = ((i % 2) * 7) + 3;
        fieldY = (i >> 1) + 1;
      } else {

        fieldX = 3;
        fieldY = i + 1;
      }
      
      left = 0;
      right = 0;
      up = 0;
      down = 0;

      if (input.twoRows) {

        if (i % 2 == 0) {
          if (idx < input.items) right = idx + 1;
          if (idx == input.items && idx != 1) right = idx - 1;
        }

        if (i % 2 && idx - 1 >= 0) {
          left = idx - 1;
        }
      
        if (i - 2 >= 0) {
          up = idx - 2;
        }
      
        if (i + 2 <= input.items - 1) {
          down = idx + 2;
        }
      } else {

        up = idx - 1;

        if (idx != input.items) down = idx + 1;
      }


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
