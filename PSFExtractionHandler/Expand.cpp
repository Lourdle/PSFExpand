#include "pch.h"
#include "framework.h"

#include <omp.h>

#include <functional>
#include <utility>
#include <deque>
#include <mutex>
using namespace std;

#include "PSFExtHandlerFrame.h"

template<typename T>
class MessageQueueBase
{
public:
	MessageQueueBase() = default;
	MessageQueueBase(const MessageQueueBase&) = delete;
	MessageQueueBase(MessageQueueBase&&) = delete;

	virtual bool ProcessMessage(function<void(T&)>MsgProc)
	{
		while (Queue.empty() && !Invalid)
			Sleep(1);

		if (Invalid && Queue.empty())
			return false;

		Mutex.lock();
		auto Value = Queue.front();
		MsgProc(Value);
		Queue.pop_front();
		Mutex.unlock();

		return true;
	}

	virtual bool PostMessageValue(const T& Value)
	{
		if (Invalid)
			return false;

		Mutex.lock();
		Queue.push_back(Value);
		Mutex.unlock();

		return true;
	}

	void InvalidateQueue()
	{
		Invalid = true;
	}

protected:
	bool Invalid = false;
	mutex Mutex;
	deque<T> Queue;
};

template<typename T>
class MessageQueue : public MessageQueueBase<pair<T, MessageQueueBase<bool>*>>
{
public:
	MessageQueue(bool bSync = false) :Sync(bSync) {}
	MessageQueue(const MessageQueue&) = delete;
	MessageQueue(MessageQueue&&) = delete;

	bool ProcessMessage(function<void(T&)>MsgProc)
	{
		while (MessageQueueBase<pair<T, MessageQueueBase<bool>*>>::Queue.empty()
			&& !MessageQueueBase<pair<T, MessageQueueBase<bool>*>>::Invalid)
				Sleep(1);

		if (MessageQueueBase<pair<T, MessageQueueBase<bool>*>>::Invalid
			&& MessageQueueBase<pair<T, MessageQueueBase<bool>*>>::Queue.empty())
			return false;

		MessageQueueBase<pair<T, MessageQueueBase<bool>*>>::Mutex.lock();
		auto Value = MessageQueueBase<pair<T, MessageQueueBase<bool>*>>::Queue.front().first;
		MsgProc(Value);
		if (Sync)
			MessageQueueBase<pair<T, MessageQueueBase<bool>*>>::Queue.front().second->PostMessageValue(true);
		MessageQueueBase<pair<T, MessageQueueBase<bool>*>>::Queue.pop_front();
		MessageQueueBase<pair<T, MessageQueueBase<bool>*>>::Mutex.unlock();

		return true;
	}

	bool SendMessageVaule(const T& Value)
	{
		if (MessageQueueBase<pair<T, MessageQueueBase<bool>*>>::Invalid)
			return false;

		MessageQueueBase<bool> q;
		MessageQueueBase<pair<T, MessageQueueBase<bool>*>>::Mutex.lock();
		MessageQueueBase<pair<T, MessageQueueBase<bool>*>>::Queue.push_back({ Value,Sync ? &q : nullptr });
		MessageQueueBase<pair<T, MessageQueueBase<bool>*>>::Mutex.unlock();

		if (Sync)
			q.ProcessMessage([](bool) {});

		return true;
	}


protected:
	const bool Sync;
	using MessageQueueBase<pair<T, MessageQueueBase<bool>*>>::PostMessageValue;
};

