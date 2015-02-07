#if !defined(AFX_XQWLIGHTCTL_H__FFD433A1_BFCF_4BA6_947F_67756786860C__INCLUDED_)
#define AFX_XQWLIGHTCTL_H__FFD433A1_BFCF_4BA6_947F_67756786860C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// XQWLightCtl.h : Declaration of the CXQWLightCtrl ActiveX Control class.

#include <objsafe.h>

/* === 以下是象棋小巫师的引擎代码 === */

// 其他常数
const int MAX_GEN_MOVES = 128; // 最大的生成走法数
const int MAX_MOVES = 256;     // 最大的历史走法数
const int LIMIT_DEPTH = 64;    // 最大的搜索深度
const int MATE_VALUE = 10000;  // 最高分值，即将死的分值
const int BAN_VALUE = MATE_VALUE - 100; // 长将判负的分值，低于该值将不写入置换表
const int WIN_VALUE = MATE_VALUE - 200; // 搜索出胜负的分值界限，超出此值就说明已经搜索出杀棋了
const int DRAW_VALUE = 20;     // 和棋时返回的分数(取负值)
const int ADVANCED_VALUE = 3;  // 先行权分值
const int RANDOM_MASK = 7;     // 随机性分值
const int NULL_MARGIN = 400;   // 空步裁剪的子力边界
const int NULL_DEPTH = 2;      // 空步裁剪的裁剪深度
const int HASH_SIZE = 1 << 20; // 置换表大小
const int HASH_ALPHA = 1;      // ALPHA节点的置换表项
const int HASH_BETA = 2;       // BETA节点的置换表项
const int HASH_PV = 3;         // PV节点的置换表项
const int BOOK_SIZE = 16384;   // 开局库大小

// RC4密码流生成器
struct RC4Struct {
  BYTE s[256];
  int x, y;

  void InitZero(void);   // 用空密钥初始化密码流生成器
  BYTE NextByte(void) {  // 生成密码流的下一个字节
    BYTE uc;
    x = (x + 1) & 255;
    y = (y + s[x]) & 255;
    uc = s[x];
    s[x] = s[y];
    s[y] = uc;
    return s[(s[x] + s[y]) & 255];
  }
  DWORD NextLong(void) { // 生成密码流的下四个字节
    BYTE uc0, uc1, uc2, uc3;
    uc0 = NextByte();
    uc1 = NextByte();
    uc2 = NextByte();
    uc3 = NextByte();
    return uc0 + (uc1 << 8) + (uc2 << 16) + (uc3 << 24);
  }
};

// Zobrist结构
struct ZobristStruct {
  DWORD dwKey, dwLock0, dwLock1;

  void InitZero(void) {                 // 用零填充Zobrist
    dwKey = dwLock0 = dwLock1 = 0;
  }
  void InitRC4(RC4Struct &rc4) {        // 用密码流填充Zobrist
    dwKey = rc4.NextLong();
    dwLock0 = rc4.NextLong();
    dwLock1 = rc4.NextLong();
  }
  void Xor(const ZobristStruct &zobr) { // 执行XOR操作
    dwKey ^= zobr.dwKey;
    dwLock0 ^= zobr.dwLock0;
    dwLock1 ^= zobr.dwLock1;
  }
  void Xor(const ZobristStruct &zobr1, const ZobristStruct &zobr2) {
    dwKey ^= zobr1.dwKey ^ zobr2.dwKey;
    dwLock0 ^= zobr1.dwLock0 ^ zobr2.dwLock0;
    dwLock1 ^= zobr1.dwLock1 ^ zobr2.dwLock1;
  }
};

// 历史走法信息(占4字节)
struct MoveStruct {
  WORD wmv;
  BYTE ucpcCaptured, ucbCheck;
  DWORD dwKey;

  void Set(int mv, int pcCaptured, BOOL bCheck, DWORD dwKey_) {
    wmv = mv;
    ucpcCaptured = pcCaptured;
    ucbCheck = bCheck;
    dwKey = dwKey_;
  }
}; // mvs

// 局面结构
struct PositionStruct {
  int sdPlayer;                   // 轮到谁走，0=红方，1=黑方
  BYTE ucpcSquares[256];          // 棋盘上的棋子
  int vlWhite, vlBlack;           // 红、黑双方的子力价值
  int nDistance, nMoveNum;        // 距离根节点的步数，历史走法数
  MoveStruct mvsList[MAX_MOVES];  // 历史走法信息列表
  ZobristStruct zobr;             // Zobrist

