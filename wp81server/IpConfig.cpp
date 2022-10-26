#include "pch.h"
#include <winsock2.h>
#include "Win32Api.h"
#include <stdio.h>
#include <stdlib.h>

#define WORKING_BUFFER_SIZE 15000
#define MAX_TRIES 3

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

void debug2(WCHAR* format, ...)
{
	va_list args;
	va_start(args, format);

	WCHAR buffer[1000];
	_vsnwprintf_s(buffer, sizeof(buffer), format, args);

	OutputDebugStringW(buffer);

	va_end(args);
}

void printIpConfig()
{

	Win32Api win32Api;

	/* Declare and initialize variables */

	DWORD dwSize = 0;
	DWORD dwRetVal = 0;

	unsigned int i = 0;

	// Set the flags to pass to GetAdaptersAddresses
	ULONG flags = GAA_FLAG_INCLUDE_PREFIX;

	// default to unspecified address family (both)
	ULONG family = AF_UNSPEC;

	LPVOID lpMsgBuf = NULL;

	PIP_ADAPTER_ADDRESSES pAddresses = NULL;
	ULONG outBufLen = 0;
	ULONG Iterations = 0;

	PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;
	PIP_ADAPTER_UNICAST_ADDRESS pUnicast = NULL;
	PIP_ADAPTER_ANYCAST_ADDRESS pAnycast = NULL;
	PIP_ADAPTER_MULTICAST_ADDRESS pMulticast = NULL;
	IP_ADAPTER_DNS_SERVER_ADDRESS *pDnServer = NULL;
	IP_ADAPTER_PREFIX *pPrefix = NULL;

	family = AF_INET;

	// Allocate a 15 KB buffer to start with.
	outBufLen = WORKING_BUFFER_SIZE;

	do {

		pAddresses = (IP_ADAPTER_ADDRESSES *)MALLOC(outBufLen);
		if (pAddresses == NULL) {
			debug2(L"Memory allocation failed for IP_ADAPTER_ADDRESSES struct\n");
			return;
		}

		dwRetVal =
			win32Api.GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);

		if (dwRetVal == ERROR_BUFFER_OVERFLOW) {
			FREE(pAddresses);
			pAddresses = NULL;
		}
		else {
			break;
		}

		Iterations++;

	} while ((dwRetVal == ERROR_BUFFER_OVERFLOW) && (Iterations < MAX_TRIES));

	if (dwRetVal == NO_ERROR) {
		// If successful, output some information from the data we received
		pCurrAddresses = pAddresses;
		while (pCurrAddresses) {
			debug2(L"\tLength of the IP_ADAPTER_ADDRESS struct: %ld\n",
				pCurrAddresses->Length);
			debug2(L"\tIfIndex (IPv4 interface): %u\n", pCurrAddresses->IfIndex);
			debug2(L"\tAdapter name: %hs\n", pCurrAddresses->AdapterName);

			pUnicast = pCurrAddresses->FirstUnicastAddress;
			if (pUnicast != NULL) {
				for (i = 0; pUnicast != NULL; i++)
				{
					auto family = pUnicast->Address.lpSockaddr->sa_family;
					if (AF_INET == family)
					{
						// IPv4
						SOCKADDR_IN* ipv4 = reinterpret_cast<SOCKADDR_IN*>(pUnicast->Address.lpSockaddr);

						char str_buffer[INET_ADDRSTRLEN] = { 0 };
						win32Api.inet_ntop(AF_INET, &(ipv4->sin_addr), str_buffer, INET_ADDRSTRLEN);
						debug2(L"\tIP address: %hs\n",str_buffer);
					}

					pUnicast = pUnicast->Next;
				}
					
				debug2(L"\tNumber of Unicast Addresses: %d\n", i);
			}
			else
				debug2(L"\tNo Unicast Addresses\n");

			pAnycast = pCurrAddresses->FirstAnycastAddress;
			if (pAnycast) {
				for (i = 0; pAnycast != NULL; i++)
					pAnycast = pAnycast->Next;
				debug2(L"\tNumber of Anycast Addresses: %d\n", i);
			}
			else
				debug2(L"\tNo Anycast Addresses\n");

			pMulticast = pCurrAddresses->FirstMulticastAddress;
			if (pMulticast) {
				for (i = 0; pMulticast != NULL; i++)
					pMulticast = pMulticast->Next;
				debug2(L"\tNumber of Multicast Addresses: %d\n", i);
			}
			else
				debug2(L"\tNo Multicast Addresses\n");

			pDnServer = pCurrAddresses->FirstDnsServerAddress;
			if (pDnServer) {
				for (i = 0; pDnServer != NULL; i++)
					pDnServer = pDnServer->Next;
				debug2(L"\tNumber of DNS Server Addresses: %d\n", i);
			}
			else
				debug2(L"\tNo DNS Server Addresses\n");

			debug2(L"\tDNS Suffix: %wS\n", pCurrAddresses->DnsSuffix);
			debug2(L"\tDescription: %wS\n", pCurrAddresses->Description);
			debug2(L"\tFriendly name: %wS\n", pCurrAddresses->FriendlyName);

			if (pCurrAddresses->PhysicalAddressLength != 0) {
				debug2(L"\tPhysical address: ");
				for (i = 0; i < (int)pCurrAddresses->PhysicalAddressLength;
					i++) {
					if (i == (pCurrAddresses->PhysicalAddressLength - 1))
						debug2(L"%.2X\n",
						(int)pCurrAddresses->PhysicalAddress[i]);
					else
						debug2(L"%.2X-",
						(int)pCurrAddresses->PhysicalAddress[i]);
				}
			}
			debug2(L"\tFlags: %ld\n", pCurrAddresses->Flags);
			debug2(L"\tMtu: %lu bytes\n", pCurrAddresses->Mtu);
			debug2(L"\tIfType: "); 
			switch (pCurrAddresses->IfType)
			{
			case 1:
				debug2(L"IF_TYPE_OTHER");
				break;
			case 6:
				debug2(L"IF_TYPE_ETHERNET_CSMACD");
				break;
			case 9:
				debug2(L"IF_TYPE_ISO88025_TOKENRING");
				break;
			case 23:
				debug2(L"IF_TYPE_PPP");
				break;
			case 24:
				debug2(L"IF_TYPE_SOFTWARE_LOOPBACK");
				break;
			case 37:
				debug2(L"IF_TYPE_ATM");
				break;
			case 71:
				debug2(L"IF_TYPE_IEEE80211");
				break;
			case 131:
				debug2(L"IF_TYPE_TUNNEL");
				break;
			case 144:
				debug2(L"IF_TYPE_IEEE1394");
				break;
			}
			debug2(L" (%ld)\n", pCurrAddresses->IfType);
			debug2(L"\tOperStatus: "); 
			switch (pCurrAddresses->OperStatus)
			{
			case 1:
				debug2(L"IfOperStatusUp");
				break;
			case 2:
				debug2(L"IfOperStatusDown");
				break;
			case 3:
				debug2(L"IfOperStatusTesting");
				break;
			case 4:
				debug2(L"IfOperStatusUnknown");
				break;
			case 5:
				debug2(L"IfOperStatusDormant");
				break;
			case 6:
				debug2(L"IfOperStatusNotPresent");
				break;
			case 7:
				debug2(L"IfOperStatusLowerLayerDown");
				break;
			}
			debug2(L" (%ld)\n", pCurrAddresses->OperStatus);
			debug2(L"\tIpv6IfIndex (IPv6 interface): %u\n",
				pCurrAddresses->Ipv6IfIndex);
			debug2(L"\tZoneIndices (hex): ");
			for (i = 0; i < 16; i++)
				debug2(L"%lx ", pCurrAddresses->ZoneIndices[i]);
			debug2(L"\n");

			debug2(L"\tTransmit link speed: %I64u bits per second\n", pCurrAddresses->TransmitLinkSpeed);
			debug2(L"\tReceive link speed: %I64u bits per second\n", pCurrAddresses->ReceiveLinkSpeed);

			pPrefix = pCurrAddresses->FirstPrefix;
			if (pPrefix) {
				for (i = 0; pPrefix != NULL; i++)
					pPrefix = pPrefix->Next;
				debug2(L"\tNumber of IP Adapter Prefix entries: %d\n", i);
			}
			else
				debug2(L"\tNumber of IP Adapter Prefix entries: 0\n");

			debug2(L"\n");

			pCurrAddresses = pCurrAddresses->Next;
		}
	}
	else {
		debug2(L"Call to GetAdaptersAddresses failed with error: %d\n",
			dwRetVal);
		if (dwRetVal == ERROR_NO_DATA)
			debug2(L"\tNo addresses were found for the requested parameters\n");
		else {
			debug2(L"\tError: %d", dwRetVal);
			if (pAddresses)
				FREE(pAddresses);
			return;
		}
	}

	if (pAddresses) {
		FREE(pAddresses);
	}

	return;
}