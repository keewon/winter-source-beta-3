/////////////////////////////////////////////////////////////////////////////
//  LocalPlayer
//                               하제 겨울프로젝트 가/서 
//                               서기원
/////////////////////////////////////////////////////////////////////////////
#include "LocalPlayer.h"
#include "stdio.h"
#include <fstream>
//#include "Checker.h"

//using namespace DirectX;
int LocalPlayer::nFlag = 0;
LocalPlayer::
LocalPlayer()
{
	bDrawing = false;
	nNagari = 0;
	nAttack = 0;
	nRemained = 0;
	nextcard[0][0]=0; nextcard[0][1]=0; nextcard[1][0]=0; nextcard[1][1]=0;
    nFlag = 0;
	nScore[0] = 0;
	nScore[1] = 0;
	nGo[0] = nGo[1] = 0;
    ///////////////////////////////////////////////////////
    // 사운드 라이브러리 사용법
    // BackGroundMusic->Play(DSBPLAY_LOOPING);
    // BackGroundMusic->Play();
    // BackGroundMusic->Stop();
    ////////////////////////////////////////////////////////
    
    // 사운드 로딩하기
    sndArray[0] = new Sound("Resource/Sound/bgm.wav");
    sndArray[1] = new Sound("Resource/Sound/1go.wav");
    sndArray[2] = new Sound("Resource/Sound/2go.wav");
    sndArray[3] = new Sound("Resource/Sound/3go.wav");
    sndArray[4] = new Sound("Resource/Sound/gameover.wav");
    sndArray[5] = new Sound("Resource/Sound/rotate.wav");
    sndArray[6] = new Sound("Resource/Sound/cursor.wav");
    sndArray[7] = new Sound("Resource/Sound/down.wav");
	sndArray[8] = new Sound("Resource/Sound/nagari.wav");
    
    // 그림 로딩하기
    // sprBackground 는 LocalPlayer의 member로서 Sprite이다.
    // 괄호는 operator overloading 되어있다.
    // Sprite에 이름을 써줄 때에 잘못된 파일 이름을 써주면 
    // 프로그램이 바로 죽게 되니 조심!
    sprBackground("Resource/background.bmp");

    char szFilename[40];
    for (int i=0; i<52; i++) {
	sprintf(szFilename, "Resource/card/%02d.bmp", i);
	sprCard[i](szFilename);
    }

    // 배경 그림 위치
    // Sprite는 거의 모든 것을 ()로 할 수 있다.
    sprBackground(0,0);

	// 숫자 그림 로딩
	for (int i=0; i<=9; i++) {
		char szName[40];
		sprintf(szName, "Resource/Number/%d.bmp", i);
		sprNumber[i](szName);
	}

	// 테두리 그림 로딩
	sprBound[0]("Resource/card/51.bmp");
	sprBound[1]("Resource/bomb2.bmp");
	sprBound[2]("Resource/bomb3.bmp");
	sprBound[3]("Resource/bomb4.bmp");

	// 그룹핑
	sprBorder[0]("Resource/border-c.bmp");
	sprBorder[1]("Resource/border-g.bmp");
	sprBorder[2]("Resource/border-m.bmp");
	sprBorder[3]("Resource/border-y.bmp");

	// 원고 투고 쓰리고
	sprGo[0]("Resource/one.bmp");
	sprGo[1]("Resource/two.bmp");
	sprGo[2]("Resource/three.bmp");

	// 화살표
	sprArrow[0]("Resource/left.bmp");
	sprArrow[1]("Resource/right.bmp");

	// 나가리 그림
	sprNagari("Resource/nagari.bmp");
	sprNagari(200,200);

    // Key 초기화 :
	keyDown[0].n2 = 0;
	keyDown[0].n4 = 0;
	keyDown[0].n5 = 0;
	keyDown[0].n6 = 0;
	keyDown[1].n2 = 0;
	keyDown[1].n4 = 0;
	keyDown[1].n5 = 0;
	keyDown[1].n6 = 0;


	// 테두리 그리는 것은 모든 테두리 정보가 왔을 때에..
	nDrawBoundLock[0] = 0;
	nDrawBoundLock[1] = 0;
    //bKeyDown5 = bKeyDown2 = bKeyDown4 = bKeyDown6 = false;

    // deck 초기화 : deck은 플레이어 각각의 상자정보를 담고 있는 Array이다.
    for (int i=0; i<2; i++) {
	for (int j=0; j<10; j++) {
	    for (int k=0; k<5; k++) {
		deck[i][j][k] = 0;
		bound[i][j][k] = 0;
	    }
	}
    }
}

