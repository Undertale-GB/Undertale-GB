const id = "UTGB_EVENT_BATTLE_TEXT";
const groups = ["Undertale GB", "EVENT_GROUP_DIALOGUE"];
const name = "Display Battle Dialouge";

const wrap8Bit = (val) => (256 + (val % 256)) % 256;

const decOct = (dec) => wrap8Bit(dec).toString(8).padStart(3, "0");

const fields = [
  {
    key: "text",
    type: "text",
    placeholder: "",
    multiple: false,
    defaultValue: "",
    flexBasis: "100%",
  },
];

const compile = (input, helpers) => {
  const {
    appendRaw,
    _addComment,
    _loadStructuredText,
    _overlayClear,
    _displayText,
    _overlayWait,
    _addNL,
  } = helpers;


  _addComment("Display Dialouge");

  _overlayClear(3, 1, 14, 4, ".UI_COLOR_BLACK", false);

  _loadStructuredText(`\\003\\04\\02${input.text}`);

  _displayText();

  _overlayWait(false, [".UI_WAIT_TEXT"]);

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