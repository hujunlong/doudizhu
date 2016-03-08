package global

import ()

//用于全局

func init() {

}

const (
	REGISTERSUCCESS = 0 //注册成功
	REGISTERERROR   = 1 //注册错误
	LOGINERROR      = 2 //登陆错误
	PASSWDERROR     = 3 //密码错误
	SAMENICK        = 4 //注册名字相同
	LOGINSUCCESS    = 5 //登陆成功
	FORBIDLOGIN     = 6 //禁止登陆
)

const (
	GetTypeId        = 0
	LoginInfoId      = 1
	RegisterInfoId   = 2
	LoginResultId    = 3
	RegisterResultId = 4
	GetGameAccountId = 5
	GameResultId     = 6
)
