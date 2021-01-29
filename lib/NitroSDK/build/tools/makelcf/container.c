/*---------------------------------------------------------------------------*
  Project:  NitroSDK - tools - makelcf
  File:     container.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: container.c,v $
  Revision 1.21  2007/07/09 12:17:54  yasu
  TARGET_NAME �̃T�|�[�g

  Revision 1.20  2007/04/12 03:29:45  yasu
  IF FIRST/LAST �� FOREACH OBJECT/LIBRARY �ł��Ή��ł���悤�ɂ���

  Revision 1.19  2007/04/10 14:13:20  yasu
  ������ SEARCH_SYMBOL �Ή�

  Revision 1.18  2006/05/10 02:06:00  yasu
  CodeWarrior 2.x �ւ̑Ή�

  Revision 1.17  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.16  2005/09/01 04:30:52  yasu
  Overlay Group �̃T�|�[�g

  Revision 1.15  2005/06/20 12:21:48  yasu
  Surffix -> Suffix �C��

  Revision 1.14  2005/02/28 05:26:03  yosizaki
  do-indent.

  Revision 1.13  2004/07/10 04:06:17  yasu
  Support command 'Library'
  Support modifier ':x' on template
  Fix up line continue '\'

  Revision 1.12  2004/07/08 02:58:53  yasu
  Support section name for multi-objects as 'Objects' parameters

  Revision 1.11  2004/07/02 07:34:53  yasu
  Support OBJECT( )

  Revision 1.10  2004/06/29 04:15:13  yasu
  Let string regions of OverlayDefs and OverlayTable expandable

  Revision 1.9  2004/06/24 07:18:54  yasu
  Support keyword "Autoload"

  Revision 1.8  2004/06/14 11:28:45  yasu
  support section filter "FOREACH.STATIC.OBJECTS=.sectionName"

  Revision 1.7  2004/03/26 05:07:11  yasu
  support variables like as -DNAME=VALUE

  Revision 1.6  2004/02/13 07:13:03  yasu
  support SDK_IRQ_STACKSIZE

  Revision 1.5  2004/02/05 07:09:03  yasu
  change SDK prefix iris -> nitro

  Revision 1.4  2004/01/15 10:51:45  yasu
  Static StackSize �̎���

  Revision 1.3  2004/01/14 12:38:08  yasu
  Change OverlayName->OverlayDefs

  Revision 1.2  2004/01/07 13:10:17  yasu
  fix all warning at compile -Wall

  Revision 1.1  2004/01/05 02:32:59  yasu
  Initial version

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include	<stdio.h>
#include	<stdlib.h>             // exit()
#include	<string.h>             // strcpy(), strcat()
#include	<malloc.h>
#include	"misc.h"
#include	"makelcf.h"
#include	"defval.h"


/*============================================================================
 *   MISC
 */
tStatic Static = { 0 };
tProperty Property = { 0 };
tForeach Foreach = { { 0 } };
tOverlayList OverlayList = { 0, 0, 0 };
tOverlayList AutoloadList = { 0, 0, 0 };

static tOverlayList *CurrentOverlay = &OverlayList;

BOOL isSame(const char *s1, const char *s2)
{
    if (s1 == s2)
    {
        return TRUE;
    }

    if (!s1 || !s2)
    {
        return FALSE;
    }

    while (*s1 == *s2)
    {
        if (!*s1)
        {
            return TRUE;
        }
        s1++;
        s2++;
    }
    return FALSE;
}


/*============================================================================
 *   OVERLAY
 */

static tOverlay *NewOverlay(tOverlayList * list)
{
    tOverlay *t = Alloc(sizeof(tOverlay));

    if (list->tail)
        list->tail->next = t;
    else
        list->head = t;
    list->tail = t;

    return t;
}

static tOverlay *GetOverlay(tOverlayList * list, const char *overlayName)
{
    tOverlay *t = list->head;

    while (t)
    {
        if (isSame(t->name, overlayName))
        {
            return t;
        }
        t = t->next;
    }
    return NULL;
}


static BOOL s_AddOverlay(tOverlayList * list, const char *overlayName)
{
    if (isSame(Static.name, overlayName) || GetOverlay(&OverlayList, overlayName)
        || GetOverlay(&AutoloadList, overlayName))
    {
        spec_yyerror("Same named MAIN/OVERLAY already existed. Aborted.");
        return FALSE;
    }
    else
    {
        tOverlay *t = NewOverlay(list);
        t->id = list->num++;
        t->name = overlayName;
        CurrentOverlay = list;
    }
    return TRUE;
}

