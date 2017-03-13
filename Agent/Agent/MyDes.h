// MyDes.h: interface for the CMyDes class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYDES_H__BA70C695_960A_4392_9CCA_9C439A3BC88D__INCLUDED_)
#define AFX_MYDES_H__BA70C695_960A_4392_9CCA_9C439A3BC88D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "NTTRACE.h"
class CMyDes  
{
public:
   CMyDes(CNTTRACE *pTrace);
	virtual ~CMyDes();
	CNTTRACE *m_trace;

	int Enc(char *idata,char *ikey,char *odata);
	int Dec(const char *idata,char *ikey,char *odata);

	int EncMsg(char *idata,char *key,char *odata);
	int DecMsg(char *idata,char *key,char *odata);

private:
	void transdata( unsigned char *input,unsigned char *output,unsigned char *function );
	void DesSel( unsigned char *input, unsigned char *output );
	void fnct( unsigned char *io, unsigned char *input );
	void _Decrypt( unsigned char *input,unsigned char *key,unsigned char *output );
	void _Encrypt( unsigned char *input,unsigned char *key,unsigned char *output );
	int Pack(unsigned char *pbyInBuffer,unsigned char *pbyOutBuffer, int iInBuffLen);
	int Unpack(unsigned char *pbyInBuffer,unsigned char *pbyOutBuffer,int iInBufLen);
	unsigned char x64[8],i64[8],k64[8],k56[7],k48[6],l32[4],r48[6],r32[4];


};

#endif // !defined(AFX_MYDES_H__BA70C695_960A_4392_9CCA_9C439A3BC88D__INCLUDED_)
