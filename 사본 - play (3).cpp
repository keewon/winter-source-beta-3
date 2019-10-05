#include "play.h"
#include "game.h"
#include "random.h"
#include "util.h"
#include <fstream>
#include <iostream>


Play::Play(int n, Game* g, Player* p, Player* o) {
	nID = n;
	game = g;
	player = p;
	opponent = o;
	score=0;
	ssak=0;
	how_many_go = 0;
	
	player->SetScore(nID, score);
	if (opponent) opponent->SetScore(1-nID, score);
	

	for(int i=0;i<=9;i++)
	{
		for(int j=0;j<=4;j++)
			checked[i][j]=0;
	}
	for(int i=0;i<=49;i++)
	{
		for(int j=0;j<=49;j++)
			bound[i][j]=0;
	}
	partynum =0;

}

Play::~Play() {
}

// Delay를 줄 때에는 이 함수를 사용!
// milli second 단위로 Wait함.
void Play::Delay(int s) {
    //delaying = true;
    Wait(s);
    //delaying = false;
}

void Play::Redraw() {
    if (opponent)    opponent->Redraw(nID, box);

    if (player)    {
		player->Redraw(nID, box);
		//player->RemainedCard(game->
	}
}

int Play::Main() {
    if (player) {
	Redraw();
    }
    if (player) {
	player->Main();
    }
	
    int key = -1;
    if (player) {
	key = player->Input();
    }
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
    return key;
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

	how_many_go = 0;
	int n=game->GetAttackStatus();
//	ofs<<n<<std::endl;
	if((n<0 && nID==0) || (n>0 && nID==1)) {RecieveIce(n>0?n:-n); game->AttackClear();}
	//RecieveIce(1);
	cardpair = game->Decard();
	player->SetScore(nID, score);
	if (opponent) opponent->SetScore(1-nID, score);
	
	// 더 이상 카드가 없다 -_-
	if (cardpair.Pair[0].num == 0 || cardpair.Pair[1].num == 0) {
		game->Lose(nID);
	}
	//cardpair=Cardset.decard();
	checkclear();
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

/*	i=check(cardpair.row[0], cardpair.col[0]);
	if(i>=3) GiveIce(i);
	if(box.deck[cardpair.row[1]][cardpair.col[1]].num)
	{
		i=check(cardpair.row[1], cardpair.col[1]);
		if(i>=3) GiveIce(i);
	}*/
	check(cardpair.row[0], cardpair.col[0]);
	if(box.deck[cardpair.row[1]][cardpair.col[1]].num) check(cardpair.row[1], cardpair.col[1]);

	if(!LoseCheck()) {
		StartTurn();
		//StartTurn(); //StartTurn은 게임매니저에서 내리는게 좋을것같습니다.
	}
	else {
		player->SoundEffect(4);
		game->Lose(nID);
	} //게임에 Lose를 선언한다.

	int j,k;
	int cardbe=0;
	if(!ssak){
		for(k=0;k<10;k++){
			for(j=0;j<5;j++){
				if(box.deck[k][j].num) cardbe=1;
			}
		}
		if(!cardbe) ssak=1;
	}
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
		box.deck[cardpair.row[0]][cardpair.col[0]]=cardpair.Pair[0];
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

/*void Play::GiveIce(int icepoint) {
	game->Attack(nID,icepoint);
}*/
void Play::RecieveIce(int icepoint) {
	std::ofstream ofs("log.txt", std::ios::app);
	ofs<<icepoint<<" Recieve!"<<std::endl;
	/*int i,j, cha;
	int k=0, l=0;
	int loop;
	for(i=0;i<10;i++){
		for(j=0;j<5;j++){
			if(box.deck[i][j].num>=1 && box.deck[i][j].num<=50) k++;
		}
	}
	if(icepoint>=k)
	{
		cha=icepoint-k;
		for(i=0;i<10;i++){
			for(j=0;j<5;j++){
				if(box.deck[i][j].num>=1 && box.deck[i][j].num<=50){
					game->Incard(box.deck[i][j].num);
					box.deck[i][j].set(51);
				}
			}
		}
		for(i=1;i<=cha;i++){
			if(i<(cha/5)*5){
				j=0;
				while(box.deck[j][(i-(i/5)*5)-1].num!=0) j++;
				box.deck[j][(i-(i/5)*5)-1].set(51);
			}
			else
			{
				int seq[5];
				seq[0]=random(5);
				do seq[1]=random(5); while(seq[1]==seq[0]);
				do seq[2]=random(5); while(seq[2]==seq[0] || seq[2]==seq[1]);
				do seq[3]=random(5); while(seq[3]==seq[0] || seq[3]==seq[1] || seq[3]==seq[2]);
				do seq[4]=random(5); while(seq[4]==seq[0] || seq[4]==seq[1] || seq[4]==seq[2] || seq[4]==seq[3]);
				do seq[5]=random(5); while(seq[5]==seq[0] || seq[5]==seq[1] || seq[5]==seq[2] || seq[5]==seq[3] || seq[5]==seq[4]);
				j=0;
				while(box.deck[j][seq[(i-(i/5)*5)-1]].num!=0) j++;
				box.deck[j][seq[(i-(i/5)*5)-1]].set(51);
			}
		}
	}
	else{
		for(loop=0;loop<icepoint;loop++){
			l=random(k)+1;
			for(i=0;i<10;i++){
				for(j=0;j<5;j++){
					if(box.deck[i][j].num>=1 && box.deck[i][j].num<=50){
						k++;
						if(k==l){
							game->Incard(box.deck[i][j].num);
							box.deck[i][j].set(51);
						}
					}
				}
			}
			k--;
		}
	}*/

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
			box.deck[psy[i]+j-1][i].set(51);
		}
	}

	if(!LoseCheck());
		//StartTurn();//StartTurn이 두번 선언되는 것이 우려됩니다.
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
	int i, j;
	for(i=0;i<=7;i++)
	{
		for(j=0;j<=4;j++)
			box.deck[i][j].set(0);
			checked[i][j]=0;
	}
	partynum=0;
}

