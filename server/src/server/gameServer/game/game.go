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

var o orm.Ormer
var Log *logs.BeeLogger
var account_log_max int64
var mysql_address string
var ServerAddress string
var Server2AccountAddress string
var ServerNoteAddress string
var DistanceTime int

func init() {
	setLog()
	readConfig()
	dbConfig()

}

func setLog() {
	Log = logs.NewLogger(account_log_max) //日志
	Log.EnableFuncCallDepth(true)
	err := Log.SetLogger("file", `{"filename":"log/game.log"}`)
	if err != nil {
		fmt.Println(err)
	}
}

func readConfig() {
	err := il8n.GetInit("config/game_cfg.ini")
	if err == nil {
		account_log_max, _ = strconv.ParseInt(il8n.Data["account_log_max"].(string), 10, 64)
		ServerAddress = il8n.Data["server_address"].(string)
		Server2AccountAddress = il8n.Data["server_2_accont_address"].(string)
		ServerNoteAddress = il8n.Data["server_note_address"].(string)
		DistanceTime, _ = strconv.Atoi(il8n.Data["distance_time"].(string))
		mysql_address = il8n.Data["mysql_user"].(string) + ":" + il8n.Data["mysql_pwd"].(string) + "@tcp(" + il8n.Data["mysql_ip"].(string) + ":" + il8n.Data["mysql_port"].(string) + ")/" + il8n.Data["mysql_db"].(string) + "?charset=utf8"
	} else {
		Log.Error(err.Error())
	}
}

func dbConfig() {
	// set default database
	err := orm.RegisterDataBase("default", "mysql", mysql_address)
	if err != nil {
		Log.Error(err.Error())
	}

	// register model
	orm.RegisterModel(new(RoleInfo))

	// create table
	orm.RunSyncdb("default", false, true)
	o = orm.NewOrm()
}
