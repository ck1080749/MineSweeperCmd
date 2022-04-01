// MineSweeperCmd.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include <iostream>
#include "CSlot.h"
#include <time.h>
#include <stdlib.h>
#include <string>
using namespace std;

struct MennSize
{
    int length; int width; int bombCount;
};
typedef struct MennSize _size;

const string VER = "v1.1-alpha";
const _size EASY = { 8, 8, 10};//minium
const _size NORMAL = { 16, 16, 40};
const _size HARD = { 16, 30, 99};
const _size MAX = { 24, 30, 668};//自訂版面的上限大小
CSlot** menn;
_size s;

void Init() {
    CSlot temp;

    menn = new CSlot*[s.length];
    for (int i = 0; i < s.length; i++)menn[i] = new CSlot[s.width];

    for (int i = 0; i < s.length; i++) {//l = row = y = i = t/b, w = col = x = j = l/r
        for (int j = 0; j < s.width; j++) {
            menn[i][j].SetSlotPos(j, i);
            for (int k = 0; k < 8; k++) {
                menn[i][j].slotsNearBy[k] = &temp;
            }
            if (i == 0) {//top
                menn[i][j].slotsNearBy[TL] = NULL;
                menn[i][j].slotsNearBy[TR] = NULL;
                menn[i][j].slotsNearBy[T] = NULL;
            }
            if (i == s.length - 1) {//bot
                menn[i][j].slotsNearBy[BR] = NULL;
                menn[i][j].slotsNearBy[B] = NULL;
                menn[i][j].slotsNearBy[BL] = NULL;
            }

            if (j == 0) {//left
                menn[i][j].slotsNearBy[TL] = NULL;
                menn[i][j].slotsNearBy[L] = NULL;
                menn[i][j].slotsNearBy[BL] = NULL;
            }
            if (j == s.width - 1) {//right
                menn[i][j].slotsNearBy[TR] = NULL;
                menn[i][j].slotsNearBy[R] = NULL;
                menn[i][j].slotsNearBy[BR] = NULL;
            }
            
            if (menn[i][j].slotsNearBy[T] == &temp)menn[i][j].slotsNearBy[T] = &menn[i - 1][j];
            if (menn[i][j].slotsNearBy[TR] == &temp)menn[i][j].slotsNearBy[TR] = &menn[i - 1][j + 1];
            if (menn[i][j].slotsNearBy[R] == &temp)menn[i][j].slotsNearBy[R] = &menn[i][j + 1];
            if (menn[i][j].slotsNearBy[BR] == &temp)menn[i][j].slotsNearBy[BR] = &menn[i + 1][j + 1];
            if (menn[i][j].slotsNearBy[B] == &temp)menn[i][j].slotsNearBy[B] = &menn[i + 1][j];
            if (menn[i][j].slotsNearBy[BL] == &temp)menn[i][j].slotsNearBy[BL] = &menn[i + 1][j - 1];
            if (menn[i][j].slotsNearBy[L] == &temp)menn[i][j].slotsNearBy[L] = &menn[i][j - 1];
            if (menn[i][j].slotsNearBy[TL] == &temp)menn[i][j].slotsNearBy[TL] = &menn[i - 1][j - 1];
        }
    }
}

void FirstLeftClick(CSlot& clicked) {//set bomb
    //srand(time(NULL));
    int heng, shu;//menn[shu][heng] menn[length][width]
    for (int i = 0; i < s.bombCount; i++) {
        srand(time(NULL));
        do {
            shu = rand() % s.length;
            heng = rand() % s.width;
            if (menn[shu][heng].IsBombHere())continue;
            else if (clicked.SamePos(&menn[shu][heng]))continue;
            else break;
        } while (true);
        menn[shu][heng].SetBombHere();
    }
    for (int i = 0; i < s.length; i++) {
        for (int j = 0; j < s.width; j++) {
            menn[i][j].BombCountChecker();
        }
    }
    clicked.OnLeftClick();
}

