# PSFExtHandler_util_ExpandCabinet
````c
PSFEXTRACTIONHANDLER_API
BOOL PSFExtHandler_util_ExpandCabinet(
    _In_        HANDLE hCabinet,
    _In_        PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESSCALLBACK pfnProgressCallback,
    _In_opt_    PVOID pvUserData
);
````
### ����
`[in] hCab`  
һ����Ч�� Cabinet ������������һ����Ч�� Cabinet ������������Ϊ[PSFExtHandler_util_OpenCabinet](PSFExtHandler_util_OpenCabinet_zh-Hans.md)��[PSFExtHandler_util_CopyHandle](PSFExtHandler_util_CopyHandle_zh-Hans.md)�ķ���ֵ��

`[in, optional] pfProgressCallback`  
��ѡ��һ������ķ���ֵΪ BOOL ����Ϊ[PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE](PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE_zh-Hans.md)ö�٣�[PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESSINFO](PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESSINFO_zh-Hans.md)�ṹ�ĳ���ָ�룬ָ������ָ����û������������ָ��ĺ���ָ�롣

����һ�� BOOL([PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE](PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE_zh-Hans.md), [PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESSINFO](PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESSINFO_zh-Hans.md)\*, PVOID) �� C ����Լ���ĺ��������ļ���չ���������ÿ���ļ��������һ���ṩ�ĺ���������ȡ�  
��һ�������ṩ��ǰ��״̬����״̬Ϊ `PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE_STATE_WRITEFILE` ʱ��ͨ��ָ������ָ���ṩ�ļ��������ָ��ָ��ľ��Ϊ NULL��Ĭ�ϣ�������Ĭ���ļ����ڳ���ĵ�ǰĿ¼�����ļ�����������д���ṩ�ľ����������Ϊ `INVALID_HANDLE_VALUE` ����Ϊ�û���Ҫ�������ļ�����״̬Ϊ `PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE_STATE_CLOSEFILE`�����ṩ�˾��������´˻ص������踺��رվ�����������������ļ�ʱ�䣻��ָ��ָ��ľ��Ϊ `NULL`���������Զ������ļ�ʱ�䡣  
����Ӧ�÷��� TRUE�������� FALSE ����Ϊ�û�Ҫȡ���������˺�����ȡ������������ LastError Ϊ `ERROR_CANCELLED`��
���Ϊ NULL������Ϊչ�����̲���Ҫ֪�����ȡ�  

`[in, optional] pvUserData`  
��ѡ���û�������ṩ�����ȱ��溯�����Զ������ݡ�

### ����ֵ
�ɹ����� TRUE��ʧ�ܷ��� FALSE��
### ��ע
���ִ���ɲ�ѯ LastError��
