#ifndef myNetDevice_
#define myNetDevice_

#include "myClass.h"

#include "ns3/abort.h"
#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/pointer.h"

using namespace std;

namespace ns3 {
    namespace lorawan {
        

        // NS_LOG_COMPONENT_DEFINE("my_device_with_lora");

        // NS_OBJECT_ENSURE_REGISTERED(my_device_with_lora);

        TypeId my_device_with_lora::GetTypeId() {

            static TypeId tid =
                TypeId("ns3::my_device_with_lora")
                    .SetParent<NetDevice>()
                    .AddConstructor<my_device_with_lora>()
                    .AddAttribute("Channel",
                                "The channel attached to this device",
                                PointerValue(),
                                MakePointerAccessor(&my_device_with_lora::DoGetChannel),
                                MakePointerChecker<LoraChannel>())
                    .AddAttribute("Phy",
                                "The PHY layer attached to this device.",
                                PointerValue(),
                                MakePointerAccessor(&my_device_with_lora::GetPhy, &my_device_with_lora::SetPhy),
                                MakePointerChecker<LoraPhy>());
       
            return tid;
        }

        my_device_with_lora::my_device_with_lora()
            : m_node(nullptr),
            m_phy(nullptr),
            m_configComplete(false)
        {
            //NS_LOG_FUNCTION_NOARGS();
        }

        my_device_with_lora::~my_device_with_lora()
        {
            //NS_LOG_FUNCTION_NOARGS();
        }


        void my_device_with_lora::SetPhy(Ptr<LoraPhy> phy, Ptr<LogicalLoraChannelHelper> helper){
            std::cout << "Setting phy" << std::endl;
            m_phy = phy;
            m_channelHelper = helper;
            
            //m_phy->SetReceiveOkCallback(MakeCallback(&my_device_with_lora::Receive, this));
            m_phy->SetReceiveFailedCallback(MakeCallback(&my_device_with_lora::FailedReception, this));
            m_phy->SetTxFinishedCallback(MakeCallback(&my_device_with_lora::TxFinished, this));
        }
        

        void my_device_with_lora::FailedReception(Ptr<const Packet> packet){
            std::cout << "Failed reception" << std::endl;
        }

        void my_device_with_lora::TxFinished(Ptr<const Packet> packet){
            std::cout << "Tx finished" << std::endl;
        }

        Ptr<LoraPhy> my_device_with_lora::GetPhy() const {
            return m_phy;
        }

        void my_device_with_lora::CompleteConfig(){
            //NS_LOG_FUNCTION_NOARGS();

            // Verify we have all the necessary pieces
            if (!m_phy || !m_node || m_configComplete)
            {
                return;
            }

            m_configComplete = true;
        }
        
        Ptr<LogicalLoraChannelHelper> my_device_with_lora::GetLogicalLoraChannelHelper(){
            return m_channelHelper;
        }

        /**
         * Set the LogicalLoraChannelHelper this MAC instance will use.
         *
         * \param helper The instance of the helper to use.
         */
        void my_device_with_lora::SetLogicalLoraChannelHelper(Ptr<LogicalLoraChannelHelper> helper){
            m_channelHelper = helper;
        }

        void my_device_with_lora::Send(Ptr<Packet> packet)
        {
            //NS_LOG_FUNCTION(this << packet);
            
            //va creato il livello mac e mandato direttamente sul livello fisico
            std::cout << "Sending packet here" << std::endl;

            //300 Bps
            LoraTxParameters params;
            params.sf = 8;
            params.headerDisabled = false;
            params.codingRate = 2;
            params.bandwidthHz = 125;
            params.nPreamble = 8;
            params.crcEnabled = true;
            params.lowDataRateOptimizationEnabled = LoraPhy::GetTSym(params) > MilliSeconds(16);

            Time duration = m_phy->GetOnAirTime(packet, params);
            

            double sendingPower = m_channelHelper->GetTxPowerForChannel(CreateObject<LogicalLoraChannel>(868e6));

            m_channelHelper->AddEvent(duration, CreateObject<LogicalLoraChannel>(868e6));

            m_phy->Send(packet, params, 868e6, sendingPower);



        }