void PrintMenn() {//U = Undigged, F = Flagged, number = digged, B = failed bomb display
    for (int i = 0; i < s.length; i++) {
        for (int j = 0; j < s.width; j++) {
            if (menn[i][j].IsFlagPlanted()) {
                cout << "F";
            }else if (menn[i][j].IsDigged()) {
                if (menn[i][j].BombsNearBy() == 0)cout << " ";
                else cout << menn[i][j].BombsNearBy();
            }
            else if (menn[i][j].IsUncertain()) {
                cout << "?";
            }
            else {
                cout << "U";
            }
        }
        cout << endl;
    }
}

void PrintCheatMenn() {//U = Undigged, F = Flagged, number = digged, B = failed bomb display
    for (int i = 0; i < s.length; i++) {
        for (int j = 0; j < s.width; j++) {
            if (menn[i][j].IsBombHere()) cout << "B";
            else if (menn[i][j].BombsNearBy() == 0)cout << " ";
            else cout << menn[i][j].BombsNearBy();
        }
        cout << endl;
    }
}

void InGameMenu(char& opt) {
    cout << "Operations: \n"
        << "1. Dig a slot\n"
        << "2. Mark/Unmark a slot\n"
        << "3. Dig nearby slots\n"
        << "4. Quit without Finishing the game\n"
        << "> ";
    cin >> opt;
}

void PrintFailedMenn() {
    system("cls");
    PrintMenn();
    cout << "-------------------------------------------------------\n";
    for (int i = 0; i < s.length; i++) {
        for (int j = 0; j < s.width; j++) {
            if (menn[i][j].IsBombHere() && menn[i][j].IsFlagPlanted()) cout << "F";
            else if(menn[i][j].IsBombHere() && !menn[i][j].IsFlagPlanted())cout << "B";
            else if (!menn[i][j].IsBombHere() && menn[i][j].IsFlagPlanted())cout << "X";
            else if (menn[i][j].BombsNearBy() == 0)cout << " ";
            else cout << menn[i][j].BombsNearBy();
        }
        cout << endl;
    }
    cout << "-------------------------------------------------------\n";
    cout << "FAILED!\n";
}

void EndGame(bool win, clock_t timeTaken) {//game Finished
    if (win)cout << "You Win!\n";
    for (int i = 0; i < s.length; i++)delete[] menn[i];
    delete[] menn;
    cout << "Used " << timeTaken << " seconds.\n";
}

void EndGame(clock_t timeTaken) {//game interrupted
    for (int i = 0; i < s.length; i++)delete[] menn[i];
    delete[] menn;
    cout << "Used " << timeTaken << " seconds.\n";
}

bool MennChecker() {
    // two end scenarios:
    // 1. all bombed slots are flagged
    // 2. all unbombed slots are digged
    // 1. || 2. -> win
    bool flagNotFinished = false;
    bool digNotFinished = false;
    for (int i = 0; i < s.length; i ++) {
        for (int j = 0; j < s.width; j++) {
            if (!flagNotFinished) {
                 if (menn[i][j].IsBombHere() && !menn[i][j].IsFlagPlanted()) {
                     flagNotFinished = true;
                 }
            }
            if (!digNotFinished) {
                if (!menn[i][j].IsBombHere() && !menn[i][j].IsDigged()) {
                    digNotFinished = true;
                }
            }
            if (flagNotFinished && digNotFinished) return false;
        }
    }
    return true;
}

