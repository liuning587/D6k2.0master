
#ifndef COLOUR_TABLE_H
#define COLOUR_TABLE_H
#pragma once

#include <vector>
#include <QColor>

class CColourTable
{
public:
	CColourTable();
	~CColourTable();
public:

private:
	std::vector<QColor> m_arrColour;
};




#endif // COLOUR_TABLE_H