//服务器配置
package game

import (
	"github.com/game_engine/i18n"
	"github.com/game_engine/logs"
	"github.com/game_engine/orm"
	_ "github.com/go-sql-driver/mysql"
)

import (
	"fmt"
	"strconv"
)

//全局性
var O orm.Ormer
var Log *logs.BeeLogger

type SysConfig struct {
	account_log_max       int64
	mysql_address         string
	ServerAddress         string
	Server2AccountAddress string
	ServerNoteAddress     string
	GameId                int32
	DistanceTime          int
}

func (this *SysConfig) Init() {
	this.setLog()
	this.readConfig()
	this.dbConfig()
}

func (this *SysConfig) setLog() {
	Log = logs.NewLogger(this.account_log_max) //日志
	Log.EnableFuncCallDepth(true)
	err := Log.SetLogger("file", `{"filename":"log/game.log"}`)
	if err != nil {
		fmt.Println(err)
	}
}

func (this *SysConfig) readConfig() {
	err := il8n.GetInit("config/game_cfg.ini")
	if err == nil {

		int_gameid, _ := strconv.Atoi(il8n.Data["game_id"].(string))
		this.GameId = int32(int_gameid)
		this.account_log_max, _ = strconv.ParseInt(il8n.Data["account_log_max"].(string), 10, 64)
		this.ServerAddress = il8n.Data["server_address"].(string)
		this.Server2AccountAddress = il8n.Data["server_2_accont_address"].(string)
		this.ServerNoteAddress = il8n.Data["server_note_address"].(string)
		this.DistanceTime, _ = strconv.Atoi(il8n.Data["distance_time"].(string))
		this.mysql_address = il8n.Data["mysql_user"].(string) + ":" + il8n.Data["mysql_pwd"].(string) + "@tcp(" + il8n.Data["mysql_ip"].(string) + ":" + il8n.Data["mysql_port"].(string) + ")/" + il8n.Data["mysql_db"].(string) + "?charset=utf8"
	} else {
		Log.Error(err.Error())
	}
}

func (this *SysConfig) dbConfig() {
	// set default database
	err := orm.RegisterDataBase("default", "mysql", this.mysql_address)
	if err != nil {
		Log.Error(err.Error())
	}

	// register model
	orm.RegisterModel(new(Player_Rank))

	// create table
	orm.RunSyncdb("default", false, true)
	O = orm.NewOrm()
}
