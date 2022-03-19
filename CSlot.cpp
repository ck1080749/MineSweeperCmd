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
	flagsNearBy = 0;
	slotX = 0;
	slotY = 0;
	//displayStyle = 'U';//U = Undigged, F = Flagged, number = digged, B = failed bomb display
}

CSlot::~CSlot()
{
	delete[] slotsNearBy;
}

void CSlot::OnRightClick(int& flagsRemain)
{
	if (isFlagPlanted) {
		flagsRemain++; 
		isFlagPlanted = false;
	}
	else {
		flagsRemain--;
		isFlagPlanted = true;
	}
}

bool CSlot::OnLeftClick()//回傳true == 安全挖掘||非法挖掘
{
	//printf("This is ( %d, %d)  calling OnLeftClick()\n", slotX, slotY);
	if (!isFlagPlanted) {
		isDigged = true;
		if (isBombHere)return false;
		if (bombsNearBy == 0) {
			for (int i = 0; i < 8; i++) {
				if (slotsNearBy[i] != NULL) slotsNearBy[i]->OnPassiveClick();
			}
		}
		else {
			for (int i = 0; i < 8; i++) {
				if (slotsNearBy[i] != NULL) {
					if (slotsNearBy[i]->bombsNearBy == 0 && slotsNearBy[i]->isDigged) slotsNearBy[i]->OnPassiveClick();
				}
			}
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
	if (bombsNearBy != 0) {
		for (int i = 0; i < 8; i++) {
			if (slotsNearBy[i] != NULL) {
				if (slotsNearBy[i]->bombsNearBy == 0) {
					isDigged = true;
					return;
				}
			}
		}
	}
	else {
		isDigged = true;
		for (int i = 0; i < 8; i++) {
			if (slotsNearBy[i] != NULL) {
				slotsNearBy[i]->OnPassiveClick();
			}
		}
	}
}

bool CSlot::OnBothClick()
{
	FlagCountChecker();
	if (flagsNearBy == bombsNearBy) {
		for (int i = 0; i < 8; i++) {
			if (slotsNearBy[i] != NULL) {
				if (slotsNearBy[i]->isDigged)continue;
				else if (slotsNearBy[i]->isFlagPlanted)continue;
				else if (slotsNearBy[i]->isBombHere)continue;
				else slotsNearBy[i]->OnLeftClick();
			}
		}
		return true;
	}
	return false;
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

bool CSlot::SamePos(CSlot* comparewith)
{
	return (this->slotX == comparewith->slotX) && (this->slotY == comparewith->slotY);
}

