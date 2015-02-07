#include <windows.h>

#ifndef ZIP_H
#define ZIP_H

typedef struct {
  int (WINAPI *print) (LPSTR, unsigned long);
  int (WINAPI *comment) (LPSTR, int, LPCSTR, LPCSTR);
  int (WINAPI *password) (LPSTR, int, LPCSTR, LPCSTR);
  int (WINAPI *split) (LPSTR);  /* This MUST be set to NULL unless you want to be queried
                                   for a destination for each split archive. */
  int (WINAPI *ServiceApplication64) (LPCSTR, unsigned __int64);
  int (WINAPI *ServiceApplication64_No_Int64) (LPCSTR, unsigned long, unsigned long);
} ZIPUSERFUNCTIONS, *LPZIPUSERFUNCTIONS;

typedef struct {
  int  argc;            /* Count of files to zip */
  LPSTR lpszZipFN;      /* name of archive to create/update */
  char **FNV;           /* array of file names to zip up */
  LPSTR lpszAltFNL;     /* pointer to a string containing a list of file
                           names to zip up, separated by whitespace. Intended
                           for use only by VB users, all others should set this
                           to NULL. */
} ZCL, *LPZCL;

typedef struct {        /* zip options */
LPSTR Date;             /* Date to include after */
LPSTR szRootDir;        /* Directory to use as base for zipping */
LPSTR szTempDir;        /* Temporary directory used during zipping */
BOOL fTemp;             /* Use temporary directory '-b' during zipping */
BOOL fSuffix;           /* include suffixes (not implemented) */
BOOL fEncrypt;          /* encrypt files */
BOOL fSystem;           /* include system and hidden files */
BOOL fVolume;           /* Include volume label */
BOOL fExtra;            /* Exclude extra attributes */
BOOL fNoDirEntries;     /* Do not add directory entries */
BOOL fExcludeDate;      /* Exclude files newer than specified date */
BOOL fIncludeDate;      /* Include only files newer than specified date */
BOOL fVerbose;          /* Mention oddities in zip file structure */
BOOL fQuiet;            /* Quiet operation */
BOOL fCRLF_LF;          /* Translate CR/LF to LF */
BOOL fLF_CRLF;          /* Translate LF to CR/LF */
BOOL fJunkDir;          /* Junk directory names */
BOOL fGrow;             /* Allow appending to a zip file */
BOOL fForce;            /* Make entries using DOS names (k for Katz) */
BOOL fMove;             /* Delete files added or updated in zip file */
BOOL fDeleteEntries;    /* Delete files from zip file */
BOOL fUpdate;           /* Update zip file--overwrite only if newer */
BOOL fFreshen;          /* Freshen zip file--overwrite only */
BOOL fJunkSFX;          /* Junk SFX prefix */
BOOL fLatestTime;       /* Set zip file time to time of latest file in it */
BOOL fComment;          /* Put comment in zip file */
BOOL fOffsets;          /* Update archive offsets for SFX files */
BOOL fPrivilege;        /* Use privileges (WIN32 only) */
BOOL fEncryption;       /* TRUE if encryption supported, else FALSE.
                           this is a read only flag */
LPSTR szSplitSize;      /* This string contains the size that you want to
                           split the archive into. i.e. 100 for 100 bytes,
                           2K for 2 k bytes, where K is 1024, m for meg
                           and g for gig. If this string is not NULL it
                           will automatically be assumed that you wish to
                           split an archive. */
LPSTR szIncludeList;    /* Pointer to include file list string (for VB) */
long IncludeListCount;  /* Count of file names in the include list array */
char **IncludeList;     /* Pointer to include file list array. Note that the last
                           entry in the array must be NULL */
LPSTR szExcludeList;    /* Pointer to exclude file list (for VB) */
long ExcludeListCount;  /* Count of file names in the include list array */
char **ExcludeList;     /* Pointer to exclude file list array. Note that the last
                           entry in the array must be NULL */
int  fRecurse;          /* Recurse into subdirectories. 1 => -r, 2 => -R */
int  fRepair;           /* Repair archive. 1 => -F, 2 => -FF */
char fLevel;            /* Compression level (0 - 9) */
} ZPOPT, *LPZPOPT;

extern "C" int WINAPI ZpInit(LPZIPUSERFUNCTIONS lpZipUserFunc);
extern "C" int WINAPI ZpArchive(ZCL C, LPZPOPT Opts);

#endif