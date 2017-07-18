#include "predict_datainfo.h"

#include <QFile>
#include <QDomNode>

CReadXml::CReadXml()
{
	map_oneYCInfo.clear();
	map_TagName.clear();
}

CReadXml::~CReadXml()
{

}


bool CReadXml::ReadGroupInfo(const QString& szPath)
{
	Q_UNUSED(szPath);
	return true;
}

bool CReadXml::ReadPredictNodeInfo()
{
	return true;
}

bool CReadXml::ReadPredictFile()
{
	QXmlStreamReader xml;
	QFile file("powerpredict.xml");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return false;
	}
	xml.setDevice(&file);
	QDomDocument document;
	if (!document.setContent(&file))
	{
		file.close();
		return false;
	}
	QDomElement root = document.documentElement();
	if (root.tagName() != "plant")
	{
		file.close();
		return false;
	}
	ReadStationData(xml);
	file.close();
	return true;
}

bool CReadXml::ReadStationData(QXmlStreamReader& reader)
{
	while (!reader.atEnd())
	{
		reader.readNext();
		QString strTmp = reader.name().toString();

		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();
			if (strTmp == "p")
			{
				ReadPlantData(reader);
			}
			else if (strTmp == "w")
			{
				ReadWeatherData(reader);
			}
			else if (strTmp == "prdt")
			{
				ReadPredictData(reader);
			}
			else if (strTmp == "inverters")
			{
				ReadInverterData(reader);
			}
		}
		else if (reader.isEndElement() && strTmp == "plant")
		{
			break;
		}
	}
	return true;
}

bool CReadXml::ReadPlantData(QXmlStreamReader& reader)
{
	while (!reader.atEnd())
	{
		QString strTmp = reader.name().toString();
		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();
			if (strTmp == "p")
			{
				QString nName = reader.attributes().value("Name").toString();				
				ReadData_Plant(reader);
				map_YCInfo.insert("Plant Data", map_oneYCInfo);
			}
		}
		else if (reader.isEndElement() && strTmp == "p")
		{
			break;
		}
		reader.readNext();
	}
	return true;
}

bool CReadXml::ReadWeatherData(QXmlStreamReader& reader)
{
	while (!reader.atEnd())
	{
		QString strTmp = reader.name().toString();
		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();
			if (strTmp == "w")
			{			
				QString nName = reader.attributes().value("Name").toString();
				ReadData_Weather(reader);
				map_YCInfo.insert("Weather Data", map_oneYCInfo);
			}
		}
		else if (reader.isEndElement() && strTmp == "w")
		{
			break;
		}
		reader.readNext();
	}
	return true;
}

bool CReadXml::ReadPredictData(QXmlStreamReader& reader)
{
	while (!reader.atEnd())
	{
		QString strTmp = reader.name().toString();
		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();
			if (strTmp == "prdt")
			{
				QString nName = reader.attributes().value("Name").toString();
				ReadData_Predict(reader);
				map_YCInfo.insert("Predict Data", map_oneYCInfo);
			}
		}
		else if (reader.isEndElement() && strTmp == "prdt")
		{
			break;
		}
		reader.readNext();
	}
	return true;
}

bool CReadXml::ReadInverterData(QXmlStreamReader& reader)
{
	while (!reader.atEnd())
	{
		QString strTmp = reader.name().toString();
		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();
			if (strTmp == "inverters")
			{
				int nSixe = reader.attributes().value("Count").toInt();
				QString szName = reader.attributes().value("Name").toString();
				ReadSinglInverterData(reader);
				map_YCInfo.insert("Inverter Data", map_oneYCInfo);
			}
		}
		else if (reader.isEndElement() && strTmp == "inverters")
		{		
			break;
		}
		reader.readNext();
	}
	return true;
}

bool CReadXml::ReadSinglInverterData(QXmlStreamReader& reader)
{
	while (!reader.atEnd())
	{
		QString strTmp = reader.name().toString();
		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();
			if (strTmp == "inverter")
			{
				QString nName = reader.attributes().value("Name").toString();
				int nID = reader.attributes().value("ID").toInt();
				ReadData_Inverter(reader);
			}
			else if (reader.isEndElement())
			{
				break;
			}
			reader.readNext();
		}

	}


	return true;
}


