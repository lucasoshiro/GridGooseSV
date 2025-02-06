//
// Created by lso on 05/02/25.
//

#ifndef MMS_SERVER_H
#define MMS_SERVER_H

#include "ns3/application.h"

using namespace ns3;

class MmsServer : public Application
{
  public:
    static TypeId GetTypeId();
    MmsServer();
    ~MmsServer();

  private:
    void StartApplication();
    void StopApplication();
};

#endif //MMS_SERVER_H
