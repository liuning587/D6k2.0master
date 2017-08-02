#include "predictwgt.h"
#include "predicition_wgt.h"
#include <memory>

static std::shared_ptr<CPredicitionWgt> s_Prediction = nullptr ;

extern "C"
{
	QWidget * GetMainWgt()
	{
		if (!s_Prediction)
		{
			s_Prediction = std::make_shared<CPredicitionWgt>();
		}
		return s_Prediction.get();
	}

	void DestroyWgt()
	{
		if (s_Prediction)
		{
			s_Prediction.reset();
		}
	}
}