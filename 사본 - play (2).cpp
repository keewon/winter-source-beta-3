#include "play.h"
#include "game.h"
#include "random.h"
#include "util.h"
#include <fstream>
#include <iostream>

int point[50];
int po;
int lasti, lastj;

Play::Play(int n, Game* g, Player* p, Player* o) {
	nID = n;
	game = g;
	player = p;
	opponent = o;

	for(int i=0;i<=9;i++)
	{
		for(int j=0;j<=4;j++)
		{
			for(int k=0;k<2;k++) checked[i][j][k]=0;
		}
	}
	for(int i=0;i<=49;i++)
	{
		for(int j=0;j<=49;j++)
		{
			for(int k=0;k<2;k++) bound[i][j][k]=0;
		}
	}
	partynum =0;

}

Play::~Play() {
}

// Delay를 줄 때에는 이 함수를 사용!
// milli second 단위로 Wait함.
void Play::Delay(int s) {
    Wait(s);
}

void Play::Redraw() {
    if (player)    {
		player->Redraw(nID, box);
		//player->RemainedCard(game->
	}
    if (opponent)    opponent->Redraw(nID, box);
}

int Play::Main() {
    if (player) {
	player->Main();
	int key = player->Input();
	switch(key) {
	case 5:
		RotateClock();
		break;
	case 4:
		MoveLeft();
		break;
	case 6:
		MoveRight();
		break;
	case 2:
		MoveDown();
		break;
	case -1:
		break;
	}
	if (key != 0) {
	    Redraw();
	}
	return key;
    }
    else return -1;
}

void 
Play::Init() {
    if (player)    {
		player->SoundEffect(0);
	}
    if (opponent)    opponent->SoundEffect(0);

}

void
Play::StartTurn() {
//	std::ofstream ofs("log.txt", std::ios::app);
//	ofs << std::endl;
//	int i;
//	i=game->GetAttackStatus(nID);
//	if(i) RecieveIce(i);
	cardpair = game->Decard();
	
	// 더 이상 카드가 없다 -_-
	if (cardpair.Pair[0].num == 0 || cardpair.Pair[1].num == 0) {
		game->Lose(nID);
	}
	//cardpair=Cardset.decard();
	Redraw();
	checkclear();
}

void
Play::EndTurn() {
    	player->SoundEffect(7);
	int i;
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

	i=check(cardpair.row[0], cardpair.col[0]);
	if(i>=3) GiveIce(i);
	if(box.deck[cardpair.row[1]][cardpair.col[1]].num)
	{
		i=check(cardpair.row[1], cardpair.col[1]);
		if(i>=3) GiveIce(i);
	}

	if(!LoseCheck()) {
		StartTurn();
		//StartTurn(); //StartTurn은 게임매니저에서 내리는게 좋을것같습니다.
	}
	else {
		player->SoundEffect(4);
		game->Lose(nID);
	} //게임에 Lose를 선언한다.
}

void
Play::MoveLeft() {
	box.deck[cardpair.row[0]][cardpair.col[0]].set(0);
	box.deck[cardpair.row[1]][cardpair.col[1]].set(0);
	if(cardpair.col[0]!=0&&cardpair.col[1]!=0
		&& box.deck[cardpair.row[0]][cardpair.col[0]-1].num==0
		&& box.deck[cardpair.row[1]][cardpair.col[1]-1].num==0){
		cardpair.col[0]--;cardpair.col[1]--;
	}
	box.deck[cardpair.row[0]][cardpair.col[0]]=cardpair.Pair[0];
	box.deck[cardpair.row[1]][cardpair.col[1]]=cardpair.Pair[1];
	player->SoundEffect(6);
}

