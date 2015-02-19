//
//  DNSConstants.h
//  DNSResolver
//
//  Created by Aaron Hartman on 21 Nov 14.
//  Copyright (c) 2014 Aaron Hartman. All rights reserved.
//

#ifndef DNSCONSTANTS_H
#define DNSCONSTANTS_H


#include <cstdint>

///
/// Name space to define constants used throughout the DNS Packet class and Resource types
///
namespace DNSConstants
{


	// Record Types

	const unsigned short ANSWER_RECORD = 2000;

	const unsigned short NS_RECORD = 3000;

	const unsigned short ADDITIONAL_RECORD = 4000;


	/**
	* TYPES
	*/
	const uint16_t A_TYPE = 0x0001;

	const uint16_t NS_TYPE = 0x0002;

	const uint16_t MD_TYPE = 0x0003;

	const uint16_t MF_TYPE = 0x0004;

	const uint16_t CNAME_TYPE = 0x0005;

	const uint16_t SOA_TYPE = 0x0006;

	const uint16_t MB_TYPE = 0x0007;

	const uint16_t MG_TYPE = 0x0008;

	const uint16_t MR_TYPE = 0x0009;

	const uint16_t NULL_TYPE = 0x000A;

	const uint16_t WKS_TYPE = 0x000B;

	const uint16_t PTR_TYPE = 0x000C;

	const uint16_t HINFO_TYPE = 0x000D;

	const uint16_t MINFO_TYPE = 0x000E;

	const uint16_t MX_TYPE = 0x000F;

	const uint16_t TXT_TYPE = 0x001F;

	const uint16_t AAAA_TYPE = 0x001C;

	const uint16_t DNSKEY_TYPE = 0x0030;

	const uint16_t RRSIG_TYPE = 0x002E;


	// Class types

	const uint16_t IN_CLASS = 0x0001;

	const uint16_t CS_CLASS = 0x0002;

	const uint16_t CH_CLASS = 0x0003;

	const uint16_t HS_CLASS = 0x0004;


	// RCODES

	const uint16_t OK_RCODE = 0x0000;

	const uint16_t FERROR_RCODE = 0x0001;

	const uint16_t SERVFAIL_RCODE = 0x0002;

	const uint16_t NAMERR_RCODE = 0x0003;

	const uint16_t NI_RCODE = 0x0004;

	const uint16_t NX_RCODE = 0x0005;


	// ALGORITHM TYPES
	const uint8_t DSA_ALG = 0X03;        //   DSA/SHA-1 [DSA]          y      [RFC2536]  OPTIONAL
	const uint8_t RSA_ALG = 0x05;        //   RSA/SHA-1 [RSASHA1]      y      [RFC3110]  MANDATORY

	
	// ROOT SERVER INFORMATION
	#define A_ROOT "198.41.0.4"
	#define B_ROOT "192.228.79.201"
	#define C_ROOT "192.33.4.12"
	#define D_ROOT "199.7.91.13"
	#define E_ROOT "192.203.230.10"
	#define F_ROOT "192.5.5.241"
	#define G_ROOT "192.112.36.4"
	#define H_ROOT "128.63.2.53"
	#define I_ROOT "192.36.148.17"
	#define J_ROOT "192.58.128.30"
	#define K_ROOT "193.0.14.129"
	#define L_ROOT "199.7.83.42"
	#define M_ROOT "202.12.27.33"

	#define DNS_PORT "53"

}

#endif
