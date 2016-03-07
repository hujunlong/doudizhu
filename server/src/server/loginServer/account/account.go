package account

import (
	"fmt"
	"github.com/game_engine/i18n"
	"github.com/game_engine/logs"
	"github.com/game_engine/orm"
	_ "github.com/go-sql-driver/mysql"
	"server/share/global"
	"strconv"
	"strings"
)

var o orm.Ormer
var count int32 //现在最大的playerid
var Log *logs.BeeLogger
var account_log_max int64
var Listen4CAddress string
var Listen4GameAddress string
var mysql_address string
var NewServerAddress map[string]string

func init() {
	setLog()
	readConfig()
	OpenNewServerConfig()
	dbConfig()
	count = getMaxId()
}

func setLog() {
	Log = logs.NewLogger(account_log_max) //日志
	Log.EnableFuncCallDepth(true)
	err := Log.SetLogger("file", `{"filename":"./account.log"}`)
	if err != nil {
		fmt.Println(err)
	}
}

func readConfig() {
	err := il8n.GetInit("./account_cfg.ini")
	if err == nil {
		account_log_max, _ = strconv.ParseInt(il8n.Data["account_log_max"].(string), 10, 64)
		Listen4CAddress = il8n.Data["login_listen_4c_ip"].(string)
		Listen4GameAddress = il8n.Data["login_listen_4game_ip"].(string)
		mysql_address = il8n.Data["mysql_user"].(string) + ":" + il8n.Data["mysql_pwd"].(string) + "@tcp(" + il8n.Data["mysql_ip"].(string) + ":" + il8n.Data["mysql_port"].(string) + ")/" + il8n.Data["mysql_db"].(string) + "?charset=utf8"
	} else {
		Log.Error(err.Error())
	}
}

func OpenNewServerConfig() {
	NewServerAddress = make(map[string]string)

	for k, v := range il8n.Data {
		if strings.Contains(k.(string), "new_server_") {
			NewServerAddress[v.(string)] = v.(string)
		}
	}
}

func dbConfig() {
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
}

func getMaxId() int32 {
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
	return int32(count)
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

func VerifyLogin(name string, pwd string) (result int32, player_id int32) {
	user := LoginBase{PlayerName: name, PlayerPwd: pwd}
	err := o.Read(&user, "PlayerName", "PlayerPwd")

	if err != nil {
		Log.Trace("name = %s pwd = %s login error", name, pwd)
		return global.LOGINERROR, 0
	}

	for_bid := ForBid{UserId: user.PlayerId}
	err = o.Read(&for_bid, "UserId")
	if err == orm.ErrNoRows {
		return global.LOGINSUCCESS, user.PlayerId
	}

	Log.Trace("name = %s pwd = %s login forbid", name, pwd)
	return global.FORBIDLOGIN, 0
}