BOOL AddOverlay(const char *overlayName)
{
    return s_AddOverlay(&OverlayList, overlayName);
}

BOOL AddAutoload(const char *overlayName)
{
    return s_AddOverlay(&AutoloadList, overlayName);
}

/*============================================================================
 *   OVERLAY - id
 */

BOOL OverlaySetId(u32 id)
{
    tOverlay *t = CurrentOverlay->tail;

    if (t->id)
    {
        spec_yyerror("OVERLAY ID already set. Ignored.");
    }
    else
    {
        t->id = id;
    }
    return TRUE;
}


/*============================================================================
 *   OVERLAY - group
 */

BOOL OverlaySetGroup(const char *group)
{
    tOverlay *t = CurrentOverlay->tail;

    if (t->group)
    {
        spec_yyerror("overlay GROUP already set. Ignored.");
    }
    else
    {
        t->group = group;
    }
    return TRUE;
}


/*============================================================================
 *   OVERLAY - address
 */

BOOL OverlaySetAddress(u32 address)
{
    tOverlay *t = CurrentOverlay->tail;

    if (t->afters.tail || t->address)
    {
        spec_yyerror("ADDRESS/AFTER already set. Ignored.");
    }
    else
    {
        t->address = address;
    }
    return TRUE;
}


/*============================================================================
 *   OVERLAY - after
 */

static tAfter *NewAfter(tAfterList * list)
{
    tAfter *t = Alloc(sizeof(tAfter));

    if (list->tail)
        list->tail->next = t;
    else
        list->head = t;
    list->tail = t;

    return t;
}


static tAfter *GetAfter(tAfterList * list, const char *name)
{
    tAfter *t = list->head;

    while (t)
    {
        if (isSame(t->name, name))
        {
            return t;
        }
        t = t->next;
    }
    return NULL;
}


BOOL AddAfter(tAfterList * list, const char *name)
{
    if (GetAfter(list, name))
    {
        spec_yyerror("Same AFTER already existed. Ignored.");
    }
    else
    {
        tAfter *t = NewAfter(list);
        t->name = name;
    }
    return TRUE;
}


BOOL OverlayAddAfter(const char *overlayName)
{
    if (CurrentOverlay->tail->address)
    {
        spec_yyerror("ADDRESS already set. Ignored.");
        return TRUE;
    }
    return AddAfter(&CurrentOverlay->tail->afters, overlayName);
}


/*============================================================================
 *   Object/Library - common
 */

tObject *NewObject(tObjectList * list)
{
    tObject *t = Alloc(sizeof(tObject));

    if (list->tail)
        list->tail->next = t;
    else
        list->head = t;
    list->tail = t;

    return t;
}


tObject *GetObject(tObjectList * list, const char *objectName, const char *sectionName)
{
    tObject *t = list->head;

    while (t)
    {
        if (isSame(t->objectName, objectName) && isSame(t->sectionName, sectionName))
        {
            return t;
        }
        t = t->next;
    }
    return NULL;
}


BOOL AddObject(tObjectList * list, const char *objectName, tObjectType objectType)
{
    tObject *t = NewObject(list);
    t->objectName = objectName;
    t->sectionName = NULL;
    t->objectType = objectType;
    return TRUE;
}


BOOL SetObjectSection(tObjectList * list, const char *sectionName)
{
    tObject *t = list->head;

    debug_printf("sectionName=[%s]\n", sectionName);

    while (t)
    {
        if (!t->sectionName)
        {
            debug_printf("list=%08x\n", list);

            if (!isNeedSection(t) && sectionName[0] == '*')
            {
                spec_yyerror("OBJECT() must be with section option like as (.text),(.data),..");
                return FALSE;
            }

            debug_printf("t->objectName=[%s]\n", t->objectName);

            if (GetObject(list, t->objectName, sectionName))
            {
                char   *s = Alloc(strlen(t->objectName) + strlen(sectionName) + 80);
                sprintf(s, "OBJECT '%s (%s)' already existed. Ignored.", t->objectName,
                        sectionName);
                spec_yyerror(s);
                Free(&s);
            }

            t->sectionName = strdup(sectionName);
        }
        t = t->next;
    }
    return TRUE;
}


/*============================================================================
 *   OVERLAY - object
 */

BOOL OverlayAddObject(const char *objectName, tObjectType objectType)
{
    return AddObject(&CurrentOverlay->tail->objects, objectName, objectType);
}


BOOL OverlaySetObjectSection(const char *sectionName)
{
    return SetObjectSection(&CurrentOverlay->tail->objects, sectionName);
}


