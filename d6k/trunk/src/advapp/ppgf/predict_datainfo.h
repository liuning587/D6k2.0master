#include <QString>

#include <QMap>
#include <QDomElement>
#include <QXmlStreamReader>


typedef struct _tagPPoint
{
	int nID;
	int nType;
	QString szName;
	QString szTagName;
}PPoint;


class CReadXml
{
public:
	CReadXml();
	~CReadXml();

public:

	bool ReadGroupInfo(const QString& szPath);
	bool ReadPredictNodeInfo();
	bool ReadPredictFile();
	bool ReadStationData(QXmlStreamReader& reader);

	bool ReadPlantData(QXmlStreamReader& reader);
	bool ReadWeatherData(QXmlStreamReader& reader);
	bool ReadPredictData(QXmlStreamReader& reader);
	bool ReadInverterData(QXmlStreamReader& reader);
	bool ReadSinglInverterData(QXmlStreamReader& reader);

	bool ReadData_Plant(QXmlStreamReader& reader);
	bool ReadData_Weather(QXmlStreamReader& reader);
	bool ReadData_Inverter(QXmlStreamReader& reader);
	bool ReadData_Predict(QXmlStreamReader& reader);

	bool ReadPlantADIN(QXmlStreamReader& reader);
	bool ReadWeatherADIN(QXmlStreamReader& reader);
	bool ReadInverterADIN(QXmlStreamReader& reader);
	bool ReadPredictADIN(QXmlStreamReader& reader);

	bool ReadPlantAi(QXmlStreamReader& reader);
	bool ReadWeatherAi(QXmlStreamReader& reader);
	bool ReadInverterAi(QXmlStreamReader& reader);
	bool ReadPredictAi(QXmlStreamReader& reader);
	bool ReadInverterDi(QXmlStreamReader& reader);

private:

	/*PPoint plantPointinfo;
	PPoint weatherPointinfo;
	PPoint predictPointinfo; 
	PPoint inverterAIPointinfo;
	PPoint inverterDIPointinfo;*/

	//一条遥测记录
	QMap<int, PPoint> map_oneYCInfo;

	//QMap<int, PPoint> map_oneYXInfo;

	//一组遥测记录
	QMap<QString, QMap<int, PPoint> > map_YCInfo;

	//多厂站
	QMap<QString, QMap<QString, QMap<int, PPoint> > > map_StnYCInfo;

	QMap<QString, QString>  map_TagName;

};


class CReadDatabase
{
public:
	CReadDatabase();
	~CReadDatabase();

public:


};