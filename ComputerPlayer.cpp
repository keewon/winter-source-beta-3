/////////////////////////////////////////////////
//
// ComputerPlayer.cpp
//
//
///////////////////////////////////////////////////
#include "ComputerPlayer.h"
#include "Util.h"
#include "Checker.h"
//#include <fstream>

// 얘는 생성자
// 적당히 초기화 할 것들은 초기화 할것
ComputerPlayer::ComputerPlayer(int n) 
: comQ(1000), nID(n), processed(false)
{
	clock = timeGetTime();
	comQ.Push("00000", 5);
}

// 얘는 소멸자
// 적당히 new 했던 것들을 delete 할 것
ComputerPlayer::~ComputerPlayer()
{
}

// 현재 Box 상황을 어떤 키 조합으로 헤쳐나가면 되는지 생각하고
// 일단은 queue에 넣어두고 이 함수가 불릴 때마다 하나씩 리턴한다
// queue는 class MsgQueue 를 사용해도 괜찮고
// 스스로 만들거나 배열을 사용해도 괜찮음.

int ComputerPlayer::Input()
{
	char t;
	//Wait(300);
	if (comQ.Size() > 0) {
		comQ.Pop(&t, 1); return t-'0';
	}
		// 리턴값은 8, 4, 5, 6, 2, -1, 0 중의 하나
	else {
		//processed = false;
		return 0;
	}
}

// 인자로 넘어오는 Box를 잘 처리해서 다음 자기 행동을 결정해야 함
// 다음 자기 행동을 queue에 넣어두면 됨
void 
ComputerPlayer::Redraw(int which, Box &b)
{
	// To Do :
	// which 변수 처리 -> 어느 것이 com의 Box이고 어느 것이 나의 Box인지 모른다.
	// ~~~~~~~~~~~~~~~

	// 박스 변경
	if ( which == nID )
	{
		box = b;

	}


}

bool
ComputerPlayer::Danger()
{
		if (
			box.deck[6][2].num != 0 &&
			box.deck[5][2].num != 0 &&
			box.deck[4][2].num != 0 &&
			box.deck[3][2].num != 0 &&
			box.deck[2][2].num != 0 &&
			box.deck[1][2].num != 0 &&
			box.deck[0][2].num != 0) {
			return true;
		}
		else return false;
}

// 이것도 신경쓰지 않아도 됨
int 
ComputerPlayer::Main()
{

	int new_tick = timeGetTime();
	if (new_tick - clock > 400) {
		clock = new_tick;
		if (!Danger() || 
			(box.deck[9][2].num == 0 && box.deck[8][2].num == 0)) {
			comQ.Push("222", 1);
		}
		processed = false;
	}

	if (processed == false && 
		box.deck[9][2].num != 0 && box.deck[8][2].num != 0) {
		// 패가 처음 등장했을 때에
		// 20가지 방법 (5*4)으로 패를 내려본다
		int best=0; int best_score=-10000;
		for (int i=0; i<20; i++) {
			Cardpair c(box.deck[8][2].num, box.deck[9][2].num);
			c.row[0]=8;
			c.row[1]=(i%4==0)?9:(i%4==2?7:8);
			c.col[0]=i/4;
			c.col[1]=(i%2==0)?(i/4):( (i%4==1)?(i/4+1):(i/4-1) );

			if (i==3 || i==17) continue;
			if (Danger() && 
				( (8<=i && i<=11) || i==5 || i==15 )
				) continue;

			// else
			{
				Box b = box;
				//Box b;
				//for (int j=0; j<10; j++) {
				//	for (int k=0; k<5; k++) {
				//		b.deck[j][k].set( box.deck[j][k].num );
				//	}
				//}
				b.deck[ c.row[0] ][ c.col[0] ].set( c.Pair[0].num );
				b.deck[ c.row[1] ][ c.col[1] ].set( c.Pair[1].num );

				b.gravity();

				//if (b.deck[8][2].num == 0) 
				{

					Checker::Party p[50];
					Checker::CheckBox cbox;
					int sum_point=0;

					for (int k=0; k<10; k++) {
						for (int j=0; j<5; j++) {
							Checker::CheckGrid(b, cbox, k, j, p[k*5+j]);
						}
					}

					for (int k=0; k<50; k++) {
						sum_point += p[k].Heuristic();
					}

					if (i/4 == 2) {
						if (Danger()) sum_point -=2000;
						else sum_point -=500;
					}
					else if (i/4 == 1 || i/4 == 3) {
						if (Danger()) sum_point -= 1800;
						else sum_point -= 200;
					}

					//std::ofstream ofs("computer.log", std::ios::app);
					//ofs << sum_point << std::endl;
					//ofs.close();

					if (sum_point > best_score) {
						best = i; best_score = sum_point;
					}
				} // end of if
			} // end of if
		} // end of for

		//std::ofstream ofs("computer.log", std::ios::app);
		//ofs<< "best : " << best << " score : " << best_score << std::endl;
		//ofs.close();

		if (best/4 == 0) comQ.Push("44", 2);
		else if (best/4 == 1) comQ.Push("4", 1);
		else if (best/4 == 2) comQ.Push("426", 3);
		else if (best/4 == 3) comQ.Push("6", 1);
		else if (best/4 == 4) comQ.Push("66", 2);

		if (best%4 == 1) comQ.Push("5", 1);
		else if (best%4 == 2) {
			if (best/4 == 4)
				comQ.Push("88", 2);
			else 
				comQ.Push("55", 2);
		}
		else if (best%4 == 3) comQ.Push("8", 1);
	
		//comQ.Push("2222", 4);
		processed = true;
	}
	

	return 0;




	// 가장 낮은 쪽으로 패를 옮기기 전략
/*
		if (box.deck[8][2].num != 0) {
			int height[5];

			for (int i=0; i<5; i++) {
				height[i] = 0;
				for (int j=1; j<= 9 ; j++) {
					if (box.deck[j][i].num == 0) {
						height[i] = j-1;
						j=100;
					}
				}
			}

			int best_x = 0;
			if (height[2] < height[best_x]) best_x = 2;
			if (height[1] <= height[best_x]) best_x = 1;
			if (height[3] <= height[best_x]) best_x = 3;
			if (height[4] <= height[best_x]) best_x = 4;
			if (height[0] <= height[best_x]) best_x = 0;

			switch (best_x) {
			case 2:
				break;
			case 1:
				comQ.Push("4",1);
				break;
			case 0:
				comQ.Push("44",2);
				break;
			case 3:
				comQ.Push("6",1);
				break;
			case 4:
				comQ.Push("66",2);
				break;
			}

			for (int j=0; j< (8-height[best_x])/2; j++) {
				comQ.Push("2", 1);
			}
		}
		else {
			comQ.Push("5", 1);
		}

		Wait(500);

    return 0;
*/
}

// 별로 신경쓰지 않아도 됨
void ComputerPlayer::Attack(char ch)
{
}
