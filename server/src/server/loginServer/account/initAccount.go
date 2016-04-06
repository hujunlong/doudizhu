package account

import (
	"fmt"
	"github.com/game_engine/i18n"
	"github.com/game_engine/logs"
	"github.com/game_engine/orm"
	_ "github.com/go-sql-driver/mysql"
	"strconv"
	"strings"
)

//全局性
var O orm.Ormer
var Log *logs.BeeLogger

type Config struct {
	count              int //现在最大的playerid
	account_log_max    int64
	Listen4CAddress    string
	Listen4GameAddress string
	mysql_address      string
	NewServerAddress   map[string]string //such as [1]"127.0.0.1:1234"
}

func (this *Config) Init() {
	this.NewServerAddress = make(map[string]string)
	this.setLog()
	this.readConfig()
	this.openNewServerConfig()
	this.dbConfig()
	this.count = this.getMaxId()
}

func (this *Config) setLog() {
	Log = logs.NewLogger(this.account_log_max) //日志
	Log.EnableFuncCallDepth(true)
	err := Log.SetLogger("file", `{"filename":"log/account.log"}`)
	if err != nil {
		fmt.Println(err)
	}
}

func (this *Config) readConfig() {
	err := il8n.GetInit("config/account_cfg.ini")
	if err == nil {
		this.account_log_max, _ = strconv.ParseInt(il8n.Data["account_log_max"].(string), 10, 64)
		this.Listen4CAddress = il8n.Data["login_listen_4c_ip"].(string)
		this.Listen4GameAddress = il8n.Data["login_listen_4game_ip"].(string)
		this.mysql_address = il8n.Data["mysql_user"].(string) + ":" + il8n.Data["mysql_pwd"].(string) + "@tcp(" + il8n.Data["mysql_ip"].(string) + ":" + il8n.Data["mysql_port"].(string) + ")/" + il8n.Data["mysql_db"].(string) + "?charset=utf8"
	} else {
		Log.Error(err.Error())
	}
}

func (this *Config) openNewServerConfig() {
	for k, v := range il8n.Data {
		if strings.Contains(k.(string), "new_") {
			key := strings.TrimLeft(k.(string), "new_")
			this.NewServerAddress[key] = v.(string)
		}
	}
	if len(this.NewServerAddress) == 0 {
		Log.Error("new player can't connect,config can't find new server id")
	}
}

func (this *Config) dbConfig() {
	err := orm.RegisterDataBase("default", "mysql", this.mysql_address)
	if err != nil {
		Log.Error(err.Error())
	}

	orm.RegisterModel(new(LoginBase), new(ForBid))
	orm.RunSyncdb("default", false, true)
	O = orm.NewOrm()
}

func (this *Config) getMaxId() int {
	var count int = 0
	var maps []orm.Params
	num, err := O.Raw("select max(player_id) from login_base").Values(&maps)
	if err == nil && num > 0 {
		if maps[0]["max(player_id)"] != nil {
			buff := maps[0]["max(player_id)"].(string)
			count, _ = strconv.Atoi(buff)
		}
	}
	return count
}
