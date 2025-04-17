#include "sv-publisher-app.h"
#include "ns3/simulator.h"
#include "ns3/uinteger.h"

#include <fstream>
#include <cmath>

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
    TypeId("ns3::SVPublisher")
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
            "DeviceIndex",
            "Index of the NetDevice that will be used to send message. 0 by default",
            UintegerValue(0),
            MakeUintegerAccessor(&SVPublisher::deviceIndex),
            MakeUintegerChecker<u_int64_t>()
        )
        .AddAttribute(
            "Frequency",
            "AC frequency",
            UintegerValue(60),
            MakeUintegerAccessor(&SVPublisher::frequency),
            MakeUintegerChecker<u_int64_t>()
        )
        .AddAttribute(
            "SamplesPerCycle",
            "Samples per cycle",
            UintegerValue(1),
            MakeUintegerAccessor(&SVPublisher::samplesPerCycle),
            MakeUintegerChecker<u_int64_t>()
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

    this->interval = MicroSeconds(1000000. / (this->frequency * this->samplesPerCycle));

    auto nodeId = this->GetNode()->GetId();
    auto path = "/NodeList/" + std::to_string(nodeId) + "/DeviceList/" + std::to_string(this->deviceIndex);

    this->svPublisher = libiec61850::SVPublisher_create(nullptr, path.c_str());

    // TODO: we should allow the user to add as many ASDUs as wanted
    this->asdu1 = libiec61850::SVPublisher_addASDU(this->svPublisher, "svpub1", nullptr, 1);

    for (int i = 0; i < 8; i++) {
        this->offsets[i] = libiec61850::SVPublisher_ASDU_addFLOAT(this->asdu1);
    }

    this->ts1 = libiec61850::SVPublisher_ASDU_addTimestamp(this->asdu1);

    libiec61850::SVPublisher_setupComplete(this->svPublisher);

    if (this->count <= 0) this->count = -1;
    this->sent.Set(0);

    this->eventId = ns3::Simulator::ScheduleNow(&SVPublisher::Send, this);
}

void
ns3::SVPublisher::StopApplication()
{
    NS_LOG_FUNCTION(this);
    ns3::Simulator::Cancel(this->eventId);
    libiec61850::SVPublisher_destroy(this->svPublisher);
    // TODO: find out why this is leading to segfault. Probably, something is going on under PacketSocket->Close()
    //libiec61850::Ethernet_destroySocket(this->ethSocket);
}

void
ns3::SVPublisher::Send()
{
    NS_LOG_FUNCTION(this);

    if (this->count == 0) return;

    Timestamp ts;
    Timestamp_clearFlags(&ts);
    Timestamp_setTimeInNanoseconds(&ts, libiec61850::Hal_getTimeInNs());

    this->UpdateValues();

    for (int i = 0; i < 8; i++) {
        SVPublisher_ASDU_setFLOAT(this->asdu1, this->offsets[i], this->vals[i]);
    }

    SVPublisher_ASDU_increaseSmpCnt(asdu1);
    libiec61850::SVPublisher_ASDU_setTimestamp(this->asdu1, this->ts1, ts);

    SVPublisher_publish(svPublisher);

    this->count--;
    this->sent++;

    this->eventId = Simulator::Schedule(this->interval, &SVPublisher::Send, this);
}

void ns3::SVPublisher::UpdateValues() {
    const float twoPi = 2 * std::numbers::pi;
    const float nowNs = Simulator::Now().GetNanoSeconds();
    const float periodNs = 1000000000.0 / this->frequency;

    // TODO: this should be a parameter
    const float amplitude = 18000;

    // Current readings
    for (int i = 0; i < 3; i++) {
        const float phaseAngle = i * twoPi / 3;
        const float angle = phaseAngle + twoPi * nowNs / periodNs;
        const float value = sin(angle) * amplitude;
        this->vals[i] = value;
    }
    this->vals[3] = 0;

    // Voltage readings
    for (int i = 4; i < 7; i++) {
        this->vals[i] = 0;
    }
    this->vals[7] = 0;
}
