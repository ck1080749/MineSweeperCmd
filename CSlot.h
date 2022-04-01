#pragma once
enum ESlotPos {
	T = 0, TR, R, BR, B, BL, L, TL
};
class CSlot
{
private:
	bool isBombHere;
	bool isFlagPlanted;
	bool isDigged;
	int bombsNearBy;
	int flagsNearBy;
	int slotX;
	int slotY;
	void FlagCountChecker();
	void OnPassiveClick();//�۰ʫ�
	bool isUncertain;
	//char displayStyle;
public:
	CSlot** slotsNearBy;
	CSlot();
	~CSlot();
	void OnRightClick(int&);//��X�l
	bool OnLeftClick();//��
	bool OnBothClick();//������K��]���X�l�ҥ~�^
	void SetBombHere();
	void BombCountChecker();
	int BombsNearBy();
	void SetSlotPos(int x, int y);
	//void SetNearbySlot(CSlot* s, enum ESlotPos);//deprecated
	//void OnGameFailed();
	bool IsDigged();
	bool IsFlagPlanted();
	bool IsBombHere();
	bool IsUncertain();
	bool SamePos(CSlot* comparewith);
};

namespace SuperSecretDeveloperFunctions {

}
