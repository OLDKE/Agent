#pragma once


// Prop1 �Ի���

class Prop1 : public CDialog
{
	DECLARE_DYNAMIC(Prop1)

public:
	Prop1(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Prop1();

// �Ի�������
	enum { IDD = IDD_PROP1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
