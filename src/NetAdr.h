

#ifndef _NETADR_H
#define _NETADR_H

/**
 * Possible host address types.
 */
enum NetAdrType
{
	ADR_BAD,
	ADR_IP,
	ADR_IP6
};

/**
 * Structure to uniquely identify a remote/local host.
 *
 * @warning Please remember that UDP packets are totally insecure and can
 *          be faked to appear to originate from any address/port!
 */
class NetAdr
{
public:
	NetAdr();
	NetAdr(unsigned long ipv4_address, unsigned short port);
	NetAdr(unsigned char ipv6_address[16], unsigned short port);

	NetAdrType GetType();

	// These are identical to the constructors above
	void ChangeTo();
	void ChangeTo(unsigned long ipv4_address, unsigned short port);
	void ChangeTo(unsigned char ipv6_address[16], unsigned short port);

	unsigned long GetIPv4Address();
	unsigned char const *GetIPv6Address();
	unsigned short GetPort();

	bool CompareWith(NetAdr *other);
	void ToString(char *buffer, int length);
	void ToSockaddr(struct sockaddr_storage *saddr);

private:
	NetAdrType m_type;

	// addr and port are stored as HOST order here!
	union {
		unsigned long ip;
		unsigned char ip6[16];
	} m_addr;
	unsigned short m_port;
};

#endif // !_NETADR_H