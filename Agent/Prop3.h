#pragma once


// Prop3 对话框

class Prop3 : public CDialog
{
	DECLARE_DYNAMIC(Prop3)

public:
	Prop3(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Prop3();

// 对话框数据
	enum { IDD = IDD_PROP3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedIconBmp1();
};
