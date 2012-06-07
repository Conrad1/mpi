// ContextRule.cpp: implementation of the ContextRule class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ContextRule.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ContextRule::ContextRule()
{

	used = 0;
	rights = 0;
	wrongs = 0;
}

ContextRule::~ContextRule()
{

}
