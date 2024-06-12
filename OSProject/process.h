#pragma once
#include "pch.h"
#include <iostream>
#include <fstream>

using namespace std;

//enum InstructionSet { I, O, W, H, C };
//指令类
class CInstruction
{
	friend class COSProjectDlg;
	friend class PCB;
public:
	CInstruction()
	{
		m_pNextInstruction = NULL;
	}

	~CInstruction()
	{
	}

	CInstruction(CString iid, int rt)
	{
		m_nInstructionID = iid;
		m_nRunTime = rt;
	}
	friend void RemoveProcess(PCB* pPCB, int i);//删除进程
	friend void RunOneTimeRange(PCB* pPCB, int i);//运行一个时间段
	friend void TreadWaittingQueue(PCB* pWaittingPCBs);//处理某个等待队列，适时将完成进程移出到后备就绪队列。
	friend void TreadAllWaittingQueues();
	friend void AppendReadyQueue(PCB* pPCB);//pPCB所指节点添加到就绪队尾
	friend void AppendWaitQueue(PCB* pPCB);//pPCB所指节点添加到等待队尾
	friend void AppendInputQueue(PCB* pPCB);//pPCB所指节点添加到Input队尾
	friend void AppendOutputQueue(PCB* pPCB);//pPCB所指节点添加到Output队尾
	friend UINT ThreadProc(LPVOID lpParam);

private:
	CInstruction* m_pNextInstruction=NULL;//用于链接一个进程的所有指令成为链表（指令序列）
	int m_nRunTime;//本指令需要运行的时间长度（时间片个数）
	CString m_nInstructionID;//指令类型标识

};

//进程控制块类
class PCB
{
	friend class COSProjectDlg;
public:
	PCB()
	{
		m_nPID = 0;
		m_csProcessName = "";
		m_nRemainedTime = 0;//
		m_pRuningInstruction = NULL;

		m_pInstructionList = NULL;
		m_pNextPCB = NULL;

	}

	//构造或创建一个进程
	PCB(int pid, CString pname)
	{
		m_nPID = pid;
		m_csProcessName = pname;
		m_nRemainedTime = 0;//
		m_pRuningInstruction = NULL;

		m_pInstructionList = NULL;
		m_pNextPCB = NULL;

	}

	~PCB()
	{
		CInstruction* pTemp;
		while (m_pInstructionList)
		{
			m_pInstructionList = m_pInstructionList->m_pNextInstruction;
			pTemp = m_pInstructionList;
			delete pTemp;

		}
	}
	//本进程添加一条指令
	friend void RemoveProcess(PCB* pPCB, int i);//删除进程
	friend void DoSchedule();//生成日志
	friend void RunOneTimeRange(PCB* pPCB, int i);//运行一个时间段
	friend void TreadWaittingQueue(PCB* pWaittingPCBs);//处理某个等待队列，适时将完成进程移出到后备就绪队列。
	friend void TreadAllWaittingQueues();
	friend void AppendReadyQueue(PCB* pPCB);//pPCB所指节点添加到就绪队尾
	friend void AppendWaitQueue(PCB* pPCB);//pPCB所指节点添加到等待队尾
	friend void AppendInputQueue(PCB* pPCB);//pPCB所指节点添加到Input队尾
	friend void AppendOutputQueue(PCB* pPCB);//pPCB所指节点添加到Output队尾
	friend UINT ThreadProc(LPVOID lpParam);
public:
	PCB* m_pNextPCB=NULL;					//进程队列的指针
	int m_nPID;							//进程标识符 
	CString m_csProcessName;	//进程名字
	int m_nRemainedTime;				//当前运行指令运行还需要的时间
	CInstruction* m_pRuningInstruction;	//指向正在运行或将要运行的指令
	CInstruction* m_pInstructionList;	//指向本进程的指令序列（线性表）的第一条指令
};


PCB* m_pReadyPCBs;//就绪队列
PCB* m_pBackupReadyPCBs;//后备就绪队列
PCB* m_pInputWaittingPCBs;//输入等待队列
PCB* m_pOutputWaittingPCBs;//输出等待队列
PCB* m_pPureWaittingPCBs;//其他等待队列
int m_nTimeSlice;//时间片大小

