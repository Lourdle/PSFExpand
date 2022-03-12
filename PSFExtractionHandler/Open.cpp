#include "pch.h"
#include "framework.h"

#include <MsXml2.h>
#include <comutil.h>

#include <stdexcept>
#include <mutex>
#include <thread>

#include <omp.h>

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
		hPSF->Files.reset(new FileInfo[hPSF->FileCount]);

		int n = omp_get_num_procs();
		if (hPSF->FileCount / n < 10)
			for (; n != 1; --n)
				if (hPSF->FileCount / n >= 10)
					break;

		mutex Mutex;
		bool except = false;

		omp_set_num_threads(n);
#pragma omp parallel
		{
			int thread = omp_get_thread_num();
			DWORD range = hPSF->FileCount / n;
			FileInfo* pFileInfo = hPSF->Files.get() + range * thread;
			if (thread == n - 1)
				range = hPSF->FileCount - range * (n - 1);

			Mutex.lock();
			XmlNode node = list[hPSF->FileCount / n * thread];
			Mutex.unlock();

			try
			{
				for (DWORD i = 0; i != range; ++i)
				{
					if (except)
						break;

					if (i)
					{
						Mutex.lock();
						try
						{
							++node;
						}
						catch (...)
						{
							Mutex.unlock();
							throw;
						}
						Mutex.unlock();
					}

					FileInfo& fi = pFileInfo[i];
					PSTR str = node.GetAttribute("name");
					fi.name.resize(strlen(str));
					for (LONG i = static_cast<LONG> (strlen(str)); i >= 0; --i)
						fi.name[i] = str[i];
					delete[] str;
					/*
					str = node.GetAttribute("time");
					fi.time = atol(str);
					delete[] str;*/

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
PSFExtHandler_OpenFile(
	PCWSTR psf,
	PCWSTR xml)
{
	unique_ptr<PSF, void(*)(HPSF)> hPSF(new PSF,
		[](HPSF hPSF)
		{
			DWORD Err = GetLastError();
			PSFExtHandler_ClosePSF(hPSF - 1);
			SetLastError(Err);
		}
	);

	if (psf)
	{
		DWORD length = GetFullPathNameW(psf, 0, nullptr, nullptr);
		if (length == 0)
			return FALSE;
		hPSF->PSF.resize(length - 1);
		if (GetFullPathNameW(psf, length, const_cast<LPWSTR>(hPSF->PSF.c_str()), nullptr) == 0)
			return FALSE;

		wstring Psf;
		length = GetLongPathNameW(hPSF->PSF.c_str(), nullptr, 0);
		if (length == 0)
		{
			Psf = L"\\\\?\\";
			Psf += hPSF->PSF;
			length = GetLongPathNameW(Psf.c_str(), nullptr, 0);
			if (length == 0)
			{
				Psf = L"\\\\?\\";
				Psf += L"UNC\\";
				Psf += hPSF->PSF;
				length = GetLongPathNameW(Psf.c_str(), nullptr, 0);
				if (length == 0)
					return FALSE;

				psf = Psf.c_str();
			}
			else
				psf = Psf.c_str();
		}
		hPSF->PSF.resize(length - 1);

		wstring tmp;
		tmp.resize(length - 1);
		if (!GetLongPathNameW(hPSF->PSF.c_str(), const_cast<LPWSTR>(tmp.c_str()), length))
			return FALSE;
		hPSF->PSF = move(tmp);

		hPSF->hPSF = CreateFileW(hPSF->PSF.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
		if (hPSF->hPSF == INVALID_HANDLE_VALUE)
			return nullptr;

		BYTE HOF[16];
		if (!ReadFile(hPSF->hPSF, HOF, 16, nullptr, nullptr))
			return FALSE;

		if (memcmp(HOF, Head, 16))
		{
			SetLastError(ERROR_BAD_FORMAT);
			return FALSE;
		}
	}

	wstring Xml = L"\\\\?\\";
	{
		HANDLE hXml = CreateFileW(xml, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
		if (hXml == INVALID_HANDLE_VALUE)
		{
			Xml += xml;
			hXml = CreateFileW(Xml.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
			if (hXml == INVALID_HANDLE_VALUE)
			{
				Xml.insert(Xml.cbegin() + 4, 4);
				for (int i = 0; i != 4; ++i)
					Xml[i + 4] = L"UNC\\"[i];

				hXml = CreateFileW(Xml.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
				if (hXml == INVALID_HANDLE_VALUE)
					return nullptr;
			}
		}
		else
			Xml = xml;
		CloseHandle(hXml);
	}

	auto Length = GetShortPathNameW(Xml.c_str(), nullptr, 0);
	if (!Length)
		return FALSE;
	else
	{
		wstring tmp;
		tmp.resize(Length - 1);
		GetShortPathNameW(Xml.c_str(), const_cast<LPWSTR>(tmp.c_str()), Length);
		tmp.swap(Xml);
	}
	if (!wcsncmp(Xml.c_str(), L"\\\\?\\", 4))
		if (!wcsncmp(Xml.c_str() + 4, L"UNC\\", 4))
			Xml.erase(Xml.cbegin(), Xml.cbegin() + 4);
		else
			Xml.erase(Xml.cbegin(), Xml.cbegin() + 8);

	Xml.shrink_to_fit();


	BOOL ret;
	thread XmlReadingThread(ReadXml, hPSF.get(), Xml.c_str(), ref(ret));
	XmlReadingThread.join();

	if (ret)
		return hPSF.release() - 1;
	else
		return nullptr;
}

VOID
PSFEXTRACTIONHANDLER_API
PSFExtHandler_ClosePSF(
	HPSF hPSF)
{
	CheckHandle(hPSF, return);

	if (hPSF->hPSF)
		CloseHandle(hPSF->hPSF);

	delete hPSF;
}
