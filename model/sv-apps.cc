#include "sv-apps.h"

#include "ns3/simulator.h"
#include "ethernet-client.h"

ns3::SVClient::
SVClient()
{
    NS_LOG_FUNCTION(this);
    this->ethSocket = nullptr;
}

ns3::TypeId
ns3::SVClient::GetTypeId()
{
    static TypeId tid =
    TypeId("ns3::SVClient")
        .SetParent<Application>()
        .SetGroupName("LibIEC61850")
        .AddConstructor<SVClient>();
    return tid;
}

void
ns3::SVClient::SetServerAddress(PacketSocketAddress serverAddr)
{
    NS_LOG_FUNCTION(this);
    this->serverAddr = serverAddr;
}

void
ns3::SVClient::StartApplication()
{
    NS_LOG_FUNCTION(this);

    auto nodeId = this->GetNode()->GetId();
    auto path = "/NodeList/" + std::to_string(nodeId) + "/DeviceList/0";

    this->svPublisher = SVPublisher_create(nullptr, path.c_str());

    this->asdu1 = SVPublisher_addASDU(this->svPublisher, "svpub1", nullptr, 1);
    this->float1 = SVPublisher_ASDU_addFLOAT(this->asdu1);
    this->float2 = SVPublisher_ASDU_addFLOAT(this->asdu1);
    this->ts1 = SVPublisher_ASDU_addTimestamp(this->asdu1);

    this->asdu2 = SVPublisher_addASDU(this->svPublisher, "svpub2", nullptr, 1);
    this->float3 = SVPublisher_ASDU_addFLOAT(this->asdu2);
    this->float4 = SVPublisher_ASDU_addFLOAT(this->asdu2);
    this->ts2 = SVPublisher_ASDU_addTimestamp(this->asdu2);

    SVPublisher_setupComplete(svPublisher);

    this->fVal1 = 1234.5678f;
    this->fVal2 = 0.12345f;

    this->count = 10;
    Simulator::ScheduleNow(&SVClient::Send, this);
}

void
ns3::SVClient::StopApplication()
{
    NS_LOG_FUNCTION(this);
    libiec61850::Ethernet_destroySocket(this->ethSocket);
}

void
ns3::SVClient::Send()
{
    NS_LOG_FUNCTION(this);

    if (this->count < 0) return;

    Timestamp ts;
    Timestamp_clearFlags(&ts);
    Timestamp_setTimeInMilliseconds(&ts, libiec61850::Hal_getTimeInMs());

    SVPublisher_ASDU_setFLOAT(this->asdu1, float1, fVal1);
    SVPublisher_ASDU_setFLOAT(this->asdu1, float2, fVal2);

    SVPublisher_ASDU_setFLOAT(this->asdu2, float3, fVal1 * 2);
    SVPublisher_ASDU_setFLOAT(this->asdu2, float4, fVal2 * 2);

    SVPublisher_ASDU_increaseSmpCnt(asdu1);
    SVPublisher_ASDU_increaseSmpCnt(asdu2);

    fVal1 += 1.1f;
    fVal2 += 0.1f;

    SVPublisher_publish(svPublisher);

    this->count--;

    Simulator::Schedule(MilliSeconds(50), &SVClient::Send, this);
}