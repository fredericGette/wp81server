#pragma once

//
// Flags used as argument to GetAdaptersAddresses().
// "SKIP" flags are added when the default is to include the information.
// "INCLUDE" flags are added when the default is to skip the information.
//
#define GAA_FLAG_SKIP_UNICAST                   0x0001
#define GAA_FLAG_SKIP_ANYCAST                   0x0002
#define GAA_FLAG_SKIP_MULTICAST                 0x0004
#define GAA_FLAG_SKIP_DNS_SERVER                0x0008
#define GAA_FLAG_INCLUDE_PREFIX                 0x0010
#define GAA_FLAG_SKIP_FRIENDLY_NAME             0x0020
#define GAA_FLAG_INCLUDE_WINS_INFO              0x0040
#define GAA_FLAG_INCLUDE_GATEWAYS               0x0080
#define GAA_FLAG_INCLUDE_ALL_INTERFACES         0x0100
#define GAA_FLAG_INCLUDE_ALL_COMPARTMENTS       0x0200
#define GAA_FLAG_INCLUDE_TUNNEL_BINDINGORDER    0x0400

#define MAX_ADAPTER_ADDRESS_LENGTH      8   // arb.
#define MAX_DHCPV6_DUID_LENGTH          130 // RFC 3315.
#define MAX_DNS_SUFFIX_STRING_LENGTH    256

#define INET_ADDRSTRLEN  22

typedef ULONG NET_IFINDEX, *PNET_IFINDEX;       // Interface Index (ifIndex)
typedef NET_IFINDEX IF_INDEX, *PIF_INDEX;

typedef enum {
	//
	// These values are from iptypes.h.
	// They need to fit in a 4 bit field.
	//
	IpPrefixOriginOther = 0,
	IpPrefixOriginManual,
	IpPrefixOriginWellKnown,
	IpPrefixOriginDhcp,
	IpPrefixOriginRouterAdvertisement,
	IpPrefixOriginUnchanged = 1 << 4
} NL_PREFIX_ORIGIN;

typedef enum {
	//
	// TODO: Remove the Nlso* definitions.
	//
	NlsoOther = 0,
	NlsoManual,
	NlsoWellKnown,
	NlsoDhcp,
	NlsoLinkLayerAddress,
	NlsoRandom,

	//
	// These values are from in iptypes.h.
	// They need to fit in a 4 bit field.
	//
	IpSuffixOriginOther = 0,
	IpSuffixOriginManual,
	IpSuffixOriginWellKnown,
	IpSuffixOriginDhcp,
	IpSuffixOriginLinkLayerAddress,
	IpSuffixOriginRandom,
	IpSuffixOriginUnchanged = 1 << 4
} NL_SUFFIX_ORIGIN;

typedef enum {
	//
	// TODO: Remove the Nlds* definitions.
	//
	NldsInvalid,
	NldsTentative,
	NldsDuplicate,
	NldsDeprecated,
	NldsPreferred,

	//
	// These values are from in iptypes.h.
	//
	IpDadStateInvalid = 0,
	IpDadStateTentative,
	IpDadStateDuplicate,
	IpDadStateDeprecated,
	IpDadStatePreferred,
} NL_DAD_STATE;

typedef NL_PREFIX_ORIGIN IP_PREFIX_ORIGIN;
typedef NL_SUFFIX_ORIGIN IP_SUFFIX_ORIGIN;
typedef NL_DAD_STATE IP_DAD_STATE;

typedef struct _IP_ADAPTER_UNICAST_ADDRESS_LH {
	union {
		ULONGLONG Alignment;
		struct {
			ULONG Length;
			DWORD Flags;
		};
	};
	struct _IP_ADAPTER_UNICAST_ADDRESS_LH *Next;
	SOCKET_ADDRESS Address;

	IP_PREFIX_ORIGIN PrefixOrigin;
	IP_SUFFIX_ORIGIN SuffixOrigin;
	IP_DAD_STATE DadState;

	ULONG ValidLifetime;
	ULONG PreferredLifetime;
	ULONG LeaseLifetime;
	UINT8 OnLinkPrefixLength;
} IP_ADAPTER_UNICAST_ADDRESS_LH,
*PIP_ADAPTER_UNICAST_ADDRESS_LH;

