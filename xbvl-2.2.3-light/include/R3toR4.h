/* %I%, %R%, %B%, %D%,  %T%,  %M% */

/*
 *          Pour garder la compativilite du code avec R3, R4, R5 de X11.
 *
 * Cet include redefinit plusieurs constantes symboliques et noms des fonctions
 * qui ont changees avec les diferentes release de X11.
 */

#ifndef _R3toR4_h_
#define _R3toR4_h_


/* AsciiSink.h */


#define XtAsciiSinkCreate          XawAsciiSinkCreate
#define XtAsciiSinkDestroy         XawAsciiSinkDestroy

#ifndef XawTextSink
#define XawTextSink    Widget
#endif
#ifndef XtTextSink
#define XtTextSink     XawTextSink
#endif

extern XawTextSink XawAsciiSinkCreate(); /* parent, args, num_args */
/* Widget parent;		*/
/* ArgList args;		*/
/* Cardinal num_args;	*/

#define XawAsciiSinkDestroy XtDestroyWidget

/* AsciiSrc.h */

#define ASCII_STRING		/* Turn on R3 AsciiDisk and AsciiString */
#define ASCII_DISK		/* Emulation modes. */

#ifdef ASCII_STRING
#define XawStringSourceDestroy XtDestroyWidget
#endif

#ifdef ASCII_DISK
#define XawDiskSourceDestroy XtDestroyWidget
#endif

#ifdef ASCII_STRING

/*	Function Name: AsciiStringSourceCreate
 *	Description: Creates a string source.
 *	Arguments: parent - the widget that will own this source.
 *                 args, num_args - the argument list.
 *	Returns: a pointer to the new text source.
 */

Widget XawStringSourceCreate(/* parent, args, num_args */);
/*
Widget parent;
ArgList args;
Cardinal num_args;
*/
#endif /* ASCII_STRING */

#ifdef ASCII_DISK
/*	Function Name: AsciiDiskSourceCreate
 *	Description: Creates a disk source.
 *	Arguments: parent - the widget that will own this source.
 *                 args, num_args - the argument list.
 *	Returns: a pointer to the new text source.
 */

Widget XawDiskSourceCreate(/* parent, args, num_args */);
/*
Widget parent;
ArgList args;
Cardinal num_args;
*/
#endif
/*************************************************************
 * For Compatibility only. Dialog.h                                  */

#define XtDialogGetValueString           XawDialogGetValueString
#define XtDialogAddButton                XawDialogAddButton

/*************************************************************/
/*************************************************************
 * For Compatibility only. Form.h                                  */

#define XtFormDoLayout                XawFormDoLayout

/*************************************************************/
/* List.h */

#define XtListReturnStruct   XawListReturnStruct

#define XT_LIST_NONE         XAW_LIST_NONE 

#define XtListChange         XawListChange
#define XtListUnhighlight    XawListUnhighlight
#define XtListHighlight      XawListHighlight
#define XtListShowCurrent    XawListShowCurrent

/* Paned.h */
extern WidgetClass vPanedWidgetClass;
/*************************************************************
 * For Compatibility only.                                   */

#define XtPanedSetMinMax        XawPanedSetMinMax
#define XtPanedGetMinMax        XawPanedGetMinMax
#define XtPanedGetNumSub        XawPanedGetNumSub
#define XtPanedAllowResize      XawPanedAllowResize
#define XtPanedSetRefigureMode  XawPanedSetRefigureMode
/* Scrollbar.h */
/*************************************************************
 * For Compatibility only.                                   */

#define XtScrollBarSetThumb      XawScrollbarSetThumb

/*************************************************************/
/* Text.h */
/************************************************************
 *
 * This Stuff is only for compatibility, and will go away in 
 * future releases.                                         */

/* preserved for Back Compatability only. */

#define XawTextSource Widget
#define XtTextSource  Widget

#define wordBreak		0x01
#define scrollVertical		0x02
#define scrollHorizontal	0x04
#define scrollOnOverflow	0x08
#define resizeWidth		0x10
#define resizeHeight		0x20
#define editable		0x40

#if 0
typedef long XtTextPosition;
#endif

#define XtTextPosition             XawTextPosition

#define XtTextBlock                XawTextBlock
#define XtTextBlockPtr             XawTextBlockPtr