void
Play::MoveRight() {
	box.deck[cardpair.row[0]][cardpair.col[0]].set(0); 
	box.deck[cardpair.row[1]][cardpair.col[1]].set(0);
	if(cardpair.col[0]!=4&&cardpair.col[1]!=4
		&& box.deck[cardpair.row[0]][cardpair.col[0]+1].num==0
		&& box.deck[cardpair.row[0]][cardpair.col[1]+1].num==0) {
		cardpair.col[0]++;cardpair.col[1]++;
	}
	box.deck[cardpair.row[0]][cardpair.col[0]]=cardpair.Pair[0];
	box.deck[cardpair.row[1]][cardpair.col[1]]=cardpair.Pair[1];
	player->SoundEffect(6);
}

void
Play::MoveDown() {
	int method; //밑에 쌓인 카드패가 있는지의 여부를 검사하는 방법 지정
	int end; //턴을 끝낼 것인지의 여부
	int m;
	//std::cout << "Cardpair :";
	//std::cout << cardpair.row[0] << " " << cardpair.row[1] << std::endl;
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
		box.deck[cardpair.row[0]][cardpair.col[0]]=cardpair.Pair[0];box.deck[cardpair.row[1]][cardpair.col[1]]=cardpair.Pair[1];
	}
	else {
		EndTurn();
	}
}

void
Play::RotateClock() {
	int r0, r1, c0, c1;
	int temp1, temp2;
	r0=cardpair.row[0]; r1=cardpair.row[1];
	c0=cardpair.col[0]; c1=cardpair.col[1];
	temp1=cardpair.row[1]+r0-r1+c0-c1;
	temp2=cardpair.col[1]+r1-r0+c0-c1;
	if(temp1>=0&&temp2>=0&&temp2<5)
	{
		box.deck[cardpair.row[1]][cardpair.col[1]].set(0);
		//box.deck[cardpair.row[0]][cardpair.col[0]].set(0);
		if(box.deck[temp1][temp2].num==0){
			cardpair.row[1]=temp1;
			cardpair.col[1]=temp2;
		}
		box.deck[cardpair.row[1]][cardpair.col[1]]=cardpair.Pair[1];
		//box.deck[cardpair.row[0]][cardpair.col[0]]=cardpair.Pair[0];
	}
	player->SoundEffect(5);
}

void
Play::RotateNonClock() {
	int r0, r1, c0, c1;
	int temp1, temp2;
	r0=cardpair.row[0]; r1=cardpair.row[1];
	c0=cardpair.col[0]; c1=cardpair.col[1];
	temp1=cardpair.row[0]+r0-r1+c1-c0;
	temp2=cardpair.col[0]+r0-r1+c0-c1;
	if(temp1>=0&&temp2>=0&&temp2<5)
	{
		box.deck[cardpair.row[0]][cardpair.col[0]].set(0);
		//box.deck[cardpair.row[1]][cardpair.col[1]].set(0);
		cardpair.row[0]=temp1;
		cardpair.col[0]=temp2;
		box.deck[cardpair.row[0]][cardpair.col[0]]=cardpair.Pair[0];
		//box.deck[cardpair.row[1]][cardpair.col[1]]=cardpair.Pair[1];
	}
	player->SoundEffect(5);
}

void Play::GiveIce(int icepoint) {
//	game->Attack(nID,icepoint);
}
void Play::RecieveIce(int icepoint) {
	int icecol[5];
	int i,j,r,pass;
	int psy[5];
	for(i=0;i<5;i++) {
		psy[i] = 0;
		icecol[i]=icepoint/5;
		while(box.deck[psy[i]][i].num!=0)
			psy[i]++;
	}
	j=icepoint-icecol[0]*5;
	pass=icecol[0];
	if(j){
		for(i=1;i<=j;i++)
		{
			do {
				r=random(5);
			} while(icecol[r]>pass);
			icecol[r]++;
		}
	}

	for(i=0;i<5;i++) {
		for(j=1;j<=icecol[i];j++)
		{
			box.deck[psy[i]+j][i].set(51);
		}
	}
	if(!LoseCheck())
		StartTurn();//StartTurn이 두번 선언되는 것이 우려됩니다.
	else {
		game->Lose(nID);
	} //게임에 Lose를 선언한다.
}
int Play::LoseCheck() {
	int i;
	i=box.deck[8][0].num||box.deck[8][1].num||box.deck[8][2].num||box.deck[8][3].num||box.deck[8][4].num;
	return i;
}

