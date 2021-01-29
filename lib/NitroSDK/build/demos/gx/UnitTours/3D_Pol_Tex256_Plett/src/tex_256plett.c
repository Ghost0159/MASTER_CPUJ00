/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/3D_Pol_Tex256_Plett
  File:     tex_256plett.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: tex_256plett.c,v $
  Revision 1.10  2006/01/18 02:12:29  kitase_hirotake
  do-indent

  Revision 1.9  2005/02/28 05:26:15  yosizaki
  do-indent.

  Revision 1.8  2004/04/07 01:23:08  yada
  fix header comment

  Revision 1.7  2004/04/06 12:48:07  yada
  fix header comment

  Revision 1.6  2004/02/05 07:09:02  yasu
  change SDK prefix iris -> nitro

  Revision 1.5  2004/01/07 09:38:05  nishida_kenji
  revises comments

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include "tex_256plett.h"

const u16 tex_256plett64x64[] = {
    0x0303, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303,
    0x0303, 0x0303,
    0x0303, 0x0303, 0x0303, 0x0303, 0x0403, 0x0404, 0x0605, 0x0606, 0x0606, 0x0807, 0x0808, 0x0808,
    0x0808, 0x0908,
    0x0a09, 0x070a, 0x0b07, 0x050c,
    0x0303, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303,
    0x0303, 0x0e03,
    0x0303, 0x0303, 0x0303, 0x0303, 0x0403, 0x0404, 0x060f, 0x0606, 0x0606, 0x0a07, 0x0808, 0x0808,
    0x1008, 0x1010,
    0x0a09, 0x070a, 0x0b07, 0x050c,
    0x0301, 0x0d01, 0x1211, 0x0313, 0x1401, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303, 0x0303,
    0x0e0e, 0x1515,
    0x0e15, 0x0e0e, 0x0303, 0x0303, 0x0e03, 0x0404, 0x060f, 0x0606, 0x0606, 0x0706, 0x0809, 0x0808,
    0x1008, 0x1010,
    0x070a, 0x0b07, 0x0c0c, 0x0505,
    0x0d01, 0x1611, 0x1717, 0x1418, 0x190d, 0x0d0d, 0x1a0d, 0x1313, 0x1313, 0x1313, 0x030d, 0x150e,
    0x1515, 0x1c1b,
    0x1d1b, 0x151e, 0x0e0e, 0x1f0e, 0x2120, 0x2221, 0x0c20, 0x0606, 0x0606, 0x0706, 0x080a, 0x0808,
    0x1010, 0x0707,
    0x0c0c, 0x0c0c, 0x0c0c, 0x0505,
    0x1813, 0x1616, 0x1716, 0x2317, 0x1801, 0x1616, 0x1716, 0x1717, 0x1717, 0x1617, 0x0e1a, 0x1e15,
    0x1d1d, 0x2524,
    0x2426, 0x1c24, 0x0e1d, 0x1f0e, 0x2827, 0x2929, 0x2729, 0x0f2a, 0x0c2b, 0x2c06, 0x082d, 0x0a08,
    0x0607, 0x0c0c,
    0x0c0c, 0x0c0c, 0x0c0c, 0x0505,
    0x1616, 0x1616, 0x1616, 0x1217, 0x110d, 0x1616, 0x1717, 0x1616, 0x1616, 0x1217, 0x1e2e, 0x2f1d,
    0x2f2f, 0x2630,
    0x2626, 0x2626, 0x1d24, 0x0e0e, 0x2720, 0x2928, 0x2929, 0x2929, 0x2121, 0x0f20, 0x3107, 0x062d,
    0x0c0c, 0x0c0c,
    0x0c0c, 0x0c0c, 0x0c0c, 0x0505,
    0x1616, 0x1616, 0x1616, 0x1716, 0x1311, 0x1616, 0x1717, 0x1616, 0x1716, 0x1917, 0x301a, 0x2f30,
    0x2f2f, 0x1c1d,
    0x3232, 0x2626, 0x2426, 0x0e1d, 0x201f, 0x2927, 0x2021, 0x1e1e, 0x0e15, 0x220e, 0x2b1f, 0x060c,
    0x0606, 0x0c0c,
    0x0c0c, 0x0c0c, 0x0c0c, 0x3305,
    0x1616, 0x1616, 0x1616, 0x1616, 0x1112, 0x1716, 0x1717, 0x1717, 0x1617, 0x1917, 0x3413, 0x2f2f,
    0x1d2f, 0x2f1d,
    0x3224, 0x3232, 0x3232, 0x151c, 0x201f, 0x3520, 0x1d15, 0x1c1b, 0x1b1b, 0x1e2f, 0x220e, 0x0f1f,
    0x060c, 0x0c06,
    0x0c0c, 0x0c0c, 0x0f0c, 0x041f,

    0x1616, 0x1616, 0x1616, 0x1616, 0x1616, 0x1616, 0x1717, 0x1717, 0x1617, 0x1917, 0x3413, 0x1d1d,
    0x1d1d, 0x1e1e,
    0x241b, 0x2424, 0x3624, 0x3830, 0x3838, 0x3838, 0x3702, 0x2f37, 0x1c2f, 0x2f1c, 0x151d, 0x220e,
    0x0f20, 0x060c,
    0x0c06, 0x330c, 0x041f, 0x0404,
    0x1616, 0x1616, 0x1616, 0x1616, 0x1716, 0x1216, 0x1717, 0x1717, 0x1617, 0x2316, 0x3013, 0x1e1d,
    0x1e1d, 0x1e1e,
    0x1b1e, 0x3624, 0x3814, 0x3838, 0x3838, 0x3838, 0x0303, 0x0303, 0x3702, 0x2f30, 0x1d2f, 0x0e15,
    0x2922, 0x0f27,
    0x3305, 0x041f, 0x0404, 0x0e0e,
    0x1616, 0x1616, 0x1616, 0x1616, 0x1616, 0x1216, 0x1617, 0x1717, 0x1717, 0x2316, 0x3013, 0x1e1e,
    0x151e, 0x1e15,
    0x391e, 0x143a, 0x383b, 0x3838, 0x3838, 0x3838, 0x3838, 0x3838, 0x0338, 0x3702, 0x2f30, 0x151e,
    0x220e, 0x2929,
    0x0421, 0x0404, 0x0e0e, 0x0e0e,
    0x1616, 0x1616, 0x1616, 0x1616, 0x1616, 0x1617, 0x1616, 0x1716, 0x1717, 0x1a18, 0x3023, 0x1e1e,
    0x0e15, 0x1d0e,
    0x3d3c, 0x3b01, 0x383b, 0x3838, 0x3838, 0x3838, 0x3838, 0x3838, 0x3838, 0x3803, 0x3037, 0x1e2f,
    0x0e15, 0x2222,
    0x2929, 0x0e0e, 0x0e0e, 0x030e,
    0x1616, 0x1612, 0x1616, 0x1616, 0x1616, 0x1817, 0x1612, 0x1716, 0x1717, 0x1a18, 0x3418, 0x1e1e,
    0x0e15, 0x3c1e,
    0x3b3e, 0x3b3b, 0x3838, 0x3838, 0x0138, 0x3801, 0x3838, 0x3838, 0x3838, 0x0338, 0x3738, 0x2f2f,
    0x0e15, 0x2222,
    0x2921, 0x0e21, 0x0303, 0x0303,
    0x1616, 0x1616, 0x1616, 0x1616, 0x1616, 0x3f16, 0x1711, 0x1616, 0x1717, 0x2318, 0x4012, 0x1e1e,
    0x371e, 0x4241,
    0x3b43, 0x383b, 0x3b38, 0x3d3b, 0x4544, 0x2c45, 0x030c, 0x3838, 0x3838, 0x3838, 0x0203, 0x3037,
    0x0e1d, 0x1504,
    0x1b1b, 0x0e1e, 0x030e, 0x0303,
    0x1711, 0x1616, 0x1616, 0x1616, 0x1616, 0x3f16, 0x1723, 0x1616, 0x1717, 0x1846, 0x4016, 0x1d1e,
    0x471e, 0x3e48,
    0x3b49, 0x3838, 0x3b3b, 0x4b4a, 0x4c4c, 0x454c, 0x064d, 0x3838, 0x3838, 0x3838, 0x0338, 0x2f0f,
    0x151d, 0x2f15,
    0x4e24, 0x2f32, 0x0e0e, 0x0303,
    0x120d, 0x1617, 0x1816, 0x1618, 0x1616, 0x4f17, 0x163f, 0x1717, 0x1617, 0x1819, 0x4016, 0x5039,
    0x5139, 0x4352,
    0x383b, 0x493b, 0x3b49, 0x4c53, 0x4c4c, 0x4c4c, 0x5445, 0x3855, 0x3838, 0x3838, 0x0338, 0x3702,
    0x152f, 0x301e,
    0x2630, 0x2656, 0x0e2f, 0x030e,

    0x1133, 0x1617, 0x2316, 0x1623, 0x1616, 0x5717, 0x123f, 0x1717, 0x1717, 0x1919, 0x5812, 0x4553,
    0x594d, 0x3b3e,
    0x3b38, 0x4949, 0x433b, 0x4c45, 0x4c4c, 0x4c4c, 0x4d4b, 0x0254, 0x3838, 0x3838, 0x3838, 0x0502,
    0x151d, 0x301d,
    0x302f, 0x564e, 0x1e32, 0x0e0e,
    0x5502, 0x1618, 0x1116, 0x121a, 0x1616, 0x1817, 0x195a, 0x1617, 0x1617, 0x1919, 0x5b12, 0x4545,
    0x5d5c, 0x3b14,
    0x4949, 0x4949, 0x4449, 0x4c4c, 0x4c4c, 0x4c4c, 0x454c, 0x032c, 0x3838, 0x3838, 0x3838, 0x0502,
    0x0e1e, 0x2f1d,
    0x1d2f, 0x5624, 0x1b4e, 0x0e15,
    0x3333, 0x173a, 0x1217, 0x191a, 0x1616, 0x1617, 0x113f, 0x1617, 0x1217, 0x1211, 0x5b16, 0x4545,
    0x515c, 0x4314,
    0x4949, 0x4949, 0x3e49, 0x4c53, 0x4c4c, 0x4c4c, 0x4c4c, 0x380c, 0x3b38, 0x3838, 0x3838, 0x0c14,
    0x1f2b, 0x2f1d,
    0x1d1d, 0x252f, 0x3256, 0x1f1e,
    0x3333, 0x180f, 0x1617, 0x191a, 0x1616, 0x1717, 0x233f, 0x1717, 0x1217, 0x1619, 0x5e16, 0x4b45,
    0x595f, 0x603f,
    0x4949, 0x4949, 0x4949, 0x4c3e, 0x4c4c, 0x4c4c, 0x454b, 0x384a, 0x3b3b, 0x3838, 0x3838, 0x0c55,
    0x350c, 0x2f1d,
    0x1e1d, 0x241d, 0x4e56, 0x0e1b,
    0x3333, 0x3a33, 0x1716, 0x1a11, 0x1612, 0x1716, 0x6157, 0x1716, 0x1217, 0x1123, 0x5812, 0x4b45,
    0x5c5f, 0x3e5d,
    0x4949, 0x4949, 0x4949, 0x6362, 0x6363, 0x454b, 0x4914, 0x3b3b, 0x3b3b, 0x383b, 0x0138, 0x0c3c,
    0x330b, 0x2f1d,
    0x1e1d, 0x1b0e, 0x564e, 0x1e32,
    0x3333, 0x0f33, 0x1758, 0x1a18, 0x1618, 0x1717, 0x5a18, 0x1718, 0x1817, 0x5a5a, 0x5812, 0x5b45,
    0x4c45, 0x615e,
    0x4962, 0x4949, 0x4949, 0x4949, 0x4949, 0x4364, 0x3b3b, 0x3b3b, 0x3b3b, 0x3b3b, 0x143b, 0x0b06,
    0x0f0b, 0x2f35,
    0x1e2f, 0x1e0e, 0x5626, 0x2f25,
    0x0f0f, 0x0f0f, 0x1606, 0x1a12, 0x1219, 0x1716, 0x5a12, 0x1719, 0x1817, 0x5a5a, 0x6512, 0x5b5b,
    0x4c4b, 0x485c,
    0x623e, 0x4949, 0x4949, 0x4949, 0x4949, 0x4949, 0x4949, 0x3b3b, 0x3b3b, 0x3b3b, 0x3c3b, 0x0b2c,
    0x0c0b, 0x2f35,
    0x1d2f, 0x1e1d, 0x4e24, 0x1c4e,
    0x0f05, 0x0f0f, 0x580f, 0x2316, 0x121a, 0x1716, 0x3f16, 0x1723, 0x1817, 0x3f5a, 0x6517, 0x665f,
    0x4c4c, 0x5c4c,
    0x615d, 0x4962, 0x4949, 0x4949, 0x4949, 0x4949, 0x4949, 0x3b49, 0x3b3b, 0x3b3b, 0x4214, 0x0b2c,
    0x0b0b, 0x1e35,
    0x2f2f, 0x1e2f, 0x4e24, 0x2456,

    0x0f0f, 0x0f05, 0x0c33, 0x2312, 0x182e, 0x1716, 0x4f16, 0x1661, 0x1917, 0x5767, 0x6517, 0x4550,
    0x4c4c, 0x4c4c,
    0x685c, 0x603f, 0x4962, 0x4949, 0x4949, 0x4949, 0x4949, 0x3b3b, 0x3b3b, 0x013b, 0x2c3c, 0x0b0b,
    0x0b0b, 0x0e0f,
    0x2f1d, 0x1e1d, 0x4e24, 0x2456,
    0x0f0f, 0x0f0f, 0x1f33, 0x193a, 0x192e, 0x1717, 0x6917, 0x1661, 0x2317, 0x113e, 0x5817, 0x4b50,
    0x4c4c, 0x4c4c,
    0x5c4c, 0x3f5d, 0x603e, 0x4962, 0x4949, 0x4949, 0x4949, 0x4949, 0x4343, 0x4a01, 0x5051, 0x0b0b,
    0x0b0b, 0x1f0c,
    0x2f15, 0x1d1d, 0x4e24, 0x244e,
    0x3333, 0x3333, 0x0f35, 0x582c, 0x231a, 0x1216, 0x1912, 0x185a, 0x2316, 0x233e, 0x1a19, 0x4c54,
    0x4c4c, 0x4c4c,
    0x4c4c, 0x6a5c, 0x4f48, 0x5a61, 0x5a5a, 0x3e3e, 0x523e, 0x6b6b, 0x3c42, 0x513c, 0x5463, 0x0b2c,
    0x0b0b, 0x330b,
    0x390e, 0x2f36, 0x4e1c, 0x1c4e,
    0x3502, 0x350e, 0x4c2d, 0x634c, 0x2b0c, 0x6c0b, 0x0b6c, 0x555a, 0x3c0c, 0x1a5a, 0x131a, 0x4c5f,
    0x4c4c, 0x4c4c,
    0x4c4c, 0x4c4c, 0x5e5c, 0x6868, 0x685e, 0x486a, 0x5c5d, 0x5c5c, 0x5968, 0x6341, 0x544d, 0x0b50,
    0x0b0b, 0x0c0b,
    0x450b, 0x2c4b, 0x2530, 0x2f26,
    0x3564, 0x6d0e, 0x4b4b, 0x6f6e, 0x7170, 0x7270, 0x7073, 0x7574, 0x2876, 0x2e3e, 0x581a, 0x4c5b,
    0x4c4c, 0x4c4c,
    0x4c4c, 0x4c4c, 0x4c4c, 0x5c4c, 0x4c5c, 0x5c5c, 0x5c5c, 0x5c5c, 0x5f5c, 0x6666, 0x544d, 0x0b50,
    0x0b0b, 0x2c0b,
    0x4b45, 0x534c, 0x1c39, 0x201b,
    0x643e, 0x4343, 0x7978, 0x7a7a, 0x7c7b, 0x7d6f, 0x7e7a, 0x7272, 0x767f, 0x5528, 0x131a, 0x4c5f,
    0x4c4c, 0x4c4c,
    0x4c4c, 0x4c4c, 0x4c4c, 0x4c4c, 0x4c4c, 0x4c4c, 0x5c5c, 0x5c5c, 0x5c5c, 0x5f5b, 0x5466, 0x2c50,
    0x0b0b, 0x500b,
    0x4b45, 0x454c, 0x3750, 0x2a20,
    0x6714, 0x8060, 0x7281, 0x827b, 0x8283, 0x7d84, 0x7d7d, 0x7e7e, 0x7f72, 0x8685, 0x2e3c, 0x4b51,
    0x4c4c, 0x4c4c,
    0x4c4c, 0x4c4c, 0x4c4c, 0x4c4c, 0x4c4c, 0x4c4c, 0x5c5c, 0x5c5c, 0x5c5c, 0x5f5b, 0x5466, 0x2c50,
    0x0b0b, 0x500b,
    0x4b45, 0x4b4c, 0x0b53, 0x2a20,
    0x8760, 0x6467, 0x7b70, 0x837b, 0x8388, 0x8489, 0x7b7b, 0x7d7d, 0x727e, 0x857f, 0x3c2a, 0x6651,
    0x4c4c, 0x4c4c,
    0x4c4c, 0x4c4c, 0x4c4c, 0x4c4c, 0x4c4c, 0x5c4c, 0x5c5c, 0x5c5c, 0x5c5c, 0x5f5b, 0x5466, 0x2c50,
    0x0b0b, 0x500b,
    0x4b45, 0x4c4c, 0x2d45, 0x8a79,

    0x8b80, 0x0b8c, 0x8889, 0x8888, 0x8888, 0x8388, 0x8283, 0x7b7b, 0x727d, 0x7f73, 0x7976, 0x5051,
    0x4c45, 0x4c4c,
    0x4c4c, 0x4c4c, 0x4c4c, 0x4c4c, 0x4c4c, 0x5c4c, 0x5c5c, 0x5c5c, 0x5c5c, 0x5f5b, 0x5466, 0x2c50,
    0x0b0b, 0x500b,
    0x4b45, 0x4c4c, 0x0745, 0x8a8a,
    0x8b80, 0x838d, 0x8e8e, 0x8888, 0x8888, 0x8888, 0x8388, 0x7b82, 0x7e7d, 0x7f73, 0x7685, 0x5007,
    0x4c66, 0x4c4c,
    0x4c4c, 0x4c4c, 0x4c4c, 0x4c4c, 0x4c4c, 0x5c4c, 0x5c5c, 0x5c5c, 0x5c5c, 0x5f5b, 0x5466, 0x2c50,
    0x0b0b, 0x500b,
    0x4b45, 0x454c, 0x6d53, 0x2727,
    0x7762, 0x8889, 0x8888, 0x8e88, 0x8888, 0x8888, 0x8888, 0x8283, 0x7e7b, 0x7372, 0x7685, 0x2d86,
    0x4b4d, 0x4c4c,
    0x4c4c, 0x4c4c, 0x4c4c, 0x4c4c, 0x4c4c, 0x5c4c, 0x5c5c, 0x5c5c, 0x5c5c, 0x5f5b, 0x5466, 0x2c50,
    0x0b0b, 0x540b,
    0x4c4b, 0x454c, 0x0754, 0x2727,
    0x8f43, 0x8888, 0x8888, 0x8888, 0x8888, 0x8888, 0x8888, 0x8388, 0x7d82, 0x737e, 0x7685, 0x0776,
    0x4b54, 0x4c4c,
    0x4c4c, 0x4c4c, 0x4c4c, 0x4c4c, 0x4c4c, 0x5c4c, 0x5c5c, 0x5c5c, 0x5c5c, 0x5f5b, 0x5466, 0x2c50,
    0x0b0b, 0x4b2c,
    0x4c4c, 0x4c4c, 0x5053, 0x276d,
    0x9190, 0x8883, 0x8888, 0x8888, 0x8888, 0x8888, 0x8888, 0x8388, 0x7d82, 0x737e, 0x7685, 0x508a,
    0x4554, 0x4c4c,
    0x4c4c, 0x4c4c, 0x4c4c, 0x4c4c, 0x4c4c, 0x5c4c, 0x5c5c, 0x5c5c, 0x5c5c, 0x5f5b, 0x5466, 0x2c50,
    0x0b07, 0x4c50,
    0x4c4c, 0x4c4c, 0x5445, 0x7950,
    0x6f92, 0x887b, 0x8888, 0x8888, 0x8888, 0x8888, 0x8888, 0x8388, 0x7d82, 0x737e, 0x7685, 0x540b,
    0x4d50, 0x4c4b,
    0x4c4b, 0x4c4c, 0x4c4c, 0x4c4c, 0x4c4c, 0x5c5c, 0x5c5c, 0x5c5c, 0x5c5c, 0x5f5b, 0x5466, 0x2c50,
    0x0b0b, 0x4c45,
    0x4c4c, 0x4c4c, 0x454c, 0x2d54,
    0x6f92, 0x837b, 0x8888, 0x8888, 0x8888, 0x8888, 0x8888, 0x8388, 0x7d82, 0x737e, 0x8f8a, 0x9493,
    0x675a, 0x8d95,
    0x5f54, 0x4c4b, 0x4c4c, 0x4c4c, 0x4c4c, 0x5c5c, 0x5c5c, 0x5c5c, 0x5b5c, 0x5f5b, 0x5466, 0x2c50,
    0x590b, 0x466a,
    0x4c5b, 0x685b, 0x4d96, 0x544d,
    0x6f92, 0x827b, 0x8888, 0x8888, 0x8888, 0x8888, 0x8888, 0x8388, 0x7d7b, 0x7c7e, 0x9397, 0x9998,
    0x6778, 0x6060,
    0x5152, 0x6654, 0x4c45, 0x4c4c, 0x4c4c, 0x5c5c, 0x5c5c, 0x5c5c, 0x5c5c, 0x5f5b, 0x5466, 0x2c50,
    0x9a59, 0x9c9b,
    0x9d63, 0x8b77, 0x678b, 0x2d8d,

    0x7c74, 0x827b, 0x8888, 0x8888, 0x8888, 0x8888, 0x8888, 0x8288, 0x847b, 0x9f9e, 0xa097, 0x9491,
    0x9999, 0x6778,
    0x3e60, 0x5042, 0x4d54, 0x4c5b, 0x4c4c, 0x5c5c, 0x5c5c, 0x6a5e, 0x686a, 0x665f, 0x544d, 0x1150,
    0x9b9b, 0x3ea1,
    0x8b67, 0x8b8b, 0x8b8b, 0x0f80,
    0x9190, 0x827d, 0x8882, 0x8888, 0x8888, 0x8888, 0x8888, 0x8983, 0xa231, 0x9f9f, 0x4497, 0x9842,
    0x9998, 0x6777,
    0x6767, 0x6760, 0x5042, 0x4d50, 0x5c5b, 0x5c5c, 0x4665, 0xa3a3, 0xa4a3, 0x5957, 0x2d2d, 0x9b11,
    0x69a3, 0x5a59,
    0x8b8b, 0x8b87, 0x8b8b, 0x43a5,
    0x9190, 0x7b6f, 0x8382, 0x8888, 0x8888, 0x8888, 0x8888, 0xa608, 0xa2a2, 0x9f9f, 0x6b9f, 0x593f,
    0x9942, 0x7877,
    0x7878, 0x6067, 0x9560, 0x508d, 0x4d50, 0x465f, 0x9a9b, 0x9b9b, 0xa39b, 0x9ba3, 0x136b, 0x189a,
    0x5b6a, 0x4445,
    0x778c, 0x774f, 0x8b8b, 0x62a5,
    0x9190, 0x7d91, 0x8282, 0x8883, 0x8888, 0x8888, 0xa788, 0xa2a6, 0xa2a2, 0xa2a2, 0x919e, 0xa897,
    0x93a8, 0x948c,
    0x7894, 0x6067, 0x6060, 0x523e, 0x2d50, 0xa36b, 0x9b9b, 0x9a9a, 0x9a9b, 0x9a9a, 0x69a3, 0x4c5c,
    0x4c4c, 0x4b4c,
    0x3f54, 0x774f, 0x8b8b, 0x62a5,
    0x91a9, 0xabaa, 0x827b, 0x8282, 0x8389, 0x8888, 0xa688, 0xa6a6, 0xa6a6, 0xa8a2, 0x9eac, 0x96a2,
    0xa2a2, 0x3f97,
    0x783c, 0x6767, 0x6060, 0x6060, 0x5042, 0x9a23, 0x9a9a, 0xad9a, 0x9a9b, 0x9a9a, 0x689b, 0x4c4c,
    0x4c4c, 0x4c4c,
    0x5445, 0x8b99, 0x8b8b, 0x2280,
    0x7529, 0xab91, 0xae7e, 0x6795, 0x9067, 0x88ae, 0xa608, 0xafa6, 0xa6af, 0x5d5d, 0xa6a6, 0x9d96,
    0xa2a2, 0x979f,
    0x7893, 0x6767, 0x6067, 0x6060, 0x3c0c, 0x9b9c, 0xad9a, 0xadad, 0xad9a, 0x9bad, 0x4c46, 0x4c4c,
    0x4c4c, 0x4c4c,
    0x454b, 0x87b0, 0xa58b, 0x220e,
    0x2921, 0xab75, 0x67ab, 0x8b8b, 0x8b8b, 0x7980, 0xa6a6, 0xafaf, 0xafaf, 0xa6af, 0xa6a6, 0xa6a6,
    0x9fa6, 0x9f97,
    0x9897, 0x6767, 0x6067, 0x6060, 0x14b1, 0x9b2e, 0xadad, 0x18ad, 0x1158, 0x2c37, 0x4c2c, 0x4c4c,
    0x4c4c, 0x4c4c,
    0x4c4c, 0x4453, 0x2278, 0x2222,
    0x29b2, 0x2829, 0x8b67, 0x8b8b, 0x8b8b, 0x80a5, 0xaf9f, 0xb3af, 0xb3b3, 0xafaf, 0xa6af, 0xa6a6,
    0x9fa6, 0x9f97,
    0x979f, 0x6798, 0x6060, 0x6060, 0x1464, 0xa42e, 0x19ad, 0x5f11, 0x2c45, 0x0f20, 0x4550, 0x4c4b,
    0x4c4c, 0x4c4c,
    0x4c4c, 0x4d4b, 0x6d0b, 0x2220,

    0x2995, 0x2929, 0x8767, 0x8cb4, 0x8b8b, 0xa5a5, 0xb6b5, 0xb3b3, 0xb3b3, 0xb3b3, 0x4f5d, 0xa6a6,
    0xa6a6, 0xa2a6,
    0x979f, 0x9993, 0x6067, 0x6060, 0x1464, 0x660d, 0x455c, 0x4c45, 0x454c, 0x5450, 0x5454, 0x4b45,
    0x4c4c, 0x4c4c,
    0x4c4c, 0x074c, 0x6d6d, 0x221f,
    0xb299, 0x2729, 0xb7b7, 0xb8b5, 0x8b87, 0x80a5, 0xb6b9, 0xb3b3, 0xb3b3, 0xb3b3, 0xa296, 0xafaf,
    0xa6a6, 0xa2a6,
    0x979f, 0x9897, 0x6778, 0x6060, 0x35ba, 0x4c2c, 0x4c4c, 0x4c4c, 0x4c4c, 0x5445, 0x5454, 0x4553,
    0x4c4b, 0x4c4c,
    0x4b4c, 0x6d31, 0x2007, 0x4a0e,
    0xbb8f, 0x2729, 0xbaba, 0x8cbc, 0x8b8b, 0x60a5, 0xbdbd, 0xb3b3, 0xb3b3, 0xb3b3, 0xb3b3, 0xafaf,
    0xa6a6, 0xa2a6,
    0x9fa2, 0x9897, 0x6778, 0x6060, 0x5260, 0x4c45, 0x4c4c, 0x4c4c, 0x4c4c, 0x4d4b, 0x5454, 0x454d,
    0x4545, 0x4c4c,
    0x0745, 0x3107, 0x202c, 0x312b,
    0xbe74, 0x2128, 0x67ba, 0xba64, 0xa580, 0xb8a5, 0xbdbd, 0xb3b6, 0xb3b3, 0xb3b3, 0xb3b3, 0xafb3,
    0xa6af, 0xa2a6,
    0x9fa2, 0x9897, 0x6778, 0x6060, 0x8d60, 0x4c45, 0x4c4c, 0x4c4c, 0x4c4c, 0x454c, 0x5454, 0x5454,
    0x4553, 0x3145,
    0x2d2d, 0x3131, 0x2d2d, 0x0b31,
    0xaa91, 0x28bb, 0xbca9, 0xbcbc, 0x4364, 0xbf43, 0xbdbd, 0xb6b6, 0xb3b3, 0xb3b3, 0xb3b3, 0xafb3,
    0xa6af, 0xa6a6,
    0x9fa2, 0x9897, 0x6778, 0x6060, 0x4260, 0x4c4c, 0x4c4c, 0x4c4c, 0x4c4c, 0x4c4c, 0x5445, 0x5454,
    0x2d50, 0x310b,
    0x2dc0, 0x0b07, 0xc02d, 0x332b,
    0xaabe, 0x86be, 0x2828, 0xb781, 0xb7b1, 0x9171, 0xbdac, 0xb6b6, 0xb3b6, 0xb3b3, 0xb3b3, 0xafb3,
    0xa6af, 0xa6a6,
    0x9fa2, 0x9897, 0x678c, 0x6060, 0xb060, 0x4c4c, 0x4c4c, 0x4c4c, 0x4c4c, 0x4c4c, 0x534b, 0x2c54,
    0x2b2b, 0x5307,
    0x0fc0, 0x221f, 0x2d07, 0x3333,
    0xaabe, 0xabaa, 0x2886, 0x2886, 0x7529, 0xaaaa, 0x9191, 0xbdac, 0xb6b6, 0xb6b6, 0xb3b3, 0xafaf,
    0xa6a6, 0xa2a6,
    0x9fa2, 0x9997, 0x8b87, 0xa58b, 0x9580, 0x4545, 0x4c4b, 0x4c4c, 0x4c4c, 0x4c4c, 0x534b, 0x2b07,
    0x0b0b, 0x2b33,
    0x222b, 0x2022, 0x0b31, 0x3333,
    0xaaaa, 0xaabe, 0x86ab, 0x2828, 0x7529, 0xaaaa, 0xaaaa, 0x9191, 0xbdac, 0xbdbd, 0xb6b6, 0xafb6,
    0xa6a6, 0xa2a6,
    0x9fa2, 0x7793, 0x8b8b, 0x8b8b, 0x60a5, 0x4563, 0x4c45, 0x4c4c, 0x4b4c, 0xa745, 0x0731, 0x0707,
    0x200b, 0x0404,
    0x2222, 0x6d22, 0x0f31, 0x3333,

    0xaaaa, 0xaabe, 0xabbe, 0x2886, 0x7528, 0xaaaa, 0xaaaa, 0xaaaa, 0x9191, 0x9191, 0xacbf, 0xbdbd,
    0xbdbd, 0xa2bd,
    0x939f, 0x8b77, 0x8b8b, 0x8b8b, 0xa58b, 0x968c, 0x4596, 0x5345, 0x3131, 0x0b07, 0x2d2d, 0x072d,
    0x0420, 0x0404,
    0x2222, 0x3127, 0x332d, 0x0303,
    0xaaaa, 0xaabe, 0xaabe, 0x86ab, 0x7528, 0xaaaa, 0xaaaa, 0xaaaa, 0xaac1, 0x91aa, 0x9191, 0x9191,
    0xbf91, 0xb9b9,
    0xb4b9, 0x8b8b, 0x7777, 0x8b8b, 0xa58b, 0x5064, 0x0b2d, 0x2d0b, 0xc031, 0x8d54, 0x2d2c, 0x2c2d,
    0x0420, 0x2204,
    0x2122, 0xc071, 0x030c, 0x0303,
    0xaabe, 0xbeaa, 0xaabe, 0xabaa, 0x2886, 0xaa8a, 0xaaaa, 0xaaaa, 0xaaaa, 0xaaaa, 0xaaaa, 0xaaaa,
    0xbb91, 0x91bb,
    0xbc91, 0x7887, 0xc2a0, 0x8c67, 0x6478, 0x07b2, 0x0707, 0x3107, 0xa7a7, 0x2cc0, 0x312d, 0x2d0a,
    0x040f, 0x2222,
    0xa921, 0xc00b, 0x032b, 0x0303,
    0xaabe, 0xbeaa, 0xaaaa, 0xaaaa, 0x86ab, 0x2829, 0xaa91, 0xaaaa, 0xaaaa, 0xaaaa, 0xaaaa, 0xaaaa,
    0xbeaa, 0x9191,
    0x9091, 0x9567, 0xa0a0, 0x9594, 0x3d95, 0x504a, 0x2d31, 0x312d, 0xa7a7, 0x0709, 0x0a07, 0x0a0a,
    0x222b, 0x2721,
    0xa9a9, 0xc00b, 0x032b, 0x0303,
    0xaaaa, 0xbeaa, 0xaaaa, 0xaaaa, 0xabaa, 0x298a, 0xbb29, 0xaaaa, 0xaaaa, 0xaaaa, 0xaaaa, 0xaaaa,
    0x91aa, 0x9191,
    0x9091, 0x6767, 0x9567, 0x9594, 0x3d95, 0x314a, 0x0b0a, 0x2d0b, 0xa7a7, 0x0708, 0x2033, 0x2020,
    0x2121, 0x81a9,
    0xb2a9, 0x6e42, 0x030c, 0x3803,
    0xbebe, 0xbebe, 0xaaaa, 0xaabe, 0xbeaa, 0x91be, 0x2875, 0xaa8a, 0xaaaa, 0xaaaa, 0xaaaa, 0xaaaa,
    0x91aa, 0x9191,
    0x9191, 0x60bc, 0xbaba, 0x9567, 0x3d95, 0x0f0f, 0x2220, 0x2022, 0xa707, 0x7908, 0x2222, 0x2122,
    0x8127, 0x81c3,
    0xb1a9, 0x6e8d, 0x032d, 0x3838,
    0xaabe, 0xaaaa, 0xbeaa, 0xbeaa, 0xaaaa, 0xaaaa, 0xabaa, 0x7586, 0xaabb, 0xaaaa, 0xaaaa, 0xaaaa,
    0x91aa, 0x9191,
    0xbb91, 0xb7a9, 0xbaba, 0x64ba, 0x0f64, 0x2020, 0x2222, 0x2122, 0x6c29, 0x296c, 0x2929, 0xa927,
    0xc481, 0xc3c4,
    0xb781, 0x452c, 0x03c0, 0x3803,
    0xbebe, 0xaaaa, 0xaaaa, 0xbeaa, 0xaaaa, 0xbebe, 0xaabe, 0xc4be, 0xbb86, 0xc1aa, 0xaaaa, 0xaaaa,
    0x91aa, 0x8abb,
    0x2975, 0x2129, 0xb1b2, 0xb1b1, 0x211f, 0x2020, 0x2122, 0x2921, 0x2929, 0x2929, 0xa927, 0xc381,
    0xc4c4, 0xc4c4,
    0xb1c3, 0x4b31, 0x0fa7, 0x3838,
};

const u16 pal_256plett[] = {
    0x1001, 0x14ec, 0x1d0b, 0x0c88, 0x1483, 0x2dad, 0x3a0f, 0x4a2f,     // 0000h
    0x7732, 0x66f1, 0x5e90, 0x41ef, 0x31ce, 0x1570, 0x10c5, 0x316b,
    0x6ef0, 0x26b9, 0x2b9e, 0x2235, 0x192e, 0x0d25, 0x2fbf, 0x2fff,     // 0001h
    0x2b3d, 0x26dc, 0x19f5, 0x0e29, 0x0e6b, 0x0d88, 0x0d66, 0x20e5,
    0x2d06, 0x2ce4, 0x20a2, 0x1e59, 0x12cd, 0x0f93, 0x1351, 0x3d06,     // 0002h
    0x4904, 0x38e3, 0x4548, 0x39ac, 0x4631, 0x5272, 0x15b4, 0x0dc9,
    0x11ec, 0x5eb3, 0x130f, 0x2949, 0x1a2f, 0x1d27, 0x2290, 0x19ab,     // 0003h
    0x0428, 0x2a0d, 0x3253, 0x042c, 0x29b1, 0x2952, 0x1d14, 0x25ba,
    0x1e94, 0x3ed7, 0x39f5, 0x1490, 0x4a57, 0x6f7b, 0x22fe, 0x3292,     // 0004h
    0x3ebc, 0x042f, 0x2d72, 0x77de, 0x7fff, 0x5af7, 0x17f4, 0x2dfd,
    0x4e94, 0x3a75, 0x31b5, 0x6719, 0x56d6, 0x296e, 0x17f7, 0x2a7d,     // 0005h
    0x371b, 0x3679, 0x2158, 0x5f9d, 0x67ff, 0x46de, 0x539f, 0x575a,
    0x18b6, 0x2198, 0x0871, 0x4eb9, 0x20f1, 0x43df, 0x5719, 0x20d8,     // 0006h
    0x4f5e, 0x227f, 0x473f, 0x2e57, 0x560b, 0x45cd, 0x7798, 0x7a88,
    0x6de7, 0x556f, 0x7a44, 0x7e23, 0x5172, 0x5127, 0x5920, 0x14bf,     // 0007h
    0x24fa, 0x498b, 0x76c4, 0x7f24, 0x6e2d, 0x7ee5, 0x7ea4, 0x71c3,
    0x0c57, 0x490c, 0x7f85, 0x7fa7, 0x7ac9, 0x6561, 0x5525, 0x0c5e,     // 0008h
    0x7fe9, 0x772b, 0x5569, 0x043f, 0x20fd, 0x4215, 0x67f1, 0x4176,
    0x4153, 0x69b3, 0x61b5, 0x3d9e, 0x2d5a, 0x3156, 0x529e, 0x49bf,     // 0009h
    0x355f, 0x293e, 0x131f, 0x0ebf, 0x05b8, 0x425f, 0x6e36, 0x51ff,
    0x59da, 0x05de, 0x5a3f, 0x065f, 0x123d, 0x043b, 0x667f, 0x7357,     // 000ah
    0x4e3d, 0x40ea, 0x7df6, 0x758e, 0x6e1a, 0x173f, 0x6aa9, 0x6e9f,
    0x3e39, 0x28ec, 0x34ea, 0x76df, 0x34fe, 0x599d, 0x7e7f, 0x2ccf,     // 000bh
    0x4d3d, 0x75bd, 0x24d4, 0x658e, 0x34f4, 0x7e3f, 0x7dd4, 0x69b9,
    0x62f5, 0x7e39, 0x4599, 0x612b, 0x796b, 0x0000, 0x0000, 0x0000,     // 000ch
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,     // 000dh
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,     // 000eh
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,     // 000fh
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};