LocalPlayer::
~LocalPlayer() {
    // Sound는 new 로 만들었으니 항상 delete로 지워줘야 한다
    for (int i=0 ;i<=8; i++)
	delete sndArray[i];
    // 하지만 Sprite는 new로 만든게 아니니 
    // LocalPlayer객체가 사라질 때에 알아서 지워진다
    // 이때에 조심할 것은 모든 Sprite 객체가 사라져야 
    // DirectX 모드를 해제할 수 있다. DirectX모드 해제 시기와
    // LocalPlayer 지우는 시기를 잘 맞춰야 한다
}

int
LocalPlayer::
Input(void)
{
    // 전에도 말했지만 Key 입력받는 패러다임을 바꿔야 한다
	if (GetAsyncKeyState(VK_ESCAPE)) {
		return -1;
	}
	if (GetAsyncKeyState(VK_NUMPAD5) || GetAsyncKeyState(VK_UP)
		|| GetAsyncKeyState(VK_DIVIDE) 
		|| GetAsyncKeyState(0xBF) ){
		// 0xBF 는 /
	    if (keyDown[0].n5 >= 0) {
		keyDown[0].n5 = -1;
		return 5;
	    }
	}
	else if (GetAsyncKeyState(VK_DECIMAL) 
		|| GetAsyncKeyState(0xBE) ) {
		// 0xBE 는 .
		if (keyDown[0].n5 >= 0) {
			keyDown[0].n5 = -1;
			return 8; // 반시계
		}
	}
	else keyDown[0].n5 = 0;

	if (GetAsyncKeyState(VK_NUMPAD4) || GetAsyncKeyState(VK_LEFT) ) {
	    // 약간 천천히 그러나 빠르게 움직임
	    // 여러번 누르면 더 빠르게 움직임
	    if (keyDown[0].n4 >= 0) {
		keyDown[0].n4 = -2;
		return 4;
	    }
	}
	else keyDown[0].n4 = 0;
	if (GetAsyncKeyState(VK_NUMPAD6) || GetAsyncKeyState(VK_RIGHT) ) {
	    // 약간 천천히 그러나 빠르게 움직임
	    // 여러번 누르면 더 빠르게 움직임
	    if (keyDown[0].n6 >= 0) {
		keyDown[0].n6 = -2;
		return 6;
	    }
	}
	else keyDown[0].n6 = 0;
	if (GetAsyncKeyState(VK_NUMPAD2) || GetAsyncKeyState(VK_DOWN) ) {
	    // 빠르게 내려감
	    if (keyDown[0].n2 >= 0) {
		keyDown[0].n2 = -1;
	 	return 2;
	    }
	}
	else keyDown[0].n2 = 0;
	return 0;
}

void 
LocalPlayer::
Redraw(int which, Box &b)
{
    // Redraw는 Play에서 호출하는 함수이다.
    // 어느 편의 정보를 업데이트한후 Draw를 불러 자동으로 다시 그리게 한다
    // 이때 다시그리는 것은 전체화면을 다시 그리는 것이다
    // Play에서 이 걸 부른다
    if (which > 1 || which < 0) return;

    for (int i=0; i<10; i++) {
	for (int j=0; j<5; j++) {
	    deck[which][i][j] = b.deck[i][j].num;
	}
    }
    //Draw();
}

void
LocalPlayer::
Redraw(Protocol::GameServerPacket& data)
{
	//std::ofstream ofs("player.log", std::ios::app);
	//ofs << (int)(data.player) << std::endl;
    // 이 함수는 Terminal에서 아마도 부를 것이다.
    if (data.player >1 || data.player < 0) return;

    for (int i=0; i<10; i++) {
	for (int j=0; j<5; j++) {
		if (0 <= data.deck[i][j] && data.deck[i][j] <= 51 )
			deck[data.player][i][j] = (int)(data.deck[i][j]);
		else
			deck[data.player][i][j] = 51;
		//ofs << (int)(data.deck[i][j]) << " ";
	}
	//ofs << std::endl;
    }
	//ofs << std::endl;
    //Draw();
}

