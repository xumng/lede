/*===========================================================================
FILE:
   QMI.c

DESCRIPTION:
   Qualcomm QMI driver code
   
FUNCTIONS:
   Generic QMUX functions
      ParseQMUX
      FillQMUX
   
   Generic QMI functions
      GetTLV
      ValidQMIMessage
      GetQMIMessageID

   Fill Buffers with QMI requests
      QMICTLGetClientIDReq
      QMICTLReleaseClientIDReq
      QMICTLReadyReq
      QMIWDSSetEventReportReq
      QMIWDSGetPKGSRVCStatusReq
      QMIDMSGetMEIDReq
      
   Parse data from QMI responses
      QMICTLGetClientIDResp
      QMICTLReleaseClientIDResp
      QMIWDSEventResp
      QMIDMSGetMEIDResp

Copyright (c) 2011, Code Aurora Forum. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Code Aurora Forum nor
      the names of its contributors may be used to endorse or promote
      products derived from this software without specific prior written
      permission.


THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
===========================================================================*/

//---------------------------------------------------------------------------
// Include Files
//---------------------------------------------------------------------------
#include "QMI.h"
//#include "QMIDevice.h"
#include "Structs.h"
//#include <linux/kernel.h>
//#include <asm/unaligned.h>


/*=========================================================================*/
// Get sizes of buffers needed by QMI requests
/*=========================================================================*/

/*===========================================================================
METHOD:
   QMUXHeaderSize (Public Method)

DESCRIPTION:
   Get size of buffer needed for QMUX
 
RETURN VALUE:
   u16 - size of buffer
===========================================================================*/
u16 QMUXHeaderSize( void )
{
	return sizeof( sQMUX );
}

/*===========================================================================
METHOD:
   QMICTLGetClientIDReqSize (Public Method)

DESCRIPTION:
   Get size of buffer needed for QMUX + QMICTLGetClientIDReq
 
RETURN VALUE:
   u16 - size of buffer
===========================================================================*/
u16 QMICTLGetClientIDReqSize( void )
{
	return sizeof( sQMUX ) + 10;
}

/*===========================================================================
METHOD:
   QMICTLReleaseClientIDReqSize (Public Method)

DESCRIPTION:
   Get size of buffer needed for QMUX + QMICTLReleaseClientIDReq
 
RETURN VALUE:
   u16 - size of header
===========================================================================*/
u16 QMICTLReleaseClientIDReqSize( void )
{
	return sizeof( sQMUX ) + 11;
}

/*===========================================================================
METHOD:
   QMICTLReadyReqSize (Public Method)

DESCRIPTION:
   Get size of buffer needed for QMUX + QMICTLReadyReq
 
RETURN VALUE:
   u16 - size of buffer
===========================================================================*/
u16 QMICTLReadyReqSize( void )
{
	return sizeof( sQMUX ) + 6;
}

/*===========================================================================
METHOD:
   QMIWDSSetEventReportReqSize (Public Method)

DESCRIPTION:
   Get size of buffer needed for QMUX + QMIWDSSetEventReportReq
 
RETURN VALUE:
   u16 - size of buffer
===========================================================================*/
u16 QMIWDSSetEventReportReqSize( void )
{
	return sizeof( sQMUX ) + 15;
}

/*===========================================================================
METHOD:
   QMIWDSGetPKGSRVCStatusReqSize (Public Method)

DESCRIPTION:
   Get size of buffer needed for QMUX + QMIWDSGetPKGSRVCStatusReq
 
RETURN VALUE:
   u16 - size of buffer
===========================================================================*/
u16 QMIWDSGetPKGSRVCStatusReqSize( void )
{
	return sizeof( sQMUX ) + 7;
}

/*===========================================================================
METHOD:
   QMIWDSStartNetWorkInterfaceReqSize (Public Method)

DESCRIPTION:
   Get size of buffer needed for QMUX + QMIWDSStartNetWorkInterfaceReqSize
 
RETURN VALUE:
   u16 - size of buffer
===========================================================================*/
u16 QMIWDSStartNetWorkInterfaceReqSize( void )
{
	return sizeof( sQMUX ) + 7;
}

/*===========================================================================
METHOD:
   QMIDMSGetMEIDReqSize (Public Method)

DESCRIPTION:
   Get size of buffer needed for QMUX + QMIDMSGetMEIDReq
 
RETURN VALUE:
   u16 - size of buffer
===========================================================================*/
u16 QMIDMSGetMEIDReqSize( void )
{
	return sizeof( sQMUX ) + 7;
}

///wzm
//wzm add 



u16 QMIWDSStartNetworkReqSize( void )
{
	return sizeof( sQMUX ) + 7;
}
u16 QMIWDASetDataFormatReqSettingsSize( void )
{
	return sizeof( sQMUX ) + 11;
}
u16 QMIWDSGetRuntimeSettingsReqSize( void )
{
	return sizeof( sQMUX ) + 14;
}


u16 QMIWDSIndictionRegisterReqSize( void )
{
	return sizeof( sQMUX ) + 11 ;
}

u16 QMIWDSGetDunCallInfoReqSize( void )
{
	return sizeof( sQMUX ) + 18 ;
}
u16 QMIWDSGetRuntimeSettingReqSize( void )
{
	return sizeof( sQMUX ) + 14 ;
}

