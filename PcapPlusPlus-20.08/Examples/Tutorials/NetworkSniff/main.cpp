#include "stdlib.h"
#include "PcapLiveDeviceList.h"
#include "PlatformSpecificUtils.h"

#include <iostream> 
#include "Packet.h"
#include "EthLayer.h"
#include "IPv4Layer.h"
#include "IPv6Layer.h"
#include "TcpLayer.h"
#include "UdpLayer.h"
#include "HttpLayer.h"
#include "Layer.h"
#include "SSLLayer.h"
#include "HttpLayer.h"
#include "DnsLayer.h"
#include <WinPcapLiveDevice.h>

using namespace std;
/**
++++++++++++++PAKET YAKALAMA STRUCTURE++++++++++++
ÖZLELLİKLERİ
++SRC PORT- DONE
++SRC IP / JUST SRC- DONE
++DST PORT- DONE
++DST IP - DONE
++TİME 
++NUMBER THESE PACKETS SO THEY ARE ORDERED AS THEY COME - DONE
++PROTOCOL - DONE
++LENGTH 
++İNFO - EH ISTE
FONKSYONLARI
--RAW paket alıp bunu hallerine böldüren bir fonksiyonu olması lazım.
--paket print etme
--structure delete edebilmesi lazim ki print ettikten sonra yer kaplamasın.
 */

// this fucntion will allow us to print types of  tcp packets we  use

std::string getProtocolTypeAsString(pcpp::ProtocolType protocolType)
{		/*
		++++++++++++Protocol tipleri+++++++++
				ETHERNET
				IPv4/v6
				TCP
				UDP 
				HTTP
				DNS
				VLAN	
				SSL
		*/

	switch (protocolType)
	{
	case pcpp::Ethernet:
		return "Protocol: Ethernet";
	case pcpp::IPv4:
		return "Protocol: IPv4";
	case pcpp::IPv6:
		return "Protocol: IPv6";
	case pcpp::TCP:
		return "Protocol: TCP";
	case pcpp::UDP:
		return "Protocol: UDP";
	case pcpp::HTTPRequest:
	case pcpp::HTTPResponse:
		return "Protocol: HTTP";
	case pcpp::DNS:
		return "Protocol: DNS";
	case pcpp::SSL:
		return "Protocol: SSL";
	case pcpp::VLAN:
		return "Protocol: VLAN";
	default:
		return "Unknown";
	}// this returns strings of wanted protocols.

}
std::string printHttpMethod(pcpp::HttpRequestLayer::HttpMethod httpMethod)
{
	switch (httpMethod)
	{
	case pcpp::HttpRequestLayer::HttpGET:
		return "GET";
	case pcpp::HttpRequestLayer::HttpPOST:
		return "POST";
	default:
		return "Other";
	}
}

std::string printTcpFlags(pcpp::TcpLayer* tcpLayer)
{
	std::string result = "";
	if (tcpLayer->getTcpHeader()->synFlag == 1)
		result += "SYN ";
	if (tcpLayer->getTcpHeader()->ackFlag == 1)
		result += "ACK ";
	if (tcpLayer->getTcpHeader()->pshFlag == 1)
		result += "PSH ";
	if (tcpLayer->getTcpHeader()->cwrFlag == 1)
		result += "CWR ";
	if (tcpLayer->getTcpHeader()->urgFlag == 1)
		result += "URG ";
	if (tcpLayer->getTcpHeader()->eceFlag == 1)
		result += "ECE ";
	if (tcpLayer->getTcpHeader()->rstFlag == 1)
		result += "RST ";
	if (tcpLayer->getTcpHeader()->finFlag == 1)
		result += "FIN ";

	return result;
}


struct PacketInfo {
	//string srcPort;
	//string dstPort;
	//string srcIPString;
	//string dstIPString;
	//string timeTaken;
	int NoOfPacket;
	std::string protocol = "null";
	std::string srcMac = "null";
	std::string dstMac = "null";
	std::string srcIP = "null";
	std::string dstIP = "null";
	int TTL;
	int srcPort;
	int dstPort;
	int WindowSize;
	//string TCPFlag;



