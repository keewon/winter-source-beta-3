// 겨울프로젝트  통신프로토콜

//* 형식

//Client가 접속을 시도할 때에 (Accept)
//Server에서는 기본적으로 CMD_NAME 하면서 자기 이름을 보낸다
//자기 이름의 길이는 MAX_CHAT_LENGTH 로 한다
//Server는 상대편이 마음에 안들면 CMD_ERROR를 보낸다
//Client는 Accept 되면 CMD_NAME 을 보내고 자기 이름을 보낸다

//Server는 하나의 Client만 accept 한다. 
//하나를 accept한 후에는 listen, accept 하지 않는다.
//언제든지 Kick 버튼을 누르면 Server는 Client를 쫓아낼 수 있다.
//이 때에 CMD_ERROR를 상대편에게 보내준다.

//게임을 시작하기 전에 서로 메시지를 주고 받을 때에는
//CMD_CHAT 다음에 MAX_CHAT_LENGTH 개의 unsigned char를 보낸다.

//Server는 게임을 시작하고 싶을 때에 CMD_GO를 보낸다
//그러고서는 게임화면으로 들어간다.
//옵션은 일단 없는 것으로 한다..

//Client는 게임화면이 완전히 초기화되면 
//CMD_GO를 보낸다
//Server는 저 메시지를 받으면 게임을 시작한다
//그러고서 Server는 Game을 시작한다
//게임이 시작하면 서로 메시지를 주고 받을 수 없다
//게임에 사용되는 키만 사용가능하기 때문이다
//Server 는 CMD_GAME_SERVER + GameServerPacket를 보내고
//Client 는 CMD_GAME_CLIENT + GameClientPacket를 보낸다
//SoundEffect는 CMD_SOUND + char

//이중 언제라도 CMD_QUIT 가 보내면 게임은 중단된다

    //enum WaveName {SE_BACK = 0, SE_CLEAR = 1, SE_ATTACK = 2, 
    //SE_ATTACKED = 3, SE_GAMEOVER =4, SE_ROTATE = 5, SE_CURSOR = 6
    //SE_DOWN = 7};
    
#ifndef _PROTOCOL_OEDALPHA_H_
#define _PROTOCOL_OEDALPHA_H_
typedef char CMD;
namespace np_CMD {
	const CMD CMD_ERROR = 100;
	const CMD CMD_NAME = 101;
	const CMD CMD_CHAT = 102;
	const CMD CMD_GO = 103;
	const CMD CMD_SOUND = 106;
	const CMD CMD_GAME_SERVER = 107;
	const CMD CMD_GAME_CLIENT = 108;
	const CMD CMD_QUIT = 109;
	const CMD CMD_NIL = 110;

	// 이 아래쪽이 좀 난잡해졌는데 
	// 잘 정리해주길 바란다
	// 그리고 네트웍으로 보낼 때에 
	// 같은 OS에서는 int 같은 것도 보낼 수는 있지만
	// 다른 OS에서는 int 의 byte순서나 크기가 다를 수도 있으므로
	// int값은 웬만하면 쓰지 않는 것이..
	// 우리 게임이 윈도에서만 돌아가긴 하지만.. 그냥 꺼림찍해서.
	
	// 아래 두개는 명령어 & 4byte
	const CMD CMD_0_SCORE = 111;
	const CMD CMD_1_SCORE = 112;

	// 아래 두개는 명령어 & 2 byte
	const CMD CMD_0_NEXT = 113;
	const CMD CMD_1_NEXT = 114;

	// 이 아래로는 무조건 명령어 & 1byte
	const CMD CMD_0_ATTACK = 122;
	const CMD CMD_1_ATTACK = 123;

	// 이거 다음에 서버 패킷
	const CMD CMD_0_BOUND = 117;
	const CMD CMD_1_BOUND = 118;

	const CMD CMD_WIN = 120;
	const CMD CMD_LOSE = 121;

	// 이거 다음에 2byte
	const CMD CMD_STATUS = 119;
}

namespace Protocol {

    // 채팅메시지를 39 char로 제한하였음.
    // 산출 근거는 없음..
    // 39 직접 수를 쓰지 말고 꼭 MAX_CHAT_LENGTH를 이용할 것
    // 그래야 쉽게 바꿀 수도 있으니
const int MAX_CHAT_LENGTH = 39;

/*
struct GameOptionPacket {
	// 아직 미정..
	bool pibak;				// 예를 들어서 써놓은 것
};
*/

struct GameServerPacket {
    	unsigned char player;		// 0은 ServerPlayer 1은 ClientPlayer
	unsigned char deck[10][5];	// card의 number (0에서 51까지였던가?)
	/*
	unsigned char money10;		// 각 Player의 돈
	unsigned char money1;		//
	unsigned char score10;		// 각 Player의 점수
	unsigned char score1;		//
	unsigned char next;		// 각 Player의 Next
	char attackStatus;		// attackStatus (절대적 기준)
	*/
};

struct GameClientPacket {
	char key;				
	// 사용자의 key 입력
};


} // namespace Protocol
#endif
