#pragma once

#include "encrycontrolwgt.h"

class CSingleControl
{
public:
	static CSingleControl *GetInstance();

	CEncryControlWgt * GetWgt();
	~CSingleControl();
private:
	CSingleControl();
	static CSingleControl *m_pSingleControl;
	//
	CEncryControlWgt *m_pControlWgt;

};