//row橫 column直
int main()
{
    //pre
    char option = '0';
    do {
        system("cls");
        cout << "BAD MINESWEEPER v" << VER << endl;
        cout << "Choose Difficulty:\n"
            << "1. EASY\n"
            << "2. NORMAL\n"
            << "3. HARD\n"
            << "4. CUSTOMIZE\n"
            << "5. QUIT\n"
            << "> ";
        cin >> option;
        switch (option)
        {
        case '1':
            s = EASY;
            break;
        case '2':
            s = NORMAL;
            break;
        case '3':
            s = HARD;
            break;
        case '4':
            do {
                cout << "row > ";
                cin >> s.length;
                if (s.length > MAX.length || s.length < EASY.length) {
                    cout << "INVALID!\n";
                    continue;
                }
                break;
            } while (true);

            do {
                cout << "column > ";
                cin >> s.width;
                if (s.width > MAX.width || s.width < EASY.width) {
                    cout << "INVALID!\n";
                    continue;
                }
                break;
            } while (true);

            do {
                cout << "bomb count > ";
                cin >> s.bombCount;
                if (s.bombCount > MAX.bombCount || s.bombCount < EASY.bombCount) {
                    cout << "INVALID!\n";
                    continue;
                }
                break;
            } while (true);
            break;
        case '5':
            cout << "BYE!\n";
            return 0;
        default:
            cout << "INVALID!\n";
            continue;
        }
        break;
    } while (true);
    option = '0';
    clock_t start, end;
    bool win = true;
    Init();

    //in
    int operations = 0; // 動作次數
    int x = 0, y = 0;
    int flagCounter = s.bombCount;
    start = clock()/1000;

    do {
        if (operations)system("pause");
        system("cls");
        PrintMenn();
        cout << "-------------------------------------------------------\n";
        PrintCheatMenn();
        //cout << "-------------------------------------------------------\n";
        if (operations) {
            cout << "Previous operation: ";
            switch (option)
            {
            case '1':
                cout << "Digged slot ( " << x << ", " << y << ")\n";
                break;
            case '2':
                cout << "Marked/Unmarked slot ( " << x << ", " << y << ")\n";
                break;
            case '3':
                cout << "Digged slots around ( " << x << ", " << y << ")\n";
                break;
            default:
                cout << "none\n";
                break;
            }
            cout << flagCounter << " Bombs/flags left." << endl;
        }
        InGameMenu(option);
        if (option == '4') {
            cout << "Are you sure about that?\nEnter \'y\' to confirm> ";
            cin >> option;
            if(option == 'y'||option  == 'Y'){
                cout << "You used " << operations << " operations.\n";
                end = clock()/1000;
                EndGame(end - start);
                cout << "Bye!\n";
                system("pause");
                return 0;
            }
        }
        else if (option == '1') {
            cout << "Which slot? ";
            cin >> x >> y;
            if (x >= s.width || y >= s.length) {
                cout << "INVALID!\n";
                continue;
            }
            else if (x < 0 || y < 0) {
                cout << "INVALID!\n";
                continue;
            }
            else if (!operations) {
                FirstLeftClick(menn[y][x]);
            }
            else if (menn[y][x].IsDigged()) {//menn[row count][col count] == menn[直][橫]
                cout << "The slot you selected is already digged!\n";
                continue;
            }
            else if (menn[y][x].IsFlagPlanted()) {
                cout << "The slot you selected is already flagged!\n";
                continue;
            }
            else {
                if (!menn[y][x].OnLeftClick()) {
                    win = false;
                    PrintFailedMenn();
                    break;
                }   
            }
        }
        else if (option == '2') {
            cout << "Which slot? ";
            cin >> x >> y;
            if (x >= s.width || y >= s.length) {
                cout << "INVALID!\n";
                continue;
            }
            else if (x < 0 || y < 0) {
                cout << "INVALID!\n";
                continue;
            }
            else if (!operations) {
                cout << "You Have to Dig a slot first!\n";
                continue;
            }
            else if (menn[y][x].IsDigged()) {//menn[row count][col count] == menn[直][橫]
                cout << "The slot you selected is already digged!\n";
                continue;
            }
            else {
                menn[y][x].OnRightClick(flagCounter);
            }
        }
        else if (option == '3') {
            cout << "Which slot? ";
            cin >> x >> y;
            if (x >= s.width || y >= s.length) {
                cout << "INVALID!\n";
                continue;
            }
            else if (x < 0 || y < 0) {
                cout << "INVALID!\n";
                continue;
            }
            else if (!operations) {
                cout << "You Have to Dig a slot first!\n";
                continue;
            }
            else if (menn[y][x].IsDigged()) {//menn[row count][col count] == menn[直][橫]
                if (!menn[y][x].OnBothClick()) {//fail
                    //cout << "You haven't flag enough yet!\n";
                    //continue;
                    win = false;
                    PrintFailedMenn();
                    break;
                }
            }
            else {
                cout << "You Have to Select a digged slot!\n";
                continue;
            }
        }
        else{
            cout << "INVALID!\n";
            continue;
        }
        operations++;
    } while (!MennChecker());
    end = clock()/1000;
    
    //post
    EndGame(win, end - start);
    system("pause");
    return 0;
}
