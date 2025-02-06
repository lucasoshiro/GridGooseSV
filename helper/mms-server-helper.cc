//
// Created by lso on 06/02/25.
//

#include "mms-server-helper.h"
#include "ns3/mms-server.h"

namespace ns3 {

MmsServerHelper::MmsServerHelper()
: ApplicationHelper(MmsServer::GetTypeId())  {

}
}