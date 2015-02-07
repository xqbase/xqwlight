/**
 * XiangQi Wizard Light - A Very Simple Chinese Chess Program
 * Designed by Morning Yellow, Version: 0.6, Last Modified: Mar. 2008
 * Copyright (C) 2004-2008 www.xqbase.com
 *
 * 象棋小巫师 0.6 的目标：
 * 一、实现开局库；
 * 二、实现PVS(主要变例搜索)；
 * 三、把根节点的搜索单独处理，增加搜索的随机性；
 * 四、克服由长将引起的置换表的不稳定性。
 */

#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "resource.h"

// 版本号
const LPCSTR cszAbout = "象棋小巫师 0.6\n象棋百科全书 荣誉出品\n\n"
    "欢迎登录 www.xqbase.com\n免费下载PC版 象棋巫师";

// 窗口和绘图属性
const int WINDOW_STYLES = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX;
const int MASK_COLOR = RGB(0, 255, 0);
const int SQUARE_SIZE = 56;
const int BOARD_EDGE = 8;
const int BOARD_WIDTH = BOARD_EDGE + SQUARE_SIZE * 9 + BOARD_EDGE;
const int BOARD_HEIGHT = BOARD_EDGE + SQUARE_SIZE * 10 + BOARD_EDGE;

// 棋盘范围
const int RANK_TOP = 3;
const int RANK_BOTTOM = 12;
const int FILE_LEFT = 3;
const int FILE_RIGHT = 11;

// 棋子编号
const int PIECE_KING = 0;
const int PIECE_ADVISOR = 1;
const int PIECE_BISHOP = 2;
const int PIECE_KNIGHT = 3;
const int PIECE_ROOK = 4;
const int PIECE_CANNON = 5;
const int PIECE_PAWN = 6;

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

// 判断棋子是否在棋盘中的数组
static const char ccInBoard[256] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// 判断棋子是否在九宫的数组
static const char ccInFort[256] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// 判断步长是否符合特定走法的数组，1=帅(将)，2=仕(士)，3=相(象)
static const char ccLegalSpan[512] = {
                       0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0
};

// 根据步长判断马是否蹩腿的数组
static const char ccKnightPin[512] = {
                              0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,-16,  0,-16,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0, -1,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0, -1,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0, 16,  0, 16,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0
};

// 帅(将)的步长
static const char ccKingDelta[4] = {-16, -1, 1, 16};
// 仕(士)的步长
static const char ccAdvisorDelta[4] = {-17, -15, 15, 17};
// 马的步长，以帅(将)的步长作为马腿
static const char ccKnightDelta[4][2] = {{-33, -31}, {-18, 14}, {-14, 18}, {31, 33}};
// 马被将军的步长，以仕(士)的步长作为马腿
static const char ccKnightCheckDelta[4][2] = {{-33, -18}, {-31, -14}, {14, 31}, {18, 33}};