bool CReadXml::ReadData_Plant(QXmlStreamReader& reader)
{
	while (!reader.atEnd())
	{
		QString strTmp = reader.name().toString();
		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();
			if (strTmp == "ADIN")
			{
				ReadPlantADIN(reader);
			}
		}
		else if (reader.isEndElement() && strTmp == "data")
		{
			break;
		}
		reader.readNext();
	}

	return true;
}

bool CReadXml::ReadData_Weather(QXmlStreamReader& reader)
{
	while (!reader.atEnd())
	{
		QString strTmp = reader.name().toString();
		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();
			if (strTmp == "ADIN")
			{
				ReadWeatherADIN(reader);
			}
		}
		else if (reader.isEndElement())
		{
			break;
		}
		reader.readNext();
	}

	return true;
}


bool CReadXml::ReadData_Inverter(QXmlStreamReader& reader)
{
	while (!reader.atEnd())
	{
		QString strTmp = reader.name().toString();
		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();
			if (strTmp == "ADIN")
			{
				ReadInverterADIN(reader);				
			}
		}
		else if (reader.isEndElement())
		{
			break;
		}
		reader.readNext();
	}

	return true;
}


bool CReadXml::ReadData_Predict(QXmlStreamReader& reader)
{
	while (!reader.atEnd())
	{
		QString strTmp = reader.name().toString();
		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();
			if (strTmp == "ADIN")
			{
				ReadPredictADIN(reader);				
			}
		}
		else if (reader.isEndElement())
		{
			break;
		}
		reader.readNext();
	}

	return true;
}



bool CReadXml::ReadPlantADIN(QXmlStreamReader& reader)
{
	while (!reader.atEnd())
	{
		QString strTmp = reader.name().toString();
		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();
			if (strTmp == "ai")
			{
				ReadPlantAi(reader);
			}
		}
		else if (reader.isEndElement() && strTmp == "ADIN")
		{
			break;
		}
		reader.readNext();
	}

	return true;
}

bool CReadXml::ReadWeatherADIN(QXmlStreamReader& reader)
{
	while (!reader.atEnd())
	{
		QString strTmp = reader.name().toString();
		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();

			if (strTmp == "ai")
			{
				ReadWeatherAi(reader);
			}
		}
		else if (reader.isEndElement() && strTmp == "ADIN")
		{
			break;
		}
		reader.readNext();
	}

	return true;
}


bool CReadXml::ReadInverterADIN(QXmlStreamReader& reader)
{
	while (!reader.atEnd())
	{
		QString strTmp = reader.name().toString();
		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();

			if (strTmp == "ai")
			{
				ReadInverterAi(reader);
			}
			else if (strTmp == "di")
			{
				ReadInverterDi(reader);
			}
		}
		else if (reader.isEndElement() && strTmp == "ADIN")
		{
			break;
		}
		reader.readNext();
	}

	return true;
}

bool CReadXml::ReadPredictADIN(QXmlStreamReader& reader)
{
	while (!reader.atEnd())
	{
		QString strTmp = reader.name().toString();
		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();

			if (strTmp == "ai")
			{
				ReadPredictAi(reader);
			}
		}
		else if (reader.isEndElement() && strTmp == "ADIN")
		{
			break;
		}
		reader.readNext();
	}

	return true;
}


bool CReadXml::ReadPlantAi(QXmlStreamReader& reader)
{
	while (!reader.atEnd())
	{
		QString strTmp = reader.name().toString();
		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();
			if (strTmp == "ai")
			{
				PPoint plantPointinfo;
				int nID = reader.attributes().value("ID").toInt();
				plantPointinfo.nID = nID;
				QString szName = reader.attributes().value("Name").toString();
				plantPointinfo.szName = szName;
				QString szLinkedName = reader.attributes().value("LinkedName").toString();
				plantPointinfo.szTagName = szLinkedName;
				int nType = reader.attributes().value("Type").toInt();
				plantPointinfo.nType = nType;
				map_oneYCInfo.insert(plantPointinfo.nID, plantPointinfo);
			}
		}
		else if (reader.isEndElement() && strTmp == "ai")
		{
			break;
		}
		reader.readNext();
	}

	return true;
}