/*============================================================================
 *   OVERLAY - library
 */

BOOL OverlayAddLibrary(const char *objectName, tObjectType objectType)
{
    return AddObject(&CurrentOverlay->tail->libraries, objectName, objectType);
}


BOOL OverlaySetLibrarySection(const char *sectionName)
{
    return SetObjectSection(&CurrentOverlay->tail->libraries, sectionName);
}


/*============================================================================
 *   OVERLAY - searchsymbol
 */

BOOL OverlayAddSearchSymbol(const char *searchName)
{
    return AddObject(&CurrentOverlay->tail->searchSymbols, searchName, OBJTYPE_NONE);
}


/*============================================================================
 *   STATIC - name
 */

BOOL StaticSetTargetName(const char *staticTargetName)
{
    tStatic *t = &Static;
    
    if (t->targetName)
    {
        return FALSE;
    }
    t->targetName = strdup(staticTargetName);
    return TRUE;
}

BOOL StaticSetName(const char *staticName)
{
    tStatic *t = &Static;
    
    if (t->name)
    {
        spec_yyerror("'Static' section already existed. Aborted.");
        return FALSE;
    }
    else if (GetOverlay(&OverlayList, staticName) || GetOverlay(&AutoloadList, staticName))
    {
        spec_yyerror("Same named 'Overlay' or 'Autoload' already existed. Aborted.");
        return FALSE;
    }
    else
    {
        t->name = staticName;
    }
    return TRUE;
}


/*============================================================================
 *   STATIC - address
 */

BOOL StaticSetAddress(u32 address)
{
    tStatic *t = &Static;

    if (t->address)
    {
        spec_yyerror("'Address' already set. Ignored.");
    }
    else
    {
        t->address = address;
    }
    return TRUE;
}


/*============================================================================
 *   STATIC - object
 */

BOOL StaticAddObject(const char *objectName, tObjectType objectType)
{
    return AddObject(&Static.objects, objectName, objectType);
}


BOOL StaticSetObjectSection(const char *sectionName)
{
    return SetObjectSection(&Static.objects, sectionName);
}


/*============================================================================
 *   STATIC - library
 */

BOOL StaticAddLibrary(const char *objectName, tObjectType objectType)
{
    return AddObject(&Static.libraries, objectName, objectType);
}


BOOL StaticSetLibrarySection(const char *sectionName)
{
    return SetObjectSection(&Static.libraries, sectionName);
}


/*============================================================================
 *   STATIC - searchsymbol
 */

BOOL StaticAddSearchSymbol(const char *searchName)
{
    return AddObject(&Static.searchSymbols, searchName, OBJTYPE_NONE);
}


/*============================================================================
 *   STATIC - address
 */

BOOL StaticSetStackSize(u32 stacksize)
{
    tStatic *t = &Static;

    if (t->stacksize)
    {
        spec_yyerror("'StackSize' already set. Ignored.");
    }
    else
    {
        t->stacksize = stacksize;
    }
    return TRUE;
}


BOOL StaticSetStackSizeIrq(u32 stacksize_irq)
{
    tStatic *t = &Static;

    if (t->stacksize_irq)
    {
        spec_yyerror("'IrqStackSize' already set. Ignored.");
    }
    else
    {
        t->stacksize_irq = stacksize_irq;
    }
    return TRUE;
}


/*============================================================================
 *   PROPERTY - OverlayTable
 *   PROPERTY - OverlayTable
 *   PROPERTY - Suffix
 */

