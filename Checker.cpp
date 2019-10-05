#include "Checker.h"
#include "Game.h"

namespace Checker {
	Party::Party() {
		number = 0;
		type = NONE;
		x=y=c=0;
	}

	//int Party::Size() {		return number;	}

	Party::~Party() {
		if (x) delete [] x;
		if (y) delete [] y;
		if (c) delete [] c;
	}

	CheckBox::CheckBox() {
		for (int i=0; i<10; i++) {
			for (int j=0; j<5; j++) {
				box[i][j] = false;
			}
		}
	}

	bool Party::Add(int x_, int y_, int n_) {
		if (number >= MAX_NUM_CARD) return false;
		Card temp; temp.set(n_);
		switch (temp.atr) {
		case -1:
		case 0:
			return false;
			break;
		case 1:
		case 2:
			// 광
			if (type != KWANG && type != NONE) {
				return false;
			} else if (type == NONE) {
				x = new int [5];
				y = new int [5];
				c = new int [5];
				type=KWANG;
			}
			break;
		case 3:
		case 5:
			// 열
			if (type != YEOL && type != NONE) {
				return false;
			} else if (type == NONE) {
				x = new int [9];
				y = new int [9];
				c = new int [9];
				type=YEOL;
			}
			break;
		case 4:
			// 쌍피열
			return false;
			break;
		case 6:
		case 7:
		case 8:
		case 11:
			// 띠
			if (type != TI && type != NONE) {
				return false;
			} else if (type == NONE) {
				x = new int [10];
				y = new int [10];
				c = new int [10];
				type = TI;
			}
			break;
		case 9:
		case 10:
			// 피
			if (type != PI && type != NONE) {
				return false;
			} else if (type == NONE) {
				x = new int [28];
				y = new int [28];
				c = new int [28];
				type = PI;
			}
			break;
		}

		if (IsIn(n_) == false) {
			x[number] = x_;
			y[number] = y_;
			c[number] = n_;
			number++; 
			return true;
		}
		else return false;
	}

	bool Party::IsIn(int n_) {
		if (n_ == 0) return true;
		for (int i=0; i<number; i++) {
			if (c[i] == n_) return true;
		}
		return false;
	}

	bool Party::Add(int x_, int y_, int n_, method m) {
		if (n_ < 1 || 50 < n_) return false;
		Card temp; temp.set(n_);
		switch(temp.atr) {
		case 0:
		case -1:
			return false; break;
		case 4:
			if (type != NONE && m != type) {
				return false;
			}
			break;
		default:
			return Add(x_, y_, n_);
		}

		if (IsIn(n_) == false) {
			if (type == NONE) {

				if (m == PI) {
					x = new int [28];
					y = new int [28];
					c = new int [28];
				}
				else if (m == YEOL) {
					x = new int [9];
					y = new int [9];
					c = new int [9];
				}
				else {
					return false;
				}
				type = m;
			}
			x[number] = x_;
			y[number] = y_;
			c[number] = n_;
			number++; 
			
			return true;
		}
		else return false;

		
	}

	int Party::Score() {
		if (number < 3) return 0;
		// 서로 떨어져있는 고도리나 청단도 같은 종류의
		// 패로 연결되어있으면 점수를 내게 되어있다.
		switch (type) {
		case NONE:
			return 0;
			break;
		case KWANG:
			{
				switch (number) {
				case 5:
					return 15;
					break;
				case 4:
					return 4;
					break;
				case 3:
					if (IsIn(45)) {
						return 2;
					}
					else return 3;
					break;

				default:
					return 0;
				}
			}
			break;
		case PI:
			{
				int v = number-9;
				if (IsIn(33)) v++; //국화열
				if (IsIn(42)) v++; //똥쌍피
				if (IsIn(48)) v++; //비쌍피
				if (IsIn(49)) v++; //상표
				if (IsIn(50)) v++; //상표

				if (v > 0) {
					return v;
				}
				else return 0;
			}
			break;
		case TI:
			{
				int v = number -4;
				if (v < 0) { v=  0;	}

				if (IsIn(22) && IsIn(34) && IsIn(38) ) { //청단
					v+=3;
				}
				if (IsIn(02) && IsIn(06) && IsIn(10) ) { //홍단
					v+=3;
				}
				if (IsIn(14) && IsIn(18) && IsIn(26) ) { //초단
					v+=3;
				}
				
				return v;
			}
			break;
		case YEOL:
			{
				int v = number-4;
				if (v < 0) { v=0; }

				if (IsIn(05) && IsIn(13) && IsIn(30) ) { //고도리
					v+=5;
				}
				return v;
			}
			break;
		}
		return 0;
	}

	//int Party::Type() { return (int)type;	}

