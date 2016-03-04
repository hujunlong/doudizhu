package account

import (
	"fmt"
	config "github.com/game_engine/i18n"
	"github.com/game_engine/logs"
	"github.com/game_engine/orm"
	_ "github.com/go-sql-driver/mysql"
	"server/share/global"
	"strconv"
)

var o orm.Ormer
var count int //现在最大的playerid
var Log *logs.BeeLogger
var account_log_max int64
var BlanceAddress string
var ListenAddress string
var mysql_address string

func init() {

	setLog()
	readConfig()
	// set default database
	err := orm.RegisterDataBase("default", "mysql", mysql_address)
	if err != nil {
		Log.Error(err.Error())
	}

	// register model
	orm.RegisterModel(new(LoginBase), new(ForBid))

	// create table
	orm.RunSyncdb("default", false, true)

	o = orm.NewOrm()

	count = getMaxId()
}

func readConfig() {
	err := config.GetInit("./account_cfg.ini")
	if err == nil {
		account_log_max, _ = strconv.ParseInt(config.Data["account_log_max"].(string), 10, 64)
		ListenAddress = config.Data["login_listen_ip"].(string) + ":" + config.Data["login_port"].(string)
		BlanceAddress = config.Data["balance_server"].(string) + ":" + config.Data["balance_port"].(string)
		mysql_address = config.Data["mysql_user"].(string) + ":" + config.Data["mysql_pwd"].(string) + "@tcp(" + config.Data["mysql_ip"].(string) + ":" + config.Data["mysql_port"].(string) + ")/" + config.Data["mysql_db"].(string) + "?charset=utf8"
	} else {
		Log.Error(err.Error())
	}
}

func setLog() {
	Log = logs.NewLogger(account_log_max) //日志
	Log.EnableFuncCallDepth(true)
	err := Log.SetLogger("file", `{"filename":"./account.log"}`)
	if err != nil {
		fmt.Println(err)
	}
}

func getMaxId() int {
	var count int = 0
	var maps []orm.Params
	num, err := o.Raw("select max(player_id) from login_base").Values(&maps)
	if err == nil && num > 0 {
		if maps[0]["max(player_id)"] != nil {
			buff := maps[0]["max(player_id)"].(string)
			count, _ = strconv.Atoi(buff)
		}
	}
	Log.Trace("getMaxId = %d", count)
	return count
}

func Register(name string, pwd string) int32 {
	//先检查是否username相同
	user := LoginBase{PlayerName: name}
	err := o.Read(&user, "PlayerName")
	if err != nil { //没有被注册
		count += 1
		user = LoginBase{PlayerId: count, PlayerName: name, PlayerPwd: pwd, Gold: 0, IsForBid: false}
		_, err = o.Insert(&user)
		if err == nil {
			return global.REGISTERSUCCESS
		}
	}
	Log.Trace("name = %s pwd = %s have same SAMENICK", name, pwd)
	return global.SAMENICK
}

func VerifyLogin(name string, pwd string) int32 {
	user := LoginBase{PlayerName: name, PlayerPwd: pwd}
	err := o.Read(&user, "PlayerName", "PlayerPwd")

	if err != nil {
		Log.Trace("name = %s pwd = %s login error", name, pwd)
		return global.LOGINERROR
	}

	for_bid := ForBid{UserId: user.PlayerId}
	err = o.Read(&for_bid, "UserId")
	if err == orm.ErrNoRows {
		return global.LOGINSUCCESS
	}

	Log.Trace("name = %s pwd = %s login forbid", name, pwd)
	return global.FORBIDLOGIN
}
