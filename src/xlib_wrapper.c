#include <X11/Xlib.h>
#include <X11/keysym.h>
#include "mruby.h"

extern Display* extract_x_display(mrb_state *mrb, mrb_value display_obj);

mrb_value
mrb_xw_keysym_to_keycode(mrb_state *mrb, mrb_value self)
{
  mrb_value display_obj;
  mrb_int keysym;
  mrb_get_args(mrb, "oi", &display_obj, &keysym);

  Display *display = extract_x_display(mrb, display_obj);
  return mrb_fixnum_value(XKeysymToKeycode(display, keysym));
}

mrb_value
mrb_xw_fetch_active_window(mrb_state *mrb, mrb_value self)
{
  mrb_value display_obj;
  mrb_int keycode, state;
  mrb_get_args(mrb, "o", &display_obj, &keycode, &state);

  Window window;
  int focus_state;
  Display *display = extract_x_display(mrb, display_obj);
  XGetInputFocus(display, &window, &focus_state);

  return mrb_fixnum_value(window);
}

mrb_value
mrb_xw_grab_key(mrb_state *mrb, mrb_value self)
{
  mrb_value display_obj;
  mrb_int keycode, state;
  mrb_get_args(mrb, "oii", &display_obj, &keycode, &state);

  Display *display = extract_x_display(mrb, display_obj);
  XGrabKey(display, XKeysymToKeycode(display, keycode), state, XDefaultRootWindow(display), True, GrabModeAsync, GrabModeAsync);

  return mrb_nil_value();
}

mrb_value
mrb_xw_ungrab_keys(mrb_state *mrb, mrb_value self)
{
  mrb_value display_obj;
  mrb_get_args(mrb, "o", &display_obj);

  Display *display = extract_x_display(mrb, display_obj);
  XUngrabKey(display, AnyKey, AnyModifier, XDefaultRootWindow(display));

  return mrb_nil_value();
}

void
mrb_xkremap_xlib_wrapper_init(mrb_state *mrb, struct RClass *mXkremap)
{
  struct RClass *cXlibWrapper = mrb_define_class_under(mrb, mXkremap, "XlibWrapper", mrb->object_class);
  mrb_define_class_method(mrb, cXlibWrapper, "keysym_to_keycode", mrb_xw_keysym_to_keycode, MRB_ARGS_REQ(2));
  mrb_define_class_method(mrb, cXlibWrapper, "fetch_active_window", mrb_xw_fetch_active_window, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, cXlibWrapper, "grab_key", mrb_xw_grab_key, MRB_ARGS_REQ(3));
  mrb_define_class_method(mrb, cXlibWrapper, "ungrab_keys", mrb_xw_ungrab_keys, MRB_ARGS_REQ(1));
}