	//some how gotta pull out chain of layers. Maybe just print them? We aren't exactly creating wireshark all over again.
	//string packetLength;
	//info is too hard right now lets focus on rest of the things right now.
	// This doesn't actually consume the packet just prints it out.
	void consumePacket(pcpp::Packet& packet) {
		//we take parsed packets 
		/*
		++++++++++++Protocol tipleri+++++++++
				ETHERNET
				IPv4/v6
				TCP
				UDP
				HTTP
				DNS
				VLAN
				SSH
		*/
		// first let's go over the layers one by one and find out its type, its total length, its header length and its payload length
		printf("+++++++Started consuming a new packet. It's Packet No is: %d +++++++++++\n", NoOfPacket++);
		
		if (packet.isPacketOfType(pcpp::Ethernet))
			protocol = "Ethernnet";
		if (packet.isPacketOfType(pcpp::IPv4))
			protocol= "IPv4";
		if (packet.isPacketOfType(pcpp::IPv6))
			protocol= "IPv6";
		if (packet.isPacketOfType(pcpp::TCP))
			protocol= "TCP";
		if (packet.isPacketOfType(pcpp::UDP))
			protocol= "UDP";
		if (packet.isPacketOfType(pcpp::HTTP))
			protocol= "HTTP";
		if (packet.isPacketOfType(pcpp::SSL))
			protocol= "SSL";
		
		for (pcpp::Layer* curLayer = packet.getFirstLayer(); curLayer != NULL; curLayer = curLayer->getNextLayer())
		{
			printf("Layer type: %s; Total data: %d [bytes]; Layer data: %d [bytes]; Layer payload: %d [bytes]\n",
				getProtocolTypeAsString(curLayer->getProtocol()).c_str(), // get layer type
				(int)curLayer->getDataLen(),                              // get total length of the layer
				(int)curLayer->getHeaderLen(),                            // get the header length of the layer
				(int)curLayer->getLayerPayloadSize());                    // get the payload length of the layer (equals total length minus header length)
		}

		// now we got the basic stats of the packet. Now we gotta peel the packet back layer by layer. B
		// But most probably not all protocols or layers are present so we create if statements.
		//lets start with ethernet protocol
		pcpp::EthLayer* ethernetLayer = packet.getLayerOfType<pcpp::EthLayer>();
		if (ethernetLayer != NULL) {
			srcMac = ethernetLayer->getSourceMac().toString();
			dstMac = ethernetLayer->getDestMac().toString();
		}
		//IPv4 Layer 
		//a packets either has ipv4 or ipv6 not boths so we don't see a problem creating if funcs for both of them.
		//couldn't do universal for them.
		pcpp::IPv4Layer* ipvFourLayer = packet.getLayerOfType<pcpp::IPv4Layer>();
		if (ipvFourLayer != NULL)
		{
			srcIP = ipvFourLayer->getSrcIpAddress().toString();
			dstIP = ipvFourLayer->getDstIpAddress().toString();
			TTL = ipvFourLayer->getIPv4Header()->timeToLive;

		}
		pcpp::IPv6Layer* ipvSixLayer = packet.getLayerOfType<pcpp::IPv6Layer>();
		if (ipvSixLayer != NULL)
		{
			srcIP = ipvSixLayer->getSrcIpAddress().toString();
			dstIP = ipvSixLayer->getDstIpAddress().toString();
			//printf("Sorry. IPv6 for PcapPlusPlus has no Time to live function.\n");

		}

		pcpp::TcpLayer* tcpLayer = packet.getLayerOfType<pcpp::TcpLayer>();
		if (tcpLayer != NULL)
		{
		
				srcPort = (int)ntohs(tcpLayer->getTcpHeader()->portSrc);
				dstPort = (int)ntohs(tcpLayer->getTcpHeader()->portDst);
				WindowSize = (int)ntohs(tcpLayer->getTcpHeader()->windowSize);
				//TCPFlag = printTcpFlags(tcpLayer).c_str();
			
			
		}
		pcpp::UdpLayer* udpLayer = packet.getLayerOfType<pcpp::UdpLayer>();
		if (udpLayer != NULL) {
			
			srcPort = (int)ntohs(udpLayer->getUdpHeader()->portSrc);
			dstPort = (int)ntohs(udpLayer->getUdpHeader()->portDst);
			
		}
		printToConsole();


		/*
		* we commented out this part cause couldn't get any usefull data to dns.
		* 
		pcpp::HttpRequestLayer* httpRequestLayer = packet.getLayerOfType<pcpp::HttpRequestLayer>();
		if (httpRequestLayer != NULL)
		{
			// print HTTP method and URI. Both appear in the first line of the HTTP request
			printf("\nHTTP method: %s\n", printHttpMethod(httpRequestLayer->getFirstLine()->getMethod()).c_str());
			printf("HTTP URI: %s\n", httpRequestLayer->getFirstLine()->getUri().c_str());

			// print values of the following HTTP field: Host, User-Agent and Cookie
			printf("HTTP host: %s\n", httpRequestLayer->getFieldByName(PCPP_HTTP_HOST_FIELD)->getFieldValue().c_str());
			printf("HTTP user-agent: %s\n", httpRequestLayer->getFieldByName(PCPP_HTTP_USER_AGENT_FIELD)->getFieldValue().c_str());
			printf("HTTP cookie: %s\n", httpRequestLayer->getFieldByName(PCPP_HTTP_COOKIE_FIELD)->getFieldValue().c_str());

			// print the full URL of this request
			printf("HTTP full URL: %s\n", httpRequestLayer->getUrl().c_str());
		}
		pcpp::DnsLayer* dnsLayer = packet.getLayerOfType<pcpp::DnsLayer>();
		if (dnsLayer != NULL) {
			printf(" \n",
				dnsLayer->getData()
				
				);
		}
		
		
		*/
		
	}

