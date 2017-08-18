#pragma once

#include "read_xml.h"

#include <QDomDocument>
#include <QFile>

CReadPowerXml::CReadPowerXml(const std::string& szPathName) : m_strPathName(szPathName)
{
}

CReadPowerXml::~CReadPowerXml()
{
}

bool CReadPowerXml::LoadInfo()
{
	Q_ASSERT(m_strPathName.size() > 0);
	if (m_strPathName.size() <= 0)
	{
		LogMsg("PathName is empty!!!");
		return false;
	}
	QDomDocument doc;

	QFile file(m_strPathName.c_str());

	if (!file.open(QIODevice::ReadOnly))
	{
		LogMsg("File open fail!!!");
		return  false;
	}

	if (!doc.setContent(&file))
	{
		file.close();
		LogMsg("File format error!!!");
		Q_ASSERT(false);
		return false;
	}

	QDomElement rootElem = doc.documentElement();

	// todo:多场站再修改
	if (rootElem.tagName() != "station")
	{
		LogMsg("File can not get <station>!", 0);
		file.close();
		return false;
	}

	for (QDomNode nChild = rootElem.firstChild(); !nChild.isNull(); nChild = nChild.nextSibling())
	{
		QDomElement childElem = nChild.toElement();

		// 场站
		if (childElem.nodeName() == "s")
		{
			QDomElement domElem = childElem.toElement();

			int nStationID = domElem.attribute("StationID").trimmed().toInt();
			QString strStationName = domElem.attribute("Description").trimmed();
			QString strAlgorithmName = domElem.attribute("Algorithm").trimmed();
			QString strReversePath = domElem.attribute("ReverseIsolationPath").trimmed();

			std::shared_ptr<CStationInfo> pStationInfo = std::make_shared<CStationInfo>();
			pStationInfo->Init();
			pStationInfo->SetStationID(nStationID);
			pStationInfo->SetStationName(strStationName.toStdString());
			pStationInfo->SetAlgorithmName(strAlgorithmName.toStdString());
			pStationInfo->SetReversePath(strReversePath.toStdString());

			// 加载场站信息
			if (!LoadStationInfo(childElem, pStationInfo))
			{
				// log
				file.close();
				return false;
			}

			m_vecStationInfo.push_back(pStationInfo);
		}		
	}	

	file.close();

	// todo:
	// 检查配置信息

	return true;
}

std::vector<std::shared_ptr<CStationInfo>>& CReadPowerXml::GetAllStationInfo()
{
	return m_vecStationInfo;
}

bool CReadPowerXml::LoadStationInfo(const QDomElement& dom, std::shared_ptr<CStationInfo> pStationInfo)
{
	Q_ASSERT(pStationInfo);
	if (pStationInfo == nullptr)
	{
		// log
		return false;
	}

	for (QDomNode domNode = dom.firstChild(); !domNode.isNull(); domNode = domNode.nextSibling())
	{
		QDomElement childElem = domNode.toElement();

		// 获取站内数据
		if (childElem.nodeName() == "plant")
		{
			// 加载站内信息
			if (!LoadPlantInfo(childElem, pStationInfo))
			{
				// log
				return false;
			}
		}
		// 获取气象站数据
		else if (childElem.nodeName() == "weather")
		{
			// 加载气象站信息
			if (!LoadWeatherInfo(childElem, pStationInfo))
			{
				// log
				return false;
			}
		}
		// 获取预测数据
		else if (childElem.nodeName() == "prdt")
		{
			if (!LoadPredictInfo(childElem, pStationInfo))
			{
				// log
				return false;
			}
		}
		// 获取逆变器数据
		else if (childElem.nodeName() == "inverters")
		{
			if (!LoadInvertersInfo(childElem, pStationInfo))
			{
				// log
				return false;
			}
		}
	}

	return true;
}

bool CReadPowerXml::LoadPlantInfo(const QDomElement& dom, std::shared_ptr<CStationInfo> pStationInfo)
{
	Q_ASSERT(pStationInfo);
	if (pStationInfo == nullptr)
	{
		return false;
	}

	for (QDomNode childDom = dom.firstChild(); !childDom.isNull(); childDom = childDom.nextSibling())
	{
		QDomElement childElem = childDom.toElement();

		// 静态info
		if (childElem.tagName() == "staticdata")
		{
			if (!LoadPlantStaticInfo(childElem, pStationInfo->GetPlant()))
			{
				return false;
			}
		}
		// 动态info
		else if (childElem.tagName() == "dynamicdata")
		{
			if (!LoadPlantDynamicInfo(childElem, pStationInfo->GetPlant(), pStationInfo))
			{
				return false;
			}
		}
	}

	return pStationInfo->GetPlant()->Init();
}

