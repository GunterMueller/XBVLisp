/* $Header: TemplateP.h,v 1.3 89/08/25 21:40:25 michael Exp $ Sony Corporation */
#include <X11/copyright.h>

/* $XConsortium: TemplateP.h,v 1.3 89/03/30 16:05:59 jim Exp $ */
/* Copyright	Massachusetts Institute of Technology	1987, 1988 */

#ifndef _TemplateP_h
#define _TemplateP_h

#include "Template.h"
/* include superclass private header file */
#include <X11/CoreP.h>

/* define unique representation types not found in <X11/StringDefs.h> */

#define XtRTemplateResource "TemplateResource"

typedef struct {
    Int empty;
} TemplateClassPart;

typedef struct _TemplateClassRec {
    CoreClassPart	core_class;
    TemplateClassPart	template_class;
} TemplateClassRec;

extern TemplateClassRec templateClassRec;

typedef struct {
    /* resources */
    char* resource;
    /* private state */
} TemplatePart;

typedef struct _TemplateRec {
    CorePart		core;
    TemplatePart	template;
} TemplateRec;

#endif /* _TemplateP_h */
