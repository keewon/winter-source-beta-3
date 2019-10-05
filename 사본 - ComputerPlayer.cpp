/////////////////////////////////////////////////
//
// ComputerPlayer.cpp
//
//
///////////////////////////////////////////////////
#include "ComputerPlayer.h"


// 얘는 생성자
// 적당히 초기화 할 것들은 초기화 할것
ComputerPlayer::ComputerPlayer(int n) 
: comQ(100), nID(n), out("debugman.txt", ios::out)
{

	
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
	if (comQ.Size() > 0) {
		comQ.Pop(&t, 1); return t-'0';
	}
		// 리턴값은 4, 5, 6, 2, -1, 0 중의 하나
	else return 0;
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
	if ( which != nID )
	{
		humanBox=b;
	}
	else
	{
		comBox=b;
	
		// Seperator 호출
		Seperator(which);

		// 추론엔진 && 결정기
		Infer_decide();

		// Linker
		//Linker();


	}


}

// Computer도 효과음을 내야하나?
// 잘 모르겠다.. 아직 신경쓰지 말것
void 
ComputerPlayer::SoundEffect(char ch)
{
}

// 일단은 신경쓰지 않아도 됨
void 
ComputerPlayer::GameOver(bool winner)
{
}

// 이것도 신경쓰지 않아도 됨
int 
ComputerPlayer::Main()
{
    return 0;
}

// 별로 신경쓰지 않아도 됨
void ComputerPlayer::Attack(char ch)
{
}

// 이것도 별로 신경쓰지 않아도 됨
void ComputerPlayer::RemainedCard(int n)
{
}

void ComputerPlayer::SetScore(int n, int) {
}

void ComputerPlayer::DrawBound(int, int, int) {
}

// Seperator :
// 입력된 Box를 떨어지는 것(drop)과 이미 쌓여 있는 것(box)로
// 구분한다.
void ComputerPlayer::Seperator(int which)
{
	int i, j, exitflag=1, l, k;

	if ( which != nID )
	{
		for ( i=0; i<10; i++ )
			for ( j=0; j<5; j++ )
				humanDrop.deck[i][j].set(0);
	
		for ( i=9; i>=0 && exitflag; i--  )
		{
			for ( j=0; j<5 && exitflag; j++ )
			{
				if ( humanBox.deck[i][j].num )
				{
					exitflag=0;
					break;
				}
			}
		}
		// exitflag에 의해 탈출하므로 exitflag는 원래보다 1 더 작아진다.

		humanpos[0][0]=0; humanpos[0][1]=0; humanpos[1][1]=0; humanpos[1][0]=0;
		humanIng[0].set(0);
		humanIng[1].set(0);
		
	
		exitflag=1;
		Rotated_state=0;
		for ( k=i; k>=0 && exitflag; k-- )
		{
			if ( !(humanBox.deck[k][j].num) )
			{
				for ( l=i; l>k; l++ )
				{
					humanDrop.deck[l][j].set(humanBox.deck[l][j].num);
					humanIng[i-l].set(humanBox.deck[l][j].num);
					humanBox.deck[l][j].set(0);
					humanpos[i-l][0]=l; humanpos[i-l][1]=j;
					Rotated_state=i-l;
				}
				if ( k==i )
				{
					if ( !(humanDrop.deck[l][j+1].num) && humanBox.deck[l+1][j+1].num && j<4)
					{
						humanDrop.deck[l+1][j+1].set(humanBox.deck[l+1][j+1].num);
						humanIng[1].set(humanBox.deck[l+1][j+1].num);
						humanBox.deck[l+1][j+1].set(0);
						humanpos[1][0]=l+1; humanpos[1][1]=j+1;
					}
					Rotated_state=2;
				}
			}
			break;

		}

	} // if humanbox
	else
		{
		for ( i=0; i<10; i++ )
			for ( j=0; j<5; j++ )
				comDrop.deck[i][j].set(0);
	
		for ( i=9; i>=0 && exitflag; i--  )
		{
			for ( j=0; j<5 && exitflag; j++ )
			{
				if ( comBox.deck[i][j].num )
				{
					exitflag=0;
					break;
				}
			}
		}
		// exitflag에 의해 탈출하므로 exitflag는 원래보다 1 더 작아진다.

		compos[0][0]=0; compos[0][1]=0; compos[1][1]=0; compos[1][0]=0;
		comIng[0].set(0);
		comIng[1].set(0);

		exitflag=1;
		Rotated_state=0;
		i++;

		for ( k=i; k>=0 && exitflag; k-- )
		{
			if ( !(comBox.deck[k][j].num) )
			{
				for ( l=i; l>k; l-- )
				{
					char buffer[100]="";
					buffer[0]=l+'0';
					buffer[1]=' ';
					buffer[2]=k+'0';
					//if (IDCANCEL == MessageBox(hhh, buffer, "내용", MB_OKCANCEL)) {
					//	exit(1);
					//}

					comDrop.deck[l][j].set(comBox.deck[l][j].num);
					comIng[i-l].set(comDrop.deck[l][j].num);
					comBox.deck[l][j].set(0);
					compos[i-l][0]=l;
					compos[i-l][1]=j;
					//out<<"**"<<i<<" "<<l<<" "<<comDrop.deck[l][j].num<<" "<<comIng[i-l].num<<" "<<endl;
					
				}
				if ( k==i )
				{
					if ( !(comDrop.deck[l][j+1].num) && comBox.deck[l+1][j+1].num && j<4)
					{
						comDrop.deck[l+1][j+1].set(comBox.deck[l+1][j+1].num);
						comIng[1].set(comBox.deck[l+1][j+1].num);
						comBox.deck[l+1][j+1].set(0);
						compos[1][0]=l+1; compos[1][1]=j+1;
					}
					
				}
				break;
			}

		}

		
/*		for ( i=9; i>=0; i-- )
		{
			for ( j=0; j<5; j++ )
				out<<comBox.deck[i][j].num<<" ";
			out<<endl;
		}
		out<<endl;

		for ( i=9; i>=0; i-- )
		{
			for ( j=0; j<5; j++ )
				out<<comDrop.deck[i][j].num<<" ";
			out<<endl;
		}

		out<<comIng[0].num<<" "<<comIng[1].num<<endl;
		out<<compos[0][0]<<","<<compos[0][1]<<"   ";
		out<<compos[1][0]<<","<<compos[1][1]<<endl;
		out<<"------------------------------------"<<endl;*/

	} // else
	
			



} // end seperator