        void my_device_with_lora::Receive(Ptr<Packet> packet)
        {
            //NS_LOG_FUNCTION(this << packet);

            // Fill protocol and address with empty stuff
            //NS_LOG_DEBUG("Calling receiveCallback");
            std::cout << "Calling receiveCallback" << std::endl;
            m_receiveCallback(this, packet, 0, Address());
        }

        /******************************************
         *    Methods inherited from NetDevice    *
         ******************************************/

        Ptr<Channel> my_device_with_lora::GetChannel() const{
            //NS_LOG_FUNCTION(this);
            return m_phy->GetChannel();
        }

        Ptr<LoraChannel> my_device_with_lora::DoGetChannel() const{
            //NS_LOG_FUNCTION(this);
            return m_phy->GetChannel();
        }

        void my_device_with_lora::SetIfIndex(const uint32_t index){
            //NS_LOG_FUNCTION(this << index);
        }

        uint32_t my_device_with_lora::GetIfIndex() const{
            //NS_LOG_FUNCTION(this);

            return 0;
        }

        void my_device_with_lora::SetAddress(Address address){
            //NS_LOG_FUNCTION(this);
        }

        Address my_device_with_lora::GetAddress() const{
            //NS_LOG_FUNCTION(this);

            return Address();
        }

        bool my_device_with_lora::SetMtu(const uint16_t mtu){
            //NS_ABORT_MSG("Unsupported");

            return false;
        }

        uint16_t my_device_with_lora::GetMtu() const{
            //NS_LOG_FUNCTION(this);

            return 0;
        }

        bool my_device_with_lora::IsLinkUp() const{
            //NS_LOG_FUNCTION(this);

            return m_phy != nullptr;
        }

        void my_device_with_lora::AddLinkChangeCallback(Callback<void> callback){
            //NS_LOG_FUNCTION(this);
        }

        bool my_device_with_lora::IsBroadcast() const{
            //NS_LOG_FUNCTION(this);
            return true;
        }

        Address my_device_with_lora::GetBroadcast() const{
            //NS_LOG_FUNCTION(this);

            return Address();
        }

        bool my_device_with_lora::IsMulticast() const {
            //NS_LOG_FUNCTION(this);

            return true;
        }

        Address my_device_with_lora::GetMulticast(Ipv4Address multicastGroup) const{
            //NS_ABORT_MSG("Unsupported");

            return Address();
        }

        Address my_device_with_lora::GetMulticast(Ipv6Address addr) const{
            //NS_LOG_FUNCTION(this);

            return Address();
        }

        bool my_device_with_lora::IsBridge() const{
           // NS_LOG_FUNCTION(this);

            return false;
        }

        bool my_device_with_lora::IsPointToPoint() const{
            //NS_LOG_FUNCTION(this);

            return false;
        }

        bool my_device_with_lora::Send(Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber){  
            //NS_LOG_FUNCTION(this << packet << dest << protocolNumber);

            // Fallback to the vanilla Send method
            Send(packet);

            return true;
        }

        bool my_device_with_lora::SendFrom(Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber){
            //NS_ABORT_MSG("Unsupported");
            return false;
        }

        Ptr<Node> my_device_with_lora::GetNode() const{
           // NS_LOG_FUNCTION(this);

            return m_node;
        }

        void my_device_with_lora::SetNode(Ptr<Node> node) {
            //NS_LOG_FUNCTION(this);

            m_node = node;
            CompleteConfig();
        }

        bool my_device_with_lora::NeedsArp() const{
           // NS_LOG_FUNCTION(this);

            return true;
        }

        void my_device_with_lora::SetReceiveCallback(ReceiveCallback cb){
            //NS_LOG_FUNCTION_NOARGS();
            m_receiveCallback = cb;
        }

        void my_device_with_lora::SetPromiscReceiveCallback(PromiscReceiveCallback cb)
        {
            //NS_LOG_FUNCTION_NOARGS();
        }

        bool
        my_device_with_lora::SupportsSendFrom() const {
            //NS_LOG_FUNCTION_NOARGS();

            return false;
        }

    }
}



#endif