# PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESS
CAB �ļ�չ��������Ϣ��
````c
typedef struct _PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESS
{
    HANDLE hFile
    ULONG ulComplitedFiles;
    ULONG ulTotalFiles;
    PCSTR pCurrentFile;
    ULONG ulSize;
    FILETIME FileTime;
}PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESS;
````
### ��Ա
##### hFile
�ļ������
##### ulComplitedFiles
����ɵ��ļ���Ŀ��
##### ulTotalFiles
���ļ�����
##### pCurrentFile
��ǰ�ļ�����
##### ulSize
��ǰ�ļ���С��
##### FileTime
�ļ�ʱ�䡣
