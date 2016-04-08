package global

import ()

//用于全局

func init() {

}

//账号服务器消息
const (
	REGISTERSUCCESS = 0 //注册成功
	REGISTERERROR   = 1 //注册错误
	LOGINERROR      = 2 //登陆错误
	PASSWDERROR     = 3 //密码错误
	SAMENICK        = 4 //注册名字相同
	LOGINSUCCESS    = 5 //登陆成功
	FORBIDLOGIN     = 6 //禁止登陆
)

//游戏服务器
const (
	REGISTERROLESUCCESS = 100 //注册角色成功
	REGISTERROLEERROR   = 101 //注册角色失败

)
