#include "myClass.h"
using namespace std;

namespace ns3 {
    namespace lorawan {

        myNetDevice::myNetDevice():
            m_node(nullptr), m_phy(nullptr) {}

        myNetDevice::~myNetDevice(){}

        void myNetDevice::setPhy(Ptr<LoraPhy> phy){
            m_phy = phy;
        }
        void myNetDevice::setNode(Ptr<Node> node){
            m_node = node;
        }

        Ptr<LoraPhy> myNetDevice::getPhy(){
            return m_phy;
        }
        Ptr<Node> myNetDevice::getNode(){
            return m_node;
        }

        void myNetDevice::send(Ptr<Packet> packet){
            cout << "sending packet" << endl;
        }

        void myNetDevice::Receive(Ptr<Packet> packet){
            cout << "receiving packet: " << packet << endl;
        }
    }
}