	void printToConsole()
	{
		
		printf("Packet No: %d | srcIP: %s | dstIP: %s| srcPort: %d | dstPort: %d | Protocol: %s| WindowSize:  %d | srcMac %s | dstMac %s\n",
			NoOfPacket,
			srcIP.c_str(),
			dstIP.c_str(),
			srcPort,
			dstPort,
			protocol.c_str(),
			WindowSize,
			srcMac.c_str(),
			dstMac.c_str());
		
	}

	void clear() {
		NoOfPacket = 0;
	}
	PacketInfo() {
		clear();
	}

};
struct PacketStats
{
	int ethPacketCount;
	int ipv4PacketCount;
	int ipv6PacketCount;
	int tcpPacketCount;
	int udpPacketCount;
	int dnsPacketCount;
	int httpPacketCount;
	int sslPacketCount;


	/**
	 * Clear all stats
	 */
	void clear() { ethPacketCount = 0; ipv4PacketCount = 0; ipv6PacketCount = 0; tcpPacketCount = 0; udpPacketCount = 0; tcpPacketCount = 0; dnsPacketCount = 0; httpPacketCount = 0; sslPacketCount = 0; }

	/**
	 * C'tor
	 */
	PacketStats() { clear(); }

	/**
	 * Collect stats from a packet
	 */
	void consumePacket(pcpp::Packet& packet)
	{
		if (packet.isPacketOfType(pcpp::Ethernet))
			ethPacketCount++;
		if (packet.isPacketOfType(pcpp::IPv4))
			ipv4PacketCount++;
		if (packet.isPacketOfType(pcpp::IPv6))
			ipv6PacketCount++;
		if (packet.isPacketOfType(pcpp::TCP))
			tcpPacketCount++;
		if (packet.isPacketOfType(pcpp::UDP))
			udpPacketCount++;
		if (packet.isPacketOfType(pcpp::HTTP))
			httpPacketCount++;
		if (packet.isPacketOfType(pcpp::SSL))
			sslPacketCount++;
	}

	/**
	 * Print stats to console
	 */
	void printToConsole()
	{
		printf("Ethernet packet count: %d\n", ethPacketCount);
		printf("IPv4 packet count:     %d\n", ipv4PacketCount);
		printf("IPv6 packet count:     %d\n", ipv6PacketCount);
		printf("TCP packet count:      %d\n", tcpPacketCount);
		printf("UDP packet count:      %d\n", udpPacketCount);
		printf("DNS packet count:      %d\n", dnsPacketCount);
		printf("HTTP packet count:     %d\n", httpPacketCount);
		printf("SSL packet count:      %d\n", sslPacketCount);
	}
};


/**
 paketleri geldiği gibi karşılayan fonksyon


 */
static void onPacketArrives(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* dev, void* cookie)
{
	// extract the stats object form the cookie
	//right now it is empty so points to void packetinfo
	PacketInfo* stats = (PacketInfo*)cookie;

	// parsed the raw packet
	pcpp::Packet parsedPacket(packet);

	// collect stats from packet
	stats->consumePacket(parsedPacket);
}


///**
// * a callback function for the blocking mode capture which is called each time a packet is captured
// */
//static bool onPacketArrivesBlockingMode(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* dev, void* cookie)
//{
//	// extract the stats object form the cookie
//	PacketInfo* stats = (PacketInfo*)cookie;
//
//	// parsed the raw packet
//	pcpp::Packet parsedPacket(packet);
//
//	// collect stats from packet
//	stats->consumePacket(parsedPacket);
//
//	// return false means we don't want to stop capturing after this callback
//	return false;
//}