bool CReadPowerXml::LoadPlantStaticInfo(const QDomElement& dom, std::shared_ptr<CPlantData> pPlantData)
{
	Q_ASSERT(pPlantData);
	if (pPlantData == nullptr)
	{
		return false;
	}

	return true;
}

bool CReadPowerXml::LoadPlantDynamicInfo(const QDomElement& dom, std::shared_ptr<CPlantData> pPlantData, std::shared_ptr<CStationInfo> pStationInfo)
{
	Q_ASSERT(pPlantData);
	if (pPlantData == nullptr)
	{
		return false;
	}

	for (QDomNode domNode = dom.firstChild(); !domNode.isNull(); domNode = domNode.nextSibling())
	{
		QDomElement childElem = domNode.toElement();

		QString strTagname = childElem.attribute("LinkedName").trimmed();

		int nID = childElem.attribute("ID").trimmed().toInt();
		QString strDesc = childElem.attribute("Description").trimmed();

		if (childElem.attribute("Name") == PLANT_ACTIVE_POWER)
		{
			pPlantData->SetActPowerTagname(strTagname.toStdString());
		}
		else if (childElem.attribute("Name") == PLANT_REACTIVE_POWER)
		{
			pPlantData->SetReactPowerTagname(strTagname.toStdString());
		}
		else
		{
			continue;
		}

		DATA_INFO dataInfo;
		dataInfo.nStationId = pStationInfo->GetStationID();
		dataInfo.nDeviceId = 0;
		dataInfo.nPointId = nID;
		dataInfo.strDesc = strDesc.toStdString();
		dataInfo.strTagName = strTagname.toStdString();

		pStationInfo->SetStationInfo(dataInfo);
	}

	return true;
}

bool CReadPowerXml::LoadWeatherInfo(const QDomElement& dom, std::shared_ptr<CStationInfo> pStationInfo)
{
	Q_ASSERT(pStationInfo);
	if (pStationInfo == nullptr)
	{
		return false;
	}

	std::shared_ptr<CWeatherStationData> pWeatherData = pStationInfo->GetWeatherStation();

	for (QDomNode childDom = dom.firstChild(); !childDom.isNull(); childDom = childDom.nextSibling())
	{
		QDomElement childElem = childDom.toElement();

		// 天气info
		if (childElem.tagName() == "data")
		{
			for (QDomNode domNode = childElem.firstChild(); !domNode.isNull(); domNode = domNode.nextSibling())
			{
				QDomElement childElem = domNode.toElement();

				QString strTagname = childElem.attribute("LinkedName").trimmed();

				int nID = childElem.attribute("ID").trimmed().toInt();
				QString strDesc = childElem.attribute("Description").trimmed();

				if (childElem.attribute("Name") == TOTAL_RADIATION)
				{
					pWeatherData->SetTotalRadiationTagname(strTagname.toStdString());
				}
				else if (childElem.attribute("Name") == DIRECT_RADIATION)
				{
					pWeatherData->SetDirectRadiationTagname(strTagname.toStdString());
				}
				else if (childElem.attribute("Name") == SCATTERING_RADIATION)
				{
					pWeatherData->SetScattRadiationTagname(strTagname.toStdString());
				}
				else if (childElem.attribute("Name") == AVERAGE_WIND_DIRECTION)
				{
					pWeatherData->SetDirectRadiationTagname(strTagname.toStdString());
				}
				else if (childElem.attribute("Name") == AVERAGE_WIND_SPEED)
				{
					pWeatherData->SetAvergWindSpeedTagname(strTagname.toStdString());
				}
				else if (childElem.attribute("Name") == AIR_TEMPERATURE)
				{
					pWeatherData->SetAirTemperatureTagname(strTagname.toStdString());
				}
				else if (childElem.attribute("Name") == RELATIVE_HUMIDITY)
				{
					pWeatherData->SetRelativeHumdtyTagname(strTagname.toStdString());
				}
				else if (childElem.attribute("Name") == AIR_PRESSURE)
				{
					pWeatherData->SetAirPressureTagname(strTagname.toStdString());
				}
				else if (childElem.attribute("Name") == COMPONENT_TEMPERATURE)
				{
					pWeatherData->SetComponentDescriptionTagname(strTagname.toStdString());
				}
				else
				{
					continue;
				}

				DATA_INFO dataInfo;
				dataInfo.nStationId = pStationInfo->GetStationID();
				dataInfo.nDeviceId = 0;
				dataInfo.nPointId = nID;
				dataInfo.strDesc = strDesc.toStdString();
				dataInfo.strTagName = strTagname.toStdString();

				pStationInfo->SetStationInfo(dataInfo);
			}
		}
	}

	return pStationInfo->GetWeatherStation()->Init();;
}

