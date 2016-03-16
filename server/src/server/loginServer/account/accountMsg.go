package account

import (
	"github.com/game_engine/i18n"
	"github.com/game_engine/orm"
	_ "github.com/go-sql-driver/mysql"
	"server/share/global"
	"sync"
)

type AccountInfo struct {
	accountMutex *sync.RWMutex
	config       *Config
}

func (this *AccountInfo) Init(config *Config) {
	this.accountMutex = new(sync.RWMutex)
	this.config = config
}

func (this *AccountInfo) Register(name string, pwd string, server_id string) (int32, int32) {
	this.accountMutex.Lock()
	defer this.accountMutex.Unlock()

	//先检查是否username相同
	user := LoginBase{PlayerName: name}
	err := O.Read(&user, "PlayerName")
	if err != nil { //没有被注册
		this.config.count += 1
		user = LoginBase{PlayerId: this.config.count, PlayerName: name, PlayerPwd: pwd, Gold: 0, ServerId: server_id, IsForBid: false}
		_, err = O.Insert(&user)
		if err == nil {
			return global.REGISTERSUCCESS, this.config.count
		}
	}
	Log.Trace("name = %s pwd = %s have same SAMENICK", name, pwd)
	return global.SAMENICK, 0
}

func (this *AccountInfo) VerifyLogin(name string, pwd string) (result int32, player_id int32, game_address string) {
	this.accountMutex.Lock()
	defer this.accountMutex.Unlock()

	user := LoginBase{PlayerName: name, PlayerPwd: pwd}
	err := O.Read(&user, "PlayerName", "PlayerPwd")

	if err != nil {
		Log.Trace("name = %s pwd = %s login error = %s", name, pwd, err)
		return global.LOGINERROR, 0, ""
	}

	for_bid := ForBid{UserId: user.PlayerId}
	err = O.Read(&for_bid, "UserId")
	if err == orm.ErrNoRows {

		if v, ok := il8n.Data[user.ServerId]; ok {
			return global.LOGINSUCCESS, user.PlayerId, v.(string)
		} else {
			Log.Error("user.ServerId not find config for serverList error")
		}
	}

	return global.FORBIDLOGIN, 0, ""
}