void Play::clear()
{
	int i, j, k;
	for(i=0;i<=9;i++)
	{
		for(j=0;j<=4;j++)
		{
			box.deck[i][j].set(0);
			for(k=0; k<2; k++) checked[i][j][k]=0;
		}
	}
	partynum=0;
}

int Play::calculate(int part, int sec)
{
//	std::ofstream ofs("log.txt", std::ios::app);
	int i,j,k,l; //소모품(?)
	//int lasti, lastj;
	int kwang[5],kw=0;
	int bikwang=0;
	int tti[10],t=0;
	int pi[26],p=0;
	int ssangpi[4],s=0;
	int blue[3],b=0;
	int red[3],r=0;
	int musa[4],m=0;
	int godori[3],g=0;
	int ssangrim[3],sg=0;
	int yul[9],y=0;

	//int point[50];
	po=0;
	for(i=0;i<=49;i++) point[i]=0;
	for(i=0;i<=4;i++) kwang[i]=0;
	for(i=0;i<=9;i++) tti[i]=0;
	for(i=0;i<=25;i++) pi[i]=0;
	for(i=0;i<=3;i++) ssangpi[i]=0;
	for(i=0;i<=2;i++) blue[i]=0;
	for(i=0;i<=2;i++) red[i]=0;
	for(i=0;i<=3;i++) musa[i]=0;
	for(i=0;i<=2;i++) godori[i]=0;
	for(i=0;i<=2;i++) ssangrim[i]=0;
	for(i=0;i<=9;i++)
	{
		for(j=0;j<=4;j++)
		{
			//속성을 검사해서 각 int 배열에 체크값을 넘김.
			if(box.deck[i][j].atr==1 || box.deck[i][j].atr==2)
				{kwang[kw]=checked[i][j][sec];kw++;}
			if(box.deck[i][j].atr==2) bikwang=checked[i][j][sec];
			if(box.deck[i][j].atr/3==1) {yul[y]=checked[i][j][sec];y++;}
			if(box.deck[i][j].atr==4) {ssangrim[sg]=checked[i][j][sec];sg++;}
			if(box.deck[i][j].atr==5) {godori[g]=checked[i][j][sec];g++;}
			if(box.deck[i][j].atr/3==2) {tti[t]=checked[i][j][sec];t++;}
			if(box.deck[i][j].atr==6) {red[r]=checked[i][j][sec];r++;}
			if(box.deck[i][j].atr==7) {blue[b]=checked[i][j][sec];b++;}
			if(box.deck[i][j].atr==8) {musa[m]=checked[i][j][sec];m++;}
			if(box.deck[i][j].atr/3==3) {pi[p]=checked[i][j][sec];p++;}
			if(box.deck[i][j].atr==9) {ssangpi[s]=checked[i][j][sec];s++;}
		}
	}
	i=0;
	while(i!=partynum)
	{
		//각 파티별로 점수를 산출한다.
		k=0;l=0;
		while(k<kw && kw!=0) {if(kwang[k]==i) l++; k++;}
		if(l>=3) point[i]=l;
		if(l==3 && bikwang==i) point[i]=2;
		if(l==5) point[i]=15;
		if(l<3) point[i]=0;
		k=0;l=0;
		while(k<y && y!=0) {if(yul[k]==i) l++; k++;}
		if(l>=5) point[i]+=l-4;
		if(godori[0]==i&&godori[1]==i&&godori[2]==i) point[i]+=5;
		k=0;l=0;
		while(k<t && t!=0) {if(tti[k]==i) l++; k++;}
		if(l>=5) point[i]+=l-4;
		if(red[0]==i&&red[1]==i&&red[2]==i) point[i]+=3;
		if(blue[0]==i&&blue[1]==i&&blue[2]==i) point[i]+=3;
		k=0;l=0;
		while(k<m && m!=0) {if(musa[k]==i) l++; k++;}
		if(l>=3) point[i]+=3;
		k=0;l=0;
		while(k<p && p!=0) {if(pi[k]==i) l++; k++;}
		if(l>=10){
			point[i]+=l-9;
			k=0;j=0;
			while(k<s && s!=0) {if(ssangpi[k]==i) j++; k++;}
			point[i]+=j;
		}
		i++;
	}
	//점수 합산부
	if(i)
	{
		for(j=1;j<=partynum;j++)
			if(bound[part][j][sec] || bound[j][part][sec] || j==part) po+=point[j];
	}
//	if(po) ofs<<"point! :"<<po<<std::endl;
/*	if(po>=3)
	{
	    player->SoundEffect(1);
		for(j=1;j<=partynum;j++){
			if(bound[part][j] || bound[j][part] || j==part)
			{
				if(point[j])//인접파티에 점수가 배당되어 있을때
				{
					for(i=9;i>=0;i--)
					{
						for(k=4;k>=0;k--)
						{
							if(checked[i][k]==j) {
								game->Incard(box.dedeck(i,k));
								lasti=i;lastj=j;}
							//이 부분에서, 고도리나 홍단 청단이 단독으로
							//점수가 날 경우에도 모든 파트가 없어져버림.
							//개선 필요.
						}
					}
					check(lasti,lastj);
				}
			}
		}
	}*/
	return po;

}

