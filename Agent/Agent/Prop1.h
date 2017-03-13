#pragma once


// Prop1 对话框

class Prop1 : public CDialog
{
	DECLARE_DYNAMIC(Prop1)

public:
	Prop1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Prop1();

// 对话框数据
	enum { IDD = IDD_PROP1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
