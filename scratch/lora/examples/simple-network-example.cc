/*
 * Copyright (c) 2017 University of Padova
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Davide Magrin <magrinda@dei.unipd.it>
 */

/*
 * This script simulates a simple network in which one end device sends one
 * packet to the gateway.
 */

#include "myClass.cc"

using namespace ns3;
using namespace lorawan;
NS_LOG_COMPONENT_DEFINE("SimpleLorawanNetworkExample");

void sendPacket(Ptr<my_device_with_lora> Device)
{
    std::cout << "sending packet" << std::endl;
    Ptr<Packet> pkt = Create<Packet>(100);

    Device->Send(pkt);
}

int
main(int argc, char* argv[])
{
    // Set up logging
    // LogComponentEnable("SimpleLorawanNetworkExample", LOG_LEVEL_ALL);
    // LogComponentEnable("LoraChannel", LOG_LEVEL_INFO);
    // LogComponentEnable("LoraPhy", LOG_LEVEL_ALL);
    // LogComponentEnable("EndDeviceLoraPhy", LOG_LEVEL_ALL);
    // LogComponentEnable("GatewayLoraPhy", LOG_LEVEL_ALL);
    // LogComponentEnable("LoraInterferenceHelper", LOG_LEVEL_ALL);
    // LogComponentEnable("LorawanMac", LOG_LEVEL_ALL);
    // LogComponentEnable("EndDeviceLorawanMac", LOG_LEVEL_ALL);
    // LogComponentEnable("ClassAEndDeviceLorawanMac", LOG_LEVEL_ALL);
    // LogComponentEnable("GatewayLorawanMac", LOG_LEVEL_ALL);
    // LogComponentEnable("LogicalLoraChannelHelper", LOG_LEVEL_ALL);
    // LogComponentEnable("LogicalLoraChannel", LOG_LEVEL_ALL);
    // LogComponentEnable("LoraHelper", LOG_LEVEL_ALL);
    // LogComponentEnable("LoraPhyHelper", LOG_LEVEL_ALL);
    // LogComponentEnable("LorawanMacHelper", LOG_LEVEL_ALL);
    // LogComponentEnable("OneShotSenderHelper", LOG_LEVEL_ALL);
    // LogComponentEnable("OneShotSender", LOG_LEVEL_ALL);
    // LogComponentEnable("LorawanMacHeader", LOG_LEVEL_ALL);
    // LogComponentEnable("LoraFrameHeader", LOG_LEVEL_ALL);
    // LogComponentEnableAll(LOG_PREFIX_FUNC);
    // LogComponentEnableAll(LOG_PREFIX_NODE);
    // LogComponentEnableAll(LOG_PREFIX_TIME);

    /************************
     *  Create the channel  *
     ************************/

    NS_LOG_INFO("Creating the channel...");

    std::cout << "Creating the channel..." << std::endl;

    // Create the lora channel object
    Ptr<LogDistancePropagationLossModel> loss = CreateObject<LogDistancePropagationLossModel>();
    loss->SetPathLossExponent(3.76);
    loss->SetReference(1, 7.7);

    Ptr<PropagationDelayModel> delay = CreateObject<ConstantSpeedPropagationDelayModel>();
   
    Ptr<LoraChannel> channel = CreateObject<LoraChannel>(loss, delay);
    Ptr<LogicalLoraChannelHelper> channelHelper = CreateObject<LogicalLoraChannelHelper>(); 

    Ptr<my_device_with_lora> mydevice= CreateObject<my_device_with_lora>();
    Ptr<my_device_with_lora> mydevice2= CreateObject<my_device_with_lora>();

    Ptr<Node> nodo= CreateObject<Node>();
    Ptr<Node> nodo2= CreateObject<Node>();

    LoraPhyHelper phyHelper = LoraPhyHelper();
    phyHelper.SetChannel(channel);
    phyHelper.SetDeviceType(LoraPhyHelper::ED);
    

    mydevice->SetPhy(phyHelper.Create(nodo, mydevice), channelHelper);
    mydevice2->SetPhy(phyHelper.Create(nodo2, mydevice2), channelHelper);
    
    Simulator::Schedule(Seconds(2), sendPacket , mydevice);
    

    /****************
     *  Simulation  *
     ****************/

    Simulator::Stop(Hours(2));

    Simulator::Run();

    Simulator::Destroy();
    std::cout << "ended"<< std::endl;
    return 0;
}
