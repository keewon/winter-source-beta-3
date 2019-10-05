#include "play.h"
#include "game.h"
#include "Checker.h"
#include "random.h"
#include "util.h"
#include <fstream>
#include <iostream>


Play::Play(int n, Game* g, Player* p, Player* o) :
//first_delay(4), 
nID(n), game(g), player(p), opponent(o), score(0), ssak(0),
nagari(01), how_many_go(0)
{
	//nID = n;
	//game = g;
	//player = p;
	//opponent = o;
	//score=0;
	//ssak=0;
	//nagari=1;
	//how_many_go = 0;
	
	//player->SetScore(nID, score);
	//if (opponent) opponent->SetScore(1-nID, score);
}

Play::~Play() {
}


void Play::Redraw() {
		if (opponent)    opponent->Redraw(nID, box);

		if (player)    {
			player->Redraw(nID, box);
		}
}

int Play::Main() {

    if (player) {
	player->Main();
    }
	
    int key = -1;
    if (player) {
	key = player->Input();
    }
    switch(key) {
	case 8:
		if (true == RotateCounterClock() ) 
		{
			player->SoundEffect(5);
			Redraw();
		}
		break;
    case 5:
	    if (true == RotateClock() ) 
		{
			player->SoundEffect(5);
			Redraw();
		}
	    break;
    case 4:
	    if (true == MoveLeft() ) 
		{
			player->SoundEffect(6);
			Redraw();
		}
	    break;
    case 6:
	    if (true == MoveRight() ) 
		{
			player->SoundEffect(6);
			Redraw();
		}
	    break;
    case 2:
		{
			int& t = nID==0?game->nDownFlag0:game->nDownFlag1;

			if (box.deck[9][2].num == 0 || t > 3) {
				MoveDown();
				// MoveDown 에서 자동으로 Redraw()
			}
		}
	    break;
    case -1:
		Redraw();
		game->Lose(nID);

	    break;
    }

    //return key;
	return 0;
}

void 
Play::Init(int nagari_) {

	nagari = nagari_;
	
    if (player)    {
		player->SetScore(nID, score);
		player->SoundEffect(0);
		player->GameStatus(1, 0);
	}
    //if (opponent)    opponent->SoundEffect(0);
	ssak=0;
	how_many_go = 0;
	for (int i=0; i<10; i++) {
		for (int j=0; j<5; j++) {
			box.deck[i][j].set(0);
		}
	}
	cardpair.Pair[0].set(0);
	cardpair.Pair[1].set(0);
	nextpair = game->Decard();
	//Delay(1000);
}

void
Play::StartTurn() {

	how_many_go = 0;
	int n=game->GetAttackStatus();

	if (n != 0) {
		if((n<0 && nID==0) || (n>0 && nID==1)) {
			ReceiveIce(n>0?n:-n); 
			Redraw();
			game->AttackClear();
			player->GameStatus(1, 0);	// 현재 공격상황 알리기
			if (opponent) opponent->GameStatus(1,0);
		}
	}
	
	if (nextpair.Pair[0].num == 0 || nextpair.Pair[1].num == 0) {
		// 직전에 다시 한번 카드 빼오기
		nextpair = game->Decard();
	}
	cardpair = nextpair;
	nextpair = game->Decard();

	// player에게 전하기
	player->Next(nID, nextpair);
	//player->SetScore(nID, score);
	if (opponent) {
		opponent->Next(nID, nextpair);
		//opponent->SetScore(nID, score);
	}
	int remained = game->GetRemainedCard();
	player->GameStatus(0, remained); // 현재 남은 카드 수 알리기
	if (opponent) opponent->GameStatus(0, remained);
	
	// 더 이상 카드가 없다 -_-
	if (cardpair.Pair[0].num == 0 || cardpair.Pair[1].num == 0) {

		player->SoundEffect(8);
		player->GameStatus(2,0);
		if (opponent) {
			opponent->SoundEffect(8);
			opponent->GameStatus(2,0);
		}
		//nagari *= 2;
		game->Lose(-1);		// AttackStatus를 0으로 set한다

	}
	else {
		box.deck[cardpair.row[0]][cardpair.col[0]].set(cardpair.Pair[0].num); 
		box.deck[cardpair.row[1]][cardpair.col[1]].set(cardpair.Pair[1].num);
		Redraw();
	}

}

