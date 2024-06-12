#pragma once
#include "pch.h"
#include <iostream>
#include <fstream>

using namespace std;

//enum InstructionSet { I, O, W, H, C };
//ָ����
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
	friend void RemoveProcess(PCB* pPCB, int i);//ɾ������
	friend void RunOneTimeRange(PCB* pPCB, int i);//����һ��ʱ���
	friend void TreadWaittingQueue(PCB* pWaittingPCBs);//����ĳ���ȴ����У���ʱ����ɽ����Ƴ����󱸾������С�
	friend void TreadAllWaittingQueues();
	friend void AppendReadyQueue(PCB* pPCB);//pPCB��ָ�ڵ���ӵ�������β
	friend void AppendWaitQueue(PCB* pPCB);//pPCB��ָ�ڵ���ӵ��ȴ���β
	friend void AppendInputQueue(PCB* pPCB);//pPCB��ָ�ڵ���ӵ�Input��β
	friend void AppendOutputQueue(PCB* pPCB);//pPCB��ָ�ڵ���ӵ�Output��β
	friend UINT ThreadProc(LPVOID lpParam);

private:
	CInstruction* m_pNextInstruction=NULL;//��������һ�����̵�����ָ���Ϊ����ָ�����У�
	int m_nRunTime;//��ָ����Ҫ���е�ʱ�䳤�ȣ�ʱ��Ƭ������
	CString m_nInstructionID;//ָ�����ͱ�ʶ

};

//���̿��ƿ���
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

	//����򴴽�һ������
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
	//���������һ��ָ��
	friend void RemoveProcess(PCB* pPCB, int i);//ɾ������
	friend void DoSchedule();//������־
	friend void RunOneTimeRange(PCB* pPCB, int i);//����һ��ʱ���
	friend void TreadWaittingQueue(PCB* pWaittingPCBs);//����ĳ���ȴ����У���ʱ����ɽ����Ƴ����󱸾������С�
	friend void TreadAllWaittingQueues();
	friend void AppendReadyQueue(PCB* pPCB);//pPCB��ָ�ڵ���ӵ�������β
	friend void AppendWaitQueue(PCB* pPCB);//pPCB��ָ�ڵ���ӵ��ȴ���β
	friend void AppendInputQueue(PCB* pPCB);//pPCB��ָ�ڵ���ӵ�Input��β
	friend void AppendOutputQueue(PCB* pPCB);//pPCB��ָ�ڵ���ӵ�Output��β
	friend UINT ThreadProc(LPVOID lpParam);
public:
	PCB* m_pNextPCB=NULL;					//���̶��е�ָ��
	int m_nPID;							//���̱�ʶ�� 
	CString m_csProcessName;	//��������
	int m_nRemainedTime;				//��ǰ����ָ�����л���Ҫ��ʱ��
	CInstruction* m_pRuningInstruction;	//ָ���������л�Ҫ���е�ָ��
	CInstruction* m_pInstructionList;	//ָ�򱾽��̵�ָ�����У����Ա��ĵ�һ��ָ��
};


PCB* m_pReadyPCBs;//��������
PCB* m_pBackupReadyPCBs;//�󱸾�������
PCB* m_pInputWaittingPCBs;//����ȴ�����
PCB* m_pOutputWaittingPCBs;//����ȴ�����
PCB* m_pPureWaittingPCBs;//�����ȴ�����
int m_nTimeSlice;//ʱ��Ƭ��С

void RemoveProcess(PCB* pPCB, int i);//ɾ������
void DoSchedule();//������־
void RunOneTimeRange(PCB* pPCB, int i);//����һ��ʱ���
void TreadWaittingQueue(PCB* pWaittingPCBs);//����ĳ���ȴ����У���ʱ����ɽ����Ƴ����󱸾������С�
void TreadAllWaittingQueues();
void AppendReadyQueue(PCB* pPCB);//pPCB��ָ�ڵ���ӵ�������β
void AppendWaitQueue(PCB* pPCB);//pPCB��ָ�ڵ���ӵ��ȴ���β
void AppendInputQueue(PCB* pPCB);//pPCB��ָ�ڵ���ӵ�Input��β
void AppendOutputQueue(PCB* pPCB);//pPCB��ָ�ڵ���ӵ�Output��β
UINT ThreadProc(LPVOID lpParam);
CFile ofs(L"test.txt",CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate);

void RemoveProcess(PCB* pPCB,int i)//ɾ������
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

void DoSchedule(ofstream ofs)//������־
{
	
}

void RunOneTimeRange(PCB* pPCB,int i)//����һ��ʱ���
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

void TreadWaittingQueue(PCB* pWaittingPCBs)//����ĳ���ȴ����У���ʱ����ɽ����Ƴ����󱸾������С�
{

}

void TreadAllWaittingQueues()
{

}


void AppendReadyQueue(PCB* pPCB)//pPCB��ָ�ڵ���ӵ�������β
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

void AppendWaitQueue(PCB* pPCB)//pPCB��ָ�ڵ���ӵ������ȴ���β
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

void AppendInputQueue(PCB* pPCB)//pPCB��ָ�ڵ���ӵ�Input��β
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

void AppendOutputQueue(PCB* pPCB)//pPCB��ָ�ڵ���ӵ�Output��β
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