void LocalPlayer::Draw() {

	if (bDrawing == true) return;
	bDrawing = true;
    // 배경 그리기
    sprBackground(0,0);

    // 투명색은 기본적으로 RGB값이 (0,0,0)이다.
    // 이 색을 투명으로 안하고 그냥 검정으로 칠하려면
    // DDBLTFAST_NOCOLORKEY 옵션을 준다
    sprBackground(DDBLTFAST_NOCOLORKEY, 256, 0);

    for (int i=0; i<2; i++) {
	for (int j=0; j<10; j++) {
	    for (int k=0; k<5; k++) {
		int num = deck[i][j][k];

		// 잘못된 정보 검사
		// 배열의 인자로 쓰이는 애들은 항상 범위가 제대로
		// 되었는지 검사하는 편이 낫다.
		// Access Violation 이 나는 주된 원인!!!
		// (Unix 쪽에서는 Segmentation Fault가 나는 원인이기도 함)
		if (num < 0 && num > 51) {
		    num = 51;
		}
		// 위치지정
		sprCard[num](50*(i*9+k+1), 50*(10-j));

		// 찍기
		if (num == 0) { 
		    // 널카드
		    // 널카드는 투명으로 찍는다
		    // 투명으로 찍을 때에는 인자를 주지 않아도 되지만
		    // 혹시나 해서 준것이다
		    sprCard[num](DDBLTFAST_SRCCOLORKEY,256,0);
		}
		else { // 카드 : 역시 불투명으로 찍는다
		    sprCard[num](DDBLTFAST_NOCOLORKEY,256,0);
		}


	    }
	}
    }

    for (int i=0; i<2; i++) {
	//if (nDrawBoundLock[i] > 0) {
	    for (int j=0; j<10; j++) {
		for (int k=0; k<5; k++) {
			switch (bound[i][j][k]) {

			//case 1:
			//case 2:
			//case 3:
			//case 4:
			//	sprBorder[ bound[i][j][k] -1 ](50*(i*9+k+1), 50*(10-j));
			//	sprBorder[ bound[i][j][k] -1 ](DDBLTFAST_SRCCOLORKEY, 256, 0);
			//	break;
			//
			case -1:
			case 255:
				switch (nDrawBoundLock[i]) {
					case 10:
						sprBound[0](50*(i*9+k+1), 50*(10-j));
						sprBound[0](DDBLTFAST_SRCCOLORKEY, 256, 0);
						break;
					case 6:
						sprBound[1](50*(i*9+k+1), 50*(10-j));
						sprBound[1](DDBLTFAST_SRCCOLORKEY, 256, 0);
						break;
					case 4:
						sprBound[2](50*(i*9+k+1), 50*(10-j));
						sprBound[2](DDBLTFAST_SRCCOLORKEY, 256, 0);
						break;
					case 2:
						sprBound[3](50*(i*9+k+1), 50*(10-j));
						sprBound[3](DDBLTFAST_SRCCOLORKEY, 256, 0);
						break;
					case 1:
					   bound[i][j][k] = 0;
					   break;
				}
				break;


		    }
		}
	    }
		if (nDrawBoundLock[i] > 0) nDrawBoundLock[i]--;
	//}
    }


	// Print Score
	
	for (int k=0; k<=1; k++) 
	{
		int score = nScore[k];
		if (score < 0) score = 0;

		for (int i=0; i<5; i++) {
			int num = score % 10;
			sprNumber[num](280 + 300*k - 20*i, 10);
			sprNumber[num]();
			score = score /10;
		}
	}

	// Go 표시
	for (int i=0; i<2; i++) {
	    if (0 < nDrawBoundLock[i] && nDrawBoundLock[i] < 6) {
			int k=0;
			switch(nGo[i]) {
			case 0:
				k = -1;
				break;
			case 1:
			case -1:
				k=0;
				break;
			case 2:
			case -2:
				k=1;
				break;
			default:
				k=2;
			}
			if (0 <= k && k <= 2) {
				sprGo[k](50*(i*9+1), 50*(10-7));
				sprGo[k](DDBLTFAST_SRCCOLORKEY,256,0);
			}
	    }
	}
	
	// Remained Card와 AttackStatus 표시
	if (nRemained >= 10) {
		sprNumber[nRemained/10](380, 200);
		sprNumber[nRemained/10]();
	}
	sprNumber[nRemained%10](400,200);
	sprNumber[nRemained%10]();

	if (nAttack > 0) {
		sprArrow[1](420, 10); sprArrow[1]();
		sprNumber[(nAttack/10)%10](380,10); sprNumber[(nAttack/10)%10]();
		sprNumber[nAttack%10](400,10); sprNumber[nAttack%10]();
	}
	else if (nAttack == 0) {
		sprNumber[0](390,10); sprNumber[0]();
	}
	else if (nAttack < 0) {
		sprArrow[0](330, 10); sprArrow[0]();
		sprNumber[((-nAttack)/10)%10](380,10); sprNumber[((-nAttack)/10)%10]();
		sprNumber[(-nAttack)%10](400,10); sprNumber[(-nAttack)%10]();
	}



	
	// Next 표시
	sprCard[nextcard[0][1]](400-75,100);
	sprCard[nextcard[0][0]](400-75,150);
	sprCard[nextcard[1][1]](400+25,100);
	sprCard[nextcard[1][0]](400+25,150);

	sprCard[nextcard[0][0]](DDBLTFAST_NOCOLORKEY,256,0);
	sprCard[nextcard[0][1]](DDBLTFAST_NOCOLORKEY,256,0);
	sprCard[nextcard[1][0]](DDBLTFAST_NOCOLORKEY,256,0);
	sprCard[nextcard[1][1]](DDBLTFAST_NOCOLORKEY,256,0);


	// 나가리 표시
	if (nNagari > 0) {
		sprNagari();
		nNagari--;
	}
	

    DirectX::DirectDraw::flip();
    // 앞에 붙어있는 DirectX는 namespace이다.
    // flip을 해야 그림이 업데이트 된다

	bDrawing = false;
}

