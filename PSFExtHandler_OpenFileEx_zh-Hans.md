# PSFExtHandler_OpenFile
��ȡ XML �ļ������� PSF��
````c
PSFEXTRACTIONHANDLER_API
_Must_inspect_result_
HPSF
PSFExtHandler_OpenFileEx(
	_In_opt_    PCWSTR pPSFFile,
	_In_        PCWSTR pXmlFile,
	_Reserved_  PDWORD Reserved,
	_In_        WORD wFlags
);
````
### ����
`[in, optional] pPSFFile`  
��ѡ��һ�� PSF �ļ����ַ��������Ϊ NULL���򷵻ص� PSF ���ֻ�������ļ���Ϣ��ѯ���޷���ȡ�ļ���

`[in] pXmlFile`  
XML �ļ������ַ�����

`Reserved`  
������һ��Ϊ NULL��

`[in] wFlags`  
�ļ��򿪱�ʶ������[PSFEXTHANDLER_OPEN_FLAG](PSFEXTHANDLER_OPEN_FLAG_zh-Hans.md)��
### ����ֵ
�ɹ����� PSF �����ʧ�ܷ��� NULL��
### ��ע
���ִ���ɲ�ѯ LastError��
