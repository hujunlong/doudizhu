package main

import (
	"fmt"
	"game_engine/config"
)

func ErrorMsg(err error) {
	if err != nil {
		fmt.Println("error Msg:", err)
	}
}

type goods struct {
}

func main() {
	cnf, err := config.NewConfig("json", "goods.json")

	ErrorMsg(err)
	fmt.Println(cnf)

	rootArray, _ := cnf.DIY("rootArray")
	rootArrayCasted := rootArray.([]interface{})
	if len(rootArrayCasted) <= 0 {
		fmt.Println("error config")
		return
	}

	for i := 0; i < len(rootArrayCasted); i++ {
		elem := rootArrayCasted[i].(map[string]interface{})

		if elem["name"] == float64(1) {
			fmt.Println("come here")
		}

		fmt.Println(elem["id"])
		fmt.Println(elem["name"])
		fmt.Println(elem["discri"])
		fmt.Println(elem["gold"])
	}

}
