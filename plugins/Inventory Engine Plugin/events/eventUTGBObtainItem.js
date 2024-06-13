const id = "UTGB_OBTAIN_ITEM";
const groups = ["Undertale GB"];
const name = "Inventory: Obtain Item";

const MAX_OPTIONS = 8;

const wrap8Bit = (val) => (256 + (val % 256)) % 256;

const decOct = (dec) => wrap8Bit(dec).toString(8).padStart(3, "0");

const fields = [
  {
    key: "itemID",
    label: "Item:",
    description: "Select the item to be collected",
    type: "select",
    options: [
      [1, "Bandage"],
      [2, "Monster Candy"],
      [3, "Spider Donut"],
      [4, "Spider Cider"],
      [5, "Butterscotch Pie"],
      [6, "Stick"],
      [7, "Toy Knife"],
      [8, "Faded Ribbon"],
    ],
    defaultValue: 1,
    width: "50%",
  },
  {
    key: "varCollected",
    label: "Variable to store \"collected\" state:",
    type: "variable",
    width: "50%",
    defaultValue: "LAST_VARIABLE",
  },
];

const compile = (input, helpers) => {
  const {
    appendRaw,
    _addComment,
    //_overlayMoveTo,
    //_loadStructuredText,
    //_overlayClear,
    //_overlayWait,
    //_choice,
    //caseVariableValue,
    //_menuItem,
    //_displayText,
    //getVariableAlias,
    getNextLabel,
    _stackPushConst,
    _stackPop,
    _setVariable,
    //seedRng,
    //_rand,
    //_switch,
    //_jump,
    _label,
    //_compilePath,
    _ifVariableConst,
    _addNL,
  } = helpers;
 

  _addComment("Collect item w. ID " + input.itemID);
  
  const skipLabel = getNextLabel();
  _ifVariableConst(".GT", input.varCollected, 0, skipLabel, 0);

    _stackPushConst(input.itemID);
    appendRaw("VM_CALL_NATIVE b_inv_obtain_item, _inv_obtain_item");
    _setVariable(input.varCollected, ".ARG0");
    _stackPop(1);

  _label(skipLabel);
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