void RemoveProcess(PCB* pPCB, int i);//删除进程
void DoSchedule();//生成日志
void RunOneTimeRange(PCB* pPCB, int i);//运行一个时间段
void TreadWaittingQueue(PCB* pWaittingPCBs);//处理某个等待队列，适时将完成进程移出到后备就绪队列。
void TreadAllWaittingQueues();
void AppendReadyQueue(PCB* pPCB);//pPCB所指节点添加到就绪队尾
void AppendWaitQueue(PCB* pPCB);//pPCB所指节点添加到等待队尾
void AppendInputQueue(PCB* pPCB);//pPCB所指节点添加到Input队尾
void AppendOutputQueue(PCB* pPCB);//pPCB所指节点添加到Output队尾
UINT ThreadProc(LPVOID lpParam);
CFile ofs(L"test.txt",CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate);

void RemoveProcess(PCB* pPCB,int i)//删除进程
{
	switch (i) {
	case 1:
		if (m_pReadyPCBs == pPCB) {
			if (m_pReadyPCBs == NULL) {
				return;
			}
			else {
				PCB* next = pPCB->m_pNextPCB;
				m_pReadyPCBs = next;
			}
			return;
		}
		else {
			PCB* P = m_pReadyPCBs;
			while (P->m_pNextPCB != pPCB) {
				P = P->m_pNextPCB;
			}
			P->m_pNextPCB = pPCB->m_pNextPCB;
		}
		break;
	case 2:
		if (m_pInputWaittingPCBs == pPCB) {
			if (m_pInputWaittingPCBs == NULL) {
				return;
			}
			else {
				PCB* next = m_pInputWaittingPCBs->m_pNextPCB;
				m_pInputWaittingPCBs = next;
			}
			return;
		}
		else {
			PCB* P = m_pInputWaittingPCBs;
			while (P->m_pNextPCB != pPCB) {
				P = P->m_pNextPCB;
			}
			P->m_pNextPCB = pPCB->m_pNextPCB;
		}
		break;
	case 3:
		if (m_pOutputWaittingPCBs == pPCB) {
			if (m_pOutputWaittingPCBs == NULL) {
				return;
			}
			else {
				PCB* next = m_pOutputWaittingPCBs ->m_pNextPCB;
				m_pOutputWaittingPCBs = next;
			}
			return;
		}
		else {
			PCB* P = m_pOutputWaittingPCBs;
			while (P->m_pNextPCB != pPCB) {
				P = P->m_pNextPCB;
			}
			P->m_pNextPCB = pPCB->m_pNextPCB;
		}
		break;
	case 4:
		if (m_pPureWaittingPCBs == pPCB) {
			if (m_pPureWaittingPCBs == NULL) {
				return;
			}
			else {
				PCB* next = m_pPureWaittingPCBs->m_pNextPCB;
				m_pPureWaittingPCBs = next;
			}
			return;
		}
		else {
			PCB* P = m_pPureWaittingPCBs;
			while (P->m_pNextPCB != pPCB) {
				P = P->m_pNextPCB;
			}
			P->m_pNextPCB = pPCB->m_pNextPCB;
		}
		break;
	}
}

void DoSchedule(ofstream ofs)//生成日志
{
	
}

