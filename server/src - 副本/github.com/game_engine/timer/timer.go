package timer

import (
	"errors"
	"fmt"
	"strconv"
	"strings"
	"time"
)

func findInputIsError(input [6]int) error {
	for data := range input {
		if data < 0 {
			return errors.New("input have less 0")
		}
	}

	if input[1] > 12 { //月
		return errors.New("input month error more than 12")
	}

	//日
	leap_year := false
	if (input[0]%4 == 0 && input[0]%100 != 0) || (input[0]%400 == 0) {
		leap_year = true
	}

	switch input[1] {
	case 2:
		if leap_year {
			if input[2] > 28 {
				return errors.New("February, more than 28")
			}
		} else {
			if input[2] > 27 {
				return errors.New("February, more than 27")
			}
		}
	case 1, 3, 5, 7, 8, 10, 12:
		if input[2] > 31 {
			return errors.New("more than 31")
		} else {
			if input[2] > 30 {
				return errors.New("more than 30")
			}
		}
	}

	//时分秒
	if input[3] > 23 {
		return errors.New("hours more than 24")
	}

	if input[4] > 60 {
		return errors.New("minutes more than 60")
	}

	if input[5] > 60 {
		return errors.New("seconds more than 60")
	}

	return nil
}

func str2timeSecond(str string) (int, error) {
	//处理字符串
	now := time.Now().Unix()
	time_split := strings.Split(str, " ") //格式 2015-01-12 16:21:60
	date_ := strings.Split(time_split[0], "-")
	time_ := strings.Split(time_split[1], ":")
	//年月日
	if (len(date_) != 3) || (len(time_) != 3) {
		return 0, errors.New("input datetime string errors")
	}

	var time_int [6]int
	for i := 0; i < len(date_); i++ {
		time_int[i], _ = strconv.Atoi(date_[i])
	}

	for j := 0; j < len(time_); j++ {
		time_int[j+3], _ = strconv.Atoi(time_[j])
	}

	//基础查错
	err := findInputIsError(time_int)
	if err != nil {
		return 0, err
	}

	timestamp := time.Date(time_int[0], time.Month(time_int[1]), time_int[2], time_int[3], time_int[4], time_int[5], 0, time.Local).Unix()
	if timestamp < now {
		return 0, errors.New("input datatime less than now")
	}

	fmt.Println(timestamp, now)
	return (int)(timestamp - now), nil
}

func dofunc(i interface{}, f func()) (int, error) {
	var second_ = 0
	switch i.(type) {
	case int:
		second_ = i.(int)
	case int32:
		second_ = i.(int)
	case int64:
		second_ = i.(int)
	case string:
		var err error
		second_, err = str2timeSecond(i.(string))
		if err != nil {
			return 0, err
		}
	default:
	}

	return second_, nil
}

func CreateTimer(i interface{}, isCycle bool, f func()) { //循环时间点 是否为循环 调用函数
	if isCycle {
		go func() { //定时循环开线程
			for {
				second, err := dofunc(i, f)
				if err != nil {
					fmt.Println(err)
					return
				}
				time.AfterFunc(time.Duration(time.Duration(second)*time.Second), f) //默认是秒
				time.Sleep(time.Duration(second) * time.Second)
			}
		}()
	} else {
		second, err := dofunc(i, f)
		if err != nil {
			fmt.Println(err)
			return
		}
		time.AfterFunc(time.Duration(time.Duration(second)*time.Second), f) //默认是秒
	}
}
