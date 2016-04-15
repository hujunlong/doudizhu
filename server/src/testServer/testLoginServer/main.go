/*
package main

import (
	"server/share/protocol"

	"server/share/global"

	"github.com/game_engine/logs"
	"github.com/golang/protobuf/proto"
)

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"net"
	"strconv"
	"time"
)

var log *logs.BeeLogger

const max_client = 1000

var end = make(chan int)

func init() {
	log = logs.NewLogger(100000) //日志
	log.EnableFuncCallDepth(true)
	log.SetLogger("file", `{"filename":"log/testLoginServer.log"}`)
}

func CheckError(err error) bool {
	if err != nil {
		fmt.Println("err:", err)
		return false
	}
	return true
}

func SendPackage(conn net.Conn, pid int, body []byte) {
	var pid_32 int32 = int32(pid)

	len := 8 + len(body)
	var len_32 = int32(len)

	len_buf := bytes.NewBuffer([]byte{})
	binary.Write(len_buf, binary.BigEndian, &len_32)

	pid_buf := bytes.NewBuffer([]byte{})
	binary.Write(pid_buf, binary.BigEndian, &pid_32)

	msg := append(len_buf.Bytes(), pid_buf.Bytes()...)
	msg2 := append(msg, body...)
	conn.Write(msg2)
	fmt.Println(msg2)
}

func GetHead(buf []byte) (int32, int32) {
	var head_len int32 = 0
	var head_pid int32 = 0
	buffer_len := bytes.NewBuffer(buf[0:4])
	buffer_pid := bytes.NewBuffer(buf[4:8])
	binary.Read(buffer_len, binary.BigEndian, &head_len)
	binary.Read(buffer_pid, binary.BigEndian, &head_pid)

	return head_len, head_pid
}
func SendMsgRegister(conn net.Conn, i int) {
	nick := strconv.Itoa(i)
	register := &protocol.Account_RegisterPlayer{
		Playername: proto.String(nick),
		Passworld:  proto.String(nick),
	}

	encObj, err := proto.Marshal(register)
	is_ok := CheckError(err)
	if is_ok {
		SendPackage(conn, 1, encObj)
	}
}

func SenMsgLogin(conn net.Conn, i int) {
	nick := strconv.Itoa(i)
	//登陆相关
	loginInfo := &protocol.Account_LoginInfo{
		Playername: proto.String(nick),
		Passworld:  proto.String(nick),
	}

	encObj, err := proto.Marshal(loginInfo)
	is_ok := CheckError(err)
	if is_ok {
		SendPackage(conn, 2, encObj)
	}
}

func ReciveResult(conn net.Conn, i int, recive_result chan int) {
	const MAXLEN = 1024
	buf := make([]byte, MAXLEN)

	for true {
		n, _ := conn.Read(buf) //接收具体消息
		//接收包的type类型用来区分包之间的区别
		_, head_pid := GetHead(buf)

		switch head_pid {
		case 2:
			result := new(protocol.Account_LoginResult)
			if err := proto.Unmarshal(buf[8:n], result); err == nil {
				switch result.GetResult() {
				case global.LOGINSUCCESS:
					log.Info("login sucessfull and player id=%d gameserver = %s", result.GetPlayerId(), result.GetGameserver())
				default:
					log.Error("login error")
				}

				conn.Close()
				recive_result <- 1
				if i == max_client-1 {
					end <- 1
				}
				return
			} else {
				fmt.Println(err)
			}
		case 1:
			result := new(protocol.Account_RegisterResult)
			if err := proto.Unmarshal(buf[8:n], result); err == nil {
				switch result.GetResult() {
				case global.REGISTERSUCCESS:
					log.Trace("register sccessfull!")
				default:
					log.Error("register error")
				}
				//注册后登陆
				SenMsgLogin(conn, i)
			} else {
				fmt.Println("err:", err)
			}
			break
		}
		time.Sleep(5 * time.Millisecond)
	}
}

func MessageRun(conn net.Conn, i int) {
	//通信先获取返回数据
	result := make(chan int)
	go ReciveResult(conn, i, result)
	SendMsgRegister(conn, i)
	//SenMsgLogin(conn, i)
	<-result
}

type ConnStruct struct {
	conn net.Conn
}

//测试账号服务器
func testAccount() {
	var arrayConnStruct [max_client]ConnStruct
	var err error
	for i := 0; i < max_client; {

		arrayConnStruct[i].conn, err = net.Dial("tcp", "127.0.0.1:8082")
		if err != nil {
			log.Error("connect error %s", err)
			time.Sleep(100 * time.Millisecond)
		} else {
			go MessageRun(arrayConnStruct[i].conn, i)
			time.Sleep(5 * time.Millisecond)
			i++
		}
	}
}

func ReciveResult4Game(conn net.Conn) {
	const MAXLEN = 1024
	buf := make([]byte, MAXLEN)

	for true {
		n, _ := conn.Read(buf) //接收具体消息
		//fmt.Println("recive Result:", buf[0:n])
		//接收包的type类型用来区分包之间的区别
		_, head_pid := GetHead(buf)
		fmt.Println(buf[0:n])

		switch head_pid {
		case 1002:
			result := new(protocol.Game_RoleInfoResult)
			if err := proto.Unmarshal(buf[8:n], result); err == nil {
				fmt.Println("result:", result.GetRoleInfo())
				fmt.Println("level:", result.GetRoleInfo().Attribute.Level)
			}
		}
		time.Sleep(5 * time.Millisecond)
	}
}

func testRegisterGame() {
	conn, err := net.Dial("tcp", "127.0.0.1:8082")
	go ReciveResult4Game(conn)

	var player_id int32 = 2
	var hero_id int32 = 901
	var nick string = "aaa"

	base := new(protocol.Game_BaseRole)
	base.PlayerId = &player_id
	base.Nick = &nick
	base.HeroId = &hero_id

	register := new(protocol.Game_RegisterRole)
	register.Info = base

	encObj, err := proto.Marshal(register)
	is_ok := CheckError(err)
	if is_ok {
		SendPackage(conn, 1001, encObj)
	}
}

func testGetInfoGame() {
	conn, err := net.Dial("tcp", "127.0.0.1:8082")
	go ReciveResult4Game(conn)

	role_info := new(protocol.Game_GetRoleInfo)

	encObj, err := proto.Marshal(role_info)
	fmt.Println(encObj)
	is_ok := CheckError(err)
	if is_ok {
		SendPackage(conn, 1002, encObj)
	}
}

func main() {
	//testAccount()
	//go testRegisterGame()
	go testGetInfoGame()

	time.Sleep(10000 * time.Microsecond)
	<-end
}
*/

package main

import (
	"encoding/csv"
	"fmt"
	"os"
	"strings"
	"time"
)

func main() {

	var pp [][]string

	var config_map map[string][][]string
	config_map = make(map[string][][]string)

	var buff [][]string
	var equial string = ""
	var index int32 = 0

	f, _ := os.OpenFile("config/csv/drop_stage.csv", os.O_RDONLY, os.ModeAppend)
	r := csv.NewReader(f)
	pp, _ = r.ReadAll()

	for _, v := range pp {
		if strings.EqualFold(equial, v[1]) {
			buff[index] = v
			index += 1
		} else {
			if index > 0 {
				config_map[equial] = buff
			}
			equial = v[1]
			index = 0
		}
	}

	fmt.Println(config_map["1001"])
	time.Sleep(100000 * time.Millisecond)
}