// 棋盘初始设置
static const BYTE cucpcStartup[256] = {
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0, 20, 19, 18, 17, 16, 17, 18, 19, 20,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0, 21,  0,  0,  0,  0,  0, 21,  0,  0,  0,  0,  0,
  0,  0,  0, 22,  0, 22,  0, 22,  0, 22,  0, 22,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0, 14,  0, 14,  0, 14,  0, 14,  0, 14,  0,  0,  0,  0,
  0,  0,  0,  0, 13,  0,  0,  0,  0,  0, 13,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0, 12, 11, 10,  9,  8,  9, 10, 11, 12,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

// 子力位置价值表
static const BYTE cucvlPiecePos[7][256] = {
  { // 帅(将)
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0, 11, 15, 11,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
  }, { // 仕(士)
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0, 20,  0, 20,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0, 23,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0, 20,  0, 20,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
  }, { // 相(象)
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0, 20,  0,  0,  0, 20,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0, 18,  0,  0,  0, 23,  0,  0,  0, 18,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0, 20,  0,  0,  0, 20,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
  }, { // 马
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0, 90, 90, 90, 96, 90, 96, 90, 90, 90,  0,  0,  0,  0,
    0,  0,  0, 90, 96,103, 97, 94, 97,103, 96, 90,  0,  0,  0,  0,
    0,  0,  0, 92, 98, 99,103, 99,103, 99, 98, 92,  0,  0,  0,  0,
    0,  0,  0, 93,108,100,107,100,107,100,108, 93,  0,  0,  0,  0,
    0,  0,  0, 90,100, 99,103,104,103, 99,100, 90,  0,  0,  0,  0,
    0,  0,  0, 90, 98,101,102,103,102,101, 98, 90,  0,  0,  0,  0,
    0,  0,  0, 92, 94, 98, 95, 98, 95, 98, 94, 92,  0,  0,  0,  0,
    0,  0,  0, 93, 92, 94, 95, 92, 95, 94, 92, 93,  0,  0,  0,  0,
    0,  0,  0, 85, 90, 92, 93, 78, 93, 92, 90, 85,  0,  0,  0,  0,
    0,  0,  0, 88, 85, 90, 88, 90, 88, 90, 85, 88,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
  }, { // 车
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,206,208,207,213,214,213,207,208,206,  0,  0,  0,  0,
    0,  0,  0,206,212,209,216,233,216,209,212,206,  0,  0,  0,  0,
    0,  0,  0,206,208,207,214,216,214,207,208,206,  0,  0,  0,  0,
    0,  0,  0,206,213,213,216,216,216,213,213,206,  0,  0,  0,  0,
    0,  0,  0,208,211,211,214,215,214,211,211,208,  0,  0,  0,  0,
    0,  0,  0,208,212,212,214,215,214,212,212,208,  0,  0,  0,  0,
    0,  0,  0,204,209,204,212,214,212,204,209,204,  0,  0,  0,  0,
    0,  0,  0,198,208,204,212,212,212,204,208,198,  0,  0,  0,  0,
    0,  0,  0,200,208,206,212,200,212,206,208,200,  0,  0,  0,  0,
    0,  0,  0,194,206,204,212,200,212,204,206,194,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
  }, { // 炮
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,100,100, 96, 91, 90, 91, 96,100,100,  0,  0,  0,  0,
    0,  0,  0, 98, 98, 96, 92, 89, 92, 96, 98, 98,  0,  0,  0,  0,
    0,  0,  0, 97, 97, 96, 91, 92, 91, 96, 97, 97,  0,  0,  0,  0,
    0,  0,  0, 96, 99, 99, 98,100, 98, 99, 99, 96,  0,  0,  0,  0,
    0,  0,  0, 96, 96, 96, 96,100, 96, 96, 96, 96,  0,  0,  0,  0,
    0,  0,  0, 95, 96, 99, 96,100, 96, 99, 96, 95,  0,  0,  0,  0,
    0,  0,  0, 96, 96, 96, 96, 96, 96, 96, 96, 96,  0,  0,  0,  0,
    0,  0,  0, 97, 96,100, 99,101, 99,100, 96, 97,  0,  0,  0,  0,
    0,  0,  0, 96, 97, 98, 98, 98, 98, 98, 97, 96,  0,  0,  0,  0,
    0,  0,  0, 96, 96, 97, 99, 99, 99, 97, 96, 96,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
  }, { // 兵(卒)
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  9,  9,  9, 11, 13, 11,  9,  9,  9,  0,  0,  0,  0,
    0,  0,  0, 19, 24, 34, 42, 44, 42, 34, 24, 19,  0,  0,  0,  0,
    0,  0,  0, 19, 24, 32, 37, 37, 37, 32, 24, 19,  0,  0,  0,  0,
    0,  0,  0, 19, 23, 27, 29, 30, 29, 27, 23, 19,  0,  0,  0,  0,
    0,  0,  0, 14, 18, 20, 27, 29, 27, 20, 18, 14,  0,  0,  0,  0,
    0,  0,  0,  7,  0, 13,  0, 16,  0, 13,  0,  7,  0,  0,  0,  0,
    0,  0,  0,  7,  0,  7,  0, 15,  0,  7,  0,  7,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
  }
};

// 判断棋子是否在棋盘中
inline BOOL IN_BOARD(int sq) {
  return ccInBoard[sq] != 0;
}

// 判断棋子是否在九宫中
inline BOOL IN_FORT(int sq) {
  return ccInFort[sq] != 0;
}

// 获得格子的横坐标
inline int RANK_Y(int sq) {
  return sq >> 4;
}

// 获得格子的纵坐标
inline int FILE_X(int sq) {
  return sq & 15;
}

// 根据纵坐标和横坐标获得格子
inline int COORD_XY(int x, int y) {
  return x + (y << 4);
}

// 翻转格子
inline int SQUARE_FLIP(int sq) {
  return 254 - sq;
}

// 纵坐标水平镜像
inline int FILE_FLIP(int x) {
  return 14 - x;
}

// 横坐标垂直镜像
inline int RANK_FLIP(int y) {
  return 15 - y;
}

// 格子水平镜像
inline int MIRROR_SQUARE(int sq) {
  return COORD_XY(FILE_FLIP(FILE_X(sq)), RANK_Y(sq));
}

// 格子水平镜像
inline int SQUARE_FORWARD(int sq, int sd) {
  return sq - 16 + (sd << 5);
}

// 走法是否符合帅(将)的步长
inline BOOL KING_SPAN(int sqSrc, int sqDst) {
  return ccLegalSpan[sqDst - sqSrc + 256] == 1;
}

// 走法是否符合仕(士)的步长
inline BOOL ADVISOR_SPAN(int sqSrc, int sqDst) {
  return ccLegalSpan[sqDst - sqSrc + 256] == 2;
}

// 走法是否符合相(象)的步长
inline BOOL BISHOP_SPAN(int sqSrc, int sqDst) {
  return ccLegalSpan[sqDst - sqSrc + 256] == 3;
}

// 相(象)眼的位置
inline int BISHOP_PIN(int sqSrc, int sqDst) {
  return (sqSrc + sqDst) >> 1;
}

// 马腿的位置
inline int KNIGHT_PIN(int sqSrc, int sqDst) {
  return sqSrc + ccKnightPin[sqDst - sqSrc + 256];
}

// 是否未过河
inline BOOL HOME_HALF(int sq, int sd) {
  return (sq & 0x80) != (sd << 7);
}

// 是否已过河
inline BOOL AWAY_HALF(int sq, int sd) {
  return (sq & 0x80) == (sd << 7);
}

// 是否在河的同一边
inline BOOL SAME_HALF(int sqSrc, int sqDst) {
  return ((sqSrc ^ sqDst) & 0x80) == 0;
}

// 是否在同一行
inline BOOL SAME_RANK(int sqSrc, int sqDst) {
  return ((sqSrc ^ sqDst) & 0xf0) == 0;
}

// 是否在同一列
inline BOOL SAME_FILE(int sqSrc, int sqDst) {
  return ((sqSrc ^ sqDst) & 0x0f) == 0;
}

// 获得红黑标记(红子是8，黑子是16)
inline int SIDE_TAG(int sd) {
  return 8 + (sd << 3);
}

// 获得对方红黑标记
inline int OPP_SIDE_TAG(int sd) {
  return 16 - (sd << 3);
}

// 获得走法的起点
inline int SRC(int mv) {
  return mv & 255;
}

// 获得走法的终点
inline int DST(int mv) {
  return mv >> 8;
}

// 根据起点和终点获得走法
inline int MOVE(int sqSrc, int sqDst) {
  return sqSrc + sqDst * 256;
}

// 走法水平镜像
inline int MIRROR_MOVE(int mv) {
  return MOVE(MIRROR_SQUARE(SRC(mv)), MIRROR_SQUARE(DST(mv)));
}

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

// 用空密钥初始化密码流生成器
void RC4Struct::InitZero(void) {
  int i, j;
  BYTE uc;

  x = y = j = 0;
  for (i = 0; i < 256; i ++) {
    s[i] = i;
  }
  for (i = 0; i < 256; i ++) {
    j = (j + s[i]) & 255;
    uc = s[i];
    s[i] = s[j];
    s[j] = uc;
  }
}

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

// Zobrist表
static struct {
  ZobristStruct Player;
  ZobristStruct Table[14][256];
} Zobrist;

// 初始化Zobrist表
static void InitZobrist(void) {
  int i, j;
  RC4Struct rc4;

  rc4.InitZero();
  Zobrist.Player.InitRC4(rc4);
  for (i = 0; i < 14; i ++) {
    for (j = 0; j < 256; j ++) {
      Zobrist.Table[i][j].InitRC4(rc4);
    }
  }
}

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
  void ChangeSide(void) {         // 交换走子方
    sdPlayer = 1 - sdPlayer;
    zobr.Xor(Zobrist.Player);
  }
  void AddPiece(int sq, int pc) { // 在棋盘上放一枚棋子
    ucpcSquares[sq] = pc;
    // 红方加分，黑方(注意"cucvlPiecePos"取值要颠倒)减分
    if (pc < 16) {
      vlWhite += cucvlPiecePos[pc - 8][sq];
      zobr.Xor(Zobrist.Table[pc - 8][sq]);
    } else {
      vlBlack += cucvlPiecePos[pc - 16][SQUARE_FLIP(sq)];
      zobr.Xor(Zobrist.Table[pc - 9][sq]);
    }
  }
  void DelPiece(int sq, int pc) { // 从棋盘上拿走一枚棋子
    ucpcSquares[sq] = 0;
    // 红方减分，黑方(注意"cucvlPiecePos"取值要颠倒)加分
    if (pc < 16) {
      vlWhite -= cucvlPiecePos[pc - 8][sq];
      zobr.Xor(Zobrist.Table[pc - 8][sq]);
    } else {
      vlBlack -= cucvlPiecePos[pc - 16][SQUARE_FLIP(sq)];
      zobr.Xor(Zobrist.Table[pc - 9][sq]);
    }
  }
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

// 初始化棋盘
void PositionStruct::Startup(void) {
  int sq, pc;
  ClearBoard();
  for (sq = 0; sq < 256; sq ++) {
    pc = cucpcStartup[sq];
    if (pc != 0) {
      AddPiece(sq, pc);
    }
  }
  SetIrrev();
}

// 搬一步棋的棋子
int PositionStruct::MovePiece(int mv) {
  int sqSrc, sqDst, pc, pcCaptured;
  sqSrc = SRC(mv);
  sqDst = DST(mv);
  pcCaptured = ucpcSquares[sqDst];
  if (pcCaptured != 0) {
    DelPiece(sqDst, pcCaptured);
  }
  pc = ucpcSquares[sqSrc];
  DelPiece(sqSrc, pc);
  AddPiece(sqDst, pc);
  return pcCaptured;
}

// 撤消搬一步棋的棋子
void PositionStruct::UndoMovePiece(int mv, int pcCaptured) {
  int sqSrc, sqDst, pc;
  sqSrc = SRC(mv);
  sqDst = DST(mv);
  pc = ucpcSquares[sqDst];
  DelPiece(sqDst, pc);
  AddPiece(sqSrc, pc);
  if (pcCaptured != 0) {
    AddPiece(sqDst, pcCaptured);
  }
}

// 走一步棋
BOOL PositionStruct::MakeMove(int mv) {
  int pcCaptured;
  DWORD dwKey;

  dwKey = zobr.dwKey;
  pcCaptured = MovePiece(mv);
  if (Checked()) {
    UndoMovePiece(mv, pcCaptured);
    return FALSE;
  }
  ChangeSide();
  mvsList[nMoveNum].Set(mv, pcCaptured, Checked(), dwKey);
  nMoveNum ++;
  nDistance ++;
  return TRUE;
}

// "GenerateMoves"参数
const BOOL GEN_CAPTURE = TRUE;

// 生成所有走法，如果"bCapture"为"TRUE"则只生成吃子走法
int PositionStruct::GenerateMoves(int *mvs, BOOL bCapture) const {
  int i, j, nGenMoves, nDelta, sqSrc, sqDst;
  int pcSelfSide, pcOppSide, pcSrc, pcDst;
  // 生成所有走法，需要经过以下几个步骤：

  nGenMoves = 0;
  pcSelfSide = SIDE_TAG(sdPlayer);
  pcOppSide = OPP_SIDE_TAG(sdPlayer);
  for (sqSrc = 0; sqSrc < 256; sqSrc ++) {

    // 1. 找到一个本方棋子，再做以下判断：
    pcSrc = ucpcSquares[sqSrc];
    if ((pcSrc & pcSelfSide) == 0) {
      continue;
    }

    // 2. 根据棋子确定走法
    switch (pcSrc - pcSelfSide) {
    case PIECE_KING:
      for (i = 0; i < 4; i ++) {
        sqDst = sqSrc + ccKingDelta[i];
        if (!IN_FORT(sqDst)) {
          continue;
        }
        pcDst = ucpcSquares[sqDst];
        if (bCapture ? (pcDst & pcOppSide) != 0 : (pcDst & pcSelfSide) == 0) {
          mvs[nGenMoves] = MOVE(sqSrc, sqDst);
          nGenMoves ++;
        }
      }
      break;
    case PIECE_ADVISOR:
      for (i = 0; i < 4; i ++) {
        sqDst = sqSrc + ccAdvisorDelta[i];
        if (!IN_FORT(sqDst)) {
          continue;
        }
        pcDst = ucpcSquares[sqDst];
        if (bCapture ? (pcDst & pcOppSide) != 0 : (pcDst & pcSelfSide) == 0) {
          mvs[nGenMoves] = MOVE(sqSrc, sqDst);
          nGenMoves ++;
        }
      }
      break;
    case PIECE_BISHOP:
      for (i = 0; i < 4; i ++) {
        sqDst = sqSrc + ccAdvisorDelta[i];
        if (!(IN_BOARD(sqDst) && HOME_HALF(sqDst, sdPlayer) && ucpcSquares[sqDst] == 0)) {
          continue;
        }
        sqDst += ccAdvisorDelta[i];
        pcDst = ucpcSquares[sqDst];
        if (bCapture ? (pcDst & pcOppSide) != 0 : (pcDst & pcSelfSide) == 0) {
          mvs[nGenMoves] = MOVE(sqSrc, sqDst);
          nGenMoves ++;
        }
      }
      break;
    case PIECE_KNIGHT:
      for (i = 0; i < 4; i ++) {
        sqDst = sqSrc + ccKingDelta[i];
        if (ucpcSquares[sqDst] != 0) {
          continue;
        }
        for (j = 0; j < 2; j ++) {
          sqDst = sqSrc + ccKnightDelta[i][j];
          if (!IN_BOARD(sqDst)) {
            continue;
          }
          pcDst = ucpcSquares[sqDst];
          if (bCapture ? (pcDst & pcOppSide) != 0 : (pcDst & pcSelfSide) == 0) {
            mvs[nGenMoves] = MOVE(sqSrc, sqDst);
            nGenMoves ++;
          }
        }
      }
      break;
    case PIECE_ROOK:
      for (i = 0; i < 4; i ++) {
        nDelta = ccKingDelta[i];
        sqDst = sqSrc + nDelta;
        while (IN_BOARD(sqDst)) {
          pcDst = ucpcSquares[sqDst];
          if (pcDst == 0) {
            if (!bCapture) {
              mvs[nGenMoves] = MOVE(sqSrc, sqDst);
              nGenMoves ++;
            }
          } else {
            if ((pcDst & pcOppSide) != 0) {
              mvs[nGenMoves] = MOVE(sqSrc, sqDst);
              nGenMoves ++;
            }
            break;
          }
          sqDst += nDelta;
        }
      }
      break;
    case PIECE_CANNON:
      for (i = 0; i < 4; i ++) {
        nDelta = ccKingDelta[i];
        sqDst = sqSrc + nDelta;
        while (IN_BOARD(sqDst)) {
          pcDst = ucpcSquares[sqDst];
          if (pcDst == 0) {
            if (!bCapture) {
              mvs[nGenMoves] = MOVE(sqSrc, sqDst);
              nGenMoves ++;
            }
          } else {
            break;
          }
          sqDst += nDelta;
        }
        sqDst += nDelta;
        while (IN_BOARD(sqDst)) {
          pcDst = ucpcSquares[sqDst];
          if (pcDst != 0) {
            if ((pcDst & pcOppSide) != 0) {
              mvs[nGenMoves] = MOVE(sqSrc, sqDst);
              nGenMoves ++;
            }
            break;
          }
          sqDst += nDelta;
        }
      }
      break;
    case PIECE_PAWN:
      sqDst = SQUARE_FORWARD(sqSrc, sdPlayer);
      if (IN_BOARD(sqDst)) {
        pcDst = ucpcSquares[sqDst];
        if (bCapture ? (pcDst & pcOppSide) != 0 : (pcDst & pcSelfSide) == 0) {
          mvs[nGenMoves] = MOVE(sqSrc, sqDst);
          nGenMoves ++;
        }
      }
      if (AWAY_HALF(sqSrc, sdPlayer)) {
        for (nDelta = -1; nDelta <= 1; nDelta += 2) {
          sqDst = sqSrc + nDelta;
          if (IN_BOARD(sqDst)) {
            pcDst = ucpcSquares[sqDst];
            if (bCapture ? (pcDst & pcOppSide) != 0 : (pcDst & pcSelfSide) == 0) {
              mvs[nGenMoves] = MOVE(sqSrc, sqDst);
              nGenMoves ++;
            }
          }
        }
      }
      break;
    }
  }
  return nGenMoves;
}

// 判断走法是否合理
BOOL PositionStruct::LegalMove(int mv) const {
  int sqSrc, sqDst, sqPin;
  int pcSelfSide, pcSrc, pcDst, nDelta;
  // 判断走法是否合法，需要经过以下的判断过程：

  // 1. 判断起始格是否有自己的棋子
  sqSrc = SRC(mv);
  pcSrc = ucpcSquares[sqSrc];
  pcSelfSide = SIDE_TAG(sdPlayer);
  if ((pcSrc & pcSelfSide) == 0) {
    return FALSE;
  }

  // 2. 判断目标格是否有自己的棋子
  sqDst = DST(mv);
  pcDst = ucpcSquares[sqDst];
  if ((pcDst & pcSelfSide) != 0) {
    return FALSE;
  }

  // 3. 根据棋子的类型检查走法是否合理
  switch (pcSrc - pcSelfSide) {
  case PIECE_KING:
    return IN_FORT(sqDst) && KING_SPAN(sqSrc, sqDst);
  case PIECE_ADVISOR:
    return IN_FORT(sqDst) && ADVISOR_SPAN(sqSrc, sqDst);
  case PIECE_BISHOP:
    return SAME_HALF(sqSrc, sqDst) && BISHOP_SPAN(sqSrc, sqDst) &&
        ucpcSquares[BISHOP_PIN(sqSrc, sqDst)] == 0;
  case PIECE_KNIGHT:
    sqPin = KNIGHT_PIN(sqSrc, sqDst);
    return sqPin != sqSrc && ucpcSquares[sqPin] == 0;
  case PIECE_ROOK:
  case PIECE_CANNON:
    if (SAME_RANK(sqSrc, sqDst)) {
      nDelta = (sqDst < sqSrc ? -1 : 1);
    } else if (SAME_FILE(sqSrc, sqDst)) {
      nDelta = (sqDst < sqSrc ? -16 : 16);
    } else {
      return FALSE;
    }
    sqPin = sqSrc + nDelta;
    while (sqPin != sqDst && ucpcSquares[sqPin] == 0) {
      sqPin += nDelta;
    }
    if (sqPin == sqDst) {
      return pcDst == 0 || pcSrc - pcSelfSide == PIECE_ROOK;
    } else if (pcDst != 0 && pcSrc - pcSelfSide == PIECE_CANNON) {
      sqPin += nDelta;
      while (sqPin != sqDst && ucpcSquares[sqPin] == 0) {
        sqPin += nDelta;
      }
      return sqPin == sqDst;
    } else {
      return FALSE;
    }
  case PIECE_PAWN:
    if (AWAY_HALF(sqDst, sdPlayer) && (sqDst == sqSrc - 1 || sqDst == sqSrc + 1)) {
      return TRUE;
    }
    return sqDst == SQUARE_FORWARD(sqSrc, sdPlayer);
  default:
    return FALSE;
  }
}

// 判断是否被将军
BOOL PositionStruct::Checked() const {
  int i, j, sqSrc, sqDst;
  int pcSelfSide, pcOppSide, pcDst, nDelta;
  pcSelfSide = SIDE_TAG(sdPlayer);
  pcOppSide = OPP_SIDE_TAG(sdPlayer);
  // 找到棋盘上的帅(将)，再做以下判断：

  for (sqSrc = 0; sqSrc < 256; sqSrc ++) {
    if (ucpcSquares[sqSrc] != pcSelfSide + PIECE_KING) {
      continue;
    }

    // 1. 判断是否被对方的兵(卒)将军
    if (ucpcSquares[SQUARE_FORWARD(sqSrc, sdPlayer)] == pcOppSide + PIECE_PAWN) {
      return TRUE;
    }
    for (nDelta = -1; nDelta <= 1; nDelta += 2) {
      if (ucpcSquares[sqSrc + nDelta] == pcOppSide + PIECE_PAWN) {
        return TRUE;
      }
    }

    // 2. 判断是否被对方的马将军(以仕(士)的步长当作马腿)
    for (i = 0; i < 4; i ++) {
      if (ucpcSquares[sqSrc + ccAdvisorDelta[i]] != 0) {
        continue;
      }
      for (j = 0; j < 2; j ++) {
        pcDst = ucpcSquares[sqSrc + ccKnightCheckDelta[i][j]];
        if (pcDst == pcOppSide + PIECE_KNIGHT) {
          return TRUE;
        }
      }
    }

    // 3. 判断是否被对方的车或炮将军(包括将帅对脸)
    for (i = 0; i < 4; i ++) {
      nDelta = ccKingDelta[i];
      sqDst = sqSrc + nDelta;
      while (IN_BOARD(sqDst)) {
        pcDst = ucpcSquares[sqDst];
        if (pcDst != 0) {
          if (pcDst == pcOppSide + PIECE_ROOK || pcDst == pcOppSide + PIECE_KING) {
            return TRUE;
          }
          break;
        }
        sqDst += nDelta;
      }
      sqDst += nDelta;
      while (IN_BOARD(sqDst)) {
        int pcDst = ucpcSquares[sqDst];
        if (pcDst != 0) {
          if (pcDst == pcOppSide + PIECE_CANNON) {
            return TRUE;
          }
          break;
        }
        sqDst += nDelta;
      }
    }
    return FALSE;
  }
  return FALSE;
}

// 判断是否被杀
BOOL PositionStruct::IsMate(void) {
  int i, nGenMoveNum, pcCaptured;
  int mvs[MAX_GEN_MOVES];

  nGenMoveNum = GenerateMoves(mvs);
  for (i = 0; i < nGenMoveNum; i ++) {
    pcCaptured = MovePiece(mvs[i]);
    if (!Checked()) {
      UndoMovePiece(mvs[i], pcCaptured);
      return FALSE;
    } else {
      UndoMovePiece(mvs[i], pcCaptured);
    }
  }
  return TRUE;
}

// 检测重复局面
int PositionStruct::RepStatus(int nRecur) const {
  BOOL bSelfSide, bPerpCheck, bOppPerpCheck;
  const MoveStruct *lpmvs;

  bSelfSide = FALSE;
  bPerpCheck = bOppPerpCheck = TRUE;
  lpmvs = mvsList + nMoveNum - 1;
  while (lpmvs->wmv != 0 && lpmvs->ucpcCaptured == 0) {
    if (bSelfSide) {
      bPerpCheck = bPerpCheck && lpmvs->ucbCheck;
      if (lpmvs->dwKey == zobr.dwKey) {
        nRecur --;
        if (nRecur == 0) {
          return 1 + (bPerpCheck ? 2 : 0) + (bOppPerpCheck ? 4 : 0);
        }
      }
    } else {
      bOppPerpCheck = bOppPerpCheck && lpmvs->ucbCheck;
    }
    bSelfSide = !bSelfSide;
    lpmvs --;
  }
  return 0;
}

// 对局面镜像
void PositionStruct::Mirror(PositionStruct &posMirror) const {
  int sq, pc;
  posMirror.ClearBoard();
  for (sq = 0; sq < 256; sq ++) {
    pc = ucpcSquares[sq];
    if (pc != 0) {
      posMirror.AddPiece(MIRROR_SQUARE(sq), pc);
    }
  }
  if (sdPlayer == 1) {
    posMirror.ChangeSide();
  }
  posMirror.SetIrrev();
}

static PositionStruct pos; // 局面实例

// 与图形界面有关的全局变量
static struct {
  HINSTANCE hInst;                              // 应用程序句柄实例
  HWND hWnd;                                    // 主窗口句柄
  HDC hdc, hdcTmp;                              // 设备句柄，只在"ClickSquare"过程中有效
  HBITMAP bmpBoard, bmpSelected, bmpPieces[24]; // 资源图片句柄
  int sqSelected, mvLast;                       // 选中的格子，上一步棋
  BOOL bFlipped, bGameOver;                     // 是否翻转棋盘，是否游戏结束(不让继续玩下去)
} Xqwl;

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

// 与搜索有关的全局变量
static struct {
  int mvResult;                  // 电脑走的棋
  int nHistoryTable[65536];      // 历史表
  int mvKillers[LIMIT_DEPTH][2]; // 杀手走法表
  HashItem HashTable[HASH_SIZE]; // 置换表
  int nBookSize;                 // 开局库大小
  BookItem BookTable[BOOK_SIZE]; // 开局库
} Search;

// 装入开局库
static void LoadBook(void) {
  HRSRC hrsrc;
  hrsrc = FindResource(Xqwl.hInst, "BOOK_DATA", RT_RCDATA);
  Search.nBookSize = SizeofResource(Xqwl.hInst, hrsrc) / sizeof(BookItem);
  if (Search.nBookSize > BOOK_SIZE) {
    Search.nBookSize = BOOK_SIZE;
  }
  memcpy(Search.BookTable, LockResource(LoadResource(Xqwl.hInst, hrsrc)),
      Search.nBookSize * sizeof(BookItem));
}

static int CompareBook(const void *lpbk1, const void *lpbk2) {
  DWORD dw1, dw2;
  dw1 = ((BookItem *) lpbk1)->dwLock;
  dw2 = ((BookItem *) lpbk2)->dwLock;
  return dw1 > dw2 ? 1 : dw1 < dw2 ? -1 : 0;
}

// 搜索开局库
static int SearchBook(void) {
  int i, vl, nBookMoves, mv;
  int mvs[MAX_GEN_MOVES], vls[MAX_GEN_MOVES];
  BOOL bMirror;
  BookItem bkToSearch, *lpbk;
  PositionStruct posMirror;
  // 搜索开局库的过程有以下几个步骤

  // 1. 如果没有开局库，则立即返回
  if (Search.nBookSize == 0) {
    return 0;
  }
  // 2. 搜索当前局面
  bMirror = FALSE;
  bkToSearch.dwLock = pos.zobr.dwLock1;
  lpbk = (BookItem *) bsearch(&bkToSearch, Search.BookTable, Search.nBookSize, sizeof(BookItem), CompareBook);
  // 3. 如果没有找到，那么搜索当前局面的镜像局面
  if (lpbk == NULL) {
    bMirror = TRUE;
    pos.Mirror(posMirror);
    bkToSearch.dwLock = posMirror.zobr.dwLock1;
    lpbk = (BookItem *) bsearch(&bkToSearch, Search.BookTable, Search.nBookSize, sizeof(BookItem), CompareBook);
  }
  // 4. 如果镜像局面也没找到，则立即返回
  if (lpbk == NULL) {
    return 0;
  }
  // 5. 如果找到，则向前查第一个开局库项
  while (lpbk >= Search.BookTable && lpbk->dwLock == bkToSearch.dwLock) {
    lpbk --;
  }
  lpbk ++;
  // 6. 把走法和分值写入到"mvs"和"vls"数组中
  vl = nBookMoves = 0;
  while (lpbk < Search.BookTable + Search.nBookSize && lpbk->dwLock == bkToSearch.dwLock) {
    mv = (bMirror ? MIRROR_MOVE(lpbk->wmv) : lpbk->wmv);
    if (pos.LegalMove(mv)) {
      mvs[nBookMoves] = mv;
      vls[nBookMoves] = lpbk->wvl;
      vl += vls[nBookMoves];
      nBookMoves ++;
      if (nBookMoves == MAX_GEN_MOVES) {
        break; // 防止"BOOK.DAT"中含有异常数据
      }
    }
    lpbk ++;
  }
  if (vl == 0) {
    return 0; // 防止"BOOK.DAT"中含有异常数据
  }
  // 7. 根据权重随机选择一个走法
  vl = rand() % vl;
  for (i = 0; i < nBookMoves; i ++) {
    vl -= vls[i];
    if (vl < 0) {
      break;
    }
  }
  return mvs[i];
}

// 提取置换表项
static int ProbeHash(int vlAlpha, int vlBeta, int nDepth, int &mv) {
  BOOL bMate; // 杀棋标志：如果是杀棋，那么不需要满足深度条件
  HashItem hsh;

  hsh = Search.HashTable[pos.zobr.dwKey & (HASH_SIZE - 1)];
  if (hsh.dwLock0 != pos.zobr.dwLock0 || hsh.dwLock1 != pos.zobr.dwLock1) {
    mv = 0;
    return -MATE_VALUE;
  }
  mv = hsh.wmv;
  bMate = FALSE;
  if (hsh.svl > WIN_VALUE) {
    if (hsh.svl < BAN_VALUE) {
      return -MATE_VALUE; // 可能导致搜索的不稳定性，立刻退出，但最佳着法可能拿到
    }
    hsh.svl -= pos.nDistance;
    bMate = TRUE;
  } else if (hsh.svl < -WIN_VALUE) {
    if (hsh.svl > -BAN_VALUE) {
      return -MATE_VALUE; // 同上
    }
    hsh.svl += pos.nDistance;
    bMate = TRUE;
  }
  if (hsh.ucDepth >= nDepth || bMate) {
    if (hsh.ucFlag == HASH_BETA) {
      return (hsh.svl >= vlBeta ? hsh.svl : -MATE_VALUE);
    } else if (hsh.ucFlag == HASH_ALPHA) {
      return (hsh.svl <= vlAlpha ? hsh.svl : -MATE_VALUE);
    }
    return hsh.svl;
  }
  return -MATE_VALUE;
};

// 保存置换表项
static void RecordHash(int nFlag, int vl, int nDepth, int mv) {
  HashItem hsh;
  hsh = Search.HashTable[pos.zobr.dwKey & (HASH_SIZE - 1)];
  if (hsh.ucDepth > nDepth) {
    return;
  }
  hsh.ucFlag = nFlag;
  hsh.ucDepth = nDepth;
  if (vl > WIN_VALUE) {
    if (mv == 0 && vl <= BAN_VALUE) {
      return; // 可能导致搜索的不稳定性，并且没有最佳着法，立刻退出
    }
    hsh.svl = vl + pos.nDistance;
  } else if (vl < -WIN_VALUE) {
    if (mv == 0 && vl >= -BAN_VALUE) {
      return; // 同上
    }
    hsh.svl = vl - pos.nDistance;
  } else {
    hsh.svl = vl;
  }
  hsh.wmv = mv;
  hsh.dwLock0 = pos.zobr.dwLock0;
  hsh.dwLock1 = pos.zobr.dwLock1;
  Search.HashTable[pos.zobr.dwKey & (HASH_SIZE - 1)] = hsh;
};

// MVV/LVA每种子力的价值
static BYTE cucMvvLva[24] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  5, 1, 1, 3, 4, 3, 2, 0,
  5, 1, 1, 3, 4, 3, 2, 0
};