	int Party::Heuristic() { 
		if (number == 1) return -500;
		switch(type) {
		case PI:
			{
				if (number >= 10) return number * 200;
				return (number) * 100;
			}
			break;
		case KWANG:
			{
				if (number == 5) return 100000;
				else if (number >= 3) return number*1000;
				else return (number-1) * 100;
			}
			break;
		case YEOL:
			{
				int score=0;
				int godori=(int)(IsIn(05)) + (int)(IsIn(13)) + (int)(IsIn(30));
				if (godori >= 2) score += godori*400;
				else if (godori >= 3) score += 5000;

				score += (number-1) * 10;

				return score;
			}
			break;
		case TI:
			{
				int score=0;
				int blue =(int)(IsIn(22)) + (int)(IsIn(34)) + (int)(IsIn(38));
				int red  =(int)(IsIn(02)) + (int)(IsIn(06)) + (int)(IsIn(10));
				int cho  =(int)(IsIn(14)) + (int)(IsIn(18)) + (int)(IsIn(26));

				if (blue >= 2) score += blue*400;
				else if (blue == 3) score += blue*1000;
				if (red >= 2) score += red*400;
				else if (red == 3) score += red*1000;
				if (cho >= 2) score += cho*400;
				else if (cho == 3) score += cho*1000;

				score += (number-1) * 10;
				return score;
			}
			break;
		default:
			return 0;
		}
	}

/*
	void Party::DrawBound(Player* p1, Player* p2, int nID) {
		if (type != NONE || number > 1) {
			if (p1) p1->DrawBound(nID, this);
			if (p2) p2->DrawBound(nID, this);
		}
	}
*/
	void Party::Incard(Game* game, Box& box, Player* p1, Player* p2, int nID) {
		if (!game) return;
		switch(type) {
		case NONE:
			return;
		case KWANG:
		case PI:
			{
				for (int i=0; i<number; i++) {
					if (1 <= c[i] && c[i] <= 50) {
						game->Incard(c[i]);
					}
					box.dedeck(x[i], y[i]);
					if (p1) p1->DrawBound(nID, x[i], y[i]);
					if (p2) p2->DrawBound(nID, x[i], y[i]);
				}
			}
			break;
		case YEOL:
			{
				int v = number-4;
				if (v <= 0 && IsIn(05) && IsIn(13) && IsIn(30) ){
					game->Incard(05);
					game->Incard(13);
					game->Incard(30);
					for (int i=0; i<number; i++) {
						if (c[i] == 5 || c[i] == 13 || c[i] == 30) {
							box.dedeck(x[i], y[i]);
							if (p1) p1->DrawBound(nID, x[i], y[i]);
							if (p2) p2->DrawBound(nID, x[i], y[i]);
						}
					}
				} else if (v >= 1) {
					for (int i=0; i<number; i++) {
						if (1 <= c[i] && c[i] <= 50) {
							game->Incard(c[i]);
						}
						box.dedeck(x[i],y[i]);
						if (p1) p1->DrawBound(nID, x[i], y[i]);
						if (p2) p2->DrawBound(nID, x[i], y[i]);
					}
				}
			}
			break;
		case TI:
			{
				int v = number-4;
				if (v <= 0 && IsIn(22) && IsIn(34) && IsIn(38) ) { //청단
					game->Incard(22);
					game->Incard(34);
					game->Incard(38);	
					for (int i=0; i<number; i++) {
						if (c[i] == 22 || c[i] == 34 || c[i] == 38) {
							box.dedeck(x[i],y[i]);
							if (p1) p1->DrawBound(nID, x[i], y[i]);
							if (p2) p2->DrawBound(nID, x[i], y[i]);
						}
					}
				}
				if (v <= 0 && IsIn(02) && IsIn(06) && IsIn(10) ) { //홍단
					game->Incard(02);
					game->Incard(06);
					game->Incard(10);	
					for (int i=0; i<number; i++) {
						if (c[i] == 02 || c[i] == 06 || c[i] == 10) {
							box.dedeck(x[i],y[i]);
							if (p1) p1->DrawBound(nID, x[i], y[i]);
							if (p2) p2->DrawBound(nID, x[i], y[i]);
						}
					}

				}
				if (v <= 0 && IsIn(14) && IsIn(18) && IsIn(26) ) { //초단
					game->Incard(14);
					game->Incard(18);
					game->Incard(26);
					for (int i=0; i<number; i++) {
						if (c[i] == 14 || c[i] == 18 || c[i] == 26) {
							box.dedeck(x[i],y[i]);
							if (p1) p1->DrawBound(nID, x[i], y[i]);
							if (p2) p2->DrawBound(nID, x[i], y[i]);
						}
					}
				}

				if (v >= 1) {
					for (int i=0; i<number; i++) {
						game->Incard(c[i]);
						box.dedeck(x[i],y[i]);
						if (p1) p1->DrawBound(nID, x[i], y[i]);
						if (p2) p2->DrawBound(nID, x[i], y[i]);
					}
				}
			}
			break;
		}
		return;
	}

	bool CheckGrid(Box& b, CheckBox& cb, int y, int x, Party& p) {
		if (x < 0 || x > 4 || y < 0 || y > 9) return false;
		if (cb.box[y][x] == true) return false;
		if (b.deck[y][x].num < 1 || b.deck[y][x].num > 50) return false;
		// atr
		//NULL:0 광:1 비광:2 
		//열(일반):3 열(쌍피):4 열(고도리):5 
		//홍단:6 청단:7 구사:8 비구사 : 11
		//쌍피:9 피:10
		switch (b.deck[y][x].atr) {

		case 0:
		case -1:
			// NULL, 얼음
			return false; 
			break;
		case 4:
			// 쌍피가능한 열
			// 일단 쌍피로 넣어보고, 그다음에 동물로 넣어본다
			// 쌍피로 들어가면, 동물로는 안들어간다..

			if (p.Add(y,x,b.deck[y][x].num, PI) ||
				p.Add(y,x,b.deck[y][x].num, YEOL) ) {
			} else {
				return false;
			}
			break;
		default:
			if (false == p.Add(y,x,b.deck[y][x].num)) {
				return false;
			}
		}

		cb.box[y][x] = true;

		CheckGrid(b, cb, y+1, x, p);
		CheckGrid(b, cb, y, x+1, p);
		CheckGrid(b, cb, y-1, x, p);
		CheckGrid(b, cb, y, x-1, p);

		return false;
	}

}