typedef struct _IP_ADAPTER_ANYCAST_ADDRESS_XP {
	union {
		ULONGLONG Alignment;
		struct {
			ULONG Length;
			DWORD Flags;
		};
	};
	struct _IP_ADAPTER_ANYCAST_ADDRESS_XP *Next;
	SOCKET_ADDRESS Address;
} IP_ADAPTER_ANYCAST_ADDRESS_XP, *PIP_ADAPTER_ANYCAST_ADDRESS_XP;

typedef struct _IP_ADAPTER_MULTICAST_ADDRESS_XP {
	union {
		ULONGLONG Alignment;
		struct {
			ULONG Length;
			DWORD Flags;
		};
	};
	struct _IP_ADAPTER_MULTICAST_ADDRESS_XP *Next;
	SOCKET_ADDRESS Address;
} IP_ADAPTER_MULTICAST_ADDRESS_XP, *PIP_ADAPTER_MULTICAST_ADDRESS_XP;

typedef struct _IP_ADAPTER_DNS_SERVER_ADDRESS_XP {
	union {
		ULONGLONG Alignment;
		struct {
			ULONG Length;
			DWORD Reserved;
		};
	};
	struct _IP_ADAPTER_DNS_SERVER_ADDRESS_XP *Next;
	SOCKET_ADDRESS Address;
} IP_ADAPTER_DNS_SERVER_ADDRESS_XP, *PIP_ADAPTER_DNS_SERVER_ADDRESS_XP;

typedef ULONG IFTYPE;

typedef enum {
	IfOperStatusUp = 1,
	IfOperStatusDown,
	IfOperStatusTesting,
	IfOperStatusUnknown,
	IfOperStatusDormant,
	IfOperStatusNotPresent,
	IfOperStatusLowerLayerDown
} IF_OPER_STATUS;

typedef struct _IP_ADAPTER_PREFIX_XP {
	union {
		ULONGLONG Alignment;
		struct {
			ULONG Length;
			DWORD Flags;
		};
	};
	struct _IP_ADAPTER_PREFIX_XP *Next;
	SOCKET_ADDRESS Address;
	ULONG PrefixLength;
} IP_ADAPTER_PREFIX_XP, *PIP_ADAPTER_PREFIX_XP;

typedef struct _IP_ADAPTER_WINS_SERVER_ADDRESS_LH {
	union {
		ULONGLONG Alignment;
		struct {
			ULONG Length;
			DWORD Reserved;
		};
	};
	struct _IP_ADAPTER_WINS_SERVER_ADDRESS_LH *Next;
	SOCKET_ADDRESS Address;
} IP_ADAPTER_WINS_SERVER_ADDRESS_LH, *PIP_ADAPTER_WINS_SERVER_ADDRESS_LH;

typedef struct _IP_ADAPTER_GATEWAY_ADDRESS_LH {
	union {
		ULONGLONG Alignment;
		struct {
			ULONG Length;
			DWORD Reserved;
		};
	};
	struct _IP_ADAPTER_GATEWAY_ADDRESS_LH *Next;
	SOCKET_ADDRESS Address;
} IP_ADAPTER_GATEWAY_ADDRESS_LH, *PIP_ADAPTER_GATEWAY_ADDRESS_LH;

typedef union _NET_LUID_LH
{
	ULONG64     Value;
	struct
	{
		ULONG64     Reserved : 24;
		ULONG64     NetLuidIndex : 24;
		ULONG64     IfType : 16;                  // equal to IANA IF type
	}Info;
} NET_LUID_LH, *PNET_LUID_LH;

typedef NET_LUID_LH NET_LUID;
typedef NET_LUID IF_LUID, *PIF_LUID;
typedef UINT32 NET_IF_COMPARTMENT_ID, *PNET_IF_COMPARTMENT_ID;
typedef GUID NET_IF_NETWORK_GUID, *PNET_IF_NETWORK_GUID;