void
Play::EndTurn() {
    player->SoundEffect(7);
	while(cardpair.row[0]>0&&box.deck[cardpair.row[0]-1][cardpair.col[0]].num==0)
	{
		box.deck[cardpair.row[0]][cardpair.col[0]].set(0);
		cardpair.row[0]--;
		box.deck[cardpair.row[0]][cardpair.col[0]]=cardpair.Pair[0];
	}
	while(cardpair.row[1]>0&&box.deck[cardpair.row[1]-1][cardpair.col[1]].num==0)
	{
		box.deck[cardpair.row[1]][cardpair.col[1]].set(0);
		cardpair.row[1]--;
		box.deck[cardpair.row[1]][cardpair.col[1]]=cardpair.Pair[1];
	}//아직 공중에 떠 있는 카드를 바닥으로 가라앉히는 부분

	bool call_go = true;
	while (call_go == true)
	{
		call_go = false;

		Checker::Party p[50];
		Checker::CheckBox cbox;
		int sum_point=0; int makepoint[50];

		for (int i=0; i<10; i++) {
			for (int j=0; j<5; j++) {
				Checker::CheckGrid(box, cbox, i, j, p[i*5+j]);
			}
		}

		for (int i=0; i<50; i++) {
			//p[i].DrawBound(player, opponent, nID);
			makepoint[i] = p[i].Score();
			sum_point += makepoint[i];
		}

		// 나면..
		if  (
				(how_many_go > 0 && sum_point >= 1) || 
				(how_many_go <= 0 && sum_point >= 3)
			) {
			how_many_go++; call_go = true;
			if (ssak) sum_point*=2;
			sum_point *= nagari;
			if (how_many_go >= 3) sum_point*= 2;
			score += (sum_point*100);
			player->SetScore(nID, score);
			if (opponent) opponent->SetScore(nID, score);

			for (int i=0; i<50; i++) {
				if (makepoint[i] > 0) {
					p[i].Incard(game, box, player, opponent, nID);
				}
			}
			int go_sound = how_many_go>3?3:how_many_go;
			if (1 <= go_sound && go_sound <= 3)
				player->SoundEffect(go_sound);
			player->Attack(nID, how_many_go);
			game->Attack(nID, sum_point);
			//game->Attack(nID, 50);
			int attack_status = game->GetAttackStatus();
			player->GameStatus(1, attack_status);	// 현재 공격상황 알리기
			player->DrawBound(nID, -1, 0);
			player->Redraw(nID, box);

			if (opponent) {
				if (1 <= go_sound && go_sound <= 3)
					opponent->SoundEffect(go_sound);
				opponent->Attack(nID, how_many_go);
				opponent->GameStatus(1, attack_status);
				opponent->DrawBound(nID, -1, 0);
				opponent->Redraw(nID, box);
			}
			Wait(1000);
			box.gravity();		// 중력 작용
		}
		// 못나도 일단 점수 내면
		else if (sum_point > 0) {
			score += (sum_point*10);
			player->SetScore(nID, score);
			if (opponent) opponent->SetScore(nID, score);
		}
	}


	if(!LoseCheck()) {
		StartTurn();
		//StartTurn(); //StartTurn은 게임매니저에서 내리는게 좋을것같습니다.
	}
	else {
		player->SoundEffect(4);
		if (opponent) opponent->SoundEffect(4);
		game->Lose(nID);
	} //게임에 Lose를 선언한다.

	int cardbe=0;
	if(!ssak){
		for(int k=0;k<10;k++){
			for(int j=0;j<5;j++){
				if(box.deck[k][j].num) cardbe=1;
			}
		}
		if(!cardbe) {
			// 원래는 싹쓸이도 화면에 표시해줘야 하는데
			// 귀찮다.. -_-
			ssak=1;
		}
	}
}


void
Play::MoveDown() {
	// 이게 필요했다.
	// 말도안되는 죽음을 피하기 위해서..
	/*
	if (nID == 0) {
		game->nDownFlag0 = 0;
	}
	else if (nID == 1) {
		game->nDownFlag1 = 0;
	}
	*/
	int method; //밑에 쌓인 카드패가 있는지의 여부를 검사하는 방법 지정
	int end; //턴을 끝낼 것인지의 여부
	int m;

	method=cardpair.row[0]==cardpair.row[1]?0:1;
	if(!method)
	{
	/*	if(cardpair.row[0]!=0)
		{*/
		end=box.deck[cardpair.row[0]-1][cardpair.col[0]].num||box.deck[cardpair.row[1]-1][cardpair.col[1]].num||cardpair.row[0]==0;
	//	}
	}
	else
	{
		m=cardpair.row[0]>cardpair.row[1]?cardpair.row[1]:cardpair.row[0];
	/*	if(method!=0)
		{*/
		end=box.deck[m-1][cardpair.col[0]].num||m==0;
		//}
	}
	player->SoundEffect(6);

	if(!end) {
		box.deck[cardpair.row[0]][cardpair.col[0]].set(0); 
		box.deck[cardpair.row[1]][cardpair.col[1]].set(0);
		cardpair.row[0]--;cardpair.row[1]--;
		box.deck[cardpair.row[0]][cardpair.col[0]]=cardpair.Pair[0];
		box.deck[cardpair.row[1]][cardpair.col[1]]=cardpair.Pair[1];
		Redraw();
	}
	else {
		EndTurn();
		Redraw();
	}
	// 이게 필요했다.
	// 말도안되는 죽음을 피하기 위해서..
	if (nID == 0) {
		game->nDownFlag0 = 0;
	}
	else if (nID == 1) {
		game->nDownFlag1 = 0;
	}
}

