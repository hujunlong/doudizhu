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
		swap(pks[a],pks[b]);
	}

	const int pk_num = 17;
	PkStruct buff;
	for (int i=0;i<51;i++)
	{
		int index = i%pk_num;
		int witch_player = i/pk_num;

		buff.pk_type = pks[index].pk_type;
		buff.pk_num = pks[index].pk_num;

		if (witch_player == 0)
			pks_player.push_back(buff);
		
		if (witch_player == 1)
			pks_npc1.push_back(buff);

		if (witch_player == 2)
			pks_npc2.push_back(buff);
	}
}

bool Rule::SelectShort(const PkStruct& p1,const PkStruct& p2){
	return p1.pk_num > p2.pk_num;
}

int Rule::Plane(vector<PkStruct> pData){
	if ( pData[0].pk_num == RedJoker  && pData[1].pk_num == BlackJoker)
	{
		return 1;
	}
	return 0;
}

int Rule::Bomb(vector<PkStruct> pData){
	int count = pData.size();
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
int Rule::GetCallBigPks(vector<PkStruct> pData){
	int score = 0;
	int count = pData.size();
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
	int plane_num = 0;
	int bomb_num = 0;
	int extra_num = 0;

	int double_one = 1;
	int double_two = 2;
	int double_three = 3;
	if (index == 0){
		sort(pks_player.begin(),pks_player.end(),Rule::SelectShort);
		plane_num = Plane(pks_player);
		bomb_num = Bomb(pks_player); 
		extra_num = GetCallBigPks(pks_player);
	}

	if (index == 1){
		sort(pks_npc1.begin(),pks_npc1.end(),Rule::SelectShort);
		plane_num = Plane(pks_npc1);
		bomb_num = Bomb(pks_npc1); 
		extra_num = GetCallBigPks(pks_npc1);
	}

	if (index == 2){
		sort(pks_npc2.begin(),pks_npc2.end(),Rule::SelectShort);
		plane_num = Plane(pks_npc2);
		bomb_num = Bomb(pks_npc2); 
		extra_num = GetCallBigPks(pks_npc2);
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

void Rule::AiSplitPks(std::vector<PkStruct> &pks){ //0 1 2 玩家 npc1 npc2 
	//排序
	sort(pks.begin(),pks.end(),Rule::SelectShort);
	PkHandleTypeStruct pk_handle_typestruct;
	
	//提取双王
	if (pks[0].pk_type == Joker && pks[1].pk_type == Joker){	
		 pk_handle_typestruct.type = PLANE_PK;
		 pk_handle_typestruct.pk_structs.push_back(pks[0]);
		 pk_handle_typestruct.pk_structs.push_back(pks[1]);
		 vec_pk_hand_type_struct.push_back(pk_handle_typestruct);
	}

	//分析牌型
	 vector<PkStruct>::iterator iter;  
	 vector<PkStruct>::iterator iter_next;  
	 std::vector<PkHandleTypeStruct> vec_pk_hand_type_struct_buff;
	for (iter=pks.begin();iter!=pks.end();iter++) {
		for (iter_next=iter; iter_next != pks.end();iter_next++){
			if (iter->pk_num == iter_next->pk_num){
				++iter;
				pk_handle_typestruct.pk_structs.push_back(*iter);
			}
		}

		if (pk_handle_typestruct.pk_structs.size() == 4){//提取炸弹
			pk_handle_typestruct.type = BOMB_PK;
			vec_pk_hand_type_struct.push_back(pk_handle_typestruct);
			break;
		}
		if (pk_handle_typestruct.pk_structs.size() == 3)
			pk_handle_typestruct.type = THREE_NO_ADD_PK;
		if (pk_handle_typestruct.pk_structs.size() == 2)
			pk_handle_typestruct.type = DOUBLE_PK;
		if (pk_handle_typestruct.pk_structs.size() == 1)
			pk_handle_typestruct.type = SINGLE_PK;

		vec_pk_hand_type_struct_buff.push_back(pk_handle_typestruct);
	}

	//牌分类
	//a) 先确定火箭：判断是否有大小王。
	//b) 再确定炸弹：判明是否有四头。
	//c) 再确定三条：在除了炸弹以外的牌中判断是否包含三条。
	//d) 再确定三顺：在已经确定的三条中判断是否包含相邻的三条，如果有，则将其组成三顺。注意，应该使三顺的数量尽可能大。即如果有444555666，则将其合成一个三顺，而不是分成444555一个三顺和666一个三条。
	//e) 再确定单顺：判断单顺时必须去除四个2以外的所有炸弹。首先判断是否存在除了三条牌（这里的三条是指所有的三条）以外的连牌，如果有，则将其提取出来。其次，将剩余的牌与每一个三条（不包含三顺）进行试组合，如果能够重新组成单顺和对子，则将原有的三条取消，重新组合成连牌和单顺（例子4566678重新组成45678和66）。最后，将已知的连牌、三条（不包含三顺）和剩下的牌张再试组合，将所有如45678（已知的连牌）999（三条）10J（单牌），重新组合成45678910J和99。通过以上的方法，就能将连牌和三条很好地重新组合。
	//f) 再确定双顺：首先，如果两单顺牌完全重合，则将其重新组合成双顺。其次，在除炸弹、三顺、三条、单顺以外的牌中检测是否包含双顺。如果有，将其提取出来。
	//g) 再确定对子：在炸弹、三顺、三条、连牌、双顺以外的牌中检测是否存在对子，如果存在将其提取出来。
	//h) 再确定单牌

	AiThreePks(vec_pk_hand_type_struct_buff);
	

}

//提取三顺 与 三个
void Rule::AiThreePks(std::vector<PkHandleTypeStruct> vec_pk_hand_type_struct_buff){
	vector<PkHandleTypeStruct> result;
	vector<PkHandleTypeStruct>::iterator iter_pk_handle_strct; //提取三顺
	std::vector<int> three_pk_vec;
	

	for (iter_pk_handle_strct = vec_pk_hand_type_struct_buff.begin();iter_pk_handle_strct != vec_pk_hand_type_struct_buff.end();iter_pk_handle_strct++){
		if (iter_pk_handle_strct->type == THREE_NO_ADD_PK){
			three_pk_vec.push_back(iter_pk_handle_strct->pk_structs[0].pk_num);
			result.push_back(*iter_pk_handle_strct);
		}
	}

	PkHandleTypeStruct old_struct = *result.begin();
	PkHandleTypeStruct end_struct = *result.end();  
	for (iter_pk_handle_strct = (result.begin()+1);iter_pk_handle_strct != result.end();iter_pk_handle_strct++)
	{   
		if ( (iter_pk_handle_strct->pk_structs[0].pk_num != Er) && (old_struct.pk_structs[0].pk_num+1) == iter_pk_handle_strct->pk_structs[0].pk_num ){
			old_struct.type = THREE_SHUNZHI_PK;
			old_struct.pk_structs.push_back(iter_pk_handle_strct->pk_structs[0]);
			old_struct.pk_structs.push_back(iter_pk_handle_strct->pk_structs[1]);
			old_struct.pk_structs.push_back(iter_pk_handle_strct->pk_structs[2]);
			 
		}else{
			vec_pk_hand_type_struct.push_back(old_struct);
			if (end_struct.pk_structs[0].pk_num == iter_pk_handle_strct->pk_structs[0].pk_num){
				vec_pk_hand_type_struct.push_back(end_struct);
			}
		}
		old_struct = *iter_pk_handle_strct;
	}
}