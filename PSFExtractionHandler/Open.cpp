#include "pch.h"
#include "framework.h"

#include <MsXml2.h>
#include <comutil.h>

#include <Shlwapi.h>

#include <stdexcept>
#include <thread>

#include "PSFExtHandlerFrame.h"

using namespace std;

class XmlNode
{
public:
	XmlNode(const XmlNode& Node) :node(Node.node), n(Node.n)
	{
		++* n;
	}
	XmlNode(IXMLDOMNode* Node)
	{
		n = new ULONG;
		*n = 1;
		node = Node;
	}

	~XmlNode()
	{
		if (n)
		{
			--* n;
			if (*n == 0)
			{
				if (node)
					node->Release();
				delete n;
			}
		}
	}

	XmlNode& operator=(const XmlNode& Node)
	{
		this->~XmlNode();

		node = Node.node;
		n = Node.n;
		++* n;

		return *this;
	}

	XmlNode& operator=(XmlNode&& Node)noexcept
	{
		this->~XmlNode();

		node = Node.node;
		n = Node.n;
		Node.n = nullptr;

		return *this;
	}

	XmlNode& operator++()
	{
		IXMLDOMNode* next;
		HRESULT hr = node->get_nextSibling(&next);

		if (hr != S_OK)
		{
			SetLastError(ERROR_INVALID_FUNCTION);
			throw runtime_error(nullptr);
		}

		this->~XmlNode();
		node = next;
		n = new ULONG;
		*n = 1;

		return *this;
	}

	XmlNode FirstChildNode()
	{
		IXMLDOMNode* first;
		HRESULT hr = node->get_firstChild(&first);

		if (hr != S_OK)
		{
			SetLastError(ERROR_BAD_FORMAT);
			throw runtime_error(nullptr);
		}
		return first;
	}

	XmlNode NextSiblingNode()
	{
		IXMLDOMNode* next;
		HRESULT hr = node->get_nextSibling(&next);

		if (hr != S_OK)
		{
			SetLastError(ERROR_BAD_FORMAT);
			throw runtime_error(nullptr);
		}
		return next;
	}

	XmlNode SelectNode(PCSTR pNodeString)
	{
		BSTR bstr = _com_util::ConvertStringToBSTR(pNodeString);
		IXMLDOMNode* node;
		HRESULT hr = this->node->selectSingleNode(bstr, &node);
		SysFreeString(bstr);

		if (hr != S_OK)
		{
			SetLastError(ERROR_BAD_FORMAT);
			throw runtime_error(nullptr);
		}
		return node;
	}

	PSTR NodeName()
	{
		BSTR name;
		HRESULT hr = node->get_nodeName(&name);

		if (hr != S_OK)
		{
			SetLastError(ERROR_BAD_FORMAT);
			throw runtime_error(nullptr);
		}

		PSTR str = _com_util::ConvertBSTRToString(name);
		SysFreeString(name);
		return str;
	}

	PSTR GetAttribute(PCSTR Name)
	{
		IXMLDOMNamedNodeMap* nnm;
		HRESULT hr = node->get_attributes(&nnm);
		if (hr != S_OK)
		{
			SetLastError(ERROR_BAD_FORMAT);
			throw runtime_error(nullptr);
		}

		IXMLDOMNode* node;
		BSTR bstr = _com_util::ConvertStringToBSTR(Name);
		hr = nnm->getNamedItem(bstr, &node);
		SysFreeString(bstr);
		if (hr != S_OK)
		{
			nnm->Release();
			SetLastError(ERROR_BAD_FORMAT);
			throw runtime_error(nullptr);
		}

		hr = node->get_text(&bstr);
		node->Release();
		nnm->Release();
		if (hr != S_OK)
		{
			SetLastError(ERROR_BAD_FORMAT);
			throw runtime_error(nullptr);
		}
		PSTR str = _com_util::ConvertBSTRToString(bstr);
		SysFreeString(bstr);
		return str;
	}

protected:
	IXMLDOMNode* node;
private:
	ULONG* n;
};

class XmlNodeList : private XmlNode
{
public:
	XmlNodeList() = delete;
	XmlNodeList(const XmlNodeList&) = delete;
	XmlNodeList(XmlNodeList&&) = delete;
	XmlNodeList(const XmlNode& Node) :XmlNode(Node)
	{
		HRESULT hr = node->get_childNodes(&list);
		if (hr != S_OK)
		{
			SetLastError(ERROR_BAD_FORMAT);
			throw runtime_error(nullptr);
		}
	}

	~XmlNodeList()
	{
		list->Release();
	}

