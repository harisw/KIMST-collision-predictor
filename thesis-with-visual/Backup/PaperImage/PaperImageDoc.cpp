
// PaperImageDoc.cpp: CPaperImageDoc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include <fstream>
#include <sstream>
#include <iomanip>
using std::ofstream;
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "PaperImage.h"
#endif

#include "PaperImageDoc.h"

#include <propkey.h>

#include "CQArea.h"
#include "CQObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPaperImageDoc

IMPLEMENT_DYNCREATE(CPaperImageDoc, CDocument)

BEGIN_MESSAGE_MAP(CPaperImageDoc, CDocument)
	ON_COMMAND(ID_32806, &CPaperImageDoc::On32806)
END_MESSAGE_MAP()


// CPaperImageDoc 생성/소멸

CPaperImageDoc::CPaperImageDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CPaperImageDoc::~CPaperImageDoc()
{
	resetData();
}

BOOL CPaperImageDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}

// CPaperImageDoc serialization

void CPaperImageDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CPaperImageDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CPaperImageDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CPaperImageDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CPaperImageDoc 진단

#ifdef _DEBUG
void CPaperImageDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPaperImageDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

// CPaperImageDoc 명령

void CPaperImageDoc::resetData()
{
	int size = (int)m_CQAreas.size();
	for (int i = 0; i < size; i++)
	{
		delete m_CQAreas[i];
	}
	m_CQAreas.clear();

	size = (int)m_CQObjects.size();
	for (int i = 0; i < size; i++)
	{
		delete m_CQObjects[i];
	}
	m_CQObjects.clear();
}

int CPaperImageDoc::getMaxCount()
{
	int max = 0;
	int size = (int)m_CQObjects.size();
	for (int i = 0; i < size; i++)
	{
		int cnt = m_CQObjects[i]->getMoveCount();
		if (max < cnt)	max = cnt;
	}
	return max;
}

void CPaperImageDoc::On32806()
{
	ofstream outfile;
	outfile.open("events.txt", ofstream::trunc); // opens the file
	if (!outfile) { // file couldn't be opened
		cerr << "Error: file could not be opened" << endl;
		exit(1);
	}

	for (int i = 0; i < m_CQObjects.size(); ++i) {
		stringstream pX, pY;
		pX << fixed << setprecision(4) << m_CQObjects[i]->m_initPoint.X;
		pY << fixed << setprecision(4) << m_CQObjects[i]->m_initPoint.Y;

		outfile << m_CQObjects[i]->m_id << "|" << m_CQObjects[i]->m_objtype << "|" << m_CQObjects[i]->m_speedtype
			<< "|" << pX.str() << "," << pY.str()
			<< "|" << m_CQObjects[i]->m_trjtype
			<< "|" << m_CQObjects[i]->m_kmh << "|" << m_CQObjects[i]->m_bFriend;
		if (i+1 < m_CQObjects.size())
			outfile << endl;
	}
	outfile.close();
	// TODO: Add your command handler code here
}
