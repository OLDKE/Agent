#pragma once


// Prop0 对话框

class Prop0 : public CDialog
{
	DECLARE_DYNAMIC(Prop0)

public:
	Prop0(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Prop0();

// 对话框数据
	enum { IDD = IDD_PROP0 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