#define	SET_PROPERTY( func, member, token )			\
    BOOL  func( const char* val )				\
    {								\
	tProperty*  t = &Property;				\
								\
	if ( t->member )					\
	{							\
            spec_yyerror( "'" #token "' already set. Ignored." );\
	}							\
	else							\
	{							\
	    t->member = val;					\
	}							\
        return TRUE;						\
    }

SET_PROPERTY(PropertySetOverlayDefs, overlaydefs, OverlayDefs)
SET_PROPERTY(PropertySetOverlayTable, overlaytable, OverlayTable)
SET_PROPERTY(PropertySetSuffix, suffix, Suffix)
/*============================================================================
 *   CHECK SPECS
 */
     static BOOL CheckAfters(const char *name, tAfterList * list)
{
    tAfter *t = list->head;

    while (t)
    {
        if (!isSame(t->name, Static.name) && !GetOverlay(&OverlayList, t->name)
            && !GetOverlay(&AutoloadList, t->name))
        {
            fprintf(stderr, "No such static/autoload/overlay %s referred in overlay %s", t->name,
                    name);
            return FALSE;
        }
        t = t->next;
    }
    return TRUE;
}


static BOOL s_CheckOverlay(tOverlayList * list)
{
    tOverlay *t = list->head;

    while (t)
    {
        if (t->address)
        {
            if (t->afters.tail)
            {
                fprintf(stderr, "Set both of 'Address' and 'After' in overlay %s", t->name);
                return FALSE;
            }
        }
        else
        {
            if (t->afters.tail)
            {
                if (!CheckAfters(t->name, &t->afters))
                {
                    return FALSE;
                }
            }
            else
            {
                fprintf(stderr, "No addressing commands 'Address'/ 'After' in overlay %s", t->name);
                return FALSE;
            }
        }
        t = t->next;
    }
    return TRUE;
}

static BOOL CheckOverlay(void)
{
    return s_CheckOverlay(&AutoloadList) && s_CheckOverlay(&OverlayList);
}

static BOOL CheckStatic(void)
{
    tStatic *t = &Static;

    if (!t->address)
    {
        fprintf(stderr, "No addressing commands 'After' in static %s", t->name);
        return FALSE;
    }

    if (!t->stacksize_irq)
    {
        t->stacksize_irq = DEFAULT_IRQSTACKSIZE;
    }

    return TRUE;
}


static BOOL CheckProperty(void)
{
    tProperty *t = &Property;
    const char *target = Static.targetName ? Static.targetName : Static.name;

    // Set default values 
    if (!t->overlaydefs)
    {
        t->overlaydefs = strdup(DEFAULT_OVERLAYDEFS);
    }
    if (t->overlaydefs[0] == '%')
    {
        char   *name = Alloc(strlen(target) + strlen(t->overlaydefs + 1) + 1);
        strcpy(name, target);
        strcat(name, t->overlaydefs + 1);
        Free(&t->overlaydefs);
        t->overlaydefs = name;
    }

    if (!t->overlaytable)
    {
        t->overlaytable = strdup(DEFAULT_OVERLAYTABLE);
    }
    if (t->overlaytable[0] == '%')
    {
        char   *name = Alloc(strlen(target) + strlen(t->overlaytable + 1) + 1);
        strcpy(name, target);
        strcat(name, t->overlaytable + 1);
        Free(&t->overlaytable);
        t->overlaytable = name;
    }

    if (!t->suffix)
    {
        t->suffix = strdup(DEFAULT_SUFFIX);
    }
    return TRUE;
}


BOOL CheckSpec(void)
{
    return (CheckStatic() && CheckOverlay() && CheckProperty());
}


/*============================================================================
 *   DUMP UTILITIES
 */

static void DumpAfters(tAfterList * list)
{
    tAfter *t = list->head;

    while (t)
    {
        printf("  After        : %s\n", t->name);
        t = t->next;
    }
}


static void DumpObjects(tObjectList * list)
{
    tObject *t = list->head;

    while (t)
    {
        printf("  Object       : %s %s\n", t->objectName, t->sectionName);
        t = t->next;
    }
}


static void DumpAutoload(void)
{
    tOverlay *t = AutoloadList.head;

    while (t)
    {
        printf("Autoload       : %s\n", t->name);
        printf("  Address      : %08lx\n", t->address);
        DumpAfters(&t->afters);
        DumpObjects(&t->objects);
        printf("\n");
        t = t->next;
    }
}


static void DumpOverlay(void)
{
    tOverlay *t = OverlayList.head;

    while (t)
    {
        printf("Overlay        : %s\n", t->name);
        printf("  Address      : %08lx\n", t->address);
        DumpAfters(&t->afters);
        DumpObjects(&t->objects);
        printf("\n");
        t = t->next;
    }
}


static void DumpStatic(void)
{
    tStatic *t = &Static;

    printf("Static         : %s\n", t->name);
    printf("  Address      : %08lx\n", t->address);
    DumpObjects(&t->objects);
    printf("  StackSize    : %08lx\n", t->stacksize);
    printf("\n");
}


static void DumpProperty(void)
{
    tProperty *t = &Property;

    printf("Property       :\n");
    printf("  OverlayDefs  : %s\n", t->overlaydefs);
    printf("  OverlayTable : %s\n", t->overlaytable);
    printf("  Suffix       : %s\n", t->suffix);
    printf("\n");
}


void DumpSpec(void)
{
    DumpStatic();
    DumpAutoload();
    DumpOverlay();
    DumpProperty();
}