int Play::calculate(int part, int ss)
{
	std::ofstream ofs("log.txt", std::ios::app);
	int i,j,k,l; //소모품(?)
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

	int po=0;

/*	int pigrim[50];//그림이 쌍피로 계산되었음을 알리는 플래그
	int pigrimx[50]; //쌍피가 그림으로 계산되었음을 알리는 플래그
	int redonly[50];
	int blueonly[50];
	int musaonly[50];
	int godorionly[50];*/

	int ssg, yy, tt;

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
				{kwang[kw]=checked[i][j];kw++;}
			if(box.deck[i][j].atr==2) bikwang=checked[i][j];
			if(box.deck[i][j].atr/3==1) {yul[y]=checked[i][j];y++;}
			if(box.deck[i][j].atr==4) {ssangrim[sg]=checked[i][j];sg++;}
			if(box.deck[i][j].atr==5) {godori[g]=checked[i][j];g++;}
			if(box.deck[i][j].atr/3==2) {tti[t]=checked[i][j];t++;}
			if(box.deck[i][j].atr==6) {red[r]=checked[i][j];r++;}
			if(box.deck[i][j].atr==7) {blue[b]=checked[i][j];b++;}
			if(box.deck[i][j].atr==8) {musa[m]=checked[i][j];m++;}
			if(box.deck[i][j].atr/3==3) {pi[p]=checked[i][j];p++;}
			if(box.deck[i][j].atr==9) {ssangpi[s]=checked[i][j];s++;}
		}
	}
	i=0;
	while(i!=partynum)
	{
		//각 파티별로 점수를 산출한다.
		point[i]=0;
		k=0;l=0; ssg=0; yy;
		tt=0;
		while(k<sg && sg!=0) {if(ssangrim[k]==i) ssg++; k++;}
		k=0;
		while(k<kw && kw!=0) {if(kwang[k]==i) l++; k++;}
		if(l>=3) point[i]=l;
		if(l==3 && bikwang==i) point[i]=2;
		if(l==5) point[i]=15;
		if(l<3) point[i]=0;
		k=0;l=0;
		while(k<y && y!=0) {if(yul[k]==i) l++; yy++; k++;}
		if(ss==0) l-=ssg;
		if(l>=5) point[i]+=l-4;
		if(godori[0]==i&&godori[1]==i&&godori[2]==i) point[i]+=5;
		k=0;l=0;
		while(k<t && t!=0) {if(tti[k]==i) tt++; l++; k++;}
		if(l>=5) point[i]+=l-4;
		if(red[0]==i&&red[1]==i&&red[2]==i) point[i]+=3;
		if(blue[0]==i&&blue[1]==i&&blue[2]==i) point[i]+=3;
		k=0;l=0;
		while(k<m && m!=0) {if(musa[k]==i) l++; k++;}
		if(l>=3) point[i]+=3;
		k=0;l=0;
		while(k<p && p!=0) {if(pi[k]==i) l++; k++;}
		if(ss==0) l+=ssg*2;
		if(l>=10){
			point[i]+=l-9;
			k=0;j=0;
			while(k<s && s!=0) {if(ssangpi[k]==i) j++; k++;}
			point[i]+=j;
//			if(ss==0) pigrimx[i]=1;
		}
		i++;
	}
	//점수 합산부
	if(i)
	{
		for(j=1;j<=partynum;j++)
			if(bound[part][j] || bound[j][part] || j==part) po+=point[j];
	}