void Play::ReceiveIce(int icepoint) {
	int remained_ice = icepoint;

	// 우선 기존 패를 얼음으로 바꾸고 패는 환원된다
	for (int i=0; i<icepoint; i++) {
		int box_card_num=0;

		for (int j=0; j<10; j++) {
			for (int k=0; k<5; k++) {
				if (1 <= box.deck[j][k].num && box.deck[j][k].num <= 50) {
					box_card_num++;
				}
			}
		}// box에 있는 card의 개수를 구한다

		if (box_card_num < 0) {break;}
		else {
			int n=rand_int(0, box_card_num-1);

			for (int j=0; j<10; j++) {
				for (int k=0; k<5; k++) {
					if (1 <= box.deck[j][k].num && box.deck[j][k].num <= 50) {
						if (n==0) {
							game->Incard(box.deck[j][k].num);
							// dedeck 은 얼음패를 없애므로 여기서 쓰면 안된다
							box.deck[j][k].set(51);
							remained_ice--;
							k=100;j=100;// exit loop
						}
						else { n--; }
					}
				}
			}

		}

	}

	// 나머지 얼음은 차례대로 쌓인다
	//while (remained_ice > 0) {
		for (int j=0; j<10; j++) {
			for (int k=0; k<5; k++) {
				if (box.deck[j][k].num == 0) {
					box.deck[j][k].set(51);
					remained_ice--;
					if (remained_ice <= 0) {
						j=100; k=100;
					}
				}
			}
		}
	//}

	box.gravity();

	if(!LoseCheck()) {
		//StartTurn();//StartTurn이 두번 선언되는 것이 우려됩니다.
	}
	else {
		game->Lose(nID);
	} //게임에 Lose를 선언한다.
}
int Play::LoseCheck() {
	int i;
	//i=box.deck[8][0].num||box.deck[8][1].num||box.deck[8][2].num||box.deck[8][3].num||box.deck[8][4].num;
	i=box.deck[8][2].num;
	return i;
}

void Play::clear()
{
	int i, j;
	for(i=0;i<=7;i++)
	{
		for(j=0;j<=4;j++)
			box.deck[i][j].set(0);
	}
}

bool Play::MoveRight() {
    int pair_left = cardpair.col[0]<cardpair.col[1]?
					cardpair.col[0]:cardpair.col[1];
    int pair_right = cardpair.col[0]>cardpair.col[1]?
					cardpair.col[0]:cardpair.col[1];
    int pair_top = cardpair.row[0]>cardpair.row[1]?
					cardpair.row[0]:cardpair.row[1];
    int pair_bottom = cardpair.row[0]<cardpair.row[1]?
					cardpair.row[0]:cardpair.row[1];

    if (pair_right >= 4) return false;
    if (box.deck[pair_top][pair_right+1].num != 0 ||
	    box.deck[pair_bottom][pair_right+1].num != 0) {
	    return false;
    }

    box.deck[cardpair.row[0]][cardpair.col[0]].set(0);
    box.deck[cardpair.row[1]][cardpair.col[1]].set(0);

    cardpair.col[0]++; cardpair.col[1]++;

    box.deck[cardpair.row[0]][cardpair.col[0]] = cardpair.Pair[0];
    box.deck[cardpair.row[1]][cardpair.col[1]] = cardpair.Pair[1];
    return true;

}

bool Play::MoveLeft() {
    int pair_left = cardpair.col[0]<cardpair.col[1]?
				    cardpair.col[0]:cardpair.col[1];
    int pair_right = cardpair.col[0]>cardpair.col[1]?
				    cardpair.col[0]:cardpair.col[1];
    int pair_top = cardpair.row[0]>cardpair.row[1]?
				    cardpair.row[0]:cardpair.row[1];
    int pair_bottom = cardpair.row[0]<cardpair.row[1]?
				    cardpair.row[0]:cardpair.row[1];

    if (pair_left <= 0) return false;
    if (box.deck[pair_top][pair_left-1].num != 0 ||
	    box.deck[pair_bottom][pair_left-1].num != 0) {
	    return false;
    }

    box.deck[cardpair.row[0]][cardpair.col[0]].set(0);
    box.deck[cardpair.row[1]][cardpair.col[1]].set(0);

    cardpair.col[0]--; cardpair.col[1]--;

    box.deck[cardpair.row[0]][cardpair.col[0]] = cardpair.Pair[0];
    box.deck[cardpair.row[1]][cardpair.col[1]] = cardpair.Pair[1];
    return true;
}