	LONG ChildNodeCount()
	{
		long length;
		HRESULT hr = list->get_length(&length);
		if (hr != S_OK)
		{
			SetLastError(ERROR_BAD_FORMAT);
			throw runtime_error(nullptr);
		}
		else
			return length;
	}

	XmlNode operator[](long i)
	{
		IXMLDOMNode* node;
		HRESULT hr = list->get_item(i, &node);
		if (hr != S_OK)
		{
			SetLastError(ERROR_INVALID_FUNCTION);
			throw runtime_error(nullptr);
		}
		else
			return node;
	}
protected:
	IXMLDOMNodeList* list;
};

class XmlDocument : public XmlNode
{
public:
	XmlDocument(const XmlDocument&) = delete;
	XmlDocument(XmlDocument&&) = delete;
	XmlDocument() = delete;
	XmlDocument(IXMLDocument2* pDoc) : XmlNode(dynamic_cast<IXMLDOMNode*>(pDoc)) {}
	XmlDocument(PCWSTR pXmlFile) : XmlNode(nullptr)
	{
		IXMLDOMDocument2* doc;

		HRESULT hr = CoCreateInstance(CLSID_DOMDocument2, nullptr, CLSCTX_INPROC_SERVER,
			IID_IXMLDOMDocument2, reinterpret_cast<LPVOID*>(&doc));

		if (hr != S_OK)
		{
			SetLastError(ERROR_INVALID_FUNCTION);
			throw runtime_error(nullptr);
		}
		VARIANT var;
		var.vt = VT_BSTR;
		var.bstrVal = SysAllocString(pXmlFile);
		SHORT s;
		hr = doc->load(var, &s);
		SysFreeString(var.bstrVal);
		if (hr != S_OK)
		{
			SetLastError(ERROR_BAD_FORMAT);
			throw runtime_error(nullptr);
		}

		node = doc;
	}

	~XmlDocument() = default;
};


template<typename T>
static
bool
CheckString(PCSTR pString1, PCSTR pString2, T& Target, T Value)
{
	if (!_strcmpi(pString1, pString2))
		Target = Value;
	else
		return false;

	return true;
}

static
inline
bool isnum(char c)
{
	if (c >= '0' && c <= '9')
		return true;

	return false;
}

constexpr
BYTE chex2num(char c)
{
	return c - (isnum(c) ? '0' : ('a' - 10));
}

static
void ReadXml(HPSF hPSF, PCWSTR pXml, BOOL& Ret)
{
	CoInitialize(nullptr);

	Ret = TRUE;

	try
	{
		XmlDocument doc(pXml);
		XmlNode root = doc.FirstChildNode().NextSiblingNode();
		XmlNode files = root.SelectNode("Files");
		XmlNodeList list = files;

		hPSF->FileCount = list.ChildNodeCount();
		if (hPSF->FileCount == 0)
		{
			SetLastError(ERROR_BAD_FORMAT);
			throw runtime_error(nullptr);
		}
		hPSF->Files=new FileInfo[hPSF->FileCount];

		bool except = false;

		FileInfo* pFileInfo = hPSF->Files;

		XmlNode node = list[static_cast<long>(pFileInfo - hPSF->Files)];

		try
		{
			for (DWORD i = 0; i != hPSF->FileCount; ++i)
			{
				if (except)
					break;
				if (i)
					++node;

				FileInfo& fi = pFileInfo[i];
				PSTR str = node.GetAttribute("name");
				fi.name.resize(strlen(str));
				for (LONG i = static_cast<LONG> (strlen(str)); i >= 0; --i)
					fi.name[i] = str[i];
				delete[] str;

				str = node.GetAttribute("time");
				*reinterpret_cast<unsigned long long*>(&fi.time) = strtoull(str, nullptr, 10);
				delete[] str;

				XmlNode Source = node.SelectNode("Delta").SelectNode("Source");

				str = Source.GetAttribute("offset");
				fi.deltaSource.offset = atol(str);
				delete[] str;

				str = Source.GetAttribute("length");
				fi.deltaSource.length = atol(str);
				delete[] str;

				str = Source.GetAttribute("type");
				if (!CheckString(str, "PA30", fi.deltaSource.type, DELTA_FLAG_NONE))
					if (!CheckString(str, "PA19", fi.deltaSource.type, DELTA_APPLY_FLAG_ALLOW_PA19))
						if (!CheckString<DELTA_FLAG_TYPE>(str, "RAW", fi.deltaSource.type, INVALID_FLAG - 1))
							fi.deltaSource.type = INVALID_FLAG;
				delete[] str;

				XmlNode Hash = Source.SelectNode("Hash");

				str = Hash.GetAttribute("alg");
				if (CheckString<ALG_ID>(str, "SHA1", fi.deltaSource.Hash.alg, CALG_SHA1)
					|| CheckString<ALG_ID>(str, "SHA256", fi.deltaSource.Hash.alg, CALG_SHA_256)
					|| CheckString<ALG_ID>(str, "SHA512", fi.deltaSource.Hash.alg, CALG_SHA_512)
					|| CheckString<ALG_ID>(str, "MD5", fi.deltaSource.Hash.alg, CALG_MD5))
				{
					delete[] str;
					str = Hash.GetAttribute("value");

					DWORD HashSize = static_cast<DWORD>(strlen(str) / 2);
					fi.deltaSource.Hash.value.reset(new BYTE[HashSize]);

					for (DWORD i = 0; i != HashSize; ++i)
						fi.deltaSource.Hash.value[i] = chex2num(str[i * 2]) * 16 + chex2num(str[i * 2 + 1]);
				}
				else
					fi.deltaSource.Hash.alg = INVALID_FLAG;
				delete[] str;
			}
		}
		catch (runtime_error&)
		{
			except = true;
		}

		if (except)
			throw runtime_error(nullptr);
	}
	catch (runtime_error&)
	{
		Ret = FALSE;
	}

	CoUninitialize();
}


