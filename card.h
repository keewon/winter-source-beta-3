#ifndef _CARD_PUREAREA_H_
#define _CARD_PUREAREA_H_

class Card
{
public:
	int num; //1~50사이의 화투장 고유의 번호 0은 NULL Card, 51은 얼음카드
	int atr; //화투장의 속성. 광, 피, 쌍피, 쌍피/그림, 그림, 띠 등
	int mon; //화투가 속해 있는 달 판단
	int rmn; //한 달 안에서 차지하는 0~3사이의 서열(?)
	Card();

	Card(int no); //Card가 생성될 때 번호를 입력받아 속성을 생성한다.
	void set(int no);

};

class Cardpair
{
public:
	Card Pair[2];
	Cardpair();
	Cardpair(int Card1, int Card2);
	int row[2], col[2];
};

class Cardset
{
	int nSet; //남아있는 카드 수
	int Hwatoo[52]; //그 카드가 셋에 있는지의 여부. 1/0
public:
	Cardset(); //카드셋은 처음에 50장 모두 채워져 있다.
	Cardpair decard(); //세트에서 카드 제거하여 새로운 페어 생성
	void incard(int inc); //세트로 카드 회수
	int number();	//개수
};


#endif