bool Play::RotateCounterClock() {
    int status = 0;
    int next_col, next_row;
    // Check Status
    //    [0]      
    // [3][x][1]
    //    [2]
    //
    if (cardpair.row[0] < cardpair.row[1]) { 
	status=0; 
	next_col=cardpair.col[0]-1;
	next_row=cardpair.row[0];
    }
    else if (cardpair.row[0] > cardpair.row[1]) { 
	status=2; 
	next_col=cardpair.col[0]+1;
	next_row=cardpair.row[0];
    }
    else {
	if (cardpair.col[0] > cardpair.col[1]) { 
	    status=3; 
	    next_col=cardpair.col[0];
	    next_row=cardpair.row[0]-1;
	}
	else { 
	    status=1; 
	    next_col=cardpair.col[0];
	    next_row=cardpair.row[0]+1;
	}
    }
    return Rotate(-1, status, next_row, next_col);
}

bool Play::RotateClock() {
    int status = 0;
    int next_col, next_row;
    // Check Status
    //    [0]      
    // [3][x][1]
    //    [2]
    //
    if (cardpair.row[0] < cardpair.row[1]) { 
	status=0; 
	next_col=cardpair.col[0]+1;
	next_row=cardpair.row[0];
    }
    else if (cardpair.row[0] > cardpair.row[1]) { 
	status=2; 
	next_col=cardpair.col[0]-1;
	next_row=cardpair.row[0];
    }
    else {
	if (cardpair.col[0] > cardpair.col[1]) { 
	    status=3; 
	    next_col=cardpair.col[0];
	    next_row=cardpair.row[0]+1;
	}
	else { 
	    status=1; 
	    next_col=cardpair.col[0];
	    next_row=cardpair.row[0]-1;
	}
    }
    return Rotate(1, status, next_row, next_col);
}

bool Play::Rotate(int direction, int status, int next_row, int next_col) {
    // if Not Rotatable
    if (0 > next_row || next_row > 9 || 0 > next_col || next_col > 4 ||
		box.deck[next_row][next_col].num != 0) {

	if (status == 0 || status == 2) {
		bool move_result = false;
	    if (status==0 && direction==1 || 
		    status==2 && direction==-1) {
			move_result = MoveLeft();
	    } 
	    else if (status==2 && direction==1 ||
		    status==0 && direction==-1) {
			move_result = MoveRight();
	    } 

	    if (move_result == true) {
		// MoveLeft/Right does side-effect
		// MoveLeft/Right and RotateAgain
		switch(direction) {
		    case 1:
		    return RotateClock();
		    break;
		    default:
		    return RotateCounterClock();
		}
	    }
	    else {
		// UpDown
		int temp_col, temp_row;
		temp_col = cardpair.col[0];
		temp_row = cardpair.row[0];
		cardpair.col[0] = cardpair.col[1];
		cardpair.row[0] = cardpair.row[1];
		cardpair.col[1] = temp_col;
		cardpair.row[1] = temp_row;
		box.deck[cardpair.row[0]][cardpair.col[0]]=cardpair.Pair[0];
		box.deck[cardpair.row[1]][cardpair.col[1]]=cardpair.Pair[1];
		return true;
	    }
	}
	else {
		// status is 1 or 3
	    // Left-Right change :: is this fair?
		int temp_col, temp_row;
		temp_col = cardpair.col[0];
		temp_row = cardpair.row[0];
		cardpair.col[0] = cardpair.col[1];
		cardpair.row[0] = cardpair.row[1];
		cardpair.col[1] = temp_col;
		cardpair.row[1] = temp_row;
		box.deck[cardpair.row[0]][cardpair.col[0]]=cardpair.Pair[0];
		box.deck[cardpair.row[1]][cardpair.col[1]]=cardpair.Pair[1];
		return true;
	}
    } else {
	// Apply Rotate
	box.deck[cardpair.row[1]][cardpair.col[1]].set(0);
	cardpair.row[1] = next_row; cardpair.col[1] = next_col;
	box.deck[cardpair.row[1]][cardpair.col[1]]=cardpair.Pair[1];
	return true;
    }
}