int board[10][5];
int move_x[4]={-1,0,1,0};
int move_y[4]={0, 1,0,-1};
int group[]={0,1,1,2,2,2,3,4,5,6,6,7};

void ComputerPlayer::Infer_decide()
{
	int currentstate=Rotated_state=1;
	int maximum=-10000, temp, maxRotated_state=Rotated_state, thatI=0;

	do
	{
		int i;
		out<<Rotated_state<<" ";
		switch ( Rotated_state )
		{
		case 0:
			for ( i=compos[0][1]-1; i>-1 && !(comBox.deck[compos[0][0]][i].num); i-- )
				; // Do nothing in for
			for ( i++; i<5 && !(comBox.deck[compos[0][0]][i].num); i++ )
			{
				tempBox=comBox;
				tempBox.deck[compos[0][0]][i]=comIng[0];
				for ( int j=compos[0][0]-1; j>-1 && !(tempBox.deck[j][i].num); j-- )
					tempBox.fall(j+1, i);
				temp=Decide();
				if ( temp>maximum )
				{
					maximum=temp;
					maxRotated_state=Rotated_state;
					thatI=i;
				}
			}
			break;
		case 1:
			for ( i=compos[1][1]-1; i>-1 && !(comBox.deck[compos[1][0]][i].num); i-- )
				; // Do nothing in for
			for ( i++; i<5 && !(comBox.deck[compos[1][0]][i].num); i++ )
			{
				tempBox=comBox;
				tempBox.deck[compos[1][0]][i]=comIng[1];
				tempBox.deck[compos[0][0]][i]=comIng[0];
				for ( int j=compos[1][0]-1; j>-1 && !(tempBox.deck[j][i].num); j-- )
					tempBox.fall(j+1, i);
				temp=Decide();
				if ( temp>maximum )
				{
					maximum=temp;
					maxRotated_state=Rotated_state;
					thatI=i;
				}
			}
			break;
		case 2:
			for ( i=compos[0][1]-1; i>-1 && !(comBox.deck[compos[0][0]][i].num); i-- )
				; // Do nothing in for
			for ( i++; i<4 && !(comBox.deck[compos[0][0]][i+1].num); i++ )
			{
				tempBox=comBox;
				tempBox.deck[compos[0][0]][i+1]=comIng[1];
				tempBox.deck[compos[0][0]][i]=comIng[0];
				for ( int j=compos[1][0]-1; j>-1 && !(tempBox.deck[j][i].num); j-- )
					tempBox.fall(j+1, i);
				for ( int j=compos[1][0]-1; j>-1 && !(tempBox.deck[j][i+1].num); j-- )
					tempBox.fall(j+1, i+1);
				temp=Decide();
				if ( temp>maximum )
				{
					maximum=temp;
					maxRotated_state=Rotated_state;
					thatI=i;
				}
			}
			break;
		case 3:
			for ( i=compos[1][1]-1; i>-1 && !(comBox.deck[compos[1][0]][i].num); i-- )
				; // Do nothing in for
			for ( i++; i<5 && !(comBox.deck[compos[1][0]][i].num); i++ )
			{
				tempBox=comBox;
				tempBox.deck[compos[0][0]-1][i]=comIng[0];
				tempBox.deck[compos[0][0]][i]=comIng[1];
				for ( int j=compos[1][0]-1; j>-1 && !(tempBox.deck[j][i].num); j-- )
					tempBox.fall(j+1, i);
				temp=Decide();
				if ( temp>maximum )
				{
					maximum=temp;
					maxRotated_state=Rotated_state;
					thatI=i;
				}
			}
			break;
		case 4:
			for ( i=compos[0][1]-1; i>-1 && !(comBox.deck[compos[0][0]][i].num); i-- )
				; // Do nothing in for
			for ( i++; i<4 && !(comBox.deck[compos[0][0]][i+1].num); i++ )
			{
				tempBox=comBox;
				tempBox.deck[compos[0][0]][i+1]=comIng[0];
				tempBox.deck[compos[0][0]][i]=comIng[1];
				for ( int j=compos[1][0]-1; j>-1 && !(tempBox.deck[j][i].num); j-- )
					tempBox.fall(j+1, i);
				for ( int j=compos[1][0]-1; j>-1 && !(tempBox.deck[j][i+1].num); j-- )
					tempBox.fall(j+1, i+1);
				temp=Decide();
				if ( temp>maximum )
				{
					maximum=temp;
					maxRotated_state=Rotated_state;
					thatI=i;
				}
			}
			break;
		}
		
		if ( Rotated_state>0 )
		{
			++Rotated_state;
			if ( Rotated_state>4 )
				Rotated_state=1;
		}
	} while ( Rotated_state!=currentstate && currentstate );
	
	char Qbuffer[4];

	while ( Rotated_state != maxRotated_state )
	{
		Qbuffer[0]='5';
		comQ.Push(Qbuffer, 1);
		Rotated_state++;
		if ( Rotated_state>4 )
			Rotated_state=1;
	}

	if ( compos[0][1]>thatI )
	{
		for ( int j=compos[0][1]-1; j>=thatI; j-- )
		{
			Qbuffer[0]='4';
			comQ.Push(Qbuffer, 1);
		}
	}
	else
	{
		for ( int j=compos[0][1]+1; j<thatI; j++ )
		{
			Qbuffer[0]='6';
			comQ.Push(Qbuffer, 1);
		}
	}
	Qbuffer[0]='2';
	comQ.Push(Qbuffer, 1);
			
				
	
}

