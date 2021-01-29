/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS - demos - cplusplus-1
  File:     main.cpp

  Copyright 2003,2004 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.cpp,v $
  Revision 1.6  2004/04/06 11:35:38  yada
  fix header comment

  Revision 1.5  2004/03/08 08:55:47  yada
  fix top comment

  Revision 1.4  2004/03/08 08:31:58  yada
  fix top comment

  Revision 1.3  2004/02/20 03:34:01  yasu
  replace OS_Panic -> OS_Halt

  Revision 1.2  2004/02/20 00:04:05  yasu
  add comments

  Revision 1.1  2004/02/18 04:19:30  yada
  C++デモ

  Revision 1.2  2004/02/12 08:53:38  yada
  コンストラクタ内newをコメント。Arenaの初期化がなされていないから。

  Revision 1.1  2004/02/12 08:51:47  yada
  c++サンプル

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include "new.h"

//================================================================
// Foo
//
class Foo
{
private:
    u32  a;
    u32  b;
    int* array;

public:
    Foo();
    ~Foo();

    void printA(void)
    {
        OS_Printf("a = %d\n", a);
    }
    void printB(void);

    void printArray(void)
    {
        for( int n=0; n<16; n++ ){ OS_Printf("%d ", array[n]); }
        OS_Printf("\n");
    }
};

/*---------------------------------------------------------------------------*
  Name:         Foo::Foo

  Description:  Constructor for class Foo.
                Simply initialize private members.

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
Foo::Foo()
{
    a = 1234;
    b = 5678;

    array = new int[16];
    for( int n=0; n<16; n++ ){ array[n] = n; }
}

/*---------------------------------------------------------------------------*
  Name:         Foo::~Foo

  Description:  Destructor for class Foo.
                Performs an OSReport so we know it got called properly.

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
Foo::~Foo()
{
    OS_Printf("Foo destructor\n");
}

/*---------------------------------------------------------------------------*
  Name:         Foo::printB

  Description:  function to print out b member

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void Foo::printB()
{
    OS_Printf("b = %d\n", b);
}


//================================================================================
//---- static class instanse
static Foo  staticFoo;

//--------------------------------------------------------------------------------
// NitroMain
//
void NitroMain()
{
    // no need to call OS_Init() because it called in NitroStartUp()
    //OS_Init();

    Foo* foo;
    foo = new Foo;

    foo->printA();
    foo->printB();
    foo->printArray();

    staticFoo.printA();
    staticFoo.printB();
    staticFoo.printArray();

    OS_Printf( "==== Finish sample programm ====\n" );
    OS_Halt();
}
