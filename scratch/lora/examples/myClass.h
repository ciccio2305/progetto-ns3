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

        class LoraChannel;
        class LoraPhy;

        class my_device_with_lora : public NetDevice{
            public:
                static TypeId GetTypeId();

                // Constructor and destructor
                my_device_with_lora();
                ~my_device_with_lora() override;
                
                void FailedReception(Ptr<const Packet> packet);
                void TxFinished(Ptr<const Packet> packet);
                
                /**
                 * Set which LorawanMac instance is linked to this device.
                 *
                 * \param mac the mac layer to use.
                 */
                void SetMac(Ptr<LorawanMac> mac);

                /**
                 * Set which LoraPhy instance is linked to this device.
                 *
                 * \param phy the phy layer to use.
                 */
                void SetPhy(Ptr<LoraPhy> phy, Ptr<LogicalLoraChannelHelper> helper);

                /**
                 * Get the LorawanMac instance that is linked to this NetDevice.
                 *
                 * \return the mac we are currently using.
                 */
                Ptr<LorawanMac> GetMac() const;

                /**
                 * Get the LoraPhy instance that is linked to this NetDevice.
                 *
                 * \return the phy we are currently using.
                 */
                Ptr<LoraPhy> GetPhy() const;

                /**
                 * Send a packet through the LoRaWAN stack.
                 *
                 * \param packet The packet to send.
                 */
                void Send(Ptr<Packet> packet);

                /**
                 * This function is implemented to achieve compliance with the NetDevice
                 * interface. Note that the dest and protocolNumber args are ignored.
                 */
                bool Send(Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber) override;

                /**
                 * Callback the Mac layer calls whenever a packet arrives and needs to be
                 * forwarded up the stack.
                 *
                 * \param packet The packet that was received.
                 */
                void Receive(Ptr<Packet> packet);

                // From class NetDevice. Some of these have little meaning for a LoRaWAN
                // network device (since, for instance, IP is not used in the standard)
                void SetReceiveCallback(NetDevice::ReceiveCallback cb) override;
                Ptr<Channel> GetChannel() const override;
                void SetNode(Ptr<Node> node) override;
                Ptr<Node> GetNode() const override;

                void SetIfIndex(const uint32_t index) override;
                uint32_t GetIfIndex() const override;
                void SetAddress(Address address) override;
                Address GetAddress() const override;
                bool SetMtu(const uint16_t mtu) override;
                uint16_t GetMtu() const override;
                bool IsLinkUp() const override;
                void AddLinkChangeCallback(Callback<void> callback) override;
                bool IsBroadcast() const override;
                Address GetBroadcast() const override;
                bool IsMulticast() const override;
                Address GetMulticast(Ipv4Address multicastGroup) const override;
                Address GetMulticast(Ipv6Address addr) const override;
                bool IsBridge() const override;
                bool IsPointToPoint() const override;
                bool SendFrom(Ptr<Packet> packet,
                            const Address& source,
                            const Address& dest,
                            uint16_t protocolNumber) override;
                bool NeedsArp() const override;
                void SetPromiscReceiveCallback(PromiscReceiveCallback cb) override;
                bool SupportsSendFrom() const override;
                Ptr<LogicalLoraChannelHelper> GetLogicalLoraChannelHelper();

                /**
                 * Set the LogicalLoraChannelHelper this MAC instance will use.
                 *
                 * \param helper The instance of the helper to use.
                 */
                void SetLogicalLoraChannelHelper(Ptr<LogicalLoraChannelHelper> helper);
            protected:
                /**
                 * Receive a packet from the lower layer and pass the
                 * packet up the stack.
                 *
                 * \param packet The packet we need to forward.
                 * \param from The from address.
                 * \param to The to address.
                 */
                void ForwardUp(Ptr<Packet> packet, Mac48Address from, Mac48Address to);

            private:
                /**
                 * Return the LoraChannel this device is connected to.
                 */
                Ptr<LoraChannel> DoGetChannel() const;

                /**
                 * Complete the configuration of this LoRa device by connecting all lower
                 * components (PHY, MAC, Channel) together.
                 */
                void CompleteConfig();

                // Member variables
                Ptr<Node> m_node;      //!< The Node this NetDevice is connected to.
                Ptr<LoraPhy> m_phy;    //!< The LoraPhy this NetDevice is connected to.
                bool m_configComplete; //!< Whether the configuration was already completed.
                
                Ptr<LogicalLoraChannelHelper> m_channelHelper;
                /**
                 * Upper layer callback used for notification of new data packet arrivals.
                 */
                NetDevice::ReceiveCallback m_receiveCallback;

        };
        
    }
}

#endif