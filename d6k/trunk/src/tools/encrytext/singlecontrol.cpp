#include "singlecontrol.h"
#include "encrycontrolwgt.h"

CSingleControl * CSingleControl::m_pSingleControl = nullptr;

CSingleControl::CSingleControl()
{
	m_pControlWgt = new CEncryControlWgt;
}


CSingleControl * CSingleControl::GetInstance()
{
	if (m_pSingleControl == nullptr)
	{
		m_pSingleControl = new CSingleControl;
	}

	return m_pSingleControl;

}

CEncryControlWgt * CSingleControl::GetWgt()
{
	return m_pControlWgt;
}

CSingleControl::~CSingleControl()
{
}
