//读取策划配置
package game

import (
	"encoding/csv"
	"fmt"
	"os"
)

type CsvConfig struct {
	Create_role map[string][]string
	Role        map[string][]string
	Drop_stage  map[string][][]string
}

func readConfig(path string, key_index int) map[string][]string {

	rfile, _ := os.Open(path)
	r := csv.NewReader(rfile)
	strs, err := r.ReadAll()

	if err != nil {
		fmt.Println("err:", err)
		return nil
	}

	//检测数据是否存在合法
	if len(strs) < 0 || len(strs[0]) < key_index {
		return nil
	}

	buf_map := make(map[string][]string)

	for _, v := range strs {
		key := v[key_index]
		buf_map[key] = v
	}
	return buf_map
}

func (this *CsvConfig) Init() {

	this.Create_role = make(map[string][]string)
	this.Role = make(map[string][]string)

	this.Create_role = readConfig("config/csv/creat_role.csv", 0)
	this.Role = readConfig("config/csv/hero.csv", 0)

}
