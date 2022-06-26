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
һ����Ч�� Cabinet ������������Ϊ[PSFExtHandler_util_OpenCabinet](PSFExtHandler_util_OpenCabinet_zh-Hans.md)�ķ���ֵ��

`[in, optional] pfProgressCallback`  
��ѡ��һ��������޷���ֵ����Ϊ[PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE](PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE_zh-Hans.md)ö�٣�[PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESS](PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESS_zh-Hans.md)�ṹ�ĳ���ָ�룬ָ������ָ����û������������ָ��ĺ���ָ�롣  
�û�����һ�� VOID([PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE](PSFEXTHANDLER_UTIL_CABEXPANSIONSTATE_zh-Hans.md), const [PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESS](PSFEXTHANDLER_UTIL_CABEXPANSIONPROGRESS_zh-Hans.md)\*, PVOID) �� C ����Լ���ĺ��������ļ���չ���������ÿ���ļ��������һ���ṩ�ĺ���������ȡ�  
��һ�������ṩ��ǰ��״̬����״̬Ϊ State_WriteFile ʱ��ͨ���޸�ָ������ָ������д���ļ��������ָ��ָ��ľ��Ϊ NULL��Ĭ�ϣ�������Ĭ���ļ����ڳ���ĵ�ǰĿ¼�����ļ�����������д���ṩ�ľ����������Ϊ INVALID_HANDLE_VALUE ����Ϊ�û�ȡ���˲���������ֹͣ������ LastError Ϊ ERROR_CANCELLED����״̬Ϊ State_Close�����ṩ�˾����������踺��رվ�����������������ļ�ʱ�䣻����ָ��ָ��ľ��Ϊ NULL���������Զ������ļ�ʱ�䡣  
���Ϊ NULL������Ϊչ�����̲���Ҫ֪�����ȡ�  

`[in, optional] pvUserData`  
��ѡ���û�������ṩ�����ȱ��溯�����Զ������ݡ�

### ����ֵ
�ɹ����� TRUE��ʧ�ܷ��� FALSE��
### ��ע
���ִ���ɲ�ѯ LastError��
