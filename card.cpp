#include "card.h"
#include "random.h"
#include <fstream>

Card::Card(int no) {
	set(no);
}
Card::
Card()
{
	num=0; atr=0; mon=0; rmn=4;
}

void Card::set(int no)
{
	if(no >0 && no <= 50)
	{
		num=no;
		mon=(num-1)/4+1; //고유 번호로 달을 정한다. 
		rmn=num%4;
		// atr
		//NULL:0 광:1 비광:2 
		//열(일반):3 열(쌍피):4 열(고도리):5 
		//홍단:6 청단:7 구사:8 
		//쌍피:9 피:10
		//비구사:11
		if(mon==13)
		{
			rmn=4; //rmn은 조커에서는 필요없음.
			atr=9; //조커의 속성은 쌍피
		}
		switch(rmn)
		{
		case 1:
			if(mon==1 || mon==3 || mon==8 || mon==11)
				atr=1;
			if(mon==12)
				atr=2;
			if(mon==7 || mon==10)
				atr=3;
			// mon==9일 때에만 그림쌍피인 것이 맞는 것 아닌가?
			//if(mon==5 || mon==6 || mon==9)
			//	atr=4;
			if (mon==5 || mon==6)
				atr=3;
			if (mon==9)
				atr=4;
			if(mon==2 || mon==4)
				atr=5;
			break;
		case 2:
			if(mon==12)
				atr=3;
			if(mon==8)
				atr=5;
			if(mon==1 || mon==2 || mon==3)
				atr=6;
			if(mon==6 || mon==9 || mon==10)
				atr=7;
			if(mon==4 || mon==5 || mon==7)
				atr=8;
			if(mon==11)
				atr=9;
			break;
		case 3:
			if(mon==12)
				atr=8;//atr=11;		//비초단
			else
				atr=10;
			break;
		case 0:
			if(mon==12)
				atr=9;
			else
				atr=10;
			break;
		} //고유번호에 의한 패의 특성
	}
	else if (no == 51)
	{
		// 얼음패 선언
		num=51;
		mon=-1;
		rmn=4;
		atr=-1;
	}
	else
	{
		// NULL Card 선언, 
		// 1에서 51을 벗어나는 숫자를 입력한 경우는 모두 NULL
		num=0;
		mon=-1;
		rmn=4;
		atr=-1;
	}

}
Cardset::Cardset()
{
	int i;
	randomize();
	for(i=1;i<=50;i++)
		Hwatoo[i]=1; // 모든 카드의 존재여부가 1
	nSet=50; //카드의 수 50
}
Cardpair::Cardpair() {
}

Cardpair::Cardpair(int Card1, int Card2)
{
	Pair[0].set(Card1);
	Pair[1].set(Card2);
	row[0]=8;row[1]=9;col[0]=2;col[1]=2;
}

Cardpair Cardset::decard()
{
//	std::ofstream ofs("log.txt", std::ios::app);

	if(nSet<2) return Cardpair(0,0);

	int selected[2]; selected[0] = -1; selected[1] = -1;

	for (int j=0; j<2; j++) {
		int start=random(nSet)+1;

		for (int i=1; i<=50; i++) {
			if (Hwatoo[i] == 1) {
				start--;
			}

			if (start == 0) {
				selected[j] = i;
				break;
			}
		}
		Hwatoo[selected[j]] = 0;
		nSet-=1;
	}

	return Cardpair(selected[0], selected[1]);
}

void Cardset::incard(int inc)
{
    if (inc >= 1 && inc <= 50) {
		if (Hwatoo[inc] != 1) {
			Hwatoo[inc]=1;
			nSet++;
		}
    }
}

int Cardset::number() { return nSet; }