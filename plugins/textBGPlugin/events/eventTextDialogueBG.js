const id = "EVENT_TEXT";
const groups = ["EVENT_GROUP_DIALOGUE"];

const autoLabel = (fetchArg, args) => {
  if (([].concat(args.text) || []).join()) {
    return `Display po "${fetchArg("text")}"`;
  } else {
    return "Display Dialogue";
  }
};

const fields = [
  {
    key: "text",
    type: "textarea",
    placeholder: "Text...",
    multiple: true,
    defaultValue: "",
    flexBasis: "100%",
  },
  {
    key: "avatarId",
    type: "avatar",
    toggleLabel: "Add Avatar",
    label: "Avatar",
    description: "The avatar image to optionally display on the left hand side of the dialogue box.",
    defaultValue: "",
    optional: true,
  },
];

const compile = (input, helpers) => {
  // copied from ScriptBuilder.ts but with .UI_COLOR_BLACK
  ((inputText = " ", avatarId) => {
    const input = Array.isArray(inputText) ? inputText : [inputText];
  
    const initialNumLines = input.map(
      (textBlock) => textBlock.split("\n").length
    );
  
    const maxNumLines = Math.max(2, Math.max.apply(null, initialNumLines));
    const textBoxHeight = Math.min(maxNumLines, MAX_DIALOGUE_LINES) + 2;
    const textBoxY = 18 - textBoxHeight;
  
    helpers._addComment("Text Dialogue");
    input.forEach((text, textIndex) => {
      let avatarIndex = undefined;
      if (avatarId) {
        const { avatars } = helpers.options;
        avatarIndex = avatars.findIndex((a) => a.id === avatarId);
        if (avatarIndex < 0) {
          avatarIndex = undefined;
        }
      }
  
      helpers._loadStructuredText(text, avatarIndex, MAX_DIALOGUE_LINES);
      helpers._overlayClear(0, 0, 20, textBoxHeight, ".UI_COLOR_BLACK", true);
      if (textIndex === 0) {
        helpers._overlayMoveTo(0, textBoxY, ".OVERLAY_IN_SPEED");
      }
      helpers._displayText();
      helpers._overlayWait(true, [
        ".UI_WAIT_WINDOW",
        ".UI_WAIT_TEXT",
        ".UI_WAIT_BTN_A",
      ]);
      if (textIndex === input.length - 1) {
        helpers._overlayMoveTo(0, 18, ".OVERLAY_OUT_SPEED");
        helpers._overlayWait(true, [".UI_WAIT_WINDOW", ".UI_WAIT_TEXT"]);
      }
    });
    helpers._addNL();
  })(input.text || " ", input.avatarId);
};

const MAX_DIALOGUE_LINES = 5;

module.exports = {
  id,
  description: "Show a dialogue box at the bottom of the game screen. When text is shown the dialogue box will slide up from the bottom of the screen and will slide down after it has been shown.",
  autoLabel,
  groups,
  fields,
  compile,
  waitUntilAfterInitFade: true,
};