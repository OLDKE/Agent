#pragma once


// Prop3 �Ի���

class Prop3 : public CDialog
{
	DECLARE_DYNAMIC(Prop3)

public:
	Prop3(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Prop3();

// �Ի�������
	enum { IDD = IDD_PROP3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedIconBmp1();
};