// 求MVV/LVA值
inline int MvvLva(int mv) {
  return (cucMvvLva[pos.ucpcSquares[DST(mv)]] << 3) - cucMvvLva[pos.ucpcSquares[SRC(mv)]];
}

// "qsort"按MVV/LVA值排序的比较函数
static int CompareMvvLva(const void *lpmv1, const void *lpmv2) {
  return MvvLva(*(int *) lpmv2) - MvvLva(*(int *) lpmv1);
}

// "qsort"按历史表排序的比较函数
static int CompareHistory(const void *lpmv1, const void *lpmv2) {
  return Search.nHistoryTable[*(int *) lpmv2] - Search.nHistoryTable[*(int *) lpmv1];
}


// 走法排序阶段
const int PHASE_HASH = 0;
const int PHASE_KILLER_1 = 1;
const int PHASE_KILLER_2 = 2;
const int PHASE_GEN_MOVES = 3;
const int PHASE_REST = 4;

// 走法排序结构
struct SortStruct {
  int mvHash, mvKiller1, mvKiller2; // 置换表走法和两个杀手走法
  int nPhase, nIndex, nGenMoves;    // 当前阶段，当前采用第几个走法，总共有几个走法
  int mvs[MAX_GEN_MOVES];           // 所有的走法

  void Init(int mvHash_) { // 初始化，设定置换表走法和两个杀手走法
    mvHash = mvHash_;
    mvKiller1 = Search.mvKillers[pos.nDistance][0];
    mvKiller2 = Search.mvKillers[pos.nDistance][1];
    nPhase = PHASE_HASH;
  }
  int Next(void); // 得到下一个走法
};