int Play::check(int row, int col)
{
//	std::ofstream ofs("log.txt", std::ios::app);
	//check시, 쌍피 그림을 피에다 편입시킨 check결과를 하나 더 
	//넣고 두 결과를 비교하는 것이 어떨까 함.
	//이 방법은 힘들것 같으니 쌍피 그림이 피 파트에 인접한 경우만 생각하기로 하자.
	checkclear();
	int i,j,p;
	int non=1;
	int chk1=0, chk2=0;
	int pon[2];//
	for(p=0;p<2;p++){//
	for(i=0;i<=9 /*&& deck[i-1][0].num+deck[i-1][1].num+deck[i-1][2].num+
		deck[i-1][3].num+deck[i-1][4].num!=0*/;i++)
	{
		for(j=0;j<=4;j++)
		{//모든 칸 탐색
			if(box.deck[i][j].num){//그 칸에 패가 있을때만 체크함.
				non=1; chk1=0; chk2=0;
				if(j!=0){
					if(box.deck[i][j-1].num){//왼쪽 칸에 패가 있을 경우
						if(!p){//
						if(box.deck[i][j-1].atr/3==box.deck[i][j].atr/3){//같은 특성이면
							checked[i][j][p]=checked[i][j-1][p];//check값을 같이함.
							chk1=1;//왼쪽 값과 같은 파티에 편성되었음을 표시.
						}
						non=0;//왼쪽, 아래에 어느곳에도 패가 없을 가능성 배재
						}//
						else{//
						if((box.deck[i][j-1].atr!=4 && box.deck[i][j].atr!=4
							&& box.deck[i][j-1].atr/3==box.deck[i][j].atr/3) ||
							(box.deck[i][j-1].atr==4 && box.deck[i][j].atr/3==3) ||
							(box.deck[i][j-1].atr/3==3 && box.deck[i][j].atr==4) ||
							(box.deck[i][j-1].atr==box.deck[i][j].atr)){//같은 특성이면
							checked[i][j][p]=checked[i][j-1][p];//check값을 같이함.
							chk1=1;//왼쪽 값과 같은 파티에 편성되었음을 표시.
						}
						non=0;//왼쪽, 아래에 어느곳에도 패가 없을 가능성 배재
						}//
					
					}
				}
				if(i!=0){
					if(box.deck[i-1][j].num){//아래칸에 패가 있을 경우
						if(!p){//
						if(box.deck[i-1][j].atr/3==box.deck[i][j].atr/3){//같은 특성이면
							checked[i][j][p]=checked[i-1][j][p];
							if(chk1) unify(checked[i][j-1][p],checked[i-1][j][p]);
							//아래 값과 같은 파티에 편성되었음을표시
							chk2=1;
						}
						non=0;
						}//
						else{//
						if((box.deck[i-1][j].atr!=4 && box.deck[i][j].atr!=4
							&& box.deck[i-1][j].atr/3==box.deck[i][j].atr/3) ||
							(box.deck[i-1][j].atr==4 && box.deck[i][j].atr/3==3) ||
							(box.deck[i-1][j].atr/3==3 && box.deck[i][j].atr==4) ||
							(box.deck[i-1][j].atr==box.deck[i][j].atr)){//같은 특성이면
							checked[i-1][j][p]=checked[i-1][j][p];
							if(chk1) unify(checked[i][j-1][p],checked[i-1][j][p]);
							//아래 값과 같은 파티에 편성되었음을표시
							chk2=1;
						}
						non=0;
						}//
					}
				}
				if(non) {
					checked[i][j][p]=++partynum;
				}
				else{
					if(chk1==0 && chk2==0) checked[i][j][p]=++partynum;
					if(!chk1 && j!=0) bound[checked[i][j-1][p]][checked[i][j][p]][p]=1;
					//왼쪽칸과 본칸의 경계를 침
					if(!chk2 && i!=0) bound[checked[i-1][j][p]][checked[i][j][p]][p]=1;
					//아래칸과 본칸의 경계를 침
				}
			}
		}
	}
/*
	for(i=9;i>=0;i--)
	{
		for(j=0;j<=4;j++) ofs<<checked[i][j]<<" ";
		ofs<<" ";
		for(j=0;j<=4;j++) ofs<<deck[i][j].atr<<"\t";
		ofs<<" ";
		for(j=0;j<=4;j++) ofs<<deck[i][j].num<<"\t";
		ofs<<std::endl;
	}
*/
	pon[p]=calculate(checked[row][col][p], p);
	}//
	//return calculate(checked[row][col]);
	p=pon[0]<pon[1]?1:0;
	if(pon[p]>=3)
	{
	    player->SoundEffect(1);
		for(j=1;j<=partynum;j++){
			if(bound[checked[row][col][p]][j][p] || bound[j][checked[row][col][p]][p] || j==checked[row][col][p])
			{
				if(point[j])//인접파티에 점수가 배당되어 있을때
				{
					for(i=9;i>=0;i--)
					{
						for(int k=4;k>=0;k--)
						{
							if(checked[i][k][p]==j) {
								game->Incard(box.dedeck(i,k));
								lasti=i;lastj=j;}
							//이 부분에서, 고도리나 홍단 청단이 단독으로
							//점수가 날 경우에도 모든 파트가 없어져버림.
							//개선 필요.
						}
					}
					check(lasti,lastj);
				}
			}
		}
	}
	return pon[p];
}

void Play::checkclear()
{
	int i,j,k;
	for(i=9;i>=0;i--)
	{
		for(j=4;j>=0;j--)
		{
			for(k=0;k<2;k++) checked[i][j][k]=0;
			//box.checked를 초기화시킨다.
		}
	}
	for(i=0;i<=49;i++)
	{
		for(j=0;j<=49;j++)
		{
			for(k=0;k<2;k++) bound[i][j][k]=0;
		}
	}
	partynum=0;
}

void Play::unify(int num1, int num2)
{
	int small1;
	int large;
	int i,j,k;
	if(num1>num2){ large=num1; small1=num2; }
	else{ small1=num1; large=num2; }
	for(i=0;i<=9;i++){
		for(j=0;j<=4;j++){
			for(k=0;k<2;k++)
			{
				if(checked[i][j][k]==large) checked[i][j][k]=small1;
			}
		}
	}
}
