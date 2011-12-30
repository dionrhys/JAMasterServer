

#ifndef _OOBCMDS_H
#define _OOBCMDS_H

#include "jams_local.h"

void JAMS_Heartbeat(Command *cmd, NetAdr *from, Q3OobMsg *msg);
void JAMS_StatusResponse(Command *cmd, NetAdr *from, Q3OobMsg *msg);
void JAMS_GetServers(Command *cmd, NetAdr *from, Q3OobMsg *msg);

#endif