typedef enum _NET_IF_CONNECTION_TYPE
{
	NET_IF_CONNECTION_DEDICATED = 1,
	NET_IF_CONNECTION_PASSIVE = 2,
	NET_IF_CONNECTION_DEMAND = 3,
	NET_IF_CONNECTION_MAXIMUM = 4
} NET_IF_CONNECTION_TYPE, *PNET_IF_CONNECTION_TYPE;

typedef enum {
	TUNNEL_TYPE_NONE = 0,
	TUNNEL_TYPE_OTHER = 1,
	TUNNEL_TYPE_DIRECT = 2,
	TUNNEL_TYPE_6TO4 = 11,
	TUNNEL_TYPE_ISATAP = 13,
	TUNNEL_TYPE_TEREDO = 14,
	TUNNEL_TYPE_IPHTTPS = 15,
} TUNNEL_TYPE, *PTUNNEL_TYPE;

typedef struct _IP_ADAPTER_DNS_SUFFIX {
	struct _IP_ADAPTER_DNS_SUFFIX *Next;
	WCHAR String[MAX_DNS_SUFFIX_STRING_LENGTH];
}IP_ADAPTER_DNS_SUFFIX, *PIP_ADAPTER_DNS_SUFFIX;

typedef struct _IP_ADAPTER_ADDRESSES_LH {
	union {
		ULONGLONG Alignment;
		struct {
			ULONG Length;
			IF_INDEX IfIndex;
		};
	};
	struct _IP_ADAPTER_ADDRESSES_LH *Next;
	PCHAR AdapterName;
	PIP_ADAPTER_UNICAST_ADDRESS_LH FirstUnicastAddress;
	PIP_ADAPTER_ANYCAST_ADDRESS_XP FirstAnycastAddress;
	PIP_ADAPTER_MULTICAST_ADDRESS_XP FirstMulticastAddress;
	PIP_ADAPTER_DNS_SERVER_ADDRESS_XP FirstDnsServerAddress;
	PWCHAR DnsSuffix;
	PWCHAR Description;
	PWCHAR FriendlyName;
	BYTE PhysicalAddress[MAX_ADAPTER_ADDRESS_LENGTH];
	ULONG PhysicalAddressLength;
	union {
		ULONG Flags;
		struct {
			ULONG DdnsEnabled : 1;
			ULONG RegisterAdapterSuffix : 1;
			ULONG Dhcpv4Enabled : 1;
			ULONG ReceiveOnly : 1;
			ULONG NoMulticast : 1;
			ULONG Ipv6OtherStatefulConfig : 1;
			ULONG NetbiosOverTcpipEnabled : 1;
			ULONG Ipv4Enabled : 1;
			ULONG Ipv6Enabled : 1;
			ULONG Ipv6ManagedAddressConfigurationSupported : 1;
		};
	};
	ULONG Mtu;
	IFTYPE IfType;
	IF_OPER_STATUS OperStatus;
	IF_INDEX Ipv6IfIndex;
	ULONG ZoneIndices[16];
	PIP_ADAPTER_PREFIX_XP FirstPrefix;

	ULONG64 TransmitLinkSpeed;
	ULONG64 ReceiveLinkSpeed;
	PIP_ADAPTER_WINS_SERVER_ADDRESS_LH FirstWinsServerAddress;
	PIP_ADAPTER_GATEWAY_ADDRESS_LH FirstGatewayAddress;
	ULONG Ipv4Metric;
	ULONG Ipv6Metric;
	IF_LUID Luid;
	SOCKET_ADDRESS Dhcpv4Server;
	NET_IF_COMPARTMENT_ID CompartmentId;
	NET_IF_NETWORK_GUID NetworkGuid;
	NET_IF_CONNECTION_TYPE ConnectionType;
	TUNNEL_TYPE TunnelType;
	//
	// DHCP v6 Info.
	//
	SOCKET_ADDRESS Dhcpv6Server;
	BYTE Dhcpv6ClientDuid[MAX_DHCPV6_DUID_LENGTH];
	ULONG Dhcpv6ClientDuidLength;
	ULONG Dhcpv6Iaid;
	PIP_ADAPTER_DNS_SUFFIX FirstDnsSuffix;
} IP_ADAPTER_ADDRESSES_LH,
*PIP_ADAPTER_ADDRESSES_LH;