void RunOneTimeRange(PCB* pPCB,int i)//运行一个时间段
{
	if (pPCB == NULL) return;
	if (pPCB->m_pInstructionList == NULL) return;
	pPCB->m_pRuningInstruction = pPCB->m_pInstructionList;
	pPCB->m_pRuningInstruction->m_nRunTime= pPCB->m_pRuningInstruction->m_nRunTime-1;
	CString str1 = _T(" ");
	ofs.Write(pPCB->m_csProcessName.GetBuffer(), (pPCB->m_csProcessName.GetLength())*2);
	ofs.Write(pPCB->m_pInstructionList->m_nInstructionID.GetBuffer(), (pPCB->m_pInstructionList->m_nInstructionID.GetLength())*2);
	ofs.Write(str1.GetBuffer(), str1.GetLength());
	if (pPCB->m_pRuningInstruction->m_nRunTime == 0) {
		if (pPCB->m_pRuningInstruction->m_pNextInstruction == NULL) {
			pPCB->m_pInstructionList = NULL;
			pPCB->m_pRuningInstruction = NULL;
			RemoveProcess(pPCB, i);
		}
		else {
			pPCB->m_pInstructionList = pPCB->m_pInstructionList->m_pNextInstruction;
			if (pPCB->m_pInstructionList->m_nInstructionID == 'C') {
				RemoveProcess(pPCB, i);
				AppendReadyQueue(pPCB);
			}
			if (pPCB->m_pInstructionList->m_nInstructionID == 'I') {
				RemoveProcess(pPCB, i);
				AppendInputQueue(pPCB);
			}
			if (pPCB->m_pInstructionList->m_nInstructionID == 'O') {
				RemoveProcess(pPCB, i);
				AppendOutputQueue(pPCB);
			}
			if (pPCB->m_pInstructionList->m_nInstructionID == 'W') {
				RemoveProcess(pPCB, i);
				AppendWaitQueue(pPCB);
			}
			if (pPCB->m_pInstructionList->m_nInstructionID == 'H') {
				RemoveProcess(pPCB, i);
			}
		}
	}
	else {
		if (i == 1) {
			RemoveProcess(pPCB, i);
			AppendReadyQueue(pPCB);
		}
	}
	return;
}

void TreadWaittingQueue(PCB* pWaittingPCBs)//处理某个等待队列，适时将完成进程移出到后备就绪队列。
{

}

void TreadAllWaittingQueues()
{

}


void AppendReadyQueue(PCB* pPCB)//pPCB所指节点添加到就绪队尾
{
	if (m_pReadyPCBs == NULL) {
		m_pReadyPCBs = pPCB;
		m_pReadyPCBs->m_pNextPCB = NULL;
	}
	else {
		PCB* P = new PCB();
		P = m_pReadyPCBs;
		while (P->m_pNextPCB != NULL) {
			P = P->m_pNextPCB;
		}
		P->m_pNextPCB = pPCB;
		P->m_pNextPCB->m_pNextPCB = NULL;
	}
}

void AppendWaitQueue(PCB* pPCB)//pPCB所指节点添加到其他等待队尾
{
	pPCB->m_pNextPCB = NULL;
	if (m_pPureWaittingPCBs == NULL) {
		m_pPureWaittingPCBs = pPCB;
		m_pPureWaittingPCBs->m_pNextPCB = NULL;
	}
	else {
		PCB* P = new PCB();
		P = m_pPureWaittingPCBs;
		while (P->m_pNextPCB != NULL) {
			P = P->m_pNextPCB;
		}
		P->m_pNextPCB = pPCB;
		P->m_pNextPCB->m_pNextPCB = NULL;
	}
}

void AppendInputQueue(PCB* pPCB)//pPCB所指节点添加到Input队尾
{
	pPCB->m_pNextPCB = NULL;
	if (m_pInputWaittingPCBs == NULL) {
		m_pInputWaittingPCBs = pPCB;
		m_pInputWaittingPCBs->m_pNextPCB = NULL;
	}
	else {
		PCB* P = new PCB();
		P = m_pInputWaittingPCBs;
		while (P->m_pNextPCB != NULL) {
			P = P->m_pNextPCB;
		}
		P->m_pNextPCB = pPCB;
		P->m_pNextPCB->m_pNextPCB = NULL;
	}
}

void AppendOutputQueue(PCB* pPCB)//pPCB所指节点添加到Output队尾
{
	pPCB->m_pNextPCB = NULL;
	if (m_pOutputWaittingPCBs == NULL) {
		m_pOutputWaittingPCBs = pPCB;
		m_pOutputWaittingPCBs->m_pNextPCB = NULL;
	}
	else {
		PCB* P = new PCB();
		P = m_pOutputWaittingPCBs;
		while (P->m_pNextPCB != NULL) {
			P = P->m_pNextPCB;
		}
		P->m_pNextPCB = pPCB;
		P->m_pNextPCB->m_pNextPCB = NULL;
	}
}