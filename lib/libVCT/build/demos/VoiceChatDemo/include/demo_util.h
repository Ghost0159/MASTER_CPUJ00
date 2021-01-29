#ifndef __VCT_DEMO_UTIL_H__
#define __VCT_DEMO_UTIL_H__

#include <nitro.h>
#include <dwc.h>
#include "vct.h"
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DEMO_EXCHANGE_MAGIC_TOKEN '_Vdm'

typedef enum _demo_status {
    DEMO_STATUS_JOIN,
    DEMO_STATUS_PHONE,
    DEMO_STATUS_TRANSCEIVER,
    DEMO_STATUS_CONFERENCE,
    DEMO_STATUS_TALKING,
	DEMO_STATUS_ALIVE,
    DEMO_STATUS_EXIT,
    NUM_OF_DEMO_STATUS
} demo_status;

typedef struct _client_info
{
    u32             magic;
    int             profileID;
    demo_status     status;
} client_info;

extern client_info gClientInfo[8];

void BroadCastStatus(demo_status sts);
void ConnectionClosedCallback(DWCError error,
                              BOOL isLocal,
                              BOOL isServer,
                              u8  aid,
                              int index,
                              void* param);

void UpdateClientInfo( u8 aid, client_info *info );
void UpdateMenuList(GameSequence *seq);
void ClearMenuList();
    
void AddMainTick(u64 tick);
void AddWaitTick(u64 tick);
void AddAudioTick(u64 tick);

const char* EventName(VCTEvent event);
const char* StateName(VCTState state);

void ShowTicks(const char *state_name);

#ifdef __cplusplus
}
#endif

#endif // __VCT_DEMO_UTIL_H__
