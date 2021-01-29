/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - g2d
  File:     g2d_TextCanvas.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.8 $
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include <string.h>

#undef NNS_G2D_UNICODE
#include <nnsys/g2d/g2d_TextCanvas.h>






//****************************************************************************
// global functions
//****************************************************************************

//----------------------------------------------------------------------------
// �`��
//----------------------------------------------------------------------------

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dTextCanvasDrawString

  Description:  �ŏ��̉��s�����������͏I�[�����܂ł�`�悵�܂��B

  Arguments:    pTxn:   �`��Ώۂ� TextCanvas �ւ̃|�C���^�B
                x:      �`��J�n�ʒu�� x ���W�B
                y:      �`��J�n�ʒu�� y ���W�B
                cl:     �����F�̐F�ԍ��B
                str:    �`�悷�镶����B
                pPos:   ���s�����܂ŕ`�悵���ꍇ�͉��s�����̎��̕����ւ�
                        �|�C���^���A�I�[�����܂ŕ`�悵���ꍇ��NULL���i�[����
                        �o�b�t�@�ւ̃|�C���^�B
                        �s�v�ȏꍇ��NULL���w�肷�邱�Ƃ��ł��܂��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void NNSi_G2dTextCanvasDrawString(
    const NNSG2dTextCanvas* pTxn,
    int x,
    int y,
    int cl,
    const void* str,
    const void** pPos
#ifdef NNS_G2D_FONT_ENABLE_DIRECTION_SUPPORT
    , NNSiG2dTextDirection d
#endif
)
{
    const void* pos;        // �`��Ώۂ̕����ւ̃|�C���^
    int charSpace;          // �����Ԋu (1�����ڂ̕����̉E�[��2�����ڂ̕����̍��[�̊Ԋu)
    const NNSG2dFont* pFont;
    u16 c;
    NNSiG2dSplitCharCallback getNextChar;

    NNS_G2D_TEXTCANVAS_ASSERT( pTxn );
    NNS_G2D_POINTER_ASSERT( str );
    NNS_G2D_ASSERT( NNS_G2dFontGetBpp(NNS_G2dTextCanvasGetFont(pTxn))
                    <= NNS_G2dTextCanvasGetCharCanvas(pTxn)->dstBpp );
    NNS_G2D_MINMAX_ASSERT( cl, 0,
        (1 << NNS_G2dTextCanvasGetCharCanvas(pTxn)->dstBpp)
        - (1 << NNS_G2dFontGetBpp(NNS_G2dTextCanvasGetFont(pTxn))) + 1 );

    charSpace   = pTxn->hSpace;
    pFont       = pTxn->pFont;
    pos         = str;
    getNextChar = NNSi_G2dFontGetSpliter(pFont);


    while( (c = getNextChar((const void**)&pos)) != 0 )
    {
        if( c == '\n' )
        {
            break;
        }

        // 1�����`��
        {
            const int w = NNS_G2dCharCanvasDrawChar(pTxn->pCanvas, pFont, x, y, cl, c) + charSpace;
#ifdef NNS_G2D_FONT_ENABLE_DIRECTION_SUPPORT
            x += w * d.x;
            y += w * d.y;
#else
            x += w;
#endif
        }
    }

    if( pPos != NULL )
    {
        *pPos = (c == '\n') ? pos: NULL;
    }
}



/*---------------------------------------------------------------------------*
  Name:         NNSi_G2dTextCanvasDrawTextAlign

  Description:  areaWidth �̕����ō��E���񂹂����ĕ`�悵�܂��B

  Arguments:    pTxn:       �`��Ώۂ� TextCanvas �ւ̃|�C���^�B
                x:          �`��J�n�ʒu�� x ���W�B
                y:          �`��J�n�ʒu�� y ���W�B
                cl:         �����F�̐F�ԍ��B
                flags:      �`��ʒu���w�肷��t���O�B
                txt:        �`�悷�镶����B
                areaWidth:  �E�񂹁A���񂹂̍ۂɎQ�Ƃ����`��̈敝�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void NNSi_G2dTextCanvasDrawTextAlign(
    const NNSG2dTextCanvas* pTxn,
    int x,
    int y,
    int areaWidth,
    int cl,
    u32 flags,
    const void* txt
#ifdef NNS_G2D_FONT_ENABLE_DIRECTION_SUPPORT
    , NNSiG2dTextDirection d
#endif
)
{
    const void* str;    // �`��Ώۂ̕�����ւ̃|�C���^
    int linefeed;       // �s����i1�s�ڂ̍s�̏�[��2�s�ڂ̍s�̏�[�̊Ԋu�j
#ifdef NNS_G2D_FONT_ENABLE_DIRECTION_SUPPORT
    int linefeedx;
    int linefeedy;
    int line;
#endif
    int charSpace;      // �����Ԋu (1�����ڂ̕����̉E�[��2�����ڂ̕����̍��[�̊Ԋu)
    const NNSG2dFont* pFont;
    int px, py;

    NNS_G2D_TEXTCANVAS_ASSERT( pTxn );
    NNS_G2D_POINTER_ASSERT( txt );
    NNS_G2D_ASSERT( NNS_G2dFontGetBpp(NNS_G2dTextCanvasGetFont(pTxn))
                    <= NNS_G2dTextCanvasGetCharCanvas(pTxn)->dstBpp );
    NNS_G2D_MINMAX_ASSERT( cl, 0,
        (1 << NNS_G2dTextCanvasGetCharCanvas(pTxn)->dstBpp)
        - (1 << NNS_G2dFontGetBpp(NNS_G2dTextCanvasGetFont(pTxn))) + 1 );

    charSpace = pTxn->hSpace;
    linefeed = NNS_G2dFontGetLineFeed(pTxn->pFont) + pTxn->vSpace;
    pFont = pTxn->pFont;
    str = txt;
#ifdef NNS_G2D_FONT_ENABLE_DIRECTION_SUPPORT
    linefeedx = linefeed * -d.y;
    linefeedy = linefeed * d.x;
    line = 0;
#else
    py = y;
#endif


    while( str != NULL )
    {
#ifdef NNS_G2D_FONT_ENABLE_DIRECTION_SUPPORT
        px = x + line * linefeedx;
        py = y + line * linefeedy;
#else
        px = x;
#endif

        if( flags & NNS_G2D_HORIZONTALALIGN_RIGHT )
        {
            const int width = NNS_G2dTextCanvasGetStringWidth(pTxn, str, NULL);
            const int offset = areaWidth - width;
#ifdef NNS_G2D_FONT_ENABLE_DIRECTION_SUPPORT
            px += offset * d.x;
            py += offset * d.y;
#else
            py += offset;
#endif
        }
        else if( flags & NNS_G2D_HORIZONTALALIGN_CENTER )
        {
            const int width = NNS_G2dTextCanvasGetStringWidth(pTxn, str, NULL);
            const int offset = (areaWidth + 1) / 2 - (width + 1) / 2;
#ifdef NNS_G2D_FONT_ENABLE_DIRECTION_SUPPORT
            px += offset * d.x;
            py += offset * d.y;
#else
            py += offset;
#endif
        }

#ifdef NNS_G2D_FONT_ENABLE_DIRECTION_SUPPORT
        NNSi_G2dTextCanvasDrawString(pTxn, px, py, cl, str, &str, d);
        line++;
#else
        NNSi_G2dTextCanvasDrawString(pTxn, px, py, cl, str, &str);
        py += linefeed;
#endif
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dTextCanvasDrawText

  Description:  (x,y) �ɑ΂��� flags �Ŏw�肳���z�u�ŕ������`�悵�܂��B

  Arguments:    pTxn:   TextCanvas�ւ̃|�C���^�B
                x:      �`�����W�B
                y:      �`�����W�B
                cl:     �����̕`��F�ԍ��B
                flags:  �z�u�w��t���O�B
                txt:    �`�悷�镶����B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void NNSi_G2dTextCanvasDrawText(
    const NNSG2dTextCanvas* pTxn,
    int x,
    int y,
    int cl,
    u32 flags,
    const void* txt
#ifdef NNS_G2D_FONT_ENABLE_DIRECTION_SUPPORT
    , NNSiG2dTextDirection d
#endif
)
{
    NNSG2dTextRect area;

    NNS_G2D_TEXTCANVAS_ASSERT( pTxn );
    NNS_G2D_POINTER_ASSERT( txt );
    NNS_G2D_ASSERT( NNS_G2dFontGetBpp(NNS_G2dTextCanvasGetFont(pTxn))
                    <= NNS_G2dTextCanvasGetCharCanvas(pTxn)->dstBpp );
    NNS_G2D_MINMAX_ASSERT( cl, 0,
        (1 << NNS_G2dTextCanvasGetCharCanvas(pTxn)->dstBpp)
        - (1 << NNS_G2dFontGetBpp(NNS_G2dTextCanvasGetFont(pTxn))) + 1 );

    // ������W�����߂�
    {
        area = NNS_G2dTextCanvasGetTextRect(pTxn, txt);

        if( flags & NNS_G2D_HORIZONTALORIGIN_CENTER )
        {
            const int offset = - (area.width + 1) / 2;
#ifdef NNS_G2D_FONT_ENABLE_DIRECTION_SUPPORT
            x += offset * d.x;
            y += offset * d.y;
#else
            x += offset;
#endif
        }
        else if( flags & NNS_G2D_HORIZONTALORIGIN_RIGHT )
        {
            const int offset = - area.width;
#ifdef NNS_G2D_FONT_ENABLE_DIRECTION_SUPPORT
            x += offset * d.x;
            y += offset * d.y;
#else
            x += offset;
#endif
        }

        if( flags & NNS_G2D_VERTICALORIGIN_MIDDLE )
        {
            const int offset = - (area.height + 1) / 2;
#ifdef NNS_G2D_FONT_ENABLE_DIRECTION_SUPPORT
            x += offset * -d.y;
            y += offset * d.x;
#else
            y += offset;
#endif
        }
        else if( flags & NNS_G2D_VERTICALORIGIN_BOTTOM )
        {
            const int offset = - area.height;
#ifdef NNS_G2D_FONT_ENABLE_DIRECTION_SUPPORT
            x += offset * -d.y;
            y += offset * d.x;
#else
            y += offset;
#endif
        }
    }

#ifdef NNS_G2D_FONT_ENABLE_DIRECTION_SUPPORT
    NNSi_G2dTextCanvasDrawTextAlign(pTxn, x, y, area.width, cl, flags, txt, d);
#else
    NNSi_G2dTextCanvasDrawTextAlign(pTxn, x, y, area.width, cl, flags, txt);
#endif
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dTextCanvasDrawTextRect

  Description:  ��`����ɕ������z�u���ĕ`�悵�܂��B
                ��`���Ɏ��܂�悤�ɕ`�悷��킯�ł͂���܂���B

  Arguments:    pTxn:   TextCanvas�ւ̃|�C���^�B
                x:      ��`������W�B
                y:      ��`������W�B
                cl:     �����̕`��F�ԍ��B
                flags:  �z�u�w��t���O�B
                txt:    �`�悷�镶����B
                w:      ��`���B
                h:      ��`�����B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void NNSi_G2dTextCanvasDrawTextRect(
    const NNSG2dTextCanvas* pTxn,
    int x,
    int y,
    int w,
    int h,
    int cl,
    u32 flags,
    const void* txt
#ifdef NNS_G2D_FONT_ENABLE_DIRECTION_SUPPORT
    , NNSiG2dTextDirection d
#endif
)
{
    NNS_G2D_TEXTCANVAS_ASSERT( pTxn );
    NNS_G2D_POINTER_ASSERT( txt );
    NNS_G2D_ASSERT( NNS_G2dFontGetBpp(NNS_G2dTextCanvasGetFont(pTxn))
                    <= NNS_G2dTextCanvasGetCharCanvas(pTxn)->dstBpp );
    NNS_G2D_MINMAX_ASSERT( cl, 0,
        (1 << NNS_G2dTextCanvasGetCharCanvas(pTxn)->dstBpp)
        - (1 << NNS_G2dFontGetBpp(NNS_G2dTextCanvasGetFont(pTxn))) + 1 );

    // y ���W���t���O�ɉ����ĕ␳
    {
        if( flags & NNS_G2D_VERTICALALIGN_BOTTOM )
        {
            const int height = NNS_G2dTextCanvasGetTextHeight(pTxn, txt);
            const int offset = h - height;
#ifdef NNS_G2D_FONT_ENABLE_DIRECTION_SUPPORT
            x += offset * -d.y;
            y += offset * d.x;
#else
            y += offset;
#endif
        }
        else if( flags & NNS_G2D_VERTICALALIGN_MIDDLE )
        {
            const int height = NNS_G2dTextCanvasGetTextHeight(pTxn, txt);
            const int offset = (h + 1)/2 - (height + 1) / 2;
#ifdef NNS_G2D_FONT_ENABLE_DIRECTION_SUPPORT
            x += offset * -d.y;
            y += offset * d.x;
#else
            y += offset;
#endif
        }
    }

#ifdef NNS_G2D_FONT_ENABLE_DIRECTION_SUPPORT
    NNSi_G2dTextCanvasDrawTextAlign(pTxn, x, y, w, cl, flags, txt, d);
#else
    NNSi_G2dTextCanvasDrawTextAlign(pTxn, x, y, w, cl, flags, txt);
#endif
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dTextCanvasDrawTaggedText

  Description:  �^�O�����ߍ��܂ꂽ�����s�̕������`�悵�܂��B
                ���䕶��(0x00-0x09,0x0B-0x1F) �𕶎��񒆂�
                ��������ƃR�[���o�b�N���Ăяo���܂��B
                �^�O�̏����͑S�ăR�[���o�b�N�ɔC����Ă��܂��B

  Arguments:    pTxn:       �`��Ώۂ� TextCanvas �ւ̃|�C���^�B
                x:          �`���ʒu�� x ���W�B
                y:          �`���ʒu�� y ���W�B
                cl:         �����F�̐F�ԍ��B
                str:        �`�悷�镶����B
                cbFunc:     �^�O�ɂ���ČĂ΂��R�[���o�b�N�֐��B
                cbParam:    �R�[���o�b�N�֐��ɓn����郆�[�U�f�[�^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void NNSi_G2dTextCanvasDrawTaggedText(
    const NNSG2dTextCanvas* pTxn,
    int x,
    int y,
    int cl,
    const void* txt,
    NNSG2dTagCallback cbFunc,
    void* cbParam
#ifdef NNS_G2D_FONT_ENABLE_DIRECTION_SUPPORT
    , NNSiG2dTextDirection d
#endif
)
{
    const void* pos;    // �`��Ώۂ̕����ւ̃|�C���^
    int linefeed;       // �s����i1�s�ڂ̍s�̏�[��2�s�ڂ̍s�̏�[�̊Ԋu�j
    int charSpace;      // �����Ԋu (1�����ڂ̕����̉E�[��2�����ڂ̕����̍��[�̊Ԋu)
    const NNSG2dFont* pFont;
    NNSG2dTagCallbackInfo cbInfo;
    u16 c;
    NNSiG2dSplitCharCallback getNextChar;

    int px = x;
    int py = y;

    NNS_G2D_TEXTCANVAS_ASSERT( pTxn );
    NNS_G2D_FONT_ASSERT( pTxn->pFont );
    NNS_G2D_POINTER_ASSERT( txt );
    NNS_G2D_ASSERT( NNS_G2dFontGetBpp(NNS_G2dTextCanvasGetFont(pTxn))
                    <= NNS_G2dTextCanvasGetCharCanvas(pTxn)->dstBpp );
    NNS_G2D_MINMAX_ASSERT( cl, 0,
        (1 << NNS_G2dTextCanvasGetCharCanvas(pTxn)->dstBpp)
        - (1 << NNS_G2dFontGetBpp(NNS_G2dTextCanvasGetFont(pTxn))) + 1 );
    NNS_G2D_POINTER_ASSERT( cbFunc );

    cbInfo.txn      = *pTxn;
    cbInfo.cbParam  = cbParam;

    charSpace   = cbInfo.txn.hSpace;
    pFont       = cbInfo.txn.pFont;
    linefeed    = NNS_G2dFontGetLineFeed(pFont) + cbInfo.txn.vSpace;
    pos         = txt;
    getNextChar = NNSi_G2dFontGetSpliter(pFont);

#ifdef NNS_G2D_FONT_ENABLE_DIRECTION_SUPPORT
    linefeed   *= (d.x != 0) ? d.x: -d.y;
#endif

    while( (c = getNextChar((const void**)&pos)) != 0 )
    {
        if( c < ' ' )
        {
            if( c == '\n' )
                // ���s�̏���
            {
#ifdef NNS_G2D_FONT_ENABLE_DIRECTION_SUPPORT
                if( d.x == 0 )
                {
                    px += linefeed;
                    py = y;
                }
                else
#endif
                {
                    px = x;
                    py += linefeed;
                }
            }
            else
                // �R�[���o�b�N�Ăяo��
            {
                cbInfo.str = (const NNSG2dChar*)pos;
                cbInfo.x = px;
                cbInfo.y = py;
                cbInfo.clr = cl;

                cbFunc(c, &cbInfo);
                NNS_G2D_TEXTCANVAS_ASSERT( &(cbInfo.txn) );
                NNS_G2D_FONT_ASSERT( cbInfo.txn.pFont );
                NNS_G2D_POINTER_ASSERT( cbInfo.str );
                NNS_G2D_MINMAX_ASSERT( cl, 0, 255 );

                pos = (const void*)cbInfo.str;
                px = cbInfo.x;
                py = cbInfo.y;
                cl = cbInfo.clr;

                pFont       = cbInfo.txn.pFont;
                charSpace   = cbInfo.txn.hSpace;
                linefeed    = NNS_G2dFontGetLineFeed(pFont) + cbInfo.txn.vSpace;
#ifdef NNS_G2D_FONT_ENABLE_DIRECTION_SUPPORT
                linefeed   *= (d.x != 0) ? d.x: -d.y;
#endif
            }

            continue;
        }
        else
            // 1�����`��
        {
            const int w = NNS_G2dCharCanvasDrawChar(cbInfo.txn.pCanvas, cbInfo.txn.pFont, px, py, cl, c) + charSpace;
#ifdef NNS_G2D_FONT_ENABLE_DIRECTION_SUPPORT
            px += w * d.x;
            py += w * d.y;
#else
            px += w;
#endif
        }
    }
}

//--------------------------------------------------------------------