const BYTE Head[] =
{ 'P','S','T','R','E','A','M',0x00,0x02,0x00,0x02,0x00,0x00,0x00,0x00,0x00 };

PSFEXTRACTIONHANDLER_API
HPSF
PSFExtHandler_OpenFileEx(
	PCWSTR psf,
	PCWSTR xml,
	PVOID,
	DWORD flags)
{
	return PSFExtHandler_OpenFile(psf, xml);
}

PSFEXTRACTIONHANDLER_API
HPSF
PSFExtHandler_OpenFile(
	PCWSTR psf,
	PCWSTR xml)
{
	unique_ptr<PSF, void(*)(HPSF)> hPSF(new PSF(),
		[](HPSF hPSF)
		{
			DWORD Err = GetLastError();
			PSFExtHandler_ClosePSF(hPSF - 1);
			SetLastError(Err);
		}
	);

	if (psf)
	{
		hPSF->hPSF = CreateFileW(psf, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
		if (hPSF->hPSF == INVALID_HANDLE_VALUE)
			return nullptr;
		BYTE HOF[16];
		if (!ReadFile(hPSF->hPSF, HOF, 16, nullptr, nullptr))
			return nullptr;

		if (memcmp(HOF, Head, 16))
		{
			SetLastError(ERROR_BAD_FORMAT);
			return nullptr;
		}
	}

	{
		HANDLE hXml = CreateFileW(xml, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
		if (hXml == INVALID_HANDLE_VALUE)
			return nullptr;
		CloseHandle(hXml);
	}

	BOOL ret;
	thread XmlReadingThread(ReadXml, hPSF.get(), xml, ref(ret));
	XmlReadingThread.join();

	if (ret)
	{
		hPSF->hGlobalEvent = CreateEventW(nullptr, FALSE, TRUE, nullptr);
		hPSF->hEvent = CreateEventW(nullptr, FALSE, TRUE, nullptr);
		hPSF->hFileEvent = CreateEventW(nullptr, FALSE, TRUE, nullptr);
		return hPSF.release() - 1;
	}
	else
	{
		delete[] hPSF->Files;
		return nullptr;
	}
}

VOID
PSFEXTRACTIONHANDLER_API
PSFExtHandler_ClosePSF(
	HPSF hPSF)
{
	CheckHandle(hPSF, return);

	WaitForMultipleObjects(2, &hPSF->hGlobalEvent, TRUE, INFINITE);
	if (hPSF->GetRefCount() == 1)
	{
		CloseHandle(hPSF->hPSF);
		CloseHandle(hPSF->hFileEvent);
		CloseHandle(hPSF->hGlobalEvent);
		delete[] hPSF->Files;
	}
	else
		SetEvent(hPSF->hGlobalEvent);

	CloseHandle(hPSF->hEvent);
	delete hPSF;
}

HPSF PSFExtHandler_CopyHandle(HPSF hPSF)
{
	CheckHandle(hPSF, return nullptr);

	WaitForSingleObject(hPSF->hGlobalEvent, INFINITE);
	HPSF hNew = new PSF(*hPSF);
	SetEvent(hPSF->hGlobalEvent);

	return hNew - 1;
}