// 得到下一个走法
int SortStruct::Next(void) {
  int mv;
  switch (nPhase) {
  // "nPhase"表示着法启发的若干阶段，依次为：

  // 0. 置换表着法启发，完成后立即进入下一阶段；
  case PHASE_HASH:
    nPhase = PHASE_KILLER_1;
    if (mvHash != 0) {
      return mvHash;
    }
    // 技巧：这里没有"break"，表示"switch"的上一个"case"执行完后紧接着做下一个"case"，下同

  // 1. 杀手着法启发(第一个杀手着法)，完成后立即进入下一阶段；
  case PHASE_KILLER_1:
    nPhase = PHASE_KILLER_2;
    if (mvKiller1 != mvHash && mvKiller1 != 0 && pos.LegalMove(mvKiller1)) {
      return mvKiller1;
    }

  // 2. 杀手着法启发(第二个杀手着法)，完成后立即进入下一阶段；
  case PHASE_KILLER_2:
    nPhase = PHASE_GEN_MOVES;
    if (mvKiller2 != mvHash && mvKiller2 != 0 && pos.LegalMove(mvKiller2)) {
      return mvKiller2;
    }

  // 3. 生成所有着法，完成后立即进入下一阶段；
  case PHASE_GEN_MOVES:
    nPhase = PHASE_REST;
    nGenMoves = pos.GenerateMoves(mvs);
    qsort(mvs, nGenMoves, sizeof(int), CompareHistory);
    nIndex = 0;

  // 4. 对剩余着法做历史表启发；
  case PHASE_REST:
    while (nIndex < nGenMoves) {
      mv = mvs[nIndex];
      nIndex ++;
      if (mv != mvHash && mv != mvKiller1 && mv != mvKiller2) {
        return mv;
      }
    }

  // 5. 没有着法了，返回零。
  default:
    return 0;
  }
}

// 对最佳走法的处理
inline void SetBestMove(int mv, int nDepth) {
  int *lpmvKillers;
  Search.nHistoryTable[mv] += nDepth * nDepth;
  lpmvKillers = Search.mvKillers[pos.nDistance];
  if (lpmvKillers[0] != mv) {
    lpmvKillers[1] = lpmvKillers[0];
    lpmvKillers[0] = mv;
  }
}

// 静态(Quiescence)搜索过程
static int SearchQuiesc(int vlAlpha, int vlBeta) {
  int i, nGenMoves;
  int vl, vlBest;
  int mvs[MAX_GEN_MOVES];
  // 一个静态搜索分为以下几个阶段

  // 1. 检查重复局面
  vl = pos.RepStatus();
  if (vl != 0) {
    return pos.RepValue(vl);
  }

  // 2. 到达极限深度就返回局面评价
  if (pos.nDistance == LIMIT_DEPTH) {
    return pos.Evaluate();
  }

  // 3. 初始化最佳值
  vlBest = -MATE_VALUE; // 这样可以知道，是否一个走法都没走过(杀棋)

  if (pos.InCheck()) {
    // 4. 如果被将军，则生成全部走法
    nGenMoves = pos.GenerateMoves(mvs);
    qsort(mvs, nGenMoves, sizeof(int), CompareHistory);
  } else {

    // 5. 如果不被将军，先做局面评价
    vl = pos.Evaluate();
    if (vl > vlBest) {
      vlBest = vl;
      if (vl >= vlBeta) {
        return vl;
      }
      if (vl > vlAlpha) {
        vlAlpha = vl;
      }
    }

    // 6. 如果局面评价没有截断，再生成吃子走法
    nGenMoves = pos.GenerateMoves(mvs, GEN_CAPTURE);
    qsort(mvs, nGenMoves, sizeof(int), CompareMvvLva);
  }

  // 7. 逐一走这些走法，并进行递归
  for (i = 0; i < nGenMoves; i ++) {
    if (pos.MakeMove(mvs[i])) {
      vl = -SearchQuiesc(-vlBeta, -vlAlpha);
      pos.UndoMakeMove();

      // 8. 进行Alpha-Beta大小判断和截断
      if (vl > vlBest) {    // 找到最佳值(但不能确定是Alpha、PV还是Beta走法)
        vlBest = vl;        // "vlBest"就是目前要返回的最佳值，可能超出Alpha-Beta边界
        if (vl >= vlBeta) { // 找到一个Beta走法
          return vl;        // Beta截断
        }
        if (vl > vlAlpha) { // 找到一个PV走法
          vlAlpha = vl;     // 缩小Alpha-Beta边界
        }
      }
    }
  }

  // 9. 所有走法都搜索完了，返回最佳值
  return vlBest == -MATE_VALUE ? pos.nDistance - MATE_VALUE : vlBest;
}

// "SearchFull"的参数
const BOOL NO_NULL = TRUE;

// 超出边界(Fail-Soft)的Alpha-Beta搜索过程
static int SearchFull(int vlAlpha, int vlBeta, int nDepth, BOOL bNoNull = FALSE) {
  int nHashFlag, vl, vlBest;
  int mv, mvBest, mvHash, nNewDepth;
  SortStruct Sort;
  // 一个Alpha-Beta完全搜索分为以下几个阶段

  // 1. 到达水平线，则调用静态搜索(注意：由于空步裁剪，深度可能小于零)
  if (nDepth <= 0) {
    return SearchQuiesc(vlAlpha, vlBeta);
  }

  // 1-1. 检查重复局面(注意：不要在根节点检查，否则就没有走法了)
  vl = pos.RepStatus();
  if (vl != 0) {
    return pos.RepValue(vl);
  }

  // 1-2. 到达极限深度就返回局面评价
  if (pos.nDistance == LIMIT_DEPTH) {
    return pos.Evaluate();
  }

  // 1-3. 尝试置换表裁剪，并得到置换表走法
  vl = ProbeHash(vlAlpha, vlBeta, nDepth, mvHash);
  if (vl > -MATE_VALUE) {
    return vl;
  }

  // 1-4. 尝试空步裁剪(根节点的Beta值是"MATE_VALUE"，所以不可能发生空步裁剪)
  if (!bNoNull && !pos.InCheck() && pos.NullOkay()) {
    pos.NullMove();
    vl = -SearchFull(-vlBeta, 1 - vlBeta, nDepth - NULL_DEPTH - 1, NO_NULL);
    pos.UndoNullMove();
    if (vl >= vlBeta) {
      return vl;
    }
  }

  // 2. 初始化最佳值和最佳走法
  nHashFlag = HASH_ALPHA;
  vlBest = -MATE_VALUE; // 这样可以知道，是否一个走法都没走过(杀棋)
  mvBest = 0;           // 这样可以知道，是否搜索到了Beta走法或PV走法，以便保存到历史表

  // 3. 初始化走法排序结构
  Sort.Init(mvHash);

  // 4. 逐一走这些走法，并进行递归
  while ((mv = Sort.Next()) != 0) {
    if (pos.MakeMove(mv)) {
      // 将军延伸
      nNewDepth = pos.InCheck() ? nDepth : nDepth - 1;
      // PVS
      if (vlBest == -MATE_VALUE) {
        vl = -SearchFull(-vlBeta, -vlAlpha, nNewDepth);
      } else {
        vl = -SearchFull(-vlAlpha - 1, -vlAlpha, nNewDepth);
        if (vl > vlAlpha && vl < vlBeta) {
          vl = -SearchFull(-vlBeta, -vlAlpha, nNewDepth);
        }
      }
      pos.UndoMakeMove();

      // 5. 进行Alpha-Beta大小判断和截断
      if (vl > vlBest) {    // 找到最佳值(但不能确定是Alpha、PV还是Beta走法)
        vlBest = vl;        // "vlBest"就是目前要返回的最佳值，可能超出Alpha-Beta边界
        if (vl >= vlBeta) { // 找到一个Beta走法
          nHashFlag = HASH_BETA;
          mvBest = mv;      // Beta走法要保存到历史表
          break;            // Beta截断
        }
        if (vl > vlAlpha) { // 找到一个PV走法
          nHashFlag = HASH_PV;
          mvBest = mv;      // PV走法要保存到历史表
          vlAlpha = vl;     // 缩小Alpha-Beta边界
        }
      }
    }
  }

  // 5. 所有走法都搜索完了，把最佳走法(不能是Alpha走法)保存到历史表，返回最佳值
  if (vlBest == -MATE_VALUE) {
    // 如果是杀棋，就根据杀棋步数给出评价
    return pos.nDistance - MATE_VALUE;
  }
  // 记录到置换表
  RecordHash(nHashFlag, vlBest, nDepth, mvBest);
  if (mvBest != 0) {
    // 如果不是Alpha走法，就将最佳走法保存到历史表
    SetBestMove(mvBest, nDepth);
  }
  return vlBest;
}

// 根节点的Alpha-Beta搜索过程
static int SearchRoot(int nDepth) {
  int vl, vlBest, mv, nNewDepth;
  SortStruct Sort;

  vlBest = -MATE_VALUE;
  Sort.Init(Search.mvResult);
  while ((mv = Sort.Next()) != 0) {
    if (pos.MakeMove(mv)) {
      nNewDepth = pos.InCheck() ? nDepth : nDepth - 1;
      if (vlBest == -MATE_VALUE) {
        vl = -SearchFull(-MATE_VALUE, MATE_VALUE, nNewDepth, NO_NULL);
      } else {
        vl = -SearchFull(-vlBest - 1, -vlBest, nNewDepth);
        if (vl > vlBest) {
          vl = -SearchFull(-MATE_VALUE, -vlBest, nNewDepth, NO_NULL);
        }
      }
      pos.UndoMakeMove();
      if (vl > vlBest) {
        vlBest = vl;
        Search.mvResult = mv;
        if (vlBest > -WIN_VALUE && vlBest < WIN_VALUE) {
          vlBest += (rand() & RANDOM_MASK) - (rand() & RANDOM_MASK);
        }
      }
    }
  }
  RecordHash(HASH_PV, vlBest, nDepth, Search.mvResult);
  SetBestMove(Search.mvResult, nDepth);
  return vlBest;
}

// 迭代加深搜索过程
static void SearchMain(void) {
  int i, t, vl, nGenMoves;
  int mvs[MAX_GEN_MOVES];

  // 初始化
  memset(Search.nHistoryTable, 0, 65536 * sizeof(int));       // 清空历史表
  memset(Search.mvKillers, 0, LIMIT_DEPTH * 2 * sizeof(int)); // 清空杀手走法表
  memset(Search.HashTable, 0, HASH_SIZE * sizeof(HashItem));  // 清空置换表
  t = clock();       // 初始化定时器
  pos.nDistance = 0; // 初始步数

  // 搜索开局库
  Search.mvResult = SearchBook();
  if (Search.mvResult != 0) {
    pos.MakeMove(Search.mvResult);
    if (pos.RepStatus(3) == 0) {
      pos.UndoMakeMove();
      return;
    }
    pos.UndoMakeMove();
  }

  // 检查是否只有唯一走法
  vl = 0;
  nGenMoves = pos.GenerateMoves(mvs);
  for (i = 0; i < nGenMoves; i ++) {
    if (pos.MakeMove(mvs[i])) {
      pos.UndoMakeMove();
      Search.mvResult = mvs[i];
      vl ++;
    }
  }
  if (vl == 1) {
    return;
  }

  // 迭代加深过程
  for (i = 1; i <= LIMIT_DEPTH; i ++) {
    vl = SearchRoot(i);
    // 搜索到杀棋，就终止搜索
    if (vl > WIN_VALUE || vl < -WIN_VALUE) {
      break;
    }
    // 超过一秒，就终止搜索
    if (clock() - t > CLOCKS_PER_SEC) {
      break;
    }
  }
}

