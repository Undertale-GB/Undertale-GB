const id = "DEAB22_RANDOM_EVENT";
const groups = ["Deab22's Plugins", "EVENT_GROUP_CONTROL_FLOW"];
const name = "Pick Random Event";

const MAX_OPTIONS = 8;

const wrap8Bit = (val) => (256 + (val % 256)) % 256;

const decOct = (dec) => wrap8Bit(dec).toString(8).padStart(3, "0");

const fields = [].concat(
  [
    {
      key: "choices",
      label: "FIELD_NUMBER_OF_OPTIONS",
      description: "FIELD_NUMBER_OF_OPTIONS_DESC",
      type: "number",
      min: 1,
      max: MAX_OPTIONS,
      defaultValue: 2,
    },
  ],
  Array(MAX_OPTIONS)
    .fill()
    .reduce((arr, _, i) => {
      arr.push({
        key: `__collapseCase${i}`,
        label: `value${i}`,
        conditions: [
          {
            key: "choices",
            gt: i,
          },
        ],
        type: "collapsable",
        defaultValue: false,
      });
      arr.push({
        key: `true${i}`,
        conditions: [
          {
            key: `__collapseCase${i}`,
            ne: true,
          },
          {
            key: "choices",
            gt: i,
          },
        ],
        type: "events",
      });
      return arr;
    }, []),
  [
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
    caseVariableValue,
    _menuItem,
    _displayText,
    getVariableAlias,
    getNextLabel,
    _stackPushConst,
    _stackPop,
    seedRng,
    _rand,
    _switch,
    _jump,
    _label,
    _compilePath,
    _addNL,
  } = helpers;
 

  _stackPushConst(0);//debug
  seedRng();
  _rand(".ARG0", 0, input.choices);
  
  const switchLabels = Array(input.choices)
    .fill()
    .map((_, i) => getNextLabel());

  const endLabel = getNextLabel();

  const switchCases = Array(input.choices)
    .fill()
    .map((_, i) => [i, `${switchLabels[i]}$`]);

  _switch(".ARG0", switchCases, 1);
  _jump(endLabel);
  _addNL();

  for (let i = 0; i < input.choices; i++) {
    _addComment(`case ${i}`);
    _label(switchLabels[i]);
    _compilePath(input[`true${i}`]);
    _jump(endLabel);
  }
  _label(endLabel);

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
