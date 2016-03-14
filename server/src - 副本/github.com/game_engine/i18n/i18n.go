package il8n

import (
	"bufio"
	"io"
	"os"
	"strings"
)

var (
	path string
	Data map[interface{}]interface{}
)

func init() {
	Data = make(map[interface{}]interface{})
	path = "server/global/locale.ini"
}

func add2Map(str string) {
	str = strings.TrimSpace(str)
	if strings.HasPrefix(str, "#") {
		return
	}

	strs := strings.Split(str, "=")
	if len(strs) == 2 {
		key := strings.TrimSpace(strs[0]) //utf8 无 bom
		value := strings.TrimSpace(strs[1])
		Data[key] = value
	}
}

func GetInit(path_ ...string) error {
	if len(path_) == 1 {
		path = path_[0]
	}
	inputFile, inputError := os.Open(path)
	if inputError != nil {
		return inputError
	}
	defer inputFile.Close()

	inputReader := bufio.NewReader(inputFile)
	for {
		inputString, readerError := inputReader.ReadString('\n')
		if (readerError == io.EOF) && (len(inputString) == 0) {
			return nil
		} else {
			add2Map(inputString)
		}
	}
	return nil
}
