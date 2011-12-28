#ifndef _OOBCMDS_H
#define _OOBCMDS_H

#include "Command.h"
#include "net.h"

void JAMS_Heartbeat(Command *cmd, NetAdr *from, Q3OobMsg *msg);
void JAMS_StatusResponse(Command *cmd, NetAdr *from, Q3OobMsg *msg);

#endif