bool CReadXml::ReadWeatherAi(QXmlStreamReader& reader)
{
	while (!reader.atEnd())
	{
		QString strTmp = reader.name().toString();
		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();
			if (strTmp == "ai")
			{
				PPoint weatherPointinfo;
				int nID = reader.attributes().value("ID").toInt();
				weatherPointinfo.nID = nID;
				QString szName = reader.attributes().value("Name").toString();
				weatherPointinfo.szName = szName;
				QString szLinkedName = reader.attributes().value("LinkedName").toString();
				weatherPointinfo.szTagName = szLinkedName;
				int nType = reader.attributes().value("Type").toInt();
				weatherPointinfo.nType = nType;
				map_oneYCInfo.insert(weatherPointinfo.nID, weatherPointinfo);
			}
		}
		else if (reader.isEndElement() && strTmp == "ai")
		{
			break;
		}
		reader.readNext();
	}

	return true;
}

bool CReadXml::ReadInverterAi(QXmlStreamReader& reader)
{
	while (!reader.atEnd())
	{
		QString strTmp = reader.name().toString();
		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();
			if (strTmp == "ai")
			{
				PPoint inverterAIPointinfo;
				int nID = reader.attributes().value("ID").toInt();
				inverterAIPointinfo.nID = nID;
				QString szName = reader.attributes().value("Name").toString();
				inverterAIPointinfo.szName = szName;
				QString szLinkedName = reader.attributes().value("LinkedName").toString();
				inverterAIPointinfo.szTagName = szLinkedName;
				int nType = reader.attributes().value("Type").toInt();
				inverterAIPointinfo.nType = nType;
				map_oneYCInfo.insert(inverterAIPointinfo.nID, inverterAIPointinfo);
			}
		}
		else if (reader.isEndElement() && strTmp == "ai")
		{
			break;
		}
		reader.readNext();
	}

	return true;
}

bool CReadXml::ReadPredictAi(QXmlStreamReader& reader)
{
	while (!reader.atEnd())
	{
		QString strTmp = reader.name().toString();
		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();
			if (strTmp == "ai")
			{
				PPoint predictPointinfo;
				int nID = reader.attributes().value("ID").toInt();
				predictPointinfo.nID = nID;
				QString szName = reader.attributes().value("Name").toString();
				predictPointinfo.szName = szName;
				QString szLinkedName = reader.attributes().value("LinkedName").toString();
				predictPointinfo.szTagName = szLinkedName;
				int nType = reader.attributes().value("Type").toInt();
				predictPointinfo.nType = nType;
				map_oneYCInfo.insert(predictPointinfo.nID, predictPointinfo);
			}
		}
		else if (reader.isEndElement() && strTmp == "ai")
		{
			break;
		}
		reader.readNext();
	}

	return true;
}


bool CReadXml::ReadInverterDi(QXmlStreamReader& reader)
{
	while (!reader.atEnd())
	{
		QString strTmp = reader.name().toString();
		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();
			if (strTmp == "di")
			{
				PPoint inverterDIPointinfo;
				int nID = reader.attributes().value("ID").toInt();
				inverterDIPointinfo.nID = nID;
				QString szName = reader.attributes().value("Name").toString();
				inverterDIPointinfo.szName = szName;
				QString szLinkedName = reader.attributes().value("LinkedName").toString();
				inverterDIPointinfo.szTagName = szLinkedName;
				int nType = reader.attributes().value("Type").toInt();
				inverterDIPointinfo.nType = nType;
				map_oneYCInfo.insert(inverterDIPointinfo.nID, inverterDIPointinfo);
			}
		}
		else if (reader.isEndElement() && strTmp == "di")
		{
			break;
		}
		reader.readNext();
	}

	return true;
}


CReadDatabase::CReadDatabase()
{

}


CReadDatabase::~CReadDatabase()
{

}