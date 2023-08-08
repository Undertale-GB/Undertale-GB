const id = "UTGB_EVENT_BUTTON_FREEZE_WRAPPER";
const groups = ["Undertale GB", "EVENT_GROUP_MISC"];
const name = "Button Freeze Wrapper";

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
    label: "Freeze state",
    type: "variable",
  },
  {
    key: "vmunlock",
    label: "VM Unlock ?",
    type: "checkbox",
    defaultValue: "true",
    width: "50%",
  },
  {
    key: "event",
    type: "events",
  },
];

const compile = (input, helpers) => {
  const {_ifVariableConst, _compilePath, _jump, variableSetToValue, getNextLabel, _label, inputScriptSet, inputScriptRemove, appendRaw} = helpers;

  const trueLabel = getNextLabel();
  const endLabel = getNextLabel();

  _ifVariableConst(".EQ", input.variable, 0, trueLabel, 0);
   // execute the events of the false path
  _jump(endLabel); // jump to the end label (after the true path)
    
  _label(trueLabel);
  if(input.vmunlock) {
    appendRaw(`VM_UNLOCK`)
  }
  variableSetToValue(input.variable, 1); // set input variable to 1
  inputScriptSet(["up", "down", "left", "right"], true); // set all input to 0
  _compilePath(input.event); // execute the events of the true path
  inputScriptRemove(["up", "down", "left", "right"]); // remove all input
  if(input.vmunlock) {
    appendRaw(`VM_LOCK`)
  }
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