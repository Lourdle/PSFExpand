# PSFExtHandler_OpenFile
��ȡ XML �ļ������� PSF��
````c
PSFEXTRACTIONHANDLER_API
_Must_inspect_result_
_Ret_maybenull_
HPSF
PSFExtHandler_OpenFileEx(
	_In_opt_    PCWSTR pPSFFile,
	_In_        PCWSTR pXmlFile,
	_Reserved_  PVOID Reserved,
	_In_        DWORD dwFlags
);
````
### ����
`[in, optional] pPSFFile`  
��ѡ��һ�� PSF �ļ����ַ��������Ϊ NULL���򷵻ص� PSF ���ֻ�������ļ���Ϣ��ѯ���޷���ȡ�ļ���

`[in] pXmlFile`  
XML �ļ������ַ�����

`Reserved`  
����������Ϊ NULL��

`[in] dwFlags`  
�ļ��򿪱�ʶ�������á�
### ����ֵ
�ɹ����� PSF �����ʧ�ܷ��� NULL��
### ��ע
���ִ���ɲ�ѯ LastError��
