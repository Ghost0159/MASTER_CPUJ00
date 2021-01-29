/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - dataShare-Model
  File:     ball.h

  Copyright 2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: ball.h,v $
  Revision 1.2  2007/10/26 09:43:12  yosizaki
  add some rules of the collision.

  Revision 1.1  2007/10/19 09:32:18  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef BALL_H_
#define BALL_H_


#ifdef  __cplusplus
extern "C" {
#endif


/*===========================================================================*/

#define BALL_PLAYER_MAX         16
#define BALL_RADIUS             4
#define BALL_FIELD_WIDTH        256
#define BALL_FIELD_HEIGHT       192
#define BALL_VELOCITY_ONE       10
#define BALL_VELOCITY_ACCEL     5
#define BALL_VELOCITY_DECREASE  1
#define BALL_VELOCITY_MAX       (BALL_VELOCITY_ONE * 6)
#define BALL_VELOCITY_BOOST     (BALL_VELOCITY_ONE * 3)

#define BALL_BUTTON_A            0x0001 // A
#define BALL_BUTTON_B            0x0002 // B
#define BALL_BUTTON_SELECT       0x0004 // SELECT
#define BALL_BUTTON_START        0x0008 // START
#define BALL_BUTTON_KEY_RIGHT    0x0010 // RIGHT of cross key
#define BALL_BUTTON_KEY_LEFT     0x0020 // LEFT  of cross key
#define BALL_BUTTON_KEY_UP       0x0040 // UP    of cross key
#define BALL_BUTTON_KEY_DOWN     0x0080 // DOWN  of cross key
#define BALL_BUTTON_R            0x0100 // R
#define BALL_BUTTON_L            0x0200 // L
#define BALL_BUTTON_X            0x0400 // X
#define BALL_BUTTON_Y            0x0800 // Y


/*===========================================================================*/

typedef struct BallData
{
    int     x;
    int     y;
    int     vx;
    int     vy;
    u16     plt;
    u16     chr;
}
BallData;

typedef struct SharedContext
{
    BallData	ball[BALL_PLAYER_MAX];
    u8          disabledPlt;    // 接続されていない時のパレット番号
    u8          connectedPlt;   // 接続された時のパレット番号
    u8          ownPlt;         // 自分自身に使用するパレット番号
    u8          padding[1];
}
SharedContext;


/*===========================================================================*/

static SharedContext shared[1];


/*===========================================================================*/

SDK_INLINE void InitBall(const char *chrArray,
                         u8 disabledPlt, u8 connectedPlt, u8 ownPlt)
{
    int     i;
    if (chrArray == 0)
    {
        chrArray = "0123456789ABCDEF";
    }
    for (i = 0; i < BALL_PLAYER_MAX; ++i)
    {
        shared->ball[i].x = (190 + (i % 4) * 5);
        shared->ball[i].y = (40 + i * 8);
        shared->ball[i].vx = 0;
        shared->ball[i].vy = 0;
        shared->ball[i].plt = disabledPlt;
        shared->ball[i].chr = chrArray[i];
    }
    shared->disabledPlt = disabledPlt;
    shared->connectedPlt = connectedPlt;
    shared->ownPlt = ownPlt;
}

SDK_INLINE void InputBallKey(int aid, int key)
{
    if ((aid >= 0) && (aid < BALL_PLAYER_MAX))
    {
        if ((key & BALL_BUTTON_KEY_RIGHT) != 0)
        {
            if (shared->ball[aid].vx == 0)
            {
                shared->ball[aid].vx += BALL_VELOCITY_BOOST;
            }
            else if (shared->ball[aid].vx < +BALL_VELOCITY_MAX)
            {
                shared->ball[aid].vx += BALL_VELOCITY_ACCEL;
            }
        }
        else if ((key & BALL_BUTTON_KEY_LEFT) != 0)
        {
            if (shared->ball[aid].vx == 0)
            {
                shared->ball[aid].vx -= BALL_VELOCITY_BOOST;
            }
            else if (shared->ball[aid].vx > -BALL_VELOCITY_MAX)
            {
                shared->ball[aid].vx -= BALL_VELOCITY_ACCEL;
            }
        }
        if ((key & BALL_BUTTON_KEY_DOWN) != 0)
        {
            if (shared->ball[aid].vy == 0)
            {
                shared->ball[aid].vy += BALL_VELOCITY_BOOST;
            }
            else if (shared->ball[aid].vy < +BALL_VELOCITY_MAX)
            {
                shared->ball[aid].vy += BALL_VELOCITY_ACCEL;
            }
        }
        if ((key & BALL_BUTTON_KEY_UP) != 0)
        {
            if (shared->ball[aid].vy == 0)
            {
                shared->ball[aid].vy -= BALL_VELOCITY_BOOST;
            }
            else if (shared->ball[aid].vy > -BALL_VELOCITY_MAX)
            {
                shared->ball[aid].vy -= BALL_VELOCITY_ACCEL;
            }
        }
        shared->ball[aid].plt = shared->connectedPlt;
    }
}

SDK_INLINE void UpdateBalls(u16 ownAID)
{
    int     i, j;
    int     bound = 0;
    if ((ownAID >= 0) && (ownAID < BALL_PLAYER_MAX))
    {
        shared->ball[ownAID].plt = shared->ownPlt;
    }
    // 加減速および外壁衝突判定
    for (i = 0; i < BALL_PLAYER_MAX; ++i)
    {
        int     x = shared->ball[i].x;
        int     y = shared->ball[i].y;
        x += shared->ball[i].vx / BALL_VELOCITY_ONE;
        y += shared->ball[i].vy / BALL_VELOCITY_ONE;
        if ((x < 0) || (x > BALL_FIELD_WIDTH - BALL_RADIUS * 2))
        {
            shared->ball[i].vx *= -1;
            bound |= (1 << i);
        }
        else if (shared->ball[i].vx != 0)
        {
            shared->ball[i].vx -= (shared->ball[i].vx > 0) ? +BALL_VELOCITY_DECREASE : -BALL_VELOCITY_DECREASE;
        }
        if ((y < 0) || (y > BALL_FIELD_HEIGHT - BALL_RADIUS * 2))
        {
            shared->ball[i].vy *= -1;
            bound |= (1 << i);
        }
        else if (shared->ball[i].vy != 0)
        {
            shared->ball[i].vy -= (shared->ball[i].vy > 0) ? +BALL_VELOCITY_DECREASE : -BALL_VELOCITY_DECREASE;
        }
        if ((bound & (1 << i)) == 0)
        {
            shared->ball[i].x = x;
            shared->ball[i].y = y;
            for (j = 0; j < BALL_PLAYER_MAX; ++j)
            {
                if (j != i)
                {
                    int     dx = shared->ball[j].x - x;
                    int     dy = shared->ball[j].y - y;
                    if (dx * dx + dy * dy < (BALL_RADIUS * 2) * (BALL_RADIUS * 2))
                    {
                        BOOL    target_bound = ((bound & (1 << j)) != 0);
                        int     vx1 = shared->ball[i].vx;
                        int     vy1 = shared->ball[i].vy;
                        int     vx2, vy2;
                        if (!target_bound)
                        {
                            vx2 = shared->ball[j].vx;
                            vy2 = shared->ball[j].vy;
                        }
                        else
                        {
                            vx2 = 0;
                            vy2 = 0;
                        }
                        {
                            int     dvx = vx2 - vx1;
                            int     dvy = vy2 - vy1;
                            int     dx2 = dx * dx;
                            int     dy2 = dy * dy;
                            int     mag2 = dx2 + dy2;
                            int     crs = dx * dy;
                            shared->ball[i].vx = ((vx1 * dy2 + vx2 * dx2) + dvy * crs) / mag2;
                            shared->ball[i].vy = ((vy1 * dx2 + vy2 * dy2) + dvx * crs) / mag2;
                            if (!target_bound)
                            {
                                shared->ball[j].vx = ((vx1 * dx2 + vx2 * dy2) - dvy * crs) / mag2;
                                shared->ball[j].vy = ((vy1 * dx2 + vy2 * dy2) - dvx * crs) / mag2;
                                bound |= (1 << j);
                            }
                        }
                        bound |= (1 << i);
                        break;
                    }
                }
            }
        }
    }
}

/*===========================================================================*/

#ifdef  __cplusplus
}       /* extern "C" */
#endif

#endif /* BALL_H_ */

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
