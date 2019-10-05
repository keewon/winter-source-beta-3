// Message Queue
// 네트워크로 부터 오는 메시지를 담아서 사용하려고 만든 Queue입니다.
// 단순::반복적인 코드입니다 디버그할 걱정을 하니 골치가 아프군요
// 다행히 기초적인 테스트는 했네요 잘되는 것 같기도..
// 2000 겨울프로젝트 할 때에 서기원 만듦

#include "MsgQueue.h"

MsgQueue::
MsgQueue(int num) : 
nSize(num), nWritePointer(0), nReadPointer(0) {
	szBuffer = new char[nSize];
	memset(szBuffer,0,nSize);
#ifdef _MSG_QUEUE_USING_CRITICAL_SECTION_
InitializeCriticalSection(&lockOfQueue);
#endif
}
MsgQueue::
~MsgQueue() {
	delete [] szBuffer;
#ifdef _MSG_QUEUE_USING_CRITICAL_SECTION_
DeleteCriticalSection(&lockOfQueue);
#endif
}
bool 
MsgQueue::
Push(void* lpvData, int nDataSize) {
	if (nDataSize > nSize) return false;
	bool bReturnValue = false;

#ifdef _MSG_QUEUE_USING_CRITICAL_SECTION_
__try { EnterCriticalSection(&lockOfQueue);
#endif
	// WritePointer가 ReadPointer의 뒤에 있으면
	if (nWritePointer >= nReadPointer) {
		// WritePointer 뒤에 남은 공간을 계산
		int nBackSize = nSize-nWritePointer;
		// Buffer가 충분하면 시키는 대로 하지만 
		if (nReadPointer+nBackSize >= nDataSize) {
			if (nBackSize >= nDataSize) {
				memcpy(szBuffer+nWritePointer, lpvData, nDataSize);
				nWritePointer+=nDataSize;
			} else {
				// 앞과 뒤에 나누어서 자료를 싣는 Circular Queue
				memcpy(szBuffer+nWritePointer, lpvData, nBackSize);
				memcpy(szBuffer, ((char*)lpvData)+nBackSize, 
						nDataSize-nBackSize);
				nWritePointer=nDataSize-nBackSize;
			}
			bReturnValue = true;
		}
	} else {
	// WritePointer가 ReadPointer의 앞에 있으면
		int nFreeSize = nReadPointer - nWritePointer;
		if (nFreeSize >= nDataSize) {
			memcpy(szBuffer+nWritePointer, lpvData, nDataSize);
			nWritePointer+= nDataSize;
			bReturnValue = true;
		}
	}

#ifdef _MSG_QUEUE_USING_CRITICAL_SECTION_
} __finally { LeaveCriticalSection(&lockOfQueue); }
#endif
	return bReturnValue;
}

bool 
MsgQueue::
Pop(void* lpvData, int nDataSize) {
	if (nDataSize > nSize) return false;
	bool bReturnValue = false;
#ifdef _MSG_QUEUE_USING_CRITICAL_SECTION_
__try { EnterCriticalSection(&lockOfQueue);
#endif
	// WritePointer가 ReadPointer의 뒤에 있을 때에
	// 별 문제 없이 그냥 써주면 된다.
	if (nWritePointer > nReadPointer) {
		int nValidSize = nWritePointer-nReadPointer;
		if (nValidSize >= nDataSize) {
			memcpy(lpvData, szBuffer+nReadPointer, nDataSize);
			nReadPointer+= nDataSize;
			bReturnValue = true;
		}
	} else {
	// WritePointer가 ReadPointer의 앞에 있을 때에
		int nBackSize = nSize - nReadPointer;
		if (nBackSize+nWritePointer >= nDataSize) {
			if (nBackSize >= nDataSize) {
				memcpy(lpvData, szBuffer+nReadPointer, nDataSize);
				nReadPointer+= nDataSize;
				bReturnValue = true;
			}
			else {
				// 뒷쪼가리 읽기
				memcpy(lpvData, szBuffer+nReadPointer, nBackSize);
				// 앞쪼가리 읽기
				memcpy(((char*)lpvData)+nBackSize, szBuffer,
						nDataSize-nBackSize);
				nReadPointer = nDataSize-nBackSize;
				bReturnValue = true;
			}
		}
	}

#ifdef _MSG_QUEUE_USING_CRITICAL_SECTION_
} __finally { LeaveCriticalSection(&lockOfQueue); }
#endif
	return bReturnValue;
}

int 
MsgQueue::
Size() {
	int nValue = -1;
#ifdef _MSG_QUEUE_USING_CRITICAL_SECTION_
__try { EnterCriticalSection(&lockOfQueue);
#endif

	nValue = nWritePointer>=nReadPointer ?
				nWritePointer-nReadPointer : 
				nSize + nWritePointer - nReadPointer;
#ifdef _MSG_QUEUE_USING_CRITICAL_SECTION_
} __finally { LeaveCriticalSection(&lockOfQueue); }
#endif

	return nValue;

}

void
MsgQueue::
Empty() {
#ifdef _MSG_QUEUE_USING_CRITICAL_SECTION_
__try { EnterCriticalSection(&lockOfQueue);
#endif

    nWritePointer = 0; nReadPointer = 0;
    memset(szBuffer,0,nSize);

#ifdef _MSG_QUEUE_USING_CRITICAL_SECTION_
} __finally { LeaveCriticalSection(&lockOfQueue); }
#endif
}

CMD
MsgQueue::
ReadCmd() {
	CMD c = np_CMD::CMD_NIL;
	if (Size() <= 0) return c;
#ifdef _MSG_QUEUE_USING_CRITICAL_SECTION_
__try { EnterCriticalSection(&lockOfQueue);
#endif

	if (nReadPointer != nWritePointer)	c = (CMD)(szBuffer[nReadPointer]);

#ifdef _MSG_QUEUE_USING_CRITICAL_SECTION_
} __finally { LeaveCriticalSection(&lockOfQueue); }
#endif
	return c;
}