//wzm add 

//wzm
/*=========================================================================*/
// Generic QMUX functions
/*=========================================================================*/

/*===========================================================================
METHOD:
   ParseQMUX (Public Method)

DESCRIPTION:
   Remove QMUX headers from a buffer

PARAMETERS
   pClientID       [ O ] - On success, will point to Client ID
   pBuffer         [ I ] - Full Message passed in
   buffSize        [ I ] - Size of pBuffer

RETURN VALUE:
   int - Positive for size of QMUX header
         Negative errno for error
===========================================================================*/
int ParseQMUX(
   u16 *    pClientID,
   void *   pBuffer,
   u16      buffSize )
{
	sQMUX * pQMUXHeader;

	if (pBuffer == 0 || buffSize < 12)
	{
		return -ENOMEM;
	}

	// QMUX Header
	pQMUXHeader = (sQMUX *)pBuffer;

	if ((pQMUXHeader->mTF != 1)
	|| ( pQMUXHeader->mLength != le16_to_cpu(buffSize - 1))
	|| ( pQMUXHeader->mCtrlFlag != 0x80) )
	{
		return -EINVAL;
	}

	// Client ID   
	*pClientID = (pQMUXHeader->mQMIClientID << 8)
	         + pQMUXHeader->mQMIService;

	return sizeof( sQMUX );
}

/*===========================================================================
METHOD:
   FillQMUX (Public Method)

DESCRIPTION:
   Fill buffer with QMUX headers

PARAMETERS
   clientID        [ I ] - Client ID
   pBuffer         [ O ] - Buffer to be filled
   buffSize        [ I ] - Size of pBuffer (must be at least 6)

RETURN VALUE:
   int - 0 for success
         Negative errno for error
===========================================================================*/
int FillQMUX(
   u16      clientID,
   void *   pBuffer,
   u16      buffSize )
{
	sQMUX * pQMUXHeader;

	if (pBuffer == 0 ||  buffSize < sizeof( sQMUX ))
	{
		return -ENOMEM;
	}

	// QMUX Header
	pQMUXHeader = (sQMUX *)pBuffer;

	pQMUXHeader->mTF = 1;
	pQMUXHeader->mLength = cpu_to_le16(buffSize - 1);
	pQMUXHeader->mCtrlFlag = 0;

	// Service and Client ID   
	pQMUXHeader->mQMIService = clientID & 0xff;
	pQMUXHeader->mQMIClientID = clientID >> 8;
	return 0;
}

/*=========================================================================*/
// Generic QMI functions
/*=========================================================================*/

/*===========================================================================
METHOD:
   GetTLV (Public Method)

DESCRIPTION:
   Get data bufffer of a specified TLV from a QMI message

   QMI Message shall NOT include SDU
   
PARAMETERS
   pQMIMessage    [ I ] - QMI Message buffer
   messageLen     [ I ] - Size of QMI Message buffer
   type           [ I ] - Desired Type
   pOutDataBuf    [ O ] - Buffer to be filled with TLV
   messageLen     [ I ] - Size of QMI Message buffer

RETURN VALUE:
   u16 - Size of TLV for success
         Negative errno for error
===========================================================================*/
u16 GetTLV(
   void *   pQMIMessage,
   u16      messageLen,
   u8       type,
   void *   pOutDataBuf,
   u16      bufferLen )
{
	u16 pos;
	u16 tlvSize = 0;
	u16 cpyCount;

	if (pQMIMessage == 0 || pOutDataBuf == 0)
	{
		return -ENOMEM;
	}   

	for (pos = 4; pos + 3 < messageLen;	pos += tlvSize + 3)
	{
		tlvSize = le16_to_cpu(*(u16 *)(pQMIMessage + pos + 1));
		if (*(u8 *)(pQMIMessage + pos) == type)
		{
			if (bufferLen < tlvSize)
			{
				return -ENOMEM;
			}

			/* replacement memcpy
			memcpy( pOutDataBuf,	pQMIMessage + pos + 3, tlvSize ); */
		 
			for (cpyCount = 0; cpyCount < tlvSize; cpyCount++)
			{
				*((char*)(pOutDataBuf + cpyCount)) = *((char*)(pQMIMessage + pos + 3 + cpyCount));
			}
		 
			return tlvSize;
		}
	}
	return -ENOMSG;
}