#define EditDone	           XawEditDone
#define EditError	           XawEditError
#define PositionError	           XawPositionError

#define XtEditDone	           XawEditDone
#define XtEditError	           XawEditError
#define XtPositionError	           XawPositionError

#define XttextRead                 XawtextRead
#define XttextAppend               XawtextAppend
#define XttextEdit                 XawtextEdit
#define XtTextEditType             XawTextEditType
#define XtselectNull               XawselectNull

#define XtselectPosition           XawselectPosition
#define XtselectChar               XawselectChar
#define XtselectWord               XawselectWord
#define XtselectLine               XawselectLine
#define XtselectParagraph          XawselectParagraph
#define XtselectAll                XawselectAll
#define XtTextSelectType           XawTextSelectType

#define XtTextDisableRedisplay     XawTextDisableRedisplay
#define XtTextEnableRedisplay      XawTextEnableRedisplay
#define XtTextGetSource            XawTextGetSource

#define XtTextDisplay              XawTextDisplay
#define XtTextDisplayCaret         XawTextDisplayCaret
#define XtTextSetSelectionArray    XawTextSetSelectionArray
#define XtTextSetLastPos           XawTextSetLastPos
#define XtTextGetSelectionPos      XawTextGetSelectionPos
#define XtTextSetSource            XawTextSetSource
#define XtTextReplace              XawTextReplace
#define XtTextTopPosition          XawTextTopPosition
#define XtTextSetInsertionPoint    XawTextSetInsertionPoint
#define XtTextGetInsertionPoint    XawTextGetInsertionPoint
#define XtTextUnsetSelection       XawTextUnsetSelection
#define XtTextChangeOptions        XawTextChangeOptions
#define XtTextGetOptions           XawTextGetOptions
#define XtTextSetSelection         XawTextSetSelection
#define XtTextInvalidate           XawTextInvalidate

#define XtDiskSourceCreate         XawDiskSourceCreate
#define XtDiskSourceDestroy        XawDiskSourceDestroy
#define XtStringSourceCreate       XawStringSourceCreate
#define XtStringSourceDestroy      XawStringSourceDestroy

extern void XawTextChangeOptions(); /* w, options */
    /* Widget        w;		*/
    /* Int    options; */

extern Int XawTextGetOptions(); /* w */
    /* Widget        w;		*/

extern void XawTextSetLastPos(); /* w, lastPos */
    /* Widget        w;		*/
    /* XawTextPosition lastPos;  */

/* TextP.h */
/*************************************************************
 * For Compatibility only.                                   */

#define XtTextLineTable                 XawTextLineTable
#define XtTextLineTablePtr              XawTextLineTablePtr
#define XtTextLineTableEntry            XawTextLineTableEntry
#define XtTextLineTableEntryPtr         XawTextLineTableEntryPtr

/*************************************************************/
/* TextSrc.h */
/*************************************************************
 * For Compatibility only.                                   */

#define _XtTextSink        _XawTextSink
#define _XtTextSource      _XawTextSource

#define XtisOn             XawisOn
#define XtisOff            XawisOff

#define XtsmTextSelect     XawsmTextSelect
#define XtsmTextExtend     XawsmTextExtend

#define XtactionStart      XawactionStart
#define XtactionAdjust     XawactionAdjust
#define XtactionEnd        XawactionEnd

#define XtsdLeft           XawsdLeft
#define XtsdRight          XawsdRight

#define XtstPositions      XawstPositions
#define XtstWhiteSpace     XawstWhiteSpace
#define XtstEOL            XawstEOL
#define XtstParagraph      XawstParagraph
#define XtstAll            XawstAll

#define XtTextSelectionAction XawTextSelectionAction
#define XtTextSelection       XawTextSelection
#define XtTextScanDirection   XawTextScanDirection
#define XtTextScanType        XawTextScanType


/*************************************************************/
/* Toggle.h */
/*************************************************************
 * For Compatibility only.                                   */

#define XtToggleChangeRadioGroup       XawToggleChangeRadioGroup
#define XtToggleSetCurrent             XawToggleSetCurrent
#define XtToggleUnsetCurrent           XawToggleUnsetCurrent
#define XtToggleGetCurrent             XawToggleGetCurrent

#endif
