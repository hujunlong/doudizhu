#include "Rule.h"
#include <stdlib.h>
#include <ctime>

Rule::Rule(){

}

Rule::~Rule(){

}
void Rule::InitPks(){
	for (int i=0;i<4;++i){
		for (int j=0;j<13;++j){
			pks[i*13+j].pk_type = i;
			pks[i*13+j].pk_num = j;
		}
	}

	pks[52].pk_type = Joker;
	pks[52].pk_num = BlackJoker;

	pks[53].pk_type = Joker;
	pks[53].pk_num = RedJoker;
}

void Rule::Shuffle(){
	srand(unsigned(time(0)));
	for (int i=0;i<54;i++){
		int a = rand()%54;
		int b = rand()%54;
		
		PkStruct buff;
		buff.pk_type = pks[a].pk_type;
		buff.pk_num = pks[a].pk_num;

		pks[a].pk_type = pks[b].pk_type;
		pks[a].pk_num = pks[b].pk_num;

		pks[b].pk_type = buff.pk_type;
		pks[b].pk_num = buff.pk_num;
	}

	const int pk_num = 17;
	for (int i=0;i<51;i++)
	{
		int index = i%pk_num;
		int witch_player = i/pk_num;

		if (witch_player == 0){
			pks_player[index].pk_type=pks[index].pk_type;
			pks_player[index].pk_num=pks[index].pk_num;
		}
		if (witch_player == 1)
		{
			pks_npc1[index].pk_type=pks[index + witch_player*pk_num].pk_type;
			pks_npc1[index].pk_num=pks[index + witch_player*pk_num].pk_num;
		}

		if (witch_player == 2)
		{
			pks_npc2[index].pk_type=pks[index + witch_player*pk_num].pk_type;
			pks_npc2[index].pk_num=pks[index + witch_player*pk_num].pk_num;
		}
	}
}


void Rule::SelectShort(PkStruct* pData,int count){
	PkStruct itemp;
	for (int i=1;i<count-1;i++){
		for (int j= i+1;j<count;j++){
			if (pData[j].pk_num>pData[i].pk_num){
				itemp.pk_num = pData[i].pk_num;
				itemp.pk_type = pData[i].pk_type;

				pData[i].pk_num = pData[j].pk_num;
				pData[i].pk_type = pData[j].pk_type;

				pData[j].pk_type = itemp.pk_type;
				pData[j].pk_num = itemp.pk_num;
			}
		}
	}
}

int Rule::Plane(PkStruct* pData,int count){
	if ( pData[0].pk_num == RedJoker  && pData[1].pk_num == BlackJoker)
	{
		return 1;
	}
	return 0;
}

int Rule::Bomb(PkStruct* pData,int count){
	int bomb_num = 0;
	int pk_num1 = 0;//牌的个数
	int pk_num2 = 0;//牌的分数
	for (int i = 0; i < count; i++){
		int buff = pData[i].pk_num;
		if (buff != pk_num2){
			pk_num2 = buff;
		}else{
			pk_num1+=1;
			if (pk_num1 == 4)
			{
				bomb_num+=1;
			}
		}
	}
	return bomb_num;
}

/*
大王4分，小王3分，一个2为2分
*/
int Rule::GetCallBigPks(PkStruct* pData,int count){
	int score = 0;
	for (int i=0;i<count;i++){
		if (pData[i].pk_num == RedJoker){
			score += 4;
		}
		if (pData[i].pk_num == BlackJoker){
			score += 3;
		}
		if (pData[i].pk_num == Er){
			score += 2;
		}
	}
	return score;
}

/*
假定火箭为8分，炸弹为6分，大王4分，小王3分，一个2为2分，则当分数
大于等于7分时叫三倍；
大于等于5分时叫二倍；
大于等于3分时叫一倍；
小于三分不叫。
*/
int Rule::Call(int index){ //叫地主规则 index 0,1,2 玩家 npc1,npc2
	int pks_num = 17;
	int plane_num = 0;
	int bomb_num = 0;
	int extra_num = 0;

	int double_one = 1;
	int double_two = 2;
	int double_three = 3;
	if (index == 0){
		SelectShort(pks_player,pks_num);
		plane_num = Plane(pks_player,pks_num);
		bomb_num = Bomb(pks_player,pks_num); 
		extra_num = GetCallBigPks(pks_player,pks_num);
	}

	if (index == 1){
		SelectShort(pks_npc1,pks_num);
		plane_num = Plane(pks_npc1,pks_num);
		bomb_num = Bomb(pks_npc1,pks_num); 
		extra_num = GetCallBigPks(pks_npc1,pks_num);
	}

	if (index == 2){
		SelectShort(pks_npc2,pks_num);
		plane_num = Plane(pks_npc2,pks_num);
		bomb_num = Bomb(pks_npc2,pks_num); 
		extra_num = GetCallBigPks(pks_npc2,pks_num);
	}

	int total_score = plane_num*7 + bomb_num*6 + extra_num;
	if (total_score >= 17)
		return double_three;
	else if (total_score >= 15)
		return double_two;
	else if (total_score >= 13)
		return double_one;
	else
	return 0;
}