// TransparentBlt 的替代函数，用来修正原函数在 Windows 98 下资源泄漏的问题
static void TransparentBlt2(HDC hdcDest, int nXOriginDest, int nYOriginDest, int nWidthDest, int nHeightDest,
    HDC hdcSrc, int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc, UINT crTransparent) {
  HDC hImageDC, hMaskDC;
  HBITMAP hOldImageBMP, hImageBMP, hOldMaskBMP, hMaskBMP;

  hImageBMP = CreateCompatibleBitmap(hdcDest, nWidthDest, nHeightDest);
  hMaskBMP = CreateBitmap(nWidthDest, nHeightDest, 1, 1, NULL);
  hImageDC = CreateCompatibleDC(hdcDest);
  hMaskDC = CreateCompatibleDC(hdcDest);
  hOldImageBMP = (HBITMAP) SelectObject(hImageDC, hImageBMP);
  hOldMaskBMP = (HBITMAP) SelectObject(hMaskDC, hMaskBMP);

  if (nWidthDest == nWidthSrc && nHeightDest == nHeightSrc) {
    BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest,
        hdcSrc, nXOriginSrc, nYOriginSrc, SRCCOPY);
  } else {
    StretchBlt(hImageDC, 0, 0, nWidthDest, nHeightDest,
        hdcSrc, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc, SRCCOPY);
  }
  SetBkColor(hImageDC, crTransparent);
  BitBlt(hMaskDC, 0, 0, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCCOPY);
  SetBkColor(hImageDC, RGB(0,0,0));
  SetTextColor(hImageDC, RGB(255,255,255));
  BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);
  SetBkColor(hdcDest, RGB(255,255,255));
  SetTextColor(hdcDest, RGB(0,0,0));
  BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest,
      hMaskDC, 0, 0, SRCAND);
  BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest,
      hImageDC, 0, 0, SRCPAINT);

  SelectObject(hImageDC, hOldImageBMP);
  DeleteDC(hImageDC);
  SelectObject(hMaskDC, hOldMaskBMP);
  DeleteDC(hMaskDC);
  DeleteObject(hImageBMP);
  DeleteObject(hMaskBMP);
}

// 绘制透明图片
inline void DrawTransBmp(HDC hdc, HDC hdcTmp, int xx, int yy, HBITMAP bmp) {
  SelectObject(hdcTmp, bmp);
  TransparentBlt2(hdc, xx, yy, SQUARE_SIZE, SQUARE_SIZE, hdcTmp, 0, 0, SQUARE_SIZE, SQUARE_SIZE, MASK_COLOR);
}

// 绘制棋盘
static void DrawBoard(HDC hdc) {
  int x, y, xx, yy, sq, pc;
  HDC hdcTmp;

  // 画棋盘
  hdcTmp = CreateCompatibleDC(hdc);
  SelectObject(hdcTmp, Xqwl.bmpBoard);
  BitBlt(hdc, 0, 0, BOARD_WIDTH, BOARD_HEIGHT, hdcTmp, 0, 0, SRCCOPY);
  // 画棋子
  for (x = FILE_LEFT; x <= FILE_RIGHT; x ++) {
    for (y = RANK_TOP; y <= RANK_BOTTOM; y ++) {
      if (Xqwl.bFlipped) {
        xx = BOARD_EDGE + (FILE_FLIP(x) - FILE_LEFT) * SQUARE_SIZE;
        yy = BOARD_EDGE + (RANK_FLIP(y) - RANK_TOP) * SQUARE_SIZE;
      } else {
        xx = BOARD_EDGE + (x - FILE_LEFT) * SQUARE_SIZE;
        yy = BOARD_EDGE + (y - RANK_TOP) * SQUARE_SIZE;
      }
      sq = COORD_XY(x, y);
      pc = pos.ucpcSquares[sq];
      if (pc != 0) {
        DrawTransBmp(hdc, hdcTmp, xx, yy, Xqwl.bmpPieces[pc]);
      }
      if (sq == Xqwl.sqSelected || sq == SRC(Xqwl.mvLast) || sq == DST(Xqwl.mvLast)) {
        DrawTransBmp(hdc, hdcTmp, xx, yy, Xqwl.bmpSelected);
      }
    }
  }
  DeleteDC(hdcTmp);
}

// 播放资源声音
inline void PlayResWav(int nResId) {
  PlaySound(MAKEINTRESOURCE(nResId), Xqwl.hInst, SND_ASYNC | SND_NOWAIT | SND_RESOURCE);
}

// 弹出不带声音的提示框
static void MessageBoxMute(LPCSTR lpszText) {
  MSGBOXPARAMS mbp;
  mbp.cbSize = sizeof(MSGBOXPARAMS);
  mbp.hwndOwner = Xqwl.hWnd;
  mbp.hInstance = NULL;
  mbp.lpszText = lpszText;
  mbp.lpszCaption = "象棋小巫师";
  mbp.dwStyle = MB_USERICON;
  mbp.lpszIcon = MAKEINTRESOURCE(IDI_INFORMATION);
  mbp.dwContextHelpId = 0;
  mbp.lpfnMsgBoxCallback = NULL;
  mbp.dwLanguageId = 0;
  if (MessageBoxIndirect(&mbp) == 0) {
    // 系统图标在 Windows 98 下会失败，所以要使用应用程序图标
    mbp.hInstance = Xqwl.hInst;
    mbp.lpszIcon = MAKEINTRESOURCE(IDI_APPICON);
    MessageBoxIndirect(&mbp);
  }
}

// "DrawSquare"参数
const BOOL DRAW_SELECTED = TRUE;

// 绘制格子
static void DrawSquare(int sq, BOOL bSelected = FALSE) {
  int sqFlipped, xx, yy, pc;

  sqFlipped = Xqwl.bFlipped ? SQUARE_FLIP(sq) : sq;
  xx = BOARD_EDGE + (FILE_X(sqFlipped) - FILE_LEFT) * SQUARE_SIZE;
  yy = BOARD_EDGE + (RANK_Y(sqFlipped) - RANK_TOP) * SQUARE_SIZE;
  SelectObject(Xqwl.hdcTmp, Xqwl.bmpBoard);
  BitBlt(Xqwl.hdc, xx, yy, SQUARE_SIZE, SQUARE_SIZE, Xqwl.hdcTmp, xx, yy, SRCCOPY);
  pc = pos.ucpcSquares[sq];
  if (pc != 0) {
    DrawTransBmp(Xqwl.hdc, Xqwl.hdcTmp, xx, yy, Xqwl.bmpPieces[pc]);
  }
  if (bSelected) {
    DrawTransBmp(Xqwl.hdc, Xqwl.hdcTmp, xx, yy, Xqwl.bmpSelected);
  }
}