bool CReadPowerXml::LoadPredictInfo(const QDomElement& dom, std::shared_ptr<CStationInfo> pStationInfo)
{
	Q_ASSERT(pStationInfo);
	if (pStationInfo == nullptr)
	{
		return false;
	}

	std::shared_ptr<CPredictCalcData> pPredictData = pStationInfo->GetPredictCalc();

	for (QDomNode childDom = dom.firstChild(); !childDom.isNull(); childDom = childDom.nextSibling())
	{
		QDomElement childElem = childDom.toElement();

		// 预测info
		if (childElem.tagName() == "data")
		{
			for (QDomNode domNode = childElem.firstChild(); !domNode.isNull(); domNode = domNode.nextSibling())
			{
				QDomElement childDomElem = domNode.toElement();

				QString strTagname = childDomElem.attribute("LinkedName").trimmed();

				int nID = childElem.attribute("ID").trimmed().toInt();
				QString strDesc = childElem.attribute("Description").trimmed();

				if (childElem.attribute("Name") == PLANT_4_HOUR_PREDICATION_VALUE)
				{
					pPredictData->SetUltraShortPredict_4_Tagname(strTagname.toStdString());
				}
				else if (childElem.attribute("Name") == PLANT_72_HOUR_PREDICATION_VALUE)
				{
					pPredictData->SetShortPredict_72_Tagname(strTagname.toStdString());
				}
				else
				{
					continue;
				}

				DATA_INFO dataInfo;
				dataInfo.nStationId = pStationInfo->GetStationID();
				dataInfo.nDeviceId = 0;
				dataInfo.nPointId = nID;
				dataInfo.strDesc = strDesc.toStdString();
				dataInfo.strTagName = strTagname.toStdString();

				pStationInfo->SetStationInfo(dataInfo);
			}
		}
	}

	return pStationInfo->GetPredictCalc()->Init();
}

bool CReadPowerXml::LoadInvertersInfo(const QDomElement& dom, std::shared_ptr<CStationInfo> pStationInfo)
{
	Q_ASSERT(pStationInfo);
	if (pStationInfo == nullptr)
	{
		return false;
	}

	std::vector<std::shared_ptr<CInverterData>>& vecInverterData = pStationInfo->GetInverter();
	// 单个逆变器分别初始化
	for (QDomNode childDom = dom.firstChild(); !childDom.isNull(); childDom = childDom.nextSibling())
	{
		QDomElement childElem = childDom.toElement();

		// 预测info
		if (childElem.tagName() == "inverter")
		{
			for (QDomNode domNode = childElem.firstChild(); !domNode.isNull(); domNode = domNode.nextSibling())
			{
				QDomElement childDomElem = domNode.toElement();

				if (childDomElem.tagName() == "data")
				{
					int nDeviceID = childDomElem.attribute("DeviceID").trimmed().toInt();

					std::shared_ptr<CInverterData> pInverterData = std::make_shared<CInverterData>();

					for (QDomNode domChildNode = childDomElem.firstChild(); !domChildNode.isNull(); domChildNode = domChildNode.nextSibling())
					{
						QDomElement nextChildDomElem = domNode.toElement();

						QString strTagname = nextChildDomElem.attribute("LinkedName").trimmed();

						int nID = childElem.attribute("ID").trimmed().toInt();
						QString strDesc = childElem.attribute("Description").trimmed();

						if (childElem.attribute("Name") == INVERTER_ACTIVE_POWER)
						{
							pInverterData->SetInverterActivePowerTagname(strTagname.toStdString());
						}
						else if (childElem.attribute("Name") == INVERTER_REACTIVE_POWER)
						{
							pInverterData->SetInverterReactivePowerTagname(strTagname.toStdString());
						}
						else if (childElem.attribute("Name") == INVERTER_VOLTAGE)
						{
							pInverterData->SetInverterVoltageTagname(strTagname.toStdString());
						}
						else if (childElem.attribute("Name") == INVERTER_STATUS)
						{
							pInverterData->SetInverterStatusTagname(strTagname.toStdString());
						}
						else
						{
							continue;
						}

						DATA_INFO dataInfo;
						dataInfo.nStationId = pStationInfo->GetStationID();
						dataInfo.nDeviceId = 0;
						dataInfo.nPointId = nID;
						dataInfo.strDesc = strDesc.toStdString();
						dataInfo.strTagName = strTagname.toStdString();

						pStationInfo->SetStationInfo(dataInfo);
					}

					if (!pInverterData->Init())
					{
						return false;
					}

					vecInverterData.push_back(pInverterData);
				}				
			}
		}
	}
	return true;
}

void CReadPowerXml::LogMsg(const char *szLogTxt, int nLevel/* = 0*/)
{
	::LogMsg(LOG_FILENAME, szLogTxt, nLevel, nullptr);
}
