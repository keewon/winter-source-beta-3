#ifndef _MSG_QUEUE_OEDALPHA_H_
#define _MSG_QUEUE_OEDALPHA_H_

// Message Queue
// 네트워크로 부터 오는 메시지를 담아서 사용하려고 만든 Queue입니다.
// Character Array 기반 Circular Queue에요.
// 아 머리빠져...

// 나중을 위해
// Critical_Section 부분은 옵션으로 할 수 있도록 하였습니다.
#define _MSG_QUEUE_USING_CRITICAL_SECTION_
// #define을 주석처리하면 CriticalSection 쪽은 사라집니다

#include <cstring>
#ifdef _MSG_QUEUE_USING_CRITICAL_SECTION_
#include <windows.h>
#endif

#include "protocol.h"

class MsgQueue {
	private:
		char* szBuffer;
		int nSize, nWritePointer, nReadPointer;
#ifdef _MSG_QUEUE_USING_CRITICAL_SECTION_
		CRITICAL_SECTION lockOfQueue;
#endif
	public:
		MsgQueue(int num);	  // 첨에 사용하기 전에 Buffer크기 정해주세요
		~MsgQueue();		  // 소멸자
		bool Push(void*, int);	  // 큐에 자료 넣기
		bool Pop (void*, int);	  // 큐에서 자료 빼내기
		void Empty();		  // 큐 청소하기
		int Size();		  // 큐에 들어있는 자료 크기
		CMD ReadCmd();		  // 현재 제일 앞부분 리턴
};

#endif
