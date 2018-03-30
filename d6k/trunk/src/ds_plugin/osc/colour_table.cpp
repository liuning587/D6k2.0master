
#include "colour_define.h"
#include "colour_table.h"

 

CColourTable::CColourTable()
{
	m_arrColour.emplace_back(QColor(XGraphics::AliceBlue));
	m_arrColour.emplace_back(QColor(XGraphics::AntiqueWhite));
	m_arrColour.emplace_back(QColor(XGraphics::Aqua));
	m_arrColour.emplace_back(QColor(XGraphics::Aquamarine));
	m_arrColour.emplace_back(QColor(XGraphics::Azure));
	m_arrColour.emplace_back(QColor(XGraphics::Beige));
	m_arrColour.emplace_back(QColor(XGraphics::Bisque));
	m_arrColour.emplace_back(QColor(XGraphics::Black));
	m_arrColour.emplace_back(QColor(XGraphics::BlanchedAlmond));
	m_arrColour.emplace_back(QColor(XGraphics::Blue));
	m_arrColour.emplace_back(QColor(XGraphics::BlueViolet));
	m_arrColour.emplace_back(QColor(XGraphics::Brown));
	m_arrColour.emplace_back(QColor(XGraphics::BurlyWood));
	m_arrColour.emplace_back(QColor(XGraphics::CadetBlue));
	m_arrColour.emplace_back(QColor(XGraphics::Chartreuse));
	m_arrColour.emplace_back(QColor(XGraphics::Chocolate));
	m_arrColour.emplace_back(QColor(XGraphics::Coral));
	m_arrColour.emplace_back(QColor(XGraphics::CornflowerBlue));
	m_arrColour.emplace_back(QColor(XGraphics::Cornsilk));
	m_arrColour.emplace_back(QColor(XGraphics::Crimson));
	m_arrColour.emplace_back(QColor(XGraphics::Cyan));
	m_arrColour.emplace_back(QColor(XGraphics::DarkBlue));
	m_arrColour.emplace_back(QColor(XGraphics::DarkCyan));
	m_arrColour.emplace_back(QColor(XGraphics::DarkGoldenrod));
	m_arrColour.emplace_back(QColor(XGraphics::DarkGray));
	m_arrColour.emplace_back(QColor(XGraphics::DarkGreen));
	m_arrColour.emplace_back(QColor(XGraphics::DarkKhaki));
	m_arrColour.emplace_back(QColor(XGraphics::DarkMagenta));
	m_arrColour.emplace_back(QColor(XGraphics::DarkOliveGreen));
	m_arrColour.emplace_back(QColor(XGraphics::DarkOrange));
	m_arrColour.emplace_back(QColor(XGraphics::DarkOrchid));
	m_arrColour.emplace_back(QColor(XGraphics::DarkRed));
	m_arrColour.emplace_back(QColor(XGraphics::DarkSalmon));
	m_arrColour.emplace_back(QColor(XGraphics::DarkSeaGreen));
	m_arrColour.emplace_back(QColor(XGraphics::DarkSlateBlue));
	m_arrColour.emplace_back(QColor(XGraphics::DarkSlateGray));
	m_arrColour.emplace_back(QColor(XGraphics::DarkTurquoise));
	m_arrColour.emplace_back(QColor(XGraphics::DarkViolet));
	m_arrColour.emplace_back(QColor(XGraphics::DeepPink));
	m_arrColour.emplace_back(QColor(XGraphics::DeepSkyBlue));
	m_arrColour.emplace_back(QColor(XGraphics::DimGray));
	m_arrColour.emplace_back(QColor(XGraphics::DodgerBlue));
	m_arrColour.emplace_back(QColor(XGraphics::Firebrick));
	m_arrColour.emplace_back(QColor(XGraphics::FloralWhite));
	m_arrColour.emplace_back(QColor(XGraphics::ForestGreen));
	m_arrColour.emplace_back(QColor(XGraphics::Fuchsia));
	m_arrColour.emplace_back(QColor(XGraphics::Gainsboro));
	m_arrColour.emplace_back(QColor(XGraphics::GhostWhite));
	m_arrColour.emplace_back(QColor(XGraphics::Gold));
	m_arrColour.emplace_back(QColor(XGraphics::Goldenrod));
	m_arrColour.emplace_back(QColor(XGraphics::Gray));
	m_arrColour.emplace_back(QColor(XGraphics::Green));
	m_arrColour.emplace_back(QColor(XGraphics::GreenYellow));
	m_arrColour.emplace_back(QColor(XGraphics::Honeydew));
	m_arrColour.emplace_back(QColor(XGraphics::HotPink));
	m_arrColour.emplace_back(QColor(XGraphics::IndianRed));
	m_arrColour.emplace_back(QColor(XGraphics::Indigo));
	m_arrColour.emplace_back(QColor(XGraphics::Ivory));
	m_arrColour.emplace_back(QColor(XGraphics::Khaki));
	m_arrColour.emplace_back(QColor(XGraphics::Lavender));
	m_arrColour.emplace_back(QColor(XGraphics::LavenderBlush));
	m_arrColour.emplace_back(QColor(XGraphics::LawnGreen));
	m_arrColour.emplace_back(QColor(XGraphics::LemonChiffon));
	m_arrColour.emplace_back(QColor(XGraphics::LightBlue));
	m_arrColour.emplace_back(QColor(XGraphics::LightCoral));
	m_arrColour.emplace_back(QColor(XGraphics::LightCyan));
	m_arrColour.emplace_back(QColor(XGraphics::LightGoldenrodYellow));
	m_arrColour.emplace_back(QColor(XGraphics::LightGreen));
	m_arrColour.emplace_back(QColor(XGraphics::LightGray));
	m_arrColour.emplace_back(QColor(XGraphics::LightPink));
	m_arrColour.emplace_back(QColor(XGraphics::LightSalmon));
	m_arrColour.emplace_back(QColor(XGraphics::LightSeaGreen));
	m_arrColour.emplace_back(QColor(XGraphics::LightSkyBlue));
	m_arrColour.emplace_back(QColor(XGraphics::LightSlateGray));
	m_arrColour.emplace_back(QColor(XGraphics::LightSteelBlue));
	m_arrColour.emplace_back(QColor(XGraphics::LightYellow));
	m_arrColour.emplace_back(QColor(XGraphics::Lime));
	m_arrColour.emplace_back(QColor(XGraphics::LimeGreen));
	m_arrColour.emplace_back(QColor(XGraphics::Linen));
	m_arrColour.emplace_back(QColor(XGraphics::Magenta));
	m_arrColour.emplace_back(QColor(XGraphics::Maroon));
	m_arrColour.emplace_back(QColor(XGraphics::MediumAquamarine));
	m_arrColour.emplace_back(QColor(XGraphics::MediumBlue));
	m_arrColour.emplace_back(QColor(XGraphics::MediumOrchid));
	m_arrColour.emplace_back(QColor(XGraphics::MediumPurple));
	m_arrColour.emplace_back(QColor(XGraphics::MediumSeaGreen));
	m_arrColour.emplace_back(QColor(XGraphics::MediumSlateBlue));
	m_arrColour.emplace_back(QColor(XGraphics::MediumSpringGreen));
	m_arrColour.emplace_back(QColor(XGraphics::MediumTurquoise));
	m_arrColour.emplace_back(QColor(XGraphics::MediumVioletRed));
	m_arrColour.emplace_back(QColor(XGraphics::MidnightBlue));
	m_arrColour.emplace_back(QColor(XGraphics::MintCream));
	m_arrColour.emplace_back(QColor(XGraphics::MistyRose));
	m_arrColour.emplace_back(QColor(XGraphics::Moccasin));
	m_arrColour.emplace_back(QColor(XGraphics::NavajoWhite));
	m_arrColour.emplace_back(QColor(XGraphics::Navy));
	m_arrColour.emplace_back(QColor(XGraphics::OldLace));
	m_arrColour.emplace_back(QColor(XGraphics::Olive));
	m_arrColour.emplace_back(QColor(XGraphics::OliveDrab));
	m_arrColour.emplace_back(QColor(XGraphics::Orange));
	m_arrColour.emplace_back(QColor(XGraphics::OrangeRed));
	m_arrColour.emplace_back(QColor(XGraphics::Orchid));
	m_arrColour.emplace_back(QColor(XGraphics::PaleGoldenrod));
	m_arrColour.emplace_back(QColor(XGraphics::PaleGreen));
	m_arrColour.emplace_back(QColor(XGraphics::PaleTurquoise));
	m_arrColour.emplace_back(QColor(XGraphics::PaleVioletRed));
	m_arrColour.emplace_back(QColor(XGraphics::PapayaWhip));
	m_arrColour.emplace_back(QColor(XGraphics::PeachPuff));
	m_arrColour.emplace_back(QColor(XGraphics::Peru));
	m_arrColour.emplace_back(QColor(XGraphics::Pink));
	m_arrColour.emplace_back(QColor(XGraphics::Plum));
	m_arrColour.emplace_back(QColor(XGraphics::PowderBlue));
	m_arrColour.emplace_back(QColor(XGraphics::Purple));
	m_arrColour.emplace_back(QColor(XGraphics::Red));
	m_arrColour.emplace_back(QColor(XGraphics::RosyBrown));
	m_arrColour.emplace_back(QColor(XGraphics::RoyalBlue));
	m_arrColour.emplace_back(QColor(XGraphics::SaddleBrown));
	m_arrColour.emplace_back(QColor(XGraphics::Salmon));
	m_arrColour.emplace_back(QColor(XGraphics::SandyBrown));
	m_arrColour.emplace_back(QColor(XGraphics::SeaGreen));
	m_arrColour.emplace_back(QColor(XGraphics::SeaShell));
	m_arrColour.emplace_back(QColor(XGraphics::Sienna));
	m_arrColour.emplace_back(QColor(XGraphics::Silver));
	m_arrColour.emplace_back(QColor(XGraphics::SkyBlue));
	m_arrColour.emplace_back(QColor(XGraphics::SlateBlue));
	m_arrColour.emplace_back(QColor(XGraphics::SlateGray));
	m_arrColour.emplace_back(QColor(XGraphics::Snow));
	m_arrColour.emplace_back(QColor(XGraphics::SpringGreen));
	m_arrColour.emplace_back(QColor(XGraphics::SteelBlue));
	m_arrColour.emplace_back(QColor(XGraphics::Tan));
	m_arrColour.emplace_back(QColor(XGraphics::Teal));
	m_arrColour.emplace_back(QColor(XGraphics::Thistle));
	m_arrColour.emplace_back(QColor(XGraphics::Tomato));
	m_arrColour.emplace_back(QColor(XGraphics::Turquoise));
	m_arrColour.emplace_back(QColor(XGraphics::Violet));
	m_arrColour.emplace_back(QColor(XGraphics::Wheat));
	m_arrColour.emplace_back(QColor(XGraphics::White));
	m_arrColour.emplace_back(QColor(XGraphics::WhiteSmoke));
	m_arrColour.emplace_back(QColor(XGraphics::Yellow));
	m_arrColour.emplace_back(QColor(XGraphics::YellowGreen));
}

CColourTable::~CColourTable()
{
	m_arrColour.clear();
}
 































































