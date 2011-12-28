

#include "NetAdr.h"

#include <cstring>
#include <cassert>
#include "platform.h"
#include "net.h"

NetAdr::NetAdr()
{
	ChangeTo();
}

NetAdr::NetAdr(unsigned long ipv4_address, unsigned short port)
{
	ChangeTo(ipv4_address, port);
}

NetAdr::NetAdr(unsigned char ipv6_address[16], unsigned short port)
{
	ChangeTo(ipv6_address, port);
}

void NetAdr::ChangeTo()
{
	m_type = ADR_BAD;
}

void NetAdr::ChangeTo(unsigned long ipv4_address, unsigned short port)
{
	m_type = ADR_IP;
	m_addr.ip = ipv4_address;
	m_port = port;
}

void NetAdr::ChangeTo(unsigned char ipv6_address[16], unsigned short port)
{
	m_type = ADR_IP6;
	memcpy(m_addr.ip6, ipv6_address, sizeof(m_addr.ip6));
	m_port = port;
}

NetAdrType NetAdr::GetType()
{
	return m_type;
}

unsigned long NetAdr::GetIPv4Address()
{
	if (m_type != ADR_IP)
	{
		assert(m_type == ADR_IP);
		return 0;
	}

	return m_addr.ip;
}

unsigned char const *NetAdr::GetIPv6Address()
{
	if (m_type != ADR_IP6)
	{
		assert(m_type == ADR_IP6);
		return NULL;
	}

	return m_addr.ip6;
}

unsigned short NetAdr::GetPort()
{
	return m_port;
}

bool NetAdr::CompareWith(NetAdr *other)
{
	// The two types must match
	if ( m_type != other->GetType() )
	{
		return false;
	}

	switch ( m_type )
	{
		case ADR_IP:
			// The IP address and the port must match
			if ( m_addr.ip == other->GetIPv4Address() && m_port == other->GetPort() )
			{
				return true;
			}

			return false;

		case ADR_IP6:
			// The IPv6 address and the port must match
			if ( memcmp(m_addr.ip6, other->GetIPv6Address(), sizeof(m_addr.ip6)) == 0 && m_port == other->GetPort() )
			{
				return true;
			}

			return false;

		default:
			return false;
	}
}

void NetAdr::ToString(char *buffer, int length)
{
	unsigned long n_ip; // IPv4 address in network byte order

	switch (m_type)
	{
		case ADR_IP:
			n_ip = htonl(m_addr.ip);
			inet_ntop(AF_INET, &n_ip, buffer, length);
			return;

		case ADR_IP6:
			inet_ntop(AF_INET6, m_addr.ip6, buffer, length);
			return;

		default:
			Error("NetAdr::ToString: Invalid NetAdrType!\n");
			return;
	}
}

void NetAdr::ToSockaddr(struct sockaddr_storage *saddr)
{
	struct sockaddr_in *saddr_in;
	struct sockaddr_in6 *saddr_in6;

	// Clear out the sockaddr structure first
	memset(saddr, 0, sizeof(*saddr));

	switch (m_type)
	{
		case ADR_IP:
			saddr_in = (struct sockaddr_in*)saddr;
			saddr_in->sin_family = AF_INET;
			saddr_in->sin_port = htons(m_port);
			saddr_in->sin_addr.s_addr = htonl(m_addr.ip);
			return;

		case ADR_IP6:
			saddr_in6 = (struct sockaddr_in6*)saddr;
			saddr_in6->sin6_family = AF_INET6;
			saddr_in6->sin6_port = htons(m_port);
			memcpy(saddr_in6->sin6_addr.s6_addr, m_addr.ip6, sizeof(saddr_in6->sin6_addr.s6_addr));
			return;

		default:
			Error("NetAdr::ToSockaddr: Invalid NetAdrType!\n");
			return;
	}
}