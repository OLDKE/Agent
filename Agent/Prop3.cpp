// Prop3.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NTAgent3.h"
#include "Prop3.h"


// Prop3 �Ի���

IMPLEMENT_DYNAMIC(Prop3, CDialog)

Prop3::Prop3(CWnd* pParent /*=NULL*/)
	: CDialog(Prop3::IDD, pParent)
{

}

Prop3::~Prop3()
{
}

void Prop3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Prop3, CDialog)
	ON_STN_CLICKED(IDI_ICON_BMP1, Prop3::OnStnClickedIconBmp1)
END_MESSAGE_MAP()


// Prop3 ��Ϣ�������

void Prop3::OnStnClickedIconBmp1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
