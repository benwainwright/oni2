/*
 * Component_Output.re
 *
 * A simple component for showing terminal-style output
 */

open Oni_Core;
open Oni_Core.Utility;

[@deriving show]
type msg = 
| VimList(Component_VimList.msg);

[@deriving show]
type model = {
  vimList: Component_VimList.model(string),
};

let initial = {
  vimList: Component_VimList.create(~rowHeight=20)
};

let set = (contents, model) => {
  let (_isMultiple, lines) = contents
  |> StringEx.splitLines;

  let vimList' = Component_VimList.set(
    ~searchText=str => str,
    lines,
    model.vimList
  );

  prerr_endline ("LINE COUNT: " ++ string_of_int(Array.length(lines)));

  { vimList: vimList' };
};

type outmsg =
  | Nothing;

let update = (msg, model) => switch (msg) {
| VimList(listMsg) => 
  let (vimList', _outmsg) = Component_VimList.update(listMsg, model.vimList);
  ({ vimList: vimList' }, Nothing)
}

let keyPress = (key, model) => {
  {
    vimList: model.vimList |> Component_VimList.keyPress(key)
  }
};

module Contributions = {
  let commands = [];
  let keybindings = [];
};

module View = {
  open Revery.UI;

  let make =
      (
        ~isActive,
        ~editorFont: Service_Font.font,
        ~uiFont: UiFont.t,
        ~theme: ColorTheme.Colors.t,
        ~model,
        ~dispatch,
        (),
      ) => {
    let bg = Feature_Theme.Colors.Terminal.background.from(theme);
    let fg = Feature_Theme.Colors.Terminal.foreground.from(theme);
    <View style=Style.[backgroundColor(bg), flexGrow(1)]>
      <Component_VimList.View
        isActive
        font=uiFont
        focusedIndex=None
        theme
        model={model.vimList}
        dispatch={msg => dispatch(VimList(msg))}
        render={
          (
          ~availableWidth as _,
          ~index as _,
          ~hovered as _,
          ~selected as _,
          text
        ) => {
          <Text
            fontFamily={editorFont.fontFamily}
            fontSize={editorFont.fontSize}
            text
            style=Style.[color(fg)]
          />
        }}
        />
    </View>;
  };
};

module Sub = {
  let sub = _model => Isolinear.Sub.none;
};
