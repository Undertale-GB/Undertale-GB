const id = "UTGB_EVENT_NON_FREEZING_TEXT";
const groups = ["Undertale GB", "EVENT_GROUP_DIALOGUE"];
const name = "Non Freezing Text";

const wrap8Bit = (val) => (256 + (val % 256)) % 256;
const decOct = (dec) => wrap8Bit(dec).toString(8).padStart(3, "0");
const autoLabel = (fetchArg) => {
  return l10n("EVENT_IF_VALUE_COMPARE_LABEL", {
    variable: fetchArg("variable"),
  });
};

const fields = [
  {
    key: "variable",
    label: "Reading state",
    type: "variable",
  },
  {
    key: "event",
    type: "events",
  },
];

const compile = (input, helpers) => {
  const {_ifVariableConst, _compilePath, _jump, variableSetToValue, getNextLabel, _label, inputScriptSet, inputScriptRemove} = helpers;

  const trueLabel = getNextLabel();
  const endLabel = getNextLabel();

  _ifVariableConst(".EQ", input.variable, 0, trueLabel, 0);
   // execute the events of the false path
  _jump(endLabel); // jump to the end label (after the true path)
    
  _label(trueLabel); 
  variableSetToValue(input.variable, 1); // set input variable to 1
  inputScriptSet(["up", "down", "left", "right"], true); // set all input to 0
  _compilePath(input.event); // execute the events of the true path
  inputScriptRemove(["up", "down", "left", "right"]); // remove all input
  variableSetToValue(input.variable, 0); // set input variable to 0
    
  _label(endLabel);

};

module.exports = {
  id,
  name,
  autoLabel,
  groups,
  fields,
  compile,
  waitUntilAfterInitFade: true,
};