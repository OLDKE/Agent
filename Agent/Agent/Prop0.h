#pragma once


// Prop0 �Ի���

class Prop0 : public CDialog
{
	DECLARE_DYNAMIC(Prop0)

public:
	Prop0(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Prop0();

// �Ի�������
	enum { IDD = IDD_PROP0 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
