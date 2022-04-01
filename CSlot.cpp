#include "CSlot.h"
#include <stdio.h>

void CSlot::FlagCountChecker()
{
	flagsNearBy = 0;
	for (int i = 0; i < 8; i++) {
		if(slotsNearBy[i]) 
			if (slotsNearBy[i]->isFlagPlanted)flagsNearBy++;
	}
}

CSlot::CSlot()
{
	slotsNearBy = new CSlot*[8];
	isFlagPlanted = false;
	isDigged = false;
	bombsNearBy = 0;
	isBombHere = false;
	isUncertain = false;
	flagsNearBy = 0;
	slotX = 0;
	slotY = 0;
	//displayStyle = 'U';//U = Undigged, F = Flagged, number = digged, B = failed bomb display
}

CSlot::~CSlot()
{
	delete[] slotsNearBy;
}

void CSlot::OnRightClick(int& flagsRemain)//���X�Ω޺X
{
	if (isFlagPlanted) {
		flagsRemain++; 
		isFlagPlanted = false;
		isUncertain = true;
	}
	else if (!isUncertain){
		if (flagsRemain == 0) {
			printf("No more flag left!\t");
			return;
		}
		flagsRemain--;
		isFlagPlanted = true;
	}
	else {
		isUncertain = false;
	}
}

bool CSlot::OnLeftClick()//�^��true == �w������||�D�k����
{
	//printf("This is ( %d, %d)  calling OnLeftClick()\n", slotX, slotY);
	if (!isFlagPlanted) {
		isDigged = true;
		if (isBombHere)return false;
		if (bombsNearBy == 0) {
			for (int i = 0; i < 8; i++) {
				if (slotsNearBy[i] != NULL) 
					slotsNearBy[i]->OnPassiveClick();
			}
		}
		else {
			/*
			for (int i = 0; i < 8; i++) {//????????
				if (slotsNearBy[i] != NULL) {
					if (slotsNearBy[i]->bombsNearBy == 0) slotsNearBy[i]->OnPassiveClick();
				}
			}
			*/
		}
		return true;
	}
	else {
		puts("invalid!");
		return true;
	}
}

void CSlot::OnPassiveClick()
{
	//printf("This is ( %d, %d)  calling OnPassiveClick()\n", slotX, slotY);
	if (isFlagPlanted)return;
	if (isBombHere)return;
	if (isDigged)return;
	isDigged = true;//�@�w���i��O���u�A�������}�S�t
	if (bombsNearBy != 0) {
		/*
		for (int i = 0; i < 8; i++) {//??????
			if (slotsNearBy[i] != NULL) {
				if (slotsNearBy[i]->bombsNearBy == 0) {
					isDigged = true;
					return;
				}
			}
		}
		*/
		return;
	}
	else {
		for (int i = 0; i < 8; i++) {
			if (slotsNearBy[i] != NULL) {
				slotsNearBy[i]->OnPassiveClick();
			}
		}
	}
}

/*
bool CSlot::OnBothClick()
{
	//�]��onBothClick�^�ǭȬ��u�O�_�ާ@���\�v�ӫD�u�O�_�z���v�A�ݭn�勵���I���I�h
	FlagCountChecker();
	if (flagsNearBy == bombsNearBy) {
		for (int i = 0; i < 8; i++) {
			if (slotsNearBy[i] != NULL) {
				if (slotsNearBy[i]->isDigged)continue;
				else if (slotsNearBy[i]->isFlagPlanted)continue;
				else if (slotsNearBy[i]->isBombHere)continue;//Here may remove
				else slotsNearBy[i]->OnLeftClick();
			}
		}
		return true;
	}
	return false;
}*/

bool CSlot::OnBothClick() {
	FlagCountChecker();
	if (flagsNearBy == bombsNearBy) {
		for (int i = 0; i < 8; i++) {
			if (slotsNearBy[i] != NULL) {
				if (slotsNearBy[i]->isDigged)continue;
				else if (slotsNearBy[i]->isFlagPlanted)continue;
				//else if (slotsNearBy[i]->isBombHere)continue;//Here may remove
				if (!slotsNearBy[i]->OnLeftClick()) {
					return false;
				}
			}
		}
	}
	else {
		printf("You haven't flag enough yet!\n");
	}
	return true;
}

void CSlot::SetBombHere()
{
	isBombHere = true;
}

void CSlot::BombCountChecker()
{
	for (int i = 0; i < 8; i++) {
		if(slotsNearBy[i] != NULL)
			if (slotsNearBy[i]->isBombHere)bombsNearBy++;
	}
}

int CSlot::BombsNearBy()
{
	return bombsNearBy;
}

void CSlot::SetSlotPos(int x, int y)
{
	slotX = x;
	slotY = y;
}

//deprecated
/*void CSlot::SetNearbySlot(CSlot* s, ESlotPos pos)//deprecated
{
	slotsNearBy[pos] = s;
}*/

bool CSlot::IsDigged()
{
	return isDigged;
}

bool CSlot::IsFlagPlanted()
{
	return isFlagPlanted;
}

bool CSlot::IsBombHere()
{
	return isBombHere;
}

bool CSlot::IsUncertain()
{
	return isUncertain;
}



bool CSlot::SamePos(CSlot* comparewith)
{
	return (this->slotX == comparewith->slotX) && (this->slotY == comparewith->slotY);
}