  void ClearBoard(void) {         // 清空棋盘
    sdPlayer = vlWhite = vlBlack = nDistance = 0;
    memset(ucpcSquares, 0, 256);
    zobr.InitZero();
  }
  void SetIrrev(void) {           // 清空(初始化)历史走法信息
    mvsList[0].Set(0, 0, Checked(), zobr.dwKey);
    nMoveNum = 1;
  }
  void Startup(void);             // 初始化棋盘
  // 以下方法已定义在".cpp"文件中
  void ChangeSide(void);
  void AddPiece(int sq, int pc);
  void DelPiece(int sq, int pc);
  // 以上方法已定义在".cpp"文件中
  int Evaluate(void) const {      // 局面评价函数
    return (sdPlayer == 0 ? vlWhite - vlBlack : vlBlack - vlWhite) + ADVANCED_VALUE;
  }
  BOOL InCheck(void) const {      // 是否被将军
    return mvsList[nMoveNum - 1].ucbCheck;
  }
  BOOL Captured(void) const {     // 上一步是否吃子
    return mvsList[nMoveNum - 1].ucpcCaptured != 0;
  }
  int MovePiece(int mv);                      // 搬一步棋的棋子
  void UndoMovePiece(int mv, int pcCaptured); // 撤消搬一步棋的棋子
  BOOL MakeMove(int mv);                      // 走一步棋
  void UndoMakeMove(void) {                   // 撤消走一步棋
    nDistance --;
    nMoveNum --;
    ChangeSide();
    UndoMovePiece(mvsList[nMoveNum].wmv, mvsList[nMoveNum].ucpcCaptured);
  }
  void NullMove(void) {                       // 走一步空步
    DWORD dwKey;
    dwKey = zobr.dwKey;
    ChangeSide();
    mvsList[nMoveNum].Set(0, 0, FALSE, dwKey);
    nMoveNum ++;
    nDistance ++;
  }
  void UndoNullMove(void) {                   // 撤消走一步空步
    nDistance --;
    nMoveNum --;
    ChangeSide();
  }
  // 生成所有走法，如果"bCapture"为"TRUE"则只生成吃子走法
  int GenerateMoves(int *mvs, BOOL bCapture = FALSE) const;
  BOOL LegalMove(int mv) const;               // 判断走法是否合理
  BOOL Checked(void) const;                   // 判断是否被将军
  BOOL IsMate(void);                          // 判断是否被杀
  int DrawValue(void) const {                 // 和棋分值
    return (nDistance & 1) == 0 ? -DRAW_VALUE : DRAW_VALUE;
  }
  int RepStatus(int nRecur = 1) const;        // 检测重复局面
  int RepValue(int nRepStatus) const {        // 重复局面分值
    int vlReturn;
    vlReturn = ((nRepStatus & 2) == 0 ? 0 : nDistance - BAN_VALUE) +
        ((nRepStatus & 4) == 0 ? 0 : BAN_VALUE - nDistance);
    return vlReturn == 0 ? DrawValue() : vlReturn;
  }
  BOOL NullOkay(void) const {                 // 判断是否允许空步裁剪
    return (sdPlayer == 0 ? vlWhite : vlBlack) > NULL_MARGIN;
  }
  void Mirror(PositionStruct &posMirror) const; // 对局面镜像
};

// 置换表项结构
struct HashItem {
  BYTE ucDepth, ucFlag;
  short svl;
  WORD wmv, wReserved;
  DWORD dwLock0, dwLock1;
};

// 开局库项结构
struct BookItem {
  DWORD dwLock;
  WORD wmv, wvl;
};

/* === 以上是象棋小巫师的引擎代码 === */

/////////////////////////////////////////////////////////////////////////////
// CXQWLightCtrl : See XQWLightCtl.cpp for implementation.

class CXQWLightCtrl : public COleControl
{
	DECLARE_DYNCREATE(CXQWLightCtrl)

// Constructor
public:
	BEGIN_INTERFACE_PART(ObjectSafety, IObjectSafety) 
		STDMETHOD(GetInterfaceSafetyOptions)(REFIID riid, DWORD __RPC_FAR *pdwSupportedOptions, DWORD __RPC_FAR *pdwEnabledOptions); 
		STDMETHOD(SetInterfaceSafetyOptions)(REFIID riid, DWORD dwOptionSetMask, DWORD dwEnabledOptions); 
	END_INTERFACE_PART(ObjectSafety) 

	DECLARE_INTERFACE_MAP(); 

	CXQWLightCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXQWLightCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual BOOL OnSetExtent(LPSIZEL lpSizeL);
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CXQWLightCtrl();

	DECLARE_OLECREATE_EX(CXQWLightCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CXQWLightCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CXQWLightCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CXQWLightCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CXQWLightCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CXQWLightCtrl)
	afx_msg BOOL GetFlip();
	afx_msg void SetFlip(BOOL bNewValue);
	afx_msg void Restart();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

// Event maps
	//{{AFX_EVENT(CXQWLightCtrl)
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CXQWLightCtrl)
	dispidFlip = 1L,
	dispidRestart = 2L,
	//}}AFX_DISP_ID
	};

/* === 以下是象棋小巫师的引擎代码 === */

  PositionStruct pos; // 局面实例

  struct {
    int mvResult;                  // 电脑走的棋
    int nHistoryTable[65536];      // 历史表
    int mvKillers[LIMIT_DEPTH][2]; // 杀手走法表
    HashItem HashTable[HASH_SIZE]; // 置换表
  } Search;
  struct {
    HDC hdc, hdcTmp;                              // 设备句柄，只在"ClickSquare"过程中有效
    int sqSelected, mvLast;                       // 选中的格子，上一步棋
    BOOL bFlipped, bGameOver;                     // 是否翻转棋盘，是否游戏结束(不让继续玩下去)
  } Xqwl;

  int SearchBook(void);
  int ProbeHash(int vlAlpha, int vlBeta, int nDepth, int &mv);
  void RecordHash(int nFlag, int vl, int nDepth, int mv);
  void SetBestMove(int mv, int nDepth);
  int SearchQuiesc(int vlAlpha, int vlBeta);
  int SearchFull(int vlAlpha, int vlBeta, int nDepth, BOOL bNoNull = FALSE);
  int SearchRoot(int nDepth);
  void SearchMain(void);
  void DrawBoard(HDC hdc);
  void MessageBoxMute(LPCSTR lpszText);
  void DrawSquare(int sq, BOOL bSelected = FALSE);
  void ResponseMove(void);
  void ClickSquare(int sq);
  void Startup(void);
/* === 以上是象棋小巫师的引擎代码 === */

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XQWLIGHTCTL_H__FFD433A1_BFCF_4BA6_947F_67756786860C__INCLUDED)
