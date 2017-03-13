#pragma once


// Prop2 对话框

class Prop2 : public CDialog
{
	DECLARE_DYNAMIC(Prop2)

public:
	Prop2(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Prop2();

// 对话框数据
	enum { IDD = IDD_PROP2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