PSFEXTRACTIONHANDLER_API
BOOL
PSFExtHandler_ExpandPSF(
	HPSF hPSF,
	PCWSTR outdir,
	WORD flags,
	PSFEXTHANDLER_PROGRESS_PROC progress,
	PVOID userdata
)
{
	CheckHandle(hPSF, return FALSE);

	if (flags & PSFEXTHANDLER_EXTRACT_FLAG_SKIP_EXISTS
		&& flags & PSFEXTHANDLER_EXTRACT_FLAG_FAIL_IF_EXISTS
		|| flags & PSFEXTHANDLER_EXTRACT_FLAG_WRITE_DATA_TO_HANDLE
		|| flags >> 10)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	bool cancel = false;
	int n = omp_get_num_procs();
	if (flags & PSFEXTHANDLER_EXTRACT_FLAG_SINGLE_THREAD)
		n = 1;

	if (hPSF->FileCount / n < 10)
		for (; n != 1; --n)
			if (hPSF->FileCount / n >= 10)
				break;

	PHANDLE hPSFFile = new HANDLE[n]();
	for (int i = 0; i != n; ++i)
	{
		hPSFFile[i] = CreateFileW(hPSF->PSF.get(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
		if (hPSFFile[i] == INVALID_HANDLE_VALUE)
		{
			DWORD Err = GetLastError();
			for (; i >= 0; --i)
				CloseHandle(hPSFFile[i]);
			SetLastError(Err);
			return FALSE;
		}
	}

	mutex Mutex;
	mutex* msgMutex = nullptr;
	if (flags & PSFEXTHANDLER_EXTRACT_FLAG_DISPATCH_MESSAGES_SYNCHRONOUSLY
		&& !(flags & PSFEXTHANDLER_EXTRACT_FLAG_SINGLE_THREAD))
		msgMutex = new mutex;

	BOOL Ret = TRUE;
	DWORD Err = ERROR_SUCCESS;
	DWORD totalbytes = 0;
	DWORD completed = 0;
	if (progress)
		for (DWORD i = 0; i != hPSF->FileCount; ++i)
			totalbytes += hPSF->Files[i].deltaSource.length;
	DWORD completedfiles = 0;
	MessageQueue<const PSFEXTHANDLER_EXPAND_INFO*>* queue = nullptr;
	if (!(flags & PSFEXTHANDLER_EXTRACT_FLAG_ALLOW_CALLING_PROGGRESS_PROC_NOT_ON_THE_MAIN_THREAD)
		&& !(flags & PSFEXTHANDLER_EXTRACT_FLAG_SINGLE_THREAD))
		queue = new MessageQueue<const PSFEXTHANDLER_EXPAND_INFO*>(flags & PSFEXTHANDLER_EXTRACT_FLAG_DISPATCH_MESSAGES_SYNCHRONOUSLY);

	omp_set_num_threads(n + 1);
#pragma omp parallel
	{
		int thread = omp_get_thread_num();
		if (thread)
			--thread;
		else
			if (!(flags & PSFEXTHANDLER_EXTRACT_FLAG_ALLOW_CALLING_PROGGRESS_PROC_NOT_ON_THE_MAIN_THREAD)
				&& !(flags & PSFEXTHANDLER_EXTRACT_FLAG_SINGLE_THREAD))
				goto ReportProgress;
			else
				goto END;

		{
			DWORD range = hPSF->FileCount / n;
			if (thread == n - 1)
				range = hPSF->FileCount - range * (n - 1);

			PSF psf;
			psf.FileCount = range + 1;
			psf.Files.reset(hPSF->Files.get() + hPSF->FileCount / n * thread);
			psf.hPSF = hPSFFile[thread];

			for (DWORD i = 0; i != range && !cancel; ++i)
			{
				DWORD ret = PSFExtHandler_ExtractFileToDirectoryByIndex(&psf, i, outdir, nullptr, flags & !PSFEXTHANDLER_EXTRACT_FLAG_ALLOW_CALLING_PROGGRESS_PROC_NOT_ON_THE_MAIN_THREAD);

				Mutex.lock();
				Ret &= ret;
				Mutex.unlock();

				DWORD err = GetLastError();
				if (err != ERROR_SUCCESS)
				{
					Mutex.lock();
					Err = err;
					Mutex.unlock();
				}
				if (!Ret && flags & PSFEXTHANDLER_EXTRACT_FLAG_CONTINUE_EVEN_IF_OPERATION_FAILS)
				{
					Mutex.lock();
					cancel = true;
					Mutex.unlock();
					break;
				}

				if (progress && ret)
				{
					Mutex.lock();
					++completedfiles;
					completed += psf.Files[i].deltaSource.length;
					Mutex.unlock();

					PSFEXTHANDLER_EXPAND_INFO ei;
					ei.dwIndex = range * thread + i;
					ei.pFileName = psf.Files[i].name.c_str();
					ei.dwCompletedBytes = completed;
					ei.dwCompletedFileCount = completedfiles;
					ei.dwCurrentFileSize = psf.Files[i].deltaSource.length;
					ei.dwTotalBytes = totalbytes;
					ei.dwTotalFileCount = hPSF->FileCount;

					if (!(flags & PSFEXTHANDLER_EXTRACT_FLAG_ALLOW_CALLING_PROGGRESS_PROC_NOT_ON_THE_MAIN_THREAD)
						&& !(flags & PSFEXTHANDLER_EXTRACT_FLAG_SINGLE_THREAD))
						queue->SendMessageVaule(
							flags & PSFEXTHANDLER_EXTRACT_FLAG_DISPATCH_MESSAGES_SYNCHRONOUSLY
							&& !(flags & PSFEXTHANDLER_EXTRACT_FLAG_SINGLE_THREAD) ? &ei : new decltype(ei)(ei));
					else
					{
						if (flags & PSFEXTHANDLER_EXTRACT_FLAG_DISPATCH_MESSAGES_SYNCHRONOUSLY
							&& !(flags & PSFEXTHANDLER_EXTRACT_FLAG_SINGLE_THREAD))
							msgMutex->lock();
						if (!progress(&ei, userdata))
						{
							Mutex.lock();
							cancel = true;
							Err = ERROR_CANCELLED;
							Mutex.unlock();
						}
						if (flags & PSFEXTHANDLER_EXTRACT_FLAG_DISPATCH_MESSAGES_SYNCHRONOUSLY
							&& !(flags & PSFEXTHANDLER_EXTRACT_FLAG_SINGLE_THREAD))
							msgMutex->unlock();
					}
				}
			}

			psf.Files.release();
		}
		if (!(flags & PSFEXTHANDLER_EXTRACT_FLAG_ALLOW_CALLING_PROGGRESS_PROC_NOT_ON_THE_MAIN_THREAD)
			&& !(flags & PSFEXTHANDLER_EXTRACT_FLAG_SINGLE_THREAD))
			if (completed == totalbytes
				|| cancel)
				queue->InvalidateQueue();

		goto END;

	ReportProgress:
		while (queue->ProcessMessage(
			[&](const PSFEXTHANDLER_EXPAND_INFO* ei)
			{
				if (!progress(ei, userdata))
				{
					Mutex.lock();
					cancel = true;
					Err = ERROR_CANCELLED;
					Mutex.unlock();
				}

				if (!(flags & PSFEXTHANDLER_EXTRACT_FLAG_DISPATCH_MESSAGES_SYNCHRONOUSLY))
					delete ei;
			}));
	END:
		;
	}

	for (int i = 0; i != n; ++i)
		CloseHandle(hPSFFile[i]);

	delete msgMutex;
	delete queue;

	SetLastError(Err);
	return Ret & !cancel;
}
