#ifndef myNetDevice_H
#define myNetDevice_H

#include "ns3/command-line.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/end-device-lora-phy.h"
#include "ns3/end-device-lorawan-mac.h"
#include "ns3/gateway-lora-phy.h"
#include "ns3/gateway-lorawan-mac.h"
#include "ns3/log.h"
#include "ns3/lora-helper.h"
#include "ns3/mobility-helper.h"
#include "ns3/node-container.h"
#include "ns3/one-shot-sender-helper.h"
#include "ns3/position-allocator.h"
#include "ns3/simulator.h"

#include <algorithm>
#include <ctime>

namespace ns3 {
    namespace lorawan {
        class myNetDevice {
            private: 
                Ptr<Node> m_node;      //!< The Node this NetDevice is connected to.
                Ptr<LoraPhy> m_phy;    //!< The LoraPhy this NetDevice is connected to.
                bool m_configComplete; //!< Whether the configuration was already completed.

            public:
                myNetDevice();
                ~myNetDevice();

                void setPhy(Ptr<LoraPhy> phy);
                void setNode(Ptr<Node> node);

                Ptr<LoraPhy> getPhy();
                Ptr<Node> getNode();

                void send(Ptr<Packet> packet);

                void Receive(Ptr<Packet> packet);

        };
    }
}

#endif