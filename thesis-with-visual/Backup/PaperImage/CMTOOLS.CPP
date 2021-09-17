#include "pch.h"
#include "Cmtools.h"
#include "CmPan.h"

CPtrList CCMTools::c_Tools;

static CCMPan		Pan_Tool(PANTOOL);

ToolMode	CCMTools::c_eToolMode = TM_NONE;
/////////////////////////////////////////////////////////////////////////////
// CCMTools implementation

CCMTools::CCMTools(ToolMode nMode)
{
	m_eToolMode = nMode;
	c_Tools.AddTail(this);

	m_down = CPoint(-1, -1);
}

CCMTools::~CCMTools()
{
	c_Tools.RemoveAll();
}

CCMTools* CCMTools::FindTool(ToolMode nMode)
{
	POSITION pos = c_Tools.GetHeadPosition();
	while(pos != NULL)
	{
		CCMTools* pTool = (CCMTools*)c_Tools.GetNext(pos);
		if (pTool->m_eToolMode == nMode) return pTool;
	}
	return NULL;
}

void CCMTools::SetCmdMode(ToolMode nCmdMode)
{
	c_eToolMode = nCmdMode;
}