int main(int argc, char* argv[])
{
	// IPv4 address of the interface we want to sniff
	// A CODE TO FIND INTERFACE IP ADDR
	// we will want IP tru string input

	std::string interfaceIPAddr; // = "n";
	std::cout << "Please enter your Network iterface Ip address (Something something. XXX.XXX.XXX.XXX):";
	std::cin >> interfaceIPAddr;
	std::cout << endl;
	std::cout << "Your IP is: " << interfaceIPAddr << endl;
	//we confirm the ip address.


	// find the interface by IP address
	pcpp::PcapLiveDevice* dev = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByIp(/*"192.168.1.58"*/interfaceIPAddr.c_str());
	if (dev == NULL)
	{
		printf("Cannot find interface with IPv4 address of '%s'\n", interfaceIPAddr.c_str());
		exit(1);
	}

	// Get device info
	// ~~~~~~~~~~~~~~~

	// before capturing packets let's print some info about this interface
	printf("Interface info:\n");
	// get interface name
	printf("   Interface name:        %s\n", dev->getName());
	// get interface description
	printf("   Interface description: %s\n", dev->getDesc());
	// get interface MAC address
	printf("   MAC address:           %s\n", dev->getMacAddress().toString().c_str());
	// get default gateway for interface
	printf("   Default gateway:       %s\n", dev->getDefaultGateway().toString().c_str());
	// get interface MTU
	printf("   Interface MTU:         %d\n", dev->getMtu());
	// get DNS server if defined for this interface
	if (dev->getDnsServers().size() > 0)
		printf("   DNS server:            %s\n", dev->getDnsServers().at(0).toString().c_str());

	// open the device before start capturing/sending packets
	if (!dev->open())
	{
		printf("Cannot open device\n");
		exit(1);
	}

	// create the stats object
	PacketInfo stats;


	// Async packet capture with a callback function
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	printf("\nEntering capture loop\n");
	bool fackVar = true;
	while (fackVar) {
		printf("\nNew Capture Loop.\n");
		printf("\nHow long do you want to capture:");
		int time;
		std::cin >> time;

		printf("\nDo you want  to apply any filters? y/n:");
		string input1, inputFilter, inputVaiable1;
		std::cin >> input1;

		// create an AND filter to combine both filters - capture only TCP traffic on port 80
		pcpp::AndFilter andFilter;
		while (true) {

			if (input1 == "y") {
				printf("\n What filter do you want? IP/port:");
				std::cin >> inputFilter;
				if (inputFilter == "IP") {
					printf("\nWhat IP? (Something something. XXX.XXX.XXX.XXX):");
					std::cin >> inputVaiable1;

					pcpp::IPFilter ipFilter(inputVaiable1, pcpp::SRC_OR_DST);
					andFilter.addFilter(&ipFilter);
					// set the filter on the device
					dev->setFilter(andFilter);

					printf("\nStarting packet capture with a filter in place...\n");

					printf("\ndo you want to add more filters? y/n:");
					std::cin >> inputVaiable1;
					if (inputVaiable1 != "y") {
						break;
					}
				}
				else if (inputFilter == "port") {
					printf("\nWhat port? :");
					int x;
					std::cin >> x;

					pcpp::PortFilter portFilter(x, pcpp::SRC_OR_DST);
					andFilter.addFilter(&portFilter);
					// set the filter on the device
					dev->setFilter(andFilter);

					printf("\nStarting packet capture with a filter in place...\n");

					printf("\ndo you want to add more filters? y/n:");
					std::cin >> inputVaiable1;
					if (inputVaiable1 != "y") {
						break;
					}
				}
				else {
					break;
				}
			}
			else if (input1 == "n") {
				printf("\nYou don't want filters. Continuing without filters.\n");
				break;
			}
			else {
				printf("\nYou didn't put correct input. Continuing without filters.\n");
				break;
			}

		}
		printf("\n ..   \n");
		
		// start capture in async mode. Give a callback function to call to whenever a packet is captured and the stats object as the cookie
		dev->startCapture(onPacketArrives, &stats);

		PCAP_SLEEP(time);

		// stop capturing packets
		dev->stopCapture();

		// print results - should capture only packets which match the filter (which is TCP port 80)
		printf("Done Capturing! Do you want to do it again? y/n:\n");
		string lastQ;
		if(lastQ != "y"){
			fackVar = false;
		}
	
	}

}
