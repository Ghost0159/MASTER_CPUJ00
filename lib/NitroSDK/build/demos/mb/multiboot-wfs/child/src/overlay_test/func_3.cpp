/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - demos - multiboot-wfs
  File:     func_3.cpp

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: func_3.cpp,v $
  Revision 1.1  2005/06/23 09:05:39  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include	<nitro.h>

#include	"func.h"


/******************************************************************************/
/* local implement */

namespace
{

/* �R���X�g���N�^ / �f�X�g���N�^�̃e�X�g�p�N���X */
    class   Foo
    {
      private:
        const int line_;
      public:
                Foo(int line):line_(line)
        {
            OS_TPrintf("func_3_cpp::Foo::constructor called. (object is at func_3_cpp:%d)\n",
                       line_);
        }
               ~Foo()
        {
            OS_TPrintf("func_3_cpp::Foo::destructor called. (object is at func_3_cpp:%d)\n", line_);
        }
    };

}                                      // unnamed namespace


/******************************************************************************/
/* variable */

static int i = 0;

/*
 * FS_StartOverlay() �̃^�C�~���O�ō\�z����܂�.
 * FS_EndOverlay() �̃^�C�~���O�ŉ�̂���܂�.
 */
static Foo foo1(__LINE__);


/******************************************************************************/
/* function */

#if	defined(__cplusplus)
extern  "C"
{
#endif


    void    func_3(char *dst)
    {
        /*
         * ����Ăяo�����̃^�C�~���O�ō\�z����܂�.
         * �\�z����Ă����, FS_EndOverlay() �̃^�C�~���O�ŉ�̂���܂�.
         */
        static Foo foo2(__LINE__);

              ++i;
                (void)OS_SPrintf(dst, "func_3. called %d time%s.\n", i, (i == 1) ? "" : "s", &i);
    }


#if	defined(__cplusplus)
}                                      /* extern "C" */
#endif


/******************************************************************************/