int ComputerPlayer::Decide()
{
	int i, j, sum=0, temp;
	if ( compos[0][0]>0 && compos[0][0]<7 )
	{

		for ( i=0; i<10; i++ )
			for ( j=0; j<5; j++ )
				board[i][j]=0;

		for ( j=0; j<5; j++ )
			for ( i=9; i>=0; i-- )
				if ( tempBox.deck[i][j].num && !board[i][j] )
				{
					temp=getcurrent(i, j);
					sum+=temp;
				}


	
	}

	out<<sum<<endl;
	for ( i=9; i>=0; i-- )
	{
		for ( j=0; j<5; j++ )
			out<<tempBox.deck[i][j].num<<" ";
		out<<endl;
	}
	out<<"----------------------------\n";

	return sum;
}

int ComputerPlayer::getcurrent(int x, int y)
{
	int num=visit(x,y);
	int score=0, exitflag=1;
	int i;
	switch ( group[tempBox.deck[x][y].atr] )
	{
	case 3:
	case 4:
	case 5:
	case 1:
		switch ( num )
		{
		case 0: score=0; break;
		case 1: score=20; break;
		case 2: score=80; break;
		case 3: score=160; break;
		case 4: score=320; break;
		case 5: score=640; break;
		}
		break;
	case 2:
		switch ( num )
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
			score=num*8; break;
		case 5:
			score=50; break;
		case 6:
			score=70; break;
		case 7:
			score=120; break;
		default:
			score=num*40-150; break;
		}
		break;
	case 6:
		switch ( num )
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
			score=num*4;
		case 9:
		case 10:
			score=num*9;
		case 11:
			score=120;
		case 12:
			score=200;
		default:
			score=200+(num-12)*10;
		}
	}

	exitflag=1;
	for ( i=9; i>=0 && exitflag ; i-- )
		for ( int j=0; j<5; j++ )
		{
			if(tempBox.deck[i][j].num)
			{
				exitflag=0;
				break;
			}
		}
	
	i++;
	score-=i*i;

	return score;
}


int ComputerPlayer::visit(int x, int y)

{
	if ( x<0 || x>9 || y<0 || y>4 )
		return 0;
	int j=1;
	if ( !board[x][y] )
	{
		board[x][y]=1;
		for ( int k=0; k<4; k++ )
			if ( x+move_x[k]>-1 && x+move_x[k]<5 && y+move_y[k]>-1 && y+move_y[k]<10 )
			{
				if ( tempBox.deck[x][y].atr==-1 )
					tempBox.deck[x][y].atr=0;
				if ( tempBox.deck[x+move_x[k]][y+move_y[k]].atr == -1 )
					tempBox.deck[x+move_x[k]][y+move_y[k]].atr =0;
				if ( group[tempBox.deck[x][y].atr]==group[tempBox.deck[x+move_x[k]][y+move_y[k]].atr] )
					j+=visit(x+move_x[k], y+move_y[k]);
			}

		return j;
	}


}