void 
LocalPlayer::SoundEffect(char data) {
    switch(data) {
	// 역시나 배열의 인자는 조심해서 집어넣는다
	case 0:
	    // 배경음악은 Loop
	    sndArray[data]->Play(DSBPLAY_LOOPING);
	    break;
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	    // 다른 효과음은 한번만
	    sndArray[data]->Play();
	    break;
    }
}

void LocalPlayer::SetScore(int which, int n) {
	if (which == 0 || which == 1) {
	    nScore[which] = n;
	}
}

void LocalPlayer::GameOver(int winner) {
    // 게임오버 화면이 오래 지속되도록 바꿔야 함
	if (winner == 1)    sprBackground("Resource/over2.bmp");
	else if (winner == 0) sprBackground("Resource/over1.bmp");
	else sprBackground("Resource/over.bmp");
    //Draw();
}

void LocalPlayer::Attack(int which, int n) {
	if (which == 0 || which == 1) {
		if (n==1 || n==2 || n==3)		nGo[which] = n;
		else nGo[which] = 3;
	}
	//nScore[c>0?1:0] = nGo[c>0?1:0];
    // Attack했다 또는 당했다는 표시를 모니터와 음향으로 해주어야 함
}
/*
void LocalPlayer::DrawBound(int which, Checker::Party* p) {

	for (int i=0; i<p->number; i++) {
		bound[which][ p->x[i] ][ p->y[i] ] = (int)(p->type);
	}
}
*/
void LocalPlayer::DrawBound(int which, int i, int j) {

    // DrawBound의 두번째 인자가 -1이면
    // 그림을 그린다..
    // 따로 함수를 안만들고 이런 변태적인 방법을 쓴 것은......
    // 으음....

    if (i < 0) { 
	nDrawBoundLock[which] = 10;
    }
    else {
	bound[which][i][j] = -1;
	//sprBound(50*(which*9+i+1), 50*(10-j));
	//sprBound(DDBLTFAST_SRCCOLORKEY, 256, 0);
	//Draw();
    }

}

void
LocalPlayer::
DrawBound(Protocol::GameServerPacket& data)
{
    // 이 함수는 Terminal에서 아마도 부를 것이다.
    if (data.player >1 || data.player < 0) return;

    for (int i=0; i<10; i++) {
	for (int j=0; j<5; j++) {
	    bound[data.player][i][j] = data.deck[i][j];
		//주의! 서버에서 -1 을 보내면 255 가 되서 돌아온다!
	}
    }

	nDrawBoundLock[data.player] = 10;
    //Draw();
}

void LocalPlayer::GameStatus(int code, int num) {
	switch(code) {
	case 0:
		nRemained = num;
		break;
	case 1:
		nAttack = num;
		break;
	case 2:
		nNagari = 10;
		break;
	}
}

void LocalPlayer::Next(int which, Cardpair c) {
	if (which==0 || which==1) {
		nextcard[which][0] = c.Pair[0].num;
		nextcard[which][1] = c.Pair[1].num;
	}
}