/*===========================================================================
METHOD:
   ValidQMIMessage (Public Method)

DESCRIPTION:
   Check mandatory TLV in a QMI message

   QMI Message shall NOT include SDU

PARAMETERS
   pQMIMessage    [ I ] - QMI Message buffer
   messageLen     [ I ] - Size of QMI Message buffer

RETURN VALUE:
   int - 0 for success (no error)
         Negative errno for error
         Positive for QMI error code
===========================================================================*/
int ValidQMIMessage(
   void *   pQMIMessage,
   u16      messageLen )
{
	char mandTLV[4];

	if (GetTLV( pQMIMessage, messageLen, 2, &mandTLV[0], 4 ) == 4)
	{
		// Found TLV
		if (*(u16 *)&mandTLV[0] != 0)
		{
			return le16_to_cpu(*(u16 *)&mandTLV[2]);
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return -ENOMSG;
	}
}      

/*===========================================================================
METHOD:
   GetQMIMessageID (Public Method)

DESCRIPTION:
   Get the message ID of a QMI message
   
   QMI Message shall NOT include SDU

PARAMETERS
   pQMIMessage    [ I ] - QMI Message buffer
   messageLen     [ I ] - Size of QMI Message buffer

RETURN VALUE:
   int - Positive for message ID
         Negative errno for error
===========================================================================*/
int GetQMIMessageID(
   void *   pQMIMessage,
   u16      messageLen )
{
	if (messageLen < 2)
	{
		return -ENODATA;
	}
	else
	{
		return le16_to_cpu(*(u16 *)pQMIMessage);
	}
}

/*=========================================================================*/
// Fill Buffers with QMI requests
/*=========================================================================*/

/*===========================================================================
METHOD:
   QMICTLGetClientIDReq (Public Method)

DESCRIPTION:
   Fill buffer with QMI CTL Get Client ID Request

PARAMETERS
   pBuffer         [ 0 ] - Buffer to be filled
   buffSize        [ I ] - Size of pBuffer
   transactionID   [ I ] - Transaction ID
   serviceType     [ I ] - Service type requested

RETURN VALUE:
   int - Positive for resulting size of pBuffer
         Negative errno for error
===========================================================================*/
int QMICTLGetClientIDReq(
   void *   pBuffer,
   u16      buffSize,
   u8       transactionID,
   u8       serviceType )
{
	if (pBuffer == 0 || buffSize < QMICTLGetClientIDReqSize() )
	{
		return -ENOMEM;
	}

	// QMI CTL GET CLIENT ID
	// Request
	*(u8 *)(pBuffer + sizeof( sQMUX ))= 0x00;
	// Transaction ID
	*(u8 *)(pBuffer + sizeof( sQMUX ) + 1) = transactionID;
	// Message ID
	*(u16 *)(pBuffer + sizeof( sQMUX ) + 2) = cpu_to_le16(0x0022);
	// Size of TLV's
	*(u16 *)(pBuffer + sizeof( sQMUX ) + 4) = cpu_to_le16(0x0004);
	// QMI Service Type
	*(u8 *)(pBuffer + sizeof( sQMUX ) + 6)  = 0x01;
	// Size
	*(u16 *)(pBuffer + sizeof( sQMUX ) + 7) = cpu_to_le16(0x0001);
	// QMI svc type
	*(u8 *)(pBuffer + sizeof( sQMUX ) + 9)  = serviceType;

	// success
	return sizeof( sQMUX ) + 10;
}

/*===========================================================================
METHOD:
   QMICTLReleaseClientIDReq (Public Method)

DESCRIPTION:
   Fill buffer with QMI CTL Release Client ID Request

PARAMETERS
   pBuffer         [ 0 ] - Buffer to be filled
   buffSize        [ I ] - Size of pBuffer
   transactionID   [ I ] - Transaction ID
   clientID        [ I ] - Service type requested

RETURN VALUE:
   int - Positive for resulting size of pBuffer
         Negative errno for error
===========================================================================*/
int QMICTLReleaseClientIDReq(
   void *   pBuffer,
   u16      buffSize,
   u8       transactionID,
   u16      clientID )
{
	if (pBuffer == 0 || buffSize < QMICTLReleaseClientIDReqSize() )
	{
		return -ENOMEM;
	}

	// QMI CTL RELEASE CLIENT ID REQ
	// Request
	*(u8 *)(pBuffer + sizeof( sQMUX ))  = 0x00;
	// Transaction ID
	*(u8 *)(pBuffer + sizeof( sQMUX ) + 1 ) = transactionID;
	// Message ID
	*(u16 *)(pBuffer + sizeof( sQMUX ) + 2) = cpu_to_le16(0x0023);
	// Size of TLV's
	*(u16 *)(pBuffer + sizeof( sQMUX ) + 4) = cpu_to_le16(0x0005);
	// Release client ID
	*(u8 *)(pBuffer + sizeof( sQMUX ) + 6)  = 0x01;
	// Size
	*(u16 *)(pBuffer + sizeof( sQMUX ) + 7) = cpu_to_le16(0x0002);
	// QMI svs type / Client ID
	*(u16 *)(pBuffer + sizeof( sQMUX ) + 9)  = clientID;

	// success
	return sizeof( sQMUX ) + 11;
}

/*===========================================================================
METHOD:
   QMICTLReadyReq (Public Method)

DESCRIPTION:
   Fill buffer with QMI CTL Get Version Info Request

PARAMETERS
   pBuffer         [ 0 ] - Buffer to be filled
   buffSize        [ I ] - Size of pBuffer
   transactionID   [ I ] - Transaction ID

RETURN VALUE:
   int - Positive for resulting size of pBuffer
         Negative errno for error
===========================================================================*/
int QMICTLReadyReq(
   void *   pBuffer,
   u16      buffSize,
   u8       transactionID )
{
	if (pBuffer == 0 || buffSize < QMICTLReadyReqSize() )
	{
		return -ENOMEM;
	}

	// QMI CTL GET VERSION INFO REQ
	// Request
	*(u8 *)(pBuffer + sizeof( sQMUX ))  = 0x00;
	// Transaction ID
	*(u8 *)(pBuffer + sizeof( sQMUX ) + 1) = transactionID;
	// Message ID
	*(u16 *)(pBuffer + sizeof( sQMUX ) + 2) = cpu_to_le16(0x0021);
	// Size of TLV's
	*(u16 *)(pBuffer + sizeof( sQMUX ) + 4) = cpu_to_le16(0x0000);

	// success
	return sizeof( sQMUX ) + 6;
}

/*===========================================================================
METHOD:
   QMIWDSSetEventReportReq (Public Method)

DESCRIPTION:
   Fill buffer with QMI WDS Set Event Report Request

PARAMETERS
   pBuffer         [ 0 ] - Buffer to be filled
   buffSize        [ I ] - Size of pBuffer
   transactionID   [ I ] - Transaction ID

RETURN VALUE:
   int - Positive for resulting size of pBuffer
         Negative errno for error
===========================================================================*/
int QMIWDSSetEventReportReq(
   void *   pBuffer,
   u16      buffSize,
   u16      transactionID )
{
	if (pBuffer == 0 || buffSize < QMIWDSSetEventReportReqSize() )
	{
		return -ENOMEM;
	}

	// QMI WDS SET EVENT REPORT REQ
	// Request
	*(u8 *)(pBuffer + sizeof( sQMUX ))  = 0x00;
	// Transaction ID
	*(u16 *)(pBuffer + sizeof( sQMUX ) + 1) = cpu_to_le16(transactionID);
	// Message ID
	*(u16 *)(pBuffer + sizeof( sQMUX ) + 3) = cpu_to_le16(0x0001);
	// Size of TLV's
	*(u16 *)(pBuffer + sizeof( sQMUX ) + 5) = cpu_to_le16(0x0008);
	// Report channel rate TLV
	*(u8 *)(pBuffer + sizeof( sQMUX ) + 7)  = 0x11;
	// Size
	*(u16 *)(pBuffer + sizeof( sQMUX ) + 8) = cpu_to_le16(0x0005);
	// Stats period
	*(u8 *)(pBuffer + sizeof( sQMUX ) + 10)  = 0x01; //zxc 1
	// Stats mask
	*(u32 *)(pBuffer + sizeof( sQMUX ) + 11)  = cpu_to_le32(0x000000ff);

	// success
	return sizeof( sQMUX ) + 15;
}

/*===========================================================================
METHOD:
   QMIWDSGetPKGSRVCStatusReq (Public Method)

DESCRIPTION:
   Fill buffer with QMI WDS Get PKG SRVC Status Request

PARAMETERS
   pBuffer         [ 0 ] - Buffer to be filled
   buffSize        [ I ] - Size of pBuffer
   transactionID   [ I ] - Transaction ID

RETURN VALUE:
   int - Positive for resulting size of pBuffer
         Negative errno for error
===========================================================================*/
int QMIWDSGetPKGSRVCStatusReq(
   void *   pBuffer,
   u16      buffSize,
   u16      transactionID )
{
	if (pBuffer == 0 || buffSize < QMIWDSGetPKGSRVCStatusReqSize() )
	{
		return -ENOMEM;
	}

	// QMI WDS Get PKG SRVC Status REQ
	// Request
	*(u8 *)(pBuffer + sizeof( sQMUX ))  = 0x00;
	// Transaction ID
	*(u16 *)(pBuffer + sizeof( sQMUX ) + 1) = cpu_to_le16(transactionID);
	// Message ID
	*(u16 *)(pBuffer + sizeof( sQMUX ) + 3) = cpu_to_le16(0x0022);
	// Size of TLV's
	*(u16 *)(pBuffer + sizeof( sQMUX ) + 5) = cpu_to_le16(0x0000);

	// success
	return sizeof( sQMUX ) + 7;
}

/*===========================================================================
METHOD:
   QMIDMSGetMEIDReq (Public Method)

DESCRIPTION:
   Fill buffer with QMI DMS Get Serial Numbers Request

PARAMETERS
   pBuffer         [ 0 ] - Buffer to be filled
   buffSize        [ I ] - Size of pBuffer
   transactionID   [ I ] - Transaction ID

RETURN VALUE:
   int - Positive for resulting size of pBuffer
         Negative errno for error
===========================================================================*/
int QMIDMSGetMEIDReq(
   void *   pBuffer,
   u16      buffSize,
   u16      transactionID )
{
	if (pBuffer == 0 || buffSize < QMIDMSGetMEIDReqSize() )
	{
		return -ENOMEM;
	}

	// QMI DMS GET SERIAL NUMBERS REQ
	// Request
	*(u8 *)(pBuffer + sizeof( sQMUX ))  = 0x00;
	// Transaction ID
	*(u16 *)(pBuffer + sizeof( sQMUX ) + 1) = cpu_to_le16(transactionID);
	// Message ID
	*(u16 *)(pBuffer + sizeof( sQMUX ) + 3) = cpu_to_le16(0x0025);
	// Size of TLV's
	*(u16 *)(pBuffer + sizeof( sQMUX ) + 5) = cpu_to_le16(0x0000);

	// success
	return sizeof( sQMUX ) + 7;
}

/*===========================================================================
METHOD:
   QMIWDASetDataFormatReq(Public Method)

DESCRIPTION:
   Negotiating QMAP UL Data Aggregation protocol

PARAMETERS
   pBuffer         [ 0 ] - Buffer to be filled
   buffSize        [ I ] - Size of pBuffer
   transactionID   [ I ] - Transaction ID

RETURN VALUE:
   int - Positive for resulting size of pBuffer
         Negative errno for error
===========================================================================*/
int QMIWDASetDataFormatReq(
      void *   pBuffer,
      u16      buffSize,
      u16      transactionID
      //,
      //sGobiUSBNet * pDev
      )
{
	if (pBuffer == 0 || buffSize < QMIWDASetDataFormatReqSize() )
	{
		return -ENOMEM;
	}

	// QMI WDA SET DATA FORMAT REQ
	// Request
	*(u8 *)(pBuffer + sizeof( sQMUX ))  = 0x00;
	// Transaction ID
	*(u16 *)(pBuffer + sizeof( sQMUX ) + 1) = cpu_to_le16(transactionID);
	// Message ID
	*(u16 *)(pBuffer + sizeof( sQMUX ) + 3) = cpu_to_le16(0x0020);
	// Size of TLV's
	//*(u16 *)(pBuffer + sizeof( sQMUX ) + 5) = 0x001C;
	*(u16 *)(pBuffer + sizeof( sQMUX ) + 5) = cpu_to_le16(0x0032);
	*(u8 *)(pBuffer + sizeof( sQMUX ) + 7)  = 0x10;
	*(u16 *)(pBuffer + sizeof( sQMUX ) + 8) = cpu_to_le16(0x0001);
	////////// *(u8 *)(pBuffer + sizeof( sQMUX ) + 10)  = 0x00;

	/* DataFormat: 0-default; 1-QoS hdr present 2 bytes */
#ifdef QOS_MODE  //QOS_MPDE not define  ,QOS header is not ok 2.6.32
	*(u8 *)(pBuffer + sizeof( sQMUX ) + 10) = 0x0001; /* QOS header */
#else
	*(u8 *)(pBuffer + sizeof( sQMUX ) + 10) = 0x0000; /* no-QOS header */
#endif

	// Link Layer protocol
	*(u8 *)(pBuffer + sizeof( sQMUX ) + 11)  = 0x11;
	// Size
	*(u16 *)(pBuffer + sizeof( sQMUX ) + 12) = cpu_to_le16(0x0004);
	// IP is enabled
	//////////  *(u32 *)(pBuffer + sizeof( sQMUX ) + 14) = 0x00000002;
	/* LinkProt: 0x1 - ETH; 0x2 - rawIP  4 bytes */
#ifdef DATA_MODE_RP  //both is ok 2.6.32
	/* Set RawIP mode */
	*(u32 *)(pBuffer + sizeof( sQMUX ) + 14) = cpu_to_le32(0x00000002);
	//  DBG("Request RawIP Data Format\n");
#else
	/* Set Ethernet  mode */
	*(u32 *)(pBuffer + sizeof( sQMUX ) + 14) = cpu_to_le32(0x00000001); 
	//  DBG("Request Ethernet Data Format\n");
#endif

	// UL Data aggregation protocol
	*(u8 *)(pBuffer + sizeof( sQMUX ) + 18)  = 0x12;
	// Size
	*(u16 *)(pBuffer + sizeof( sQMUX ) + 19) = cpu_to_le16(0x0004);
	// UL QMAP is enabled
	*(u32 *)(pBuffer + sizeof( sQMUX ) + 21) = 0x00000000; //0x00000005; by chenlei

	// DL Data aggregation protocol
	*(u8 *)(pBuffer + sizeof( sQMUX ) + 25)  = 0x13;
	// Size
	*(u16 *)(pBuffer + sizeof( sQMUX ) + 26) = cpu_to_le16(0x0004);
	// DL QMAP is enabled
	//////////////*(u32 *)(pBuffer + sizeof( sQMUX ) + 28) = 0x00000005;
	*(u32 *)(pBuffer + sizeof( sQMUX ) + 28) = 0x00000000;
	// DL Data aggregation Max datagrams
	*(u8 *)(pBuffer + sizeof( sQMUX ) + 32)  = 0x15;
	// Size
	*(u16 *)(pBuffer + sizeof( sQMUX ) + 33) = cpu_to_le16(0x0004);
	// Datagram is set as 32768
	*(u32 *)(pBuffer + sizeof( sQMUX ) + 35) = 0x00000000; //0x00000020; by chenlei
	// DL Data aggregation Max datagrams
	*(u8 *)(pBuffer + sizeof( sQMUX ) + 39)  = 0x16;
	// Size
	*(u16 *)(pBuffer + sizeof( sQMUX ) + 40) = cpu_to_le16(0x0004);
	// Datagram is set as 32768
	*(u32 *)(pBuffer + sizeof( sQMUX ) + 42) = 0x00000000; //0x00008000; by chenlei

	// DL Data aggregation Max datagrams
	*(u8 *)(pBuffer + sizeof( sQMUX ) + 46)  = 0x17;
	// Size
	*(u16 *)(pBuffer + sizeof( sQMUX ) + 47) = cpu_to_le16(0x0008);
	// Datagram is set as 32768
	*(u32 *)(pBuffer + sizeof( sQMUX ) + 49) = cpu_to_le32(0x00000005);  //0x00000002 by chenlei
	// *(u32 *)(pBuffer + sizeof( sQMUX ) + 53) = pDev->mpEndpoints->mIntfNum;
	*(u32 *)(pBuffer + sizeof( sQMUX ) + 53) = 0; //4;
	// success
	return QMIWDASetDataFormatReqSize();
}

u16 QMIWDASetDataFormatReqSize( void )
{
	return sizeof( sQMUX ) + 57;
}

int QMIWDASetDataFormatResp(
   void *   pBuffer,
   u16      buffSize,
   u32 *    ULDatagram,
   u32 *    ULDatagramSize )
{
	int result;

	// Ignore QMUX and SDU
	u8 offset = sizeof( sQMUX ) + 3;
	u8 pktLinkProtocol[4];

	if (pBuffer == 0 || buffSize < offset || ULDatagram == 0 || ULDatagramSize == 0)
	{
		return -ENOMEM;
	}

	pBuffer = pBuffer + offset;
	buffSize -= offset;

	result = GetQMIMessageID( pBuffer, buffSize );
	if (result != 0x20)
	{
		return -EFAULT;
	}

	result = ValidQMIMessage( pBuffer, buffSize );
	if (result != 0)
	{
		return -EFAULT;
	}

	result = GetTLV( pBuffer, buffSize, 0x15, (void*)ULDatagram, 4 );
	if (result != 4)
	{
		return -EFAULT;
	}

	result = GetTLV( pBuffer, buffSize, 0x16, (void*)ULDatagramSize, 4 );
	if (result != 4)
	{
		return -EFAULT;
	}

	/* Check response message link protocol */
	result = GetTLV( pBuffer, buffSize, 0x11,
	             &pktLinkProtocol[0], 4);
	if (result != 4)
	{
		DBG("EFAULT: Wrong TLV format\n"); 
		return 0;
	}

#ifdef DATA_MODE_RP
	if (pktLinkProtocol[0] != 2)
	{
		DBG("EFAULT: Data Format Cannot be set to RawIP Mode\n"); 
		return pktLinkProtocol[0];
	}
	DBG("Data Format Set to RawIP\n");
#else
	if (pktLinkProtocol[0] != 1)
	{
		DBG("EFAULT: Data Format Cannot be set to Ethernet Mode\n"); 
		return pktLinkProtocol[0];
	}
	DBG("Data Format Set to Ethernet Mode \n");
#endif
	return pktLinkProtocol[0];
}

/*=========================================================================*/
// Parse data from QMI responses
/*=========================================================================*/

/*===========================================================================
METHOD:
   QMICTLGetClientIDResp (Public Method)

DESCRIPTION:
   Parse the QMI CTL Get Client ID Resp

PARAMETERS
   pBuffer         [ I ] - Buffer to be parsed
   buffSize        [ I ] - Size of pBuffer
   pClientID       [ 0 ] - Recieved client ID

RETURN VALUE:
   int - 0 for success
         Negative errno for error
===========================================================================*/
int QMICTLGetClientIDResp(
   void * pBuffer,
   u16    buffSize,
   u16 *  pClientID )
{
	int result;

	// Ignore QMUX and SDU
	//    QMI CTL SDU is 2 bytes, not 3
	u8 offset = sizeof( sQMUX ) + 2;

	if (pBuffer == 0 || buffSize < offset)
	{
		return -ENOMEM;
	}

	pBuffer = pBuffer + offset;
	buffSize -= offset;

	result = GetQMIMessageID( pBuffer, buffSize );
	if (result != 0x22)
	{
		return -EFAULT;
	}

	result = ValidQMIMessage( pBuffer, buffSize );
	if (result != 0)
	{
		return -EFAULT;
	}

	result = GetTLV( pBuffer, buffSize, 0x01, pClientID, 2 );
	if (result != 2)
	{
		return -EFAULT;
	}
	return 0;
}

/*===========================================================================
METHOD:
   QMICTLReleaseClientIDResp (Public Method)

DESCRIPTION:
   Verify the QMI CTL Release Client ID Resp is valid

PARAMETERS
   pBuffer         [ I ] - Buffer to be parsed
   buffSize        [ I ] - Size of pBuffer

RETURN VALUE:
   int - 0 for success
         Negative errno for error
===========================================================================*/
int QMICTLReleaseClientIDResp(
   void *   pBuffer,
   u16      buffSize )
{
	int result;

	// Ignore QMUX and SDU
	//    QMI CTL SDU is 2 bytes, not 3
	u8 offset = sizeof( sQMUX ) + 2;

	if (pBuffer == 0 || buffSize < offset)
	{
		return -ENOMEM;
	}

	pBuffer = pBuffer + offset;
	buffSize -= offset;

	result = GetQMIMessageID( pBuffer, buffSize );
	if (result != 0x23)
	{
		return -EFAULT;
	}

	result = ValidQMIMessage( pBuffer, buffSize );
	if (result != 0)
	{
		return -EFAULT;
	}

	return 0;
}

/*===========================================================================
METHOD:
   QMIWDSEventResp (Public Method)

DESCRIPTION:
   Parse the QMI WDS Set Event Report Resp/Indication or
      QMI WDS Get PKG SRVC Status Resp/Indication

   Return parameters will only be updated if value was received

PARAMETERS
   pBuffer         [ I ] - Buffer to be parsed
   buffSize        [ I ] - Size of pBuffer
   pTXOk           [ O ] - Number of transmitted packets without errors
   pRXOk           [ O ] - Number of recieved packets without errors
   pTXErr          [ O ] - Number of transmitted packets with framing errors
   pRXErr          [ O ] - Number of recieved packets with framing errors
   pTXOfl          [ O ] - Number of transmitted packets dropped due to overflow
   pRXOfl          [ O ] - Number of recieved packets dropped due to overflow
   pTXBytesOk      [ O ] - Number of transmitted bytes without errors
   pRXBytesOk      [ O ] - Number of recieved bytes without errors
   pbLinkState     [ 0 ] - Is the link active?
   pbReconfigure   [ 0 ] - Must interface be reconfigured? (reset IP address)

RETURN VALUE:
   int - 0 for success
         Negative errno for error
===========================================================================*/
int QMIWDSEventResp(
   void *   pBuffer,
   u16      buffSize,
   u32 *    pTXOk,
   u32 *    pRXOk,
   u32 *    pTXErr,
   u32 *    pRXErr,
   u32 *    pTXOfl,
   u32 *    pRXOfl,
   u64 *    pTXBytesOk,
   u64 *    pRXBytesOk,
   bool *   pbLinkState,
   bool *   pbReconfigure )
{
	int result;
	u8 pktStatusRead[2];

	// Ignore QMUX and SDU
	u8 offset = sizeof( sQMUX ) + 3;

	if (pBuffer == 0 
	|| buffSize < offset
	|| pTXOk == 0
	|| pRXOk == 0
	|| pTXErr == 0
	|| pRXErr == 0
	|| pTXOfl == 0
	|| pRXOfl == 0
	|| pTXBytesOk == 0
	|| pRXBytesOk == 0
	|| pbLinkState == 0
	|| pbReconfigure == 0 )
	{
		return -ENOMEM;
	}

	pBuffer = pBuffer + offset;
	buffSize -= offset;

	// Note: Indications.  No Mandatory TLV required

	result = GetQMIMessageID( pBuffer, buffSize );
	// QMI WDS Set Event Report Resp
	if (result == 0x01)
	{
		// TLV's are not mandatory
		GetTLV( pBuffer, buffSize, 0x10, (void*)pTXOk, 4 );
		put_unaligned( le32_to_cpu(*pTXOk), pTXOk);
		GetTLV( pBuffer, buffSize, 0x11, (void*)pRXOk, 4 );
		put_unaligned( le32_to_cpu(*pRXOk), pRXOk);
		GetTLV( pBuffer, buffSize, 0x12, (void*)pTXErr, 4 );
		put_unaligned( le32_to_cpu(*pTXErr), pTXErr);
		GetTLV( pBuffer, buffSize, 0x13, (void*)pRXErr, 4 );
		put_unaligned( le32_to_cpu(*pRXErr), pRXErr);
		GetTLV( pBuffer, buffSize, 0x14, (void*)pTXOfl, 4 );
		put_unaligned( le32_to_cpu(*pTXOfl), pTXOfl);
		GetTLV( pBuffer, buffSize, 0x15, (void*)pRXOfl, 4 );
		put_unaligned( le32_to_cpu(*pRXOfl), pRXOfl);
		GetTLV( pBuffer, buffSize, 0x19, (void*)pTXBytesOk, 8 );
		put_unaligned( le64_to_cpu(*pTXBytesOk), pTXBytesOk);
		GetTLV( pBuffer, buffSize, 0x1A, (void*)pRXBytesOk, 8 );
		put_unaligned( le64_to_cpu(*pRXBytesOk), pRXBytesOk);
	}
	// QMI WDS Get PKG SRVC Status Resp
	else if (result == 0x22)
	{
		result = GetTLV( pBuffer, buffSize, 0x01, &pktStatusRead[0], 2 );
		// 1 or 2 bytes may be received
		if (result >= 1)
		{
			if (pktStatusRead[0] == 0x02)
			{
				*pbLinkState = true;
			}
			else
			{
				*pbLinkState = false;
			}
		}

		if (result == 2)
		{
			if (pktStatusRead[1] == 0x01)
			{
				*pbReconfigure = true;
			}
			else
			{
				*pbReconfigure = false;
			}
		}

		if (result < 0)
		{
			return result;
		}
	}
	else
	{
		return -EFAULT;
	}

	return 0;
}

/*===========================================================================
METHOD:
   QMIDMSGetMEIDResp (Public Method)

DESCRIPTION:
   Parse the QMI DMS Get Serial Numbers Resp

PARAMETERS
   pBuffer         [ I ] - Buffer to be parsed
   buffSize        [ I ] - Size of pBuffer
   pMEID           [ O ] - Device MEID
   meidSize        [ I ] - Size of MEID buffer (at least 14)

RETURN VALUE:
   int - 0 for success
         Negative errno for error
===========================================================================*/
int QMIDMSGetMEIDResp(
   void *   pBuffer,
   u16      buffSize,
   char *   pMEID,
   int      meidSize )
{
	int result;

	// Ignore QMUX and SDU
	u8 offset = sizeof( sQMUX ) + 3;

	if (pBuffer == 0 || buffSize < offset || meidSize < 14)
	{
		return -ENOMEM;
	}

	pBuffer = pBuffer + offset;
	buffSize -= offset;

	result = GetQMIMessageID( pBuffer, buffSize );
	if (result != 0x25)
	{
		return -EFAULT;
	}

	result = ValidQMIMessage( pBuffer, buffSize );
	if (result != 0)
	{
		return -EFAULT;
	}

	result = GetTLV( pBuffer, buffSize, 0x12, (void*)pMEID, 14 );
	if (result != 14)
	{
		return -EFAULT;
	}

	return 0;
}

//modified by chenlei for qcrmcall at 20161222 begin
int QMIWDSBindMuxPortReq(
   void *   pBuffer,
   u16      buffSize,
   u16      transactionID,
   sGobiUSBNet *pDev
  )
{
	sQMIDev *QMIDev = NULL;
	if (pBuffer == 0 || buffSize < QMIWDSBindMuxPortReqSize() )
	{
		return -ENOMEM;
	}

	QMIDev = &pDev->mQMIDev;

	// QMI WDS BIND_MUX_DATA_PORT
	// Request
	*(u8 *)(pBuffer + sizeof( sQMUX ))  = 0x00;
	// Transaction ID
	*(u16 *)(pBuffer + sizeof( sQMUX ) + 1) = cpu_to_le16(transactionID);
	// Message ID
	*(u16 *)(pBuffer + sizeof( sQMUX ) + 3) = cpu_to_le16(0x00a2);
	// Size of TLV's
	*(u16 *)(pBuffer + sizeof( sQMUX ) + 5) = cpu_to_le16(0x0016);
	// ep type, len as 8 and value as HSUSB, Ifc number as 4
	*(u8 *)(pBuffer + sizeof( sQMUX ) + 7)  = 0x10;
	*(u16 *)(pBuffer + sizeof( sQMUX ) + 8)	= cpu_to_le16(0x0008);
	/////////// *(u32 *)(pBuffer + sizeof( sQMUX ) + 10)	= 0x00000002; //HSUSB 
	*(u32 *)(pBuffer + sizeof( sQMUX ) + 10)	= cpu_to_le32(0x00000005);
	/////////// *(u32 *)(pBuffer + sizeof( sQMUX ) + 14)	= pDev->mpEndpoints->mIntfNum; //Interface Num
	*(u32 *)(pBuffer + sizeof( sQMUX ) + 14)	= cpu_to_le32(QMIDev->mIntfNum); //0x00000008; //Interface Num

	// muxid , value is 0x81
	*(u8 *)(pBuffer + sizeof( sQMUX ) + 18)	= 0x11;
	*(u16 *)(pBuffer + sizeof( sQMUX ) + 19)  = cpu_to_le16(0x0001);
	//*(u8 *)(pBuffer + sizeof( sQMUX ) + 21)  = QMIDev->MuxId;
	//////// *(u8 *)(pBuffer + sizeof( sQMUX ) + 21)	= 0x81;//both 0x81 and 0x0 is ok 2.6.32
	*(u8 *)(pBuffer + sizeof( sQMUX ) + 21)	= QMIDev->MuxId; //0x0;

	///ep_type/iface_id/MuxId:0x00000005/0x00000008/0x0    is ok kernel2.6.32
	///ep_type/iface_id/MuxId:0x00000002/0x00000004/0x81   is ok kernel2.6.32
	// client type as tethered
	*(u8 *)(pBuffer + sizeof( sQMUX ) + 22)	= 0x13;
	*(u16 *)(pBuffer + sizeof( sQMUX ) + 23)  = cpu_to_le16(0x0004);
	*(u32 *)(pBuffer + sizeof( sQMUX ) + 25)  = cpu_to_le32(0x00000001); //teathered
	// success
	return sizeof( sQMUX ) + 29;
}
//modified by chenlei for qcrmcall at 20161222 end

u16 QMIWDSBindMuxPortReqSize( void )
{
	return sizeof( sQMUX ) + 29;
}

//added by chenlei for qcrmcall at 20161222 begin
/*===========================================================================
METHOD:
   QMIWDSBindMuxDataPortResp (Public Method)

DESCRIPTION:
   Parse the QMI WDS Bind Mux Data Port Response

PARAMETERS
   pBuffer         [ I ] - Buffer to be parsed
   buffSize        [ I ] - Size of pBuffer

RETURN VALUE:
   int - 0 for success
         Negative errno for error
===========================================================================*/
int QMIWDSBindMuxPortResp(
   void *   pBuffer,
   u16      buffSize )
{

	int result;

	u8 pktLinkProtocol[4];

	// Ignore QMUX and SDU
	// QMI SDU is 3 bytes
	u8 offset = sizeof( sQMUX ) + 3;

	if (pBuffer == 0 || buffSize < offset)
	{
		return -ENOMEM;
	}

	pBuffer = pBuffer + offset;
	buffSize -= offset;

	result = GetQMIMessageID( pBuffer, buffSize );
	if (result != 0xA2)
	{
		return -EFAULT;
	}

	/* Check response message result TLV */
	result = ValidQMIMessage( pBuffer, buffSize );
	if (result != 0)
	{
		DBG("EFAULT: Data Format Mode Bad Response\n"); 
		return -EFAULT; //0;
	}

	/* Check response message link protocol */
	result = GetTLV( pBuffer, buffSize, 0x02,
	             &pktLinkProtocol[0], 4);
	if (result != 4)
	{
		DBG("EFAULT: Wrong TLV format\n"); 
		return -EFAULT; //0;
	}

	return *((int *)&pktLinkProtocol[0]);
}
