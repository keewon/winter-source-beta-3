#ifndef LOCAL_PLAYER_H_OEDALPHA
#define LOCAL_PLAYER_H_OEDALPHA

#include "player.h"

using namespace DirectX;
class LocalPlayer : public Player
{
    public:
	LocalPlayer();	// 생성자
	~LocalPlayer();  // 소멸자
	int Input();	// 여기서 Input을 받을 때
	void Redraw(int which, Box &b);	
	void Redraw(Protocol::GameServerPacket&);
	void SoundEffect(char);
	// 여기에 Display정보 Update 할 때에

	void GameOver(int );
	void Attack(int, int);
	void SetScore(int, int );
	void DrawBound(int, int, int);
	void DrawBound(Protocol::GameServerPacket& data);
	//void DrawBound(int, Checker::Party*);
	void Draw();
	void GameStatus(int remained, int attack);
	void Next(int which, Cardpair c);

	struct structKeyDown {
	    int n4;
	    int n5;
	    int n6;
	    int n2;
	} keyDown[2];
	int nDrawBoundLock[2];

    private:
	Sprite sprBackground;		// 배경그림
	Sprite sprCard[52];		// 카드그림	
	Sound* sndArray[9];		// 사운드 배열
	Sprite sprNumber[10];	// 숫자
	Sprite sprBound[4];
	Sprite sprGo[3];
	Sprite sprArrow[2];
	Sprite sprNagari;
	Sprite sprBorder[4];

	int deck[2][10][5]; 	// 상자[너?나][세로][가로]
	int bound[2][10][5];
	static int nFlag;
	int nNagari;
	int nScore[2];
	int nGo[2];
	char szTemp[80];
	int nextcard[2][2];
	int nRemained;
	int nAttack;
	bool bDrawing;
};

#endif