//	if(po) ofs<<"point! :"<<po<<std::endl;

//	ofs<<score<<std::endl;
	return po;
}

void Play::check(int row, int col)
{
	int lasti=-1, lastj=-1;

	std::ofstream ofs("log.txt", std::ios::app);
	//check시, 쌍피 그림을 피에다 편입시킨 check결과를 하나 더 
	//넣고 두 결과를 비교하는 것이 어떨까 함.
	//이 방법은 힘들것 같으니 쌍피 그림이 피 파트에 인접한 경우만 생각하기로 하자.
	checkclear();
	int i,j,k;
	int ssagri=0;
	int non=1;
	int pnt[2];
	int chk1=0, chk2=0;
	int godoripnt=0;
	for(k=0;k<2;k++){
		for(i=0;i<=9 /*&& deck[i-1][0].num+deck[i-1][1].num+deck[i-1][2].num+
			deck[i-1][3].num+deck[i-1][4].num!=0*/;i++)
		{
			for(j=0;j<=4;j++)
			{//모든 칸 탐색
				if(box.deck[i][j].num){//그 칸에 패가 있을때만 체크함.
					non=1; chk1=0; chk2=0;
					if(j!=0){
						if(box.deck[i][j-1].num){//왼쪽 칸에 패가 있을 경우
							if(k==1){
								if(box.deck[i][j-1].atr/3==box.deck[i][j].atr/3){
									checked[i][j]=checked[i][j-1];//check값을 같이함.
									chk1=1;//왼쪽 값과 같은 파티에 편성되었음을 표시.
								}
							}
							else{
								if(box.deck[i][j-1].atr/3==box.deck[i][j].atr/3 ||
									(box.deck[i][j-1].atr==4 && box.deck[i][j].atr/3==3) ||
									(box.deck[i][j-1].atr/3==3 && box.deck[i][j].atr==4)){//같은 특성이면
									checked[i][j]=checked[i][j-1];//check값을 같이함.
									chk1=1;//왼쪽 값과 같은 파티에 편성되었음을 표시.
								}
							}
							non=0;//왼쪽, 아래에 어느곳에도 패가 없을 가능성 배재
						}
					}
					if(i!=0){
						if(box.deck[i-1][j].num){//아래칸에 패가 있을 경우
							if(k==1){
								if(box.deck[i-1][j].atr/3==box.deck[i][j].atr/3){
									checked[i][j]=checked[i-1][j];
									if(chk1) unify(checked[i][j-1],checked[i-1][j]);
									//아래 값과 같은 파티에 편성되었음을표시
									chk2=1;
								}
							}
							else{
								if(box.deck[i-1][j].atr/3==box.deck[i][j].atr/3 || 
									(box.deck[i-1][j].atr==4 && box.deck[i][j].atr/3==3) ||
									(box.deck[i-1][j].atr/3==3 && box.deck[i][j].atr==4)){
									checked[i][j]=checked[i-1][j];
									if(chk1) unify(checked[i][j-1],checked[i-1][j]);
									//아래 값과 같은 파티에 편성되었음을표시
									chk2=1;
								}
							}
							non=0;
						}
					}
					if(non) {
						checked[i][j]=++partynum;
					}
					else{
						if(chk1==0 && chk2==0) checked[i][j]=++partynum;
						if(!chk1 && j!=0) bound[checked[i][j-1]][checked[i][j]]=1;
						//왼쪽칸과 본칸의 경계를 침
						if(!chk2 && i!=0) bound[checked[i-1][j]][checked[i][j]]=1;
						//아래칸과 본칸의 경계를 침
					}
				}
			}
		}
		pnt[k]=calculate(checked[row][col], k);
	}
	if(pnt[0]>pnt[1]){
		non=1;chk1=0;chk2=0;
		checkclear();
		for(i=0;i<=9 /*&& deck[i-1][0].num+deck[i-1][1].num+deck[i-1][2].num+
			deck[i-1][3].num+deck[i-1][4].num!=0*/;i++)
		{
			for(j=0;j<=4;j++)
			{//모든 칸 탐색
				if(box.deck[i][j].num){//그 칸에 패가 있을때만 체크함.
					non=1; chk1=0; chk2=0;
					if(j!=0){
						if(box.deck[i][j-1].num){//왼쪽 칸에 패가 있을 경우
							if(box.deck[i][j-1].atr/3==box.deck[i][j].atr/3 ||
							(box.deck[i][j-1].atr==4 && box.deck[i][j].atr/3==3) ||
								(box.deck[i][j-1].atr/3==3 && box.deck[i][j].atr==4)){
								//같은 특성이면
								checked[i][j]=checked[i][j-1];//check값을 같이함.
								chk1=1;//왼쪽 값과 같은 파티에 편성되었음을 표시.
							}
							non=0;//왼쪽, 아래에 어느곳에도 패가 없을 가능성 배재
						}
					}
					if(i!=0){
						if(box.deck[i-1][j].num){//아래칸에 패가 있을 경우
							if(box.deck[i-1][j].atr/3==box.deck[i][j].atr/3 || 
								(box.deck[i-1][j].atr==4 && box.deck[i][j].atr/3==3) ||
								(box.deck[i-1][j].atr/3==3 && box.deck[i][j].atr==4)){
								checked[i][j]=checked[i-1][j];
								if(chk1) unify(checked[i][j-1],checked[i-1][j]);
								//아래 값과 같은 파티에 편성되었음을표시
								chk2=1;
							}
							non=0;
						}
					}
					if(non) {
						checked[i][j]=++partynum;
					}
					else{
						if(chk1==0 && chk2==0) checked[i][j]=++partynum;
						if(!chk1 && j!=0) bound[checked[i][j-1]][checked[i][j]]=1;
						//왼쪽칸과 본칸의 경계를 침
						if(!chk2 && i!=0) bound[checked[i-1][j]][checked[i][j]]=1;
						//아래칸과 본칸의 경계를 침
					}
				}
			}
		}
		calculate(checked[row][col],0);
	}//체크를 다시 처음부터 강행한다.
	godoripnt=pnt[0]>pnt[1]?pnt[0]:pnt[1];
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
//	ofs<<score<<std::endl;



	if(godoripnt>=3 || (godoripnt >= 1 && how_many_go > 1) )
	{
		if(point[checked[row][col]]){
			player->SoundEffect(1);
			for(j=1;j<=partynum;j++){
				if(bound[checked[row][col]][j] || bound[j][checked[row][col]] || j==checked[row][col])
				{
					if(point[j])//인접파티에 점수가 배당되어 있을때
					{
						for(i=9;i>=0;i--)
						{
							for(k=4;k>=0;k--)
							{
								if(checked[i][k]==j) {
									game->Incard(box.dedeck(i,k));
									player->DrawBound(nID, i,k);
									if (opponent) opponent->DrawBound(nID, i, k);
									player->Redraw(nID, box);
									if (opponent) opponent->Redraw(nID, box);
									lasti=i;lastj=j;
								}
							}
						}
						if (0 <= i && i < 9 && 0 <= k && k <= 4) {
							check(lasti,lastj);
						}
					}
				}
			}
			score+=godoripnt*100*(ssak+1)+how_many_go*50;
			ssagri=ssak;
			ssak=0;
			game->Attack(nID, godoripnt*(ssagri+1)+how_many_go);
			ofs<<godoripnt*(ssagri+1)+how_many_go<<std::endl;
			//how_many_go++;
			//player->SetScore(nID, how_many_go);
			player->DrawBound(nID, -1, 0);	// Bound를 표시하라는 명령
			if (opponent) opponent->DrawBound(nID, -1, 0);
			player->Attack(++how_many_go);
			Delay(1000);
		}
	}
	score+=godoripnt*10*(ssagri+1);

}

void Play::checkclear()
{
	int i,j;
	for(i=9;i>=0;i--)
	{
		for(j=4;j>=0;j--)
		{
			checked[i][j]=0;
			//box.checked를 초기화시킨다.
		}
	}
	for(i=0;i<=49;i++)
	{
		point[i]=0;
		for(j=0;j<=49;j++)
		{
			bound[i][j]=0;
		}
	}
	partynum=0;
}

void Play::unify(int num1, int num2)
{
	int small1;
	int large;
	int i,j;
	if(num1>num2){ large=num1; small1=num2; }
	else{ small1=num1; large=num2; }
	for(i=0;i<=9;i++){
		for(j=0;j<=4;j++){
			if(checked[i][j]==large) checked[i][j]=small1;
		}
	}
}
