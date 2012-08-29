/*
 *      Copyright (C) 2005-2011 Team XBMC
 *      http://www.xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#include <Python.h>

#include "guilib/GUIToggleButtonControl.h"
#include "guilib/GUIFontManager.h"
#include "control.h"
#include "pyutil.h"

using namespace std;


#ifdef __cplusplus
extern "C" {
#endif

namespace PYXBMC
{
  PyObject* ControlToggle_New(
                              PyTypeObject *type,
                              PyObject *args,
                              PyObject *kwds )
  {
    static const char *keywords[] = {
      "x", "y", "width", "height", "label",
      "focusTexture", "noFocusTexture",
      "textOffsetX", "textOffsetY", "alignment",
      "font", "textColor", "disabledColor", "angle", "shadowColor", "focusedColor", NULL };
    ControlToggle *self;
    char* cFont = NULL;
    char* cTextureFocus = NULL;
    char* cTextureNoFocus = NULL;
    char* cTextColor = NULL;
    char* cDisabledColor = NULL;
    char* cShadowColor = NULL;
    char* cFocusedColor = NULL;
    
    PyObject* pObjectText;
    
    self = (ControlToggle*)type->tp_alloc(type, 0);
    if (!self) return NULL;
    new(&self->bIsSelected) bool();
    
    if (!PyArg_ParseTupleAndKeywords(
                                     args,
                                     kwds,
                                     (char*)"llllO|sslllssslss",
                                     (char**)keywords,
                                     &self->dwPosX,
                                     &self->dwPosY,
                                     &self->dwWidth,
                                     &self->dwHeight,
                                     &pObjectText,
                                     &cTextureFocus,
                                     &cTextureNoFocus,
                                     &cFont,
                                     &cTextColor,
                                     &cDisabledColor,
                                     &cShadowColor,
                                     &cFocusedColor))
    {
      Py_DECREF( self );
      return NULL;
    }
    
    
    cTextureFocus :
    PyXBMCGetDefaultImage((char*)"button", (char*)"texturefocus", (char*)"button-focus.png");
    cTextureNoFocus :
    PyXBMCGetDefaultImage((char*)"button", (char*)"texturenofocus", (char*)"button-nofocus.jpg");

    return (PyObject*)self;
  }
  
  void ControlToggle_Dealloc(ControlToggle* self)
  {
    self->ob_type->tp_free((PyObject*)self);
  }
  
  
  // isSelected() Method
  PyDoc_STRVAR(isSelected__doc__,
    "isSelected() -- Returns the toggle buttons's selected status.\n"
    "\n"
    "example:\n"
    "  - is = self.togglebutton.isSelected()\n");

  PyObject* ControlToggle_IsSelected(ControlToggle *self, PyObject *args)
  {
    bool isSelected = false;

    PyXBMCGUILock();
    if (self->pGUIControl)
      isSelected = ((CGUIToggleButtonControl*)self->pGUIControl)->IsSelected();
    PyXBMCGUIUnlock();

    return Py_BuildValue((char*)"b", isSelected);
  }


  PyMethodDef ControlToggle_methods[] = {
    {(char*)"isSelected", (PyCFunction)ControlToggle_IsSelected, METH_VARARGS, isSelected__doc__},
    {NULL, NULL, 0, NULL}
  };

  // ControlRadioButton class
  PyDoc_STRVAR(ControlToggle__doc__,
    "ControlRadioButton class.\n"
    "\n"
    "ControlRadioButton(x, y, width, height, label[, focusTexture, noFocusTexture, textOffsetX, textOffsetY,\n"
    "              alignment, font, textColor, disabledColor, angle, shadowColor, focusedColor,\n"
    "              radioFocusTexture, noRadioFocusTexture])\n"
    "\n"
    "x                   : integer - x coordinate of control.\n"
    "y                   : integer - y coordinate of control.\n"
    "width               : integer - width of control.\n"
    "height              : integer - height of control.\n"
    "label               : string or unicode - text string.\n"
    "focusTexture        : [opt] string - filename for focus texture.\n"
    "noFocusTexture      : [opt] string - filename for no focus texture.\n"
    "textOffsetX         : [opt] integer - x offset of label.\n"
    "textOffsetY         : [opt] integer - y offset of label.\n"
    "alignment           : [opt] integer - alignment of label - *Note, see xbfont.h\n"
    "font                : [opt] string - font used for label text. (e.g. 'font13')\n"
    "textColor           : [opt] hexstring - color of enabled radio button's label. (e.g. '0xFFFFFFFF')\n"
    "disabledColor       : [opt] hexstring - color of disabled radio button's label. (e.g. '0xFFFF3300')\n"
    "angle               : [opt] integer - angle of control. (+ rotates CCW, - rotates CW)\n"
    "shadowColor         : [opt] hexstring - color of radio button's label's shadow. (e.g. '0xFF000000')\n"
    "focusedColor        : [opt] hexstring - color of focused radio button's label. (e.g. '0xFF00FFFF')\n"
    "radioFocusTexture   : [opt] string - filename for radio focus texture.\n"
    "noRadioFocusTexture : [opt] string - filename for radio no focus texture.\n"
    "\n"
    "*Note, You can use the above as keywords for arguments and skip certain optional arguments.\n"
    "       Once you use a keyword, all following arguments require the keyword.\n"
    "       After you create the control, you need to add it to the window with addControl().\n"
    "\n"
    "example:\n"
    "  - self.radiobutton = xbmcgui.ControlToggleButton(100, 250, 200, 50, 'Status', font='font14')\n");

  // Restore code and data sections to normal.

  PyTypeObject ControlToggle_Type;

  void initControlToggle_Type()
  {
    PyXBMCInitializeTypeObject(&ControlToggle_Type);

    ControlToggle_Type.tp_name = (char*)"xbmcgui.ControlToggleButton";
    ControlToggle_Type.tp_basicsize = sizeof(ControlToggle);
    ControlToggle_Type.tp_dealloc = (destructor)ControlToggle_Dealloc;
    ControlToggle_Type.tp_compare = 0;
    ControlToggle_Type.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE;
    ControlToggle_Type.tp_doc = ControlToggle__doc__;
    ControlToggle_Type.tp_methods = ControlToggle_methods;
    ControlToggle_Type.tp_base = &Control_Type;
    ControlToggle_Type.tp_new = ControlToggle_New;
  }
}

#ifdef __cplusplus
}
#endif
