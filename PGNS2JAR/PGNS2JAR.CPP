/*
PGNS2JAR - a XQBoss Jar Making Program
Designed by Morning Yellow, Version: 1.31, Last Modified: Jan. 2010
Copyright (C) 2004-2010 www.xqbase.com

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <windows.h>
#include <shlobj.h>
#include <stdio.h>
#include <string.h>
#include "zip.h"

const int MAX_CHAR = 1024;
const char *const cszMidletVersion = "1.31";

// 棋谱信息
static struct {
  BOOL bBig5;                 // 是否繁体
  char szTitle[MAX_CHAR];     // 棋谱标题
  char szSrcFolder[MAX_CHAR]; // 棋谱来源文件夹
  char szDstFolder[MAX_CHAR]; // 棋谱目标文件夹
  char szJarFile[MAX_CHAR];   // 手机棋谱程序文件
} Pgns2Jar;

inline const char *CnTw(const char *szCn, const char *szTw) {
  return Pgns2Jar.bBig5 ? szTw : szCn;
}

// 打开文件错误时终止程序
static void ErrorOpen(const char *szFile) {
  char szMsg[MAX_CHAR];
  sprintf(szMsg, CnTw("无法打开文件 %s", "礚猭ゴ秨ゅン %s"), szFile);
  MessageBox(NULL, szMsg, CnTw("手机棋谱制作工具", "も诀囱眯籹ㄣ"), MB_ICONSTOP);
  ExitProcess(0);
}

// 定位棋谱目标文件夹
static void LocateDestFolder(void) {
  char *lpBackSlash;
  GetModuleFileName(NULL, Pgns2Jar.szDstFolder, MAX_PATH);
  lpBackSlash = strrchr(Pgns2Jar.szDstFolder, '\\');
  if (lpBackSlash == NULL) {
    strcpy(Pgns2Jar.szDstFolder, "PGNS2JAR");
  } else {
    strcpy(lpBackSlash, "\\PGNS2JAR");
  }
}

// 选择棋谱来源文件夹
static bool ChooseSourceFolder(void) {
  char *lpBackSlash;
  BROWSEINFO bi;
  LPITEMIDLIST pidl;

  bi.hwndOwner = NULL;
  bi.pidlRoot = NULL;
  bi.pszDisplayName = NULL;
  bi.lpszTitle = CnTw("请选择象棋棋谱(*.PGN)所在的文件夹", "叫匡拒禜囱囱眯(*.PGN)┮ゅンЖ");
  bi.ulFlags = BIF_RETURNONLYFSDIRS;
  bi.lpfn = NULL;
  bi.lParam = NULL;
  bi.iImage = 0;
  pidl = SHBrowseForFolder(&bi);
  if (!SHGetPathFromIDList(pidl, Pgns2Jar.szSrcFolder)) {
    return false;
  }

  // 获取棋谱标题
  lpBackSlash = strrchr(Pgns2Jar.szSrcFolder, '\\');
  if (lpBackSlash == NULL || lpBackSlash[1] == '\0') {
    strcpy(Pgns2Jar.szTitle, CnTw("无标题", "礚夹肈"));
  } else {
    strcpy(Pgns2Jar.szTitle, lpBackSlash + 1);
  }
  return true;
}

// 选择要生成的手机棋谱文件
static bool ChooseJarFile(void) {
  char *lpFileName;
  OPENFILENAME ofn;

  strcpy(Pgns2Jar.szJarFile, Pgns2Jar.szTitle);
  strcat(Pgns2Jar.szJarFile, ".jar");
  memset(&ofn, 0, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.lpstrFilter = CnTw("手机应用程序 (*.jar)\0*.jar\0所有文件 (*.*)\0*.*\0",
      "も诀莱ノ祘(*.jar)\0*.jar\0┮Τゅン(*.*)\0*.*\0");
  ofn.lpstrFile = Pgns2Jar.szJarFile;
  ofn.nMaxFile = MAX_PATH;
  ofn.lpstrTitle = CnTw("请指定要生成的手机棋谱程序", "叫﹚璶ネΘも诀囱眯祘");
  ofn.lpstrDefExt = "jar";
  ofn.Flags = OFN_PATHMUSTEXIST + OFN_HIDEREADONLY + OFN_OVERWRITEPROMPT;

  while (true) {
    if (GetSaveFileName(&ofn) == 0) {
      return false;
    }

    // 检查文件名是否有中文
    lpFileName = strrchr(Pgns2Jar.szJarFile, '\\');
    lpFileName = (lpFileName == NULL ? Pgns2Jar.szJarFile : lpFileName + 1);
    if (MultiByteToWideChar(CP_ACP, 0, lpFileName, -1, NULL, 0) < (int) strlen(lpFileName)) {
      if (MessageBox(NULL, CnTw("含有中文的JAR文件名在某些手机上无法使用，是否继续？",
          "ΤいゅJARゅン琘ㄇも诀礚猭ㄏノ琌膥尿"),
          CnTw("手机棋谱制作工具", "も诀囱眯籹ㄣ"), MB_OKCANCEL + MB_ICONWARNING) == IDCANCEL) {
        continue;
      }
    }
    return true;
  }
}

static void SearchFolder(const char *szSrcFolder, const char *szDstFolder);

// 搜索到指定的文件
static void SearchFile(const char *szSrcFile, const char *szDstFolder, int nIndex, const WIN32_FIND_DATA &wfd) {
  int nFileLen;
  FILE *fpFileList;
  char *lpDstFile, *lpBackSlash;
  char szPgnFile[MAX_CHAR], szDstFile[MAX_CHAR], szTitle[MAX_CHAR];

  // 把szDstFile定位为szDstFolder，用strcpy(lpDstFile, ...)定位文件
  strcpy(szDstFile, szDstFolder);
  lpDstFile = szDstFile + strlen(szDstFolder);
  if (*(lpDstFile - 1) != '\\') {
    *lpDstFile = '\\';
    lpDstFile ++;
  }
  // 打开FILELIST
  strcpy(lpDstFile, "FILELIST");
  fpFileList = fopen(szDstFile, "at");
  if (fpFileList == NULL) {
    ErrorOpen(szDstFile);
  }

  // 获取文件标题
  lpBackSlash = strrchr(szSrcFile, '\\');
  if (lpBackSlash == NULL || lpBackSlash[1] == '\0') {
    strcpy(szTitle, CnTw("无标题", "礚夹肈"));
  } else {
    strcpy(szTitle, lpBackSlash + 1);
  }

  if ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
    nFileLen = strlen(szSrcFile) - 4;
    if (nFileLen > 0 && strnicmp(szSrcFile + nFileLen, ".PGN", 4) == 0) {
      // 是PGN文件，写入上级文件夹的FILELIST，并拷贝之
      fprintf(fpFileList, "%d.PGN=%s\n", nIndex, szTitle);
      sprintf(szPgnFile, "%d.PGN", nIndex);
      strcpy(lpDstFile, szPgnFile);
      CopyFile(szSrcFile, szDstFile, FALSE);
    }
  } else if (strcmp(wfd.cFileName, ".") != 0 && strcmp(wfd.cFileName, "..") != 0) {
    // 是目录，写入上级文件夹的FILELIST，并建立、搜索之
    fprintf(fpFileList, "/%d=%s\n", nIndex, szTitle);
    sprintf(szPgnFile, "%d", nIndex);
    strcpy(lpDstFile, szPgnFile);
    CreateDirectory(szDstFile, NULL);
    SearchFolder(szSrcFile, szDstFile);
  }
  fclose(fpFileList);
}

// 搜索指定文件夹
static void SearchFolder(const char *szSrcFolder, const char *szDstFolder) {
  HANDLE hFind;
  WIN32_FIND_DATA wfd;
  int nIndex;
  char *lpSrcFile;
  char szSrcFile[MAX_CHAR];

  // 把szSrcFile定位为szSrcFolder，用strcpy(lpSrcFile, ...)定位文件
  strcpy(szSrcFile, szSrcFolder);
  lpSrcFile = szSrcFile + strlen(szSrcFolder) + 1;
  *(lpSrcFile - 1) = '\\';
  // szSrcFile改为...\*，作为搜索对象
  strcpy(lpSrcFile, "*");
  hFind = FindFirstFile(szSrcFile, &wfd);
  if (hFind != INVALID_HANDLE_VALUE) {
    strcpy(lpSrcFile, wfd.cFileName);
    SearchFile(szSrcFile, szDstFolder, 1, wfd);
    nIndex = 2;
    while (FindNextFile(hFind, &wfd)) {
      strcpy(lpSrcFile, wfd.cFileName);
      SearchFile(szSrcFile, szDstFolder, nIndex, wfd);
      nIndex ++;
    }
  }
  FindClose(hFind);
}

static void SearchFolderToDelete(const char *szDstFolder);

// 搜索到指定的文件加以删除
static void SearchFileToDelete(const char *szDstFile, const WIN32_FIND_DATA &wfd) {
  if ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
    DeleteFile(szDstFile);
  } else if (strcmp(wfd.cFileName, ".") != 0 && strcmp(wfd.cFileName, "..") != 0) {
    SearchFolderToDelete(szDstFile);
    RemoveDirectory(szDstFile);
  }
}

// 搜索指定文件夹加以删除
static void SearchFolderToDelete(const char *szDstFolder) {
  HANDLE hFind;
  WIN32_FIND_DATA wfd;
  char *lpDstFile;
  char szDstFile[MAX_CHAR];

  // 把szDstFile定位为szDstFolder，用strcpy(lpDstFile, ...)定位文件
  strcpy(szDstFile, szDstFolder);
  lpDstFile = szDstFile + strlen(szDstFolder) + 1;
  *(lpDstFile - 1) = '\\';
  // szDstFile改为...\*，作为搜索对象
  strcpy(lpDstFile, "*");
  hFind = FindFirstFile(szDstFile, &wfd);
  if (hFind != INVALID_HANDLE_VALUE) {
    strcpy(lpDstFile, wfd.cFileName);
    SearchFileToDelete(szDstFile, wfd);
    while (FindNextFile(hFind, &wfd)) {
      strcpy(lpDstFile, wfd.cFileName);
      SearchFileToDelete(szDstFile, wfd);
    }
  }
  FindClose(hFind);
}

// 把ANSI转换为UTF8
static const char *Ansi2Utf8(char *szUtf8, const char *szAnsi) {
  wchar_t wszUnicode[MAX_CHAR];
  MultiByteToWideChar(CP_ACP, 0, szAnsi, -1, wszUnicode, MAX_CHAR);
  WideCharToMultiByte(CP_UTF8, 0, wszUnicode, -1, szUtf8, MAX_CHAR, NULL, NULL);
  return szUtf8;
}

// 生成MetaFile文件
static void CreateManifest(void) {
  FILE *fpManifest;
  char szTitle[MAX_CHAR], szDescript[MAX_CHAR], szManifest[MAX_CHAR];

  Ansi2Utf8(szTitle, Pgns2Jar.szTitle);
  Ansi2Utf8(szDescript, CnTw("象棋小巫师手机棋谱", "禜囱畍も诀囱眯"));
  strcpy(szManifest, Pgns2Jar.szDstFolder);
  strcat(szManifest, "\\META-INF\\MANIFEST.MF");
  fpManifest = fopen(szManifest, "wt");
  if (fpManifest == NULL) {
    ErrorOpen(szManifest);
  }

  fprintf(fpManifest, "Manifest-Version: 1.0\n");
  fprintf(fpManifest, "MIDlet-1: %s, /images/xqboss.png, xqboss.XQBossMIDlet\n", szTitle);
  fprintf(fpManifest, "MIDlet-Name: %s\n", szTitle);
  fprintf(fpManifest, "MIDlet-Description: %s - %s\n", szTitle, szDescript);
  fprintf(fpManifest, "MIDlet-Vendor: www.xqbase.com\n");
  fprintf(fpManifest, "MIDlet-Info-URL: http://www.xqbase.com/\n");
  fprintf(fpManifest, "MIDlet-Icon: /images/xqboss.png\n");
  fprintf(fpManifest, "MIDlet-Version: %s\n", cszMidletVersion);
  fprintf(fpManifest, "MicroEdition-Configuration: CLDC-1.0\n");
  fprintf(fpManifest, "MicroEdition-Profile: MIDP-2.0\n");

  fclose(fpManifest);
}

// 生成JAD文件，同时删除MANIFEST.MF
static void CreateJadFile(void) {
  int nFileLen;
  FILE *fpJarFile, *fpJadFile;
  char *lpBackSlash;
  char szManifest[MAX_CHAR], szJadFile[MAX_CHAR], szJarFile[MAX_CHAR];

  // 如果文件名不以JAR结尾，那么不生成JAD文件
  nFileLen = strlen(Pgns2Jar.szJarFile) - 4;
  if (nFileLen <= 0 || strnicmp(Pgns2Jar.szJarFile + nFileLen, ".JAR", 4) != 0) {
    return;
  }

  // 把MANIFEST.MF改名为JAD文件
  strcpy(szManifest, Pgns2Jar.szDstFolder);
  strcat(szManifest, "\\META-INF\\MANIFEST.MF");
  strncpy(szJadFile, Pgns2Jar.szJarFile, nFileLen);
  strcpy(szJadFile + nFileLen, ".jad");
  DeleteFile(szJadFile);
  MoveFile(szManifest, szJadFile);

  // 把JAR文件名和字节数写入JAD中
  fpJarFile = fopen(Pgns2Jar.szJarFile, "rb");
  if (fpJarFile == NULL) {
    ErrorOpen(Pgns2Jar.szJarFile);
  }
  fseek(fpJarFile, 0, SEEK_END);  
  lpBackSlash = strrchr(Pgns2Jar.szJarFile, '\\');
  Ansi2Utf8(szJarFile, lpBackSlash == NULL ? Pgns2Jar.szJarFile : lpBackSlash + 1);

  fpJadFile = fopen(szJadFile, "at");
  if (fpJadFile == NULL) {
    ErrorOpen(szJadFile);
  }
  fprintf(fpJadFile, "MIDlet-Jar-Size: %d\n", ftell(fpJarFile));
  fprintf(fpJadFile, "MIDlet-Jar-URL: %s\n", szJarFile);
  fclose(fpJadFile);

  fclose(fpJarFile);
};

// InfoZip参数
static int WINAPI PrintFunction(LPSTR, unsigned long) {
  return 0;
}
const char *cszCurrDir[1] = {"."};

// 用InfoZip生成JAR文件
void CreateJarFile(void) {
  ZIPUSERFUNCTIONS zuf;
  ZPOPT zpopt;
  ZCL zcl;

  memset(&zuf, 0, sizeof(ZIPUSERFUNCTIONS));
  zuf.print = PrintFunction;
  ZpInit(&zuf);

  memset(&zpopt, 0, sizeof(ZPOPT));
  zpopt.szRootDir = Pgns2Jar.szDstFolder;
  zpopt.fExtra = TRUE;
  zpopt.fRecurse = 1;

  memset(&zcl, 0, sizeof(ZCL));
  zcl.argc = 1;
  zcl.lpszZipFN = Pgns2Jar.szJarFile;
  zcl.FNV = (char **) cszCurrDir;
  ZpArchive(zcl, &zpopt);
}

// 主程序
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  char szPgnFolder[MAX_CHAR], szSrcCharTab[MAX_CHAR], szDstCharTab[MAX_CHAR], szMsg[MAX_CHAR];

  // 确定当前操作系统字符集
  Pgns2Jar.bBig5 = GetOEMCP() == 950;

  // 定位棋谱目标文件夹
  LocateDestFolder();
  // 让用户选择棋谱来源文件夹
  if (!ChooseSourceFolder()) {
    return 0;
  }
  // 让用户选择手机棋谱程序文件
  if (!ChooseJarFile()) {
    return 0;
  }
  // 搜索并整理棋谱文件
  strcpy(szPgnFolder, Pgns2Jar.szDstFolder);
  strcat(szPgnFolder, "\\PGNS");
  CreateDirectory(szPgnFolder, NULL);
  SearchFolder(Pgns2Jar.szSrcFolder, szPgnFolder);
  // 复制码表
  strcpy(szSrcCharTab, Pgns2Jar.szDstFolder);
  strcat(szSrcCharTab, CnTw(".DAT\\GB.DAT", ".DAT\\BIG5.DAT"));
  strcpy(szDstCharTab, Pgns2Jar.szDstFolder);
  strcat(szDstCharTab, "\\xqboss\\CHARTAB.DAT");
  CopyFile(szSrcCharTab, szDstCharTab, FALSE);
  // 生成MANIFEST.MF、JAR文件和JAD文件
  CreateManifest();
  CreateJarFile();
  CreateJadFile();
  // 删除目标文件夹
  SearchFolderToDelete(szPgnFolder);
  RemoveDirectory(szPgnFolder);

  sprintf(szMsg, CnTw("手机棋谱 %s(.jad) 制作成功。", "も诀囱眯%s(.jad)籹Θ"), Pgns2Jar.szJarFile);
  MessageBox(NULL, szMsg, CnTw("手机棋谱制作工具", "も诀囱眯籹ㄣ"), MB_ICONINFORMATION);
  return 0;
}