typedef  IP_ADAPTER_ADDRESSES_LH IP_ADAPTER_ADDRESSES;
typedef  IP_ADAPTER_ADDRESSES_LH *PIP_ADAPTER_ADDRESSES;

typedef  IP_ADAPTER_UNICAST_ADDRESS_LH *PIP_ADAPTER_UNICAST_ADDRESS;
typedef  IP_ADAPTER_ANYCAST_ADDRESS_XP *PIP_ADAPTER_ANYCAST_ADDRESS;

typedef IP_ADAPTER_MULTICAST_ADDRESS_XP *PIP_ADAPTER_MULTICAST_ADDRESS;

typedef IP_ADAPTER_DNS_SERVER_ADDRESS_XP IP_ADAPTER_DNS_SERVER_ADDRESS;

typedef IP_ADAPTER_PREFIX_XP IP_ADAPTER_PREFIX;

extern "C" {
	WINBASEAPI HMODULE WINAPI LoadLibraryExW(LPCWSTR lpLibFileName, HANDLE hFile, DWORD dwFlags);
	WINBASEAPI HMODULE WINAPI GetModuleHandleW(LPCWSTR lpModuleName);

	ULONG WINAPI GetAdaptersAddresses(ULONG Family, ULONG Flags, PVOID Reserved, PIP_ADAPTER_ADDRESSES AdapterAddresses, PULONG SizePointer);

	WINBASEAPI HLOCAL WINAPI LocalFree(HLOCAL hMem);

	PCSTR WSAAPI inet_ntop(INT Family,const VOID * pAddr,PSTR pStringBuf,size_t StringBufSize);
}

#define WIN32API_TOSTRING(x) #x

// Link exported function
#define WIN32API_INIT_PROC(Module, Name)  \
  Name(reinterpret_cast<decltype(&::Name)>( \
      ::GetProcAddress((Module), WIN32API_TOSTRING(Name))))

// Convenientmacro to declare function
#define WIN32API_DEFINE_PROC(Name) const decltype(&::Name) Name

class Win32Api {

private:
	// Returns a base address of KernelBase.dll
	static HMODULE GetKernelBase() {
		return GetBaseAddress(&::DisableThreadLibraryCalls);
	}

	// Returns a base address of the given address
	static HMODULE GetBaseAddress(const void *Address) {
		MEMORY_BASIC_INFORMATION mbi = {};
		if (!::VirtualQuery(Address, &mbi, sizeof(mbi))) {
			return nullptr;
		}
		const auto mz = *reinterpret_cast<WORD *>(mbi.AllocationBase);
		if (mz != IMAGE_DOS_SIGNATURE) {
			return nullptr;
		}
		return reinterpret_cast<HMODULE>(mbi.AllocationBase);
	}
public:
	const HMODULE m_Kernelbase;
	WIN32API_DEFINE_PROC(LoadLibraryExW);
	WIN32API_DEFINE_PROC(GetModuleHandleW);
	const HMODULE m_IPHLPAPI;
	WIN32API_DEFINE_PROC(GetAdaptersAddresses);
	const HMODULE m_Kernel32legacy;
	WIN32API_DEFINE_PROC(LocalFree);
	const HMODULE m_WS2_32;
	WIN32API_DEFINE_PROC(inet_ntop);

	Win32Api()
		: m_Kernelbase(GetKernelBase()),
		WIN32API_INIT_PROC(m_Kernelbase, LoadLibraryExW),
		WIN32API_INIT_PROC(m_Kernelbase, GetModuleHandleW),
		m_IPHLPAPI(LoadLibraryExW(L"IPHLPAPI.DLL", NULL, NULL)),
		WIN32API_INIT_PROC(m_IPHLPAPI, GetAdaptersAddresses),
		m_Kernel32legacy(LoadLibraryExW(L"KERNEL32LEGACY.dll", NULL, NULL)),
		WIN32API_INIT_PROC(m_Kernel32legacy, LocalFree),
		m_WS2_32(LoadLibraryExW(L"WS2_32.DLL", NULL, NULL)),
		WIN32API_INIT_PROC(m_WS2_32, inet_ntop)
	{};

};