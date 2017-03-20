// Prop1.cpp : 实现文件
//

#include "stdafx.h"
#include "NTAgent3.h"
#include "Prop1.h"


// Prop1 对话框

IMPLEMENT_DYNAMIC(Prop1, CDialog)

Prop1::Prop1(CWnd* pParent /*=NULL*/)
	: CDialog(Prop1::IDD, pParent)
{

}

Prop1::~Prop1()
{
}

void Prop1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Prop1, CDialog)
END_MESSAGE_MAP()


// Prop1 消息处理程序

void Prop1::OnBnClickedButton1()
{
	MessageBox(_T("aaa"));
}
