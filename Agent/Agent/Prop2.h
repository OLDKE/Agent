#pragma once


// Prop2 �Ի���

class Prop2 : public CDialog
{
	DECLARE_DYNAMIC(Prop2)

public:
	Prop2(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Prop2();

// �Ի�������
	enum { IDD = IDD_PROP2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
