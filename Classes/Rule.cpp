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
			pks[i*j+j].pk_type = i;
			pks[i*j+j].pk_num = j;
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

}