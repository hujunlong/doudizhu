package account

import (
	"github.com/game_engine/cache/redis"
	"github.com/game_engine/i18n"
	_ "github.com/go-sql-driver/mysql"
	"server/share/global"
	"strings"
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

func (this *AccountInfo) Register(name string, pwd string, server_id string) (int, int) {
	this.accountMutex.Lock()
	defer this.accountMutex.Unlock()

	//通过内存数据库 先检查是否username相同
	redis_login_base := new(LoginBase)
	err := redis.Find("PlayerName:"+name, redis_login_base)
	if err != nil { //没有查到数据
		//写内存数据库与mysql
		this.config.count += 1
		user := LoginBase{PlayerId: this.config.count, PlayerName: name, PlayerPwd: pwd, Gold: 0, ServerId: server_id, IsForBid: false}

		//内存数据库
		err_redis := redis.Add("PlayerName:"+name, user)
		//mysql
		_, err_mysql := O.Insert(&user)

		if err_redis == nil && err_mysql == nil {
			return global.REGISTERSUCCESS, this.config.count
		} else {
			redis.Del("PlayerName:" + name)
			O.Delete(&user)
		}
	}

	Log.Trace("name = %s pwd = %s have same SAMENICK", name, pwd)
	return global.SAMENICK, 0
}

func (this *AccountInfo) VerifyLogin(name string, pwd string) (result int, player_id int, game_address string) {
	redis_login_base := new(LoginBase)
	err := redis.Find("PlayerName:"+name, redis_login_base)
	if err == nil {
		if strings.EqualFold(redis_login_base.PlayerName, name) && strings.EqualFold(redis_login_base.PlayerPwd, pwd) && !redis_login_base.IsForBid {
			if v, ok := il8n.Data[redis_login_base.ServerId]; ok {
				return global.LOGINSUCCESS, redis_login_base.PlayerId, v.(string)
			} else {
				Log.Error("user.ServerId not find config for serverList error")
			}
		}
	}
	return global.LOGINERROR, 0, ""
}
