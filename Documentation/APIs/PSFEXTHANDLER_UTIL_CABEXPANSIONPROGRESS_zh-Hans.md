# PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESS
CAB �ļ�չ�����Ⱥ����ṩ�ļ���Ϣ��
````c
typedef struct _PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESS
{
	WORD wComplitedFiles;
	WORD wTotalFiles;
	PCSTR pCurrentFile;
	WORD wCurrentFileWrittenSize;
	FILETIME FileTime;
}PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESS;
````
### ��Ա
##### wComplitedFiles
����ɵ��ļ���Ŀ��
##### wTotalFiles
���ļ�����
##### pCurrentFile
��ǰ�ļ�����
##### wCurrentFileWrittenSize
��ǰ�ļ���д��Ĵ�С��
##### FileTime
�ļ�ʱ�䡣
