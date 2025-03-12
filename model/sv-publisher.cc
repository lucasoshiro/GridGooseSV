#include "sv-publisher.h"

#include "../../../src/core/model/integer.h"
#include "ethernet-client.h"

#include "ns3/simulator.h"
#include "ns3/uinteger.h"

#include <fstream>

ns3::SVPublisher::
SVPublisher()
{
    NS_LOG_FUNCTION(this);
    this->ethSocket = nullptr;
}

ns3::TypeId
ns3::SVPublisher::GetTypeId()
{
    static TypeId tid =
    TypeId("ns3::SVClient")
        .SetParent<Application>()
       .SetGroupName("LibIEC61850")
        .AddConstructor<SVPublisher>()
        .AddAttribute(
            "MaxPackets",
            "Maximum number of packets to send or 0 for inifinite",
            UintegerValue(0),
            MakeUintegerAccessor(&SVPublisher::count),
            MakeUintegerChecker<u_int64_t>()
            )
        .AddAttribute(
            "Interval",
            "Interval between packet sent. Equivalent to the SV sample rate",
            TimeValue(MilliSeconds(50)),
            MakeTimeAccessor(&SVPublisher::interval),
            MakeTimeChecker()
            )
        .AddTraceSource(
            "Sent",
            "Number of sent packages",
            MakeTraceSourceAccessor(&SVPublisher::sent),
            "ns3::TracedValueCallback::Uint64"
            );

    return tid;
}

void
ns3::SVPublisher::StartApplication()
{
    NS_LOG_FUNCTION(this);
    auto nodeId = this->GetNode()->GetId();
    auto path = "/NodeList/" + std::to_string(nodeId) + "/DeviceList/0";

    this->svPublisher = libiec61850::SVPublisher_create(nullptr, path.c_str());

    this->asdu1 = libiec61850::SVPublisher_addASDU(this->svPublisher, "svpub1", nullptr, 1);
    this->float1 = libiec61850::SVPublisher_ASDU_addFLOAT(this->asdu1);
    this->float2 = libiec61850::SVPublisher_ASDU_addFLOAT(this->asdu1);
    this->ts1 = libiec61850::SVPublisher_ASDU_addTimestamp(this->asdu1);

    this->asdu2 = libiec61850::SVPublisher_addASDU(this->svPublisher, "svpub2", nullptr, 1);
    this->float3 = libiec61850::SVPublisher_ASDU_addFLOAT(this->asdu2);
    this->float4 = libiec61850::SVPublisher_ASDU_addFLOAT(this->asdu2);
    this->ts2 = libiec61850::SVPublisher_ASDU_addTimestamp(this->asdu2);

    libiec61850::SVPublisher_setupComplete(this->svPublisher);

    this->fVal1 = 1234.5678f;
    this->fVal2 = 0.12345f;

    if (this->count <= 0) this->count = -1;
    this->sent.Set(0);

    ns3::Simulator::ScheduleNow(&SVPublisher::Send, this);
}

void
ns3::SVPublisher::StopApplication()
{
    NS_LOG_FUNCTION(this);
    libiec61850::Ethernet_destroySocket(this->ethSocket);
}

void
ns3::SVPublisher::Send()
{
    NS_LOG_FUNCTION(this);

    if (this->count == 0) return;

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
    this->sent++;

    Simulator::Schedule(this->interval, &SVPublisher::Send, this);
}