// 电脑回应一步棋
static void ResponseMove(void) {
  int vlRep;
  // 电脑走一步棋
  SetCursor((HCURSOR) LoadImage(NULL, IDC_WAIT, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
  SearchMain();
  SetCursor((HCURSOR) LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
  pos.MakeMove(Search.mvResult);
  // 清除上一步棋的选择标记
  DrawSquare(SRC(Xqwl.mvLast));
  DrawSquare(DST(Xqwl.mvLast));
  // 把电脑走的棋标记出来
  Xqwl.mvLast = Search.mvResult;
  DrawSquare(SRC(Xqwl.mvLast), DRAW_SELECTED);
  DrawSquare(DST(Xqwl.mvLast), DRAW_SELECTED);
  // 检查重复局面
  vlRep = pos.RepStatus(3);
  if (pos.IsMate()) {
    // 如果分出胜负，那么播放胜负的声音，并且弹出不带声音的提示框
    PlayResWav(IDR_LOSS);
    MessageBoxMute("请再接再厉！");
    Xqwl.bGameOver = TRUE;
  } else if (vlRep > 0) {
    vlRep = pos.RepValue(vlRep);
    // 注意："vlRep"是对玩家来说的分值
    PlayResWav(vlRep < -WIN_VALUE ? IDR_LOSS : vlRep > WIN_VALUE ? IDR_WIN : IDR_DRAW);
    MessageBoxMute(vlRep < -WIN_VALUE ? "长打作负，请不要气馁！" :
        vlRep > WIN_VALUE ? "电脑长打作负，祝贺你取得胜利！" : "双方不变作和，辛苦了！");
    Xqwl.bGameOver = TRUE;
  } else if (pos.nMoveNum > 100) {
    PlayResWav(IDR_DRAW);
    MessageBoxMute("超过自然限着作和，辛苦了！");
    Xqwl.bGameOver = TRUE;
  } else {
    // 如果没有分出胜负，那么播放将军、吃子或一般走子的声音
    PlayResWav(pos.InCheck() ? IDR_CHECK2 : pos.Captured() ? IDR_CAPTURE2 : IDR_MOVE2);
    if (pos.Captured()) {
      pos.SetIrrev();
    }
  }
}

// 点击格子事件处理
static void ClickSquare(int sq) {
  int pc, mv, vlRep;
  Xqwl.hdc = GetDC(Xqwl.hWnd);
  Xqwl.hdcTmp = CreateCompatibleDC(Xqwl.hdc);
  sq = Xqwl.bFlipped ? SQUARE_FLIP(sq) : sq;
  pc = pos.ucpcSquares[sq];

  if ((pc & SIDE_TAG(pos.sdPlayer)) != 0) {
    // 如果点击自己的子，那么直接选中该子
    if (Xqwl.sqSelected != 0) {
      DrawSquare(Xqwl.sqSelected);
    }
    Xqwl.sqSelected = sq;
    DrawSquare(sq, DRAW_SELECTED);
    if (Xqwl.mvLast != 0) {
      DrawSquare(SRC(Xqwl.mvLast));
      DrawSquare(DST(Xqwl.mvLast));
    }
    PlayResWav(IDR_CLICK); // 播放点击的声音

  } else if (Xqwl.sqSelected != 0 && !Xqwl.bGameOver) {
    // 如果点击的不是自己的子，但有子选中了(一定是自己的子)，那么走这个子
    mv = MOVE(Xqwl.sqSelected, sq);
    if (pos.LegalMove(mv)) {
      if (pos.MakeMove(mv)) {
        Xqwl.mvLast = mv;
        DrawSquare(Xqwl.sqSelected, DRAW_SELECTED);
        DrawSquare(sq, DRAW_SELECTED);
        Xqwl.sqSelected = 0;
        // 检查重复局面
        vlRep = pos.RepStatus(3);
        if (pos.IsMate()) {
          // 如果分出胜负，那么播放胜负的声音，并且弹出不带声音的提示框
          PlayResWav(IDR_WIN);
          MessageBoxMute("祝贺你取得胜利！");
          Xqwl.bGameOver = TRUE;
        } else if (vlRep > 0) {
          vlRep = pos.RepValue(vlRep);
          // 注意："vlRep"是对电脑来说的分值
          PlayResWav(vlRep > WIN_VALUE ? IDR_LOSS : vlRep < -WIN_VALUE ? IDR_WIN : IDR_DRAW);
          MessageBoxMute(vlRep > WIN_VALUE ? "长打作负，请不要气馁！" :
              vlRep < -WIN_VALUE ? "电脑长打作负，祝贺你取得胜利！" : "双方不变作和，辛苦了！");
          Xqwl.bGameOver = TRUE;
        } else if (pos.nMoveNum > 100) {
          PlayResWav(IDR_DRAW);
          MessageBoxMute("超过自然限着作和，辛苦了！");
          Xqwl.bGameOver = TRUE;
        } else {
          // 如果没有分出胜负，那么播放将军、吃子或一般走子的声音
          PlayResWav(pos.InCheck() ? IDR_CHECK : pos.Captured() ? IDR_CAPTURE : IDR_MOVE);
          if (pos.Captured()) {
            pos.SetIrrev();
          }
          ResponseMove(); // 轮到电脑走棋
        }
      } else {
        PlayResWav(IDR_ILLEGAL); // 播放被将军的声音
      }
    }
    // 如果根本就不符合走法(例如马不走日字)，那么程序不予理会
  }
  DeleteDC(Xqwl.hdcTmp);
  ReleaseDC(Xqwl.hWnd, Xqwl.hdc);
}

// 初始化棋局
static void Startup(void) {
  pos.Startup();
  Xqwl.sqSelected = Xqwl.mvLast = 0;
  Xqwl.bGameOver = FALSE;
}

// 窗体事件捕捉过程
static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  int x, y;
  HDC hdc;
  RECT rect;
  PAINTSTRUCT ps;
  MSGBOXPARAMS mbp;

  switch (uMsg) {
  // 新建窗口
  case WM_CREATE:
    // 调整窗口位置和尺寸
    GetWindowRect(hWnd, &rect);
    x = rect.left;
    y = rect.top;
    rect.right = rect.left + BOARD_WIDTH;
    rect.bottom = rect.top + BOARD_HEIGHT;
    AdjustWindowRect(&rect, WINDOW_STYLES, TRUE);
    MoveWindow(hWnd, x, y, rect.right - rect.left, rect.bottom - rect.top, TRUE);
    break;
  // 退出
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  // 菜单命令
  case WM_COMMAND:
    switch (LOWORD(wParam)) {
    case IDM_FILE_RED:
    case IDM_FILE_BLACK:
      Xqwl.bFlipped = (LOWORD(wParam) == IDM_FILE_BLACK);
      Startup();
      hdc = GetDC(Xqwl.hWnd);
      DrawBoard(hdc);
      if (Xqwl.bFlipped) {
        Xqwl.hdc = hdc;
        Xqwl.hdcTmp = CreateCompatibleDC(Xqwl.hdc);
        ResponseMove();
        DeleteDC(Xqwl.hdcTmp);
      }
      ReleaseDC(Xqwl.hWnd, hdc);
      break;
    case IDM_FILE_EXIT:
      DestroyWindow(Xqwl.hWnd);
      break;
    case IDM_HELP_HOME:
      ShellExecute(NULL, NULL, "http://www.xqbase.com/", NULL, NULL, SW_SHOWNORMAL);
      break;
    case IDM_HELP_ABOUT:
      // 弹出带象棋小巫师图标的对话框
      MessageBeep(MB_ICONINFORMATION);
      mbp.cbSize = sizeof(MSGBOXPARAMS);
      mbp.hwndOwner = hWnd;
      mbp.hInstance = Xqwl.hInst;
      mbp.lpszText = cszAbout;
      mbp.lpszCaption = "关于象棋小巫师";
      mbp.dwStyle = MB_USERICON;
      mbp.lpszIcon = MAKEINTRESOURCE(IDI_APPICON);
      mbp.dwContextHelpId = 0;
      mbp.lpfnMsgBoxCallback = NULL;
      mbp.dwLanguageId = 0;
      MessageBoxIndirect(&mbp);
      break;
    }
    break;
  // 绘图
  case WM_PAINT:
    hdc = BeginPaint(Xqwl.hWnd, &ps);
    DrawBoard(hdc);
    EndPaint(Xqwl.hWnd, &ps);
    break;
  // 鼠标点击
  case WM_LBUTTONDOWN:
    x = FILE_LEFT + (LOWORD(lParam) - BOARD_EDGE) / SQUARE_SIZE;
    y = RANK_TOP + (HIWORD(lParam) - BOARD_EDGE) / SQUARE_SIZE;
    if (x >= FILE_LEFT && x <= FILE_RIGHT && y >= RANK_TOP && y <= RANK_BOTTOM) {
      ClickSquare(COORD_XY(x, y));
    }
    break;
  // 其他事件
  default:
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
  }
  return FALSE;
}

// 装入资源图片
inline HBITMAP LoadResBmp(int nResId) {
  return (HBITMAP) LoadImage(Xqwl.hInst, MAKEINTRESOURCE(nResId), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
}

// 入口过程
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  int i;
  MSG msg;
  WNDCLASSEX wce;

  // 初始化全局变量
  srand((DWORD) time(NULL));
  InitZobrist();
  Xqwl.hInst = hInstance;
  LoadBook();
  Xqwl.bFlipped = FALSE;
  Startup();

  // 装入图片
  Xqwl.bmpBoard = LoadResBmp(IDB_BOARD);
  Xqwl.bmpSelected = LoadResBmp(IDB_SELECTED);
  for (i = PIECE_KING; i <= PIECE_PAWN; i ++) {
    Xqwl.bmpPieces[SIDE_TAG(0) + i] = LoadResBmp(IDB_RK + i);
    Xqwl.bmpPieces[SIDE_TAG(1) + i] = LoadResBmp(IDB_BK + i);
  }

  // 设置窗口
  wce.cbSize = sizeof(WNDCLASSEX);
  wce.style = 0;
  wce.lpfnWndProc = (WNDPROC) WndProc;
  wce.cbClsExtra = wce.cbWndExtra = 0;
  wce.hInstance = hInstance;
  wce.hIcon = (HICON) LoadImage(hInstance, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 32, 32, LR_SHARED);
  wce.hCursor = (HCURSOR) LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
  wce.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);
  wce.lpszMenuName = MAKEINTRESOURCE(IDM_MAINMENU);
  wce.lpszClassName = "XQWLIGHT";
  wce.hIconSm = (HICON) LoadImage(hInstance, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 16, 16, LR_SHARED);
  RegisterClassEx(&wce);

  // 打开窗口
  Xqwl.hWnd = CreateWindow("XQWLIGHT", "象棋小巫师", WINDOW_STYLES,
      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
  if (Xqwl.hWnd == NULL) {
    return 0;
  }
  ShowWindow(Xqwl.hWnd, nCmdShow);
  UpdateWindow(Xqwl.hWnd);

  // 接收消息
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
}