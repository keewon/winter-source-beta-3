#include "box.h"
#include <fstream>

Box::Box()
{
	int i,j;

	for (i=0; i<=9; i++) {
		for (j=0; j<=4; j++) {
			deck[i][j].set(0);
		}
	}
}

void Box::gravity() {
			// 패들이 다 떨어지게 하자.
			for (int i=0; i<5; i++) {
				for (int j=1; j<=9; j++) {
					if (deck[j][i].num != 0) {
						for (int k=j-1; k>=0 && deck[k][i].num ==0; k--) {
							deck[k][i].set(deck[k+1][i].num);
							deck[k+1][i].set(0);
						}
					}
				}
			}
}

/*
void Box::fall(int row, int col)
{
	int i;
	//이때 카드의 속성 등이 재정의될 필요가 있으니 유념할것.
	i=deck[row][col].num;
	if(deck[row][col].num)
	{
		deck[row-1][col].set(i);
		deck[row][col].set(0);
		if(deck[row+1][col].num)
			fall(row+1, col);
	}
}
*/

	//int yul[9],y=0;
int Box::dedeck(int row, int col)
{
	int i;
	i=deck[row][col].num;
	//cardset->incard(i); //cardset의 이름에 따라 변경 요함
	deck[row][col].set(0);

	// 얼음패를 여기서 없애기로 했다... -_-
	if (row<=9) {
		if (deck[row+1][col].num == 51) {
			deck[row+1][col].set(0);
		}
	}
	if (row >= 1) {
		if (deck[row-1][col].num == 51) {
			deck[row-1][col].set(0);
		}
	}
	if (col <= 3) {
		if (deck[row][col+1].num == 51) {
			deck[row][col+1].set(0);
		}
	}
	if (col >= 1) {
		if (deck[row][col-1].num == 51) {
			deck[row][col].set(0);
		}
	}

	// 패 떨어뜨리는 것은 play에서 한다..
	/*
	if(deck[row+1][col].num) {
		fall(row+1, col);
	}
	*/
	//check(row-1,col);
	return i;	//없애야할 카드의 번호 return
}

